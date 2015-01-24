#include "StdAfx.h"
#include "meshes/gateBracketMesh.h"
#include "meshes/gateWireMesh.h"

Gate::Gate()
{
   m_phitgate = NULL;
   m_plineseg = NULL;
   bracketIndexBuffer=NULL;
   bracketVertexBuffer=NULL;
   wireIndexBuffer=NULL;
   wireVertexBuffer=NULL;
}

Gate::~Gate()
{
    if (bracketVertexBuffer)
    {
        bracketVertexBuffer->release();
        bracketVertexBuffer = NULL;
    }
    if (bracketIndexBuffer)
    {
        bracketIndexBuffer->release();
        bracketIndexBuffer = NULL;
    }
    if (wireIndexBuffer)
    {
        wireIndexBuffer->release();
        wireIndexBuffer = NULL;
    }
    if (wireVertexBuffer)
    {
        wireVertexBuffer->release();
        wireVertexBuffer = NULL;
    }
}

HRESULT Gate::Init(PinTable *ptable, float x, float y, bool fromMouseClick)
{
   HRESULT hr = S_OK;

   m_ptable = ptable;

   m_d.m_vCenter.x = x;
   m_d.m_vCenter.y = y;

   SetDefaults(fromMouseClick);

   InitVBA(fTrue, 0, NULL);

   return hr;
}

void Gate::SetDefaults(bool fromMouseClick)
{
   HRESULT hr;
   float fTmp;
   int iTmp;

   hr = GetRegStringAsFloat("DefaultProps\\Gate","Length", &fTmp);
   if ((hr == S_OK) && fromMouseClick)
      m_d.m_length = fTmp;
   else
      m_d.m_length = 100;

   hr = GetRegStringAsFloat("DefaultProps\\Gate","Height", &fTmp);
   if ((hr == S_OK) && fromMouseClick)
      m_d.m_height = fTmp;
   else
      m_d.m_height = 50;

   hr = GetRegStringAsFloat("DefaultProps\\Gate","Rotation", &fTmp);
   if ((hr == S_OK) && fromMouseClick)
      m_d.m_rotation = fTmp;
   else
      m_d.m_rotation = -90;

   hr = GetRegInt("DefaultProps\\Gate","ShowBracket", &iTmp);
   if ((hr == S_OK) && fromMouseClick)
      m_d.m_fShowBracket = iTmp == 0 ? false : true;
   else
      m_d.m_fShowBracket = true;

   hr = GetRegInt("DefaultProps\\Gate","Collidable", &iTmp);
   if ((hr == S_OK) && fromMouseClick)
      m_d.m_fCollidable = iTmp == 0 ? false : true;
   else
      m_d.m_fCollidable = true;

   hr = GetRegStringAsFloat("DefaultProps\\Gate","AngleMin", &fTmp);
   if ((hr == S_OK) && fromMouseClick)
      m_d.m_angleMin = fTmp;
   else
      m_d.m_angleMin = 0;

   hr = GetRegStringAsFloat("DefaultProps\\Gate","AngleMax", &fTmp);
   if ((hr == S_OK)  && fromMouseClick)
      m_d.m_angleMax = fTmp;
   else
      m_d.m_angleMax = (float)(M_PI/2.0);

   hr = GetRegInt("DefaultProps\\Gate","Visible", &iTmp);
   if ((hr == S_OK)  && fromMouseClick)
      m_d.m_fVisible = iTmp == 0 ? false : true;
   else
      m_d.m_fVisible = true;

   hr = GetRegInt("DefaultProps\\Gate","TimerEnabled", &iTmp);
   if ((hr == S_OK) && fromMouseClick)
      m_d.m_tdr.m_fTimerEnabled = iTmp == 0 ? fFalse : fTrue;
   else
      m_d.m_tdr.m_fTimerEnabled = fFalse;

   hr = GetRegInt("DefaultProps\\Gate","TimerInterval", &iTmp);
   if ((hr == S_OK) && fromMouseClick)
      m_d.m_tdr.m_TimerInterval = iTmp;
   else
      m_d.m_tdr.m_TimerInterval = 100;

   hr = GetRegString("DefaultProps\\Gate","Surface", &m_d.m_szSurface, MAXTOKEN);
   if ((hr != S_OK) || !fromMouseClick)
      m_d.m_szSurface[0] = 0;

   hr = GetRegStringAsFloat("DefaultProps\\Gate","Elasticity", &fTmp);
   if ((hr == S_OK)  && fromMouseClick)
      m_d.m_elasticity = fTmp;
   else
      m_d.m_elasticity = 0.3f;

   hr = GetRegStringAsFloat("DefaultProps\\Gate","Friction", &fTmp);
   if ((hr == S_OK) && fromMouseClick)
      m_d.m_friction =  fTmp;
   else
      m_d.m_friction = 0.02f;	

   hr = GetRegStringAsFloat("DefaultProps\\Gate","Scatter", &fTmp);
   if ((hr == S_OK) && fromMouseClick)
      m_d.m_scatter = fTmp;
   else
      m_d.m_scatter = 0;

   hr = GetRegString("DefaultProps\\Gate","ImageFront", &m_d.m_szImageFront, MAXTOKEN);
   if ((hr != S_OK) || !fromMouseClick)
      m_d.m_szImageFront[0] = 0;

   hr = GetRegString("DefaultProps\\Gate","ImageBack", &m_d.m_szImageBack, MAXTOKEN);
   if ((hr != S_OK) || !fromMouseClick)
      m_d.m_szImageBack[0] = 0;

   hr = GetRegInt("DefaultProps\\Gate","TwoWay", &iTmp);
   if ((hr == S_OK) && fromMouseClick)
       m_d.m_twoWay = iTmp == 0 ? false : true;
   else
       m_d.m_twoWay = true;

}


