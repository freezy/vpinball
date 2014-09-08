#include "StdAfx.h"

Ramp::Ramp()
{
   m_menuid = IDR_SURFACEMENU;
   m_d.m_fCollidable = true;
   m_d.m_fVisible = true;
   staticVertexBuffer = 0;
   dynamicVertexBuffer = 0;
   dynamicIndexBuffer = 0;
   dynamicVertexBufferRegenerate = true;
   m_d.m_enableLightingImage = true;
   m_d.m_depthBias = 0.0f;
   m_d.m_wireDiameter = 6.0f;
   m_d.m_wireDistanceX = 38.0f;
   m_d.m_wireDistanceY = 88.0f;
}

Ramp::~Ramp()
{
	if(staticVertexBuffer)
		staticVertexBuffer->release();

	if(dynamicVertexBuffer)
		dynamicVertexBuffer->release();

    if (dynamicIndexBuffer)
        dynamicIndexBuffer->release();
}

HRESULT Ramp::Init(PinTable *ptable, float x, float y, bool fromMouseClick)
{
   m_ptable = ptable;
   m_d.m_fVisible = true;

   float length = 0.5f * GetRegStringAsFloatWithDefault("DefaultProps\\Ramp", "Length", 400.0f);

   CComObject<DragPoint> *pdp;
   CComObject<DragPoint>::CreateInstance(&pdp);
   if (pdp)
   {
      pdp->AddRef();
      pdp->Init(this, x, y+length);
      pdp->m_fSmooth = fTrue;
      m_vdpoint.AddElement(pdp);
   }

   CComObject<DragPoint>::CreateInstance(&pdp);
   if (pdp)
   {
      pdp->AddRef();
      pdp->Init(this, x, y-length);
      pdp->m_fSmooth = fTrue;
      m_vdpoint.AddElement(pdp);
   }

   SetDefaults(fromMouseClick);

   InitVBA(fTrue, 0, NULL);

   return S_OK;
}

void Ramp::SetDefaults(bool fromMouseClick)
{
   static const char strKeyName[] = "DefaultProps\\Ramp";

   HRESULT hr;

   m_d.m_heightbottom = fromMouseClick ? GetRegStringAsFloatWithDefault(strKeyName,"HeightBottom", 0.0f) : 0.0f;
   m_d.m_heighttop = fromMouseClick ? GetRegStringAsFloatWithDefault(strKeyName,"HeightTop", 50.0f) : 50.0f;
   m_d.m_widthbottom = fromMouseClick ? GetRegStringAsFloatWithDefault(strKeyName,"WidthBottom", 75.0f) : 75.0f;
   m_d.m_widthtop = fromMouseClick ? GetRegStringAsFloatWithDefault(strKeyName,"WidthTop", 60.0f) : 60.0f;
   m_d.m_color = fromMouseClick ? GetRegIntWithDefault(strKeyName,"Color", RGB(50,200,50)) : RGB(50,200,50);
   m_d.m_type = fromMouseClick ? (RampType)GetRegIntWithDefault(strKeyName,"RampType", RampTypeFlat) : RampTypeFlat;

   m_d.m_tdr.m_fTimerEnabled = fromMouseClick ? GetRegBoolWithDefault(strKeyName,"TimerEnabled", false) : false;
   m_d.m_tdr.m_TimerInterval = fromMouseClick ? GetRegIntWithDefault(strKeyName,"TimerInterval", 100) : 100;

   hr = GetRegString(strKeyName,"Image", m_d.m_szImage, MAXTOKEN);
   if ((hr != S_OK) || !fromMouseClick)
      m_d.m_szImage[0] = 0;

   m_d.m_imagealignment = fromMouseClick ? (RampImageAlignment)GetRegIntWithDefault(strKeyName,"ImageMode", ImageModeWorld) : ImageModeWorld;
   m_d.m_fImageWalls = fromMouseClick ? GetRegBoolWithDefault(strKeyName,"ImageWalls", true) : true;
   m_d.m_fCastsShadow = fromMouseClick ? GetRegBoolWithDefault(strKeyName,"CastsShadow", true) : true;
   m_d.m_transparent = fromMouseClick ? GetRegBoolWithDefault(strKeyName,"Transparent", false) : false;
   m_d.m_opacity = fromMouseClick ? GetRegIntWithDefault(strKeyName,"Opacity", 255) : 255;

   m_d.m_leftwallheight = fromMouseClick ? GetRegStringAsFloatWithDefault(strKeyName,"LeftWallHeight", 62.0f) : 62.0f;
   m_d.m_rightwallheight = fromMouseClick ? GetRegStringAsFloatWithDefault(strKeyName,"RightWallHeight", 62.0f) : 62.0f;
   m_d.m_leftwallheightvisible = fromMouseClick ? GetRegStringAsFloatWithDefault(strKeyName,"LeftWallHeightVisible", 30.0f) : 30.0f;
   m_d.m_rightwallheightvisible = fromMouseClick ? GetRegStringAsFloatWithDefault(strKeyName,"RightWallHeightVisible", 30.0f) : 30.0f;

   m_d.m_elasticity = fromMouseClick ? GetRegStringAsFloatWithDefault(strKeyName,"Elasticity", 0.3f) : 0.3f;
   m_d.m_friction = fromMouseClick ? GetRegStringAsFloatWithDefault(strKeyName,"Friction", 0) : 0;
   m_d.m_scatter = fromMouseClick ? GetRegStringAsFloatWithDefault(strKeyName,"Scatter", 0) : 0;

   m_d.m_fVisible = fromMouseClick ? GetRegBoolWithDefault(strKeyName,"Visible", true) : true;
   m_d.m_fCollidable = fromMouseClick ? GetRegBoolWithDefault(strKeyName,"Collidable", true) : true;

   m_d.m_enableLightingImage = fromMouseClick ? GetRegBoolWithDefault(strKeyName,"EnableLightingOnImage", true) : true;

   m_d.m_wireDiameter = fromMouseClick ? GetRegStringAsFloatWithDefault(strKeyName,"WireDiameter", 60.0f) : 60.0f;
   m_d.m_wireDistanceX = fromMouseClick ? GetRegStringAsFloatWithDefault(strKeyName,"WireDistanceX", 38.0f) : 38.0f;
   m_d.m_wireDistanceY = fromMouseClick ? GetRegStringAsFloatWithDefault(strKeyName,"WireDistanceY", 88.0f) : 88.0f;
}

void Ramp::WriteRegDefaults()
{
   static const char strKeyName[] = "DefaultProps\\Ramp";

   SetRegValueFloat(strKeyName,"HeightBottom", m_d.m_heightbottom);
   SetRegValueFloat(strKeyName,"HeightTop", m_d.m_heighttop);
   SetRegValueFloat(strKeyName,"WidthBottom", m_d.m_widthbottom);
   SetRegValueFloat(strKeyName,"WidthTop", m_d.m_widthtop);
   SetRegValueInt(strKeyName,"Color", m_d.m_color);
   SetRegValueInt(strKeyName,"RampType", m_d.m_type);
   SetRegValue(strKeyName,"TimerEnabled",REG_DWORD,&m_d.m_tdr.m_fTimerEnabled,4);
   SetRegValue(strKeyName,"TimerInterval",REG_DWORD,&m_d.m_tdr.m_TimerInterval,4);
   SetRegValue(strKeyName,"Image", REG_SZ, &m_d.m_szImage, lstrlen(m_d.m_szImage));
   SetRegValue(strKeyName,"ImageMode",REG_DWORD,&m_d.m_imagealignment,4);
   SetRegValueBool(strKeyName,"ImageWalls",m_d.m_fImageWalls);
   SetRegValueBool(strKeyName,"CastsShadow",m_d.m_fCastsShadow);
   SetRegValueBool(strKeyName,"Transparent",m_d.m_transparent);
   SetRegValueInt(strKeyName,"Opacity", m_d.m_opacity);
   SetRegValueFloat(strKeyName,"LeftWallHeight", m_d.m_leftwallheight);
   SetRegValueFloat(strKeyName,"RightWallHeight", m_d.m_rightwallheight);
   SetRegValueFloat(strKeyName,"LeftWallHeightVisible",m_d.m_leftwallheightvisible);
   SetRegValueFloat(strKeyName,"RightWallHeightVisible",m_d.m_rightwallheightvisible);
   SetRegValueFloat(strKeyName,"Elasticity", m_d.m_elasticity);
   SetRegValueFloat(strKeyName,"Friction", m_d.m_friction);
   SetRegValueFloat(strKeyName,"Scatter", m_d.m_scatter);
   SetRegValueBool(strKeyName,"Collidable",m_d.m_fCollidable);
   SetRegValueBool(strKeyName,"Visible",m_d.m_fVisible);
   SetRegValueBool(strKeyName,"EnableLighingOnImage",m_d.m_enableLightingImage);
   SetRegValueFloat(strKeyName,"WireDiameter", m_d.m_wireDiameter);
   SetRegValueFloat(strKeyName,"WireDistanceX", m_d.m_wireDistanceX);
   SetRegValueFloat(strKeyName,"WireDistanceY", m_d.m_wireDistanceY);
}

void Ramp::GetPointDialogPanes(Vector<PropertyPane> *pvproppane)
{
   PropertyPane *pproppane;

   pproppane = new PropertyPane(IDD_PROPPOINT_VISUALS, IDS_VISUALS);
   pvproppane->AddElement(pproppane);

   pproppane = new PropertyPane(IDD_PROPPOINT_POSITION_WITHZ, IDS_POSITION);
   pvproppane->AddElement(pproppane);
}


void Ramp::PreRender(Sur * const psur)
{
   //make 1 wire ramps look unique in editor - uses ramp color
   if (m_ptable->RenderSolid())
   psur->SetFillColor((m_d.m_type == RampType1Wire) ? m_d.m_color : RGB(192,192,192));
   else
       psur->SetFillColor(-1);
   psur->SetBorderColor(-1,false,0);
   psur->SetObject(this);

   int cvertex;
   Vertex2D * const rgvLocal = GetRampVertex(cvertex, NULL, NULL, NULL, NULL);

   psur->Polygon(rgvLocal, cvertex*2);

   delete [] rgvLocal;
}

void Ramp::Render(Sur * const psur)
{
   psur->SetFillColor(-1);
   psur->SetBorderColor(RGB(0,0,0),false,0);
   psur->SetLineColor(RGB(0,0,0),false,0);
   psur->SetObject(this);
   psur->SetObject(NULL); // NULL so this won't be hit-tested

   int cvertex;
   bool *pfCross;
   Vertex2D *middlePoints;
   const Vertex2D *rgvLocal = GetRampVertex(cvertex, NULL, &pfCross, NULL, &middlePoints);
   psur->Polygon(rgvLocal, cvertex*2);

   if( isHabitrail() )
   {
       psur->Polyline(middlePoints, cvertex);
   if (m_d.m_type == RampType4Wire || m_d.m_type == RampType3WireRight)
   {
      psur->SetLineColor(RGB(0,0,0),false,3);
      psur->Polyline(rgvLocal, cvertex);
   }
   if (m_d.m_type == RampType4Wire || m_d.m_type == RampType3WireLeft)
   {
      psur->SetLineColor(RGB(0,0,0),false,3);
      psur->Polyline(&rgvLocal[cvertex], cvertex);
   }
   }
   else
   {
       for (int i=0;i<cvertex;i++)
           if (pfCross[i])
               psur->Line(rgvLocal[i].x, rgvLocal[i].y, rgvLocal[cvertex*2 - i - 1].x, rgvLocal[cvertex*2 - i - 1].y);

   }
   
   delete [] rgvLocal;
   delete [] pfCross;
   delete [] middlePoints;

   bool fDrawDragpoints = ( (m_selectstate != eNotSelected) || (g_pvp->m_fAlwaysDrawDragPoints) );
   // if the item is selected then draw the dragpoints (or if we are always to draw dragpoints)
   if (!fDrawDragpoints)
   {
      // if any of the dragpoints of this object are selected then draw all the dragpoints
      for (int i=0;i<m_vdpoint.Size();i++)
      {
         const CComObject<DragPoint> * const pdp = m_vdpoint.ElementAt(i);
         if (pdp->m_selectstate != eNotSelected)
         {
            fDrawDragpoints = true;
            break;
         }
      }
   }

   if (fDrawDragpoints)
   {
      for (int i=0;i<m_vdpoint.Size();i++)
      {
         CComObject<DragPoint> * const pdp = m_vdpoint.ElementAt(i);
         psur->SetFillColor(-1);
         psur->SetBorderColor(RGB(255,0,0),false,0);
         psur->SetObject(pdp);

         if (pdp->m_fDragging)
            psur->SetBorderColor(RGB(0,255,0),false,0);

         psur->Ellipse2(pdp->m_v.x, pdp->m_v.y, 8);
      }
   }
}

void Ramp::RenderOutline(Sur * const psur)
{
   psur->SetFillColor(-1);
   psur->SetBorderColor(RGB(0,0,0),false,0);
   psur->SetLineColor(RGB(0,0,0),false,0);
   psur->SetObject(this);
   psur->SetObject(NULL); // NULL so this won't be hit-tested

   int cvertex;
   bool *pfCross;
   Vertex2D *middlePoints;
   const Vertex2D *rgvLocal= GetRampVertex(cvertex, NULL, &pfCross, NULL, &middlePoints);

   psur->Polygon(rgvLocal, cvertex*2);
   if( isHabitrail() )
   {
       psur->Polyline(middlePoints, cvertex);
       if (m_d.m_type == RampType4Wire || m_d.m_type == RampType3WireRight)
       {
           psur->SetLineColor(RGB(0,0,0),false,3);
           psur->Polyline(rgvLocal, cvertex);
       }
       if (m_d.m_type == RampType4Wire || m_d.m_type == RampType3WireLeft)
       {
           psur->SetLineColor(RGB(0,0,0),false,3);
           psur->Polyline(&rgvLocal[cvertex], cvertex);
       }
   }

   for (int i=0;i<cvertex;i++)
      if (pfCross[i])
         psur->Line(rgvLocal[i].x, rgvLocal[i].y, rgvLocal[cvertex*2 - i - 1].x, rgvLocal[cvertex*2 - i - 1].y);

   delete [] rgvLocal;
   delete [] pfCross;
   delete [] middlePoints;
}

