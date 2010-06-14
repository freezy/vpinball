#include "stdafx.h"


BumperHitCircle::BumperHitCircle()
	{
	m_bumperanim.m_iframe = -1;
	m_bumperanim.m_TimeReset = 0;
	m_bumperanim.m_iframedesired = 0;
	m_bumperanim.m_fAutoTurnedOff = fFalse;
	m_elasticity = 0.3f;
	m_antifriction = 1.0f;
	m_scatter = 0;
	m_bumperanim.m_fVisible = fTrue;
	for (int i=0;i<2;i++)
		{
		m_bumperanim.m_pobjframe[i]= NULL;
		}
	}

BumperHitCircle::~BumperHitCircle()
	{
	for (int i=0;i<2;i++)
		{
		if (m_bumperanim.m_pobjframe[i])
			delete m_bumperanim.m_pobjframe[i];
		}
	}

void BumperHitCircle::Collide(Ball *pball, Vertex3Ds *phitnormal)
	{
	const float dot = phitnormal->x * pball->vx + phitnormal->y * pball->vy;

	pball->CollideWall(phitnormal, m_elasticity, m_antifriction, m_scatter);	//reflect ball from wall

	// if the bumper is disabled then don't activate the bumper
	if (m_pbumper->m_fDisabled)
		return;

	if (dot <= -m_pbumper->m_d.m_threshold)// if velocity greater than threshold level
		{
		pball->vx += phitnormal[0].x * m_pbumper->m_d.m_force;	// add a chunk of velocity to drive ball away 
		pball->vy += phitnormal[0].y * m_pbumper->m_d.m_force;

		if (m_pbumper->m_d.m_fFlashWhenHit)
			{
			m_bumperanim.m_TimeReset = g_pplayer->m_timeCur + 100;
			m_bumperanim.m_fAutoTurnedOff = fTrue;
			}

		m_pbumper->FireGroupEvent(DISPID_HitEvents_Hit);
		}
	}

void BumperAnimObject::Check3D()
	{
	if(!m_fVisible) return;
	
	if ((m_iframe == 0) && (m_TimeReset != 0))
		{
		m_iframe = 1;
		m_fInvalid = fTrue;
		}
	else if (m_fAutoTurnedOff && (m_iframe == 1) && (m_TimeReset < g_pplayer->m_timeCur) && (m_iframe != m_iframedesired))
		{
		m_iframe = m_iframedesired;
		m_fInvalid = fTrue;
		m_TimeReset = 0;
		m_fAutoTurnedOff = fFalse;
		}
	else if (m_iframe == -1)
		{
		m_iframe = 0;
		m_fInvalid = fTrue;
		}
	else if (!m_fAutoTurnedOff && (m_iframe != m_iframedesired))
		{
		m_iframe = m_iframedesired;
		m_fInvalid = fTrue;
		}
	}

ObjFrame *BumperAnimObject::Draw3D(RECT *prc)
	{
	if(!m_fVisible || m_iframe == -1) return NULL;
	
	ObjFrame * const pobjframe = m_pobjframe[m_iframe];

	return pobjframe;
	}

void BumperAnimObject::Reset()
	{
	m_iframe = -2;
	}

LineSegSlingshot::LineSegSlingshot()
	{
	m_slingshotanim.m_iframe = 0;
	m_slingshotanim.m_TimeReset = 0; // zero means the slingshot is currently reset
	m_elasticity = 0.3f;
	m_antifriction = 1.0f;
	m_scatter = 0;
	}

LineSegSlingshot::~LineSegSlingshot()
	{
	delete m_slingshotanim.m_pobjframe;
	}


PINFLOAT LineSegSlingshot::HitTest(Ball *pball, PINFLOAT dtime, Vertex3Ds *phitnormal)//rlc begin change >>>>>>>>>>>>>>>>
	{
	if (!m_fEnabled) return -1.0f;	

	return this->HitTestBasic(pball, dtime, phitnormal, true, true, true);	
	}


