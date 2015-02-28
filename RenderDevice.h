#pragma once

#include <map>
#include <d3d9.h>
#include <d3dx9.h>
#include "Material.h"
#include "Texture.h"

#define CHECKD3D(s) { HRESULT hr = (s); if (FAILED(hr)) ReportError(hr, __FILE__, __LINE__); }

void ReportError(HRESULT hr, const char *file, int line);

typedef IDirect3DTexture9 D3DTexture;
typedef D3DVIEWPORT9 ViewPort;
typedef IDirect3DSurface9 RenderTarget;
typedef D3DVERTEXELEMENT9 VertexElement;
typedef IDirect3DVertexDeclaration9 VertexDeclaration;

struct VideoMode
{
   int width;
   int height;
   int depth;
   int refreshrate;
};

void EnumerateDisplayModes(const int adapter, std::vector<VideoMode>& modes);


enum TransformStateType {
    TRANSFORMSTATE_WORLD      = D3DTS_WORLD,
    TRANSFORMSTATE_VIEW       = D3DTS_VIEW,
    TRANSFORMSTATE_PROJECTION = D3DTS_PROJECTION
};

enum UsageFlags {
    USAGE_DYNAMIC   = D3DUSAGE_DYNAMIC      // to be used for vertex/index buffers which are locked every frame
};

class RenderDevice;

class TextureManager
{
public:
    TextureManager(RenderDevice& rd) : m_rd(rd)
    { }

    ~TextureManager()
    {
        UnloadAll();
    }

    D3DTexture* LoadTexture(BaseTexture* memtex);
    void SetDirty(BaseTexture* memtex);
    void UnloadTexture(BaseTexture* memtex);
    void UnloadAll();

private:
    struct TexInfo
    {
        D3DTexture* d3dtex;
        int texWidth;
        int texHeight;
        bool dirty;
    };

    RenderDevice& m_rd;
    std::map<BaseTexture*, TexInfo> m_map;
    typedef std::map<BaseTexture*, TexInfo>::iterator Iter;
};

class VertexBuffer : public IDirect3DVertexBuffer9
{
public:
    enum LockFlags
    {
        WRITEONLY = 0,                        // in DX9, this is specified during VB creation
        NOOVERWRITE = D3DLOCK_NOOVERWRITE,    // meaning: no recently drawn vertices are overwritten. only works with dynamic VBs.
                                              // it's only needed for VBs which are locked several times per frame
        DISCARDCONTENTS = D3DLOCK_DISCARD     // discard previous contents; only works with dynamic VBs
    };

	void lock( unsigned int offsetToLock, unsigned int sizeToLock, void **dataBuffer, DWORD flags )
    {
       CHECKD3D(this->Lock(offsetToLock, sizeToLock, dataBuffer, flags));
   }
    
	void unlock(void)
    {
        CHECKD3D(this->Unlock());
    }
    
	void release(void)
    {
        SAFE_RELEASE_NO_SET(this);
    }
private:
    VertexBuffer();     // disable default constructor
};


class IndexBuffer : public IDirect3DIndexBuffer9
{
public:
    enum Format {
        FMT_INDEX16 = D3DFMT_INDEX16,
        FMT_INDEX32 = D3DFMT_INDEX32
    };
    enum LockFlags
    {
        WRITEONLY = 0,                      // in DX9, this is specified during VB creation
        NOOVERWRITE = D3DLOCK_NOOVERWRITE,  // meaning: no recently drawn vertices are overwritten. only works with dynamic VBs.
                                            // it's only needed for VBs which are locked several times per frame
        DISCARD = D3DLOCK_DISCARD           // discard previous contents; only works with dynamic VBs
    };

	void lock( unsigned int offsetToLock, unsigned int sizeToLock, void **dataBuffer, DWORD flags )
    {
        CHECKD3D(this->Lock(offsetToLock, sizeToLock, dataBuffer, flags) );
    }
    
	void unlock(void)
    {
        CHECKD3D(this->Unlock());
    }
    
	void release(void)
    {
        SAFE_RELEASE_NO_SET(this);
    }

private:
    IndexBuffer();      // disable default constructor
};

class Shader;

class RenderDevice
{
public:
   enum RenderStates
   {
      ALPHABLENDENABLE   = D3DRS_ALPHABLENDENABLE,
      ALPHATESTENABLE    = D3DRS_ALPHATESTENABLE,
      ALPHAREF           = D3DRS_ALPHAREF,
      ALPHAFUNC          = D3DRS_ALPHAFUNC,
	  BLENDOP            = D3DRS_BLENDOP,
      CLIPPING           = D3DRS_CLIPPING,
      CLIPPLANEENABLE    = D3DRS_CLIPPLANEENABLE,
      CULLMODE           = D3DRS_CULLMODE,
      DESTBLEND          = D3DRS_DESTBLEND,
      LIGHTING           = D3DRS_LIGHTING,
      SRCBLEND           = D3DRS_SRCBLEND,
      ZENABLE            = D3DRS_ZENABLE,
      ZFUNC              = D3DRS_ZFUNC,
      ZWRITEENABLE       = D3DRS_ZWRITEENABLE,
      TEXTUREFACTOR      = D3DRS_TEXTUREFACTOR,
      DEPTHBIAS          = D3DRS_DEPTHBIAS,
      COLORWRITEENABLE   = D3DRS_COLORWRITEENABLE
   };