void Gate::WriteRegDefaults()
{
   SetRegValueFloat("DefaultProps\\Gate","Length", m_d.m_length);
   SetRegValueFloat("DefaultProps\\Gate","Height", m_d.m_height);
   SetRegValueFloat("DefaultProps\\Gate","Rotation", m_d.m_rotation);
   SetRegValueBool("DefaultProps\\Gate","ShowBracket", m_d.m_fShowBracket);
   SetRegValueBool("DefaultProps\\Gate","Collidable", m_d.m_fCollidable);
   SetRegValueFloat("DefaultProps\\Gate","AngleMin", m_d.m_angleMin);
   SetRegValueFloat("DefaultProps\\Gate","AngleMax", m_d.m_angleMax);
   SetRegValueBool("DefaultProps\\Gate","Visible", m_d.m_fVisible);
   SetRegValue("DefaultProps\\Gate","TimerEnabled",REG_DWORD,&m_d.m_tdr.m_fTimerEnabled,4);
   SetRegValue("DefaultProps\\Gate","TimerInterval", REG_DWORD, &m_d.m_tdr.m_TimerInterval, 4);
   SetRegValue("DefaultProps\\Gate","Surface", REG_SZ, &m_d.m_szSurface,lstrlen(m_d.m_szSurface));
   SetRegValueFloat("DefaultProps\\Gate","Elasticity", m_d.m_elasticity);
   SetRegValueFloat("DefaultProps\\Gate","Friction", m_d.m_friction);
   SetRegValueFloat("DefaultProps\\Gate","Scatter", m_d.m_scatter);
   SetRegValue("DefaultProps\\Gate","ImageFront", REG_SZ, &m_d.m_szImageFront,lstrlen(m_d.m_szImageFront));
   SetRegValue("DefaultProps\\Gate","ImageBack", REG_SZ, &m_d.m_szImageBack,lstrlen(m_d.m_szImageBack));
   SetRegValueBool("DefaultProps\\Gate","TwoWay", m_d.m_twoWay);
}

void Gate::PreRender(Sur * const psur)
{
}

void Gate::Render(Sur * const psur)
{
   psur->SetLineColor(RGB(0,0,0),false,2);
   psur->SetObject(this);

   float halflength = m_d.m_length * 0.5f;	
   float len1 = halflength *0.5f;
   float len2 = len1*0.5f;
   Vertex2D tmp;

   {
   const float radangle = ANGTORAD(m_d.m_rotation);
   {
      const float sn = sinf(radangle);
      const float cs = cosf(radangle);

      psur->Line(m_d.m_vCenter.x + cs*halflength, m_d.m_vCenter.y + sn*halflength,
         m_d.m_vCenter.x - cs*halflength, m_d.m_vCenter.y - sn*halflength);

      // Draw Arrow
      psur->SetLineColor(RGB(0,0,0),false,1);

      tmp.x = m_d.m_vCenter.x + sn*len1;
      tmp.y = m_d.m_vCenter.y - cs*len1;

      psur->Line(tmp.x, tmp.y,
         m_d.m_vCenter.x, m_d.m_vCenter.y);
   }

   {
      const float arrowang = radangle+0.6f;
      const float sn = sinf(arrowang);
      const float cs = cosf(arrowang);

      psur->Line(tmp.x, tmp.y,
         m_d.m_vCenter.x + sn*len2, m_d.m_vCenter.y - cs*len2);
   }
   }

   {
	   const float arrowang = ANGTORAD(m_d.m_rotation) - 0.6f;
	   const float sn = sinf(arrowang);
	   const float cs = cosf(arrowang);

	   psur->Line(tmp.x, tmp.y,
		   m_d.m_vCenter.x + sn*len2, m_d.m_vCenter.y - cs*len2);
   }

   if ( m_d.m_twoWay )
   {
       const float radangle = ANGTORAD(m_d.m_rotation-180.f);
       {
           const float sn = sinf(radangle);
           const float cs = cosf(radangle);

           // Draw Arrow
           psur->SetLineColor(RGB(0,0,0),false,1);

           tmp.x = m_d.m_vCenter.x + sn*len1;
           tmp.y = m_d.m_vCenter.y - cs*len1;

           psur->Line(tmp.x, tmp.y,
               m_d.m_vCenter.x, m_d.m_vCenter.y);
       }

       {
           const float arrowang = radangle+0.6f;
           const float sn = sinf(arrowang);
           const float cs = cosf(arrowang);

           psur->Line(tmp.x, tmp.y,
               m_d.m_vCenter.x + sn*len2, m_d.m_vCenter.y - cs*len2);
       }

       const float arrowang = radangle-0.6f;
       const float sn = sinf(arrowang);
       const float cs = cosf(arrowang);

       psur->Line(tmp.x, tmp.y,
           m_d.m_vCenter.x + sn*len2, m_d.m_vCenter.y - cs*len2);
   }
}

