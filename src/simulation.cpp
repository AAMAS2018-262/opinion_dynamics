#include "simulation.h"

#include "defs.h"

#include <algorithm>

Simulation::Simulation(Graph const& graph, DynamicsType dynamics_type,
                       Coloring initial_coloring)
	: graph(graph), dynamics(dynamics_type, graph), initial_coloring(initial_coloring),
	current_coloring(graph.getNumberOfNodes()), next_coloring(graph.getNumberOfNodes())
{
	debug_assert(initial_coloring.size() == graph.getNumberOfNodes());

	clear();
}

Result Simulation::run(std::int64_t max_rounds, float win_threshold)
{
	clear();
	max_rounds = (max_rounds == -1 ? graph.getNumberOfNodes() : max_rounds);

	// run simulation
	std::size_t round = 0;
	while (round < (std::size_t)max_rounds &&
		   getLargestVolumeFraction() < win_threshold) {

		dynamics.simulateOneRound(current_coloring, next_coloring);
		current_coloring.swap(next_coloring);
		++round;
	}

	debug_assert(current_coloring.size() > 0);
	return Result{
		graph.getFilename(),
		getWinningColor(win_threshold),
		current_coloring.getColorFractions(),
		getColorVolumes(),
		round
	};
}

float Simulation::getLargestVolumeFraction() const
{
	auto volumes = getColorVolumes();
	return *std::max_element(volumes.begin(), volumes.end());
}

Color Simulation::getWinningColor(float win_threshold) const
{
	auto volumes = getColorVolumes();

	for (auto color: COLORS) {
		std::size_t color_index = static_cast<std::size_t>(color);
		if (volumes[color_index] >= win_threshold) {
			return color;
		}
	}

	return Color::None;
}

std::vector<float> Simulation::getColorVolumes() const
{

	// count
	std::vector<std::size_t> counts(COLORS.size());
	for (Graph::NodeID node_id = 0; node_id < graph.getNumberOfNodes(); ++node_id) {
		auto color = current_coloring.get(node_id);
		auto color_index = static_cast<std::size_t>(color);
		counts[color_index] += graph.degree(node_id);
	}

	// normalize
	std::vector<float> volume(COLORS.size());
	for (std::size_t i = 0; i < volume.size(); ++i) {
		volume[i] = (float)counts[i]/graph.getNumberOfEdges();
	}

	return volume;
}

void Simulation::clear()
{
	current_coloring.assign(initial_coloring);
}
