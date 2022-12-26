#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <string>
#include <deque>

#include "engine.h"
#include "kernel.h"

void engine::get_macro(std::string arg) {
    std::filesystem::current_path();
    std::fstream file_reader;
    std::string executable = "scripts/" + arg;
    file_reader.open(executable, std::ios::in);         //open a file to perform read operation using file object
    if (file_reader.is_open()) {                       //checking whether the file is open
        std::string tp;
        while (getline(file_reader, tp)) {              //read data from file object and put it into string.
            if (tp == "") continue;
            interpret(tp);                              //print the data of the string
        }
        file_reader.close();                            //close the file object.
    }
    else {
        std::cout << "The file don't exists" << std::endl;
    }
}
void engine::interpret(std::string str) {
    machine_view mv;
    auto it = nav_map.find(std::make_pair("desktop", "discord"));
    std::stringstream ss(str);
    std::stringstream echo_stream;
    std::string token;
    std::deque<std::string> command_dqe;
    while (ss >> token)
    {
        command_dqe.push_back(token); 
    }
    switch (command_map[command_dqe[0]]) {
    case commands::click:
        bot.click();
        break;
    case commands::move:
        bot.move(stoi(command_dqe[1]), stoi(command_dqe[2]));
        break;
    case commands::go:
        //tested.....now needs to recursively evaluate context
        it = nav_map.find(std::make_pair(command_dqe[1], command_dqe[2]));
        if (it != nav_map.end()){
            c_move = mv.start_view(it->second);
        }
        if (c_move.first != -1 && c_move.second != -1) {
            bot.move(c_move.first, c_move.second);
        }
        else {
            std::cout << "ERROR: in commands::go, move not found" << std::endl;
        }
        break;
    case commands::control:
        bot.c_key(command_dqe[1]);
        break;
    case commands::write:
        for (size_t i = 1; i < command_dqe.size(); ++i) {
            if (command_dqe[i] != "\"") {
                echo_stream << command_dqe[i] << " ";
            }
        }
        bot.echo(echo_stream.str());
        break;
    default:
        std::cout << "error at parser switch" << std::endl;
        break;
    }
}