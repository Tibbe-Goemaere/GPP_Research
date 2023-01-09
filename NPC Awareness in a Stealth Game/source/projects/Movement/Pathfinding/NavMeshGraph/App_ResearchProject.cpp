//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
using namespace Elite;

//Includes
#include "App_ResearchProject.h"
#include "projects/Shared/NavigationColliderElement.h"

#include "projects/Movement/SteeringBehaviors/SteeringNpcAgent.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"

#include "framework\EliteAI\EliteNavigation\Algorithms\ENavGraphPathfinding.h"

#include "projects/Shared/Interests/InterestRecord.h"
#include "projects/Shared/Interests/InterestSource.h"

//Statics
bool App_ResearchProject::sShowPolygon = true;
bool App_ResearchProject::sShowGraph = false;
bool App_ResearchProject::sDrawPortals = false;
bool App_ResearchProject::sDrawFinalPath = true;
bool App_ResearchProject::sDrawNonOptimisedPath = false;

//Destructor
App_ResearchProject::~App_ResearchProject()
{
	for (auto pNC : m_vNavigationColliders)
		SAFE_DELETE(pNC);
	m_vNavigationColliders.clear();

	SAFE_DELETE(m_pNavGraph);
	SAFE_DELETE(m_pSeekBehavior);
	SAFE_DELETE(m_pArriveBehavior);
	SAFE_DELETE(m_pAgent);
	SAFE_DELETE(m_pPatrolBehavior);
	for (auto npc : m_pNpcAgents)
	{
		SAFE_DELETE(npc);
	}
	m_pNpcAgents.clear();
	SAFE_DELETE(m_pInterestRecord);
	SAFE_DELETE(m_pDeadBody);
	
}

//Functions
void App_ResearchProject::Start()
{
	//Initialization of your application. 
	//----------- CAMERA ------------
	DEBUGRENDERER2D->GetActiveCamera()->SetZoom(36.782f);
	DEBUGRENDERER2D->GetActiveCamera()->SetCenter(Elite::Vector2(12.9361f, 0.2661f));

	//----------- WORLD ------------
	m_vNavigationColliders.push_back(new NavigationColliderElement(Elite::Vector2(25.f, 12.f), 45.0f, 7.0f));
	m_vNavigationColliders.push_back(new NavigationColliderElement(Elite::Vector2(-35.f, 7.f), 14.0f, 10.0f));
	m_vNavigationColliders.push_back(new NavigationColliderElement(Elite::Vector2(-13.f, -8.f), 30.0f, 2.0f));
	m_vNavigationColliders.push_back(new NavigationColliderElement(Elite::Vector2(15.f, -21.f), 50.0f, 3.0f));
	m_vNavigationColliders.push_back(new NavigationColliderElement(Elite::Vector2(70.f, -45.f), 20.f, 3.0f));

	//----------- NAVMESH  ------------
	std::list<Elite::Vector2> baseBox
	{ { -90, 60 },{ -90, -60 },{ 90, -60 },{ 90, 60 } };

	m_pNavGraph = new Elite::NavGraph(Elite::Polygon(baseBox), m_AgentRadius);

	//----------- DeadBody ------------
	m_pDeadBody = new BaseAgent();
	m_pDeadBody->SetBodyColor(Color{0,0,0});
	//----------- InterestRecord ------------
	m_pInterestRecord = new InterestRecord();
	//----------- NPC's ------------
	m_pPatrolBehavior = new Patrol();
	SteeringNpcAgent* agent = new SteeringNpcAgent();
	agent->SetPosition(Vector2{20,20});
	agent->SetVisionCone(VisionCone{ agent->GetRotation() ,agent->GetPosition()});
	m_pPatrolBehavior->SetTargets(Vector2{20,20},Vector2{40,20});
	agent->SetSteeringBehavior(m_pPatrolBehavior);
	agent->SetMaxLinearSpeed(m_NpcSpeed);
	agent->SetAutoOrient(true);
	agent->SetMass(0.1f);
	m_pNpcAgents.push_back(agent);
}

void App_ResearchProject::Update(float deltaTime)
{
	//Add interest source at mouse pos
	if (INPUTMANAGER->IsMouseButtonUp(InputMouseButton::eLeft))
	{
		auto mouseData = INPUTMANAGER->GetMouseData(Elite::InputType::eMouseButton, Elite::InputMouseButton::eMiddle);
		Elite::Vector2 mouseTarget = DEBUGRENDERER2D->GetActiveCamera()->ConvertScreenToWorld(
			Elite::Vector2((float)mouseData.X, (float)mouseData.Y));
		PlaceInterestSource(mouseTarget);
	}
	
	UpdateImGui();

	for (auto npc : m_pNpcAgents)
	{
		npc->Update(deltaTime);
	}
}

