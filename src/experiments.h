#pragma once

#include "graph.h"
#include "basic_types.h"
#include "simulation.h"

#include <string>

class Experiments
{
public:
	Experiments(std::string const& experiments_file, std::string const& result_files_prefix)
		: experiments_file(experiments_file), result_files_prefix(result_files_prefix) {}
	void run();

private:
	std::string const experiments_file;
	std::string const result_files_prefix;

	using ExperimentID = std::size_t;

	ExperimentsData readExperiments(std::string const& experiments_file);
	void run(ExperimentID id, ExperimentData const& experiment_data);
	void writeInformationToFile(ExperimentID id, ExperimentData const& experiment_data,
	                            Graph const& graph, Coloring const& initial_coloring,
	                            Simulation const& simulation);
	void writeResultToFile(ExperimentID id, ExperimentData const& experiment_data,
	                       Result const& result, std::size_t round);
	void writeSummaryToFile(ExperimentID id, ExperimentData const& experiment_data,
	                        Results const& results);
};
