#pragma once
#include <QUndoStack>
#include <vector>
#include <memory>


class CommandStack
{
public:
	static CommandStack& instance()
	{
		static CommandStack c;
		return c;
	}
	static void append()
	{
		instance().qu.emplace_back(std::make_unique<QUndoStack>());
	}
	static QUndoStack& current()
	{
		return instance().current_i();
	}
	static void set_current(size_t index)
	{
		instance().set_current_i(index);
	}
	static void clear()
	{
		instance().qu.clear();
		instance().index = 0;
	}
public:
	QUndoStack& current_i()
	{
		return *qu[index];
	}
	void set_current_i(size_t xindex)
	{
		if((int)xindex >= 0)
			index = xindex;
	}
private:
	CommandStack() = default;
	CommandStack(const CommandStack&) = delete;
	CommandStack(CommandStack&&) = delete;
	~CommandStack()
	{
		for (auto& i : qu)
			i->disconnect();
	}
private:
	std::vector<std::unique_ptr<QUndoStack>> qu;
	size_t index = 0;
};