void Ramp::RenderBlueprint(Sur *psur)
{
   RenderOutline(psur);
}

void Ramp::RenderShadow(ShadowSur * const psur, const float height)
{
   if (!m_d.m_fCastsShadow || !m_ptable->m_fRenderShadows || !m_d.m_fVisible) 
      return; //skip render if not visible

   psur->SetFillColor(RGB(0,0,0));
   psur->SetBorderColor(-1,false,0);
   psur->SetLineColor(RGB(0,0,0),false,2);
   psur->SetObject(this);

   float *rgheight1;
   int cvertex;
   Vertex2D * const rgvLocal = GetRampVertex(cvertex, &rgheight1, NULL, NULL, NULL);

   // Find the range of vertices to draw a shadow for
   int startvertex = cvertex;
   int stopvertex = 0;
   for (int i=0;i<cvertex;i++)
      if (rgheight1[i] >= height)
      {
         if(i < startvertex)
            startvertex = i;
         stopvertex = i;
      }

   const int range = (stopvertex - startvertex);

   if (range > 0)
   {
      if (isHabitrail())
      {
         float * const rgheight2 = new float[cvertex];

         for (int i=0;i<cvertex;i++)
            rgheight2[i] = rgheight1[cvertex - i - 1];

         if (m_d.m_type != RampType1Wire)
            psur->PolylineSkew(rgvLocal, cvertex, rgheight1, 0, 0);

         psur->PolylineSkew(&rgvLocal[cvertex], cvertex, rgheight2, 0, 0);

         for (int i=0;i<cvertex;i++)
         {
            rgheight1[i] += 44.0f;
            rgheight2[i] += 44.0f;
         }

         if (m_d.m_type == RampType4Wire || m_d.m_type == RampType3WireRight)
            psur->PolylineSkew(rgvLocal, cvertex, rgheight1, 0, 0);

         if (m_d.m_type == RampType4Wire || m_d.m_type == RampType3WireLeft)
            psur->PolylineSkew(&rgvLocal[cvertex], cvertex, rgheight2, 0, 0);

         delete [] rgheight2;
      }
      else
      {
         Vertex2D * const rgv2 = new Vertex2D[range*2];
         float * const rgheight2 = new float[range*2];

         for (int i=0;i<range;i++)
         {
            rgv2[i] = rgvLocal[i + startvertex];
            rgv2[range*2 - i - 1] = rgvLocal[cvertex*2 - i - 1 - startvertex];
            rgheight2[i] = rgheight1[i + startvertex];
            rgheight2[range*2 - i - 1] = rgheight1[i + startvertex];
         }

         psur->PolygonSkew(rgv2, range*2, rgheight2);

         delete [] rgv2;
         delete [] rgheight2;
      }
   }

   delete [] rgvLocal;
   delete [] rgheight1;
}

void Ramp::GetBoundingVertices(Vector<Vertex3Ds> * const pvvertex3D)
{
   float *rgheight1;
   int cvertex;
   const Vertex2D * const rgvLocal = GetRampVertex(cvertex, &rgheight1, NULL, NULL, NULL );

   for (int i=0;i<cvertex;i++)
   {
      {
         Vertex3Ds * const pv = new Vertex3Ds();
         pv->x = rgvLocal[i].x;
         pv->y = rgvLocal[i].y;
         pv->z = rgheight1[i]+50.0f; // leave room for ball
         pvvertex3D->AddElement(pv);
      }

      Vertex3Ds * const pv = new Vertex3Ds();
      pv->x = rgvLocal[cvertex*2-i-1].x;
      pv->y = rgvLocal[cvertex*2-i-1].y;
      pv->z = rgheight1[i]+50.0f; // leave room for ball
      pvvertex3D->AddElement(pv);
   }

   delete [] rgvLocal;
   delete [] rgheight1;
}

/*
 * Compute the vertices and additional information for the ramp shape.
 *
 * Output:
 *  pcvertex     - number of vertices for the central curve
 *  return value - size 2*cvertex, vertices forming the 2D outline of the ramp
 *                 order: first forward along right side of ramp, then backward along the left side
 *  ppheight     - size cvertex, height of the ramp at the i-th vertex
 *  ppfCross     - size cvertex, true if i-th vertex corresponds to a control point
 *  ppratio      - how far along the ramp length the i-th vertex is, 1=start=bottom, 0=end=top (??)
 */
Vertex2D *Ramp::GetRampVertex(int &pcvertex, float ** const ppheight, bool ** const ppfCross, float ** const ppratio, Vertex2D **pMiddlePoints)
{
   Vector<RenderVertex> vvertex;
   GetCentralCurve(&vvertex);
   // vvertex are the 2D vertices forming the central curve of the ramp as seen from above

   const int cvertex = vvertex.size();
   Vertex2D * const rgvLocal = new Vertex2D[cvertex * 2];
   if (ppheight)
   {
      *ppheight = new float[cvertex];
   }
   if (ppfCross)
   {
      *ppfCross = new bool[cvertex];
   }
   if (ppratio)
   {
      *ppratio = new float[cvertex];
   }

   if( pMiddlePoints )
   {
       *pMiddlePoints = new Vertex2D[cvertex];
   }
   // Compute an approximation to the length of the central curve
   // by adding up the lengths of the line segments.
   float totallength = 0;
   for (int i=0; i<(cvertex-1); i++)
   {
      const RenderVertex & v1 = vvertex[i];
      const RenderVertex & v2 = vvertex[i+1];

      const float dx = v1.x - v2.x;
      const float dy = v1.y - v2.y;
      const float length = sqrtf(dx*dx + dy*dy);

      totallength += length;
   }

   float currentlength = 0;
   for (int i=0; i<cvertex; i++)
   {
      const RenderVertex & vprev = vvertex[(i>0) ? i-1 : i];
      const RenderVertex & vnext = vvertex[(i < (cvertex-1)) ? i+1 : i];
      const RenderVertex & vmiddle = vvertex[i];

      Vertex2D vnormal;
      {
         // Get normal at this point
         // Notice that these values equal the ones in the line
         // equation and could probably be substituted by them.
         Vertex2D v1normal(vprev.y - vmiddle.y, vmiddle.x - vprev.x);   // vector vmiddle-vprev rotated RIGHT
         Vertex2D v2normal(vmiddle.y - vnext.y, vnext.x - vmiddle.x);   // vector vnext-vmiddle rotated RIGHT

         if (i == (cvertex-1))
         {
            v1normal.Normalize();
            vnormal = v1normal;
         }
         else if (i == 0)
         {
            v2normal.Normalize();
            vnormal = v2normal;
         }
         else
         {
            v1normal.Normalize();
            v2normal.Normalize();
            if (fabsf(v1normal.x - v2normal.x) < 0.0001f && fabsf(v1normal.y - v2normal.y) < 0.0001f)
            {
               // Two parallel segments
               v1normal.Normalize();
               vnormal = v1normal;
            }
            else
            {
               v1normal.Normalize();
               v2normal.Normalize();

               // Find intersection of the two edges meeting this points, but
               // shift those lines outwards along their normals

               // First line
               const float A = vprev.y - vmiddle.y;
               const float B = vmiddle.x - vprev.x;

               // Shift line along the normal
               const float C = -(A*(vprev.x - v1normal.x) + B*(vprev.y - v1normal.y));

               // Second line
               const float D = vnext.y - vmiddle.y;
               const float E = vmiddle.x - vnext.x;

               // Shift line along the normal
               const float F = -(D*(vnext.x - v2normal.x) + E*(vnext.y - v2normal.y));

               const float det = A*E - B*D;
               const float inv_det = (det != 0.0f) ? 1.0f/det : 0.0f;

               const float intersectx = (B*F-E*C)*inv_det;
               const float intersecty = (C*D-A*F)*inv_det;

               vnormal.x = vmiddle.x - intersectx;
               vnormal.y = vmiddle.y - intersecty;
            }
         }
      }

      // Update current length along the ramp.
      {
         const float dx = vprev.x - vmiddle.x;
         const float dy = vprev.y - vmiddle.y;
         const float length = sqrtf(dx*dx + dy*dy);

         currentlength += length;
      }

      const float percentage = currentlength / totallength;
      const float widthcur = percentage * (m_d.m_widthtop - m_d.m_widthbottom) + m_d.m_widthbottom;

      if (ppheight)
      {
         (*ppheight)[i] = percentage * (m_d.m_heighttop - m_d.m_heightbottom) + m_d.m_heightbottom;
      }

      if (ppratio)
      {
         (*ppratio)[i] = 1.0f - percentage;
      }

      if( pMiddlePoints )
      {
         (*pMiddlePoints)[i] = vmiddle +  vnormal;
      }
      if( isHabitrail())
      {
          if( m_d.m_type==RampType1Wire )
          {
              const float width = m_d.m_wireDiameter*0.5f;
              rgvLocal[i] = vmiddle + (width*0.5f) * vnormal;
              rgvLocal[cvertex*2 - i - 1] = vmiddle - (width*0.5f) * vnormal;
          }
          else
          {
              const float width = (m_d.m_wireDiameter*0.5f)+m_d.m_wireDistanceX;
              rgvLocal[i] = vmiddle + (width*0.5f) * vnormal;
              rgvLocal[cvertex*2 - i - 1] = vmiddle - (width*0.5f) * vnormal;
          }
      }
      else
      {
      rgvLocal[i] = vmiddle + (widthcur*0.5f) * vnormal;
      rgvLocal[cvertex*2 - i - 1] = vmiddle - (widthcur*0.5f) * vnormal;
   }
   }

   if (ppfCross)
      for (int i=0;i<cvertex;i++)
         (*ppfCross)[i] = vvertex.ElementAt(i)->fControlPoint;

   for (int i=0;i<cvertex;i++)
      delete vvertex.ElementAt(i);

   pcvertex = cvertex;
   return rgvLocal;
}

/*
 * Get an approximation of the curve described by the control points of this ramp.
 */
void Ramp::GetCentralCurve(Vector<RenderVertex> * const pvv)
{
   const float accuracy = m_d.m_transparent ? 4.0f*powf(10.0f, (10.0f-m_ptable->GetAlphaRampsAccuracy())*(float)(1.0/1.5))
       : (1.0f / (0.5f * 0.5f)); // min = 4, max = 4 * 10^(10/1.5) = 18.000.000

   IHaveDragPoints::GetRgVertex(pvv, false, accuracy);
}

float Ramp::GetSurfaceHeight(float x, float y)
{
    Vector<RenderVertex> vvertex;
    GetCentralCurve(&vvertex);

    const int cvertex = vvertex.Size();

    int iSeg;
    Vertex2D vOut;
    ClosestPointOnPolygon(vvertex, Vertex2D(x,y), &vOut, &iSeg, false);

    // Go through vertices (including iSeg itself) counting control points until iSeg
    float totallength = 0.f;
    float startlength = 0.f;
    float zheight = 0.f;

    if (iSeg == -1)
   {
        //zheight = 0;
        goto HeightError;
        //return 0; // Object is not on ramp path
    }

    for (int i2=1;i2<cvertex;i2++)
    {
        const float dx = vvertex.ElementAt(i2)->x - vvertex.ElementAt(i2-1)->x;
        const float dy = vvertex.ElementAt(i2)->y - vvertex.ElementAt(i2-1)->y;
        const float len = sqrtf(dx*dx + dy*dy);
        if (i2 <= iSeg)
        {
            startlength += len;
        }
        totallength += len;
    }

    {
        const float dx = vOut.x - vvertex.ElementAt(iSeg)->x;
        const float dy = vOut.y - vvertex.ElementAt(iSeg)->y;
        const float len = sqrtf(dx*dx + dy*dy);
        startlength += len; // Add the distance the object is between the two closest polyline segments.  Matters mostly for straight edges.

        zheight = (startlength/totallength) * (m_d.m_heighttop - m_d.m_heightbottom) + m_d.m_heightbottom;
   }
HeightError:
    for (int i2=0;i2<cvertex;i2++)
        delete vvertex.ElementAt(i2);

    return zheight;
}

void Ramp::GetTimers(Vector<HitTimer> * const pvht)
{
   IEditable::BeginPlay();

   HitTimer * const pht = new HitTimer();
   pht->m_interval = m_d.m_tdr.m_TimerInterval;
   pht->m_nextfire = pht->m_interval;
   pht->m_pfe = (IFireEvents *)this;

   m_phittimer = pht;

   if (m_d.m_tdr.m_fTimerEnabled)
      pvht->AddElement(pht);
}

