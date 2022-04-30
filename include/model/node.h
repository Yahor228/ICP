#pragma once
#include <unordered_map>
#include <string>

enum class Access
{
	Public,
	Private,
	Protected
};

class Node
{
public:
	Node() = default;
public:
	std::unordered_map<std::u16string, Access> data;
	std::unordered_map<std::u16string, Access> methods;
	std::vector<Node*> aggregates;
	std::vector<Node*> composes;
	std::u16string name;
	std::u16string alias;
};
