#pragma once

class InterestSource;

class InterestRecord
{
public:
	InterestRecord();
	~InterestRecord();

	void AddInterestSource(InterestSource* pInterest);
private:
	//C++ make the class non-copyable
	InterestRecord(const InterestRecord&) = delete;
	InterestRecord& operator=(const InterestRecord&) = delete;

	//Member Variables
	std::list<InterestSource*> m_pInterests{};
};