void Ramp::GetHitShapes(Vector<HitObject> * const pvho)
{
   int cvertex;
   float *rgheight1;
   Vertex2D * const rgvLocal = GetRampVertex(cvertex, &rgheight1, NULL, NULL, NULL);

   float wallheightright, wallheightleft;

   if (m_d.m_type == RampTypeFlat)
   {
      wallheightright = m_d.m_rightwallheight;
      wallheightleft = m_d.m_leftwallheight;
   }
   else if (m_d.m_type == RampType1Wire) //add check for 1 wire
   {
      // backwards compatible physics
      wallheightright = 31.0f;
      wallheightleft = 31.0f;
   }
   else if (m_d.m_type == RampType2Wire)
   {
      // backwards compatible physics
      wallheightright = 31.0f;
      wallheightleft = 31.0f;
   }
   else if (m_d.m_type == RampType4Wire)
   {
      wallheightleft = 62.0f;
      wallheightright = 62.0f;
   }
   else if (m_d.m_type == RampType3WireRight)
   {
      wallheightright = 62.0f;
      wallheightleft = (float)(6+12.5);
   }
   else if (m_d.m_type == RampType3WireLeft)
   {
      wallheightleft = 62.0f;
      wallheightright = (float)(6+12.5);
   }

   // Add line segments for right ramp wall.
   if (wallheightright > 0.f)
   {
      for (int i=0;i<(cvertex-1);i++)
      {
         const Vertex2D * const pv1 = (i>0) ? &rgvLocal[i-1] : NULL;
         const Vertex2D * const pv2 = &rgvLocal[i];
         const Vertex2D * const pv3 = &rgvLocal[i+1];
         const Vertex2D * const pv4 = (i<(cvertex-2)) ? &rgvLocal[i+2] : NULL;

#ifndef RAMPTEST
         AddLine(pvho, pv2, pv3, pv1, rgheight1[i], rgheight1[i+1]+wallheightright);
         AddLine(pvho, pv3, pv2, pv4, rgheight1[i], rgheight1[i+1]+wallheightright);
#else
         AddSideWall(pvho, pv2, pv3,rgheight1[i], rgheight1[i+1], wallheightright);
         AddSideWall(pvho, pv3, pv2,rgheight1[i+1], rgheight1[i], wallheightright);
#endif

         // add joints at start and end of right wall
         if (i == 0)
             AddJoint2D(pvho, *pv2, rgheight1[0], rgheight1[0] + wallheightright);
         else if (i == cvertex-2)
             AddJoint2D(pvho, *pv3, rgheight1[cvertex-1], rgheight1[cvertex-1] + wallheightright);
      }
   }

   // Add line segments for left ramp wall.
   if (wallheightleft > 0.f)
   {
      for (int i=0;i<(cvertex-1);i++)
      {
         const Vertex2D * const pv1 = (i>0) ? &rgvLocal[cvertex + i - 1] : NULL;
         const Vertex2D * const pv2 = &rgvLocal[cvertex + i];
         const Vertex2D * const pv3 = &rgvLocal[cvertex + i + 1];
         const Vertex2D * const pv4 = (i<(cvertex-2)) ? &rgvLocal[cvertex + i + 2] : NULL;

#ifndef RAMPTEST
         AddLine(pvho, pv2, pv3, pv1, rgheight1[cvertex - i - 2], rgheight1[cvertex - i - 1] + wallheightleft);
         AddLine(pvho, pv3, pv2, pv4, rgheight1[cvertex - i - 2], rgheight1[cvertex - i - 1] + wallheightleft);
#else
         AddSideWall(pvho, pv2, pv3, rgheight1[cvertex - i - 1], rgheight1[cvertex - i - 2], wallheightleft);
         AddSideWall(pvho, pv3, pv2, rgheight1[cvertex - i - 2], rgheight1[cvertex - i - 1], wallheightleft);
#endif

         // add joints at start and end of left wall
         if (i == 0)
             AddJoint2D(pvho, *pv2, rgheight1[cvertex-1], rgheight1[cvertex-1] + wallheightleft);
         else if (i == cvertex-2)
             AddJoint2D(pvho, *pv3, rgheight1[0], rgheight1[0] + wallheightleft);
      }
   }

#ifndef RAMPTEST
   // Add hit triangles for the ramp floor.
   {
      HitTriangle *ph3dpolyOld = NULL;
      const Vertex2D *pv1, *pv2, *pv3, *pv4;

      for (int i=0;i<(cvertex-1);i++)
      {
         /*
          * Layout of one ramp quad seen from above, ramp direction is bottom to top:
          *
          *    3 - - 4
          *    | \   |
          *    |   \ |
          *    2 - - 1
          */
         pv1 = &rgvLocal[i];                    // i-th right
         pv2 = &rgvLocal[cvertex*2 - i - 1];    // i-th left
         pv3 = &rgvLocal[cvertex*2 - i - 2];    // (i+1)-th left
         pv4 = &rgvLocal[i+1];                  // (i+1)-th right

         {
            // left ramp floor triangle, CCW order
            Vertex3Ds rgv3D[3];
            rgv3D[0] = Vertex3Ds(pv2->x,pv2->y,rgheight1[i]);
            rgv3D[1] = Vertex3Ds(pv1->x,pv1->y,rgheight1[i]);
            rgv3D[2] = Vertex3Ds(pv3->x,pv3->y,rgheight1[i+1]);

            // add joint for starting edge of ramp
            if (i == 0)
                AddJoint(pvho, rgv3D[0], rgv3D[1]);

            // add joint for left edge
            AddJoint(pvho, rgv3D[0], rgv3D[2]);

            HitTriangle * const ph3dpoly = new HitTriangle(rgv3D); //!! this is not efficient at all, use native triangle-soup directly somehow

            if (ph3dpoly->IsDegenerate())       // degenerate triangles happen if width is 0 at some point
            {
                delete ph3dpoly;
            }
            else
            {
                ph3dpoly->m_elasticity = m_d.m_elasticity;
                ph3dpoly->SetFriction(m_d.m_friction);
                ph3dpoly->m_scatter = ANGTORAD(m_d.m_scatter);

                if (m_d.m_type == RampTypeFlat)
                    ph3dpoly->m_fVisible = fTrue;

                pvho->AddElement(ph3dpoly);

                m_vhoCollidable.push_back(ph3dpoly);	//remember hit components of ramp
                ph3dpoly->m_fEnabled = m_d.m_fCollidable;

                CheckJoint(pvho, ph3dpolyOld, ph3dpoly);
                ph3dpolyOld = ph3dpoly;
            }
         }

         // right ramp floor triangle, CCW order
         Vertex3Ds rgv3D[3];
         rgv3D[0] = Vertex3Ds(pv3->x,pv3->y,rgheight1[i+1]);
         rgv3D[1] = Vertex3Ds(pv1->x,pv1->y,rgheight1[i]);
         rgv3D[2] = Vertex3Ds(pv4->x,pv4->y,rgheight1[i+1]);

         // add joint for right edge
         AddJoint(pvho, rgv3D[1], rgv3D[2]);

         HitTriangle * const ph3dpoly = new HitTriangle(rgv3D);
         if (ph3dpoly->IsDegenerate())
         {
             delete ph3dpoly;
         }
         else
         {
             ph3dpoly->m_elasticity = m_d.m_elasticity;
             ph3dpoly->SetFriction(m_d.m_friction);
             ph3dpoly->m_scatter = ANGTORAD(m_d.m_scatter);

             if (m_d.m_type == RampTypeFlat)
                 ph3dpoly->m_fVisible = fTrue;

             pvho->AddElement(ph3dpoly);

             m_vhoCollidable.push_back(ph3dpoly);	//remember hit components of ramp
             ph3dpoly->m_fEnabled = m_d.m_fCollidable;

             CheckJoint(pvho, ph3dpolyOld, ph3dpoly);
             ph3dpolyOld = ph3dpoly;
         }
      }

      // add joint for final edge of ramp
      Vertex3Ds v1(pv4->x,pv4->y,rgheight1[cvertex-1]);
      Vertex3Ds v2(pv3->x,pv3->y,rgheight1[cvertex-1]);
      AddJoint(pvho, v1, v2);

      ph3dpolyOld = NULL;
   }

   // add outside bottom, 
   // joints at the intersections are not needed since the inner surface has them
   // this surface is identical... except for the direction of the normal face.
   // hence the joints protect both surface edges from having a fall through

   for (int i=0; i<(cvertex-1); i++)
   {
      // see sketch above
      const Vertex2D * const pv1 = &rgvLocal[i];
      const Vertex2D * const pv2 = &rgvLocal[cvertex*2 - i - 1];
      const Vertex2D * const pv3 = &rgvLocal[cvertex*2 - i - 2];
      const Vertex2D * const pv4 = &rgvLocal[i+1];

      {
         // left ramp triangle, order CW
         const Vertex3Ds rgv3D[3] = {
			Vertex3Ds(pv1->x,pv1->y,rgheight1[i]),
			Vertex3Ds(pv2->x,pv2->y,rgheight1[i]),
			Vertex3Ds(pv3->x,pv3->y,rgheight1[i+1])};

         HitTriangle * const ph3dpoly = new HitTriangle(rgv3D);
         if (ph3dpoly->IsDegenerate())
         {
             delete ph3dpoly;
         }
         else
         {
             ph3dpoly->m_elasticity = m_d.m_elasticity;
             ph3dpoly->SetFriction(m_d.m_friction);
             ph3dpoly->m_scatter = ANGTORAD(m_d.m_scatter);

             pvho->AddElement(ph3dpoly);

             m_vhoCollidable.push_back(ph3dpoly);	//remember hit components of ramp
             ph3dpoly->m_fEnabled = m_d.m_fCollidable;
         }
      }

      // right ramp triangle, order CW
      const Vertex3Ds rgv3D[3] = {
		Vertex3Ds(pv3->x,pv3->y,rgheight1[i+1]),
		Vertex3Ds(pv4->x,pv4->y,rgheight1[i+1]),
		Vertex3Ds(pv1->x,pv1->y,rgheight1[i])};

      HitTriangle * const ph3dpoly = new HitTriangle(rgv3D);
      if (ph3dpoly->IsDegenerate())
      {
          delete ph3dpoly;
      }
      else
      {
          ph3dpoly->m_elasticity = m_d.m_elasticity;
          ph3dpoly->SetFriction(m_d.m_friction);
          ph3dpoly->m_scatter = ANGTORAD(m_d.m_scatter);

          pvho->AddElement(ph3dpoly);

          m_vhoCollidable.push_back(ph3dpoly);	//remember hit components of ramp
          ph3dpoly->m_fEnabled = m_d.m_fCollidable;
      }
   }
#endif

   delete [] rgheight1;
   delete [] rgvLocal;
}

void Ramp::GetHitShapesDebug(Vector<HitObject> * const pvho)
{
}

#ifdef RAMPTEST
void Ramp::AddSideWall(Vector<HitObject> * const pvho, const Vertex2D * const pv1, const Vertex2D * const pv2, const float height1, const float height2, const float wallheight)
{
   Vertex3Ds * const rgv3D = new Vertex3Ds[4];
   rgv3D[0] = Vertex3Ds(pv1->x,pv1->y,height1 - (float)PHYS_SKIN);
   rgv3D[1] = Vertex3Ds(pv2->x,pv2->y,height2 - (float)PHYS_SKIN);
   rgv3D[2] = Vertex3Ds(pv2->x + WALLTILT,pv2->y + WALLTILT,height2 + wallheight);
   rgv3D[3] = Vertex3Ds(pv1->x + WALLTILT,pv1->y + WALLTILT,height1 + wallheight);

   Hit3DPoly * const ph3dpoly = new Hit3DPoly(rgv3D,4); //!!
   ph3dpoly->m_elasticity = m_d.m_elasticity;
   ph3dpoly->SetFriction(m_d.m_friction);
   ph3dpoly->m_scatter = ANGTORAD(m_d.m_scatter);

   pvho->AddElement(ph3dpoly);

   m_vhoCollidable.push_back(ph3dpoly);	//remember hit components of ramp
   ph3dpoly->m_fEnabled = m_d.m_fCollidable;
}
#endif

void Ramp::CheckJoint(Vector<HitObject> * const pvho, const HitTriangle * const ph3d1, const HitTriangle * const ph3d2)
{
   if (ph3d1)   // may be null in case of degenerate triangles
   {
       const Vertex3Ds vjointnormal = CrossProduct(ph3d1->normal, ph3d2->normal);
       if (vjointnormal.LengthSquared() < 1e-8f)
           return;  // coplanar triangles need no joints
   }

   // By convention of the calling function, points 1 [0] and 2 [1] of the second polygon will
   // be the common-edge points
   AddJoint(pvho, ph3d2->m_rgv[0], ph3d2->m_rgv[1]);
}

void Ramp::AddJoint(Vector<HitObject> * pvho, const Vertex3Ds& v1, const Vertex3Ds& v2)
{
   HitLine3D * const ph3dc = new HitLine3D(v1, v2);
   ph3dc->m_elasticity = m_d.m_elasticity;
   ph3dc->SetFriction(m_d.m_friction);
   ph3dc->m_scatter = ANGTORAD(m_d.m_scatter);
   ph3dc->m_fEnabled = m_d.m_fCollidable;

   pvho->AddElement(ph3dc);
   m_vhoCollidable.push_back(ph3dc);	//remember hit components of ramp
}

void Ramp::AddJoint2D(Vector<HitObject> * pvho, const Vertex2D& p, float zlow, float zhigh)
{
    HitLineZ * const pjoint = new HitLineZ(p, zlow, zhigh);
    pjoint->m_elasticity = m_d.m_elasticity;
    pjoint->SetFriction(m_d.m_friction);
    pjoint->m_scatter = ANGTORAD(m_d.m_scatter);
    pjoint->m_fEnabled = m_d.m_fCollidable;

    pvho->AddElement(pjoint);
    m_vhoCollidable.push_back(pjoint); //remember hit components of ramp
}


void Ramp::AddLine(Vector<HitObject> * const pvho, const Vertex2D * const pv1, const Vertex2D * const pv2, const Vertex2D * const pv3, const float height1, const float height2)
{
   LineSeg * const plineseg = new LineSeg(*pv1, *pv2);
   plineseg->m_elasticity = m_d.m_elasticity;
   plineseg->SetFriction(m_d.m_friction);
   plineseg->m_scatter = ANGTORAD(m_d.m_scatter);
   plineseg->m_fEnabled = m_d.m_fCollidable;
   plineseg->m_pfe = NULL;
   plineseg->m_rcHitRect.zlow = height1;
   plineseg->m_rcHitRect.zhigh = height2;

   pvho->AddElement(plineseg);

   m_vhoCollidable.push_back(plineseg);	//remember hit components of ramp

   if (pv3)
   {
      const Vertex2D vt1 = *pv1 - *pv2;
      const Vertex2D vt2 = *pv1 - *pv3;
      const float dot = vt1.Dot(vt2);

      if (dot < 0.f) // Inside edges don't need joint hit-testing (dot == 0 continuous segments should mathematically never hit)
          AddJoint2D(pvho, *pv1, height1, height2);
   }
}

