#include "stdafx.h"
#include "SteeringNpcAgent.h"

SteeringNpcAgent::~SteeringNpcAgent()
{
	SAFE_DELETE(m_pVisionCone);
	//SAFE_DELETE(m_pPatrolBehavior);
	//SAFE_DELETE(m_pInvestigateBehavior);
	//SAFE_DELETE(m_pNextInterestSource);
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

bool SteeringNpcAgent::CheckInterestSources(const std::list<InterestSource>& interestSources)
{
	std::vector<InterestSource> validInterestSources{};
	for (auto interestSource : interestSources)
	{
		if (IsInVision(interestSource.GetSource().position))
		{
			validInterestSources.push_back(interestSource);
		}
	}

	if (validInterestSources.size() == 0)
	{
		return false;
	}

	InterestSource nextInterestSource{ validInterestSources[0]};

	for (auto interestSource : validInterestSources)
	{
		if (IsInVision(interestSource.GetSource().position))
		{
			if (interestSource.GetPriority() < nextInterestSource.GetPriority())
			{
				nextInterestSource = interestSource;
			}
		}
	}
	m_nextInterestSource = nextInterestSource;
	m_IsInvestegating = true;
	return true;
}

bool SteeringNpcAgent::IsInvestegating()
{
	return m_IsInvestegating;
}

bool SteeringNpcAgent::HasInterest(InterestSource& interestSource)
{
	return true;
}
