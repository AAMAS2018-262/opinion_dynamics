#pragma once

#include "basic_types.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

template <typename T>
class UnionFind
{
public:
	using Relation = std::pair<T, T>;

	UnionFind() = default;
	std::unordered_set<T> run(std::unordered_set<T> const& base_set,
	                          std::vector<Relation> const& relations);

private:
	// data strucutres
	using ElementID = int64_t;
	static ElementID const NO_ID = -1;

	struct Element
	{
		ElementID const id;
		T const value;

		ElementID parent_id;
		int tree_size;

		Element(ElementID id, T value)
			: id(id), value(value), parent_id(NO_ID), tree_size(1) {}
	};

	std::unordered_map<T, ElementID> to_id;
	std::vector<Element> elements;
	Random random;

	ElementID max_id;
	int max_size;

	// helper functions
	void initDataStructures(std::unordered_set<T> const& base_set);
	ElementID findRoot(ElementID id);
	void uniteSets(ElementID root1, ElementID root2);
};

template <typename T>
std::unordered_set<T> UnionFind<T>::run(std::unordered_set<T> const& base_set,
                                        std::vector<Relation> const& relations)
{
	if (base_set.empty()) { return std::unordered_set<T>(); }

	initDataStructures(base_set);

	// partition wrt the relations
	for (auto const& relation: relations) {
		auto root1 = findRoot(to_id[relation.first]);
		auto root2 = findRoot(to_id[relation.second]);

		if (root1 != root2) {
			uniteSets(root1, root2);
		}
	}

	// collect largest partition
	std::unordered_set<T> largest_partition;
	largest_partition.reserve(max_size);
	for (auto const& element: elements) {
		if (findRoot(element.id) == max_id) {
			largest_partition.insert(element.value);
		}
	}

	return largest_partition;
}

template <typename T>
void UnionFind<T>::initDataStructures(std::unordered_set<T> const& base_set)
{
	ElementID id = 0;
	for (auto value: base_set) {
		elements.emplace_back(id, value);
		to_id.emplace(value, id);

		++id;
	}

	max_id = 0;
	max_size = 1;
}

template <typename T>
auto UnionFind<T>::findRoot(ElementID const id) -> ElementID
{
	// find root
	ElementID current_id = id;
	while (elements[current_id].parent_id != NO_ID) {
		current_id = elements[current_id].parent_id;
	}

	auto root = current_id;

	// path compression
	current_id = id;
	while (current_id != root) {
		auto parent_id = elements[current_id].parent_id;
		elements[current_id].parent_id = root;
		current_id = parent_id;
	}

	return root;
}

template <typename T>
void UnionFind<T>::uniteSets(ElementID root1_id, ElementID root2_id)
{
	auto& root1 = elements[root1_id];
	auto& root2 = elements[root2_id];

	// use randomization to decide which tree is hung under which
	if (random.throwCoin()) {
		root1.parent_id = root2_id;
		root2.tree_size += root1.tree_size;

		if (root2.tree_size > max_size) {
			max_id = root2_id;
			max_size = root2.tree_size;
		}
	}
	else {
		root2.parent_id = root1_id;
		root1.tree_size += root2.tree_size;

		if (root1.tree_size > max_size) {
			max_id = root1_id;
			max_size = root1.tree_size;
		}
	}
}
