#pragma once
#include <Windows.h>
#include <string>
#include <map>
enum class alpha : WORD {
	A=0x41, B=0x42, C=0x43, D=0x44, E=0x45,
	F=0x46, G=0x47, H=0x48, I=0x49, J=0x4a,
	K=0x4b, L=0x4c, M=0x4d, N=0x4e, O=0x4f,
	P=0x50, Q=0x51, R=0x52, S=0x53, T=0x54,
	U=0x55, V=0x56, W=0x57, X=0x58, Y=0x59,
	Z=0x5a, SP=0x20
};
class kernel
{
public:
	void click();
	void move(int x, int y);
	void echo(std::string str);
private:
	void prompt(WORD letter);
	std::map<char, alpha> alphamap{
	{'a', alpha::A}, {'b', alpha::B}, {'c', alpha::C}, {'d', alpha::D}, {'e', alpha::E},
	{'f', alpha::F}, {'g', alpha::G}, {'h', alpha::H}, {'i', alpha::I}, {'j', alpha::J},
	{'k', alpha::K}, {'l', alpha::L}, {'m', alpha::M}, {'n', alpha::N}, {'o', alpha::O},
	{'p', alpha::P}, {'q', alpha::Q}, {'r', alpha::R}, {'s', alpha::S}, {'t', alpha::T},
	{'u', alpha::U}, {'v', alpha::V}, {'w', alpha::W}, {'x', alpha::X}, {'y', alpha::Y},
	{'z', alpha::Z}, {' ', alpha::SP}
	};
};

