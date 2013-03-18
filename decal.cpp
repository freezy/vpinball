// Decal.cpp: implementation of the Decal class.
//
//////////////////////////////////////////////////////////////////////
 
#include "stdafx.h" 

#define AUTOLEADING (tm.tmAscent - tm.tmInternalLeading/4)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Decal::Decal()
	{
	m_pIFont = NULL;
	} 
  
Decal::~Decal() 
	{
	m_pIFont->Release();
	}

HRESULT Decal::Init(PinTable *ptable, float x, float y, bool fromMouseClick)
	{
	m_ptable = ptable;

	m_d.m_vCenter.x = x;
	m_d.m_vCenter.y = y;

	SetDefaults(fromMouseClick);

	InitVBA(fTrue, 0, NULL);

	EnsureSize();

	return S_OK;
	}

void Decal::SetDefaults(bool fromMouseClick)
	{

	HRESULT hr;
	float fTmp;
	int iTmp;

	hr = GetRegStringAsFloat("DefaultProps\\Decal","Width", &fTmp);
	m_d.m_width = (hr == S_OK) && fromMouseClick ? fTmp : 100.0f;
	
	hr = GetRegStringAsFloat("DefaultProps\\Decal","Height", &fTmp);
	m_d.m_height = (hr == S_OK) && fromMouseClick ? fTmp : 100.0f;

	hr = GetRegStringAsFloat("DefaultProps\\Decal","Rotation", &fTmp);
	m_d.m_rotation = (hr == S_OK) && fromMouseClick ? fTmp : 0;

	hr = GetRegString("DefaultProps\\Decal","Image", m_d.m_szImage, MAXTOKEN);
	if ((hr != S_OK) || !fromMouseClick)
		m_d.m_szImage[0] = 0;
	hr = GetRegString("DefaultProps\\Decal","Surface", m_d.m_szSurface, MAXTOKEN);
	if ((hr != S_OK) || !fromMouseClick)
		m_d.m_szSurface[0] = 0;

	hr = GetRegInt("DefaultProps\\Decal", "DecalType", &iTmp);
	m_d.m_decaltype = (hr == S_OK) && fromMouseClick ? (enum DecalType)iTmp : DecalImage;
	
	hr = GetRegString("DefaultProps\\Decal","Text", m_d.m_sztext, MAXSTRING);
	if  ((hr != S_OK) || !fromMouseClick)
		m_d.m_sztext[0] = '\0';

	hr = GetRegInt("DefaultProps\\Decal", "Sizing", &iTmp);
	m_d.m_sizingtype = (hr == S_OK) && fromMouseClick ? (enum SizingType)iTmp : ManualSize;
	
	hr = GetRegInt("DefaultProps\\Decal", "Color", &iTmp);
	m_d.m_color = (hr == S_OK) && fromMouseClick ? iTmp : RGB(0,0,0);

	hr = GetRegInt("DefaultProps\\Decal", "VerticalText", &iTmp);
	if ((hr == S_OK) && fromMouseClick)
		m_d.m_fVerticalText = iTmp == 0 ? false : true;
	else
		m_d.m_fVerticalText = fFalse;

	if (!m_pIFont)
		{
		FONTDESC fd;
		fd.cbSizeofstruct = sizeof(FONTDESC);
		
		hr = GetRegStringAsFloat("DefaultProps\\Decal","FontSize", &fTmp);
		fd.cySize.int64 = (hr == S_OK) && fromMouseClick ? (LONGLONG)(fTmp * 10000.0) : 142500;

		char tmp[256];
		hr = GetRegString("DefaultProps\\Decal","FontName", tmp, 256);
		if ((hr != S_OK) || !fromMouseClick)
			fd.lpstrName = L"Arial Black";
		else
		{
			unsigned int len = strlen(&tmp[0])+1;
			fd.lpstrName = (LPOLESTR) malloc(len*sizeof(WCHAR));
			UNICODE_FROM_ANSI(fd.lpstrName, &tmp[0], len); 
			fd.lpstrName[len] = 0;
		}

		hr = GetRegInt("DefaultProps\\Decal", "FontWeight", &iTmp);
		fd.sWeight = (hr == S_OK) && fromMouseClick ? iTmp : FW_NORMAL;
	
		hr = GetRegInt("DefaultProps\\Decal", "FontCharSet", &iTmp);
		fd.sCharset = (hr == S_OK) && fromMouseClick ? iTmp : 0;
		
		hr = GetRegInt("DefaultProps\\Decal", "FontItalic", &iTmp);
		if ((hr == S_OK) && fromMouseClick)
			fd.fItalic = iTmp == 0 ? false : true;
		else
			fd.fItalic = 0;

		hr = GetRegInt("DefaultProps\\Decal", "FontUnderline", &iTmp);
		if ((hr == S_OK) && fromMouseClick)
			fd.fUnderline = iTmp == 0 ? false : true;
		else
			fd.fUnderline = 0;
		
		hr = GetRegInt("DefaultProps\\Decal", "FontStrikeThrough", &iTmp);
		if ((hr == S_OK) && fromMouseClick)
			fd.fStrikethrough = iTmp == 0 ? false : true;
		else
			fd.fStrikethrough = 0;
		
		OleCreateFontIndirect(&fd, IID_IFont, (void **)&m_pIFont);
		}
	}

