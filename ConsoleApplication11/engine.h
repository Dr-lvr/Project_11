#pragma once
#include <string>
#include <utility>
#include <map>

#include "kernel.h"
#include "db_test.h"
#include "machine_view.h"

enum class commands {
	move, click, write, go, control
};
class engine
{
public:
	void get_macro(std::string arg);
private:
	kernel bot;
	void interpret(std::string string);
	std::pair<int, int> c_move = std::make_pair(-1, -1);
	std::map<std::string, commands> command_map{
	{"move", commands::move}, {"click", commands::click}, {"echo", commands::write}, {"go", commands::go}, {"control", commands::control}
	};
	std::map<std::pair<std::string, std::string>, std::string> nav_map = db_test::test_navigation();
};