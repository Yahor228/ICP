#pragma once
#include <array>
#include <QString>


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
Access GetAccess(const QString& s);
QString FromAccess(Access a);

constexpr std::array<std::string_view, size_t(Access::size)> access_strings{
#define X(a) #a,
	LIST_ACCESS()
#undef X
};