void Decal::WriteRegDefaults()
	{
	char strTmp[MAXTOKEN];
	float fTmp;

	sprintf_s(strTmp, 40, "%f", m_d.m_width);
	SetRegValue("DefaultProps\\Decal","Width", REG_SZ, &strTmp,strlen(strTmp));
	sprintf_s(strTmp, 40, "%f", m_d.m_height);
	SetRegValue("DefaultProps\\Decal","Height", REG_SZ, &strTmp,strlen(strTmp));
	sprintf_s(strTmp, 40, "%f", m_d.m_rotation);
	SetRegValue("DefaultProps\\Decal","Rotation", REG_SZ, &strTmp,strlen(strTmp));
	SetRegValue("DefaultProps\\Decal","Image", REG_SZ, &m_d.m_szImage,strlen(m_d.m_szImage));
	SetRegValue("DefaultProps\\Decal","DecalType",REG_DWORD,&m_d.m_decaltype,4);
	SetRegValue("DefaultProps\\Decal","Text", REG_SZ, &m_d.m_sztext,strlen(m_d.m_sztext));
	SetRegValue("DefaultProps\\Decal","Sizing",REG_DWORD,&m_d.m_sizingtype,4);
	SetRegValue("DefaultProps\\Decal","Color",REG_DWORD,&m_d.m_color,4);
	SetRegValue("DefaultProps\\Decal","VerticalText",REG_DWORD,&m_d.m_fVerticalText,4);
	SetRegValue("DefaultProps\\Decal","Surface", REG_SZ, m_d.m_szSurface, strlen(m_d.m_szSurface)); 

	if (m_pIFont)
		{
		FONTDESC fd;
		fd.cbSizeofstruct = sizeof(FONTDESC);
		m_pIFont->get_Size(&fd.cySize); 
		m_pIFont->get_Name(&fd.lpstrName); 
		m_pIFont->get_Weight(&fd.sWeight); 
		m_pIFont->get_Charset(&fd.sCharset); 
		m_pIFont->get_Italic(&fd.fItalic);
		m_pIFont->get_Underline(&fd.fUnderline); 
		m_pIFont->get_Strikethrough(&fd.fStrikethrough); 
		
		fTmp = (float)(fd.cySize.int64 / 10000.0);
		sprintf_s(strTmp, 40, "%f", fTmp);
		SetRegValue("DefaultProps\\Decal","FontSize", REG_SZ, &strTmp,strlen(strTmp));
		int charCnt = wcslen(fd.lpstrName) +1;
		WideCharToMultiByte(CP_ACP, 0, fd.lpstrName, charCnt, strTmp, 2*charCnt, NULL, NULL);
		SetRegValue("DefaultProps\\Decal","FontName", REG_SZ, &strTmp,strlen(strTmp));
		SetRegValue("DefaultProps\\Decal","FontWeight",REG_DWORD,&fd.sWeight,4);
		SetRegValue("DefaultProps\\Decal","FontCharSet",REG_DWORD,&fd.sCharset,4);
		SetRegValue("DefaultProps\\Decal","FontItalic",REG_DWORD,&fd.fItalic,4);
		SetRegValue("DefaultProps\\Decal","FontUnderline",REG_DWORD,&fd.fUnderline,4);
		SetRegValue("DefaultProps\\Decal","FontStrikeThrough",REG_DWORD,&fd.fStrikethrough,4);
		}
	}


HRESULT Decal::InitVBA(BOOL fNew, int id, WCHAR *wzName)
	{
#ifdef VBA
	HRESULT hr;

	if (fNew)
		{
		hr = ApcControl.Define(m_ptable->ApcProjectItem,
			(IDispatch *)this, axControlNoCodeBehind, NULL);
		}
	else
		{
		hr = ApcControl.Register(m_ptable->ApcProjectItem, id, (IDispatch *)this, NULL);
		}

	return hr;
#else
	return S_OK;
#endif
	}

void Decal::PreRender(Sur * const psur)
	{
	if( !m_fBackglass || GetPTable()->GetDecalsEnabled() )
		{
		psur->SetBorderColor(-1,false,0);
		psur->SetFillColor(RGB(0,0,255));
		psur->SetObject(this);

		const float halfwidth = m_realwidth/*m_d.m_width*/ * 0.5f;
		const float halfheight = m_realheight/*m_d.m_height*/ * 0.5f;

		const float radangle = ANGTORAD(m_d.m_rotation);
		const float sn = sinf(radangle);
		const float cs = cosf(radangle);

		const Vertex2D rgv[4] = {
		Vertex2D(m_d.m_vCenter.x + sn*halfheight - cs*halfwidth,
		         m_d.m_vCenter.y - cs*halfheight - sn*halfwidth),

		Vertex2D(m_d.m_vCenter.x + sn*halfheight + cs*halfwidth,
				 m_d.m_vCenter.y - cs*halfheight + sn*halfwidth),

		Vertex2D(m_d.m_vCenter.x - sn*halfheight + cs*halfwidth,
				 m_d.m_vCenter.y + cs*halfheight + sn*halfwidth),

		Vertex2D(m_d.m_vCenter.x - sn*halfheight - cs*halfwidth,
				 m_d.m_vCenter.y + cs*halfheight - sn*halfwidth)};

		psur->Polygon(rgv, 4);
		}
	}

