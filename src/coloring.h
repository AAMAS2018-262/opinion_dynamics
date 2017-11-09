#pragma once

#include "basic_types.h"

#include <vector>

class Coloring
{
public:
	Coloring(std::size_t size);
	Coloring(std::size_t size, Color color);

	std::size_t size() const { return colors.size(); }

	Color get(std::size_t index) const;
	void set(std::size_t index, Color new_color);
	void assign(Coloring const& coloring);
	void swap(Coloring& coloring);
	bool isUnimodal() const;

	Color getWinningColor() const;
	std::vector<float> getColorFractions() const;
	std::vector<float> getColorVolumes() const;

private:
	std::vector<Color> colors;
	std::vector<std::size_t> color_counts;
};
