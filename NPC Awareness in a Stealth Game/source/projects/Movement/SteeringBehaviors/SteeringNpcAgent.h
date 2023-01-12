#pragma once
#include "SteeringAgent.h"
#include "projects/Shared/Interests/VisionCone.h"
#include "projects/Shared/Interests/InterestRecord.h"
#include "projects/Shared/Interests/InterestSource.h"
#include "framework\EliteAI\EliteNavigation\Algorithms\EPathSmoothing.h"

namespace Elite
{
	class NavGraph;
	struct Portal;
}

class SteeringNpcAgent : public SteeringAgent
{
public:
	//--- Constructor & Destructor ---
	SteeringNpcAgent() = default;
	SteeringNpcAgent(float radius) : SteeringAgent(radius) {};
	virtual ~SteeringNpcAgent();

	//--- Agent Functions ---
	void Update(float dt) override;
	void Render(float dt) override;

	void SetVisionCone(const VisionCone& visionCone);
	bool IsInVision(Elite::Vector2 pos);
	bool IsInRadius(Elite::Vector2 pos, float radius);

	bool CheckInterestSources(const std::list<InterestSource>& interestSources, Elite::NavGraph* pNavGraph, std::vector<Elite::Vector2> debugNodePositions, std::vector<Elite::Portal> portals);
	
	bool IsInvestegating();

	InterestSource GetNextInterestSource() { return m_NextInterestSource; };

	void SetPatrolBehavior(ISteeringBehavior* pBehavior) { m_pPatrolBehavior = pBehavior; }
	ISteeringBehavior* GetPatrolBehavior() const { return m_pPatrolBehavior; }

	bool FinishedInvestigating() const { return !m_IsInvestegating; };
	void SetStartAngle() { m_StartAngle = GetRotation(); };
	void StartLookingAround() {	m_IsLookingAround = true;	};
	bool HasLookedAround();
	float GetMaxLookAroundTime() const { return m_LookAroundTime; };
protected:
	//--- Datamembers ---
	VisionCone* m_pVisionCone{};
	bool m_IsInvestegating{false};
	float m_StartAngle{};
	ISteeringBehavior* m_pPatrolBehavior = nullptr;
	float m_Timer{};
	const float m_LookAroundTime{4.f};
	InterestSource m_NextInterestSource = InterestSource();
	bool m_IsLookingAround{ false };
	//--- Helper Functions ---
	bool HasInterest(InterestSource& interestSource);
	float GetPathDistance(const std::vector <Elite::Vector2> path);
};

