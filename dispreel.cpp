/******************************************************************************
**
** Module name   : DispReel.c
** Module type   : Component source file
** Compiler(s)   : Microsoft Visual C++
** Environment(s): Intel 386 and above
** Author        : Chris Leathley
**
** Description:
**
** This file provides the functionality for the Electro Mechanical Reel Display
**
** Contents:
**  DispReel()
**  ~DispReel()
**  Init()
**  SetDefaults()
**  InterfaceSupportsErrorInfo()
**
**  PreRender()
**  Render()
**
**	GetTimers()
**	GetHitShapes()
**	EndPlay()
**	RenderStatic()
**	RenderMovers()
**	RenderAnimation()
**	RenderText()
**
**	SetObjectPos()
**	MoveOffset()
**	GetCenter()
**	PutCenter()
**	SaveData()
**	InitLoad()
**	LoadToken()
**	InitPostLoad()
**
**	get_BackColor()					Access methods for properties (from VBS & Editor)
**	put_BackColor()
**	get_Reels()
**	put_Reels()
**  get_Width()
**	put_Width()
**  get_Height()
**	put_Height()
**  get_X()
**	put_X()
**  get_Y()
**	put_X()
**	get_IsTransparent()
**	put_IsTransparent
**	get_Image()
**	put_Image()
**	get_Spacing()
**	put_Spacing()
**	get_Sound()
**	put_Sound()
**	get_Steps()
**	put_Steps()
**	get_IsShading()
**	put_IsShading()
**	get_Type()
**	put_Type()
**	get_Font()
**	put_Font()
**	putref_Font()
**	get_FontColor()
**	put_FontColor()
**	get_ReelColor()
**	put_ReelColor()
**	get_Range()
**	put_Range()
**	get_UpdateInterval()
**	put_UpdateInterval()
**
**	AddValue()						Object control Methods (from VBS)
**	ResetToZero()
**	SpinReel()
**	SetValue()
**
**	getBoxWidth()					Private functions
**	getBoxHeight()
**	UpdateObjFrame()
**
** REVISION HISTORY:
** -----------------
**
** v0.0  15/11/01  CL   Created
** v0.1	 28/11/01  CL   Released to Randy
** v0.2	  1/12/01  CL   Changed ReelImage to use a horizontal strip to
**						allow creation of vertical strips greater than
**						1000 pixels (you cannot import a bitmap > 1000)
**
******************************************************************************/

#include "stdafx.h"
#include "main.h"

static const char REEL_NUMBER_TEXT[] = "01234567890";


// Constructor
//
DispReel::DispReel()
{
	m_pIFont = NULL;
	m_ptu = NULL;
}


// Destructor
//
DispReel::~DispReel()
{
	m_pIFont->Release();
}


// This function is called when ever a new instance of this object is created
// (along with the constructor (above))
//
HRESULT DispReel::Init(PinTable *ptable, float x, float y)
{
	m_ptable = ptable;

	SetDefaults();

	m_d.m_v1.x = x;
	m_d.m_v1.y = y;
	m_d.m_v2.x = x+getBoxWidth();
	m_d.m_v2.y = y+getBoxHeight();

	m_pobjframe = NULL;
    //m_preelframe = NULL;

	FONTDESC fd;

	fd.cbSizeofstruct = sizeof(FONTDESC);
	fd.lpstrName = L"Times New Roman";
	fd.cySize.int64 = 260000;
	//fd.cySize.Lo = 0;
	fd.sWeight = FW_BOLD;
	fd.sCharset = 0;
    fd.fItalic = 0;
	fd.fUnderline = 0;
	fd.fStrikethrough = 0;

	OleCreateFontIndirect(&fd, IID_IFont, (void **)&m_pIFont);

    return InitVBA(fTrue, 0, NULL);
}



// set the defaults for the objects persistant data (m_d.*) in case this
// is a new instance of this object of there is a backwards compatability
// issue (old version of object doesn't contain all the needed fields)
//
void DispReel::SetDefaults()
{
    // object is only available on the backglass
	m_fBackglass = fTrue;

    // set all the Data defaults
    m_d.m_reeltype = ReelText;
	m_d.m_szImage[0] = 0;
    m_d.m_szSound[0] = 0;
	m_d.m_fUseImageGrid = fFalse;
    m_d.m_imagesPerGridRow = 1;
	m_d.m_fTransparent = fFalse;
    m_d.m_reelcount = 5;
    m_d.m_width = 30;
    m_d.m_height = 40;
    m_d.m_reelspacing = 4;
    m_d.m_motorsteps = 2;
	m_d.m_digitrange = 9;
    m_d.m_fShading = fFalse;
    m_d.m_updateinterval = 50;

    m_d.m_backcolor = RGB(64,64,64);
    m_d.m_fontcolor = RGB(0,0,0);
    m_d.m_reelcolor = RGB(255,255,255);

	m_d.m_tdr.m_fTimerEnabled = fFalse;
	m_d.m_tdr.m_TimerInterval = 100;
}



STDMETHODIMP DispReel::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] =
	{
        &IID_IDispReel,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}



// this function draws the shape of the object with a solid fill
// only used in the editor and not the game
//
// this is called before the grid lines are drawn on the map
//
void DispReel::PreRender(Sur *psur)
{
    int     i;
    Vertex  rgv[4];
    float   x,y;

    psur->SetBorderColor(-1,fFalse,0);
	psur->SetFillColor(m_d.m_backcolor);
	psur->SetObject(this);

    // draw background box
    psur->Rectangle(m_d.m_v1.x, m_d.m_v1.y, m_d.m_v2.x, m_d.m_v2.y);

    // draw n reels in the box (in blue)
    psur->SetFillColor(m_d.m_reelcolor);
    for (i=0; i<m_d.m_reelcount; i++)
    {
        // set up top corner point
        x = m_d.m_v1.x + i*m_d.m_width + i*m_d.m_reelspacing + m_d.m_reelspacing;
        y = m_d.m_v1.y + m_d.m_reelspacing;

        // set up points (clockwise)
        rgv[0].x = x;
        rgv[0].y = y;
        rgv[1].x = x+m_d.m_width;
        rgv[1].y = y;
        rgv[2].x = x+m_d.m_width;
        rgv[2].y = y+m_d.m_height;
        rgv[3].x = x;
        rgv[3].y = y+m_d.m_height;
        psur->Polygon(rgv, 4);
    }
}



// this function draws the shape of the object with a black outline (no solid fill)
// only used in the editor and not the game
//
// this is called after the grid lines have been drawn on the map.  draws a solid
// outline over the grid lines
//
void DispReel::Render(Sur *psur)
{
	int     i;
    Vertex  rgv[4];
    float   x,y;

	psur->SetBorderColor(RGB(0,0,0),fFalse,0);
	psur->SetFillColor(-1);
	psur->SetObject(this);
	psur->SetObject(NULL);

    // draw background box
	psur->Rectangle(m_d.m_v1.x, m_d.m_v1.y, m_d.m_v2.x, m_d.m_v2.y);

    // draw n reels in the box
    for (i=0; i<m_d.m_reelcount; i++)
    {
        // set up top corner point
        x = m_d.m_v1.x + i*m_d.m_width + i*m_d.m_reelspacing + m_d.m_reelspacing;
        y = m_d.m_v1.y + m_d.m_reelspacing;

        // set up points (clockwise)
        rgv[0].x = x;
        rgv[0].y = y;
        rgv[1].x = x+m_d.m_width;
        rgv[1].y = y;
        rgv[2].x = x+m_d.m_width;
        rgv[2].y = y+m_d.m_height;
        rgv[3].x = x;
        rgv[3].y = y+m_d.m_height;
        psur->Polygon(rgv, 4);
    }
}



