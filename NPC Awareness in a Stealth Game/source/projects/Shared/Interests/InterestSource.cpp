#include "stdafx.h"
#include "InterestSource.h"
#include "framework/EliteMath/EMath.h"

InterestSource::InterestSource(const Senses& sense, const int priority, const float radius, const Elite::Vector2 pos, const bool livesForever, const float lifeTime)
	:m_Sense{sense}
	,m_Priority{priority}
	,m_Radius{radius}
	,m_Position{pos}
	,m_LivesForever{livesForever}
	,m_LifeTime{lifeTime}
{
}

InterestSource::~InterestSource()
{
}

InterestSource::InterestSource(const InterestSource& other)
	:InterestSource(other.m_Sense,other.m_Priority,other.m_Radius,other.m_Position,other.m_LivesForever,other.m_LifeTime)
{
	
}

InterestSource& InterestSource::operator=(const InterestSource& other)
{
	m_Sense = other.m_Sense;
	m_Priority = other.m_Priority;
	m_Radius = other.m_Radius;
	m_Position = other.m_Position;
	m_LivesForever = other.m_LivesForever;
	m_LifeTime = other.m_LifeTime;
	return *this;
}

void InterestSource::Update(const float dt)
{
	if (!m_LivesForever)
	{
		m_LifeTime -= dt;
	}
}

Elite::EPhysicsCircleShape InterestSource::GetSource() const
{
	Elite::EPhysicsCircleShape source{ m_Position,m_Radius };
	return source;
}

int InterestSource::GetPriority() const
{
	return m_Priority;
}