void LineSegSlingshot::Collide(Ball *pball, Vertex3Ds *phitnormal)
	{	
	const float dot = pball->vx*phitnormal->x  + pball->vy*phitnormal->y; // normal velocity to slingshot

	const bool threshold = (dot <= -m_psurface->m_d.m_slingshot_threshold);  // normal greater than threshold?

	if (!m_psurface->m_fDisabled && threshold) // enabled and if velocity greater than threshold level		
		{

#define TANX  phitnormal->y
#define TANY -phitnormal->x

		const float len = (v2.x - v1.x)*TANX + (v2.y - v1.y)*TANY; //rlc length of segment, Unit TAN points from V1 to V2

		Vertex2D vhitpoint;
		vhitpoint.x = pball->x - phitnormal->x * pball->radius; //project ball radius along norm
		vhitpoint.y = pball->y - phitnormal->y * pball->radius;

		// vhitpoint will now be the point where the ball hits the line
		// Calculate this distance from the center of the slingshot to get force

		const float btd = (vhitpoint.x - v1.x)*TANX + (vhitpoint.y - v1.y)*TANY; //rlc distance to vhit from V1
		float force = (btd+btd)/len - 1.0f;		// -1..+1
		force = 0.5f *(1.0f-force*force);	//rlc maximum value 0.5 ...I think this should have been 1.0...oh well
												// will match the previous physics
		force *= m_force;//-80;

		pball->vx -= phitnormal->x * force;	// boost velocity, drive into slingshot (counter normal)
		pball->vy -= phitnormal->y * force;	// allow CollideWall to handle the remainder
		}

	pball->CollideWall(phitnormal, m_elasticity, m_antifriction, m_scatter);

	if (m_pfe && !m_psurface->m_fDisabled)
		{			
			if (dot <= -m_threshold)
			{
			const float dx = pball->m_Event_Pos.x - pball->x; // is this the same place as last event????
			const float dy = pball->m_Event_Pos.y - pball->y; // if same then ignore it
			const float dz = pball->m_Event_Pos.z - pball->z;

			if ((dx*dx + dy*dy + dz*dz) > 0.25f)// must be a new place if only by a little
				{
				if (m_psurface->m_d.m_slingshot_threshold != 0.0f)// if new slingshot threshold is set use it
					{			
					if (threshold)
						{
						m_pfe->FireGroupEvent(DISPID_SurfaceEvents_Slingshot);
						m_slingshotanim.m_TimeReset = g_pplayer->m_timeCur + 100;
						}
					}	
				else if (dot <= -m_threshold)//legacy wall threshold
					{
					m_pfe->FireGroupEvent(DISPID_SurfaceEvents_Slingshot);
					m_slingshotanim.m_TimeReset = g_pplayer->m_timeCur + 100;
					}
				}
			}
		pball->m_Event_Pos.x = pball->x; 
		pball->m_Event_Pos.y = pball->y; 
		pball->m_Event_Pos.z = pball->z; //remember last collide position
		}
	}

PINFLOAT LineSegLevelEdge::HitTest(Ball *pball, PINFLOAT dtime, Vertex3Ds *phitnormal)
	{
	if (!m_fEnabled) return -1.0f;	

		// approach from either face, lateral contact, not rigid
	return this->HitTestBasic(pball, dtime, phitnormal, false, true, false);
	}

void LineSegLevelEdge::Collide(Ball *pball, Vertex3Ds *phitnormal)
	{
	}

void SlingshotAnimObject::Check3D()
	{
	if ((m_iframe == 0) && (m_TimeReset != 0) && m_fAnimations)
		{
		m_iframe = 1;
		m_fInvalid = fTrue;
		}
	else if ((m_iframe == 1) && (m_TimeReset < g_pplayer->m_timeCur))
		{
		m_iframe = 0;
		m_fInvalid = fTrue;
		m_TimeReset = 0;
		}
	}

ObjFrame *SlingshotAnimObject::Draw3D(RECT *prc)
	{
	if (m_iframe == 1)
		{
		ObjFrame * const pobjframe = m_pobjframe;
		return pobjframe;
		}

	return NULL;
	}

void SlingshotAnimObject::Reset()
	{
	// Do nothing right now - just let it draw as not hit
	}

HitGate::HitGate(Gate *pgate)
	{
	m_pgate = pgate;

	m_gateanim.m_angleMin = pgate->m_d.m_angleMin;
	m_gateanim.m_angleMax = pgate->m_d.m_angleMax;

	m_gateanim.m_friction = pgate->m_d.m_friction;
	m_gateanim.m_fVisible = pgate->m_d.m_fVisible;	

	m_gateanim.m_iframe = -1; //force new animation frame draw

	m_gateanim.m_angle = m_gateanim.m_angleMin;
	m_gateanim.m_anglespeed = 0;

	m_gateanim.m_pgate = pgate;
	m_gateanim.m_fOpen = fFalse;
	}

//rlc begin Change >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
PINFLOAT HitGate::HitTest(Ball *pball, PINFLOAT dtime, Vertex3Ds *phitnormal)
	{	
	if (!m_fEnabled) return -1.0f;		

	//return LineSeg::HitTest(pball, dtime, phitnormal);		//rlc test ok

	return HitTestBasic(pball, dtime, phitnormal, true, true, false);// normal face, lateral, non-rigid

	}
//rlc end Change >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void HitGate::Collide(Ball *pball, Vertex3Ds *phitnormal)
	{

	const PINFLOAT dot = pball->vx * phitnormal->x + pball->vy * phitnormal->y;
	if (dot > 0.0f) return;	//hit from back doesn't count

	const float h = m_pgate->m_d.m_height;

	//linear speed = ball speed
	//angular speed = linear/radius (height of hit)

	// h is the height of the gate axis.
	m_gateanim.m_anglespeed = fabsf(dot); // use this until a better value comes along

	if (fabsf(h - pball->radius) > 1.0f)				// avoid divide by zero
		m_gateanim.m_anglespeed /= h - pball->radius;

	if (m_pfe)
		{			
		const float dx = pball->m_Event_Pos.x - pball->x; // is this the same place as last event????
		const float dy = pball->m_Event_Pos.y - pball->y; // if same then ignore it
		const float dz = pball->m_Event_Pos.z - pball->z;

		pball->m_Event_Pos.x = pball->x;
		pball->m_Event_Pos.y = pball->y;
		pball->m_Event_Pos.z = pball->z; //remember last collide position

		if (dx*dx + dy*dy + dz*dz > 0.25f)// must be a new place if only by a little
			{
			m_pfe->FireGroupEvent(DISPID_HitEvents_Hit);
			}		
		}
	}