void App_ResearchProject::Render(float deltaTime) const
{
	if (sShowGraph)
	{
		m_GraphRenderer.RenderGraph(m_pNavGraph, true, true);
	}

	if (sShowPolygon)
	{
		DEBUGRENDERER2D->DrawPolygon(m_pNavGraph->GetNavMeshPolygon(),
			Color(0.1f, 0.1f, 0.1f));
		DEBUGRENDERER2D->DrawSolidPolygon(m_pNavGraph->GetNavMeshPolygon(),
			Color(0.0f, 0.5f, 0.1f, 0.05f), 0.4f);
	}

	if (sDrawPortals)
	{
		for (const auto &portal : m_Portals)
		{
			DEBUGRENDERER2D->DrawSegment(portal.Line.p1, portal.Line.p2, Color(1.f, .5f, 0.f), -0.1f);
			//Draw just p1 p2
			std::string p1{ "p1" };
			std::string p2{ "p2" };
			//Add the positions to the debugdrawing
			//p1 +=" x:" + std::to_string(portal.Line.p1.x) + ", y: " + std::to_string(portal.Line.p1.y);
			//p2 +=" x:" + std::to_string(portal.Line.p2.x) + ", y: " + std::to_string(portal.Line.p2.y);
			DEBUGRENDERER2D->DrawString(portal.Line.p1, p1.c_str(), Color(1.f, .5f, 0.f), -0.1f);
			DEBUGRENDERER2D->DrawString(portal.Line.p2, p2.c_str(), Color(1.f, .5f, 0.f), -0.1f);

		}
	}

	if (sDrawNonOptimisedPath)
	{
		for (auto pathNode : m_DebugNodePositions)
			DEBUGRENDERER2D->DrawCircle(pathNode, 2.0f, Color(0.f, 0.f, 1.f), -0.45f);
	}

	if (sDrawFinalPath && m_vPath.size() > 0)
	{

		for (auto pathPoint : m_vPath)
			DEBUGRENDERER2D->DrawCircle(pathPoint, 2.0f, Color(1.f, 0.f, 0.f), -0.2f);

		DEBUGRENDERER2D->DrawSegment(m_pAgent->GetPosition(), m_vPath[0], Color(1.f, 0.0f, 0.0f), -0.2f);
		for (size_t i = 0; i < m_vPath.size() - 1; i++)
		{
			float g = float(i) / m_vPath.size();
			DEBUGRENDERER2D->DrawSegment(m_vPath[i], m_vPath[i+1], Color(1.f, g, g), -0.2f);
		}
			
	}

	for (auto npc : m_pNpcAgents)
	{
		npc->Render(deltaTime);
	}

	//std::cout << m_pAgent->GetRotation() << "\n";
}

void App_ResearchProject::UpdateImGui()
{
	//------- UI --------
#ifdef PLATFORM_WINDOWS
#pragma region UI
	{
		//Setup
		int menuWidth = 150;
		int const width = DEBUGRENDERER2D->GetActiveCamera()->GetWidth();
		int const height = DEBUGRENDERER2D->GetActiveCamera()->GetHeight();
		bool windowActive = true;
		ImGui::SetNextWindowPos(ImVec2((float)width - menuWidth - 10, 10));
		ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)height - 90));
		ImGui::Begin("Gameplay Programming", &windowActive, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		ImGui::PushAllowKeyboardFocus(false);
		ImGui::SetWindowFocus();
		ImGui::PushItemWidth(70);
		//Elements
		ImGui::Text("CONTROLS");
		ImGui::Indent();
		ImGui::Unindent();

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("STATS");
		ImGui::Indent();
		ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Unindent();

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Checkbox("Show Polygon", &sShowPolygon);
		ImGui::Checkbox("Show Graph", &sShowGraph);
		ImGui::Checkbox("Show Portals", &sDrawPortals);
		ImGui::Checkbox("Show Path Nodes", &sDrawNonOptimisedPath);
		ImGui::Checkbox("Show Final Path", &sDrawFinalPath);
		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::SliderFloat("AgentSpeed", &m_AgentSpeed, 0.0f, 22.0f))
		{
			m_pAgent->SetMaxLinearSpeed(m_AgentSpeed);
		}

		ImGui::Spacing();
		ImGui::Spacing();
		UpdateInterestsUI();
		//End
		ImGui::PopAllowKeyboardFocus();
		ImGui::End();
	}
#pragma endregion
#endif
}

void App_ResearchProject::UpdateInterestsUI()
{
	//------- UI --------
#ifdef PLATFORM_WINDOWS
#pragma region UI
	{
		switch (m_CurrentInterest)
		{
		case App_ResearchProject::Interests::DeadBody:
			ImGui::Text("DeadBody");
			break;
		default:
			break;
		}
		
		
	}
#pragma endregion
#endif
}

void App_ResearchProject::PlaceInterestSource(const Vector2& pos)
{
	InterestSource* pInterest{};
	switch (m_CurrentInterest)
	{
	case App_ResearchProject::Interests::DeadBody:
		pInterest = new InterestSource(InterestSource::Senses::Sight,7,m_pDeadBody->GetRadius(),pos,true);
		m_pInterestRecord->AddInterestSource(pInterest);
		break;
	default:
		break;
	}
	SAFE_DELETE(pInterest);
}