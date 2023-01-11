#pragma once
#include "SteeringAgent.h"
#include "projects/Shared/Interests/VisionCone.h"
#include "projects/Shared/Interests/InterestRecord.h"
#include "projects/Shared/Interests/InterestSource.h"

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

	bool CheckInterestSources(const std::list<InterestSource>& interestSources);
	
	bool IsInvestegating();

	InterestSource GetNextInterestSource() { return m_NextInterestSource; };

	void SetPatrolBehavior(ISteeringBehavior* pBehavior) { m_pPatrolBehavior = pBehavior; }
	ISteeringBehavior* GetPatrolBehavior() const { return m_pPatrolBehavior; }

	bool FinishedInvestigating() { return !m_IsInvestegating; };
	void SetStartAngle() { m_StartAngle = GetRotation(); };

protected:
	//--- Datamembers ---
	VisionCone* m_pVisionCone{};
	bool m_IsInvestegating{false};
	float m_StartAngle{};
	ISteeringBehavior* m_pPatrolBehavior = nullptr;

	InterestSource m_NextInterestSource = InterestSource();
	//--- Helper Functions ---
	bool HasInterest(InterestSource& interestSource);
};

