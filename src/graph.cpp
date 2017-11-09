#include "graph.h"

#include "defs.h"
#include "union_find.h"

#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <sstream>

void Graph::buildFromFile(std::string const& graph_file)
{
	filename = graph_file;

	auto parser_edges = readEdges(graph_file);
	reduceToLargestScc(parser_edges);
	auto edges = convertIDs(parser_edges);
	addAllReverseEdges(edges);
	sortAndMakeUnique(edges);
	fillOffsetsAndNeighbors(edges);
}

std::string const& Graph::getFilename() const
{
	return filename;
}

auto Graph::readEdges(std::string const& graph_file) const -> ParserEdges
{
	ParserEdges parser_edges;

	std::ifstream file(graph_file);
	if (!file.is_open()) {
		Error("The graph file couldn't be opened");
	}

	std::string line;
	ParserNodeID source, target;
	while (std::getline(file, line)) {
		if (line.empty() || line[0] == '#') {
			continue;
		}

		std::stringstream ss(line);
		ss >> source >> target;

		// remove loops as they are annoying
		if (source != target) {
			parser_edges.emplace_back(source, target);
		}
	}

	return parser_edges;
}

void Graph::reduceToLargestScc(ParserEdges& parser_edges) const
{
	std::unordered_set<ParserNodeID> nodes;
	for (auto const& parser_edge: parser_edges) {
		nodes.insert(parser_edge.first);
		nodes.insert(parser_edge.second);
	}

	auto largest_partition = UnionFind<ParserNodeID>().run(nodes, parser_edges);

	auto not_in_largest_partition = [&](ParserEdge const& parser_edge) {
		return !largest_partition.count(parser_edge.first);
	};
	auto first_to_erase = std::remove_if(parser_edges.begin(),
	                                     parser_edges.end(),
	                                     not_in_largest_partition);
	parser_edges.erase(first_to_erase, parser_edges.end());
}

auto Graph::convertIDs(ParserEdges& parser_edges) -> Edges
{
	// First fill to_id map while assigning IDs ...
	std::unordered_map<ParserNodeID, NodeID> to_id;
	NodeID current_id = 0;
	for (auto const& parser_edge: parser_edges) {
		if (!to_id.count(parser_edge.first)) {
			to_id.emplace(parser_edge.first, current_id);
			old_ids.push_back(parser_edge.first);

			++current_id;
		}

		if (!to_id.count(parser_edge.second)) {
			to_id.emplace(parser_edge.second, current_id);
			old_ids.push_back(parser_edge.second);

			++current_id;
		}
	}

	// ... then write new edges with new IDs into new edges vector
	Edges edges;
	for (auto& parser_edge: parser_edges) {
		auto source_id = to_id.at(parser_edge.first);
		auto target_id = to_id.at(parser_edge.second);
		edges.emplace_back(source_id, target_id);
	}

	return edges;
}

void Graph::addAllReverseEdges(Edges& edges) const
{
	// Note: We use this type of loop as we cannot use a range-based loop due to
	// possible iterator invalidation on push.
	auto old_size = edges.size();
	for (std::size_t i = 0; i < old_size; ++i) {
		auto const& edge = edges[i];
		edges.emplace_back(edge.second, edge.first);
	}
}

void Graph::sortAndMakeUnique(Edges& edges) const
{
	// Note: Pairs are sorted lexicographically and thus exactly as we want.
	std::sort(edges.begin(), edges.end());
	auto first_to_erase = std::unique(edges.begin(), edges.end());
	edges.erase(first_to_erase, edges.end());
}

void Graph::fillOffsetsAndNeighbors(Edges const& edges)
{
	NodeID current_source = 0;
	offsets.push_back(current_source);

	std::size_t i;
	for (i = 0; i < edges.size(); ++i) {
		auto const& edge = edges[i];

		// offsets
		while (current_source != edge.first) {
			offsets.push_back(i);
			++current_source;
		}

		// neighbors
		neighbors.push_back(edge.second);
	}
	offsets.push_back(neighbors.size());
}

std::size_t Graph::getNumberOfNodes() const
{
	return old_ids.size();
}

std::size_t Graph::getNumberOfEdges() const
{
	return neighbors.size();
}

std::size_t Graph::degree(NodeID node_id) const
{
	return offsets[node_id + 1] - offsets[node_id];
}

auto Graph::getNodesSortedByDegree() const -> std::vector<NodeID>
{
	std::vector<NodeID> node_ids(getNumberOfNodes());
	std::iota(node_ids.begin(), node_ids.end(), 0);

	auto comp_degree = [&](NodeID node_id1, NodeID node_id2) {
		return degree(node_id1) > degree(node_id2);
	};
	std::sort(node_ids.begin(), node_ids.end(), comp_degree);

	return node_ids;
}

auto Graph::getNeighborRange(NodeID node_id) const -> NeighborRange
{
	auto const begin = neighbors.cbegin() + offsets[node_id];
	auto const end = neighbors.cbegin() + offsets[node_id+1];
	return NeighborRange(begin, end);
}

auto Graph::getRandomNeighbor(NodeID node_id, Random& random) const -> NodeID
{
	debug_assert(degree(node_id) != 0);

	auto neighbor_offset = random.getSizeT(0, degree(node_id) - 1);
	auto neighbor_index = offsets[node_id] + neighbor_offset;

	return neighbors[neighbor_index];
}
