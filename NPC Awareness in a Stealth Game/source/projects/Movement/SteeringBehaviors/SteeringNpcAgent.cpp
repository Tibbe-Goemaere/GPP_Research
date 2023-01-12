#include "stdafx.h"
#include "SteeringNpcAgent.h"
#include "framework\EliteAI\EliteNavigation\Algorithms\ENavGraphPathfinding.h"


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
	m_pVisionCone->UpdatePos(GetRotation(), GetPosition());
	//std::cout << GetRotation() << "\n";
	switch (m_NextInterestSource.GetType())
	{
	case::InterestSource::Senses::Sight:
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
	if (m_IsLookingAround)
	{
		m_Timer += dt;
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

bool SteeringNpcAgent::CheckInterestSources(const std::list<InterestSource>& interestSources, Elite::NavGraph* pNavGraph, std::vector<Elite::Vector2> debugNodePositions, std::vector<Elite::Portal> portals)
{
	std::vector<InterestSource> validInterestSources{};
	for (auto interestSource : interestSources)
	{
		auto path = Elite::NavMeshPathfinding::FindPath(interestSource.GetSource().position, GetPosition(), pNavGraph, debugNodePositions, portals);
		if (interestSource.GetType() == InterestSource::Senses::Sight)
		{
			if (IsInVision(interestSource.GetSource().position) && path.size() <= 2)
			{
				validInterestSources.push_back(interestSource);
			}
		}

		if (interestSource.GetType() == InterestSource::Senses::Sound)
		{
			if (IsInRadius(interestSource.GetSource().position, interestSource.GetRadius()) && path.size() <= 2)
			{
				validInterestSources.push_back(interestSource);
			}
			else if (IsInRadius(interestSource.GetSource().position, interestSource.GetRadius()) && GetPathDistance(path) <= interestSource.GetRadius())
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

	for (auto interestSource : validInterestSources)
	{
		if (interestSource.GetPriority() >= nextInterestSource.GetPriority())
		{
			nextInterestSource = interestSource;
		}
	}
	if (nextInterestSource.GetPriority() >= m_NextInterestSource.GetPriority())
	{
		m_NextInterestSource = nextInterestSource;
	}
	
	m_IsInvestegating = true;
	return true;
}

bool SteeringNpcAgent::IsInvestegating()
{
	return m_IsInvestegating;
}

bool SteeringNpcAgent::HasLookedAround()
{
	if (m_Timer >= m_LookAroundTime)
	{
		m_IsLookingAround = false;
		m_Timer = 0;
		return true;
	}
	return false;
}

void SteeringNpcAgent::IsDoneInvestigating()
{
	m_NextInterestSource = InterestSource();
}

bool SteeringNpcAgent::HasInterest(InterestSource& interestSource)
{
	return true;
}

float SteeringNpcAgent::GetPathDistance(const std::vector <Elite::Vector2> path)
{
	float distance{};
	for (size_t i = 0; i < path.size() - 1; i++)
	{
		distance += path[i].Distance(path[i + 1]);
	}
	std::cout << distance << "\n";
	return distance;
}