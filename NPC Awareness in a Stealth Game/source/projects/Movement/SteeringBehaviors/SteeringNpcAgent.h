#pragma once
#include "SteeringAgent.h"
#include "projects/Shared/VisionCone.h"

class SteeringNpcAgent : public SteeringAgent
{
public:
	//--- Constructor & Destructor ---
	SteeringNpcAgent() = default;
	SteeringNpcAgent(float radius) : SteeringAgent(radius) {};
	virtual ~SteeringNpcAgent() = default;

	//--- Agent Functions ---
	void Update(float dt) override;
	void Render(float dt) override;

	float GetMaxLinearSpeed() const { return m_MaxLinearSpeed; }
	void SetMaxLinearSpeed(float maxLinSpeed) { m_MaxLinearSpeed = maxLinSpeed; }

	float GetMaxAngularSpeed() const { return m_MaxAngularSpeed; }
	void SetMaxAngularSpeed(float maxAngSpeed) { m_MaxAngularSpeed = maxAngSpeed; }

	bool IsAutoOrienting() const { return m_AutoOrient; }
	void SetAutoOrient(bool autoOrient) { m_AutoOrient = autoOrient; }

	Elite::Vector2 GetDirection() const { return GetLinearVelocity().GetNormalized(); }

	virtual void SetSteeringBehavior(ISteeringBehavior* pBehavior) { m_pSteeringBehavior = pBehavior; }
	ISteeringBehavior* GetSteeringBehavior() const { return m_pSteeringBehavior; }

	void SetRenderBehavior(bool isEnabled) { m_RenderBehavior = isEnabled; }
	bool CanRenderBehavior() const { return m_RenderBehavior; }

	Elite::Vector2 GetOldPos() const;
	void SetOldPos(Elite::Vector2 oldPos);

	void SetVisionCone(const VisionCone& visionCone);
	bool IsInVision(Elite::Vector2 pos);

protected:
	//--- Datamembers ---
	ISteeringBehavior* m_pSteeringBehavior = nullptr;

	float m_MaxLinearSpeed = 10.f;
	float m_MaxAngularSpeed = 10.f;
	bool m_AutoOrient = false;
	bool m_RenderBehavior = false;

	Elite::Vector2 m_OldPos = { 0,0 };

	VisionCone* m_pVisionCone{};
};

