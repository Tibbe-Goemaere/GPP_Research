#pragma once

class InterestSource;

class InterestRecord
{
public:
	InterestRecord();
	~InterestRecord();

	void Update(float dt);
	void Render(float dt) const;
	void AddInterestSource(const InterestSource& interestSource);
	void RemoveInterest(const InterestSource& interestSource);
	std::list<InterestSource> GetInterestSources() const;
private:
	//Member Variables
	std::list<InterestSource> m_Interests{};
};