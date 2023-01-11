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
		//std::cout << "check other\n";
		if (interestSource.GetType() == InterestSource::Senses::Sound)
		{
			//std::cout << "check it\n";
			if (IsInRadius(interestSource.GetSource().position,interestSource.GetRadius()))
			{
				//std::cout << "isInRadius\n";
				if (interestSource.GetPriority() < nextInterestSource.GetPriority())
				{
					//std::cout << "nextSource\n";
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
