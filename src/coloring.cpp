#include "coloring.h"

#include "defs.h"

Coloring::Coloring(std::size_t size)
	: Coloring(size, Color::Red)
{

}

Coloring::Coloring(std::size_t size, Color color)
	: colors(size, color), color_counts(COLORS.size(), 0)
{
	color_counts[static_cast<std::size_t>(color)] = colors.size();
}

Color Coloring::get(std::size_t index) const
{
	return colors[index];
}

void Coloring::set(std::size_t index, Color new_color)
{
	auto old_color = colors[index];
	colors[index] = new_color;

	--color_counts[static_cast<std::size_t>(old_color)];
	++color_counts[static_cast<std::size_t>(new_color)];
}

void Coloring::assign(Coloring const& coloring)
{
	debug_assert(size() == coloring.size());

	auto const& other_colors = coloring.colors;
	auto const& other_counts = coloring.color_counts;

	colors.assign(other_colors.begin(), other_colors.end());
	color_counts.assign(other_counts.begin(), other_counts.end());
}

void Coloring::swap(Coloring& coloring)
{
	colors.swap(coloring.colors);
	color_counts.swap(coloring.color_counts);
}

bool Coloring::isUnimodal() const
{
	bool seen_non_zero_color = false;
	for (auto count: color_counts) {
		if (count > 0) {
			if (seen_non_zero_color) {
				return false;
			}

			seen_non_zero_color = true;
		}
	}

	return true;
}

Color Coloring::getWinningColor() const
{
	if (colors.empty()) { return Color::None; }
	if (isUnimodal()) { return colors[0]; }
	return Color::None;
}

std::vector<float> Coloring::getColorFractions() const
{
	std::vector<float> fractions(color_counts.size());
	for (std::size_t i = 0; i < fractions.size(); ++i) {
		fractions[i] = (float)color_counts[i]/colors.size();
	}

	return fractions;
}
