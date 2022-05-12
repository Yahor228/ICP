#pragma once
#include <unordered_map>
#include <QString>
#include <array>


#define LIST_ACCESS()\
X(Public)\
X(Private)\
X(Protected)

enum class Access
{
#define X(a) a,
	LIST_ACCESS()
#undef X
	size
};
Access GetAccess(QString s);
QString FromAccess(Access a);

constexpr std::array<std::string_view, size_t(Access::size)> access_strings{
#define X(a) #a,
	LIST_ACCESS()
#undef X
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
	QString name;
	QString alias;
};