void GateAnimObject::UpdateDisplacements(PINFLOAT dtime)
	{
	m_angle += m_anglespeed * dtime;

	if (m_angle > m_angleMax)
		{
		m_angle = m_angleMax;
		m_pgate->FireVoidEventParm(DISPID_LimitEvents_EOS, fabsf(RADTOANG(m_anglespeed)));	// send EOS event
		if (m_anglespeed > 0.0f) m_anglespeed = 0.0f;
		}
	if (m_angle < m_angleMin)
		{
		m_angle = m_angleMin;
		m_pgate->FireVoidEventParm(DISPID_LimitEvents_BOS, fabsf(RADTOANG(m_anglespeed)));	// send Park event
		if (m_anglespeed < 0.0f) m_anglespeed = 0.0f;
		}
	}

void GateAnimObject::UpdateVelocities(PINFLOAT dtime)
	{
	if (!m_fOpen)
		{
		if (m_angle == m_angleMin) m_anglespeed = 0.0f;
		else m_anglespeed -= sinf(m_angle)*dtime * 0.0025f; // Center of gravity towards bottom of object, makes it stop vertical
		//else m_anglespeed -= sinf((m_angle - m_angleMin)*0.5f)*dtime * 0.02f; // Center of gravity towards bottom of object, makes it stop vertical
		m_anglespeed *= 1.0f - m_friction*dtime;
		}	
	}

void GateAnimObject::Check3D()
	{
	if (!m_fVisible)
		{
		if (m_iframe != -1)
			{
			m_fInvalid = fTrue;
			m_iframe = -1;
			}
		return;
		}
	
	int frame;
	if (m_pgate->m_d.m_angleMin != m_pgate->m_d.m_angleMax)
		{
		frame = (int)(((m_angle - m_pgate->m_d.m_angleMin)/(m_pgate->m_d.m_angleMax - m_pgate->m_d.m_angleMin)
				* (float)m_vddsFrame.Size()) + 0.5f);
		}
	else frame = 1;

	if (frame == m_vddsFrame.Size()) // Happens when the angle exactly equals m_angleMax
		{
		frame = m_vddsFrame.Size()-1;
		}

	if (frame != m_iframe)
		{
		m_iframe = frame;
		m_fInvalid = fTrue;
		}
	}

ObjFrame *GateAnimObject::Draw3D(RECT *prc)
	{
	if (!m_fVisible || m_iframe == -1) return NULL;

	ObjFrame * const pobjframe = m_vddsFrame.ElementAt(m_iframe);

	return pobjframe;
	}

void GateAnimObject::Reset()
	{
	m_iframe = -1;
	}

HitSpinner::HitSpinner(Spinner *pspinner, float height)
	{
	m_spinneranim.m_pspinner = pspinner;

	const float halflength = pspinner->m_d.m_length * 0.5f;

	const float radangle = ANGTORAD(pspinner->m_d.m_rotation);
	const float sn = sinf(radangle);
	const float cs = cosf(radangle);

	m_lineseg[0].m_rcHitRect.zlow = height;
	m_lineseg[0].m_rcHitRect.zhigh = height + 50.0f;
	m_lineseg[1].m_rcHitRect.zlow = height;
	m_lineseg[1].m_rcHitRect.zhigh = height + 50.0f;

	m_lineseg[0].m_pfe = NULL;
	m_lineseg[1].m_pfe = NULL;

	m_lineseg[0].v2.x = pspinner->m_d.m_vCenter.x + cs*(halflength + (float)PHYS_SKIN); //oversize  by the ball radius
	m_lineseg[0].v2.y = pspinner->m_d.m_vCenter.y + sn*(halflength + (float)PHYS_SKIN); //this will prevent clipping
	m_lineseg[0].v1.x = pspinner->m_d.m_vCenter.x - cs*(halflength + (float)PHYS_SKIN); // throught the edge of the
	m_lineseg[0].v1.y = pspinner->m_d.m_vCenter.y - sn*(halflength + (float)PHYS_SKIN); //spinner

	m_lineseg[1].v1.x = m_lineseg[0].v2.x;
	m_lineseg[1].v1.y = m_lineseg[0].v2.y;
	m_lineseg[1].v2.x = m_lineseg[0].v1.x;
	m_lineseg[1].v2.y = m_lineseg[0].v1.y;

	m_lineseg[0].CalcNormal();
	m_lineseg[1].CalcNormal();

	m_spinneranim.m_angleMax = ANGTORAD(pspinner->m_d.m_angleMax);
	m_spinneranim.m_angleMin = ANGTORAD(pspinner->m_d.m_angleMin);

	m_spinneranim.m_iframe = -1;

	m_spinneranim.m_angle = 0;
	m_spinneranim.m_anglespeed = 0;

	if (m_spinneranim.m_angle > m_spinneranim.m_angleMax) m_spinneranim.m_angle = m_spinneranim.m_angleMax;
	else if (m_spinneranim.m_angle < m_spinneranim.m_angleMin) m_spinneranim.m_angle = m_spinneranim.m_angleMin;	
	
	m_spinneranim.m_elasticity = pspinner->m_d.m_elasticity;
	m_spinneranim.m_friction = 1.0f - pspinner->m_d.m_friction;	//antifriction
	m_spinneranim.m_scatter = pspinner->m_d.m_scatter;
	m_spinneranim.m_fVisible = pspinner->m_d.m_fVisible;	
	}

