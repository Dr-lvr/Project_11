#pragma once
#include <string>
#include <map>

#include "kernel.h"

enum class commands {
	move, click, write
};
class engine
{
public:
	void get_macro(std::string arg);
private:
	kernel bot;
	void interpret(std::string string);
	std::map<std::string, commands> command_map{
	{"move", commands::move}, {"click", commands::click}, {"echo", commands::write}
	};
};