void Decal::Render(Sur * const psur)
	{
	if( !m_fBackglass || GetPTable()->GetDecalsEnabled() )
		{
		psur->SetBorderColor(RGB(0,0,0),false,0);
		psur->SetFillColor(-1);
		psur->SetObject(this);
		psur->SetObject(NULL);

		const float halfwidth = m_realwidth * 0.5f;
		const float halfheight = m_realheight * 0.5f;

		const float radangle = ANGTORAD(m_d.m_rotation);
		const float sn = sinf(radangle);
		const float cs = cosf(radangle);

		const Vertex2D rgv[4] = {
		Vertex2D(m_d.m_vCenter.x + sn*halfheight - cs*halfwidth,
				 m_d.m_vCenter.y - cs*halfheight - sn*halfwidth),

		Vertex2D(m_d.m_vCenter.x + sn*halfheight + cs*halfwidth,
			     m_d.m_vCenter.y - cs*halfheight + sn*halfwidth),

		Vertex2D(m_d.m_vCenter.x - sn*halfheight + cs*halfwidth,
			     m_d.m_vCenter.y + cs*halfheight + sn*halfwidth),

		Vertex2D(m_d.m_vCenter.x - sn*halfheight - cs*halfwidth,
				 m_d.m_vCenter.y + cs*halfheight - sn*halfwidth)};

		psur->Polygon(rgv, 4);
		}
	}

void Decal::GetTimers(Vector<HitTimer> * const pvht)
	{
	IEditable::BeginPlay();
	}

void Decal::GetTextSize(int * const px, int * const py)
	{
	const int len = lstrlen(m_d.m_sztext);
	HFONT hFont = GetFont();
	const int alignment = DT_LEFT;

	HDC hdcNull = GetDC(NULL);
	HFONT hFontOld = (HFONT)SelectObject(hdcNull, hFont);

	TEXTMETRIC tm;
	GetTextMetrics(hdcNull, &tm);

	if (m_d.m_fVerticalText)
		{
		// Do huge amounts of work to get rid of the descent and internal ascent of the font, because it leaves ugly spaces
		*py = AUTOLEADING * len;
		*px = 0;
		for (int i=0;i<len;i++)
			{
			RECT rcOut;
			rcOut.left = 0;
			rcOut.top = 0;		//-tm.tmInternalLeading + 2; // Leave a pixel for anti-aliasing;
			rcOut.right = 0x1;
			rcOut.bottom = 0x1;
			DrawText(hdcNull, &m_d.m_sztext[i], 1, &rcOut, alignment | DT_NOCLIP | DT_NOPREFIX | DT_WORDBREAK | DT_CALCRECT);

			*px = max(*px, rcOut.right);
			}
		}
	else
		{
		*py = tm.tmAscent;

		RECT rcOut;
		rcOut.left = 0;
		rcOut.top = 0;			//-tm.tmInternalLeading + 2; // Leave a pixel for anti-aliasing;
		rcOut.right = 0x1;
		rcOut.bottom = 0x1;
		DrawText(hdcNull, m_d.m_sztext, len, &rcOut, alignment | DT_NOCLIP | DT_NOPREFIX | DT_WORDBREAK | DT_CALCRECT);

		*px = rcOut.right;
		}

	SelectObject(hdcNull, hFontOld);
	ReleaseDC(NULL, hdcNull);

	DeleteObject(hFont);
	}