   enum TextureAddressMode {
       TEX_WRAP          = D3DTADDRESS_WRAP,
       TEX_CLAMP         = D3DTADDRESS_CLAMP,
       TEX_MIRROR        = D3DTADDRESS_MIRROR
   };

   RenderDevice(const HWND hwnd, int width, int height, const bool fullscreen, const int colordepth, int &refreshrate, int VSync, const bool useAA, const bool stereo3D, const bool FXAA);
   ~RenderDevice();

   void BeginScene();
   void EndScene();

   void Clear(const DWORD numRects, const D3DRECT* rects, const DWORD flags, const D3DCOLOR color, const D3DVALUE z, const DWORD stencil);
   void Flip(const bool vsync);

   D3DTexture* GetBackBufferTexture() const { return m_pOffscreenBackBufferTexture; }
   D3DTexture* GetBackBufferTmpTexture() const { return m_pOffscreenBackBufferTmpTexture; }
   RenderTarget* GetOutputBackBuffer() const { return m_pBackBuffer; }

   D3DTexture* GetBloomBufferTexture() const { return m_pBloomBufferTexture; }
   D3DTexture* GetBloomTmpBufferTexture() const { return m_pBloomTmpBufferTexture; }

   RenderTarget* DuplicateRenderTarget(RenderTarget* src);
   D3DTexture* DuplicateTexture(RenderTarget* src);
   D3DTexture* DuplicateTextureSingleChannel(RenderTarget* src);
   D3DTexture* DuplicateDepthTexture(RenderTarget* src);

   void SetRenderTarget( RenderTarget* );
   void SetZBuffer( RenderTarget* );

   RenderTarget* AttachZBufferTo(RenderTarget* surf);
   void CopySurface(RenderTarget* dest, RenderTarget* src);
   void CopySurface(D3DTexture* dest, RenderTarget* src);
   void CopySurface(RenderTarget* dest, D3DTexture* src);
   void CopySurface(D3DTexture* dest, D3DTexture* src);
   void CopyDepth(D3DTexture* dest, RenderTarget* src);

   D3DTexture* CreateSystemTexture(BaseTexture* surf);
   D3DTexture* UploadTexture(BaseTexture* surf, int *pTexWidth=NULL, int *pTexHeight=NULL);
   void UpdateTexture(D3DTexture* tex, BaseTexture* surf);

   void SetRenderState( const RenderStates p1, DWORD p2 );
   void SetTextureFilter(const DWORD texUnit, DWORD mode);
   void SetTextureAddressMode(const DWORD texUnit, const TextureAddressMode mode);
   void SetTextureStageState(const DWORD stage, const D3DTEXTURESTAGESTATETYPE type, const DWORD value);
   void SetSamplerState(const DWORD Sampler, const D3DSAMPLERSTATETYPE Type, const DWORD Value);

   void CreateVertexBuffer(const unsigned int numVerts, const DWORD usage, const DWORD fvf, VertexBuffer **vBuffer );
   void CreateIndexBuffer(const unsigned int numIndices, const DWORD usage, const IndexBuffer::Format format, IndexBuffer **idxBuffer);

   IndexBuffer* CreateAndFillIndexBuffer(const unsigned int numIndices, const unsigned int * indices);
   IndexBuffer* CreateAndFillIndexBuffer(const unsigned int numIndices, const WORD * indices);
   IndexBuffer* CreateAndFillIndexBuffer(const std::vector<unsigned int>& indices);
   IndexBuffer* CreateAndFillIndexBuffer(const std::vector<WORD>& indices);

   void DrawPrimitive(const D3DPRIMITIVETYPE type, const DWORD fvf, const void* vertices, const DWORD vertexCount);
   void DrawIndexedPrimitive(const D3DPRIMITIVETYPE type, const DWORD fvf, const void* vertices, const DWORD vertexCount, const WORD* indices, const DWORD indexCount);
   void DrawPrimitiveVB(const D3DPRIMITIVETYPE type, const DWORD fvf, VertexBuffer* vb, const DWORD startVertex, const DWORD vertexCount);
   void DrawIndexedPrimitiveVB(const D3DPRIMITIVETYPE type, const DWORD fvf, VertexBuffer* vb, const DWORD startVertex, const DWORD vertexCount, IndexBuffer* ib, const DWORD startIndex, const DWORD indexCount);