// Registers the timer with the game call which then makes a call back when the interval
// has expired.
//
// for this sort of object (reel driver) it is basically not really required but hey, somebody
// might use it..
//
void DispReel::GetTimers(Vector<HitTimer> *pvht)
{
	IEditable::BeginPlay();

	HitTimer *pht;
	pht = new HitTimer();
	pht->m_interval = m_d.m_tdr.m_TimerInterval;
	pht->m_nextfire = pht->m_interval;
	pht->m_pfe = (IFireEvents *)this;

	m_phittimer = pht;

	if (m_d.m_tdr.m_fTimerEnabled)
    {
        pvht->AddElement(pht);
    }
}



// This function is supposed to return the hit shapes for the object but since it is
// off screen we use it to register the screen updater in the game engine.. this means
// that Check3d (and Draw3d) are called in the updater class.
//
void DispReel::GetHitShapes(Vector<HitObject> *pvho)
{
    m_ptu = new DispReelUpdater(this);

	m_ptu->m_dispreelanim.m_znear = 0;
	m_ptu->m_dispreelanim.m_zfar = 0;

	// HACK - adding object directly to screen update list.  Someday make hit objects and screenupdaters seperate objects
	g_pplayer->m_vscreenupdate.AddElement(&m_ptu->m_dispreelanim);
}

void DispReel::GetHitShapesDebug(Vector<HitObject> *pvho)
	{
	}


// This method is called as the game exits..
// it cleans up any allocated memory used by the instace of the object
//
void DispReel::EndPlay()
{
    // free up object frame (buffer)
    if (m_pobjframe != NULL)    // Failed Player case
	{
		delete m_pobjframe;
		m_pobjframe = NULL;
	}

    // free up reel buffer
    if (m_vreelframe.Size() != 0)   // Failed Player case
	{
		while (m_vreelframe.Size() != 0)
			{
			delete m_vreelframe.ElementAt(0);
			m_vreelframe.RemoveElementAt(0);
			}
	}

	if (m_ptu)
	{
		delete m_ptu;
		m_ptu = NULL;
	}

	IEditable::EndPlay();
}

void DispReel::RenderStatic(LPDIRECT3DDEVICE7 pd3dDevice)
{

}


void DispReel::RenderMoversFromCache(Pin3D *ppin3d)
	{
	RenderMovers(ppin3d->m_pd3dDevice);
	/*m_pobjframe = new ObjFrame();
	ppin3d->ReadObjFrameFromCacheFile(m_pobjframe);*/
	}
	
