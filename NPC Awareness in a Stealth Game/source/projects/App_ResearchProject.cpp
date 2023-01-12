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

//Destructor
App_ResearchProject::~App_ResearchProject()
{
	for (auto pNC : m_vNavigationColliders)
		SAFE_DELETE(pNC);
	m_vNavigationColliders.clear();

	SAFE_DELETE(m_pNavGraph);
	SAFE_DELETE(m_pPatrolBehavior);
	SAFE_DELETE(m_pSeekBehavior);
	SAFE_DELETE(m_pArriveBehavior);
	SAFE_DELETE(m_pTurnBehavior);
	SAFE_DELETE(m_pLookAroundBehavior);
	for (auto npc : m_pNpcAgents)
	{
		SAFE_DELETE(npc);
	}
	m_pNpcAgents.clear();
	for (auto deadBody : m_pDeadBodies)
	{
		SAFE_DELETE(deadBody);
	}
	m_pDeadBodies.clear();
	SAFE_DELETE(m_pInterestRecord);
	
}

//Functions
void App_ResearchProject::Start()
{
	//Initialization of your application. 
	//----------- CAMERA ------------
	DEBUGRENDERER2D->GetActiveCamera()->SetZoom(36.782f);
	DEBUGRENDERER2D->GetActiveCamera()->SetCenter(Elite::Vector2(12.9361f, 0.2661f));

	//----------- WORLD ------------
	m_vNavigationColliders.push_back(new NavigationColliderElement(Elite::Vector2(45.f, 20.f), 1.0f, 30.0f));
	//m_vNavigationColliders.push_back(new NavigationColliderElement(Elite::Vector2(-35.f, 7.f), 14.0f, 10.0f));
	//m_vNavigationColliders.push_back(new NavigationColliderElement(Elite::Vector2(-13.f, -8.f), 30.0f, 2.0f));
	//m_vNavigationColliders.push_back(new NavigationColliderElement(Elite::Vector2(15.f, -21.f), 50.0f, 3.0f));
	//m_vNavigationColliders.push_back(new NavigationColliderElement(Elite::Vector2(70.f, -45.f), 20.f, 3.0f));
	//m_vNavigationColliders.push_back(new NavigationColliderElement(Elite::Vector2(80.f, 10.f), 1.f, 10.f));

	//----------- NAVMESH  ------------
	std::list<Elite::Vector2> baseBox
	{ { 0, 100 },{ 0, 0 },{ 100, 0 },{ 100, 100 } };

	m_pNavGraph = new Elite::NavGraph(Elite::Polygon(baseBox), m_AgentRadius);

	//----------- DeadBody ------------
	m_pDeadBodies = std::vector<BaseAgent*>{};
	//----------- InterestRecord ------------
	m_pInterestRecord = new InterestRecord();
	//----------- NPC's ------------
	m_pPatrolBehavior = new Patrol();
	m_pSeekBehavior = new Seek();
	m_pArriveBehavior = new Arrive();
	m_pTurnBehavior = new Turn();
	m_pLookAroundBehavior = new LookAround();

	SteeringNpcAgent* agent = new SteeringNpcAgent();
	agent->SetPosition(Vector2{20,20});
	agent->SetVisionCone(VisionCone{ agent->GetRotation() ,agent->GetPosition()});
	m_pPatrolBehavior->SetTargets(Vector2{20,20},Vector2{40,20});
	agent->SetSteeringBehavior(m_pPatrolBehavior);
	agent->SetMaxLinearSpeed(m_NpcSpeed);
	agent->SetAutoOrient(true);
	agent->SetMass(0.1f);
	agent->SetPatrolBehavior(m_pPatrolBehavior);
	m_pLookAroundBehavior->SetMaxTime(agent->GetMaxLookAroundTime());
	m_pNpcAgents.push_back(agent);
}

