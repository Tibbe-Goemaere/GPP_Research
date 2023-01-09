#include "stdafx.h"
#include "InterestRecord.h"
#include "InterestSource.h"

InterestRecord::InterestRecord()
	:m_pInterests{}
{
}

InterestRecord::~InterestRecord()
{
}

void InterestRecord::AddInterestSource(InterestSource* pInterest)
{
	m_pInterests.push_back(pInterest);
}
