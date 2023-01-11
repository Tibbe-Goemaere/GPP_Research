#include "stdafx.h"
#include "InterestRecord.h"
#include "InterestSource.h"

InterestRecord::InterestRecord()
	:m_Interests{}
{
}

InterestRecord::~InterestRecord()
{
	
}

void InterestRecord::Update(float dt)
{
	for (InterestSource& interest: m_Interests)
	{
		interest.Update(dt);
		if ((interest.GetLifetime() <= 0 && !interest.LivesForever()) || interest.HasBeenInvestigated())
		{
			m_Interests.remove(interest);
			break;
		}
	}
}

void InterestRecord::Render(float dt) const
{
	for (const InterestSource& interest : m_Interests)
	{
		const Elite::Vector2 pos{ interest.GetSource().position };
		const float radius{ interest.GetSource().radius };
		const Elite::Color color{ interest.GetColor() };
		switch (interest.GetType())
		{
		case::InterestSource::Senses::Sight:
			DEBUGRENDERER2D->DrawCircle(pos,radius,color,0);
			DEBUGRENDERER2D->DrawSegment({pos.x + radius,pos.y}, { pos.x - radius,pos.y }, color);
			DEBUGRENDERER2D->DrawSegment({ pos.x,pos.y + radius}, { pos.x,pos.y - radius }, color);
		break;
		case::InterestSource::Senses::Sound:
			DEBUGRENDERER2D->DrawCircle(pos, radius, color, 0);
		break;
		default:
			break;
		}
	}
}

void InterestRecord::AddInterestSource(const InterestSource& interestSource)
{
	m_Interests.push_back(interestSource);
}

void InterestRecord::RemoveInterest(const InterestSource& interestSource)
{
	m_Interests.remove(interestSource);
}

std::list<InterestSource> InterestRecord::GetInterestSources() const
{
	return m_Interests;
}