   void SetViewport( const ViewPort* );
   void GetViewport( ViewPort* );

   void SetTransform(const TransformStateType, const D3DMATRIX* );
   void GetTransform(const TransformStateType, D3DMATRIX* );

   void ForceAnisotropicFiltering( const bool enable ) { m_force_aniso = enable; }
   void CompressTextures( const bool enable ) { m_compress_textures = enable; }

   // performance counters
   unsigned int Perf_GetNumDrawCalls() const      { return m_frameDrawCalls; }
   unsigned int Perf_GetNumStateChanges() const   { return m_frameStateChanges; }
   unsigned int Perf_GetNumTextureChanges() const { return m_frameTextureChanges; }

   void FreeShader();

   inline void CreateVertexDeclaration( const VertexElement * const element, VertexDeclaration ** declaration )
   {
       CHECKD3D(m_pD3DDevice->CreateVertexDeclaration( element, declaration ));
   }

   inline void SetVertexDeclaration(VertexDeclaration * declaration)
   {
      if ( declaration!=currentDeclaration)
      {
         CHECKD3D(m_pD3DDevice->SetVertexDeclaration(declaration));
         currentDeclaration = declaration;

		 m_curStateChanges++;
      }
   }

   inline IDirect3DDevice9* GetCoreDevice() const
   {
       return m_pD3DDevice;
   }

   Material materialStateCache; // for caching

private:
#ifdef USE_D3D9EX
   IDirect3D9Ex* m_pD3D;

   IDirect3DDevice9Ex* m_pD3DDevice;
#else
   IDirect3D9* m_pD3D;

   IDirect3DDevice9* m_pD3DDevice;
#endif

   IDirect3DSurface9* m_pBackBuffer;

   D3DTexture* m_pOffscreenBackBufferTexture;
   D3DTexture* m_pOffscreenBackBufferTmpTexture;

   D3DTexture* m_pBloomBufferTexture;
   D3DTexture* m_pBloomTmpBufferTexture;

   UINT m_adapter;      // index of the display adapter to use

   static const DWORD RENDER_STATE_CACHE_SIZE=256;
   static const DWORD TEXTURE_STATE_CACHE_SIZE=256;
   static const DWORD TEXTURE_SAMPLER_CACHE_SIZE=14;

   DWORD renderStateCache[RENDER_STATE_CACHE_SIZE];          // for caching
   DWORD textureStateCache[8][TEXTURE_STATE_CACHE_SIZE];     // dto.
   DWORD textureSamplerCache[8][TEXTURE_SAMPLER_CACHE_SIZE]; // dto.

   VertexBuffer* m_curVertexBuffer;       // for caching
   IndexBuffer* m_curIndexBuffer;         // dto.
   VertexDeclaration *currentDeclaration; // dto.

   DWORD m_maxaniso;
   bool m_mag_aniso;

   bool m_autogen_mipmap;
   //bool m_RESZ_support;
   bool m_force_aniso;
   bool m_compress_textures;

public:
   // performance counters
   unsigned m_curDrawCalls, m_frameDrawCalls;
   unsigned m_curStateChanges, m_frameStateChanges;
   unsigned m_curTextureChanges, m_frameTextureChanges;

   Shader *basicShader;
   Shader *DMDShader;
   Shader *FBShader;
   Shader *flasherShader;

   Shader* m_curShader; // for caching

   TextureManager m_texMan;

   static VertexDeclaration* m_pVertexTexelDeclaration;
   static VertexDeclaration* m_pVertexNormalTexelDeclaration;
   //static VertexDeclaration* m_pVertexNormalTexelTexelDeclaration;
   static VertexDeclaration* m_pVertexTrafoTexelDeclaration;
};

class Shader 
{
public:
    Shader(RenderDevice *renderDevice);
    ~Shader();

    bool Load( const BYTE* shaderCodeName, UINT codeSize );
    void Unload();

    ID3DXEffect *Core() const
    {
        return m_shader;
    }

    void Begin( const unsigned int pass );
    void End();

    void SetAlphaTestValue(const float value);
    void SetAlphaValue(const float value);
    void SetTexture(const D3DXHANDLE texelName, Texture *texel);
    void SetTexture(const D3DXHANDLE texelName, D3DTexture *texel);
    void SetStaticColor(const D3DXVECTOR4& color);
    void SetMaterial( const Material * const mat );
    void SetTechnique(const char * const technique);

private:
    ID3DXEffect* m_shader;
    RenderDevice *m_renderDevice;
    
    // caches:

    static const DWORD TEXTURESET_STATE_CACHE_SIZE=5; // current convention: SetTexture gets "TextureX", where X 0..4
    BaseTexture *currentTexture[TEXTURESET_STATE_CACHE_SIZE];
    float   currentAlphaTestValue;
    float   currentAlphaValue;
    D3DXVECTOR4 currentColor;
    char    currentTechnique[64];
};