void Ramp::EndPlay()
{
    IEditable::EndPlay();
    m_vhoCollidable.clear();

   	if(staticVertexBuffer) {
		staticVertexBuffer->release();
		staticVertexBuffer = 0;
	}

	if(dynamicVertexBuffer) {
		dynamicVertexBuffer->release();
		dynamicVertexBuffer = 0;
		dynamicVertexBufferRegenerate = true;
	}

	if(dynamicIndexBuffer) {
		dynamicIndexBuffer->release();
		dynamicIndexBuffer = 0;
    }
}

static const WORD rgicrosssection[] = {
   0,1,16,
   1,17,16,
   1,2,17,
   2,18,17,
   2,3,18,
   3,19,18,
   3,0,19,
   0,16,19,

   8,9,24,
   9,25,24,
   9,10,25,
   10,26,25,
   10,11,26,
   11,27,26,
   11,8,27,
   8,24,27,

   4,5,20,
   5,21,20,
   5,6,21,
   6,22,21,
   6,7,22,
   7,23,22,
   7,4,23,
   4,20,23,

   12,13,28,
   13,29,28,
   13,14,29,
   14,30,29,
   14,15,30,
   15,31,30,
   15,12,31,
   12,28,31
};


float Ramp::GetDepth(const Vertex3Ds& viewDir) const
{
    // TODO: improve
    Vertex2D center2D;
    GetCenter(&center2D);
    const float centerZ = 0.5f * (m_d.m_heightbottom + m_d.m_heighttop);
    return m_d.m_depthBias + viewDir.x * center2D.x + viewDir.y * center2D.y + viewDir.z * centerZ;
}


bool Ramp::isHabitrail() const
{
    return  m_d.m_type == RampType4Wire
         || m_d.m_type == RampType1Wire
         || m_d.m_type == RampType2Wire
         || m_d.m_type == RampType3WireLeft
         || m_d.m_type == RampType3WireRight;
}

void Ramp::RenderStaticHabitrail(RenderDevice* pd3dDevice)
{
   pd3dDevice->SetRenderState(RenderDevice::SPECULARENABLE, TRUE);
   
   Pin3D * const ppin3d = &g_pplayer->m_pin3d;
   Texture * const pin = m_ptable->GetImage(m_d.m_szImage);

   if ( !pin )
   {
       Material habitrailMaterial;
       pd3dDevice->basicShader->Core()->SetFloat("vMaterialPower",8.0f);
       pd3dDevice->basicShader->Core()->SetBool("bSpecular", true);
       pd3dDevice->basicShader->Core()->SetTechnique("basic_without_texture");
   }
   else
   {
       pin->CreateAlphaChannel();
       D3DXVECTOR4 color(1.0f,1.0f,1.0f,1.0f);   
       pd3dDevice->basicShader->Core()->SetVector("vDiffuseColor",&color);
       pd3dDevice->basicShader->SetTexture("Texture0",pin);
       pd3dDevice->basicShader->Core()->SetTechnique("basic_with_texture");

       g_pplayer->m_pin3d.SetTextureFilter(ePictureTexture, TEXTURE_MODE_TRILINEAR);
}
   pd3dDevice->basicShader->Begin(0);

   int offset=0;
   for (int i=0; i<rampVertex-1; i++,offset+=32)
   {
      RenderPolygons(pd3dDevice, offset, (WORD*)rgicrosssection, 0, 16);
      if (m_d.m_type == RampType4Wire || m_d.m_type == RampType3WireRight)
         RenderPolygons(pd3dDevice, offset, (WORD*)rgicrosssection, 16, 24);

      if (m_d.m_type == RampType4Wire || m_d.m_type == RampType3WireLeft)
         RenderPolygons(pd3dDevice, offset, (WORD*)rgicrosssection, 24, 32);
   }
   pd3dDevice->basicShader->End();  

   pd3dDevice->SetRenderState(RenderDevice::SPECULARENABLE, FALSE);
   if ( !pin )
       pd3dDevice->basicShader->Core()->SetBool("bSpecular", false);

}

void Ramp::RenderPolygons(RenderDevice* pd3dDevice, int offset, WORD * const rgi, const int start, const int stop)
{
   if (m_d.m_type == RampType1Wire)
      pd3dDevice->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, staticVertexBuffer, offset, 32, rgi+stop/2*3, 3*(stop-stop/2));
   else
      pd3dDevice->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, staticVertexBuffer, offset, 32, rgi+start*3, 3*(stop-start));
}

void Ramp::prepareHabitrail(RenderDevice* pd3dDevice )
{
   Matrix3D matView = g_pplayer->m_pin3d.GetViewTransform();
   Vertex3Ds prevTangent;
   Vertex2D *middlePoints;

   rgvInit = GetRampVertex(rampVertex, &rgheightInit, NULL, &rgratioInit, &middlePoints);

   const int numVertices = (rampVertex - 1)*32;
   pd3dDevice->CreateVertexBuffer(numVertices, 0, MY_D3DFVF_NOTEX2_VERTEX, &staticVertexBuffer);

   Vertex3D_NoTex2 *buf;
   staticVertexBuffer->lock(0,0,(void**)&buf, VertexBuffer::WRITEONLY);

   int offset=0;
   float l;
   Vertex3D_NoTex2 rgv3D[32];
   const float halfDiameter=m_d.m_wireDiameter/2.0f;
   for (int i=0;i<rampVertex;i++)
   {
      rgv3D[0].x = -halfDiameter;
      rgv3D[0].y = -halfDiameter;
      rgv3D[0].z = 0;
      rgv3D[0].nx = 1.0f;
      rgv3D[0].ny = 1.0f;
      rgv3D[0].nz = 0;
      l = 1.0f/sqrtf(rgv3D[0].nx*rgv3D[0].nx + rgv3D[0].ny*rgv3D[0].ny + rgv3D[0].nz*rgv3D[0].nz);
      rgv3D[0].nx *= l;
      rgv3D[0].ny *= l;
      rgv3D[0].nz *= l;

      rgv3D[1].x = halfDiameter;
      rgv3D[1].y = -halfDiameter;
      rgv3D[1].z = 0;
      rgv3D[1].nx = -1.0f;
      rgv3D[1].ny = 1.0f;
      rgv3D[1].nz = 0;
      l = 1.0f/sqrtf(rgv3D[1].nx*rgv3D[1].nx + rgv3D[1].ny*rgv3D[1].ny + rgv3D[1].nz*rgv3D[1].nz);
      rgv3D[1].nx *= l;
      rgv3D[1].ny *= l;
      rgv3D[1].nz *= l;

      rgv3D[2].x = halfDiameter;
      rgv3D[2].y = halfDiameter;
      rgv3D[2].z = 0;
      rgv3D[2].nx = -1.0f;
      rgv3D[2].ny = -1.0f;
      rgv3D[2].nz = 0;
      l = 1.0f/sqrtf(rgv3D[2].nx*rgv3D[2].nx + rgv3D[2].ny*rgv3D[2].ny + rgv3D[2].nz*rgv3D[2].nz);
      rgv3D[2].nx *= l;
      rgv3D[2].ny *= l;
      rgv3D[2].nz *= l;

      rgv3D[3].x = -halfDiameter;
      rgv3D[3].y = halfDiameter;
      rgv3D[3].z = 0;
      rgv3D[3].nx = 1.0f;
      rgv3D[3].ny = -1.0f;
      rgv3D[3].nz = 0;
      l = 1.0f/sqrtf(rgv3D[3].nx*rgv3D[3].nx + rgv3D[3].ny*rgv3D[3].ny + rgv3D[3].nz*rgv3D[3].nz);
      rgv3D[3].nx *= l;
      rgv3D[3].ny *= l;
      rgv3D[3].nz *= l;

      const float halfWireDistanceY=m_d.m_wireDistanceY/2.0f;
      const float halfWireDistanceX=m_d.m_wireDistanceX/2.0f;
      const float wireOffset=9.5f;
      if (m_d.m_type != RampType1Wire)
      {
         for (int l=0;l<4;l++)
         {
            rgv3D[l+ 4].x = rgv3D[l].x + halfWireDistanceY; //44.0f
            rgv3D[l+ 4].y = rgv3D[l].y - halfWireDistanceX; //22.0f
            rgv3D[l+ 4].z = rgv3D[l].z;
            rgv3D[l+ 8].x = rgv3D[l].x + wireOffset;
            rgv3D[l+ 8].y = rgv3D[l].y + halfWireDistanceX;
            rgv3D[l+ 8].z = rgv3D[l].z;
            rgv3D[l+12].x = rgv3D[l].x + halfWireDistanceY;
            rgv3D[l+12].y = rgv3D[l].y + halfWireDistanceX;
            rgv3D[l+12].z = rgv3D[l].z;

            rgv3D[l+ 4].nx = rgv3D[l].nx;
            rgv3D[l+ 4].ny = rgv3D[l].ny;
            rgv3D[l+ 4].nz = rgv3D[l].nz;
            rgv3D[l+ 8].nx = rgv3D[l].nx;
            rgv3D[l+ 8].ny = rgv3D[l].ny;
            rgv3D[l+ 8].nz = rgv3D[l].nz;
            rgv3D[l+12].nx = rgv3D[l].nx;
            rgv3D[l+12].ny = rgv3D[l].ny;
            rgv3D[l+12].nz = rgv3D[l].nz;

            rgv3D[l+ 4].tu = rgv3D[l].tu;
            rgv3D[l+ 4].tv = rgv3D[l].tv;
            rgv3D[l+ 8].tu = rgv3D[l].tu;
            rgv3D[l+ 8].tv = rgv3D[l].tv;
            rgv3D[l+12].tu = rgv3D[l].tu;
            rgv3D[l+12].tv = rgv3D[l].tv;
         }
         for (int l=0;l<4;l++)
         {
            rgv3D[l].x += wireOffset;
            rgv3D[l].y += -halfWireDistanceX;
         }
      }
      else
      {
         for (int l=0;l<4;l++)
         {
            rgv3D[l+ 4].x = rgv3D[l].x+halfWireDistanceY; //44.0f
            rgv3D[l+ 4].y = rgv3D[l].y;
            rgv3D[l+ 4].z = rgv3D[l].z;
            rgv3D[l+ 8].x = rgv3D[l].x + wireOffset;
            rgv3D[l+ 8].y = rgv3D[l].y;
            rgv3D[l+ 8].z = rgv3D[l].z;
            rgv3D[l+12].x = rgv3D[l].x+halfWireDistanceY;
            rgv3D[l+12].y = rgv3D[l].y;
            rgv3D[l+12].z = rgv3D[l].z;

            rgv3D[l+ 4].nx = rgv3D[l].nx;
            rgv3D[l+ 4].ny = rgv3D[l].ny;
            rgv3D[l+ 4].nz = rgv3D[l].nz;
            rgv3D[l+ 8].nx = rgv3D[l].nx;
            rgv3D[l+ 8].ny = rgv3D[l].ny;
            rgv3D[l+ 8].nz = rgv3D[l].nz;
            rgv3D[l+12].nx = rgv3D[l].nx;
            rgv3D[l+12].ny = rgv3D[l].ny;
            rgv3D[l+12].nz = rgv3D[l].nz;

            rgv3D[l+ 4].tu = rgv3D[l].tu;
            rgv3D[l+ 4].tv = rgv3D[l].tv;
            rgv3D[l+ 8].tu = rgv3D[l].tu;
            rgv3D[l+ 8].tv = rgv3D[l].tv;
            rgv3D[l+12].tu = rgv3D[l].tu;
            rgv3D[l+12].tv = rgv3D[l].tv;
         }
      }

      const int p1 = (i==0) ? 0 : (i-1);
      const int p2 = i;
      const int p3 = (i==(rampVertex-1)) ? i : (i+1);
      const int p4 = rampVertex*2 - i - 1; //!! ?? *2 valid?

      Vertex3Ds vacross(rgvInit[p4].x - rgvInit[p2].x, rgvInit[p4].y - rgvInit[p2].y, 0.0f);

      // The vacross vector is our local up vector.  Rotate the cross-section
      // later to match this up
      vacross.Normalize();

      Vertex3Ds tangent(rgvInit[p3].x - rgvInit[p1].x, rgvInit[p3].y - rgvInit[p1].y, rgheightInit[p3] - rgheightInit[p1]);

      // This is the vector describing the tangent to the ramp at this point
      tangent.Normalize();

      /* Vertex3Ds up(0,0,1);
      // Get axis of rotation to rotate our cross-section into place
      CrossProduct(tangent, up, &rotationaxis);*/
      Vertex3Ds rotationaxis(tangent.y, -tangent.x, 0.0f);
      if (rotationaxis.LengthSquared() <= 1e-6f)
          rotationaxis.Set(1, 0, 0);
      else
          rotationaxis.Normalize();

      const float dot = tangent.z; //tangent.Dot(&up);
      const float angle = acosf(dot);

      RotateAround(rotationaxis, rgv3D, 16, angle);

      // vnewup is the beginning up vector of the cross-section
      const Vertex2D vnewupdef(0.0f,1.0f);
      const Vertex3Ds vnewup = RotateAround(rotationaxis, vnewupdef, angle);

      // vacross is not out real up vector, but the up vector for the cross-section isn't real either
      //Vertex3D vrampup;
      //CrossProduct(&tangent, &vacross, &vrampup);
      const float dotupcorrection = vnewup.Dot(vacross);
      float angleupcorrection = acosf(dotupcorrection);

      if (vacross.x >= 0.f)
         angleupcorrection = -angleupcorrection;

      RotateAround(tangent, rgv3D, 16, -angleupcorrection);
      for (int l=0;l<16;l++)
      {
         rgv3D[l].x += middlePoints[i].x;
         rgv3D[l].y += middlePoints[i].y;
         rgv3D[l].z += rgheightInit[i]*m_ptable->m_zScale;
      }

      // apply environment texture coords if a texture was assigned to a wired ramp
      for( int k=0;k<16;k++ )
      {
          Vertex3Ds norm(rgv3D[k].nx, rgv3D[k].ny, rgv3D[k].nz);
          matView.MultiplyVectorNoTranslate(norm, norm);
          rgv3D[k].tu = 0.5f + norm.x*0.5f;
          rgv3D[k].tv = 0.5f + norm.y*0.5f;
      }
      if (i != 0)
      {
         memcpy( &buf[offset], rgv3D, sizeof(Vertex3D_NoTex2)*32);
         offset+=32;
      }
      memcpy(&rgv3D[16], rgv3D, sizeof(Vertex3D_NoTex2)*16);
   }
   staticVertexBuffer->unlock();  

   delete[] rgvInit;
   delete[] rgheightInit;
   delete[] rgratioInit;

   assert(offset == numVertices);

}

