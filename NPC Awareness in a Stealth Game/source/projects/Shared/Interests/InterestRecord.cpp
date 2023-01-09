#include "stdafx.h"
#include "InterestRecord.h"
#include "InterestSource.h"

InterestRecord::InterestRecord()
	:m_pInterests{}
{
}

InterestRecord::~InterestRecord()
{
	for (auto interestSource : m_pInterests)
	{
		SAFE_DELETE(interestSource);
	}
}

void InterestRecord::AddInterestSource(const InterestSource& interestSource)
{
	InterestSource* pInterestSource{};
	pInterestSource = new InterestSource(interestSource);
	m_pInterests.push_back(pInterestSource);
}

std::list<InterestSource*> InterestRecord::GetInterestSources() const
{
	return m_pInterests;
}
