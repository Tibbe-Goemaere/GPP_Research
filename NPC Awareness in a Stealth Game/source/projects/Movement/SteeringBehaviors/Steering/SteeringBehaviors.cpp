//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

//Includes
#include "SteeringBehaviors.h"
#include "../SteeringAgent.h"
#include "framework\EliteMath\EMatrix2x3.h"

using namespace Elite;

//SEEK
//****
SteeringOutput Seek::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	steering.LinearVelocity = m_Target.Position - pAgent->GetPosition();
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(),steering.LinearVelocity,5,{0,1,0});
	}

	return steering;
}

//Flee
//****
SteeringOutput Flee::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	steering.LinearVelocity = pAgent->GetPosition() - m_Target.Position;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0,1,0 });
	}

	return steering;
}

//Arrive
//******
SteeringOutput Arrive::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	const float slowRadius{ 15.f };

	steering.LinearVelocity = m_Target.Position - pAgent->GetPosition();
	const float distance{ steering.LinearVelocity.Magnitude() };
	steering.LinearVelocity.Normalize();
	if (distance < slowRadius)
	{
		steering.LinearVelocity *= pAgent->GetMaxLinearSpeed() * (distance / slowRadius);
	}
	else
	{
		steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();
	}
	

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0,1,0 });
	}

	return steering;
}

//Face
//****
SteeringOutput Face::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};
	
	Vector2 toTarget = m_Target.Position - pAgent->GetPosition();
	Vector2 agentDirection{ cosf(pAgent->GetRotation()),sinf(pAgent->GetRotation()) };
	
	
	if (AngleBetween( toTarget, agentDirection) < 0)
	{
		steering.AngularVelocity += pAgent->GetMaxAngularSpeed();
	}
	else if (AngleBetween(toTarget, agentDirection) > 0)
	{
		steering.AngularVelocity -= pAgent->GetMaxAngularSpeed();
	}
	else if (AngleBetween(toTarget, agentDirection) == 0)
	{
		steering.AngularVelocity = 0;
	}

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0,1,0 });
	}

	return steering;
}

//Wander
//******
SteeringOutput Wander::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};
	

	Vector2 direction{ cosf(pAgent->GetRotation()) ,sinf(pAgent->GetRotation()) };
	Vector2 center{ pAgent->GetPosition() + direction * m_OffsetRadius };
	m_WanderAngle += randomFloat( m_MaxAngleChange * 2 ) - m_MaxAngleChange;
	Vector2 newTarget{center.x + m_Radius * cosf(ToRadians(m_WanderAngle)), center.y + m_Radius * sinf(ToRadians(m_WanderAngle))};

	steering.LinearVelocity = newTarget - pAgent->GetPosition();
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0,1,0 });
	}
	//DEBUGRENDERER2D->DrawCircle(center, m_Radius, { 0,0,1}, 0);

	return steering;
}

void Wander::SetWanderOffset(float offset)
{
	m_OffsetRadius = offset;
}

void Wander::SetWanderRadius(float radius)
{
	m_Radius = radius;
}

void Wander::SetMaxAngleChange(float rad)
{
	m_MaxAngleChange = rad;
}

//PURSUIT
//*******
SteeringOutput Pursuit::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	Vector2 toTarget{m_Target.Position - pAgent->GetPosition()};
	
	Vector2 target{m_Target.Position};
	
	if (toTarget.Magnitude() >= m_Target.LinearVelocity.Magnitude())
	{
		target += m_Target.LinearVelocity;
	}
	else if(toTarget.Magnitude() > 0.f)
	{
		target += m_Target.LinearVelocity * toTarget / m_Target.LinearVelocity.Magnitude();
	}
	steering.LinearVelocity = target - pAgent->GetPosition();
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0,1,0 });
	}

	return steering;
}

//EVADE
//*****
SteeringOutput Evade::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	Vector2 fromTarget = pAgent->GetPosition() - m_Target.Position;
	float distance = fromTarget.Magnitude();
	SteeringOutput steering = {};

	if (distance > m_EvadeRadius)
	{
		steering.IsValid = false;
		return steering;
	}



	Vector2 toTarget{ m_Target.Position - pAgent->GetPosition() };
	Vector2 target{ m_Target.Position };

	if (toTarget.Magnitude() >= m_Target.LinearVelocity.Magnitude())
	{
		target += m_Target.LinearVelocity;
	}
	else if (toTarget.Magnitude() > 0.f)
	{
		target += m_Target.LinearVelocity * toTarget / m_Target.LinearVelocity.Magnitude();
	}
	steering.LinearVelocity = pAgent->GetPosition() - target;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0,1,0 });
	}

	return steering;
}

//PATROL
//******
SteeringOutput Patrol::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	if (m_IsGoingToFirstTarget)
	{
		steering.LinearVelocity = m_Target.Position - pAgent->GetPosition();
		steering.LinearVelocity.Normalize();
		steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();
		if (pAgent->GetPosition().DistanceSquared(m_Target.Position) <= pAgent->GetRadius() * pAgent->GetRadius())
		{
			m_IsGoingToFirstTarget = false;
		}
	}
	else
	{
		steering.LinearVelocity = m_Target2.Position - pAgent->GetPosition();
		steering.LinearVelocity.Normalize();
		steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();
		if (pAgent->GetPosition().DistanceSquared(m_Target2.Position) <= pAgent->GetRadius() * pAgent->GetRadius())
		{
			m_IsGoingToFirstTarget = true;
		}
	}

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0,1,0 });
	}

	return steering;
}
