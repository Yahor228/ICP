/**
 * @file commandstack.h
 * @author Yahor Senichak (xsenic00)
 * @brief declaration of stack for undo redo 
 */


#pragma once
#include <QUndoStack>
#include <vector>
#include <memory>

/// @brief Command stack for each tab in the system
class CommandStack
{
public:
	/// @brief Singletone instance of the stack
	/// @return static instance reference
	static CommandStack& instance()
	{
		static CommandStack c;
		return c;
	}

	/// @brief Appends a stack for new tab
	static void append()
	{
		instance().qu.emplace_back(std::make_unique<QUndoStack>());
	}

	/// @brief Returns current stack
	/// @return reference to current stack
	static QUndoStack& current()
	{
		return instance().current_i();
	}

	/// @brief Sets current index according to selected tab
	/// @param index index of tab
	static void set_current(size_t index)
	{
		instance().set_current_i(index);
	}

	/// @brief Removes undo stack according to index
	/// @param index index of tab
	static void remove_at(size_t index)
	{
		auto& x = instance().qu;
		x.at(index)->disconnect();
		x.erase(x.begin() + index);
	}

	/// @brief Removes all the contents of the stacks
	/// needed for valid deletion
	static void clear()
	{
		instance().qu.clear();
		instance().index = 0;
	}
public:

	/// @brief [internal] indexation function
	/// @return current stack
	QUndoStack& current_i()
	{
		return *qu[index];
	}

	/// @brief [internal] sets index of current stack
	/// @param xindex 
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
