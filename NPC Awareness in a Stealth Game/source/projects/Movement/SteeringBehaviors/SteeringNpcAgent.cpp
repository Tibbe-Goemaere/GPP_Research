#include "stdafx.h"
#include "SteeringNpcAgent.h"

void SteeringNpcAgent::Update(float dt)
{
	SteeringAgent::Update(dt);
	m_pVisionCone->UpdatePos(GetRotation(),GetPosition());
}

void SteeringNpcAgent::Render(float dt)
{
	SteeringAgent::Render(dt);
	m_pVisionCone->Render();
}

Elite::Vector2 SteeringNpcAgent::GetOldPos() const
{
	return SteeringAgent::GetOldPos();
}

void SteeringNpcAgent::SetOldPos(Elite::Vector2 oldPos)
{
	SteeringAgent::SetOldPos(oldPos);
}

void SteeringNpcAgent::SetVisionCone(const VisionCone& visionCone)
{
	m_pVisionCone = new VisionCone(visionCone);
}

bool SteeringNpcAgent::IsInVision(Elite::Vector2 pos)
{
	return m_pVisionCone->IsOverlapping(pos);
}
