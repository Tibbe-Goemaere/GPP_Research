#pragma once
class App_ResearchProject;

class InterestSource
{
public:
	enum class Senses { Sight, Sound };

	InterestSource(const Senses& sense = Senses::Sight, const int priority = 0,const float radius = 0, const Elite::Vector2 pos = {}, const Elite::Color color = {}, const bool livesForever = true, const float lifeTime = 0.f);
	~InterestSource();
	//C++ make the class copyable
	InterestSource(const InterestSource& other);
	InterestSource& operator=(const InterestSource& other);
	bool operator ==(const InterestSource& interestSource) const;
	bool operator !=(const InterestSource& interestSource) const;

	void Update(float dt);

	Elite::EPhysicsCircleShape GetSource() const;
	int GetPriority() const;
	float GetRadius() const;
	Senses GetType() const;
	float GetLifetime() const;
	bool LivesForever() const;
	Elite::Color GetColor() const;
	bool HasBeenInvestigated() const;
	void IsInvestigated();
	
private:
	//--- Datamembers ---
	Senses m_Sense{};
	int m_Priority{};
	float m_Radius{};
	Elite::Vector2 m_Position{};
	bool m_LivesForever{};
	float m_LifeTime{};
	Elite::Color m_Color{};
	bool m_HasBeenInvestigated{};
};

