#pragma once
class InterestSource
{
public:
	enum class Senses { Sight, Sound };

	InterestSource(const Senses& sense, const int priority,const float radius, const Elite::Vector2 pos, const bool livesForever, const float lifeTime = 0.f);
	~InterestSource();
	//C++ make the class copyable
	InterestSource(const InterestSource& other);
	InterestSource& operator=(const InterestSource& other);

	void Update(const float dt);

	Elite::EPhysicsCircleShape GetSource() const;
	int GetPriority() const;

private:
	//--- Datamembers ---
	Senses m_Sense{};
	int m_Priority{};
	float m_Radius{};
	Elite::Vector2 m_Position{};
	bool m_LivesForever{};
	float m_LifeTime{};
	
};