void Decal::GetHitShapes(Vector<HitObject> * const pvho)
	{
	if (m_d.m_decaltype != DecalImage)
		{
		RECT rcOut;
		const int len = lstrlen(m_d.m_sztext);
		HFONT hFont, hFontOld;
		hFont = GetFont();
		int alignment = DT_LEFT;

		HDC hdcNull;
		hdcNull = GetDC(NULL);
		hFontOld = (HFONT)SelectObject(hdcNull, hFont);

		TEXTMETRIC tm;
		GetTextMetrics(hdcNull, &tm);

		float charheight;
		if (m_d.m_fVerticalText)
			{
			int maxwidth = 0;
			
			for (int i=0;i<len;i++)
				{
				rcOut.left = 0;
				rcOut.top = 0;//-tm.tmInternalLeading + 2; // Leave a pixel for anti-aliasing;
				rcOut.right = 1;
				rcOut.bottom = 1;
				DrawText(hdcNull, &m_d.m_sztext[i], 1, &rcOut, alignment | DT_NOCLIP | DT_NOPREFIX | DT_WORDBREAK | DT_CALCRECT);
				maxwidth = max(maxwidth, rcOut.right);
				}

			rcOut.bottom += AUTOLEADING * (len-1);
			rcOut.right = maxwidth;

			charheight = m_realheight/(float)len;
			}
		else
			{
			rcOut.left = 0;
			rcOut.top = 0;//-tm.tmInternalLeading + 2; // Leave a pixel for anti-aliasing;
			rcOut.right = 1;
			rcOut.bottom = 1;
			DrawText(hdcNull, m_d.m_sztext, len, &rcOut, alignment | DT_NOCLIP | DT_NOPREFIX | DT_WORDBREAK | DT_CALCRECT);

			charheight = m_realheight;
			}

		SelectObject(hdcNull, hFontOld);
		ReleaseDC(NULL, hdcNull);

		// Calculate the percentage of the texture which is for oomlats and commas.
		const float invascent = charheight/(float)tm.tmAscent;
		m_leading = (float)tm.tmInternalLeading * invascent /*m_d.m_height*/;
		m_descent = (float)tm.tmDescent * invascent;

		m_pinimage.m_width = rcOut.right;
		m_pinimage.m_height = rcOut.bottom;
		m_pinimage.m_pdsBuffer = NULL;
		m_pinimage.m_pdsBuffer = g_pvp->m_pdd.CreateTextureOffscreen(m_pinimage.m_width, m_pinimage.m_height);

		if (m_d.m_color == RGB(255,255,255))
			m_d.m_color = RGB(254,255,255); //m_pinimage.SetTransparentColor(RGB(0,0,0));
		else if (m_d.m_color == RGB(0,0,0))
			m_d.m_color = RGB(0,0,1);

		HDC hdc;
		m_pinimage.m_pdsBuffer->GetDC(&hdc);
		/*if (m_d.m_color == RGB(255,255,255))
			{
			SelectObject(hdc, GetStockObject(BLACK_BRUSH));
			}
		else*/
			{
			SelectObject(hdc, GetStockObject(WHITE_BRUSH));
			}

		PatBlt(hdc,0,0,m_pinimage.m_width,m_pinimage.m_height,PATCOPY);
		hFontOld = (HFONT)SelectObject(hdc, hFont);
		
		SetTextColor(hdc, m_d.m_color);
		SetBkMode(hdc, TRANSPARENT);
		SetTextAlign(hdc, TA_LEFT | TA_TOP | TA_NOUPDATECP);
		alignment = DT_CENTER;

		if (m_d.m_fVerticalText)
			{
			for (int i=0;i<len;i++)
				{
				rcOut.top = AUTOLEADING*i;//-tm.tmInternalLeading + 2; // Leave a pixel for anti-aliasing;
				rcOut.bottom = rcOut.top + 100;
				/*const int foo =*/ DrawText(hdc, &m_d.m_sztext[i], 1, &rcOut, alignment | DT_NOCLIP | DT_NOPREFIX | DT_WORDBREAK);
				}
			}
		else
			{
			DrawText(hdc, m_d.m_sztext, len, &rcOut, alignment | DT_NOCLIP | DT_NOPREFIX | DT_WORDBREAK);
			}

		SelectObject(hdcNull, hFontOld);
		ReleaseDC(NULL, hdcNull);

		m_pinimage.m_pdsBuffer->ReleaseDC(hdc);

		g_pvp->m_pdd.SetOpaque(m_pinimage.m_pdsBuffer, m_pinimage.m_width, m_pinimage.m_height);
		DeleteObject(hFont);
		}
	}

void Decal::GetHitShapesDebug(Vector<HitObject> * const pvho)
	{
	}

void Decal::EndPlay()
	{
	if (m_pinimage.m_pdsBuffer)
		{
		m_pinimage.FreeStuff();
		}
	}

void Decal::PostRenderStatic(const RenderDevice* pd3dDevice)
	{
	}

static const Material decalmtrl = {1.f,1.f,1.f,.5f, 1.f,1.f,1.f,.5f, 0.f,0.f,0.f,0.f, 0.f,0.f,0.f,0.f, 0.f};