void App_ResearchProject::Update(float deltaTime)
{
	//Add interest source at mouse pos
	if (INPUTMANAGER->IsMouseButtonUp(InputMouseButton::eLeft))
	{
		auto mouseData = INPUTMANAGER->GetMouseData(Elite::InputType::eMouseButton, Elite::InputMouseButton::eLeft);
		Elite::Vector2 mouseTarget = DEBUGRENDERER2D->GetActiveCamera()->ConvertScreenToWorld(
		Elite::Vector2((float)mouseData.X, (float)mouseData.Y));
		PlaceInterestSource(mouseTarget);
	}
	
	//Switch between interest sources
	if (INPUTMANAGER->IsMouseButtonUp(InputMouseButton::eMiddle))
	{
		SwitchCurrentInterest();
	}

	UpdateImGui();

	m_pInterestRecord->Update(deltaTime);

	for (auto npc : m_pNpcAgents)
	{
		if (npc->CheckInterestSources(m_pInterestRecord->GetInterestSources(),m_pNavGraph,m_DebugNodePositions,m_Portals))
		{
			if (npc->GetNextInterestSource().GetType() == InterestSource::Senses::Sight)
			{
				if (npc->GetPosition().Distance(npc->GetNextInterestSource().GetSource().position) <= 6.f)
				{
					npc->SetAutoOrient(false);
					npc->StartLookingAround();
					npc->SetSteeringBehavior(m_pLookAroundBehavior);
				}
				else
				{
					npc->SetAutoOrient(true);
					m_pArriveBehavior->SetTarget(npc->GetNextInterestSource().GetSource().position);
					m_pArriveBehavior->SetArriveDistance(5.f);
					npc->SetSteeringBehavior(m_pArriveBehavior);
					npc->SetStartAngle();
					m_pLookAroundBehavior->Reset();
				}
			}
			else if (npc->GetNextInterestSource().GetType() == InterestSource::Senses::Sound)
			{
				if (npc->GetPosition().Distance(npc->GetNextInterestSource().GetSource().position) <= 1.f)
				{
					npc->SetAutoOrient(false);
					npc->SetSteeringBehavior(m_pTurnBehavior);
				}
				else
				{
					npc->SetAutoOrient(true);
					m_pSeekBehavior->SetTarget(npc->GetNextInterestSource().GetSource().position);
					npc->SetSteeringBehavior(m_pSeekBehavior);
					npc->SetStartAngle();
				}
			}

		}
		else if (npc->FinishedInvestigating() || npc->HasLookedAround()) 
		{
			m_pInterestRecord->RemoveInterest(npc->GetNextInterestSource());
			npc->SetAutoOrient(true);
			npc->SetSteeringBehavior(npc->GetPatrolBehavior());
		}
		npc->Update(deltaTime);
	}

	for (auto deadBody : m_pDeadBodies)
	{
		deadBody->Update(deltaTime);
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

	for (auto npc : m_pNpcAgents)
	{
		npc->Render(deltaTime);
	}

	m_pInterestRecord->Render(deltaTime);
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
		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::SliderFloat("NpcSpeed", &m_NpcSpeed, 0.0f, 22.0f))
		{
			for (auto npc : m_pNpcAgents)
			{
				npc->SetMaxLinearSpeed(m_NpcSpeed);
			}
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
		case App_ResearchProject::Interests::QuitSound:
			ImGui::Text("QuitSound");
			break;
		case App_ResearchProject::Interests::LoudSound:
			ImGui::Text("LoudSound");
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
	BaseAgent* pDeadBody{};
	const float smallRadius{ 10.f };
	const float bigRadius{ 15.f };

	switch (m_CurrentInterest)
	{
	case App_ResearchProject::Interests::DeadBody:
		m_pInterestRecord->AddInterestSource(InterestSource(InterestSource::Senses::Sight, 7, m_AgentRadius, pos, { 0,0,0 }, true));
		break;
	case App_ResearchProject::Interests::QuitSound:
		m_pInterestRecord->AddInterestSource(InterestSource(InterestSource::Senses::Sound, 4, smallRadius, pos,{1,0,0}, false, 5.f));
		break;
	case App_ResearchProject::Interests::LoudSound:
		m_pInterestRecord->AddInterestSource(InterestSource(InterestSource::Senses::Sound, 5, bigRadius, pos, { 1,0,0 }, false, 10.f));
		break;
	default:
		break;
	}
}

void App_ResearchProject::SwitchCurrentInterest()
{
	switch (m_CurrentInterest)
	{
	case App_ResearchProject::Interests::DeadBody:
		m_CurrentInterest = Interests::QuitSound;
		break;
	case App_ResearchProject::Interests::QuitSound:
		m_CurrentInterest = Interests::LoudSound;
		break;
	case App_ResearchProject::Interests::LoudSound:
		m_CurrentInterest = Interests::DeadBody;
		break;
	default:
		break;
	}
}