static const WORD rgiRampStatic1[4] = {0,3,2,1};

void Ramp::prepareStatic(RenderDevice* pd3dDevice)
{
   rgvInit = GetRampVertex(rampVertex, &rgheightInit, NULL, &rgratioInit, NULL);

   const int numVertices = (rampVertex-1)*4*5;
   pd3dDevice->CreateVertexBuffer( numVertices, 0, MY_D3DFVF_NOTEX2_VERTEX, &staticVertexBuffer);

   Pin3D *const ppin3d = &g_pplayer->m_pin3d;

   Texture * const pin = m_ptable->GetImage(m_d.m_szImage);

   const float tablewidth = m_ptable->m_right - m_ptable->m_left;
   const float tableheight = m_ptable->m_bottom - m_ptable->m_top;

   const float scalewidth  = (float) g_pplayer->m_pin3d.m_dwRenderWidth  * (float)(1.0/64.055);		// 64.0f is texture width.			
   const float scaleheight = (float) g_pplayer->m_pin3d.m_dwRenderHeight * (float)(1.0/64.055);		// 64.0f is texture height.

   const float inv_width = scalewidth / (float)g_pplayer->m_pin3d.m_dwRenderWidth;
   const float inv_height = scaleheight / (float)g_pplayer->m_pin3d.m_dwRenderHeight;

   const float inv_width2 = 1.0f / tablewidth;
   const float inv_height2 = 1.0f / tableheight;

   Vertex3D_NoTex2 *buf;
   staticVertexBuffer->lock(0,0,(void**)&buf, VertexBuffer::WRITEONLY);

   int offset=0;
   for (int i=0; i<(rampVertex-1); i++)
   {
      Vertex3D_NoTex2 rgv3D[4];
      rgv3D[0].x = rgvInit[i].x;
      rgv3D[0].y = rgvInit[i].y;
      rgv3D[0].z = rgheightInit[i]*m_ptable->m_zScale;

      rgv3D[3].x = rgvInit[i+1].x;
      rgv3D[3].y = rgvInit[i+1].y;
      rgv3D[3].z = rgheightInit[i+1]*m_ptable->m_zScale;

      rgv3D[2].x = rgvInit[rampVertex*2-i-2].x;
      rgv3D[2].y = rgvInit[rampVertex*2-i-2].y;
      rgv3D[2].z = rgheightInit[i+1]*m_ptable->m_zScale;

      rgv3D[1].x = rgvInit[rampVertex*2-i-1].x;
      rgv3D[1].y = rgvInit[rampVertex*2-i-1].y;
      rgv3D[1].z = rgheightInit[i]*m_ptable->m_zScale;

      if (pin)
      {
         if (m_d.m_imagealignment == ImageModeWorld)
         {
               rgv3D[0].tu = rgv3D[0].x * inv_width2;
               rgv3D[0].tv = rgv3D[0].y * inv_height2;
               rgv3D[1].tu = rgv3D[1].x * inv_width2;
               rgv3D[1].tv = rgv3D[1].y * inv_height2;
               rgv3D[2].tu = rgv3D[2].x * inv_width2;
               rgv3D[2].tv = rgv3D[2].y * inv_height2;
               rgv3D[3].tu = rgv3D[3].x * inv_width2;
               rgv3D[3].tv = rgv3D[3].y * inv_height2;
            }
         else
         {
            rgv3D[0].tu = 1.0f;
            rgv3D[0].tv = rgratioInit[i];
            rgv3D[1].tu = 0;
            rgv3D[1].tv = rgratioInit[i];
            rgv3D[2].tu = 0;
            rgv3D[2].tv = rgratioInit[i+1];
            rgv3D[3].tu = 1.0f;
            rgv3D[3].tv = rgratioInit[i+1];
         }
      }

      SetNormal(rgv3D, rgi0123, 4, NULL, NULL, NULL);
      // Draw the floor of the ramp.
      memcpy( &buf[offset], rgv3D, sizeof(Vertex3D_NoTex2)*4 );
      offset+=4;
   }

   for (int i=0;i<(rampVertex-1);i++)
   {
      Vertex3D_NoTex2 rgv3D[4];
      rgv3D[0].x = rgvInit[i].x;
      rgv3D[0].y = rgvInit[i].y;
      rgv3D[0].z = rgheightInit[i]*m_ptable->m_zScale;

      rgv3D[3].x = rgvInit[i+1].x;
      rgv3D[3].y = rgvInit[i+1].y;
      rgv3D[3].z = rgheightInit[i+1]*m_ptable->m_zScale;

      rgv3D[2].x = rgvInit[i+1].x;
      rgv3D[2].y = rgvInit[i+1].y;
      rgv3D[2].z = (rgheightInit[i+1] + m_d.m_rightwallheightvisible)*m_ptable->m_zScale;

      rgv3D[1].x = rgvInit[i].x;
      rgv3D[1].y = rgvInit[i].y;
      rgv3D[1].z = (rgheightInit[i] + m_d.m_rightwallheightvisible)*m_ptable->m_zScale;

      if (pin && m_d.m_fImageWalls)
      {
         if (m_d.m_imagealignment == ImageModeWorld)
         {
               rgv3D[0].tu = rgv3D[0].x * inv_width2;
               rgv3D[0].tv = rgv3D[0].y * inv_height2;
               rgv3D[2].tu = rgv3D[2].x * inv_width2;
               rgv3D[2].tv = rgv3D[2].y * inv_height2;

               rgv3D[1].tu = rgv3D[0].tu;
               rgv3D[1].tv = rgv3D[0].tv;
               rgv3D[3].tu = rgv3D[2].tu;
               rgv3D[3].tv = rgv3D[2].tv;
            }
         else
         {
            rgv3D[0].tu = 1.0f;
            rgv3D[0].tv = rgratioInit[i];
            rgv3D[2].tu = 1.0f;
            rgv3D[2].tv = rgratioInit[i+1];

            rgv3D[1].tu = rgv3D[0].tu;
            rgv3D[1].tv = rgv3D[0].tv;
            rgv3D[3].tu = rgv3D[2].tu;
            rgv3D[3].tv = rgv3D[2].tv;
         }
      }

      // 2-Sided polygon
      SetNormal(rgv3D, rgi0123, 4, NULL, NULL, NULL);
      // Draw the wall of the ramp.
      memcpy( &buf[offset], rgv3D, sizeof(Vertex3D_NoTex2)*4 );
      offset+=4;

      SetNormal(rgv3D, rgiRampStatic1, 4, NULL, NULL, NULL);
      // Draw the wall of the ramp.
      memcpy( &buf[offset], rgv3D, sizeof(Vertex3D_NoTex2)*4 );
      offset+=4;
   }

   for (int i=0;i<(rampVertex-1);i++)
   {
      Vertex3D_NoTex2 rgv3D[4];
      rgv3D[0].x = rgvInit[rampVertex*2-i-2].x;
      rgv3D[0].y = rgvInit[rampVertex*2-i-2].y;
      rgv3D[0].z = rgheightInit[i+1]*m_ptable->m_zScale;

      rgv3D[3].x = rgvInit[rampVertex*2-i-1].x;
      rgv3D[3].y = rgvInit[rampVertex*2-i-1].y;
      rgv3D[3].z = rgheightInit[i]*m_ptable->m_zScale;

      rgv3D[2].x = rgvInit[rampVertex*2-i-1].x;
      rgv3D[2].y = rgvInit[rampVertex*2-i-1].y;
      rgv3D[2].z = (rgheightInit[i] + m_d.m_leftwallheightvisible)*m_ptable->m_zScale;

      rgv3D[1].x = rgvInit[rampVertex*2-i-2].x;
      rgv3D[1].y = rgvInit[rampVertex*2-i-2].y;
      rgv3D[1].z = (rgheightInit[i+1] + m_d.m_leftwallheightvisible)*m_ptable->m_zScale;

      if (pin && m_d.m_fImageWalls)
      {
         if (m_d.m_imagealignment == ImageModeWorld)
         {
               rgv3D[0].tu = rgv3D[0].x * inv_width2;
               rgv3D[0].tv = rgv3D[0].y * inv_height2;
               rgv3D[2].tu = rgv3D[2].x * inv_width2;
               rgv3D[2].tv = rgv3D[2].y * inv_height2;

               rgv3D[1].tu = rgv3D[0].tu;
               rgv3D[1].tv = rgv3D[0].tv;
               rgv3D[3].tu = rgv3D[2].tu;
               rgv3D[3].tv = rgv3D[2].tv;
            }
         else
         {
            rgv3D[0].tu = 0;
            rgv3D[0].tv = rgratioInit[i];
            rgv3D[2].tu = 0;
            rgv3D[2].tv = rgratioInit[i+1];

            rgv3D[1].tu = rgv3D[0].tu;
            rgv3D[1].tv = rgv3D[0].tv;
            rgv3D[3].tu = rgv3D[2].tu;
            rgv3D[3].tv = rgv3D[2].tv;
         }
      }

      // 2-Sided polygon
      SetNormal(rgv3D, rgi0123, 4, NULL, NULL, NULL);
      // Draw the wall of the ramp.
      memcpy( &buf[offset], rgv3D, sizeof(Vertex3D_NoTex2)*4 );
      offset+=4;

      SetNormal(rgv3D, rgiRampStatic1, 4, NULL, NULL, NULL);
      // Draw the wall of the ramp.
      memcpy( &buf[offset], rgv3D, sizeof(Vertex3D_NoTex2)*4 );
      offset+=4;
   }
   
   staticVertexBuffer->unlock();  

   delete[] rgvInit;
   delete[] rgheightInit;
   delete[] rgratioInit;

   assert(offset == numVertices);
}

void Ramp::RenderSetup(RenderDevice* pd3dDevice)
{
   solidMaterial.setColor( 1.0f, m_d.m_color );

   if( !staticVertexBuffer && m_d.m_fVisible && !m_d.m_transparent )
   {
      if (isHabitrail())
         prepareHabitrail( pd3dDevice );
      else
         prepareStatic( pd3dDevice );
   }
   else if( !dynamicVertexBuffer && m_d.m_fVisible && m_d.m_transparent )
   {
      if (isHabitrail())
         prepareHabitrail( pd3dDevice );
      else
          GenerateVertexBuffer(pd3dDevice);
      }

}

