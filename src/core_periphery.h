#pragma once

#include "graph.h"
#include "coloring.h"

Coloring calculateCorePeripheryColoring(Graph const& graph, CPMethod method);

std::pair<float, float> calcDominanceAndRobustness(Graph const& graph, Coloring const& coloring);
