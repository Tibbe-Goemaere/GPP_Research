#pragma once
#include "framework/EliteAI/EliteNavigation/ENavigation.h"

namespace Elite
{
	template <class T_NodeType, class T_ConnectionType>
	class AStar
	{
	public:
		AStar(IGraph<T_NodeType, T_ConnectionType>* pGraph, Heuristic hFunction);

		// stores the optimal connection to a node and its total costs related to the start and end node of the path
		struct NodeRecord
		{
			T_NodeType* pNode = nullptr;
			T_ConnectionType* pConnection = nullptr;
			float costSoFar = 0.f; // accumulated g-costs of all the connections leading up to this one
			float estimatedTotalCost = 0.f; // f-cost (= costSoFar + h-cost)

			bool operator==(const NodeRecord& other) const
			{
				return pNode == other.pNode
					&& pConnection == other.pConnection
					&& costSoFar == other.costSoFar
					&& estimatedTotalCost == other.estimatedTotalCost;
			};

			bool operator<(const NodeRecord& other) const
			{
				return estimatedTotalCost < other.estimatedTotalCost;
			};
		};

		std::vector<T_NodeType*> FindPath(T_NodeType* pStartNode, T_NodeType* pDestinationNode);

	private:
		float GetHeuristicCost(T_NodeType* pStartNode, T_NodeType* pEndNode) const;

		IGraph<T_NodeType, T_ConnectionType>* m_pGraph;
		Heuristic m_HeuristicFunction;
	};

	template <class T_NodeType, class T_ConnectionType>
	AStar<T_NodeType, T_ConnectionType>::AStar(IGraph<T_NodeType, T_ConnectionType>* pGraph, Heuristic hFunction)
		: m_pGraph(pGraph)
		, m_HeuristicFunction(hFunction)
	{
	}

	template <class T_NodeType, class T_ConnectionType>
	std::vector<T_NodeType*> AStar<T_NodeType, T_ConnectionType>::FindPath(T_NodeType* pStartNode, T_NodeType* pGoalNode)
	{
		std::vector<T_NodeType*> path;
		std::vector<NodeRecord> openList;
		std::vector<NodeRecord> closedList;
		NodeRecord currentRecord;

		currentRecord.pNode = pStartNode;
		currentRecord.pConnection = nullptr;
		currentRecord.estimatedTotalCost = GetHeuristicCost(pStartNode, pGoalNode);
		openList.push_back(currentRecord);

		while (openList.empty() == false)
		{
			currentRecord = *std::min_element(openList.begin(), openList.end());

			if (currentRecord.pNode == pGoalNode)
				break;

			for (auto& connection : m_pGraph->GetNodeConnections(currentRecord.pNode))
			{
				NodeRecord nextRecord{};
				nextRecord.pNode = m_pGraph->GetNode(connection->GetTo());
				nextRecord.costSoFar = currentRecord.costSoFar + connection->GetCost();
				nextRecord.pConnection = connection;
				nextRecord.estimatedTotalCost = nextRecord.costSoFar + GetHeuristicCost(nextRecord.pNode, pGoalNode);
				
				bool willContinue{false};

				for (auto& element : closedList)
				{
					if (element.pNode == nextRecord.pNode)
					{
						if (element.costSoFar <= nextRecord.costSoFar)
						{
							willContinue = true;
							break;
						}
						closedList.erase(std::remove(closedList.begin(), closedList.end(), element));
					}
				}
				if (willContinue) { continue; }

				for (auto& element : openList)
				{
					if (element.pNode == nextRecord.pNode)
					{
						if (element.costSoFar <= nextRecord.costSoFar)
						{
							willContinue = true;
							break;
						}
						openList.erase(std::remove(openList.begin(), openList.end(), element));
					}
				}
				if (willContinue) { continue; }

				openList.push_back(nextRecord);
			}

			closedList.push_back(currentRecord);
			openList.erase(std::remove(openList.begin(), openList.end(), currentRecord));
		}

		while (currentRecord.pNode != pStartNode)
		{
			path.push_back(currentRecord.pNode);
			for (auto& element : closedList)
			{
				if (element.pNode == m_pGraph->GetNode(currentRecord.pConnection->GetFrom()))
				{
					currentRecord = element;
					break;
				}
			}
		}
		path.push_back(pStartNode);
		std::reverse(path.begin(), path.end());
		return path;
	}

	template <class T_NodeType, class T_ConnectionType>
	float Elite::AStar<T_NodeType, T_ConnectionType>::GetHeuristicCost(T_NodeType* pStartNode, T_NodeType* pEndNode) const
	{
		Vector2 toDestination = m_pGraph->GetNodePos(pEndNode) - m_pGraph->GetNodePos(pStartNode);
		return m_HeuristicFunction(abs(toDestination.x), abs(toDestination.y));
	}
}