void Gate::RenderBlueprint(Sur *psur)
{
}

void Gate::GetTimers(Vector<HitTimer> * const pvht)
{
   IEditable::BeginPlay();

   HitTimer * const pht = new HitTimer();
   pht->m_interval = m_d.m_tdr.m_TimerInterval;
   pht->m_nextfire = pht->m_interval;
   pht->m_pfe = (IFireEvents *)this;

   m_phittimer = pht;

   if (m_d.m_tdr.m_fTimerEnabled)
   {
      pvht->AddElement(pht);
   }
}

void Gate::GetHitShapes(Vector<HitObject> * const pvho)
{
   const float height = m_ptable->GetSurfaceHeight(m_d.m_szSurface, m_d.m_vCenter.x, m_d.m_vCenter.y);
   const float h = m_d.m_height;		//relative height of the gate 

   float halflength = m_d.m_length * 0.5f;	

   const float angleMin = min(m_d.m_angleMin, m_d.m_angleMax); // correct angle inversions
   const float angleMax = max(m_d.m_angleMin, m_d.m_angleMax);

   m_d.m_angleMin = angleMin;	
   m_d.m_angleMax = angleMax;

   const float radangle = ANGTORAD(m_d.m_rotation);
   const float sn = sinf(radangle);
   const float cs = cosf(radangle);

   const Vertex2D rgv[2] = {
      Vertex2D(m_d.m_vCenter.x + cs*(halflength + (float)PHYS_SKIN),//oversize by the ball's radius
      m_d.m_vCenter.y + sn*(halflength + (float)PHYS_SKIN)),// to prevent the ball from clipping through

      Vertex2D(m_d.m_vCenter.x - cs*(halflength + (float)PHYS_SKIN),//the gate's edge
      m_d.m_vCenter.y - sn*(halflength + (float)PHYS_SKIN))};

      if ( !m_d.m_twoWay )
      {
          m_plineseg = new LineSeg();

          m_plineseg->m_pfe = NULL;

          m_plineseg->m_rcHitRect.zlow = height;
          m_plineseg->m_rcHitRect.zhigh = height + (float)(2.0*PHYS_SKIN); //!! = ball diameter

          m_plineseg->v1.x = rgv[0].x;
          m_plineseg->v1.y = rgv[0].y;

          m_plineseg->v2.x = rgv[1].x;
          m_plineseg->v2.y = rgv[1].y;

          m_plineseg->CalcNormal();

          m_plineseg->m_elasticity = m_d.m_elasticity;
          m_plineseg->SetFriction(m_d.m_friction);
          m_plineseg->m_scatter = m_d.m_scatter;

          pvho->AddElement(m_plineseg);
      }
      m_phitgate = new HitGate(this, height);
      m_phitgate->m_twoWay = m_d.m_twoWay;
      m_phitgate->m_pfe = (IFireEvents *)this;
      pvho->AddElement(m_phitgate);

      m_phitgate->m_fEnabled = m_d.m_fCollidable;

      if(m_d.m_fShowBracket)
      {
         {
            HitCircle * const phitcircle = new HitCircle();
            phitcircle->m_pfe = NULL;
            phitcircle->center.x = m_d.m_vCenter.x + cs*halflength;
            phitcircle->center.y = m_d.m_vCenter.y + sn*halflength;
            phitcircle->radius = 0.01f;
            phitcircle->zlow = height;
            phitcircle->zhigh = height+h; //+50;

            pvho->AddElement(phitcircle);
         }

         HitCircle * const phitcircle = new HitCircle();
         phitcircle->m_pfe = NULL;
         phitcircle->center.x = m_d.m_vCenter.x - cs*halflength;
         phitcircle->center.y = m_d.m_vCenter.y - sn*halflength;
         phitcircle->radius = 0.01f;
         phitcircle->zlow = height;
         phitcircle->zhigh = height+h; //+50;
         pvho->AddElement(phitcircle);
      }
}

void Gate::GetHitShapesDebug(Vector<HitObject> * const pvho)
{
}

void Gate::EndPlay()
{
   IEditable::EndPlay();

   m_phitgate = NULL;
   m_plineseg = NULL;

   if (bracketVertexBuffer)
   {
       bracketVertexBuffer->release();
       bracketVertexBuffer = NULL;
   }
   if (bracketIndexBuffer)
   {
       bracketIndexBuffer->release();
       bracketIndexBuffer = NULL;
   }
   if (wireIndexBuffer)
   {
       wireIndexBuffer->release();
       wireIndexBuffer = NULL;
   }
   if (wireVertexBuffer)
   {
       wireVertexBuffer->release();
       wireVertexBuffer = NULL;
   }
}