void Ramp::RenderStatic(RenderDevice* pd3dDevice)
{	
   if (!m_d.m_fVisible) return;		// return if no Visible

   // dont render alpha shaded ramps into static buffer, these are done per frame later-on
   if (m_d.m_transparent) return;

   /* TODO: This is a misnomer right now, but clamp fixes some visual glitches (single-pixel lines)
    * with transparent textures. Probably the option should simply be renamed to ImageModeClamp,
    * since the texture coordinates always stay within [0,1] anyway.
    */
   if (m_d.m_imagealignment == ImageModeWrap)
       pd3dDevice->SetTextureAddressMode(ePictureTexture, RenderDevice::TEX_CLAMP);

   const float r = (float)(m_d.m_color & 255) * (float)(1.0/255.0);
   const float g = (float)(m_d.m_color & 65280) * (float)(1.0/65280.0);
   const float b = (float)(m_d.m_color & 16711680) * (float)(1.0/16711680.0);
   D3DXVECTOR4 matColor(r,g,b,1.0f);   

   pd3dDevice->SetVertexDeclaration( pd3dDevice->m_pVertexNormalTexelDeclaration );
   pd3dDevice->basicShader->Core()->SetVector("vDiffuseColor",&matColor);

   if (isHabitrail())
   {
      RenderStaticHabitrail(pd3dDevice);
   }
   else
   {
      Pin3D * const ppin3d = &g_pplayer->m_pin3d;
      Texture * const pin = m_ptable->GetImage(m_d.m_szImage);
      pd3dDevice->basicShader->Core()->SetFloat("vMaterialPower",0.0f);

      if (pin)
      {
         pin->CreateAlphaChannel();
         //pin->Set( ePictureTexture );

         pd3dDevice->basicShader->SetTexture("Texture0", pin );
         pd3dDevice->basicShader->Core()->SetTechnique("basic_with_texture");
         D3DXVECTOR4 color(1.0f,1.0f,1.0f,1.0f);   
         pd3dDevice->basicShader->Core()->SetVector("vDiffuseColor",&color);

         if (pin->m_fTransparent)
         {
            pd3dDevice->SetRenderState(RenderDevice::CULLMODE, D3DCULL_NONE);
         }
         else
         {
            pd3dDevice->SetRenderState(RenderDevice::CULLMODE, D3DCULL_CCW);
         }
         ppin3d->EnableAlphaBlend( 1, false );

         ppin3d->SetTextureFilter ( ePictureTexture, TEXTURE_MODE_TRILINEAR );

         pd3dDevice->SetMaterial(textureMaterial);
//          if ( !m_d.m_enableLightingImage )
//             pd3dDevice->SetRenderState( RenderDevice::LIGHTING, FALSE );
      }
      else
      {
         pd3dDevice->basicShader->Core()->SetTechnique("basic_without_texture");
      }

      pd3dDevice->basicShader->Begin(0);

      int offset=0;
      for (int i=0; i<(rampVertex-1); i++,offset+=4)
         pd3dDevice->DrawPrimitiveVB(D3DPT_TRIANGLEFAN, staticVertexBuffer, offset, 4);
      pd3dDevice->basicShader->End();  

      if (pin && !m_d.m_fImageWalls)
      {
         ppin3d->SetTexture(NULL);
         pd3dDevice->basicShader->Core()->SetTechnique("basic_without_texture");
//         pd3dDevice->SetMaterial(solidMaterial);
//         if ( !m_d.m_enableLightingImage )
//            pd3dDevice->SetRenderState( RenderDevice::LIGHTING, TRUE );
      }

      pd3dDevice->basicShader->Begin(0);
      for (int i=0;i<(rampVertex-1);i++)
      {
         pd3dDevice->DrawPrimitiveVB(D3DPT_TRIANGLEFAN, staticVertexBuffer, offset, 4);
         offset+=4;
         pd3dDevice->DrawIndexedPrimitiveVB(D3DPT_TRIANGLEFAN, staticVertexBuffer, offset, 4, rgiRampStatic1, 4);
         offset+=4;
      }
      //render second wall
      for (int i=0;i<(rampVertex-1);i++)
      {
         pd3dDevice->DrawPrimitiveVB(D3DPT_TRIANGLEFAN, staticVertexBuffer, offset, 4);
         offset+=4;
         pd3dDevice->DrawIndexedPrimitiveVB(D3DPT_TRIANGLEFAN, staticVertexBuffer, offset, 4, rgiRampStatic1, 4);
         offset+=4;
      }
      pd3dDevice->basicShader->End();  

      ppin3d->SetTexture(NULL);

      pd3dDevice->SetTextureStageState(ePictureTexture, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      ppin3d->DisableAlphaBlend();

//       if ( !m_d.m_enableLightingImage && pin!=NULL )
//          pd3dDevice->SetRenderState( RenderDevice::LIGHTING, TRUE );
      pd3dDevice->SetRenderState(RenderDevice::CULLMODE, D3DCULL_CCW);
   }
}

void Ramp::SetObjectPos()
{
   g_pvp->SetObjectPosCur(0, 0);
}

void Ramp::MoveOffset(const float dx, const float dy)
{
   for (int i=0;i<m_vdpoint.Size();i++)
   {
      CComObject<DragPoint> * const pdp = m_vdpoint.ElementAt(i);

      pdp->m_v.x += dx;
      pdp->m_v.y += dy;
   }

   m_ptable->SetDirtyDraw();
}

void Ramp::ClearForOverwrite()
{
   ClearPointsForOverwrite();
}

HRESULT Ramp::SaveData(IStream *pstm, HCRYPTHASH hcrypthash, HCRYPTKEY hcryptkey)
{
   BiffWriter bw(pstm, hcrypthash, hcryptkey);

   bw.WriteFloat(FID(HTBT), m_d.m_heightbottom);
   bw.WriteFloat(FID(HTTP), m_d.m_heighttop);
   bw.WriteFloat(FID(WDBT), m_d.m_widthbottom);
   bw.WriteFloat(FID(WDTP), m_d.m_widthtop);
   bw.WriteInt(FID(COLR), m_d.m_color);
   bw.WriteBool(FID(TMON), m_d.m_tdr.m_fTimerEnabled);
   bw.WriteInt(FID(TMIN), m_d.m_tdr.m_TimerInterval);
   bw.WriteInt(FID(TYPE), m_d.m_type);
   bw.WriteWideString(FID(NAME), (WCHAR *)m_wzName);
   bw.WriteString(FID(IMAG), m_d.m_szImage);
   bw.WriteInt(FID(ALGN), m_d.m_imagealignment);
   bw.WriteBool(FID(IMGW), m_d.m_fImageWalls);
   bw.WriteBool(FID(CSHD), m_d.m_fCastsShadow);
   bw.WriteFloat(FID(WLHL), m_d.m_leftwallheight);
   bw.WriteFloat(FID(WLHR), m_d.m_rightwallheight);
   bw.WriteFloat(FID(WVHL), m_d.m_leftwallheightvisible);
   bw.WriteFloat(FID(WVHR), m_d.m_rightwallheightvisible);
   bw.WriteFloat(FID(ELAS), m_d.m_elasticity);
   bw.WriteFloat(FID(RFCT), m_d.m_friction);
   bw.WriteFloat(FID(RSCT), m_d.m_scatter);
   bw.WriteBool(FID(CLDRP), m_d.m_fCollidable);
   bw.WriteBool(FID(RVIS), m_d.m_fVisible);
   bw.WriteBool(FID(ERLI), m_d.m_enableLightingImage);
   bw.WriteFloat(FID(RADB), m_d.m_depthBias);
   bw.WriteFloat(FID(RADI), m_d.m_wireDiameter);
   bw.WriteFloat(FID(RADX), m_d.m_wireDistanceX);
   bw.WriteFloat(FID(RADY), m_d.m_wireDistanceY);
   bw.WriteBool(FID(ALPH), m_d.m_transparent);
   bw.WriteInt(FID(OPAC), m_d.m_opacity);

   ISelect::SaveData(pstm, hcrypthash, hcryptkey);

   bw.WriteTag(FID(PNTS));
   HRESULT hr;
   if(FAILED(hr = SavePointData(pstm, hcrypthash, hcryptkey)))
      return hr;

   bw.WriteTag(FID(ENDB));

   return S_OK;
}

HRESULT Ramp::InitLoad(IStream *pstm, PinTable *ptable, int *pid, int version, HCRYPTHASH hcrypthash, HCRYPTKEY hcryptkey)
{
   SetDefaults(false);

   BiffReader br(pstm, this, pid, version, hcrypthash, hcryptkey);

   m_ptable = ptable;

   br.Load();
   return S_OK;
}

BOOL Ramp::LoadToken(int id, BiffReader *pbr)
{
   if (id == FID(PIID))
   {
      pbr->GetInt((int *)pbr->m_pdata);
   }
   else if (id == FID(HTBT))
   {
      pbr->GetFloat(&m_d.m_heightbottom);
   }
   else if (id == FID(HTTP))
   {
      pbr->GetFloat(&m_d.m_heighttop);
   }
   else if (id == FID(WDBT))
   {
      pbr->GetFloat(&m_d.m_widthbottom);
   }
   else if (id == FID(WDTP))
   {
      pbr->GetFloat(&m_d.m_widthtop);
   }
   else if (id == FID(COLR))
   {
      pbr->GetInt(&m_d.m_color);
   }
   else if (id == FID(TMON))
   {
      pbr->GetBool(&m_d.m_tdr.m_fTimerEnabled);
   }
   else if (id == FID(TMIN))
   {
      pbr->GetInt(&m_d.m_tdr.m_TimerInterval);
   }
   else if (id == FID(TYPE))
   {
      pbr->GetInt(&m_d.m_type);
   }
   else if (id == FID(IMAG))
   {
      pbr->GetString(m_d.m_szImage);
   }
   else if (id == FID(ALGN))
   {
      pbr->GetInt(&m_d.m_imagealignment);
   }
   else if (id == FID(IMGW))
   {
      pbr->GetBool(&m_d.m_fImageWalls);
   }
   else if (id == FID(CSHD))
   {
      pbr->GetBool(&m_d.m_fCastsShadow);
   }
   else if (id == FID(NAME))
   {
      pbr->GetWideString((WCHAR *)m_wzName);
   }
   else if (id == FID(WLHL))
   {
      pbr->GetFloat(&m_d.m_leftwallheight);
   }
   else if (id == FID(WLHR))
   {
      pbr->GetFloat(&m_d.m_rightwallheight);
   }
   else if (id == FID(WVHL))
   {
      pbr->GetFloat(&m_d.m_leftwallheightvisible);
   }
   else if (id == FID(WVHR))
   {
      pbr->GetFloat(&m_d.m_rightwallheightvisible);
   }
   else if (id == FID(ELAS))
   {
      pbr->GetFloat(&m_d.m_elasticity);
   }
   else if (id == FID(RFCT))
   {
      pbr->GetFloat(&m_d.m_friction);
   }
   else if (id == FID(RSCT))
   {
      pbr->GetFloat(&m_d.m_scatter);
   }
   else if (id == FID(CLDRP))
   {
      pbr->GetBool(&m_d.m_fCollidable);
   }
   else if (id == FID(RVIS))
   {
      pbr->GetBool(&m_d.m_fVisible);
   }
   else if (id == FID(ERLI))
   {
      pbr->GetBool(&m_d.m_enableLightingImage);
   }
   else if (id == FID(RADB))
   {
      pbr->GetFloat(&m_d.m_depthBias);
   }
   else if (id == FID(RADI))
   {
       pbr->GetFloat(&m_d.m_wireDiameter);
   }
   else if (id == FID(RADX))
   {
       pbr->GetFloat(&m_d.m_wireDistanceX);
   }
   else if (id == FID(RADY))
   {
       pbr->GetFloat(&m_d.m_wireDistanceY);
   }
   else if (id == FID(ALPH))
   {
      pbr->GetInt(&m_d.m_transparent);
   }
   else if (id == FID(OPAC))
   {
      pbr->GetInt(&m_d.m_opacity);
   }
   else
   {
      LoadPointToken(id, pbr, pbr->m_version);
      ISelect::LoadToken(id, pbr);
   }
   return fTrue;
}

HRESULT Ramp::InitPostLoad()
{
   return S_OK;
}

void Ramp::DoCommand(int icmd, int x, int y)
{
   ISelect::DoCommand(icmd, x, y);

   switch (icmd)
   {
   case ID_WALLMENU_FLIP:
      {
         Vertex2D vCenter;
         GetPointCenter(&vCenter);
         FlipPointY(&vCenter);
      }
      break;

   case ID_WALLMENU_MIRROR:
      {
         Vertex2D vCenter;
         GetPointCenter(&vCenter);
         FlipPointX(&vCenter);
      }
      break;

   case ID_WALLMENU_ROTATE:
      RotateDialog();
      break;

   case ID_WALLMENU_SCALE:
      ScaleDialog();
      break;

   case ID_WALLMENU_TRANSLATE:
      TranslateDialog();
      break;

   case ID_WALLMENU_ADDPOINT:
      {
         STARTUNDO

         RECT rc;
         GetClientRect(m_ptable->m_hwnd, &rc);

         HitSur * const phs = new HitSur(NULL, m_ptable->m_zoom, m_ptable->m_offsetx, m_ptable->m_offsety, rc.right - rc.left, rc.bottom - rc.top, 0, 0, NULL);

         const Vertex2D v = phs->ScreenToSurface(x, y);
         delete phs;

         Vector<RenderVertex> vvertex;
         GetCentralCurve(&vvertex);

         const int cvertex = vvertex.size();
         Vertex2D vOut;
         int iSeg;
         ClosestPointOnPolygon(vvertex, v, &vOut, &iSeg, false);

         // Go through vertices (including iSeg itself) counting control points until iSeg
         int icp = 0;
         for (int i=0;i<(iSeg+1);i++)
            if (vvertex.ElementAt(i)->fControlPoint)
               icp++;

         for (int i=0;i<cvertex;i++)
            delete vvertex.ElementAt(i);

         //if (icp == 0) // need to add point after the last point
         //icp = m_vdpoint.Size();

         CComObject<DragPoint> *pdp;
         CComObject<DragPoint>::CreateInstance(&pdp);
         if (pdp)
         {
            pdp->AddRef();
            pdp->Init(this, vOut.x, vOut.y);
            pdp->m_fSmooth = fTrue; // Ramps are usually always smooth
            m_vdpoint.InsertElementAt(pdp, icp); // push the second point forward, and replace it with this one.  Should work when index2 wraps.
         }

         SetDirtyDraw();

         STOPUNDO
      }
      break;
   }
}

void Ramp::FlipY(Vertex2D * const pvCenter)
{
   IHaveDragPoints::FlipPointY(pvCenter);
}

void Ramp::FlipX(Vertex2D * const pvCenter)
{
   IHaveDragPoints::FlipPointX(pvCenter);
}

void Ramp::Rotate(float ang, Vertex2D *pvCenter)
{
   IHaveDragPoints::RotatePoints(ang, pvCenter);
}

void Ramp::Scale(float scalex, float scaley, Vertex2D *pvCenter)
{
   IHaveDragPoints::ScalePoints(scalex, scaley, pvCenter);
}

void Ramp::Translate(Vertex2D *pvOffset)
{
   IHaveDragPoints::TranslatePoints(pvOffset);
}

STDMETHODIMP Ramp::InterfaceSupportsErrorInfo(REFIID riid)
{
   static const IID* arr[] =
   {
      &IID_IRamp
   };

   for (size_t i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
      if (InlineIsEqualGUID(*arr[i],riid))
         return S_OK;

   return S_FALSE;
}

STDMETHODIMP Ramp::get_HeightBottom(float *pVal)
{
   *pVal = m_d.m_heightbottom;

   return S_OK;
}

STDMETHODIMP Ramp::put_HeightBottom(float newVal)
{
   if(m_d.m_heightbottom != newVal)
   {
	   STARTUNDO

	   m_d.m_heightbottom = newVal;
	   dynamicVertexBufferRegenerate = true;

	   STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Ramp::get_HeightTop(float *pVal)
{
   *pVal = m_d.m_heighttop;

   return S_OK;
}

STDMETHODIMP Ramp::put_HeightTop(float newVal)
{
   if(m_d.m_heighttop != newVal)
   {
	   STARTUNDO

	   m_d.m_heighttop = newVal;
	   dynamicVertexBufferRegenerate = true;

	   STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Ramp::get_WidthBottom(float *pVal)
{
   *pVal = m_d.m_widthbottom;

   return S_OK;
}

STDMETHODIMP Ramp::put_WidthBottom(float newVal)
{
   if(m_d.m_widthbottom != newVal)
   {
	   STARTUNDO

	   m_d.m_widthbottom = newVal;
	   dynamicVertexBufferRegenerate = true;

	   STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Ramp::get_WidthTop(float *pVal)
{
   *pVal = m_d.m_widthtop;

   return S_OK;
}

STDMETHODIMP Ramp::put_WidthTop(float newVal)
{
   if(m_d.m_widthtop != newVal)
   {
	   STARTUNDO

	   m_d.m_widthtop = newVal;
	   dynamicVertexBufferRegenerate = true;

	   STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Ramp::get_Color(OLE_COLOR *pVal)
{
   *pVal = m_d.m_color;

   return S_OK;
}

STDMETHODIMP Ramp::put_Color(OLE_COLOR newVal)
{
   if(m_d.m_color != newVal)
   {
	   STARTUNDO

	   m_d.m_color = newVal;

	   STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Ramp::get_Type(RampType *pVal)
{
   *pVal = m_d.m_type;

   return S_OK;
}

STDMETHODIMP Ramp::put_Type(RampType newVal)
{
   STARTUNDO

   m_d.m_type = newVal;
   dynamicVertexBufferRegenerate = true;

   STOPUNDO

   return S_OK;
}

void Ramp::GetDialogPanes(Vector<PropertyPane> *pvproppane)
{
   PropertyPane *pproppane;

   pproppane = new PropertyPane(IDD_PROP_NAME, NULL);
   pvproppane->AddElement(pproppane);

   pproppane = new PropertyPane(IDD_PROPRAMP_VISUALS, IDS_VISUALS);
   pvproppane->AddElement(pproppane);

   pproppane = new PropertyPane(IDD_PROPRAMP_POSITION, IDS_POSITION);
   pvproppane->AddElement(pproppane);

   pproppane = new PropertyPane(IDD_PROPRAMP_PHYSICS, IDS_PHYSICS);
   pvproppane->AddElement(pproppane);

   pproppane = new PropertyPane(IDD_PROP_TIMER, IDS_MISC);
   pvproppane->AddElement(pproppane);
}


STDMETHODIMP Ramp::get_Image(BSTR *pVal)
{
   WCHAR wz[512];

   MultiByteToWideChar(CP_ACP, 0, m_d.m_szImage, -1, wz, 32);
   *pVal = SysAllocString(wz);

   return S_OK;
}

STDMETHODIMP Ramp::put_Image(BSTR newVal)
{
   char m_szImage[MAXTOKEN];
   WideCharToMultiByte(CP_ACP, 0, newVal, -1, m_szImage, 32, NULL, NULL);

   if(strcmp(m_szImage,m_d.m_szImage) != 0)
   {
	   STARTUNDO

	   strcpy_s(m_d.m_szImage, MAXTOKEN, m_szImage);
	   dynamicVertexBufferRegenerate = true;

	   STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Ramp::get_ImageAlignment(RampImageAlignment *pVal)
{
   *pVal = m_d.m_imagealignment;

   return S_OK;
}

STDMETHODIMP Ramp::put_ImageAlignment(RampImageAlignment newVal)
{
   if(m_d.m_imagealignment != newVal)
   {
	   STARTUNDO

	   m_d.m_imagealignment = newVal;
	   dynamicVertexBufferRegenerate = true;

	   STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Ramp::get_HasWallImage(VARIANT_BOOL *pVal)
{
   *pVal = (VARIANT_BOOL)FTOVB(m_d.m_fImageWalls);

   return S_OK;
}

STDMETHODIMP Ramp::put_HasWallImage(VARIANT_BOOL newVal)
{
   if(m_d.m_fImageWalls != VBTOF(newVal))
   {
	   STARTUNDO

	   m_d.m_fImageWalls = VBTOF(newVal);
	   dynamicVertexBufferRegenerate = true;

	   STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Ramp::get_CastsShadow(VARIANT_BOOL *pVal)
{
   *pVal = (VARIANT_BOOL)FTOVB(m_d.m_fCastsShadow);

   return S_OK;
}

STDMETHODIMP Ramp::put_CastsShadow(VARIANT_BOOL newVal)
{
   STARTUNDO
   
   m_d.m_fCastsShadow = VBTOF(newVal);
   
   STOPUNDO

   return S_OK;
}

STDMETHODIMP Ramp::get_Transparent(VARIANT_BOOL *pVal)
{
   *pVal = (VARIANT_BOOL)FTOVB(m_d.m_transparent);
   return S_OK;
}

STDMETHODIMP Ramp::put_Transparent(VARIANT_BOOL newVal)
{
   STARTUNDO
   m_d.m_transparent = !!VBTOF(newVal);
   STOPUNDO

   return S_OK;
}

STDMETHODIMP Ramp::get_Opacity(int *pVal)
{
   *pVal = m_d.m_opacity;
   return S_OK;
}

STDMETHODIMP Ramp::put_Opacity(int newVal)
{
   STARTUNDO
   m_d.m_opacity = clamp(newVal, 0, 255);
   STOPUNDO

   return S_OK;
}

STDMETHODIMP Ramp::get_Solid(VARIANT_BOOL *pVal)
{
   *pVal = !FTOVB(!m_d.m_transparent);
   
   return S_OK;
}

STDMETHODIMP Ramp::put_Solid(VARIANT_BOOL newVal)
{
   STARTUNDO
  
   if (VBTOF(newVal)) {
      m_d.m_transparent = false;
   }
   
   STOPUNDO

   return S_OK;
}

STDMETHODIMP Ramp::get_LeftWallHeight(float *pVal)
{
   *pVal = m_d.m_leftwallheight;

   return S_OK;
}

STDMETHODIMP Ramp::put_LeftWallHeight(float newVal)
{
   const float nv = newVal < 0.f ? 0.f : newVal;

   if(m_d.m_leftwallheight != nv)
   {
	   STARTUNDO

	   m_d.m_leftwallheight = nv;
	   dynamicVertexBufferRegenerate = true;

	   STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Ramp::get_RightWallHeight(float *pVal)
{
   *pVal = m_d.m_rightwallheight;

   return S_OK;
}

STDMETHODIMP Ramp::put_RightWallHeight(float newVal)
{
   const float nv = newVal < 0.f ? 0.f : newVal;

   if(m_d.m_rightwallheight != nv)
   {
	   STARTUNDO

	   m_d.m_rightwallheight = nv;
	   dynamicVertexBufferRegenerate = true;

	   STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Ramp::get_VisibleLeftWallHeight(float *pVal)
{
   *pVal = m_d.m_leftwallheightvisible;

   return S_OK;
}

STDMETHODIMP Ramp::put_VisibleLeftWallHeight(float newVal)
{
   const float nv = newVal < 0.f ? 0.f : newVal;

   if(m_d.m_leftwallheightvisible != nv)
   {
	   STARTUNDO

	   m_d.m_leftwallheightvisible = nv;
	   dynamicVertexBufferRegenerate = true;

	   STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Ramp::get_VisibleRightWallHeight(float *pVal)
{
   *pVal = m_d.m_rightwallheightvisible;

   return S_OK;
}

STDMETHODIMP Ramp::put_VisibleRightWallHeight(float newVal)
{
   const float nv = newVal < 0.f ? 0.f : newVal;

   if(m_d.m_rightwallheightvisible != nv)
   {
	   STARTUNDO

	   m_d.m_rightwallheightvisible = nv;
	   dynamicVertexBufferRegenerate = true;

	   STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Ramp::get_Elasticity(float *pVal)
{
   *pVal = m_d.m_elasticity;

   return S_OK;
}

STDMETHODIMP Ramp::put_Elasticity(float newVal)
{
   STARTUNDO

   m_d.m_elasticity = newVal;

   STOPUNDO

   return S_OK;
}

STDMETHODIMP Ramp::get_Friction(float *pVal)
{
   *pVal = m_d.m_friction;

   return S_OK;
}

STDMETHODIMP Ramp::put_Friction(float newVal)
{
   STARTUNDO

   if (newVal > 1.0f) newVal = 1.0f;
      else if (newVal < 0.f) newVal = 0.f;
 
   m_d.m_friction = newVal;

   STOPUNDO

   return S_OK;
}

STDMETHODIMP Ramp::get_Scatter(float *pVal)
{
   *pVal = m_d.m_scatter;

   return S_OK;
}

STDMETHODIMP Ramp::put_Scatter(float newVal)
{
   STARTUNDO

   m_d.m_scatter = newVal;

   STOPUNDO

   return S_OK;
}

STDMETHODIMP Ramp::get_Collidable(VARIANT_BOOL *pVal)
{
   *pVal = (VARIANT_BOOL)FTOVB((!g_pplayer) ? m_d.m_fCollidable : m_vhoCollidable[0]->m_fEnabled);

   return S_OK;
}

STDMETHODIMP Ramp::put_Collidable(VARIANT_BOOL newVal)
{
   BOOL fNewVal = VBTOF(newVal);	
   if (!g_pplayer)
   {	
      STARTUNDO

      m_d.m_fCollidable = !!fNewVal;		

      STOPUNDO
   }
   else
	   for (unsigned i=0; i < m_vhoCollidable.size(); i++)
	      m_vhoCollidable[i]->m_fEnabled = fNewVal;	//copy to hit checking on enities composing the object

   return S_OK;
}

STDMETHODIMP Ramp::get_Visible(VARIANT_BOOL *pVal) //temporary value of object
{
   *pVal = (VARIANT_BOOL)FTOVB(m_d.m_fVisible);

   return S_OK;
}

STDMETHODIMP Ramp::put_Visible(VARIANT_BOOL newVal)
{
      STARTUNDO
    m_d.m_fVisible = VBTOF(newVal);
	  STOPUNDO

   return S_OK;
}

STDMETHODIMP Ramp::get_EnableLightingImage(VARIANT_BOOL *pVal)
{
   *pVal = (VARIANT_BOOL)FTOVB(m_d.m_enableLightingImage);

   return S_OK;
}

STDMETHODIMP Ramp::put_EnableLightingImage(VARIANT_BOOL newVal)
{
   STARTUNDO

   m_d.m_enableLightingImage= VBTOF(newVal);

   STOPUNDO

   return S_OK;
}

STDMETHODIMP Ramp::get_DepthBias(float *pVal)
{
   *pVal = m_d.m_depthBias;

   return S_OK;
}

STDMETHODIMP Ramp::put_DepthBias(float newVal)
{
   if(m_d.m_depthBias != newVal)
   {
      STARTUNDO

      m_d.m_depthBias = newVal;
      dynamicVertexBufferRegenerate = true;

      STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Ramp::get_WireDiameter(float *pVal)
{
    *pVal = m_d.m_wireDiameter;

    return S_OK;
}

STDMETHODIMP Ramp::put_WireDiameter(float newVal)
{
    if(m_d.m_wireDiameter != newVal)
    {
        STARTUNDO

        m_d.m_wireDiameter = newVal;

        STOPUNDO
    }

    return S_OK;
}

STDMETHODIMP Ramp::get_WireDistanceX(float *pVal)
{
    *pVal = m_d.m_wireDistanceX;

    return S_OK;
}

STDMETHODIMP Ramp::put_WireDistanceX(float newVal)
{
    if(m_d.m_wireDistanceX != newVal)
    {
        STARTUNDO

        m_d.m_wireDistanceX = newVal;

        STOPUNDO
    }

    return S_OK;
}

STDMETHODIMP Ramp::get_WireDistanceY(float *pVal)
{
    *pVal = m_d.m_wireDistanceY;

    return S_OK;
}

STDMETHODIMP Ramp::put_WireDistanceY(float newVal)
{
    if(m_d.m_wireDistanceY != newVal)
    {
        STARTUNDO

        m_d.m_wireDistanceY = newVal;

        STOPUNDO
    }

    return S_OK;
}

// Always called each frame to render over everything else (along with primitives)
// Same code as RenderStatic (with the exception of the alpha tests).
// Also has less drawing calls by bundling seperate calls.
void Ramp::PostRenderStatic(RenderDevice* pd3dDevice)
{
    TRACE_FUNCTION();

   // don't render if invisible or not a transparent ramp
   if (!m_d.m_fVisible || !m_d.m_transparent)
       return;

   if ( m_d.m_widthbottom==0.0f && m_d.m_widthtop==0.0f )
   {
      dynamicVertexBufferRegenerate=false;
      return;
   }

   pd3dDevice->SetVertexDeclaration( pd3dDevice->m_pVertexNormalTexelDeclaration );

   // see the comment in RenderStatic() above
   if (m_d.m_imagealignment == ImageModeWrap)
       pd3dDevice->SetTextureAddressMode(ePictureTexture, RenderDevice::TEX_CLAMP);

   solidMaterial.setColor(1.0f, m_d.m_color );
   const float r = (float)(m_d.m_color & 255) * (float)(1.0/255.0);
   const float g = (float)(m_d.m_color & 65280) * (float)(1.0/65280.0);
   const float b = (float)(m_d.m_color & 16711680) * (float)(1.0/16711680.0);
   D3DXVECTOR4 matColor(r,g,b,1.0f);   
   pd3dDevice->basicShader->Core()->SetVector("vDiffuseColor",&matColor);

   if (isHabitrail())
   {
      RenderStaticHabitrail(pd3dDevice);
   }
   else
   {
      pd3dDevice->basicShader->Core()->SetVector("vDiffuseColor",&matColor);
      pd3dDevice->basicShader->Core()->SetFloat("vMaterialPower",0.0f);

      Pin3D * const ppin3d = &g_pplayer->m_pin3d;
      Texture * const pin = m_ptable->GetImage(m_d.m_szImage);

      if (pin)
      {
         pin->CreateAlphaChannel();
         //pin->Set( ePictureTexture );

         pd3dDevice->basicShader->SetTexture( "Texture0", pin );
         pd3dDevice->basicShader->Core()->SetTechnique("basic_with_texture");
         D3DXVECTOR4 color(1.0f,1.0f,1.0f,1.0f);   
         pd3dDevice->basicShader->Core()->SetVector("vDiffuseColor",&color);

         ppin3d->SetTextureFilter ( ePictureTexture, TEXTURE_MODE_TRILINEAR );

//          if ( !m_d.m_enableLightingImage )
//             pd3dDevice->SetRenderState( RenderDevice::LIGHTING, FALSE );
      }
      else
      {
         ppin3d->SetTexture(NULL);
         pd3dDevice->basicShader->Core()->SetTechnique("basic_without_texture");
      }

      if (!dynamicVertexBuffer || dynamicVertexBufferRegenerate)
         GenerateVertexBuffer(pd3dDevice);

      ppin3d->EnableAlphaBlend( 1, false );

      pd3dDevice->SetTextureStageState(ePictureTexture, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
      pd3dDevice->SetTextureStageState(ePictureTexture, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
      pd3dDevice->SetRenderState(RenderDevice::TEXTUREFACTOR, (m_d.m_opacity << 24) | 0xffffff);

      unsigned int offset=0;
      pd3dDevice->basicShader->Begin(0);
      pd3dDevice->DrawIndexedPrimitiveVB(D3DPT_TRIANGLELIST, dynamicVertexBuffer, offset, m_numVertices, dynamicIndexBuffer, 0, (rampVertex-1)*6);
      pd3dDevice->basicShader->End();  

      offset += m_numVertices;

      if (pin && !m_d.m_fImageWalls)
      {
         ppin3d->SetTexture(NULL);
         pd3dDevice->basicShader->Core()->SetTechnique("basic_without_texture");
//          if ( !m_d.m_enableLightingImage )
//             pd3dDevice->SetRenderState( RenderDevice::LIGHTING, TRUE );
      }

      pd3dDevice->basicShader->Begin(0);

      if(m_d.m_rightwallheightvisible!=0.f && m_d.m_leftwallheightvisible!=0.f) //only render left & right side if the height is >0
      {
         pd3dDevice->DrawIndexedPrimitiveVB(D3DPT_TRIANGLELIST, dynamicVertexBuffer, offset, m_numVertices*2*2, dynamicIndexBuffer, 0, (rampVertex-1)*6*2*2);
      }
	  else
	  {
		if ( m_d.m_rightwallheightvisible!=0.f ) //only render right side if the height is >0
        {
			pd3dDevice->DrawIndexedPrimitiveVB(D3DPT_TRIANGLELIST, dynamicVertexBuffer, offset, m_numVertices*2, dynamicIndexBuffer, 0, (rampVertex-1)*6*2);
        }
		offset+=2*m_numVertices;

		if ( m_d.m_leftwallheightvisible!=0.f ) //only render left side if the height is >0
        {
			pd3dDevice->DrawIndexedPrimitiveVB(D3DPT_TRIANGLELIST, dynamicVertexBuffer, offset, m_numVertices*2, dynamicIndexBuffer, 0, (rampVertex-1)*6*2);
        }
	  }
      pd3dDevice->basicShader->End();  

      ppin3d->DisableAlphaBlend();

//       pd3dDevice->SetTextureStageState(ePictureTexture, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
//       pd3dDevice->SetTextureStageState(ePictureTexture, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
//       pd3dDevice->SetRenderState(RenderDevice::TEXTUREFACTOR, 0xffffffff);

//       if ( !m_d.m_enableLightingImage && pin!=NULL )
//          pd3dDevice->SetRenderState( RenderDevice::LIGHTING, TRUE );
   }
}


void Ramp::GenerateVertexBuffer(RenderDevice* pd3dDevice)
{
    dynamicVertexBufferRegenerate = false;

    Texture * const pin = m_ptable->GetImage(m_d.m_szImage);
    float *rgheight, *rgratio;
    const Vertex2D * const rgvLocal = GetRampVertex(rampVertex, &rgheight, NULL, &rgratio, NULL);

    const float inv_tablewidth = 1.0f/(m_ptable->m_right - m_ptable->m_left);
    const float inv_tableheight = 1.0f/(m_ptable->m_bottom - m_ptable->m_top);

    m_numVertices=(rampVertex-1)*4;
    unsigned int offset=0;
    const unsigned int rgioffset = (rampVertex-1)*6;

    if (dynamicVertexBuffer)
        dynamicVertexBuffer->release();
    pd3dDevice->CreateVertexBuffer(m_numVertices*5, 0, MY_D3DFVF_NOTEX2_VERTEX, &dynamicVertexBuffer);

    Vertex3D_NoTex2 *buf;
    dynamicVertexBuffer->lock(0,0,(void**)&buf, VertexBuffer::WRITEONLY);

    Vertex3D_NoTex2* rgvbuf = new Vertex3D_NoTex2[m_numVertices];
    std::vector<WORD> rgibuf( (rampVertex-1)*6*2*2 );

    for (int i=0;i<(rampVertex-1);i++)
    {
        Vertex3D_NoTex2 * const rgv3D = &rgvbuf[0]+i*4;
        rgv3D[0].x = rgvLocal[i].x;
        rgv3D[0].y = rgvLocal[i].y;
        rgv3D[0].z = rgheight[i]*m_ptable->m_zScale;

        rgv3D[3].x = rgvLocal[i+1].x;
        rgv3D[3].y = rgvLocal[i+1].y;
        rgv3D[3].z = rgheight[i+1]*m_ptable->m_zScale;

        rgv3D[2].x = rgvLocal[rampVertex*2-i-2].x;
        rgv3D[2].y = rgvLocal[rampVertex*2-i-2].y;
        rgv3D[2].z = rgheight[i+1]*m_ptable->m_zScale;

        rgv3D[1].x = rgvLocal[rampVertex*2-i-1].x;
        rgv3D[1].y = rgvLocal[rampVertex*2-i-1].y;
        rgv3D[1].z = rgheight[i]*m_ptable->m_zScale;

        if (pin)
        {
            if (m_d.m_imagealignment == ImageModeWorld)
            {
                rgv3D[0].tu = rgv3D[0].x * inv_tablewidth;
                rgv3D[0].tv = rgv3D[0].y * inv_tableheight;
                rgv3D[1].tu = rgv3D[1].x * inv_tablewidth;
                rgv3D[1].tv = rgv3D[1].y * inv_tableheight;
                rgv3D[2].tu = rgv3D[2].x * inv_tablewidth;
                rgv3D[2].tv = rgv3D[2].y * inv_tableheight;
                rgv3D[3].tu = rgv3D[3].x * inv_tablewidth;
                rgv3D[3].tv = rgv3D[3].y * inv_tableheight;
            }
            else
            {
                rgv3D[0].tu = 1.0f;
                rgv3D[0].tv = rgratio[i];
                rgv3D[1].tu = 0;
                rgv3D[1].tv = rgratio[i];
                rgv3D[2].tu = 0;
                rgv3D[2].tv = rgratio[i+1];
                rgv3D[3].tu = 1.0f;
                rgv3D[3].tv = rgratio[i+1];
            }
        }

        SetNormal(rgv3D, rgi0123, 4, NULL, NULL, NULL);
        // Draw the floor of the ramp.
        rgibuf[i*6]   = i*4;
        rgibuf[i*6+1] = i*4+1;
        rgibuf[i*6+2] = i*4+2;
        rgibuf[i*6+3] = i*4;
        rgibuf[i*6+4] = i*4+2;
        rgibuf[i*6+5] = i*4+3;

        rgibuf[i*6+rgioffset]   = i*4+m_numVertices;
        rgibuf[i*6+rgioffset+1] = i*4+m_numVertices+3;
        rgibuf[i*6+rgioffset+2] = i*4+m_numVertices+2;
        rgibuf[i*6+rgioffset+3] = i*4+m_numVertices;
        rgibuf[i*6+rgioffset+4] = i*4+m_numVertices+2;
        rgibuf[i*6+rgioffset+5] = i*4+m_numVertices+1;

        rgibuf[i*6+rgioffset*2]   = i*4+m_numVertices*2;
        rgibuf[i*6+rgioffset*2+1] = i*4+m_numVertices*2+1;
        rgibuf[i*6+rgioffset*2+2] = i*4+m_numVertices*2+2;
        rgibuf[i*6+rgioffset*2+3] = i*4+m_numVertices*2;
        rgibuf[i*6+rgioffset*2+4] = i*4+m_numVertices*2+2;
        rgibuf[i*6+rgioffset*2+5] = i*4+m_numVertices*2+3;

        rgibuf[i*6+rgioffset*3]   = i*4+m_numVertices*3;
        rgibuf[i*6+rgioffset*3+1] = i*4+m_numVertices*3+3;
        rgibuf[i*6+rgioffset*3+2] = i*4+m_numVertices*3+2;
        rgibuf[i*6+rgioffset*3+3] = i*4+m_numVertices*3;
        rgibuf[i*6+rgioffset*3+4] = i*4+m_numVertices*3+2;
        rgibuf[i*6+rgioffset*3+5] = i*4+m_numVertices*3+1;
    }
    memcpy( &buf[offset], &rgvbuf[0], sizeof(Vertex3D_NoTex2)*m_numVertices );
    offset+=m_numVertices;

    if (dynamicIndexBuffer)
        dynamicIndexBuffer->release();
    dynamicIndexBuffer = pd3dDevice->CreateAndFillIndexBuffer( rgibuf );

    WORD maxidx = 0;
    for (unsigned i = 0; i < rgibuf.size(); ++i)
        maxidx = std::max(maxidx, rgibuf[i]);

    for (int i=0; i<(rampVertex-1); i++)
    {
        Vertex3D_NoTex2 * const rgv3D = &rgvbuf[0]+i*4;
        rgv3D[2].x = rgvLocal[i+1].x;
        rgv3D[2].y = rgvLocal[i+1].y;
        rgv3D[2].z = (rgheight[i+1] + m_d.m_rightwallheightvisible)*m_ptable->m_zScale;

        rgv3D[1].x = rgvLocal[i].x;
        rgv3D[1].y = rgvLocal[i].y;
        rgv3D[1].z = (rgheight[i] + m_d.m_rightwallheightvisible)*m_ptable->m_zScale;

        if (pin && m_d.m_fImageWalls)
        {
            if (m_d.m_imagealignment == ImageModeWorld)
            {
                rgv3D[0].tu = rgv3D[0].x * inv_tablewidth;
                rgv3D[0].tv = rgv3D[0].y * inv_tableheight;
                rgv3D[2].tu = rgv3D[2].x * inv_tablewidth;
                rgv3D[2].tv = rgv3D[2].y * inv_tableheight;
            }
            else
            {
                rgv3D[0].tu = 0;
                rgv3D[0].tv = rgratio[i];
                rgv3D[2].tu = 0;
                rgv3D[2].tv = rgratio[i+1];
            }

            rgv3D[1].tu = rgv3D[0].tu;
            rgv3D[1].tv = rgv3D[0].tv;
            rgv3D[3].tu = rgv3D[2].tu;
            rgv3D[3].tv = rgv3D[2].tv;
        }

        // 2-Sided polygon
        SetNormal(rgv3D, rgi0123, 4);
    }
    memcpy( &buf[offset], &rgvbuf[0], sizeof(Vertex3D_NoTex2)*m_numVertices );
    offset+=m_numVertices;

    // Flip Normals and redraw
    for (int i=0;i<(rampVertex-1);i++)
        for(int j = 0; j < 4; ++j) {
            rgvbuf[i*4+j].nx = -rgvbuf[i*4+j].nx;
            rgvbuf[i*4+j].ny = -rgvbuf[i*4+j].ny;
            rgvbuf[i*4+j].nz = -rgvbuf[i*4+j].nz;
        }

    memcpy( &buf[offset], &rgvbuf[0], sizeof(Vertex3D_NoTex2)*m_numVertices );
    offset+=m_numVertices;

    // only calculate vertices if one or both sides are visible (!=0)
    if( m_d.m_leftwallheightvisible!=0.f || m_d.m_rightwallheightvisible!=0.f )
    {
        for (int i=0;i<(rampVertex-1);i++)
        {
            Vertex3D_NoTex2 * const rgv3D = &rgvbuf[0]+i*4;
            rgv3D[0].x = rgvLocal[rampVertex*2-i-2].x;
            rgv3D[0].y = rgvLocal[rampVertex*2-i-2].y;
            rgv3D[0].z = rgheight[i+1]*m_ptable->m_zScale;

            rgv3D[3].x = rgvLocal[rampVertex*2-i-1].x;
            rgv3D[3].y = rgvLocal[rampVertex*2-i-1].y;
            rgv3D[3].z = rgheight[i]*m_ptable->m_zScale;

            rgv3D[2].x = rgv3D[3].x;
            rgv3D[2].y = rgv3D[3].y;
            rgv3D[2].z = (rgheight[i] + m_d.m_leftwallheightvisible)*m_ptable->m_zScale;

            rgv3D[1].x = rgv3D[0].x;
            rgv3D[1].y = rgv3D[0].y;
            rgv3D[1].z = (rgheight[i+1] + m_d.m_leftwallheightvisible)*m_ptable->m_zScale;

            if (pin && m_d.m_fImageWalls)
            {
                if (m_d.m_imagealignment == ImageModeWorld)
                {
                    rgv3D[0].tu = rgv3D[0].x * inv_tablewidth;
                    rgv3D[0].tv = rgv3D[0].y * inv_tableheight;
                    rgv3D[2].tu = rgv3D[2].x * inv_tablewidth;
                    rgv3D[2].tv = rgv3D[2].y * inv_tableheight;
                }
                else
                {
                    rgv3D[0].tu = 0;
                    rgv3D[0].tv = rgratio[i];
                    rgv3D[2].tu = 0;
                    rgv3D[2].tv = rgratio[i+1];
                }

                rgv3D[1].tu = rgv3D[0].tu;
                rgv3D[1].tv = rgv3D[0].tv;
                rgv3D[3].tu = rgv3D[2].tu;
                rgv3D[3].tv = rgv3D[2].tv;
            }

            // 2-Sided polygon
            SetNormal(rgv3D, rgi0123, 4, NULL, NULL, NULL);
        }
        memcpy( &buf[offset], &rgvbuf[0], sizeof(Vertex3D_NoTex2)*m_numVertices );
        offset+=m_numVertices;

        // Flip Normals and redraw
        for (int i=0;i<(rampVertex-1);i++)
        {
            for(int j = 0; j < 4; ++j) {
                rgvbuf[i*4+j].nx = -rgvbuf[i*4+j].nx;
                rgvbuf[i*4+j].ny = -rgvbuf[i*4+j].ny;
                rgvbuf[i*4+j].nz = -rgvbuf[i*4+j].nz;
            }
        }
        memcpy( &buf[offset], &rgvbuf[0], sizeof(Vertex3D_NoTex2)*m_numVertices );
    }

    dynamicVertexBuffer->unlock();

    delete [] rgvbuf;
    delete [] rgvLocal;
    delete [] rgheight;
    delete [] rgratio;
}
