#pragma once
#include <Windows.h>
#include <string>
class kernel
{
public:
	void click();
	void move(int x, int y);
	void echo(std::string str);
};
// map and code direct commands by keyboard https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes

