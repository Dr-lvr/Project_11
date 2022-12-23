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