void Gate::UpdateWire( RenderDevice *pd3dDevice )
{
    Matrix3D fullMatrix;
    Matrix3D rotzMat,rotxMat;
    Vertex3D_NoTex2 *buf;

    fullMatrix.SetIdentity();
    rotxMat.RotateXMatrix(m_d.m_twoWay ? m_phitgate->m_gateanim.m_angle : -m_phitgate->m_gateanim.m_angle);
    rotxMat.Multiply(fullMatrix, fullMatrix);
    rotzMat.RotateZMatrix(ANGTORAD(m_d.m_rotation));
    rotzMat.Multiply(fullMatrix, fullMatrix);

    wireVertexBuffer->lock(0, 0, (void**)&buf, VertexBuffer::DISCARDCONTENTS);
    for( int i=0;i<gateWireNumVertices;i++ )
    {
        Vertex3Ds vert(gateWire[i].x,gateWire[i].y,gateWire[i].z);
        vert = fullMatrix.MultiplyVector(vert);

        buf[i].x = vert.x*m_d.m_length+m_d.m_vCenter.x;
        buf[i].y = vert.y*m_d.m_length+m_d.m_vCenter.y;
        buf[i].z = vert.z*m_d.m_length*m_ptable->m_zScale + m_d.m_height + baseHeight;
        vert = Vertex3Ds( gateWire[i].nx, gateWire[i].ny, gateWire[i].nz );
        vert = fullMatrix.MultiplyVectorNoTranslate(vert);
        buf[i].nx = vert.x;
        buf[i].ny = vert.y;
        buf[i].nz = vert.z;
        buf[i].tu = gateWire[i].tu;
        buf[i].tv = gateWire[i].tv;
    }
    wireVertexBuffer->unlock();

}

void Gate::RenderObject( RenderDevice* pd3dDevice)
{
    pd3dDevice->SetVertexDeclaration( pd3dDevice->m_pVertexNormalTexelDeclaration );

    Material *mat = m_ptable->GetMaterial( m_d.m_szMaterial);
    pd3dDevice->basicShader->SetMaterial(mat);

    Pin3D * const ppin3d = &g_pplayer->m_pin3d;
    COLORREF rgbTransparent = RGB(255,0,255); //RGB(0,0,0);

    ppin3d->EnableAlphaBlend(1,false);

    pd3dDevice->basicShader->SetTechnique("basic_without_texture");
    if ( m_d.m_fShowBracket )
    {
        // render bracket
        pd3dDevice->basicShader->Begin(0);
        pd3dDevice->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, bracketVertexBuffer, 0, gateBracketNumVertices, bracketIndexBuffer, 0, gateBracketNumFaces );
        pd3dDevice->basicShader->End();
    }

    UpdateWire(pd3dDevice);
    // render wire
    pd3dDevice->basicShader->Begin(0);
    pd3dDevice->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, wireVertexBuffer, 0, gateWireNumVertices, wireIndexBuffer, 0, gateWireNumFaces);
    pd3dDevice->basicShader->End();
}

void Gate::PostRenderStatic(RenderDevice* pd3dDevice)
{
    TRACE_FUNCTION();

    if (!m_phitgate->m_gateanim.m_fVisible)
        return;
    RenderObject(pd3dDevice);
}

void Gate::RenderSetup(RenderDevice* pd3dDevice)
{
    baseHeight = m_ptable->GetSurfaceHeight(m_d.m_szSurface, m_d.m_vCenter.x, m_d.m_vCenter.y);
    std::vector<WORD> indices(gateBracketNumFaces);

    for( int i=0;i<gateBracketNumFaces;i++ ) indices[i] = gateBracketIndices[i];
    if (bracketIndexBuffer)
        bracketIndexBuffer->release();
    bracketIndexBuffer = pd3dDevice->CreateAndFillIndexBuffer( indices );

    if (!bracketVertexBuffer)
        pd3dDevice->CreateVertexBuffer(gateBracketNumVertices, D3DUSAGE_DYNAMIC, MY_D3DFVF_NOTEX2_VERTEX, &bracketVertexBuffer);

    Matrix3D fullMatrix;
    fullMatrix.RotateZMatrix(ANGTORAD(m_d.m_rotation));

    Vertex3D_NoTex2 *buf;
    bracketVertexBuffer->lock(0, 0, (void**)&buf, 0);
    for( int i=0;i<gateBracketNumVertices;i++ )
    {
        Vertex3Ds vert(gateBracket[i].x,gateBracket[i].y,gateBracket[i].z);
        vert = fullMatrix.MultiplyVector(vert);

        buf[i].x = vert.x*m_d.m_length+m_d.m_vCenter.x;
        buf[i].y = vert.y*m_d.m_length+m_d.m_vCenter.y;
        buf[i].z = vert.z*m_d.m_length*m_ptable->m_zScale + m_d.m_height + baseHeight;
        vert = Vertex3Ds( gateBracket[i].nx, gateBracket[i].ny, gateBracket[i].nz );
        vert = fullMatrix.MultiplyVectorNoTranslate(vert);
        buf[i].nx = vert.x;
        buf[i].ny = vert.y;
        buf[i].nz = vert.z;
        buf[i].tu = gateBracket[i].tu;
        buf[i].tv = gateBracket[i].tv;
    }
    bracketVertexBuffer->unlock();

    indices.clear();
    indices.resize(gateWireNumFaces);
    for( int i=0;i<gateWireNumFaces;i++ ) indices[i] = gateWireIndices[i];
    if (wireIndexBuffer)
        wireIndexBuffer->release();
    wireIndexBuffer = pd3dDevice->CreateAndFillIndexBuffer( indices );

    if (!wireVertexBuffer)
        pd3dDevice->CreateVertexBuffer(gateWireNumVertices, 0, MY_D3DFVF_NOTEX2_VERTEX, &wireVertexBuffer);

    wireVertices = new Vertex3D_NoTex2[gateWireNumVertices];
    for( int i=0;i<gateWireNumVertices;i++ )
    {
        Vertex3Ds vert(gateWire[i].x,gateWire[i].y,gateWire[i].z);
        vert = fullMatrix.MultiplyVector(vert);

        wireVertices[i].x = vert.x*m_d.m_length+m_d.m_vCenter.x;
        wireVertices[i].y = vert.y*m_d.m_length+m_d.m_vCenter.y;
        wireVertices[i].z = vert.z*m_d.m_length*m_ptable->m_zScale + m_d.m_height + baseHeight;
        vert = Vertex3Ds( gateWire[i].nx, gateWire[i].ny, gateWire[i].nz );
        vert = fullMatrix.MultiplyVectorNoTranslate(vert);
        wireVertices[i].nx = vert.x;
        wireVertices[i].ny = vert.y;
        wireVertices[i].nz = vert.z;
        wireVertices[i].tu = gateWire[i].tu;
        wireVertices[i].tv = gateWire[i].tv;
    }
    wireVertexBuffer->lock(0, 0, (void**)&buf, 0);
    memcpy( buf, wireVertices, sizeof(Vertex3D_NoTex2)*gateWireNumVertices);
    wireVertexBuffer->unlock();
   delete [] wireVertices;
}

