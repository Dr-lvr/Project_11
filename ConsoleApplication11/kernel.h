#pragma once
#include <Windows.h>
#include <string>
enum class c_keyboard : WORD {
	ENTER = 0x0d,
	HOME = 0x5b
};
class kernel
{
public:
	void click();
	void move(int x, int y);
	void echo(std::string str);
	void c_key(std::string str);
private:
	std::map<std::string, c_keyboard> c_key_map{
		{"enter", c_keyboard::ENTER},
		{"home", c_keyboard::HOME}
	};
};