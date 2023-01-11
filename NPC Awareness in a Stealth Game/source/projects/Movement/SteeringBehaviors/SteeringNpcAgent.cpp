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
	//std::cout << GetRotation() << "\n";
	switch (m_NextInterestSource.GetType())
	{
	case::InterestSource::Senses::Sight:
		if (m_pSteeringBehavior)
		{
			
		}
		break;
	case::InterestSource::Senses::Sound:
		if (Elite::ToDegrees(GetRotation()) > Elite::ToDegrees(m_StartAngle) - 1.f &&
			Elite::ToDegrees(GetRotation()) < Elite::ToDegrees(m_StartAngle) - 0.1f)
		{
			m_IsInvestegating = false;
		}
		break;
	default:
		break;
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

bool SteeringNpcAgent::IsInRadius(Elite::Vector2 pos, float radius)
{
	return GetPosition().Distance(pos) <= radius + GetRadius();
}

bool SteeringNpcAgent::CheckInterestSources(const std::list<InterestSource>& interestSources)
{
	std::vector<InterestSource> validInterestSources{};
	for (auto interestSource : interestSources)
	{
		if (interestSource.GetType() == InterestSource::Senses::Sight)
		{
			if (IsInVision(interestSource.GetSource().position))
			{
				validInterestSources.push_back(interestSource);
			}
		}

		if (interestSource.GetType() == InterestSource::Senses::Sound)
		{
			if (IsInRadius(interestSource.GetSource().position, interestSource.GetRadius()))
			{
				validInterestSources.push_back(interestSource);
			}
			
		}
		
	}

	if (validInterestSources.size() == 0)
	{
		return false;
	}

	InterestSource nextInterestSource{ validInterestSources[0] };
	if (m_IsInvestegating)
	{
		nextInterestSource = m_NextInterestSource;
	}


	for (auto interestSource : validInterestSources)
	{
		if (interestSource.GetType() == InterestSource::Senses::Sight)
		{
			if (IsInVision(interestSource.GetSource().position))
			{
				if (interestSource.GetPriority() < nextInterestSource.GetPriority())
				{
					nextInterestSource = interestSource;
				}
			}
		}
		
		if (interestSource.GetType() == InterestSource::Senses::Sound)
		{
			if (IsInRadius(interestSource.GetSource().position,interestSource.GetRadius()))
			{
				if (interestSource.GetPriority() < nextInterestSource.GetPriority())
				{
					nextInterestSource = interestSource;
				}
			}
		}
		
	}
	m_NextInterestSource = nextInterestSource;
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
