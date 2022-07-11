/**
 * @file ChangeMode.h
 * @author Yahor Senichak (xsenic00)
 * @brief change flags for visitor
 */


#pragma once

enum ChangeMode
{
	none,
	name = 1,
	data = 2,
	methods = 4,
	values = data | methods,
	all = values | name,
};