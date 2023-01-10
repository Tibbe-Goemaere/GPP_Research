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

void InterestRecord::AddInterestSource(const InterestSource& interestSource)
{
	m_Interests.push_back(interestSource);
}

std::list<InterestSource> InterestRecord::GetInterestSources() const
{
	return m_Interests;
}
