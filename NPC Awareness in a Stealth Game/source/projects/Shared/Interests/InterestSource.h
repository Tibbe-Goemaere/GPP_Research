#pragma once
class InterestSource
{
public:
	enum class Senses { Sight, Sound };

	InterestSource(const Senses& sense, const int priority,const float radius, const Elite::Vector2 pos, const bool livesForever, const float lifeTime = 0.f);
	~InterestSource();

	
private:
	//C++ make the class non-copyable
	InterestSource(const InterestSource&) = delete;
	InterestSource& operator=(const InterestSource&) = delete;

};