PINFLOAT HitSpinner::HitTest(Ball *pball, PINFLOAT dtime, Vertex3Ds *phitnormal)
	{
	PINFLOAT hittime;

	if (!m_fEnabled) return -1.0f;	

	hittime = m_lineseg[0].HitTestBasic(pball, dtime, phitnormal, false, true, false);// any face, lateral, non-rigid
	if (hittime >= 0)
		{
		phitnormal[1].x = 1.0f;
		phitnormal[1].y = 0;

		return hittime;
		}

	hittime = m_lineseg[1].HitTestBasic(pball, dtime, phitnormal, false, true, false);// any face, lateral, non-rigid
	if (hittime >= 0)
		{
		phitnormal[1].x = 0;
		}

	return hittime;
	}

void HitSpinner::Collide(Ball *pball, Vertex3Ds *phitnormal)
	{
	const PINFLOAT dot = pball->vx * phitnormal->x + pball->vy * phitnormal->y;
	if (dot < 0) return;	//hit from back doesn't count
	
	const PINFLOAT h = m_spinneranim.m_pspinner->m_d.m_height*0.5f  + 30.0f;

	//linear speed = ball speed
	//angular speed = linear/radius (height of hit)

	// h is the height of the spinner axis;
	// Since the spinner has no mass in our equation, the spot
	// h -pball->radius will be moving a at linear rate of
	// 'speed'.  We can calculate the angular speed from that.

	m_spinneranim.m_anglespeed = fabsf(dot);		// use this until a better value comes along

	if (fabsf(h - pball->radius) > 1.0f)				// avoid divide by zero
		m_spinneranim.m_anglespeed /= h - pball->radius;

	// We encoded which side of the spinner the ball hit
	if (phitnormal[1].x > 0.0f)
		{
		m_spinneranim.m_anglespeed = -m_spinneranim.m_anglespeed; 
		}
	}

void SpinnerAnimObject::UpdateDisplacements(PINFLOAT dtime)
	{

	if (m_pspinner->m_d.m_angleMin != m_pspinner->m_d.m_angleMax)	//blocked spinner, limited motion spinner
		{
		m_angle += m_anglespeed * dtime;

		if (m_angle > m_angleMax)
			{
			m_angle = m_angleMax;
			m_pspinner->FireVoidEventParm(DISPID_LimitEvents_EOS, fabsf(m_anglespeed*(float)(180.0/M_PI)));	// send EOS event

			if (m_anglespeed > 0) m_anglespeed *= -(0.005f + m_elasticity);
			}
		if (m_angle < m_angleMin)
			{
			m_angle = m_angleMin;

			m_pspinner->FireVoidEventParm(DISPID_LimitEvents_BOS, fabsf(m_anglespeed*(float)(180.0/M_PI)));	// send Park event

			if (m_anglespeed < 0) m_anglespeed *= -(0.005f + m_elasticity);
			}
		}
	else
		{
		float target;

		if (m_anglespeed > 0)
			{
			if (m_angle < (float)M_PI) target = (float)M_PI;
			else target = (float)(3.0*M_PI);
			}
		else
			{
			if (m_angle < (float)M_PI) target = (float)(-M_PI);
			else target = (float)M_PI;
			}

		m_angle += m_anglespeed * dtime;

		if (m_anglespeed > 0)
			{
			if (m_angle > target) 
				m_pspinner->FireGroupEvent(DISPID_SpinnerEvents_Spin);
			}
		else
			{
			if (m_angle < target) 
				m_pspinner->FireGroupEvent(DISPID_SpinnerEvents_Spin);
			}

		while (m_angle > (float)(2.0*M_PI)) m_angle -= (float)(2.0*M_PI);
		while (m_angle < 0.0f) m_angle += (float)(2.0*M_PI);
		}
	}

void SpinnerAnimObject::UpdateVelocities(PINFLOAT dtime)
	{
	m_anglespeed -= sinf(m_angle) *dtime *0.0025f; // Center of gravity towards bottom of object, makes it stop vertical

	m_anglespeed *= 1.0f - (1.0f - m_pspinner->m_d.m_antifriction)*dtime;
	}

