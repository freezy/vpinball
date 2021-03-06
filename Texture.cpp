#include "stdafx.h"
#include "Texture.h"
#include "freeimage.h"

BaseTexture* BaseTexture::CreateFromFreeImage(FIBITMAP* dib)
{
   // check if Textures exceed the maximum texture dimension
   int maxTexDim;
   HRESULT hrMaxTex = GetRegInt("Player", "MaxTexDimension", &maxTexDim);
   if (hrMaxTex != S_OK)
      maxTexDim = 0; // default: Don't resize textures
   if (maxTexDim <= 0)
      maxTexDim = 65536;

   const int pictureWidth = FreeImage_GetWidth(dib);
   const int pictureHeight = FreeImage_GetHeight(dib);
   FIBITMAP* dibResized = dib;

   if ((pictureHeight > maxTexDim) || (pictureWidth > maxTexDim))
   {
      int newWidth = max(min(pictureWidth, maxTexDim), MIN_TEXTURE_SIZE);
      int newHeight = max(min(pictureHeight, maxTexDim), MIN_TEXTURE_SIZE);
      /*
       * The following code tries to maintain the aspect ratio while resizing.
       */
      if (pictureWidth - newWidth > pictureHeight - newHeight)
          newHeight = min(pictureHeight * newWidth / pictureWidth, maxTexDim);
      else
          newWidth = min(pictureWidth * newHeight / pictureHeight, maxTexDim);
      dibResized = FreeImage_Rescale(dib, newWidth, newHeight, FILTER_BILINEAR);
   }
   else if (pictureWidth < MIN_TEXTURE_SIZE || pictureHeight < MIN_TEXTURE_SIZE)
   {
      // some drivers seem to choke on small (1x1) textures, so be safe by scaling them up
      const int newWidth = max(pictureWidth, MIN_TEXTURE_SIZE);
      const int newHeight = max(pictureHeight, MIN_TEXTURE_SIZE);
      dibResized = FreeImage_Rescale(dib, newWidth, newHeight, FILTER_BOX);
   }

   const FREE_IMAGE_TYPE img_type = FreeImage_GetImageType(dibResized);
   const bool rgbf = (img_type == FIT_FLOAT) || (img_type == FIT_DOUBLE) || (img_type == FIT_RGBF) || (img_type == FIT_RGBAF); //(FreeImage_GetBPP(dibResized) > 32);
   FIBITMAP* dib32 = rgbf ? FreeImage_ConvertToRGBF(dibResized) : FreeImage_ConvertTo32Bits(dibResized);

   BaseTexture* tex = new BaseTexture(FreeImage_GetWidth(dib32), FreeImage_GetHeight(dib32), rgbf ? RGB_FP : RGBA);
   tex->m_realWidth = pictureWidth;
   tex->m_realHeight = pictureHeight;

   BYTE * const psrc = FreeImage_GetBits(dib32), *pdst = tex->data();
   const int pitchdst = FreeImage_GetPitch(dib32), pitchsrc = tex->pitch();
   const int height = tex->height();

   for (int y = 0; y < height; ++y)
      memcpy(pdst + (height - y - 1)*pitchdst, psrc + y*pitchsrc, pitchsrc);

   FreeImage_Unload(dib32);
   if (dibResized != dib)      // did we allocate a rescaled copy?
      FreeImage_Unload(dibResized);

   return tex;
}