void Decal::RenderStatic(const RenderDevice* _pd3dDevice)
{
   RenderDevice* pd3dDevice=(RenderDevice*)_pd3dDevice;
	Pin3D * const ppin3d = &g_pplayer->m_pin3d;

	const float height = m_ptable->GetSurfaceHeight(m_d.m_szSurface, m_d.m_vCenter.x, m_d.m_vCenter.y);

	pd3dDevice->SetMaterial((Material*)&decalmtrl);

	float leading, descent; // For fonts
	float maxtu, maxtv;
	PinImage *pin;
	if (m_d.m_decaltype != DecalImage)
		{
		leading = m_leading;
		descent = m_descent;

		DDSURFACEDESC2 ddsd;
		ddsd.dwSize = sizeof(ddsd);
		m_pinimage.m_pdsBuffer->GetSurfaceDesc(&ddsd);
		maxtu = (float)m_pinimage.m_width / (float)ddsd.dwWidth;
		maxtv = (float)m_pinimage.m_height / (float)ddsd.dwHeight;
		pin = &m_pinimage;
		}
	else
		{
		m_pinimage.m_pdsBuffer = NULL;
		pin = m_ptable->GetImage(m_d.m_szImage);
		leading = 0;
		descent = 0;
		maxtu = 1.0f;
		maxtv = 1.0f;
		}

	pd3dDevice->SetRenderState( RenderDevice::ZWRITEENABLE, FALSE);

	// Set texture to mirror, so the alpha state of the texture blends correctly to the outside
	pd3dDevice->SetTextureStageState( ePictureTexture, D3DTSS_ADDRESS, D3DTADDRESS_MIRROR);

	if (pin)
		{
		if (m_d.m_decaltype == DecalImage)
			{
			m_ptable->GetTVTU(pin, &maxtu, &maxtv);
			}

		pin->EnsureColorKey();
		pd3dDevice->SetTexture(ePictureTexture, pin->m_pdsBufferColorKey);

		pd3dDevice->SetRenderState( RenderDevice::COLORKEYENABLE, FALSE);
		g_pplayer->m_pin3d.SetTextureFilter ( ePictureTexture, TEXTURE_MODE_TRILINEAR );

		pd3dDevice->SetRenderState( RenderDevice::ALPHABLENDENABLE, TRUE);

		ppin3d->EnableLightMap(fFalse, -1);
		}
	else // No image by that name
		{
		pd3dDevice->SetTexture(ePictureTexture, NULL);

		pd3dDevice->SetRenderState( RenderDevice::COLORKEYENABLE, FALSE);
		}

	// Check if we are in hardware.
	if (g_pvp->m_pdd.m_fHardwareAccel)
		{
		// Render all alpha pixels.
         g_pplayer->m_pin3d.m_pd3dDevice->SetRenderState(RenderDevice::ALPHAREF, (DWORD)0x00000001);
         g_pplayer->m_pin3d.m_pd3dDevice->SetRenderState(RenderDevice::ALPHAFUNC, D3DCMP_GREATEREQUAL);
         g_pplayer->m_pin3d.m_pd3dDevice->SetRenderState(RenderDevice::ALPHATESTENABLE, FALSE); 

		// Turn on anisotopic filtering. 
		g_pplayer->m_pin3d.SetTextureFilter ( ePictureTexture, TEXTURE_MODE_ANISOTROPIC );
		}

	Vertex3D_NoTex2 rgv3D[4];
	for (int l=0;l<4;l++)
		rgv3D[l].z = height + 0.2f;

	ppin3d->ClearExtents(&m_rcBounds, NULL, NULL);

	const float halfwidth = m_realwidth * 0.5f;
	const float halfheight = m_realheight * 0.5f;

	const float radangle = ANGTORAD(m_d.m_rotation);
	const float sn = sinf(radangle);
	const float cs = cosf(radangle);

	rgv3D[0].x = m_d.m_vCenter.x + sn*(halfheight+leading) - cs*halfwidth;
	rgv3D[0].y = m_d.m_vCenter.y - cs*(halfheight+leading) - sn*halfwidth;
	rgv3D[0].tu = 0;
	rgv3D[0].tv = 0;

	rgv3D[1].x = m_d.m_vCenter.x + sn*(halfheight+leading) + cs*halfwidth;
	rgv3D[1].y = m_d.m_vCenter.y - cs*(halfheight+leading) + sn*halfwidth;
	rgv3D[1].tu = maxtu;
	rgv3D[1].tv = 0;

	rgv3D[2].x = m_d.m_vCenter.x - sn*(halfheight+descent) + cs*halfwidth;
	rgv3D[2].y = m_d.m_vCenter.y + cs*(halfheight+descent) + sn*halfwidth;
	rgv3D[2].tu = maxtu;
	rgv3D[2].tv = maxtv;

	rgv3D[3].x = m_d.m_vCenter.x - sn*(halfheight+descent) - cs*halfwidth;
	rgv3D[3].y = m_d.m_vCenter.y + cs*(halfheight+descent) - sn*halfwidth;
	rgv3D[3].tu = 0;
	rgv3D[3].tv = maxtv;

	if (!m_fBackglass)
		{
		SetNormal(rgv3D, rgi0123, 4, NULL, NULL, 0);
		pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, MY_D3DFVF_NOTEX2_VERTEX,rgv3D, 4,(LPWORD)rgi0123,4,0);
		//pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, MY_D3DFVF_NOTEX2_VERTEX,rgv3D, 4,0);
		}
	else
		{
		SetHUDVertices(rgv3D, 4);
		SetDiffuse(rgv3D, 4, 0xFFFFFF);

		if( GetPTable()->GetDecalsEnabled() )
			{
			pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, MY_D3DTRANSFORMED_NOTEX2_VERTEX,rgv3D, 4,(LPWORD)rgi0123,4,0);
			//pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, MY_D3DTRANSFORMED_NOTEX2_VERTEX,rgv3D, 4,0);
			}
		}

	ppin3d->ExpandExtents(&m_rcBounds, rgv3D, NULL, NULL, 4, m_fBackglass);

	// Set the texture state.
	pd3dDevice->SetTexture(ePictureTexture, NULL);
	g_pplayer->m_pin3d.SetTextureFilter ( ePictureTexture, TEXTURE_MODE_TRILINEAR );

	// Set the render state.
   pd3dDevice->SetRenderState(RenderDevice::ALPHABLENDENABLE, FALSE);
   pd3dDevice->SetRenderState(RenderDevice::ZWRITEENABLE, TRUE);
	pd3dDevice->SetTextureStageState( ePictureTexture, D3DTSS_ADDRESS, D3DTADDRESS_WRAP);
	}
	
