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

	void SetInterestRecord(const InterestRecord& interestRecord);
	bool CheckInterestSources(const InterestSource& interestSource);
	
	bool IsInvestegating();

protected:
	//--- Datamembers ---
	VisionCone* m_pVisionCone{};
	bool m_IsInvestegating{false};

	ISteeringBehavior* m_pPatrolBehavior = nullptr;
	ISteeringBehavior* m_pInvestigateBehavior = nullptr;
	//--- Helper Functions ---
	bool HasInterest(InterestSource& interestSource);
};

