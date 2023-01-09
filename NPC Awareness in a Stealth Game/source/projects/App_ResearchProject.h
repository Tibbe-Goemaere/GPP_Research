#ifndef INFLUENCE_MAP_APPLICATION_H
#define INFLUENCE_MAP_APPLICATION_H
//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "framework/EliteInterfaces/EIApp.h"
#include "projects/Movement/SteeringBehaviors/SteeringHelpers.h"

#include "framework\EliteAI\EliteGraphs\EliteGraphUtilities\EGraphRenderer.h"
#include "framework\EliteAI\EliteNavigation\Algorithms\EPathSmoothing.h"

class NavigationColliderElement;
class BaseAgent;
class SteeringAgent;
class SteeringNpcAgent;
class Seek;
class Arrive;
class Patrol;
class InterestRecord;
class InterestSource;

namespace Elite
{
	class NavGraph;
}
//-----------------------------------------------------------------
// Application
//-----------------------------------------------------------------
class App_ResearchProject final : public IApp
{
public:
	//Constructor & Destructor
	App_ResearchProject() = default;
	virtual ~App_ResearchProject();

	//App Functions
	void Start() override;
	void Update(float deltaTime) override;
	void Render(float deltaTime) const override;

private:
	//Datamembers
	// --Agents--
	std::vector<BaseAgent*> m_pDeadBodies;
	Patrol* m_pPatrolBehavior = nullptr;
	Seek* m_pSeekBehavior = nullptr;
	Arrive* m_pArriveBehavior = nullptr;
	TargetData m_Target = {};
	float m_AgentRadius = 1.0f;

	std::vector<SteeringNpcAgent*> m_pNpcAgents{};
	float m_NpcSpeed = 8.0f;
	// --Level--
	std::vector<NavigationColliderElement*> m_vNavigationColliders = {};

	// --Pathfinder--
	std::vector<Elite::Vector2> m_vPath;

	// --Graph--
	Elite::NavGraph* m_pNavGraph = nullptr;
	Elite::GraphRenderer m_GraphRenderer{};

	// --Debug drawing information--
	std::vector<Elite::Portal> m_Portals;
	std::vector<Elite::Vector2> m_DebugNodePositions;
	static bool sShowPolygon;
	static bool sShowGraph;

	void UpdateImGui();

	// --InterestSources-- //
	enum class Interests{DeadBody};
	Interests m_CurrentInterest{};
	void UpdateInterestsUI();
	void PlaceInterestSource(const Elite::Vector2& pos);
	InterestRecord* m_pInterestRecord{};

private:
	//C++ make the class non-copyable
	App_ResearchProject(const App_ResearchProject&) = delete;
	App_ResearchProject& operator=(const App_ResearchProject&) = delete;

};
#endif