BaseTexture* BaseTexture::CreateFromFile(const char *szfile)
{
   if (szfile == NULL || szfile[0] == '\0')
      return NULL;

   FREE_IMAGE_FORMAT fif;

   // check the file signature and deduce its format
   fif = FreeImage_GetFileType(szfile, 0);
   if (fif == FIF_UNKNOWN) {
      // try to guess the file format from the file extension
      fif = FreeImage_GetFIFFromFilename(szfile);
   }

   // check that the plugin has reading capabilities ...
   if ((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
      // ok, let's load the file
      FIBITMAP *dib = FreeImage_Load(fif, szfile, 0);
      
      BaseTexture* mySurface = BaseTexture::CreateFromFreeImage(dib);
      FreeImage_Unload(dib);

      //if (bitsPerPixel == 24)
      //   mySurface->SetOpaque();

      return mySurface;
   }
   else
      return NULL;
}

BaseTexture* BaseTexture::CreateFromData(const void *data, const size_t size)
{
   FREE_IMAGE_FORMAT fif;

   // check the file signature and deduce its format
   FIMEMORY *dataHandle = FreeImage_OpenMemory((BYTE*)data, size);
   fif = FreeImage_GetFileTypeFromMemory(dataHandle, size);

   // check that the plugin has reading capabilities ...
   if ((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
      // ok, let's load the file
      FIBITMAP *dib = FreeImage_LoadFromMemory(fif, dataHandle, 0);

      BaseTexture* mySurface = BaseTexture::CreateFromFreeImage(dib);
      FreeImage_Unload(dib);

      //if (bitsPerPixel == 24)
      //   mySurface->SetOpaque();

      FreeImage_CloseMemory(dataHandle);
      return mySurface;
   }
   FreeImage_CloseMemory(dataHandle);
   return NULL;
}

// from the FreeImage FAQ page
static FIBITMAP* HBitmapToFreeImage(HBITMAP hbmp)
{
   BITMAP bm;
   GetObject(hbmp, sizeof(BITMAP), &bm);
   FIBITMAP* dib = FreeImage_Allocate(bm.bmWidth, bm.bmHeight, bm.bmBitsPixel);
   // The GetDIBits function clears the biClrUsed and biClrImportant BITMAPINFO members (dont't know why)
   // So we save these infos below. This is needed for palettized images only.
   const int nColors = FreeImage_GetColorsUsed(dib);
   const HDC dc = GetDC(NULL);
   const int Success = GetDIBits(dc, hbmp, 0, FreeImage_GetHeight(dib),
      FreeImage_GetBits(dib), FreeImage_GetInfo(dib), DIB_RGB_COLORS);
   ReleaseDC(NULL, dc);
   // restore BITMAPINFO members
   FreeImage_GetInfoHeader(dib)->biClrUsed = nColors;
   FreeImage_GetInfoHeader(dib)->biClrImportant = nColors;
   return dib;
}

BaseTexture* BaseTexture::CreateFromHBitmap(const HBITMAP hbm)
{
   FIBITMAP *dib = HBitmapToFreeImage(hbm);
   BaseTexture* pdds = BaseTexture::CreateFromFreeImage(dib);
   FreeImage_Unload(dib);
   return pdds;
}


////////////////////////////////////////////////////////////////////////////////


Texture::Texture()
{
   m_pdsBuffer = NULL;
   m_hbmGDIVersion = NULL;
   m_ppb = NULL;
   m_alphaTestValue = 1.0f;
   memset(m_szName, 0, MAXTOKEN);
}

Texture::Texture(BaseTexture * const base)
{
   m_pdsBuffer = base;
   SetSizeFrom(base);

   m_hbmGDIVersion = NULL;
   m_ppb = NULL;
   m_alphaTestValue = 1.0f;
   memset(m_szName, 0, MAXTOKEN);
}

Texture::~Texture()
{
   FreeStuff();
}

HRESULT Texture::SaveToStream(IStream *pstream, PinTable *pt)
{
   BiffWriter bw(pstream, NULL);

   bw.WriteString(FID(NAME), m_szName);
   bw.WriteString(FID(INME), m_szInternalName);
   bw.WriteString(FID(PATH), m_szPath);

   bw.WriteInt(FID(WDTH), m_width);
   bw.WriteInt(FID(HGHT), m_height);

   if (!m_ppb)
   {
      bw.WriteTag(FID(BITS));

      // 32-bit picture
      LZWWriter lzwwriter(pstream, (int *)m_pdsBuffer->data(), m_width * 4, m_height, m_pdsBuffer->pitch());
      lzwwriter.CompressBits(8 + 1);
   }
   else // JPEG (or other binary format)
   {
      if (!pt->GetImageLink(this))
      {
         bw.WriteTag(FID(JPEG));
         m_ppb->SaveToStream(pstream);
      }
      else
         bw.WriteInt(FID(LINK), 1);
   }
   bw.WriteFloat(FID(ALTV), m_alphaTestValue);
   bw.WriteTag(FID(ENDB));

   return S_OK;
}

HRESULT Texture::LoadFromStream(IStream *pstream, int version, PinTable *pt)
{
   BiffReader br(pstream, this, pt, version, NULL, NULL);

   br.Load();

   return ((m_pdsBuffer != NULL) ? S_OK : E_FAIL);
}


bool Texture::LoadFromMemory(BYTE * const data, const DWORD size)
{
   FIMEMORY *hmem = FreeImage_OpenMemory(data, size);
   FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(hmem, 0);
   FIBITMAP *dib = FreeImage_LoadFromMemory(fif, hmem, 0);
   FreeImage_CloseMemory(hmem);

   if (m_pdsBuffer)
      FreeStuff();

   m_pdsBuffer = BaseTexture::CreateFromFreeImage(dib);
   SetSizeFrom(m_pdsBuffer);

   FreeImage_Unload(dib);

   return true;
}


BOOL Texture::LoadToken(int id, BiffReader *pbr)
{
   if (id == FID(NAME))
   {
      pbr->GetString(m_szName);
   }
   else if (id == FID(INME))
   {
      pbr->GetString(m_szInternalName);
   }
   else if (id == FID(PATH))
   {
      pbr->GetString(m_szPath);
   }
   else if (id == FID(WDTH))
   {
      pbr->GetInt(&m_width);
   }
   else if (id == FID(HGHT))
   {
      pbr->GetInt(&m_height);
   }
   else if (id == FID(ALTV))
   {
      pbr->GetFloat(&m_alphaTestValue);
   }
   else if (id == FID(BITS))
   {
      if (m_pdsBuffer)
         FreeStuff();

      m_pdsBuffer = new BaseTexture(m_width, m_height);
      SetSizeFrom(m_pdsBuffer);

      // 32-bit picture
      LZWReader lzwreader(pbr->m_pistream, (int *)m_pdsBuffer->data(), m_width * 4, m_height, m_pdsBuffer->pitch());
      lzwreader.Decoder();

      const int lpitch = m_pdsBuffer->pitch();

      // Assume our 32 bit color structure
      // Find out if all alpha values are zero
      BYTE * const pch = (BYTE *)m_pdsBuffer->data();
      bool allAlphaZero = true;
      for (int i = 0; i < m_height; i++)
      {
         for (int l = 0; l < m_width; l++)
         {
            if (pch[i*lpitch + 4 * l + 3] != 0)
            {
               allAlphaZero = false;
               goto endAlphaCheck;
            }
         }
      }
   endAlphaCheck:

      // all alpha values are 0: set them all to 0xff
      if (allAlphaZero)
         for (int i = 0; i < m_height; i++)
            for (int l = 0; l < m_width; l++)
               pch[i*lpitch + 4 * l + 3] = 0xff;
   }
   else if (id == FID(JPEG))
   {
      m_ppb = new PinBinary();
      m_ppb->LoadFromStream(pbr->m_pistream, pbr->m_version);
      // m_ppb->m_szPath has the original filename
      // m_ppb->m_pdata() is the buffer
      // m_ppb->m_cdata() is the filesize
      return LoadFromMemory((BYTE*)m_ppb->m_pdata, m_ppb->m_cdata);
   }
   else if (id == FID(LINK))
   {
      int linkid;
      PinTable * const pt = (PinTable *)pbr->m_pdata;
      pbr->GetInt(&linkid);
      m_ppb = pt->GetImageLinkBinary(linkid);
      return LoadFromMemory((BYTE*)m_ppb->m_pdata, m_ppb->m_cdata);
   }

   return fTrue;
}

void Texture::FreeStuff()
{
   delete m_pdsBuffer;
   m_pdsBuffer = NULL;
   if (m_hbmGDIVersion)
   {
      DeleteObject(m_hbmGDIVersion);
      m_hbmGDIVersion = NULL;
   }
   if (m_ppb)
   {
      delete m_ppb;
      m_ppb = NULL;
   }
}

void Texture::EnsureHBitmap()
{
   if (!m_hbmGDIVersion)
      CreateGDIVersion();
}

void Texture::CreateGDIVersion()
{
   HDC hdcScreen = GetDC(NULL);
   m_hbmGDIVersion = CreateCompatibleBitmap(hdcScreen, m_width, m_height);
   HDC hdcNew = CreateCompatibleDC(hdcScreen);
   HBITMAP hbmOld = (HBITMAP)SelectObject(hdcNew, m_hbmGDIVersion);

   BITMAPINFO bmi;
   ZeroMemory(&bmi, sizeof(bmi));
   bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   bmi.bmiHeader.biWidth = m_width;
   bmi.bmiHeader.biHeight = -m_height;
   bmi.bmiHeader.biPlanes = 1;
   bmi.bmiHeader.biBitCount = 32;
   bmi.bmiHeader.biCompression = BI_RGB;
   bmi.bmiHeader.biSizeImage = 0;

   BYTE *tmp = new BYTE[m_width*m_height * 4];
   m_pdsBuffer->CopyTo_ConvertAlpha(tmp);

   SetStretchBltMode(hdcNew, COLORONCOLOR);
   StretchDIBits(hdcNew,
      0, 0, m_width, m_height,
      0, 0, m_width, m_height,
      tmp, &bmi, DIB_RGB_COLORS, SRCCOPY);

   delete[] tmp;

   SelectObject(hdcNew, hbmOld);
   DeleteDC(hdcNew);
   ReleaseDC(NULL, hdcScreen);
}

void Texture::GetTextureDC(HDC *pdc)
{
   EnsureHBitmap();
   *pdc = CreateCompatibleDC(NULL);
   m_oldHBM = (HBITMAP)SelectObject(*pdc, m_hbmGDIVersion);
}

void Texture::ReleaseTextureDC(HDC dc)
{
   SelectObject(dc, m_oldHBM);
   DeleteDC(dc);
}

void Texture::CreateFromResource(const int id)
{
   const HBITMAP hbm = (HBITMAP)LoadImage(g_hinst, MAKEINTRESOURCE(id), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

   if (m_pdsBuffer)
      FreeStuff();

   if (hbm == NULL)
      return;

   m_pdsBuffer = CreateFromHBitmap(hbm);
}

BaseTexture* Texture::CreateFromHBitmap(const HBITMAP hbm)
{
   BaseTexture* const pdds = BaseTexture::CreateFromHBitmap(hbm);
   SetSizeFrom(pdds);
   return pdds;
}

void Texture::CreateTextureOffscreen(const int width, const int height)
{
   if (m_pdsBuffer)
      FreeStuff();

   m_pdsBuffer = new BaseTexture(width, height);
   SetSizeFrom(m_pdsBuffer);
}

void BaseTexture::SetOpaque()
{
   if (m_format == BaseTexture::RGB_FP)
      return;

   // Assume our 32 bit color structure
   BYTE *pch = data();

   for (int i = 0; i < height(); i++)
   {
      unsigned int offs = 3;
      for (int l = 0; l < width(); l++,offs+=4)
         pch[offs] = 0xff;

      pch += pitch();
   }
}