// This function is called during the redering process // Old comments from RenderStatic
// (before the game starts, but after play has been pressed)
//
// it is designed to generate any static information/graphics for the table
// to use
//
// This function makes up a strip graphic from either the specified bitmap or
// generates one from the font specification..
//
// It then works out the correct scaling of all the items depending on the rendering
// screen size
//	
// This function is called during the redering process
// (before the game starts, but after play has been pressed)
//
// it is designed to generate any 3d animation as the 3D map has been set up
// to use in the process
//
void DispReel::RenderMovers(LPDIRECT3DDEVICE7 pd3dDevice)
{
    int      i;
    int      x1, x2;
    int      width, height;
    int      m_renderspacingx, m_renderspacingy;
    PinImage *pin;                              // pointer to image information from the image manager

    // set any defaults for the game rendering
    m_timenextupdate = g_pplayer->m_timeCur + m_d.m_updateinterval;
    m_fforceupdate = false;

    // get information about the table player (size sizes, resolution, etc..)
    Pin3D *ppin3d = &g_pplayer->m_pin3d;

    // allocate some object frames
    m_pobjframe  = new ObjFrame();

    if (m_pobjframe == NULL)
        return;

    // get the render sizes of the objects (reels and frame)
    m_renderwidth    = max(0, (int)((m_d.m_width / 1000) * ppin3d->m_dwRenderWidth));
    m_renderheight   = max(0, (int)((m_d.m_height / 750) * ppin3d->m_dwRenderHeight));
    m_renderspacingx = max(0, (int)((m_d.m_reelspacing / 1000) * ppin3d->m_dwRenderWidth));
    m_renderspacingy = max(0, (int)((m_d.m_reelspacing / 750) * ppin3d->m_dwRenderHeight));

    // get the size of the object frame (size of entire reel set and border)
	m_pobjframe->rc.left = (int)((m_d.m_v1.x / 1000) * ppin3d->m_dwRenderWidth);
	m_pobjframe->rc.top = (int)((m_d.m_v1.y / 750) * ppin3d->m_dwRenderHeight);
	// i cant use v2 as it really doesn't scale properly.
	m_pobjframe->rc.right = m_pobjframe->rc.left + ((int)m_d.m_reelcount * (m_renderwidth+m_renderspacingx)) + m_renderspacingx;
	m_pobjframe->rc.bottom = m_pobjframe->rc.top + m_renderheight + (2 * m_renderspacingy);

    // set the boundarys of the object frame (used for clipping I assume)
	m_ptu->m_dispreelanim.m_rcBounds = m_pobjframe->rc;

    // set up all the reel positions within the object frame
    x1 = m_renderspacingx;
    x2 = x1 + m_renderwidth;

    for (i=0; i<m_d.m_reelcount; i++)
    {
        ReelInfo[i].position.left	= x1/* + m_pobjframe->rc.left*/;
        ReelInfo[i].position.right	= x2/* + m_pobjframe->rc.left*/;
        ReelInfo[i].position.top	= m_renderspacingy/* + m_pobjframe->rc.top*/;
        ReelInfo[i].position.bottom	= m_renderspacingy + m_renderheight/* + m_pobjframe->rc.top*/;

        ReelInfo[i].currentValue	= 0;
        ReelInfo[i].motorPulses		= 0;
        ReelInfo[i].motorStepCount	= 0;
        ReelInfo[i].motorCalcStep	= 0;
        ReelInfo[i].motorOffset		= 0;

        // move to the next reel
        x1 += m_renderspacingx+m_renderwidth;
        x2 += m_renderspacingx+m_renderwidth;
    }

    // Set up the reel strip (either using bitmaps or fonts)
    if (m_d.m_reeltype == ReelImage)
    {
		// get a pointer to the image specified in the object
		pin = m_ptable->GetImage(m_d.m_szImage);

        // was there a valid image (if not then m_preelframe->pdds remains NULL and void)
        if (pin)
        {
            //HDC hdcImage;
			//HDC hdcReelFrame;
			int	GridCols, GridRows;

			// get the number of images per row of the image
			if (m_d.m_fUseImageGrid == fTrue)
			{
				GridCols = m_d.m_imagesPerGridRow;
				if (GridCols != 0) // best to be safe
				{
					GridRows = (int)(m_d.m_digitrange+1) / GridCols;
					if ( (GridRows * GridCols) < (int)(m_d.m_digitrange+1) )
					{
						GridRows++;
					}
				}
				else
				{
					GridRows = 1;
				}
			}
			else
			{
				GridCols = (int)(m_d.m_digitrange+1);
				GridRows = 1;
			}

			// save the color to use in any transparent blitting
			m_rgbImageTransparent = pin->m_rgbTransparent;

            // get the size of the individual reel digits (if m_digitrange is wrong we can forget the rest)
            m_reeldigitwidth  = pin->m_width / GridCols;
            m_reeldigitheight = pin->m_height / GridRows;

            // work out the size of the reel image strip (adds room for an extra digit at the end)
            width  = m_reeldigitwidth;
            height = (m_reeldigitheight * (int)(m_d.m_digitrange+1)) + m_reeldigitheight;
            // allocate some memory for this strip
			for (i=0; i <= m_d.m_digitrange; i++)
				{
				ObjFrame *pobjframe = new ObjFrame();
				pobjframe->pdds	= NULL;
				m_vreelframe.AddElement(pobjframe);
				if (pobjframe == NULL)
					{
					return;
					}
				}
            
            for (i=0; i<=(int)m_d.m_digitrange; i++)
				{
				m_vreelframe.ElementAt(i)->pdds = ppin3d->CreateOffscreenWithCustomTransparency(/*m_reeldigitwidth*/m_renderwidth, /*m_reeldigitheight*/m_renderheight, m_rgbImageTransparent);
				}

			int gr, gc;
			
			// now make the reel image strip..  It has to use the BitBlt function and not the direct draw
			// blt function (ie. m_preelframe->pdds->Blt) to copy the bitmaps as this function coverts the
			// destination bitmap to the same colour depth of the game (all images in the image manager
			// are stored as 32bit).

			// from this point on we can use the Direct Draw Blt function as it handles and scaling and
			// is much faster

			// get the HDC of the source image
			//pin->m_pdsBuffer->GetDC(&hdcImage);
			// get the HDC of the reel frame object
			//m_preelframe->pdds->GetDC(&hdcReelFrame);

			gr = gc = 0;
			
			// Render images and collect them
			
			// New rendering stuff
				{					
				ppin3d->SetMaterial(1, 1, 1, 0.5f);
				
				pin->EnsureMaxTextureCoordinates();
				
				pd3dDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);

				// Set texture to mirror, so the alpha state of the texture blends correctly to the outside
				pd3dDevice->SetTextureStageState( ePictureTexture, D3DTSS_ADDRESS, D3DTADDRESS_MIRROR);
				
				pin->EnsureColorKey();
				pd3dDevice->SetTexture(ePictureTexture, pin->m_pdsBufferColorKey);

				//pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP);
				//pd3dDevice->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, TRUE);
				ppin3d->SetColorKeyEnabled(fFalse);
				ppin3d->SetFiltersLinear();
				ppin3d->SetAlphaEnabled(fTrue);
				
				pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, TRUE);
				pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHAREF, 0xe0);
				pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATER);

				ppin3d->EnableLightMap(fFalse, -1);
				}
				
			WORD rgi[4];
			Vertex3D rgv3D[4];
			
			int l;
			for (l=0;l<4;l++)
				{
				rgi[l] = l;
				rgv3D[l].z = 1;//height + 0.2f;
				}
				
			RECT rectSrc;
			rectSrc.left = 0;
			rectSrc.top = 0;
			rectSrc.right = m_renderwidth;
			rectSrc.bottom = m_renderheight;
				
			rgv3D[0].x = 0;//(float)rectSrc.left;
			rgv3D[0].y = 0;//(float)rectSrc.top;
			rgv3D[0].tu = 0;
			rgv3D[0].tv = 0;

			rgv3D[1].x = m_d.m_width;//(((double)m_renderwidth)/ppin3d->m_dwRenderWidth)*1000;// m_d.m_width;//(float)rectSrc.right;
			rgv3D[1].y = 0;//(float)rectSrc.top;
			rgv3D[1].tu = pin->m_maxtu;
			rgv3D[1].tv = 0;

			rgv3D[2].x = rgv3D[1].x;//m_d.m_width;//(float)rectSrc.right;
			rgv3D[2].y = m_d.m_height;//(float)rectSrc.bottom;
			rgv3D[2].tu = pin->m_maxtu;
			rgv3D[2].tv = pin->m_maxtv;

			rgv3D[3].x = 0;//(float)rectSrc.left;
			rgv3D[3].y = m_d.m_height;//(float)rectSrc.bottom;
			rgv3D[3].tu = 0;
			rgv3D[3].tv = pin->m_maxtv;
			
			SetHUDVertices(rgv3D, 4);
			
			D3DMATERIAL7 mtrl;
			pd3dDevice->GetMaterial(&mtrl);
			SetDiffuseFromMaterial(rgv3D, 4, &mtrl);
			
			//ppin3d->ExpandExtents(&m_preelframe->rc, rgv3D, NULL, NULL, 4, m_fBackglass);

			// Reset color key in back buffer
			// this is usually not done since the buffer
			// should be clear from the last object,
			// but for caching, this object will draw
			// when others don't, so be safe.
			DDBLTFX ddbltfx;
			ddbltfx.dwSize = sizeof(DDBLTFX);
			ddbltfx.dwFillColor = m_rgbImageTransparent;
			ppin3d->m_pddsBackBuffer->Blt(&rectSrc, NULL,
					&rectSrc, DDBLT_COLORFILL, &ddbltfx);

			for (i=0; i<=(int)m_d.m_digitrange; i++)
			{
				rgv3D[0].tu = rgv3D[3].tu = (float)(((double)gc*m_reeldigitwidth)/pin->m_width) * pin->m_maxtu;
				rgv3D[0].tv = rgv3D[1].tv = (float)(((double)gr*m_reeldigitheight)/pin->m_height) * pin->m_maxtv;
			
				rgv3D[1].tu = rgv3D[2].tu = (float)(((double)(gc+1)*m_reeldigitwidth)/pin->m_width) * pin->m_maxtu;
				rgv3D[2].tv = rgv3D[3].tv = (float)(((double)(gr+1)*m_reeldigitheight)/pin->m_height) * pin->m_maxtv;
			
				pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, MY_D3DTRANSFORMED_VERTEX,
												  rgv3D, 4,
												  rgi, 4, NULL);
												  
				RECT rectDst;
				rectDst.left = 0;
				rectDst.top = 0;//m_renderheight*i;
				rectDst.right = m_renderwidth;
				rectDst.bottom = rectDst.top + m_renderheight;
												  
				m_vreelframe.ElementAt(i)->pdds->Blt(&rectDst, ppin3d->m_pddsBackBuffer, &rectSrc, 0, NULL);

				// Reset color key in back buffer
				DDBLTFX ddbltfx;
				ddbltfx.dwSize = sizeof(DDBLTFX);
				ddbltfx.dwFillColor = m_rgbImageTransparent;
				ppin3d->m_pddsBackBuffer->Blt(&rectSrc, NULL,
						&rectSrc, DDBLT_COLORFILL, &ddbltfx);
				/*BitBlt(hdcReelFrame,			// handle to destination device context
						0,						// x-coordinate of destination rectangle's upper-left corner
						i*m_reeldigitheight,	// y-coordinate of destination rectangle's upper-left corner
						m_reeldigitwidth,		// width of destination rectangle
						m_reeldigitheight,		// height of destination rectangle
						hdcImage,				// handle to source device context
						gc*m_reeldigitwidth,	// x-coordinate of source rectangle's upper-left corner
						gr*m_reeldigitheight,	// y-coordinate of source rectangle's upper-left corner
						SRCCOPY);				// raster operation code
				*/
				
				/*StretchBlt(hdcReelFrame,			// handle to destination device context
						0,						// x-coordinate of destination rectangle's upper-left corner
						i*m_renderheight,	// y-coordinate of destination rectangle's upper-left corner
						m_renderwidth,		// width of destination rectangle
						m_renderheight,		// height of destination rectangle
						hdcImage,				// handle to source device context
						gc*m_reeldigitwidth,	// x-coordinate of source rectangle's upper-left corner
						gr*m_reeldigitheight,	// y-coordinate of source rectangle's upper-left corner
						m_reeldigitwidth,
						m_reeldigitheight,
						SRCCOPY);				// raster operation code
				*/
				
				gc++;
				if (gc >= GridCols)
				{
					gc = 0;
					gr++;
				}
				
			if (i == m_d.m_digitrange)
				{
				// Go back and draw the first picture at the end of the strip
				gc = 0;
				gr = 0;
				}

			}

			// now copy the first digit graphic onto the end on the reel object frame
			// this means the bitmap starts and ends with the same graphic
			/*BitBlt(hdcReelFrame,
					0,
					m_reeldigitheight * (int)(m_d.m_digitrange+1),	// start after the last number
					m_reeldigitwidth,
					m_reeldigitheight,
					hdcImage,
					0,
					0,
					SRCCOPY);*/

			//m_preelframe->pdds->ReleaseDC(hdcReelFrame);
			//pin->m_pdsBuffer->ReleaseDC(hdcImage);
			
		pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, FALSE);
        
        // reset device 
        
        pd3dDevice->SetTexture(ePictureTexture, NULL);
		pd3dDevice->SetTextureStageState(ePictureTexture, D3DTSS_MAGFILTER, D3DTFG_LINEAR);
		pd3dDevice->SetTextureStageState(ePictureTexture, D3DTSS_MINFILTER, D3DTFN_LINEAR);
		pd3dDevice->SetTextureStageState(ePictureTexture, D3DTSS_MIPFILTER, D3DTFP_LINEAR);

		pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
		pd3dDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
		pd3dDevice->SetTextureStageState( ePictureTexture, D3DTSS_ADDRESS, D3DTADDRESS_WRAP);
        }
    }
    else    /* generate a strip of numbers using font rendering */
	{
        HDC     hdc;
        RECT	rcOut;
        int     length;
        HFONT   hFont;
        int     maxwidth, maxheight;

        // text reels are purely 0-9 and nothing else
        m_d.m_digitrange = 9;

        // make a clone of the specified font
        m_pIFont->Clone(&m_pIFontPlay);
        // scale the font (either up or down) to suit the screen resolution
        CY size;
        m_pIFontPlay->get_Size(&size);
        size.int64 = size.int64 / 1000 * ppin3d->m_dwRenderWidth;
        m_pIFontPlay->put_Size(size);

        m_pIFontPlay->get_hFont(&hFont);
	    hdc = GetDC(NULL);
        SelectObject(hdc, hFont);
        SetTextAlign(hdc, TA_LEFT | TA_TOP | TA_NOUPDATECP);

        // work out the maximum width and height for the selected font
        maxwidth = m_renderwidth;
        maxheight = m_renderheight;
        length = lstrlen(REEL_NUMBER_TEXT);
        for (i=0; i<length; i++)
        {
            rcOut.left = 0;
            rcOut.top = 0;
            rcOut.right = maxwidth;
            rcOut.bottom = maxwidth;
            DrawText(hdc, &REEL_NUMBER_TEXT[i], 1, &rcOut, DT_NOCLIP | DT_NOPREFIX | DT_CALCRECT);
            maxwidth = max(maxwidth, rcOut.right);
            maxheight = max(maxheight, rcOut.bottom);
        }
        ReleaseDC(NULL, hdc);

        // set the size of the individual reel digits
        m_reeldigitwidth  = maxwidth;
        m_reeldigitheight = maxheight;
        
		for (i=0; i < length; i++)
			{
			ObjFrame *pobjframe = new ObjFrame();
			pobjframe->pdds	= NULL;
			m_vreelframe.AddElement(pobjframe);
			if (pobjframe == NULL)
				{
				return;
				}
			}

        // allocate some memory for this strip
		for (i=0; i<length; i++)
			{
			m_vreelframe.ElementAt(i)->pdds = ppin3d->CreateOffscreen(maxwidth, maxheight);
			}

		SetTextColor(hdc, m_d.m_fontcolor);		// set the font colour
		SetBkMode(hdc, TRANSPARENT);
		for (i=0; i<length; i++)
        {
			// fill the strip with the reel colour
			m_vreelframe.ElementAt(i)->pdds->GetDC(&hdc);
			HBRUSH hbrush = CreateSolidBrush(m_d.m_reelcolor);
			HBRUSH hbrushold = (HBRUSH)SelectObject(hdc, hbrush);
			PatBlt(hdc, 0, 0, maxwidth, maxheight, PATCOPY);
			SelectObject(hdc, hbrushold);
			DeleteObject(hbrush);
			
			// set the font plotting parameters
			SelectObject(hdc, hFont);
			SetTextAlign(hdc, TA_LEFT | TA_TOP | TA_NOUPDATECP);
        
            rcOut.left = 0;
            rcOut.top = 0;//i * maxheight;
            rcOut.right = maxwidth;
            rcOut.bottom = rcOut.top + maxheight;
            DrawText(hdc, (char *)&REEL_NUMBER_TEXT[i], 1, &rcOut, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX);
            m_vreelframe.ElementAt(i)->pdds->ReleaseDC(hdc);
		}
		m_pIFontPlay->Release();
		
		// For text, try to pick a color that won't be used (hack)
		m_rgbImageTransparent = 0xff;
		if ((m_d.m_fontcolor & 0xffffff) == m_rgbImageTransparent)
			{
			m_rgbImageTransparent = 0xffff;
			}
			
		if ((m_d.m_reelcolor & 0xffffff) == m_rgbImageTransparent)
			{
			m_rgbImageTransparent = 0xff00;
			}
			
		if ((m_d.m_fontcolor & 0xffffff) == m_rgbImageTransparent)
			{
			m_rgbImageTransparent = 0xff0000;
			}
	}
	
	// allocate the memory for this object (returns with a LPDIRECTDRAWSURFACE7)
	m_pobjframe->pdds = ppin3d->CreateOffscreenWithCustomTransparency(m_pobjframe->rc.right - m_pobjframe->rc.left, m_pobjframe->rc.bottom - m_pobjframe->rc.top, m_rgbImageTransparent);
	
	//ppin3d->WriteObjFrameToCacheFile(m_pobjframe);
}



