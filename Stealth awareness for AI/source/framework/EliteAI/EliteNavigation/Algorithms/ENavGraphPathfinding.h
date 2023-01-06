#pragma once
#include <vector>
#include <iostream>
#include "framework/EliteMath/EMath.h"
#include "framework\EliteAI\EliteGraphs\ENavGraph.h"
#include "framework\EliteAI\EliteGraphs\EliteGraphAlgorithms\EAStar.h"

namespace Elite
{
	class NavMeshPathfinding
	{
	public:
		static std::vector<Vector2> FindPath(Vector2 startPos, Vector2 endPos, NavGraph* pNavGraph, std::vector<Vector2>& debugNodePositions, std::vector<Portal>& debugPortals)
		{
			//Create the path to return
			std::vector<Vector2> finalPath{};

			//Get the start and endTriangle
			const Triangle* startTriangle = pNavGraph->GetNavMeshPolygon()->GetTriangleFromPosition(startPos);
			const Triangle* endTriangle = pNavGraph->GetNavMeshPolygon()->GetTriangleFromPosition(endPos);
			//We have valid start/end triangles and they are not the same
			if (startTriangle == nullptr || endTriangle == nullptr)
			{
				return finalPath;
			}

			if (startTriangle == endTriangle)
			{
				finalPath.push_back(endPos);
				return finalPath;
			}
			//=> Start looking for a path
			//Copy the graph
			auto graphClone = pNavGraph->Clone();
			IGraph<NavGraphNode,GraphConnection2D>* graphCopy = graphClone.get();
			//Create extra node for the Start Node (Agent's position)
			NavGraphNode* startNode{};
			startNode = new NavGraphNode(graphCopy->GetNextFreeNodeIndex(), -1, startPos);
			graphCopy->AddNode(startNode);
			for (int edgeIdx : startTriangle->metaData.IndexLines)
			{
				const int idx{ pNavGraph->GetNodeIdxFromLineIdx(edgeIdx) };
				if (idx != invalid_node_index)
				{
					GraphConnection2D* connection = new GraphConnection2D{ idx, startNode->GetIndex(),Distance(startPos,graphCopy->GetNode(idx)->GetPosition())};
					graphCopy->AddConnection(connection);
				}
			}
			
			//Create extra node for the endNode
			NavGraphNode* endNode{};
			endNode = new NavGraphNode(graphCopy->GetNextFreeNodeIndex(), -1, endPos);
			graphCopy->AddNode(endNode);
			for (int edgeIdx : endTriangle->metaData.IndexLines)
			{
				const int idx{ pNavGraph->GetNodeIdxFromLineIdx(edgeIdx) };
				if (idx != invalid_node_index)
				{
					GraphConnection2D* connection = new GraphConnection2D{ idx, endNode->GetIndex(),Distance(endPos,graphCopy->GetNode(idx)->GetPosition()) };
					graphCopy->AddConnection(connection);
				}

			}
			
			//Run A star on new graph
			auto pathfinder = AStar<NavGraphNode, GraphConnection2D>(graphCopy,HeuristicFunctions::Chebyshev);
			std::vector<NavGraphNode*> path;
			path = pathfinder.FindPath(startNode, endNode);
			for (auto node : path)
			{
				finalPath.push_back(node->GetPosition());
			}
			//OPTIONAL BUT ADVICED: Debug Visualisation
			
			//Run optimiser on new graph, MAKE SURE the A star path is working properly before starting this section and uncommenting this!!!
			debugPortals = SSFA::FindPortals(path, pNavGraph->GetNavMeshPolygon());
			finalPath = SSFA::OptimizePortals(debugPortals);

			return finalPath;
		}
	};
}
