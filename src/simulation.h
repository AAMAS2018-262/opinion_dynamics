#pragma once

#include "dynamics.h"
#include "graph.h"
#include "basic_types.h"

#include <cstdint>

class Simulation
{
public:
	Simulation (Graph const& graph, DynamicsType dynamics_type, Coloring initial_coloring);
	Result run(std::int64_t max_rounds, float win_threshold);

	float getLargestVolumeFraction() const;
	Color getWinningColor(float win_threshold) const;
	std::vector<float> getColorVolumes() const;

private:
	Graph const& graph;
	Dynamics dynamics;
	Coloring const initial_coloring;

	Coloring current_coloring;
	Coloring next_coloring;
	std::size_t max_rounds;

	void clear();
};