// This function is called during Check3D.  It basically check to see if the update
// interval has expired and if so handles the rolling of the reels according to the
// number of motor steps queued up for each reel
//
// if a screen update is required it returns true..
//
bool DispReel::RenderAnimation()
{
    int     i;
    bool    rc = false;
    float   step;
    int     OverflowValue, AdjustValue;
    OLECHAR mySound[256];

    if (g_pplayer->m_timeCur >= m_timenextupdate)
	{
        m_timenextupdate = g_pplayer->m_timeCur + m_d.m_updateinterval;

        // work out the roll over values
        OverflowValue = (int)m_d.m_digitrange;
        AdjustValue   = OverflowValue+1;

        // start at the last reel and work forwards (right to left)
        for (i=(int)m_d.m_reelcount-1; i>=0; i--)
        {
            // if the motor has stoped, and there are still motor steps then start another one
            if ((ReelInfo[i].motorPulses != 0) && (ReelInfo[i].motorStepCount == 0))
            {
                // get the number of steps (or increments) needed to move the reel
				step = m_reeldigitheight / m_d.m_motorsteps;

                ReelInfo[i].motorStepCount = (int)m_d.m_motorsteps;

				if (ReelInfo[i].motorPulses > 0)
				{
	                ReelInfo[i].motorCalcStep = step;
	                ReelInfo[i].motorOffset = 0;
				}
				else
				{
	                ReelInfo[i].motorCalcStep = -step;
	                ReelInfo[i].motorOffset = 0;
				}

                // play the sound (if any) for each click of the reel
				if (m_d.m_szSound[0] != 0)
				{
	               MultiByteToWideChar(CP_ACP, 0, m_d.m_szSound, -1, mySound, 32);
		           m_ptable->PlaySound(mySound, 0, 1.0f);
				}
            }

            // is the reel in the process of moving??
            if (ReelInfo[i].motorStepCount != 0)
            {
                ReelInfo[i].motorOffset += ReelInfo[i].motorCalcStep;
                ReelInfo[i].motorStepCount -= 1;
                // have re reached the end of the step
                if (ReelInfo[i].motorStepCount <= 0)
                {
					ReelInfo[i].motorStepCount = 0;      // best to be safe (paranoid)
					ReelInfo[i].motorOffset = 0;

					if (ReelInfo[i].motorPulses < 0)
					{
						ReelInfo[i].motorPulses	 += 1;
					    ReelInfo[i].currentValue -= 1;
				        if (ReelInfo[i].currentValue < 0)
						{
		                    ReelInfo[i].currentValue += AdjustValue;
			                // if not the first reel then decrement the next reel by 1
					        if (i != 0)
						    {
			                    ReelInfo[i-1].motorPulses -= 1;
							}
						}
					}
					else
					{
						ReelInfo[i].motorPulses  -= 1;
					    ReelInfo[i].currentValue += 1;
				        if (ReelInfo[i].currentValue > OverflowValue)
						{
		                    ReelInfo[i].currentValue -= AdjustValue;
			                // if not the first reel then increment the next reel
							// along by 1 (just like a car odometer)
					        if (i != 0)
						    {
			                    ReelInfo[i-1].motorPulses += 1;
							}
						}
					}
				}
				// there is a change in the animation, redraw the frame
				rc = true;
            }
        }

        // if there is a change or we are forced to update, then do so..
        if ((rc == true) || (m_fforceupdate == true))
        {
			// redraw the reels (and boarder) into the objectframe
			RenderText();
            m_fforceupdate = false;
            rc = true;
		}
	}

	return(rc);
}



