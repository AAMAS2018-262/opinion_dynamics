#pragma once

#include "coloring.h"
#include "graph.h"
#include "random.h"

class Dynamics
{
public:
	Dynamics(DynamicsType dynamics_type, Graph const& graph);

	void simulateOneRound(Coloring const& current_coloring,
	                      Coloring& next_coloring);
	DynamicsType getType() const;

private:
	DynamicsType const type;
	Graph const& graph;
	Random random;

	void executeVoterModel(Coloring const& current_coloring,
	                       Coloring& next_coloring);
	void executeTwoChoices(Coloring const& current_coloring,
                           Coloring& next_coloring);
};
