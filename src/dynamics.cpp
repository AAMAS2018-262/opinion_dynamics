#include "dynamics.h"

Dynamics::Dynamics(DynamicsType dynamics_type, Graph const& graph)
	: type(dynamics_type), graph(graph) {}

void Dynamics::simulateOneRound(Coloring const& current_coloring,
                                Coloring& next_coloring)
{
	switch (type) {
	case DynamicsType::VoterModel:
		executeVoterModel(current_coloring, next_coloring);
		break;
	case DynamicsType::TwoChoices:
		executeTwoChoices(current_coloring, next_coloring);
		break;
	}
}

DynamicsType Dynamics::getType() const
{
	return type;
}

void Dynamics::executeVoterModel(Coloring const& current_coloring,
                                 Coloring& next_coloring)
{
	for (Graph::NodeID node_id = 0; node_id < graph.getNumberOfNodes(); ++node_id) {
		auto neighbor = graph.getRandomNeighbor(node_id, random);
		auto neighbor_color = current_coloring.get(neighbor);

		next_coloring.set(node_id, neighbor_color);
	}
}

void Dynamics::executeTwoChoices(Coloring const& current_coloring,
                                 Coloring& next_coloring)
{
	for (Graph::NodeID node_id = 0; node_id < graph.getNumberOfNodes(); ++node_id) {
		auto neighbor1 = graph.getRandomNeighbor(node_id, random);
		auto neighbor2 = graph.getRandomNeighbor(node_id, random);
		auto neighbor1_color = current_coloring.get(neighbor1);
		auto neighbor2_color = current_coloring.get(neighbor2);

		if (neighbor1_color == neighbor2_color) {
			next_coloring.set(node_id, neighbor1_color);
		}
		else {
			next_coloring.set(node_id, current_coloring.get(node_id));
		}
	}
}
