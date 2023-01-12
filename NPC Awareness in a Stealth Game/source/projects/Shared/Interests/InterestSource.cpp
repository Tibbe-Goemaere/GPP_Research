#include "stdafx.h"
#include "InterestSource.h"
#include "framework/EliteMath/EMath.h"

InterestSource::InterestSource(const Senses& sense, const int priority, const float radius, const Elite::Vector2 pos, const Elite::Color color, const bool livesForever, const float lifeTime)
	:m_Sense{ sense }
	, m_Priority{ priority }
	, m_Radius{ radius }
	, m_Position{ pos }
	, m_LivesForever{ livesForever }
	, m_LifeTime{ lifeTime }
	, m_Color{ color }
	, m_HasBeenInvestigated{ false }
{
}

InterestSource::~InterestSource()
{
}

InterestSource::InterestSource(const InterestSource& other)
	:InterestSource(other.m_Sense, other.m_Priority, other.m_Radius, other.m_Position, other.m_Color, other.m_LivesForever, other.m_LifeTime)
{

}

InterestSource& InterestSource::operator=(const InterestSource& other)
{
	m_Sense = other.m_Sense;
	m_Priority = other.m_Priority;
	m_Radius = other.m_Radius;
	m_Position = other.m_Position;
	m_Color = other.m_Color;
	m_LivesForever = other.m_LivesForever;
	m_LifeTime = other.m_LifeTime;
	return *this;
}

bool InterestSource::operator==(const InterestSource& interestSource) const
{
	return m_Sense == interestSource.m_Sense &&
		m_Priority == interestSource.m_Priority &&
		m_Radius == interestSource.m_Radius &&
		m_Position == interestSource.m_Position &&
		m_LivesForever == interestSource.m_LivesForever &&
		m_LifeTime == interestSource.m_LifeTime;
}

bool InterestSource::operator!=(const InterestSource& interestSource) const
{
	return !(operator==(interestSource));
}

void InterestSource::Update(float dt)
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

float InterestSource::GetRadius() const
{
	return m_Radius;
}

InterestSource::Senses InterestSource::GetType() const
{
	return m_Sense;
}

float InterestSource::GetLifetime() const
{
	return m_LifeTime;
}

bool InterestSource::LivesForever() const
{
	return m_LivesForever;
}

Elite::Color InterestSource::GetColor() const
{
	return m_Color;
}

bool InterestSource::HasBeenInvestigated() const
{
	return m_HasBeenInvestigated;
}

void InterestSource::IsInvestigated()
{
	m_HasBeenInvestigated = true;
}