void SpinnerAnimObject::Check3D()
	{	
	if (!m_fVisible)
		{
		if (m_iframe != -1)
			{
			m_fInvalid = fTrue;
			m_iframe = -1;
			}
		return;
		}

	int frame;	
	const int cframes = m_vddsFrame.Size();

	if (m_pspinner->m_d.m_angleMin != m_pspinner->m_d.m_angleMax)
		{
		const float ang = RADTOANG(m_angle);

		frame = (int)(((ang - m_pspinner->m_d.m_angleMin)/(m_pspinner->m_d.m_angleMax - m_pspinner->m_d.m_angleMin))
				* (float)cframes - 0.5f);

		if (frame >= cframes) 
			frame = cframes-1;		//hold 	
		else if (frame < 0) 
			frame = 0; 
		}
	else 
		{
		float ang = m_angle;
		while (ang > (float)(2.0*M_PI)) ang -= (float)(2.0*M_PI);
		while (ang < 0.0f) ang += (float)(2.0*M_PI);

		frame = (int)(((ang * (float)(1.0/(2.0*M_PI))) * (float)cframes) + 0.5f);

		if (frame >= cframes) frame = 0;		//wrap
		}

	if (frame != m_iframe)
		{
		m_iframe = frame;
		m_fInvalid = fTrue;
		}
	}

ObjFrame *SpinnerAnimObject::Draw3D(RECT *prc)
	{
	if (!m_fVisible || m_iframe == -1) return NULL;

	ObjFrame * const pobjframe = m_vddsFrame.ElementAt(m_iframe);

	return pobjframe;
	}

void SpinnerAnimObject::Reset()
	{
	m_iframe = -1;
	}

void HitSpinner::Draw(HDC hdc)
	{
	m_lineseg[0].Draw(hdc);
	m_lineseg[1].Draw(hdc);
	}

void HitSpinner::CalcHitRect()
	{
	// Bounding rect for both lines will be the same
	m_lineseg[0].CalcHitRect();
	m_rcHitRect = m_lineseg[0].m_rcHitRect;
	}



Hit3DPoly::Hit3DPoly(Vertex3D *rgv, int count, bool keepptr) : m_cvertex(count)
	{
	if(keepptr)
		m_rgv = rgv;
	else {
		m_rgv = new Vertex3D[count];
		memcpy(m_rgv, rgv, count * sizeof(Vertex3D));
	}

	CalcNormal();

	m_fVisible = fFalse;
	m_elasticity = 0.3f;
	m_antifriction = 1.0f;
	m_scatter = 0;
	}

Hit3DPoly::~Hit3DPoly()
	{
	delete m_rgv;
	}

PINFLOAT Hit3DPoly::HitTest(Ball *pball, PINFLOAT dtime, Vertex3Ds *phitnormal)
	{
	return HitTestBasicPolygon(pball, dtime, phitnormal, (m_ObjType != eTrigger), (m_ObjType != eTrigger));
	}