void Decal::RenderMovers(const RenderDevice* pd3dDevice)
	{
	}

void Decal::SetObjectPos()
	{
	g_pvp->SetObjectPosCur(m_d.m_vCenter.x, m_d.m_vCenter.y);
	}

void Decal::MoveOffset(const float dx, const float dy)
	{
	m_d.m_vCenter.x += dx;
	m_d.m_vCenter.y += dy;

	m_ptable->SetDirtyDraw();
	}

void Decal::GetCenter(Vertex2D * const pv) const
	{
	*pv = m_d.m_vCenter;
	}

void Decal::PutCenter(const Vertex2D * const pv)
	{
	m_d.m_vCenter = *pv;

	m_ptable->SetDirtyDraw();
	}

void Decal::Rotate(float ang, Vertex2D *pvCenter)
	{
	ISelect::Rotate(ang, pvCenter);

	m_d.m_rotation += ang;
	}

HRESULT Decal::SaveData(IStream *pstm, HCRYPTHASH hcrypthash, HCRYPTKEY hcryptkey)
	{
	BiffWriter bw(pstm, hcrypthash, hcryptkey);

#ifdef VBA
	bw.WriteInt(FID(PIID), ApcControl.ID());
#endif
	bw.WriteStruct(FID(VCEN), &m_d.m_vCenter, sizeof(Vertex2D));
	bw.WriteFloat(FID(WDTH), m_d.m_width);
	bw.WriteFloat(FID(HIGH), m_d.m_height);
	bw.WriteFloat(FID(ROTA), m_d.m_rotation);
	bw.WriteString(FID(IMAG), m_d.m_szImage);
	bw.WriteString(FID(SURF), m_d.m_szSurface);
	bw.WriteWideString(FID(NAME), (WCHAR *)m_wzName);
	bw.WriteString(FID(TEXT), m_d.m_sztext);
	bw.WriteInt(FID(TYPE), m_d.m_decaltype);

	bw.WriteInt(FID(COLR), m_d.m_color);
	bw.WriteInt(FID(SIZE), m_d.m_sizingtype);

	bw.WriteBool(FID(VERT), m_d.m_fVerticalText);

	bw.WriteBool(FID(BGLS), m_fBackglass);

	ISelect::SaveData(pstm, hcrypthash, hcryptkey);

	bw.WriteTag(FID(FONT));
	IPersistStream * ips;
	m_pIFont->QueryInterface(IID_IPersistStream, (void **)&ips);
	ips->Save(pstm, TRUE);

	bw.WriteTag(FID(ENDB));

	return S_OK;
	}

HRESULT Decal::InitLoad(IStream *pstm, PinTable *ptable, int *pid, int version, HCRYPTHASH hcrypthash, HCRYPTKEY hcryptkey)
	{
	SetDefaults(false);
#ifndef OLDLOAD
	BiffReader br(pstm, this, pid, version, hcrypthash, hcryptkey);

	m_ptable = ptable;

	br.Load();
	return S_OK;
#else
	ULONG read = 0;
	HRESULT hr = S_OK;

	m_ptable = ptable;

	DWORD dwID;
	if(FAILED(hr = pstm->Read(&dwID, sizeof dwID, &read)))
		return hr;

	if(FAILED(hr = pstm->Read(&m_d, sizeof(DecalData), &read)))
		return hr;

	*pid = dwID;

	return hr;
#endif
	}