void Gate::RenderStatic(RenderDevice* pd3dDevice) // only the support structures are rendered here
{
}

void Gate::SetObjectPos()
{
   g_pvp->SetObjectPosCur(m_d.m_vCenter.x, m_d.m_vCenter.y);
}

void Gate::MoveOffset(const float dx, const float dy)
{
   m_d.m_vCenter.x += dx;
   m_d.m_vCenter.y += dy;

   m_ptable->SetDirtyDraw();
}

void Gate::GetCenter(Vertex2D * const pv) const
{
   *pv = m_d.m_vCenter;
}

void Gate::PutCenter(const Vertex2D * const pv)
{
   m_d.m_vCenter = *pv;

   m_ptable->SetDirtyDraw();
}

HRESULT Gate::SaveData(IStream *pstm, HCRYPTHASH hcrypthash, HCRYPTKEY hcryptkey)
{
   BiffWriter bw(pstm, hcrypthash, hcryptkey);

   bw.WriteStruct(FID(VCEN), &m_d.m_vCenter, sizeof(Vertex2D));
   bw.WriteFloat(FID(LGTH), m_d.m_length);
   bw.WriteFloat(FID(HGTH), m_d.m_height);
   bw.WriteFloat(FID(ROTA), m_d.m_rotation);
   bw.WriteString(FID(MATR), m_d.m_szMaterial);
   bw.WriteBool(FID(TMON), m_d.m_tdr.m_fTimerEnabled);
   bw.WriteBool(FID(GSUPT), m_d.m_fShowBracket);
   bw.WriteBool(FID(GCOLD), m_d.m_fCollidable);
   bw.WriteInt(FID(TMIN), m_d.m_tdr.m_TimerInterval);
   bw.WriteString(FID(IMGF), m_d.m_szImageFront);
   bw.WriteString(FID(IMGB), m_d.m_szImageBack);
   bw.WriteString(FID(SURF), m_d.m_szSurface);
   bw.WriteFloat(FID(ELAS), m_d.m_elasticity);
   bw.WriteFloat(FID(GAMAX), m_d.m_angleMax);
   bw.WriteFloat(FID(GAMIN), m_d.m_angleMin);
   bw.WriteFloat(FID(GFRCT), m_d.m_friction);
   bw.WriteBool(FID(GVSBL), m_d.m_fVisible);
   bw.WriteWideString(FID(NAME), (WCHAR *)m_wzName);
   bw.WriteBool(FID(TWWA), m_d.m_twoWay);

   ISelect::SaveData(pstm, hcrypthash, hcryptkey);

   bw.WriteTag(FID(ENDB));

   return S_OK;
}

HRESULT Gate::InitLoad(IStream *pstm, PinTable *ptable, int *pid, int version, HCRYPTHASH hcrypthash, HCRYPTKEY hcryptkey)
{
   SetDefaults(false);

   BiffReader br(pstm, this, pid, version, hcrypthash, hcryptkey);

   m_ptable = ptable;

   br.Load();
   return S_OK;
}