PINFLOAT Hit3DPoly::HitTestBasicPolygon(Ball *pball, PINFLOAT dtime, Vertex3Ds *phitnormal, bool direction, bool rigid)
	{
	if (!m_fEnabled) return -1.0f;

	const PINFLOAT bnv = (normal.x*pball->vx) + (normal.y*pball->vy) + (normal.z*pball->vz);  //speed in Normal-vector direction

	if (direction && bnv >= 0)								//rlc ... return if clearly ball is receding from object
		return -1.0f;

	PINFLOAT hitx, hity, hitz; // Point on the ball that will hit the polygon, if it hits at all

	const PINFLOAT bRadius = pball->radius;
	hitx = pball->x - normal.x * bRadius; //rlc nearest point on ball ... projected radius along norm
	hity = pball->y - normal.y * bRadius;
	hitz = pball->z - normal.z * bRadius;

	const PINFLOAT bnd = normal.x * hitx + normal.y * hity + normal.z * hitz + D; // distance from plane to ball

	bool bUnHit = (bnv > C_LOWNORMVEL);
	bool inside = (bnd <= 0);									// in ball inside object volume

	PINFLOAT hittime;
	if (rigid) //rigid polygon
		{
		if (bnd < (float)(-PHYS_SKIN)) return -1.0f;	// (ball normal distance) excessive pentratration of object skin ... no collision HACK
			
		if (bnd >= (float)(-PHYS_SKIN) && bnd <= (float)PHYS_TOUCH)
			{
			if (inside || fabsf(bnv) > C_CONTACTVEL)		// fast velocity, return zero time
				hittime = 0;													//zero time for rigid fast bodies
			else if(bnd <= (float)(-PHYS_TOUCH)) hittime = 0;					// slow moving but embedded
			else hittime = bnd*(float)(1.0/(2.0*PHYS_TOUCH)) + 0.5f;	// don't compete for fast zero time events
			}
		else if (fabsf(bnv) > C_LOWNORMVEL )					// not velocity low ????
			hittime = bnd/(-bnv);								// rate ok for safe divide 
		else return -1.0f;										// wait for touching
		}
	else //non-rigid polygon
		{
		if (bnv * bnd >= 0)										// outside-receding || inside-approaching
			{
			if((m_ObjType != eTrigger) ||				// not a trigger
			   (!pball->m_vpVolObjs) ||					// temporary ball
			   (fabsf(bnd) >= (float)(PHYS_SKIN/2.0)) ||		// not to close ... nor to far away
			   (inside != (pball->m_vpVolObjs->IndexOf(m_pObj) < 0)))// ...ball outside and hit set or  ball inside and no hit set
				return -1.0f;

			hittime = 0;
			bUnHit = !inside;	// ball on outside is UnHit, otherwise it's a Hit
			}
		else hittime = bnd/(-bnv);	
		}

	if (hittime < 0 || hittime > dtime) return -1.0f;	// time is outside this frame ... no collision

	hitx += pball->vx*hittime;	// advance hit point to contact
	hity += pball->vy*hittime;
	hitz += pball->vz*hittime;

	// Do a point in poly test, using the xy plane, to see if the hit point is inside the polygon
	//this need to be changed to a point in polygon on 3D plane

	int crosscount=0;	// count of lines which the hit point is to the left of
	for (int i=0;i<m_cvertex;i++)
		{
		const int j = (i+1) % m_cvertex;

		const PINFLOAT x1 = m_rgv[i].x;
		const PINFLOAT y1 = m_rgv[i].y;
		const PINFLOAT x2 = m_rgv[j].x;
		const PINFLOAT y2 = m_rgv[j].y;

		if ((y1==y2) ||
		    (hity <= y1 && hity <= y2) || (hity > y1 && hity > y2) || // if out of y range, forget about this segment
		    (hitx >= x1 && hitx >= x2)) // Hit point is on the right of the line
			continue;

		if (hitx < x1 && hitx < x2)
			{
			crosscount++;
			continue;
			}

		if (x2 == x1)
			{
			if (hitx < x2)
				crosscount++;
			continue;
			}

		// Now the hard part - the hit point is in the line bounding box

		if (x2 - (y2 - hity)*(x1 - x2)/(y1 - y2) > hitx)
			crosscount++;
		}

	if (crosscount & 1)
		{
		phitnormal->x = m_rgv[0].z;
		phitnormal->y = m_rgv[2].z;

		if (!rigid)								// non rigid body collision? return direction
			phitnormal[1].x = (float)bUnHit;	// UnHit signal	is receding from outside target
			
		pball->m_HitDist = bnd;					//rlc-3dhit actual contact distance ... 
		pball->m_HitNormVel = bnv;
		pball->m_HitRigid = rigid;				// collision type

		return hittime;
		}

	return -1.0f;
	}


void Hit3DPoly::Draw(HDC hdc)
	{
	}

void Hit3DPoly::Collide(Ball *pball, Vertex3Ds *phitnormal)
	{
	if (m_ObjType != eTrigger) pball->Collide3DWall(&normal, m_elasticity, m_antifriction, m_scatter); 
	else		
		{
		if (!pball->m_vpVolObjs) return;

		const int i = pball->m_vpVolObjs->IndexOf(m_pObj); // if -1 then not in objects volume set (i.e not already hit)

		if ((phitnormal[1].x < 1.0f) == (i < 0))	// Hit == NotAlreadyHit
			{			
			pball->x += pball->vx * STATICTIME; //move ball slightly forward
			pball->y += pball->vy * STATICTIME;		
			pball->z += pball->vz * STATICTIME; 	
				
			if (i < 0)
				{	
				pball->m_vpVolObjs->AddElement(m_pObj);
				((Trigger*)m_pObj)->FireGroupEvent(DISPID_HitEvents_Hit);				
				}		
			else			
				{
				pball->m_vpVolObjs->RemoveElementAt(i);
				((Trigger*)m_pObj)->FireGroupEvent(DISPID_HitEvents_Unhit);				
				}
			}	
		}
	}

void Hit3DPoly::CalcNormal()
	{
	WORD * const rgi = new WORD[m_cvertex];

	for (int i=0;i<m_cvertex;i++)
		{
		rgi[i]= i;
		}

	SetNormal(m_rgv, rgi, m_cvertex, NULL, NULL, NULL);

	normal.x = m_rgv[0].nx;
	normal.y = m_rgv[0].ny;
	normal.z = m_rgv[0].nz;
	D = -(normal.x * m_rgv[0].x + normal.y * m_rgv[0].y + normal.z * m_rgv[0].z);

	delete rgi;
	}

void Hit3DPoly::CalcHitRect()
	{
	m_rcHitRect.left = FLT_MAX;
	m_rcHitRect.right = -FLT_MAX;
	m_rcHitRect.top = FLT_MAX;
	m_rcHitRect.bottom = -FLT_MAX;
	m_rcHitRect.zlow = FLT_MAX;
	m_rcHitRect.zhigh = -FLT_MAX;

	for (int i=0;i<m_cvertex;i++)
		{
		m_rcHitRect.left = min(m_rgv[i].x, m_rcHitRect.left);
		m_rcHitRect.right = max(m_rgv[i].x, m_rcHitRect.right);
		m_rcHitRect.top = min(m_rgv[i].y, m_rcHitRect.top);
		m_rcHitRect.bottom = max(m_rgv[i].y, m_rcHitRect.bottom);
		m_rcHitRect.zlow = min(m_rgv[i].z, m_rcHitRect.zlow);
		m_rcHitRect.zhigh = max(m_rgv[i].z, m_rcHitRect.zhigh);
		}
	}

