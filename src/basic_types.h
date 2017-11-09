#pragma once

#include "defs.h"

#include <array>
#include <string>
#include <vector>

//
// DynamicsType
//

enum class DynamicsType {
	VoterModel,
	TwoChoices,
};
DynamicsType toDynamicsType(std::string const& dynamics_type_string);
std::string toString(DynamicsType dynamics_type);

//
// CPMethod
//

enum class CPMethod {
	KRichClub,
	DensestCore
};
CPMethod toCPMethod(std::string const& cp_method_string);
std::string toString(CPMethod cp_method);

//
// ExperimentData
//

struct ExperimentData
{
	std::string graph_file;
	DynamicsType dynamics_type;
	CPMethod cp_method;
	std::int64_t max_rounds;
	float win_threshold;
	std::size_t number_of_exps;
};
using ExperimentsData = std::vector<ExperimentData>;

//
// Color
//

enum class Color : int_fast8_t {
	Red = 0,
	Blue = 1,
	None = -1
};
std::array<Color, 2> const COLORS = {Color::Red, Color::Blue};

std::string toString(Color color);

//
// Result
//

struct Result
{
	std::string graph_file;
	Color winning_color;
	std::vector<float> color_fractions;
	std::vector<float> color_volumes;
	std::size_t number_of_rounds;
};
using Results = std::vector<Result>;