BOOL Gate::LoadToken(int id, BiffReader *pbr)
{
   if (id == FID(PIID))
   {
      pbr->GetInt((int *)pbr->m_pdata);
   }
   else if (id == FID(VCEN))
   {
      pbr->GetStruct(&m_d.m_vCenter, sizeof(Vertex2D));
   }
   else if (id == FID(LGTH))
   {
      pbr->GetFloat(&m_d.m_length);
   }
   else if (id == FID(HGTH))
   {
      pbr->GetFloat(&m_d.m_height);
   }
   else if (id == FID(ROTA))
   {
      pbr->GetFloat(&m_d.m_rotation);
   }
   else if (id == FID(MATR))
   {
      pbr->GetString(m_d.m_szMaterial);
   }
   else if (id == FID(TMON))
   {
      pbr->GetBool(&m_d.m_tdr.m_fTimerEnabled);
   }
   else if (id == FID(GSUPT))
   {
      pbr->GetBool(&m_d.m_fShowBracket); 
   }
   else if (id == FID(GCOLD))
   {
      pbr->GetBool(&m_d.m_fCollidable); 
   }
   else if (id == FID(TWWA))
   {
      pbr->GetBool(&m_d.m_twoWay); 
   }
   else if (id == FID(GVSBL))
   {
      pbr->GetBool(&m_d.m_fVisible); 
   }
   else if (id == FID(TMIN))
   {
      pbr->GetInt(&m_d.m_tdr.m_TimerInterval);
   }
   else if (id == FID(IMGF))
   {
      pbr->GetString(m_d.m_szImageFront);
   }
   else if (id == FID(IMGB))
   {
      pbr->GetString(m_d.m_szImageBack);
   }
   else if (id == FID(SURF))
   {
      pbr->GetString(m_d.m_szSurface);
   }
   else if (id == FID(NAME))
   {
      pbr->GetWideString((WCHAR *)m_wzName);
   }
   else if (id == FID(ELAS))
   {
      pbr->GetFloat(&m_d.m_elasticity);	
   }
   else if (id == FID(GAMAX))
   {
      pbr->GetFloat(&m_d.m_angleMax);
   }
   else if (id == FID(GAMIN))
   {
      pbr->GetFloat(&m_d.m_angleMin);
   }
   else if (id == FID(GFRCT))
   {
      pbr->GetFloat(&m_d.m_friction);
   }
   else
   {
      ISelect::LoadToken(id, pbr);
   }

   return fTrue;
}

HRESULT Gate::InitPostLoad()
{
   return S_OK;
}

