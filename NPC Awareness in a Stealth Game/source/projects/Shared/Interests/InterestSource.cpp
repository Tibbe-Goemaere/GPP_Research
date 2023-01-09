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