// This function gets calls just before the game starts to draw the first instance of
// the object on the screen.  it is not called after that.  Check3D and Draw3D handle
// any dynamic updates.
//
void DispReel::RenderText()
{
	Pin3D	*ppin3d = &g_pplayer->m_pin3d;

    // update the object frame (or in this case, draw it for the first time)
    UpdateObjFrame();

    // copy the object frame onto the back buffer
    m_ptu->m_dispreelanim.Draw3D(NULL);
    //ppin3d->m_pddsBackBuffer->Blt(&m_pobjframe->rc, m_pobjframe->pdds, NULL, DDBLTFAST_SRCCOLORKEY/*DDBLTFAST_WAIT*/, NULL);
}



void DispReel::SetObjectPos()
{
	g_pvp->SetObjectPosCur(m_d.m_v1.x, m_d.m_v1.y);
}



void DispReel::MoveOffset(float dx, float dy)
{
	m_d.m_v1.x += dx;
	m_d.m_v1.y += dy;

	m_d.m_v2.x += dx;
	m_d.m_v2.y += dy;

	m_ptable->SetDirtyDraw();
}



void DispReel::GetCenter(Vertex *pv)
{
}



void DispReel::PutCenter(Vertex *pv)
{
}



HRESULT DispReel::SaveData(IStream *pstm, HCRYPTHASH hcrypthash, HCRYPTKEY hcryptkey)
{
	BiffWriter bw(pstm, hcrypthash, hcryptkey);

#ifdef VBA
	bw.WriteInt(FID(PIID), ApcProjectItem.ID());
#endif
	bw.WriteStruct(FID(VER1), &m_d.m_v1, sizeof(Vertex));
	bw.WriteStruct(FID(VER2), &m_d.m_v2, sizeof(Vertex));
    bw.WriteInt(FID(TYPE), m_d.m_reeltype);
	bw.WriteInt(FID(CLRB), m_d.m_backcolor);
	bw.WriteInt(FID(CLRF), m_d.m_fontcolor);
    bw.WriteInt(FID(CLRR),  m_d.m_reelcolor);
    bw.WriteBool(FID(TMON), m_d.m_tdr.m_fTimerEnabled);
	bw.WriteInt(FID(TMIN), m_d.m_tdr.m_TimerInterval);
	bw.WriteBool(FID(TRNS), m_d.m_fTransparent);
	bw.WriteString(FID(IMAG), m_d.m_szImage);
    bw.WriteString(FID(SOUN), m_d.m_szSound);
	bw.WriteWideString(FID(NAME), (WCHAR *)m_wzName);
	bw.WriteFloat(FID(WDTH), m_d.m_width);
	bw.WriteFloat(FID(HIGH), m_d.m_height);
    bw.WriteFloat(FID(RCNT), m_d.m_reelcount);
    bw.WriteFloat(FID(RSPC), m_d.m_reelspacing);
    bw.WriteFloat(FID(MSTP), m_d.m_motorsteps);
    bw.WriteBool(FID(SHAD), m_d.m_fShading);
    bw.WriteFloat(FID(RANG), m_d.m_digitrange);
    bw.WriteInt(FID(UPTM), m_d.m_updateinterval);
    bw.WriteBool(FID(UGRD), m_d.m_fUseImageGrid);
    bw.WriteInt(FID(GIPR), m_d.m_imagesPerGridRow);

	bw.WriteTag(FID(FONT));
	IPersistStream * ips;
	m_pIFont->QueryInterface(IID_IPersistStream, (void **)&ips);
	ips->Save(pstm, TRUE);

	bw.WriteTag(FID(ENDB));

	return S_OK;
}



