#include "stdafx.h"
#include "ENavGraph.h"
#include "framework\EliteAI\EliteGraphs\EliteGraphAlgorithms\EAStar.h"

using namespace Elite;

Elite::NavGraph::NavGraph(const Polygon& contourMesh, float playerRadius = 1.0f) :
	Graph2D(false),
	m_pNavMeshPolygon(nullptr)
{
	//Create the navigation mesh (polygon of navigatable area= Contour - Static Shapes)
	m_pNavMeshPolygon = new Polygon(contourMesh); // Create copy on heap

	//Get all shapes from all static rigidbodies with NavigationCollider flag
	auto vShapes = PHYSICSWORLD->GetAllStaticShapesInWorld(PhysicsFlags::NavigationCollider);

	//Store all children
	for (auto shape : vShapes)
	{
		shape.ExpandShape(playerRadius);
		m_pNavMeshPolygon->AddChild(shape);
	}

	//Triangulate
	m_pNavMeshPolygon->Triangulate();

	//Create the actual graph (nodes & connections) from the navigation mesh
	CreateNavigationGraph();
}

Elite::NavGraph::~NavGraph()
{
	delete m_pNavMeshPolygon; 
	m_pNavMeshPolygon = nullptr;
}

int Elite::NavGraph::GetNodeIdxFromLineIdx(int lineIdx) const
{
	auto nodeIt = std::find_if(m_Nodes.begin(), m_Nodes.end(), [lineIdx](const NavGraphNode* n) { return n->GetLineIndex() == lineIdx; });
	if (nodeIt != m_Nodes.end())
	{
		return (*nodeIt)->GetIndex();
	}

	return invalid_node_index;
}

Elite::Polygon* Elite::NavGraph::GetNavMeshPolygon() const
{
	return m_pNavMeshPolygon;
}

void Elite::NavGraph::CreateNavigationGraph()
{
	//1. Go over all the edges of the navigationmesh and create nodes
	for (auto& lines : m_pNavMeshPolygon->GetLines())
	{
		if (m_pNavMeshPolygon->GetTrianglesFromLineIndex(lines->index).size() > 1)
		{
			const Vector2 middle{ (lines->p1 + lines->p2) / 2.f };
			NavGraphNode* newNode{};
			newNode = new NavGraphNode( GetNextFreeNodeIndex(), lines->index, middle);
			AddNode(newNode);
		}
	}
	
	std::vector<int> idx{};
	for (auto& triangle : m_pNavMeshPolygon->GetTriangles())
	{
		for (auto& linesIdx : triangle->metaData.IndexLines)
		{
			if (GetNodeIdxFromLineIdx(linesIdx) != invalid_node_index)
			{
				idx.push_back(GetNodeIdxFromLineIdx(linesIdx));
			}
		}
		//2. Create connections now that every node is created
		if (idx.size() == 2)
		{
			GraphConnection2D* connection = new GraphConnection2D{ idx[0], idx[1] };
			AddConnection(connection);
		}
		else if (idx.size() == 3)
		{
			GraphConnection2D* connection = new GraphConnection2D{ idx[0], idx[1] };
			AddConnection(connection);
			connection = new GraphConnection2D{ idx[1], idx[2] };
			AddConnection(connection);
			connection = new GraphConnection2D{ idx[2], idx[0] };
			AddConnection(connection);
		}

		idx.clear();
	}

	//3. Set the connections cost to the actual distance
	SetConnectionCostsToDistance();
	
	
	
}

