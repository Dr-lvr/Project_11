#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>

#include "editor.h"

void editor::edit_file() {
	std::filesystem::current_path();
	std::filesystem::create_directory("scripts");
	std::string filename;
	std::string program;
	std::cout << "Enter file name: " << std::endl;
	std::cin >> filename;
	std::cout << std::endl;
	std::cout << "Write the program: " << std::endl;
	std::string save_in="scripts/"+filename;
	std::ofstream MyFile(save_in);
	std::getline(std::cin, program);
	if (program == "end") {
		MyFile.close();
		return;
	}
	MyFile << program << "\n";
	MyFile.close();
	while (1) {
		program = "";
		std::ofstream MyFile(save_in, std::ios::app);
		std::getline(std::cin, program);
		if (program == "end") {
			MyFile.close();
			break;
		}
		// Write to the file
		MyFile << program << "\n";
		// Close the file
		MyFile.close();
	};
}