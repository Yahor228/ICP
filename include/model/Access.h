#pragma once
#include <array>
#include <QString>
#include <util/util.h>


#define LIST_ACCESS()\
X(Public)\
X(Private)\
X(Protected)\
X(Default)

enum class Access
{
#define X(a) a,
	LIST_ACCESS()
#undef X
	size
};

inline Access GetAccess(const QString& s)
{
	switch (s[0].toLatin1())
	{
	case '+':return Access::Public;
	case '~':return Access::Default;
	case '-':return Access::Private;
	case '#':return Access::Protected;
	}
	return Access::Private;
}

inline QString FromAccess(Access a)
{
	switch (a)
	{
	case Access::Public:return qsl("+");
	case Access::Private:return qsl("-");
	case Access::Protected:return qsl("#");
	case Access::Default:return qsl("~");
	}
	return qsl("~");
}
constexpr std::array<std::string_view, size_t(Access::size)> access_strings{
#define X(a) #a,
	LIST_ACCESS()
#undef X
};