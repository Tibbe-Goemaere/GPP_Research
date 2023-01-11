/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere, Thomas Goussaert
/*=============================================================================*/
// SteeringBehaviors.h: SteeringBehaviors interface and different implementations
/*=============================================================================*/
#ifndef ELITE_STEERINGBEHAVIORS
#define ELITE_STEERINGBEHAVIORS

//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "../SteeringHelpers.h"
class SteeringAgent;
class Obstacle;

#pragma region **ISTEERINGBEHAVIOR** (BASE)
class ISteeringBehavior
{
public:
	ISteeringBehavior() = default;
	virtual ~ISteeringBehavior() = default;

	virtual SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) = 0;

	//Seek Functions
	void SetTarget(const TargetData& target) { m_Target = target; }
	//Patrol Functions
	void SetTargets(const TargetData& target, const TargetData& target2) { m_Target = target; m_Target2 = target2; }
	template<class T, typename std::enable_if<std::is_base_of<ISteeringBehavior, T>::value>::type* = nullptr>
	T* As()
	{ return static_cast<T*>(this); }

protected:
	TargetData m_Target;
	TargetData m_Target2;
};
#pragma endregion

///////////////////////////////////////
//SEEK
//****
class Seek : public ISteeringBehavior
{
public:
	Seek() = default;
	virtual ~Seek() = default;

	//Seek Behaviour
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};

///////////////////////////////////////
//FLEE
//****

class Flee : public ISteeringBehavior
{
public:
	Flee() = default;
	virtual ~Flee() = default;

	//Flee Behaviour
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};

///////////////////////////////////////
//ARRIVE
//******

class Arrive : public ISteeringBehavior
{
public:
	Arrive() = default;
	virtual ~Arrive() = default;

	//Arrive Behaviour
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
	void SetArriveDistance(const float distance) { m_ArriveDistance = distance; };

private:
	float m_ArriveDistance{};
};

///////////////////////////////////////
//FACE
//****

class Face : public ISteeringBehavior
{
public:
	Face() = default;
	virtual ~Face() = default;

	//Face Behaviour
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};

///////////////////////////////////////
//Wander
//******

class Wander : public Seek
{
public:
	Wander() = default;
	virtual ~Wander() = default;

	//Wander Behaviour
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

	void SetWanderOffset(float offset);
	void SetWanderRadius(float radius);
	void SetMaxAngleChange(float rad);

protected:
	float m_OffsetRadius = 5.f;
	float m_Radius = 4.f;
	float m_MaxAngleChange = 45;
	float m_WanderAngle = 0.f;
};

///////////////////////////////////////
//PURSUIT
//*******

class Pursuit : public ISteeringBehavior
{
public:
	Pursuit() = default;
	virtual ~Pursuit() = default;

	//Pursuit Behaviour
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};

///////////////////////////////////////
//EVADE
//*****

class Evade : public ISteeringBehavior
{
public:
	Evade() = default;
	virtual ~Evade() = default;

	//Evade Behaviour
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
	void SetEvadeRadius(float evadeRadius) { m_EvadeRadius = evadeRadius; };
private:
	float m_EvadeRadius = 10.f;
};

///////////////////////////////////////
//PATROL
//******
class Patrol : public ISteeringBehavior
{
public:
	Patrol() = default;
	virtual ~Patrol() = default;

	//Patrol Behaviour
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
private:
	bool m_IsGoingToFirstTarget{ true };
};

///////////////////////////////////////
//TURN
//****

class Turn : public ISteeringBehavior
{
public:
	Turn() = default;
	virtual ~Turn() = default;

	//Turn Behaviour
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};

///////////////////////////////////////
//LOOKAROUND
//**********

class LookAround : public ISteeringBehavior
{
public:
	LookAround() = default;
	virtual ~LookAround() = default;

	//LookAround Behaviour
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
	bool IsDone() { return m_IsReady; };
private:
	float m_Timer{};
	bool m_IsLookingUp{ true };
	bool m_IsReady{ false };
};
#endif


