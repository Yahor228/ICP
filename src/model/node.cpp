#include <model/node.h>
#include <util/util.h>

Access GetAccess(QString s)
{
	switch (s[0].toLatin1())
	{
	case '+':return Access::Public;
	case '~':
	case '-':return Access::Private;
	case '#':return Access::Protected;
	}
	return Access::Private;
}

QString FromAccess(Access a)
{
	switch (a)
	{
	case Access::Public:return qsl("+");
	case Access::Private:return qsl("-");
	case Access::Protected:return qsl("#");
	}
	return qsl("~");
}