Hit3DCylinder::Hit3DCylinder(Vertex3D *pv1, Vertex3D *pv2, Vertex3Ds *pvnormal)
	{
	v1.x = (*pv1).x;
	v1.y = (*pv1).y;
	v1.z = (*pv1).z;
	v2.x = (*pv2).x;
	v2.y = (*pv2).y;
	v2.z = (*pv2).z;
	normal = *pvnormal;
	radius = 0;
	CacheHitTransform();
	}

void Hit3DCylinder::CacheHitTransform()
	{
	Vertex3Ds vLine;
	vLine.x = v2.x - v1.x;
	vLine.y = v2.y - v1.y;
	vLine.z = v2.z - v1.z;

	vLine.Normalize();

	Vertex3Ds vup;
	vup.x = 0;
	vup.y = 0;
	vup.z = 1.0f;

	// Axis of rotation to make 3D cynlinder a cylinder along the z-axis
	CrossProduct(&vLine, &vup, &transaxis);

	// Angle to rotate the cylinder into the z-axis
	const PINFLOAT dot = vLine.Dot(&vup);

	transangle = acosf(-dot);

	vtrans[0] = v1;
	vtrans[1] = v2;

	RotateAround(&transaxis, vtrans, 2, transangle);
	}

PINFLOAT Hit3DCylinder::HitTest(Ball *pball, PINFLOAT dtime, Vertex3Ds *phitnormal)
	{
	if (!m_fEnabled) return -1.0f;	

	Ball ballT = *pball;

	Vertex3Ds vball;
	vball.x = ballT.x;
	vball.y = ballT.y;
	vball.z = ballT.z;

	RotateAround(&transaxis, &vball, 1, transangle);

	Vertex3Ds vvelocity;
	vvelocity.x = ballT.vx;
	vvelocity.y = ballT.vy;
	vvelocity.z = ballT.vz;

	RotateAround(&transaxis, &vvelocity, 1, transangle);

	ballT.x = vball.x;
	ballT.y = vball.y;
	ballT.z = vball.z + pball->radius;
	ballT.vx = vvelocity.x;
	ballT.vy = vvelocity.y;
	ballT.vz = vvelocity.z;
	ballT.radius = pball->radius;

	center.x = vtrans[0].x;
	center.y = vtrans[0].y;
	zlow = min(vtrans[0].z, vtrans[1].z);
	zhigh = max(vtrans[0].z, vtrans[1].z);

	const PINFLOAT hittime = HitTestRadius(&ballT, dtime, phitnormal);

	if (hittime >= 0)
		{
		Vertex3Ds mynormal;
		mynormal.x = phitnormal->x;
		mynormal.y = phitnormal->y;
		mynormal.z = 0;

		RotateAround(&transaxis, &mynormal, 1, -transangle);
		*phitnormal = mynormal;
		}

	return hittime;
	}

void Hit3DCylinder::Collide(Ball *pball, Vertex3Ds *phitnormal)
	{
	pball->Collide3DWall(phitnormal, m_elasticity, m_antifriction, m_scatter);
	}

void Hit3DCylinder::CalcHitRect()
	{
	m_rcHitRect.left = min(v1.x, v2.x);
	m_rcHitRect.right = max(v1.x, v2.x);
	m_rcHitRect.top = min(v1.y, v2.y);
	m_rcHitRect.bottom = max(v1.y, v2.y);
	m_rcHitRect.zlow = min(v1.z, v2.z);
	m_rcHitRect.zhigh = max(v1.z, v2.z);
	}

Hit3DPolyDrop::Hit3DPolyDrop(Vertex3D *rgv, int count, bool keepptr) : Hit3DPoly(rgv, count, keepptr)
	{
	m_polydropanim.m_iframe = -1;
	m_fVisible = fFalse;
	}

void PolyDropAnimObject::Check3D()
	{
	if (m_iframe != m_iframedesire)
		{
		m_iframe = m_iframedesire;
		m_fInvalid = fTrue;
		}
	}

ObjFrame *PolyDropAnimObject::Draw3D(RECT *prc)
	{
	if (m_iframe == -1) return NULL;

	ObjFrame * const pobjframe = m_pobjframe[m_iframe];
	return pobjframe;
	}

void PolyDropAnimObject::Reset()
	{
	m_iframe = -1;
	}

ObjFrame *TextboxAnimObject::Draw3D(RECT *prc)
	{
	g_pplayer->m_pin3d.m_pddsBackBuffer->Blt(&m_ptextbox->m_pobjframe->rc, m_ptextbox->m_pobjframe->pdds, NULL, DDBLTFAST_WAIT, NULL);

	return NULL;
	}