BOOL Decal::LoadToken(int id, BiffReader *pbr)
	{
	if (id == FID(PIID))
		{
		pbr->GetInt((int *)pbr->m_pdata);
		}
	else if (id == FID(VCEN))
		{
		pbr->GetStruct(&m_d.m_vCenter, sizeof(Vertex2D));
		}
	else if (id == FID(WDTH))
		{
		pbr->GetFloat(&m_d.m_width);
		}
	else if (id == FID(HIGH))
		{
		pbr->GetFloat(&m_d.m_height);
		}
	else if (id == FID(ROTA))
		{
		pbr->GetFloat(&m_d.m_rotation);
		}
	else if (id == FID(IMAG))
		{
		pbr->GetString(m_d.m_szImage);
		}
	else if (id == FID(SURF))
		{
		pbr->GetString(m_d.m_szSurface);
		}
	else if (id == FID(NAME))
		{
		pbr->GetWideString((WCHAR *)m_wzName);
		}
	else if (id == FID(TEXT))
		{
		pbr->GetString(m_d.m_sztext);
		}
	else if (id == FID(TYPE))
		{
		pbr->GetInt(&m_d.m_decaltype);
		}
	else if (id == FID(COLR))
		{
		pbr->GetInt(&m_d.m_color);
//		if (!(m_d.m_color & MINBLACKMASK)) {m_d.m_color |= MINBLACK;}	// set minimum black
		}
	else if (id == FID(SIZE))
		{
		pbr->GetInt(&m_d.m_sizingtype);
		}
	else if (id == FID(VERT))
		{
		pbr->GetBool(&m_d.m_fVerticalText);
		}
	else if (id == FID(BGLS))
		{
		pbr->GetBool(&m_fBackglass);
		}
	else if (id == FID(FONT))
		{
		if (!m_pIFont)
			{
			FONTDESC fd;
			fd.cbSizeofstruct = sizeof(FONTDESC);
			fd.lpstrName = L"Arial";
			fd.cySize.int64 = 142500;
			fd.sWeight = FW_NORMAL;
			fd.sCharset = 0;
			fd.fItalic = 0;
			fd.fUnderline = 0;
			fd.fStrikethrough = 0;
			OleCreateFontIndirect(&fd, IID_IFont, (void **)&m_pIFont);
			}

		IPersistStream * ips;
		m_pIFont->QueryInterface(IID_IPersistStream, (void **)&ips);

		ips->Load(pbr->m_pistream);
		}
	else
		{
		ISelect::LoadToken(id, pbr);
		}

	return fTrue;
	}

HRESULT Decal::InitPostLoad()
	{
	EnsureSize();

	return S_OK;
	}

void Decal::EnsureSize()
	{
	if (((m_d.m_sizingtype != AutoSize) ||
		(m_d.m_decaltype == DecalImage)) && (m_d.m_sizingtype != AutoWidth) ||
		(m_d.m_decaltype == DecalText && (lstrlen(m_d.m_sztext) == 0)))
		{
		m_realwidth = m_d.m_width;
		m_realheight = m_d.m_height;
		}
	else if ((m_d.m_sizingtype == AutoSize) && (m_d.m_decaltype != DecalImage))
		{
		// ignore the auto aspect flag
		int sizex, sizey;
		GetTextSize(&sizex, &sizey);

		CY cy;
		m_pIFont->get_Size(&cy);

		m_realheight = (float)cy.Lo * (float)(1.0/2545.0);

		if (m_d.m_fVerticalText)
			{
			m_realheight*=lstrlen(m_d.m_sztext);
			}
		m_realwidth = m_realheight * (float)sizex / (float)sizey;
		}
	else // Auto aspect
		{
		m_realheight = m_d.m_height;

		if (m_d.m_decaltype == DecalImage)
			{
			PinImage * const pin = m_ptable->GetImage(m_d.m_szImage);
			if (pin)
				{
				m_realwidth = m_realheight * (float)pin->m_width / (float)pin->m_height;
				}
			else
				{
				m_realwidth = m_realheight;
				}
			}
		else
			{
			CY cy;
			m_pIFont->get_Size(&cy);

			int sizex, sizey;
			GetTextSize(&sizex, &sizey);

			m_realheight = (float)cy.Lo * (float)(1.0/2545.0);

			if (m_d.m_fVerticalText)
				{
				m_realheight *= lstrlen(m_d.m_sztext);
				m_realwidth = m_d.m_width;
				}
			else
				{
				m_realwidth = m_realheight * (float)sizex / (float)sizey;
				m_realheight = m_d.m_height;
				}
			}
		}
	}

HFONT Decal::GetFont()
	{
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(lf));

	lf.lfHeight = -72;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfQuality = NONANTIALIASED_QUALITY;

	CComBSTR bstr;
	HRESULT hr = m_pIFont->get_Name(&bstr);

	WideCharToMultiByte(CP_ACP, 0, bstr, -1, lf.lfFaceName, LF_FACESIZE, NULL, NULL);

	BOOL bl;
	hr = m_pIFont->get_Bold(&bl);

	lf.lfWeight = bl ? FW_BOLD : FW_NORMAL;

	hr = m_pIFont->get_Italic(&bl);

	lf.lfItalic = (BYTE)bl;



	HFONT hFont = CreateFontIndirect(&lf);

	return hFont;
	}

STDMETHODIMP Decal::get_Rotation(float *pVal)
{
	*pVal = m_d.m_rotation;

	return S_OK;
}

STDMETHODIMP Decal::put_Rotation(float newVal)
{
	STARTUNDO

	m_d.m_rotation = newVal;

	STOPUNDO

	return S_OK;
}

STDMETHODIMP Decal::get_Image(BSTR *pVal)
{
	WCHAR wz[512];

	MultiByteToWideChar(CP_ACP, 0, m_d.m_szImage, -1, wz, 32);
	*pVal = SysAllocString(wz);

	return S_OK;
}

