#pragma once

#include "random.h"

#include <string>
#include <vector>

class Graph
{
public:
	// member types
	using NodeID = std::size_t;
	using ParserNodeID = std::string;
	using Neighbors = std::vector<NodeID>;
	class NeighborRange
	{
		using const_iterator = Neighbors::const_iterator;
		const_iterator const _begin;
		const_iterator const _end;

	public:
		NeighborRange(const_iterator begin, const_iterator end)
			: _begin(begin), _end(end) {}

		const_iterator begin() const { return _begin; }
		const_iterator end() const { return _end; }
	};

	// member functions

	Graph() = default;

	void buildFromFile(std::string const& graph_file);

	std::string const& getFilename() const;
	std::size_t getNumberOfNodes() const;
	std::size_t getNumberOfEdges() const;
	std::size_t degree(NodeID node_id) const;
	// Note: This function builds a new vector with the size being the number
	// of nodes. So, beware of calling this too often.
	std::vector<NodeID> getNodesSortedByDegree() const;

	NeighborRange getNeighborRange(NodeID node_id) const;
	NodeID getRandomNeighbor(NodeID node_id, Random& random) const;

private:
	std::string filename;

	// node structures
	std::vector<ParserNodeID> old_ids;

	// edge structures
	std::vector<std::size_t> offsets;
	Neighbors neighbors;

	// helper definitions and functions for buildFromFile
	using ParserEdge = std::pair<ParserNodeID, ParserNodeID>;
	using ParserEdges = std::vector<ParserEdge>;
	using Edge = std::pair<NodeID, NodeID>;
	using Edges = std::vector<Edge>;

	ParserEdges readEdges(std::string const& graph_file) const;
	void reduceToLargestScc(ParserEdges& edges) const;
	Edges convertIDs(ParserEdges& edges);
	void addAllReverseEdges(Edges& edges) const;
	void sortAndMakeUnique(Edges& edges) const;
	void fillOffsetsAndNeighbors(Edges const& edges);
};
