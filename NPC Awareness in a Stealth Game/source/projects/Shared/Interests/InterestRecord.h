#pragma once

class InterestSource;

class InterestRecord
{
public:
	InterestRecord();
	~InterestRecord();

	void AddInterestSource(const InterestSource& interestSource);
	std::list<InterestSource*> GetInterestSources() const;
private:
	//Member Variables
	std::list<InterestSource*> m_pInterests{};
};