STDMETHODIMP Decal::put_Image(BSTR newVal)
{
	STARTUNDO

	WideCharToMultiByte(CP_ACP, 0, newVal, -1, m_d.m_szImage, 32, NULL, NULL);

	STOPUNDO

	return S_OK;
}

STDMETHODIMP Decal::get_Width(float *pVal)
{
	*pVal = m_d.m_width;

	return S_OK;
}

STDMETHODIMP Decal::put_Width(float newVal)
{
	STARTUNDO

	m_d.m_width = newVal;

	EnsureSize();

	STOPUNDO

	return S_OK;
}

STDMETHODIMP Decal::get_Height(float *pVal)
{
	*pVal = m_d.m_height;

	return S_OK;
}

STDMETHODIMP Decal::put_Height(float newVal)
{
	STARTUNDO

	m_d.m_height = newVal;

	EnsureSize();

	STOPUNDO

	return S_OK;
}

STDMETHODIMP Decal::get_X(float *pVal)
{
	*pVal = m_d.m_vCenter.x;

	return S_OK;
}

STDMETHODIMP Decal::put_X(float newVal)
{
	STARTUNDO

	m_d.m_vCenter.x = newVal;

	STOPUNDO

	return S_OK;
}

STDMETHODIMP Decal::get_Y(float *pVal)
{
	*pVal = m_d.m_vCenter.y;

	return S_OK;
}

STDMETHODIMP Decal::put_Y(float newVal)
{
	STARTUNDO

	m_d.m_vCenter.y = newVal;

	STOPUNDO

	return S_OK;
}

STDMETHODIMP Decal::get_Surface(BSTR *pVal)
{
	WCHAR wz[512];

	MultiByteToWideChar(CP_ACP, 0, m_d.m_szSurface, -1, wz, 32);
	*pVal = SysAllocString(wz);

	return S_OK;
}

STDMETHODIMP Decal::put_Surface(BSTR newVal)
{
	STARTUNDO

	WideCharToMultiByte(CP_ACP, 0, newVal, -1, m_d.m_szSurface, 32, NULL, NULL);

	STOPUNDO

	return S_OK;
}

void Decal::GetDialogPanes(Vector<PropertyPane> *pvproppane)
	{
	PropertyPane *pproppane;

	pproppane = new PropertyPane(IDD_PROPDECAL_VISUALS, IDS_VISUALS);
	pvproppane->AddElement(pproppane);

	pproppane = new PropertyPane(IDD_PROPDECAL_POSITION, IDS_POSITION);
	pvproppane->AddElement(pproppane);
	}

STDMETHODIMP Decal::get_Type(DecalType *pVal)
{
	*pVal = m_d.m_decaltype;

	return S_OK;
}

STDMETHODIMP Decal::put_Type(DecalType newVal)
{
	STARTUNDO

	m_d.m_decaltype = newVal;

	EnsureSize();

	STOPUNDO

	return S_OK;
}

STDMETHODIMP Decal::get_Text(BSTR *pVal)
{
	WCHAR wz[512];

	MultiByteToWideChar(CP_ACP, 0, (char *)m_d.m_sztext, -1, wz, 512);
	*pVal = SysAllocString(wz);

	return S_OK;
}

STDMETHODIMP Decal::put_Text(BSTR newVal)
{
	if (lstrlenW(newVal) < 512)
		{
		STARTUNDO

		WideCharToMultiByte(CP_ACP, 0, newVal, -1, m_d.m_sztext, 512, NULL, NULL);

		EnsureSize();

		STOPUNDO
		}

	return S_OK;
}

STDMETHODIMP Decal::get_SizingType(SizingType *pVal)
{
	*pVal = m_d.m_sizingtype;

	return S_OK;
}

STDMETHODIMP Decal::put_SizingType(SizingType newVal)
{
	STARTUNDO

	m_d.m_sizingtype = newVal;

	EnsureSize();

	STOPUNDO

	return S_OK;
}

STDMETHODIMP Decal::get_FontColor(OLE_COLOR *pVal)
{
	*pVal = m_d.m_color;

	return S_OK;
}

STDMETHODIMP Decal::put_FontColor(OLE_COLOR newVal)
{
	STARTUNDO

	m_d.m_color = newVal;

	STOPUNDO

	return S_OK;
}

STDMETHODIMP Decal::get_Font(IFontDisp **pVal)
{
	m_pIFont->QueryInterface(IID_IFontDisp, (void **)pVal);

	return S_OK;
}

STDMETHODIMP Decal::putref_Font(IFontDisp *pFont)
{
	//We know that our own property browser gives us the same pointer

	SetDirtyDraw();

	EnsureSize();

	return S_OK;
}

STDMETHODIMP Decal::get_HasVerticalText(VARIANT_BOOL *pVal)
{
	*pVal = (VARIANT_BOOL)FTOVB(m_d.m_fVerticalText);

	return S_OK;
}

STDMETHODIMP Decal::put_HasVerticalText(VARIANT_BOOL newVal)
{
	STARTUNDO

	m_d.m_fVerticalText = VBTOF(newVal);

	SetDirtyDraw();

	EnsureSize();

	STOPUNDO

	return S_OK;
}