HRESULT DispReel::InitLoad(IStream *pstm, PinTable *ptable, int *pid, int version, HCRYPTHASH hcrypthash, HCRYPTKEY hcryptkey)
{
	SetDefaults();
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

    if(FAILED(hr = pstm->Read(&m_d, sizeof(DispReelData), &read)))
		return hr;

	FONTDESC fd;
	fd.cbSizeofstruct = sizeof(FONTDESC);
	fd.lpstrName = L"Times New Roman";
	fd.cySize.int64 = 260000;
	//fd.cySize.Lo = 0;
	fd.sWeight = FW_BOLD;
	fd.sCharset = 0;
    fd.fItalic = 0;
	fd.fUnderline = 0;
	fd.fStrikethrough = 0;
	OleCreateFontIndirect(&fd, IID_IFont, (void **)&m_pIFont);

	IPersistStream * ips;
	m_pIFont->QueryInterface(IID_IPersistStream, (void **)&ips);

	ips->Load(pstm);

	//ApcProjectItem.Register(ptable->ApcProject, GetDispatch(), dwID);
	*pid = dwID;

	return hr;
#endif
}



BOOL DispReel::LoadToken(int id, BiffReader *pbr)
{
	if (id == FID(PIID))
	{
		pbr->GetInt((int *)pbr->m_pdata);
	}
	else if (id == FID(VER1))
		{
			pbr->GetStruct(&m_d.m_v1, sizeof(Vertex));
		}
	else if (id == FID(VER2))
		{
			pbr->GetStruct(&m_d.m_v2, sizeof(Vertex));
		}
	else if (id == FID(WDTH))
		{
			pbr->GetFloat(&m_d.m_width);
		}
	else if (id == FID(HIGH))
		{
			pbr->GetFloat(&m_d.m_height);
		}
	else if (id == FID(CLRB))
		{
			pbr->GetInt(&m_d.m_backcolor);
		}
	else if (id == FID(CLRF))
		{
			pbr->GetInt(&m_d.m_fontcolor);
		}
    else if (id == FID(CLRR))
		{
			pbr->GetInt(&m_d.m_reelcolor);
		}
	else if (id == FID(TMON))
		{
			pbr->GetBool(&m_d.m_tdr.m_fTimerEnabled);
		}
	else if (id == FID(TMIN))
		{
			pbr->GetInt(&m_d.m_tdr.m_TimerInterval);
		}
	else if (id == FID(NAME))
		{
			pbr->GetWideString((WCHAR *)m_wzName);
		}
	else if (id == FID(TRNS))
		{
			pbr->GetBool(&m_d.m_fTransparent);
		}
	else if (id == FID(IMAG))
		{
			pbr->GetString(m_d.m_szImage);
		}
    else if (id == FID(RCNT))
		{
			pbr->GetFloat(&m_d.m_reelcount);
		}
    else if (id == FID(RSPC))
		{
			pbr->GetFloat(&m_d.m_reelspacing);
		}
    else if (id == FID(MSTP))
		{
			pbr->GetFloat(&m_d.m_motorsteps);
		}
    else if (id == FID(SOUN))
		{
			pbr->GetString(m_d.m_szSound);
		}
    else if (id == FID(SHAD))
		{
			pbr->GetBool(&m_d.m_fShading);
		}
	else if (id == FID(TYPE))
		{
			pbr->GetInt(&m_d.m_reeltype);
		}
	else if (id == FID(UGRD))
		{
			pbr->GetBool(&m_d.m_fUseImageGrid);
		}
	else if (id == FID(GIPR))
		{
   			pbr->GetInt(&m_d.m_imagesPerGridRow);
		}
	else if (id == FID(RANG))
		{
            pbr->GetFloat(&m_d.m_digitrange);
		}
    else if (id == FID(UPTM))
		{
            pbr->GetInt(&m_d.m_updateinterval);
		}
	else if (id == FID(FONT))
		{
		if (!m_pIFont)
			{
				FONTDESC fd;
				fd.cbSizeofstruct = sizeof(FONTDESC);
				fd.lpstrName = L"Times New Roman";
				fd.cySize.int64 = 260000;
				//fd.cySize.Lo = 0;
				fd.sWeight = FW_BOLD;
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
	return fTrue;
}



HRESULT DispReel::InitPostLoad()
{
	m_pobjframe = NULL;

	return S_OK;
}

void DispReel::GetDialogPanes(Vector<PropertyPane> *pvproppane)
	{
	PropertyPane *pproppane;

	pproppane = new PropertyPane(IDD_PROP_NAME, NULL);
	pvproppane->AddElement(pproppane);

	pproppane = new PropertyPane(IDD_PROPDISPREEL_VISUALS, IDS_VISUALS);
	pvproppane->AddElement(pproppane);

	pproppane = new PropertyPane(IDD_PROPDISPREEL_POSITION, IDS_POSITION);
	pvproppane->AddElement(pproppane);

	pproppane = new PropertyPane(IDD_PROPDISPREEL_STATE, IDS_STATE);
	pvproppane->AddElement(pproppane);

	pproppane = new PropertyPane(IDD_PROP_TIMER, IDS_MISC);
	pvproppane->AddElement(pproppane);
	}

// These methods provide the interface to the object through both the editor
// and the script for a of the object properties
//
STDMETHODIMP DispReel::get_BackColor(OLE_COLOR *pVal)
{
	*pVal = m_d.m_backcolor;

	return S_OK;
}

STDMETHODIMP DispReel::put_BackColor(OLE_COLOR newVal)
{
	STARTUNDO
	m_d.m_backcolor = newVal;
	STOPUNDO

	return S_OK;
}

STDMETHODIMP DispReel::get_Reels(float *pVal)
{
    *pVal = m_d.m_reelcount;

    return S_OK;
}

STDMETHODIMP DispReel::put_Reels(float newVal)
{
	STARTUNDO
    m_d.m_reelcount = max(1, newVal);               // must have at least 1 reel
    if (m_d.m_reelcount > MAX_REELS) m_d.m_reelcount = MAX_REELS;   // and a max of MAX_REELS
	m_d.m_v2.x = m_d.m_v1.x+getBoxWidth();
	m_d.m_v2.y = m_d.m_v1.y+getBoxHeight();
	STOPUNDO

    return S_OK;
}

STDMETHODIMP DispReel::get_Width(float *pVal)
{
	*pVal = m_d.m_width;

	return S_OK;
}

STDMETHODIMP DispReel::put_Width(float newVal)
{
	STARTUNDO
    m_d.m_width = max(0, newVal);
	m_d.m_v2.x = m_d.m_v1.x+getBoxWidth();
	STOPUNDO

	return S_OK;
}

STDMETHODIMP DispReel::get_Height(float *pVal)
{
	*pVal = m_d.m_height;

	return S_OK;
}

STDMETHODIMP DispReel::put_Height(float newVal)
{
	STARTUNDO
    m_d.m_height = max(0, newVal);
	m_d.m_v2.y = m_d.m_v1.y+getBoxHeight();
	STOPUNDO

	return S_OK;
}

STDMETHODIMP DispReel::get_X(float *pVal)
{
	*pVal = m_d.m_v1.x;

	return S_OK;
}

STDMETHODIMP DispReel::put_X(float newVal)
{
	STARTUNDO
	float delta = newVal - m_d.m_v1.x;
	m_d.m_v1.x += delta;
	m_d.m_v2.x = m_d.m_v1.x+getBoxWidth();
	STOPUNDO

	return S_OK;
}

STDMETHODIMP DispReel::get_Y(float *pVal)
{
	*pVal = m_d.m_v1.y;

	return S_OK;
}

STDMETHODIMP DispReel::put_Y(float newVal)
{
	STARTUNDO

	float delta = newVal - m_d.m_v1.y;
	m_d.m_v1.y += delta;
	m_d.m_v2.y = m_d.m_v1.y+getBoxHeight();

	STOPUNDO

	return S_OK;
}

STDMETHODIMP DispReel::get_IsTransparent(VARIANT_BOOL *pVal)
{
	*pVal = FTOVB(m_d.m_fTransparent);

	return S_OK;
}

STDMETHODIMP DispReel::put_IsTransparent(VARIANT_BOOL newVal)
{
	STARTUNDO
	m_d.m_fTransparent = VBTOF(newVal);
	STOPUNDO

	return S_OK;
}

STDMETHODIMP DispReel::get_Image(BSTR *pVal)
{
	OLECHAR wz[512];

    MultiByteToWideChar(CP_ACP, 0, m_d.m_szImage, -1, wz, 32);
	*pVal = SysAllocString(wz);

	return S_OK;
}

STDMETHODIMP DispReel::put_Image(BSTR newVal)
{
	STARTUNDO
	WideCharToMultiByte(CP_ACP, 0, newVal, -1, m_d.m_szImage, 32, NULL, NULL);
	STOPUNDO

	return S_OK;
}

STDMETHODIMP DispReel::get_Spacing(float *pVal)
{
    *pVal = m_d.m_reelspacing;
    return S_OK;
}

STDMETHODIMP DispReel::put_Spacing(float newVal)
{
	STARTUNDO
    m_d.m_reelspacing = max(0, newVal);
	m_d.m_v2.x = m_d.m_v1.x+getBoxWidth();
	m_d.m_v2.y = m_d.m_v1.y+getBoxHeight();
	STOPUNDO
	return S_OK;
}

STDMETHODIMP DispReel::get_Sound(BSTR *pVal)
{
	OLECHAR wz[512];

    MultiByteToWideChar(CP_ACP, 0, m_d.m_szSound, -1, wz, 32);
	*pVal = SysAllocString(wz);

	return S_OK;
}

STDMETHODIMP DispReel::put_Sound(BSTR newVal)
{
	STARTUNDO
    WideCharToMultiByte(CP_ACP, 0, newVal, -1, m_d.m_szSound, 32, NULL, NULL);
	STOPUNDO

	return S_OK;
}

STDMETHODIMP DispReel::get_Steps(float *pVal)
{
    *pVal = (float)((int)m_d.m_motorsteps);

    return S_OK;
}

STDMETHODIMP DispReel::put_Steps(float newVal)
{
	STARTUNDO
    m_d.m_motorsteps = (float)((int)max(1, newVal));	// must have at least 1 step
	STOPUNDO

    return S_OK;
}

STDMETHODIMP DispReel::get_IsShading(VARIANT_BOOL *pVal)
{
    *pVal = FTOVB(m_d.m_fShading);

	return S_OK;
}

STDMETHODIMP DispReel::put_IsShading(VARIANT_BOOL newVal)
{
	STARTUNDO
    m_d.m_fShading = VBTOF(newVal);
	STOPUNDO

	return S_OK;
}

STDMETHODIMP DispReel::get_Type(ReelType *pVal)
{
    *pVal = m_d.m_reeltype;

	return S_OK;
}

STDMETHODIMP DispReel::put_Type(ReelType newVal)
{
	STARTUNDO
    m_d.m_reeltype = newVal;
	STOPUNDO

	return S_OK;
}

STDMETHODIMP DispReel::get_Font(IFontDisp **pVal)
{
	m_pIFont->QueryInterface(IID_IFontDisp, (void **)pVal);

	return S_OK;
}

STDMETHODIMP DispReel::put_Font(IFontDisp *newVal)
{
	// Does anybody use this way of setting the font?  Need to add to idl file.
	return S_OK;
}

STDMETHODIMP DispReel::putref_Font(IFontDisp* pFont)
{
	//We know that our own property browser gives us the same pointer

	//m_pIFont->Release();
	//pFont->QueryInterface(IID_IFont, (void **)&m_pIFont);

	SetDirtyDraw();
    return S_OK;
}

STDMETHODIMP DispReel::get_FontColor(OLE_COLOR *pVal)
{
	*pVal = m_d.m_fontcolor;

	return S_OK;
}

STDMETHODIMP DispReel::put_FontColor(OLE_COLOR newVal)
{
	STARTUNDO
	m_d.m_fontcolor = newVal;
	STOPUNDO

	return S_OK;
}

STDMETHODIMP DispReel::get_ReelColor(OLE_COLOR *pVal)
{
    *pVal = m_d.m_reelcolor;

	return S_OK;
}

STDMETHODIMP DispReel::put_ReelColor(OLE_COLOR newVal)
{
	STARTUNDO
    m_d.m_reelcolor = newVal;
	STOPUNDO

	return S_OK;
}

STDMETHODIMP DispReel::get_Range(float *pVal)
{
    *pVal = (float)((int)m_d.m_digitrange);

    return S_OK;
}

STDMETHODIMP DispReel::put_Range(float newVal)
{
	STARTUNDO
    m_d.m_digitrange = (float)((int)max(0, newVal));        // must have at least 1 digit (0 is a digit)
    if (m_d.m_digitrange >= 200) m_d.m_digitrange = 200-1;  // and a max of 200 (0->199)
	STOPUNDO
	return S_OK;
}

STDMETHODIMP DispReel::get_UpdateInterval(long *pVal)
{
    *pVal = m_d.m_updateinterval;

    return S_OK;
}

STDMETHODIMP DispReel::put_UpdateInterval(long newVal)
{
	STARTUNDO
    m_d.m_updateinterval = max(5, newVal);
	if (g_pplayer)
    {
        m_timenextupdate = g_pplayer->m_timeCur + m_d.m_updateinterval;
    }
	STOPUNDO
	return S_OK;
}

STDMETHODIMP DispReel::get_UseImageGrid(VARIANT_BOOL *pVal)
{
    *pVal = FTOVB(m_d.m_fUseImageGrid);

	return S_OK;
}

STDMETHODIMP DispReel::put_UseImageGrid(VARIANT_BOOL newVal)
{
	STARTUNDO
    m_d.m_fUseImageGrid = VBTOF(newVal);
	STOPUNDO

	return S_OK;
}
STDMETHODIMP DispReel::get_ImagesPerGridRow(long *pVal)
{
    *pVal = m_d.m_imagesPerGridRow;

    return S_OK;
}

STDMETHODIMP DispReel::put_ImagesPerGridRow(long newVal)
{
	STARTUNDO
    m_d.m_imagesPerGridRow = max(1, newVal);
	STOPUNDO
	return S_OK;
}



// function Methods available for the scripters.
//
STDMETHODIMP DispReel::AddValue(long Value)
{
	long	val, valbase;
	int		i;
	int		digitValue;
	BOOL	bNegative;

	if (Value < 0)
	{
		bNegative = fTrue;
	}
	else
	{
		bNegative = fFalse;
	}

	// ensure a positive number
	val = labs(Value);

	// get the base of this reel
	valbase = (int)(m_d.m_digitrange+1);

	// start at the right most reel and move left
	i = (int)m_d.m_reelcount-1;
	while ( (val != 0) && (i >= 0) )
	{
		digitValue = val % valbase;

		if (bNegative == fTrue)
		{
			ReelInfo[i].motorPulses -= digitValue;
		}
		else
		{
			ReelInfo[i].motorPulses += digitValue;
		}
		// remove the value for this reel from the overall number
		val = val / valbase;
		// move to next reel
		i--;
	}

	return S_OK;
}


STDMETHODIMP DispReel::SetValue(long Value)
{
	long	val, valbase;
	int		i;
	int		digitValue;

	// ensure a positive number
	val = labs(Value);

	// get the base of this reel
	valbase = (int)(m_d.m_digitrange+1);

    // reset the motor
    for (i=0; i<m_d.m_reelcount; i++)
    {
		ReelInfo[i].currentValue	= 0;
        ReelInfo[i].motorPulses 	= 0;
        ReelInfo[i].motorStepCount	= 0;
        ReelInfo[i].motorCalcStep	= 0;
        ReelInfo[i].motorOffset		= 0;
	}

    // set the reel values (startint at the right most reel and move left)
	i = (int)m_d.m_reelcount-1;
	while ( (val != 0) && (i >= 0) )
	{
		digitValue = val % valbase;
        ReelInfo[i].currentValue = digitValue;
		// remove the value for this reel from the overall number
		val = val / valbase;
		// move to next reel
		i--;
    }

    // force a immediate screen update
    m_fforceupdate = true;
	m_timenextupdate = g_pplayer->m_timeCur;

    return S_OK;
}


STDMETHODIMP DispReel::ResetToZero(void)
{
    int i;
    int carry;
    int adjust;
    int overflowValue;

    carry = 0;
    overflowValue = (int)m_d.m_digitrange+1;

    // work for the last reel to the first one
    for (i=(int)m_d.m_reelcount-1; i>=0; i--)
    {
		adjust = overflowValue - carry;
        carry = 0;

        adjust -= ReelInfo[i].currentValue;

		if (adjust != overflowValue)
		{
			// overwrite the pulse count with the adjust value
			ReelInfo[i].motorPulses = adjust;
			// as this reel returns to zero it will roll over the next reel along
			carry = 1;
		}
    }

	return S_OK;
}


STDMETHODIMP DispReel::SpinReel(long ReelNumber, long PulseCount)
{
	if ( (ReelNumber >= 1) && (ReelNumber <= m_d.m_reelcount) )
	{
		int reel = (int)ReelNumber-1;
		ReelInfo[reel].motorPulses += (int)PulseCount;
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}


// Private functions
//
float DispReel::getBoxWidth(void)
{
    float width;

    width =  m_d.m_reelcount * m_d.m_width;
    width += m_d.m_reelcount * m_d.m_reelspacing;
    width += m_d.m_reelspacing;				// spacing also includes edges

    return (width);
}


float DispReel::getBoxHeight(void)
{
    float height;

    height =  m_d.m_height;
    height += 2 * m_d.m_reelspacing;		// spacing also includes edges

    return (height);
}


// This function renders the in-game object which is then drawn onto the screen (in Draw3D)
//
// It firsts draws the object box (either solid or transparent) and draws the reels within it
//
void DispReel::UpdateObjFrame(void)
{
    int     i;
    RECT    reelstriprc;
	Pin3D	*ppin3d = &g_pplayer->m_pin3d;
	DDBLTFX	bltFx;
	DWORD	flags;
	
	LPDIRECTDRAWSURFACE7 pdds = g_pplayer->m_pin3d.m_pddsBackBuffer;

    // is the background box transparent?
#define Foo 1
#ifdef Foo
    if (m_d.m_fTransparent == TRUE)
    {
        // yes, then copy the current backgrount into the object frame
        m_pobjframe->pdds->Blt(NULL, ppin3d->m_pddsStatic, &m_pobjframe->rc, DDBLTFAST_WAIT, NULL);
    }
    else
    {
        // nope, fill the box with the background colour
        HDC hdc;

        m_pobjframe->pdds->GetDC(&hdc);
        HBRUSH hbrush = CreateSolidBrush(m_d.m_backcolor);
        HBRUSH hbrushold = (HBRUSH)SelectObject(hdc, hbrush);
        PatBlt(hdc, 0, 0, m_pobjframe->rc.right - m_pobjframe->rc.left, m_pobjframe->rc.bottom - m_pobjframe->rc.top, PATCOPY);
        SelectObject(hdc, hbrushold);
        DeleteObject(hbrush);
        m_pobjframe->pdds->ReleaseDC(hdc);
    }
#endif

    // render the reels onto the screen (providing the reel generation worked)

    if (m_pobjframe->pdds && (m_vreelframe.Size() > 0))
    {
		memset(&bltFx, 0x00, sizeof(bltFx));
		bltFx.dwSize = sizeof(bltFx);
		flags = DDBLTFAST_WAIT;

		if (m_d.m_reeltype == ReelImage)
		{
			bltFx.ddckSrcColorkey.dwColorSpaceLowValue  = m_rgbImageTransparent;
			bltFx.ddckSrcColorkey.dwColorSpaceHighValue = m_rgbImageTransparent;
			flags |= DDBLT_KEYSRCOVERRIDE;
		}

        reelstriprc.left  = 0;
	    reelstriprc.right = m_renderwidth;//m_reeldigitwidth;
        for (i=0; i<m_d.m_reelcount; i++)
        {
            reelstriprc.top = /*(ReelInfo[i].currentValue * m_renderheight) +*/ (int)(ReelInfo[i].motorOffset);
			if (reelstriprc.top < 0)
			{
				reelstriprc.top += m_renderheight/*m_reeldigitheight*/ * (int)(m_d.m_digitrange+1);
			}
			reelstriprc.bottom = /*reelstriprc.top +*/ m_renderheight/*m_reeldigitheight*/;

    		// Set the color key for this bitmap (black)

//#define FOO2
#ifdef FOO2
            m_pobjframe->pdds->Blt(&ReelInfo[i].position,   // destination rectangle
                                   m_preelframe->pdds,      // source image (LPDIRECTDRAWSURFACE7)
                                   &reelstriprc,            // source rectangle
                                   flags,
                                   &bltFx);            // no blit effects ;-(
#else
			m_pobjframe->pdds->BltFast(ReelInfo[i].position.left, ReelInfo[i].position.top, m_vreelframe.ElementAt(ReelInfo[i].currentValue)->pdds,
						&reelstriprc, DDBLTFAST_NOCOLORKEY/*DDBLTFAST_SRCCOLORKEY*/);
#endif FOO2

			if (ReelInfo[i].motorOffset != 0)
				{
				int nextval = (ReelInfo[i].currentValue + 1) % m_vreelframe.Size();
				int top = ReelInfo[i].position.top + (reelstriprc.bottom - reelstriprc.top);
				reelstriprc.top = 0;
				reelstriprc.bottom = (int)ReelInfo[i].motorOffset;
				
				m_pobjframe->pdds->BltFast(ReelInfo[i].position.left, top, m_vreelframe.ElementAt(nextval)->pdds,
						&reelstriprc, DDBLTFAST_NOCOLORKEY/*DDBLTFAST_SRCCOLORKEY*/);
				}
        }
    }
    // objframe is now upto date
}