void TextboxAnimObject::Reset()
	{
	}

// this function is called every frame to see if the object needs updating
//
void DispReelAnimObject::Check3D()
{
    // update the reels animation (returns saying weither to redraw the frame object or not)
    m_fInvalid = m_pDispReel->RenderAnimation();
}

// this function is called when it is time to be drawn (Z-Ordered)
// and only if Check3D() tells it to. (m_fInvalid == true)
//
ObjFrame *DispReelAnimObject::Draw3D(RECT *prc)
{
	if(!m_pDispReel || !m_pDispReel->m_pobjframe)  //rlc-problem6 end bad pointers, fix needed
		return NULL;

	RECT	rc;
	rc.left = 0;		
	rc.top = 0;	
	
	rc.right = m_pDispReel->m_pobjframe->rc.right - m_pDispReel->m_pobjframe->rc.left; //rlc uninitialized
	rc.bottom = m_pDispReel->m_pobjframe->rc.bottom - m_pDispReel->m_pobjframe->rc.top;	
	
	g_pplayer->m_pin3d.m_pddsBackBuffer->BltFast(m_pDispReel->m_pobjframe->rc.left,
												 m_pDispReel->m_pobjframe->rc.top,
												 m_pDispReel->m_pobjframe->pdds,
												 &rc,
												 DDBLTFAST_SRCCOLORKEY);
	// return with no object
	return (ObjFrame *)NULL;
}

void DispReelAnimObject::Reset()
	{
	}

// this function is called every frame to see if the object needs updating
//
void LightSeqAnimObject::Check3D()
{
    // update the reels animation (returns saying weither to redraw the frame object or not)
    m_fInvalid = m_pLightSeq->RenderAnimation();
}

// this function is called when it is time to be drawn (Z-Ordered)
// and only if Check3D() tells it to. (m_fInvalid == true)
//
ObjFrame *LightSeqAnimObject::Draw3D(RECT *prc)
{
	// return with no object
	return NULL;
}

TriggerLineSeg::TriggerLineSeg()
	{
	m_fEnabled = fTrue;
	}

//rlc change >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

PINFLOAT TriggerLineSeg::HitTest(Ball *pball, PINFLOAT dtime, Vertex3Ds *phitnormal)//rlc problem-5
	{

	if (!m_ptrigger->m_hitEnabled) return -1.0f;				//rlc custom shape trigger
	
    // approach either face, not lateral-rolling point (assume center),  not a rigid body contact
	return this->HitTestBasic(pball, dtime, phitnormal, false,false, false); 	
	}


	//rlc  end change <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

void TriggerLineSeg::Collide(Ball *pball, Vertex3Ds *phitnormal)
	{
	if((m_ObjType != eTrigger) ||
	   (!pball->m_vpVolObjs)) return;

	const int i = pball->m_vpVolObjs->IndexOf(m_pObj); // if -1 then not in objects volume set (i.e not already hit)

	if ((phitnormal[1].x < 1.0f) == (i < 0))	// Hit == NotAlreadyHit
		{		
		pball->x += pball->vx * STATICTIME; //move ball slightly forward
		pball->y += pball->vy * STATICTIME;		
		pball->z += pball->vz * STATICTIME; 	
		
		if (i < 0)
			{	
			pball->m_vpVolObjs->AddElement(m_pObj);
			((Trigger*)m_pObj)->FireGroupEvent(DISPID_HitEvents_Hit);			
			}		
		else			
			{
			pball->m_vpVolObjs->RemoveElementAt(i);
			((Trigger*)m_pObj)->FireGroupEvent(DISPID_HitEvents_Unhit);			
			}
		}	
	}

TriggerHitCircle::TriggerHitCircle()
	{
	m_fEnabled = fTrue;
	}


PINFLOAT TriggerHitCircle::HitTest(Ball *pball, PINFLOAT dtime, Vertex3Ds *phitnormal)
	{
	return HitTestBasicRadius(pball, dtime, phitnormal, false, false, false);//any face, not-lateral, non-rigid
	}


void TriggerHitCircle::Collide(Ball *pball, Vertex3Ds *phitnormal)
	{
	if((m_ObjType < eTrigger) || // triggers and kickers
	   (!pball->m_vpVolObjs)) return;

	const int i = pball->m_vpVolObjs->IndexOf(m_pObj); // if -1 then not in objects volume set (i.e not already hit)

	if ((phitnormal[1].x < 1.0f) == (i < 0))	// Hit == NotAlreadyHit
		{			
		pball->x += pball->vx * STATICTIME; //move ball slightly forward
		pball->y += pball->vy * STATICTIME;		
		pball->z += pball->vz * STATICTIME; 		

		if (i < 0)
			{	
			pball->m_vpVolObjs->AddElement(m_pObj);
			((Trigger*)m_pObj)->FireGroupEvent(DISPID_HitEvents_Hit);
			}		
		else			
			{
			pball->m_vpVolObjs->RemoveElementAt(i);
			((Trigger*)m_pObj)->FireGroupEvent(DISPID_HitEvents_Unhit);
			}
		}	
	}	
