// ConsoleApplication11.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//
#include "demo.h"
#include <iostream>
int main(){
	std::string path;
	std::cout << "Insert path: " << std::endl;
	std::cin >> path;
	demo::engine_demo(path);
}