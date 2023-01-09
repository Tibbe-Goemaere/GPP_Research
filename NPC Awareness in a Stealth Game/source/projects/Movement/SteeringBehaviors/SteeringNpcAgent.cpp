#include "stdafx.h"
#include "SteeringNpcAgent.h"

SteeringNpcAgent::~SteeringNpcAgent()
{
	SAFE_DELETE(m_pVisionCone);
	//SAFE_DELETE(m_pPatrolBehavior);
	//SAFE_DELETE(m_pInvestigateBehavior);
}

void SteeringNpcAgent::Update(float dt)
{
	SteeringAgent::Update(dt);
	m_pVisionCone->UpdatePos(GetRotation(),GetPosition());
	if (m_IsInvestegating)
	{

	}
}

void SteeringNpcAgent::Render(float dt)
{
	SteeringAgent::Render(dt);
	m_pVisionCone->Render();
}

void SteeringNpcAgent::SetVisionCone(const VisionCone& visionCone)
{
	m_pVisionCone = new VisionCone(visionCone);
}

bool SteeringNpcAgent::IsInVision(Elite::Vector2 pos)
{
	return m_pVisionCone->IsOverlapping(pos);
}

bool SteeringNpcAgent::CheckInterestSources(const InterestSource& interestSource)
{
	if (IsInVision(interestSource.GetSource().position))
	{
		SetSteeringBehavior(m_pInvestigateBehavior);
		m_IsInvestegating = true;
		return true;
	}
	return false;
}

bool SteeringNpcAgent::IsInvestegating()
{
	return m_IsInvestegating;
}

bool SteeringNpcAgent::HasInterest(InterestSource& interestSource)
{
	return true;
}
