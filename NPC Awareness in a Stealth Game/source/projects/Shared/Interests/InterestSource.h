#pragma once
class InterestSource
{
public:
	enum class Senses { Sight, Sound };

	InterestSource(const Senses& sense, const int priority,const float radius, const Elite::Vector2 pos, const bool livesForever, const float lifeTime = 0.f);
	~InterestSource();

	void Update(const float dt);

	Elite::EPhysicsCircleShape GetSource() const;

private:
	//--- Datamembers ---
	Senses m_Sense{};
	const int m_Priority{};
	const float m_Radius{};
	const Elite::Vector2 m_Position{};
	const bool m_LivesForever{};
	float m_LifeTime{};
	
};

