#pragma once
#include <unordered_map>
#include <QString>
#include <model/Access.h>


class Node
{
public:
	Node() = default;
public:
	std::unordered_map<std::u16string, Access> data;

	std::vector<std::pair<Access, QString>> methods;
	std::vector<Node*> aggregates;
	std::vector<Node*> composes;
	QString name;
	QString alias;
};