STDMETHODIMP Gate::InterfaceSupportsErrorInfo(REFIID riid)
{
   static const IID* arr[] =
   {
      &IID_IGate,
   };

   for (size_t i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
   {
      if (InlineIsEqualGUID(*arr[i],riid))
         return S_OK;
   }
   return S_FALSE;
}

STDMETHODIMP Gate::get_Length(float *pVal)
{
   *pVal = m_d.m_length;

   return S_OK;
}

STDMETHODIMP Gate::put_Length(float newVal)
{
   STARTUNDO

      m_d.m_length = newVal;

   STOPUNDO

      return S_OK;
}

STDMETHODIMP Gate::get_Height(float *pVal)
{
   *pVal = m_d.m_height;

   return S_OK;
}

STDMETHODIMP Gate::put_Height(float newVal)
{
   STARTUNDO

      m_d.m_height = newVal;

   STOPUNDO

      return S_OK;
}

STDMETHODIMP Gate::get_Rotation(float *pVal)
{
   *pVal = m_d.m_rotation;

   return S_OK;
}

STDMETHODIMP Gate::put_Rotation(float newVal)
{
   STARTUNDO

      m_d.m_rotation = newVal;

   STOPUNDO;

   return S_OK;
}

STDMETHODIMP Gate::get_X(float *pVal)
{
   *pVal = m_d.m_vCenter.x;

   return S_OK;
}

STDMETHODIMP Gate::put_X(float newVal)
{
   STARTUNDO

      m_d.m_vCenter.x = newVal;

   STOPUNDO

      return S_OK;
}

STDMETHODIMP Gate::get_Y(float *pVal)
{
   *pVal = m_d.m_vCenter.y;

   return S_OK;
}

STDMETHODIMP Gate::put_Y(float newVal)
{
   STARTUNDO

      m_d.m_vCenter.y = newVal;

   STOPUNDO

      return S_OK;
}

STDMETHODIMP Gate::get_Surface(BSTR *pVal)
{
   WCHAR wz[512];
   MultiByteToWideChar(CP_ACP, 0, m_d.m_szSurface, -1, wz, 32);
   *pVal = SysAllocString(wz);

   return S_OK;
}

STDMETHODIMP Gate::put_Surface(BSTR newVal)
{
   STARTUNDO

      WideCharToMultiByte(CP_ACP, 0, newVal, -1, m_d.m_szSurface, 32, NULL, NULL);

   STOPUNDO

      return S_OK;
}

STDMETHODIMP Gate::get_Material(BSTR *pVal)
{
   WCHAR wz[512];

   MultiByteToWideChar(CP_ACP, 0, m_d.m_szMaterial, -1, wz, 32);
   *pVal = SysAllocString(wz);

   return S_OK;
}

STDMETHODIMP Gate::put_Material(BSTR newVal)
{
   STARTUNDO

   WideCharToMultiByte(CP_ACP, 0, newVal, -1, m_d.m_szMaterial, 32, NULL, NULL);

   STOPUNDO

      return S_OK;
}

STDMETHODIMP Gate::get_Open(VARIANT_BOOL *pVal)
{
   *pVal = (VARIANT_BOOL)FTOVB((m_phitgate) ? m_phitgate->m_gateanim.m_fOpen : false);

   return S_OK;
}

STDMETHODIMP Gate::put_Open(VARIANT_BOOL newVal)
{
   if (m_phitgate)
   {
      m_phitgate->m_gateanim.m_angleMax = m_d.m_angleMax;
      m_phitgate->m_gateanim.m_angleMin = m_d.m_angleMin;

      if (newVal)
      {
         m_phitgate->m_gateanim.m_fOpen = true;
         m_phitgate->m_fEnabled = false;
		 if ( !m_d.m_twoWay )
	         m_plineseg->m_fEnabled = false;

         if (m_phitgate->m_gateanim.m_angle < m_phitgate->m_gateanim.m_angleMax)
            m_phitgate->m_gateanim.m_anglespeed = 0.2f;
      }
      else
      {
         m_phitgate->m_gateanim.m_fOpen = false;

         m_phitgate->m_fEnabled = m_d.m_fCollidable;
		 if ( !m_d.m_twoWay )
	         m_plineseg->m_fEnabled = m_d.m_fCollidable;					

         if (m_phitgate->m_gateanim.m_angle > m_phitgate->m_gateanim.m_angleMin)
            m_phitgate->m_gateanim.m_anglespeed = -0.2f;
      }
   }

   return S_OK;
}

STDMETHODIMP Gate::get_Elasticity(float *pVal)
{
   *pVal = m_d.m_elasticity;

   return S_OK;
}

STDMETHODIMP Gate::put_Elasticity(float newVal)
{
   STARTUNDO

      m_d.m_elasticity = newVal;

   STOPUNDO

      return S_OK;
}

STDMETHODIMP Gate::get_ShowBracket(VARIANT_BOOL *pVal)
{
   *pVal = (VARIANT_BOOL)FTOVB(m_d.m_fShowBracket);

   return S_OK;
}

STDMETHODIMP Gate::put_ShowBracket(VARIANT_BOOL newVal)
{
   STARTUNDO

      m_d.m_fShowBracket = VBTOF(newVal);

   STOPUNDO

      return S_OK;
}

STDMETHODIMP Gate::get_CloseAngle(float *pVal)
{
   *pVal = RADTOANG(g_pplayer ? m_phitgate->m_gateanim.m_angleMin : m_d.m_angleMin);

   return S_OK;
}

STDMETHODIMP Gate::put_CloseAngle(float newVal)
{
   if (m_d.m_fCollidable) 
   {
      newVal = 0;
      ShowError("Gate is collidable! closing angles other than 0 aren't possible!");
   }
   else newVal = ANGTORAD(newVal);

   if (g_pplayer)
   {
      if (newVal > m_d.m_angleMax) newVal = m_d.m_angleMax;
      else if (newVal < m_d.m_angleMin) newVal = m_d.m_angleMin;	

      if (m_phitgate->m_gateanim.m_angleMax > newVal)	// max is bigger
         m_phitgate->m_gateanim.m_angleMin = newVal;	//then set new minumum
      else m_phitgate->m_gateanim.m_angleMax = newVal;//else set new max
   }
   else
   {
      STARTUNDO
         m_d.m_angleMin = newVal;
      STOPUNDO
   }
   return S_OK;
}


STDMETHODIMP Gate::get_OpenAngle(float *pVal)
{
   *pVal = RADTOANG((g_pplayer) ? m_phitgate->m_gateanim.m_angleMax : m_d.m_angleMax);	//player active value

   return S_OK;
}

STDMETHODIMP Gate::put_OpenAngle(float newVal)
{
   if (m_d.m_fCollidable) newVal = 0;
   else newVal = ANGTORAD(newVal);

   if (g_pplayer)
   {
      if (newVal > m_d.m_angleMax) newVal = m_d.m_angleMax;
      else if (newVal < m_d.m_angleMin) newVal = m_d.m_angleMin;	

      if (m_phitgate->m_gateanim.m_angleMin < newVal)	// min is smaller
         m_phitgate->m_gateanim.m_angleMax = newVal;	//then set new maximum
      else m_phitgate->m_gateanim.m_angleMin = newVal;//else set new min
   }
   else
   {
      STARTUNDO
         m_d.m_angleMax = newVal;
      STOPUNDO
   }
   return S_OK;
}


STDMETHODIMP Gate::get_Collidable(VARIANT_BOOL *pVal)
{
   *pVal = (VARIANT_BOOL)FTOVB((g_pplayer) ? m_phitgate->m_fEnabled : m_d.m_fCollidable);

   return S_OK;
}


STDMETHODIMP Gate::put_Collidable(VARIANT_BOOL newVal)
{	
   if (g_pplayer)
   {
      m_phitgate->m_fEnabled = VBTOF(newVal);
	  if ( !m_d.m_twoWay )
	      m_plineseg->m_fEnabled = VBTOF(newVal);
      m_phitgate->m_gateanim.m_angleMax = m_d.m_angleMax;
      m_phitgate->m_gateanim.m_angleMin = m_d.m_angleMin;

      if (newVal) m_phitgate->m_gateanim.m_angleMin = 0;
   }
   else
   {
      STARTUNDO

         m_d.m_fCollidable = VBTOF(newVal);

      if (newVal) m_d.m_angleMin = 0;	

      STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Gate::Move(int dir, float speed, float angle)//move non-collidable gate, for graphic effects only
{
   if (g_pplayer)
   {
      m_phitgate->m_gateanim.m_fOpen = true;// move always turns off natural swing
      m_phitgate->m_fEnabled = false;		// and collidable off
	  if ( !m_d.m_twoWay )
	      m_plineseg->m_fEnabled = false;		

      if (speed <= 0.0f) speed = 0.2f;		//default gate angle speed
      else speed *= (float)(M_PI/180.0);	// convert to radians

      if (!dir || angle != 0)				// if no direction or non-zero angle
      {
         angle *= (float)(M_PI/180.0);		// convert to radians

         if (angle < m_d.m_angleMin) angle = m_d.m_angleMin;
         else if (angle > m_d.m_angleMax) angle = m_d.m_angleMax;

         const float da = angle - m_phitgate->m_gateanim.m_angle; //calc true direction

         if (da > 1.0e-5f) dir = +1;
         else if (da < -1.0e-5f) dir = -1;
         else 
         {
            dir = 0;									// do nothing
            m_phitgate->m_gateanim.m_anglespeed = 0;	//stop 
         }
      }
      else {angle = (dir < 0) ? m_d.m_angleMin : m_d.m_angleMax;}	//dir selected and angle not specified			

      if (dir > 0)
      {
         m_phitgate->m_gateanim.m_angleMax = angle;

         if (m_phitgate->m_gateanim.m_angle < m_phitgate->m_gateanim.m_angleMax)
            m_phitgate->m_gateanim.m_anglespeed = speed;
      }
      else if (dir < 0)
      {
         m_phitgate->m_gateanim.m_angleMin = angle;

         if (m_phitgate->m_gateanim.m_angle > m_phitgate->m_gateanim.m_angleMin)
            m_phitgate->m_gateanim.m_anglespeed = -speed;
      }
   }

   return S_OK;
}

STDMETHODIMP Gate::get_Friction(float *pVal)
{
   *pVal = (g_pplayer) ? m_phitgate->m_gateanim.m_friction : m_d.m_friction;	

   return S_OK;
}

STDMETHODIMP Gate::put_Friction(float newVal)
{	
   if (newVal < 0) newVal = 0;
   else if (newVal > 1.0f) newVal = 1.0f;

   if (g_pplayer)
   {
      m_phitgate->m_gateanim.m_friction = newVal;	
   }
   else
   {
      STARTUNDO

         m_d.m_friction =  newVal;

      STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Gate::get_Visible(VARIANT_BOOL *pVal)
{
   *pVal = (VARIANT_BOOL)FTOVB((g_pplayer) ? m_phitgate->m_gateanim.m_fVisible : m_d.m_fVisible);

   return S_OK;
}

STDMETHODIMP Gate::put_Visible(VARIANT_BOOL newVal)
{
   if (g_pplayer)
   {
      m_phitgate->m_gateanim.m_fVisible = VBTOF(newVal);
   }
   else
   {
      STARTUNDO

         m_d.m_fVisible = VBTOF(newVal);

      STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Gate::get_TwoWay(VARIANT_BOOL *pVal)
{
   *pVal = (VARIANT_BOOL)FTOVB(m_d.m_twoWay);

   return S_OK;
}

STDMETHODIMP Gate::put_TwoWay(VARIANT_BOOL newVal)
{	
    if (g_pplayer)
    {
        m_phitgate->m_twoWay = VBTOF(newVal);
    }
    else
    {
       STARTUNDO

          m_d.m_twoWay = VBTOF(newVal);

       STOPUNDO
    }
      return S_OK;
}

STDMETHODIMP Gate::get_CurrentAngle(float *pVal)
{
    if (g_pplayer)
    {
        *pVal = RADTOANG(m_phitgate->m_gateanim.m_angle);
        return S_OK;
    }
    else
        return E_FAIL;
}

void Gate::GetDialogPanes(Vector<PropertyPane> *pvproppane)
{
   PropertyPane *pproppane;

   pproppane = new PropertyPane(IDD_PROP_NAME, NULL);
   pvproppane->AddElement(pproppane);

   pproppane = new PropertyPane(IDD_PROPGATE_VISUALS, IDS_VISUALS);
   pvproppane->AddElement(pproppane);

   pproppane = new PropertyPane(IDD_PROPGATE_POSITION, IDS_POSITION);
   pvproppane->AddElement(pproppane);

   pproppane = new PropertyPane(IDD_PROPGATE_PHYSICS, IDS_PHYSICS);
   pvproppane->AddElement(pproppane);

   pproppane = new PropertyPane(IDD_PROP_TIMER, IDS_MISC);
   pvproppane->AddElement(pproppane);
}
