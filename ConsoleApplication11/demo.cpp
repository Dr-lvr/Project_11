#include "demo.h"
#include "engine.h"
#include "editor.h"
#include <iostream>
#include <string>

void demo::exec_macro() {
	std::string path;
	std::cout << "Insert path: " << std::endl;
	std::cin >> path;
	demo::engine_demo(path);
}
void demo::engine_demo(std::string str) {
	engine e;
	e.get_macro(str);
}
void demo::edit_demo() {
	editor e;
	e.edit_file();
}