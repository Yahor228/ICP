#pragma once
#include <unordered_map>
#include <string_view>

class Connection;

class ConnMap
{
public:
	ConnMap();
public:
	void AttachFrom(std::u16string_view, Connection*);
	void AttachTo(std::u16string_view, Connection*);
	Connection* DetachFrom(std::u16string_view);
	Connection* DetachTo(std::u16string_view);
private:
	std::unordered_map<std::u16string_view, Connection*> from;
	std::unordered_map<std::u16string_view, Connection*> to;
};