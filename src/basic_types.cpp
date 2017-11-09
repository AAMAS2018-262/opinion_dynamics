#include "basic_types.h"

//
// DynamicsType
//

DynamicsType toDynamicsType(std::string const& dynamics_type_string)
{
	if (dynamics_type_string == "VoterModel") {
		return DynamicsType::VoterModel;
	}
	else if (dynamics_type_string == "TwoChoices") {
		return DynamicsType::TwoChoices;
	}

	Error("No matching dynamics type on call of toDynamicsType");
}

std::string toString(DynamicsType dynamics_type)
{
	switch (dynamics_type) {
	case DynamicsType::VoterModel: return "VoterModel";
	case DynamicsType::TwoChoices: default: return "TwoChoices";
	}
}

//
// CPMethod
//

CPMethod toCPMethod(std::string const& cp_method_string)
{
	if (cp_method_string == "KRichClub") {
		return CPMethod::KRichClub;
	}
	else if (cp_method_string == "DensestCore") {
		return CPMethod::DensestCore;
	}

	Error("No matching core extraction method on call of toCPMethod");
}

std::string toString(CPMethod cp_method)
{
	switch (cp_method) {
	case CPMethod::KRichClub: return "KRichClub";
	case CPMethod::DensestCore: default: return "DensestCore";
	}
}

//
// Color
//

std::string toString(Color color)
{
	switch (color) {
	case Color::Red: return "red";
	case Color::Blue: return "blue";
	case Color::None: default: return "none";
	}
}
