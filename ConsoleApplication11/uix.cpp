#include <filesystem>
#include <iostream>
#include <string>

#include "uix.h"
#include "editor.h"
#include "demo.h"

void uix::get_choice() {
    int scelta;
    system("cls");
    std::cout << "x---------------------------x" << std::endl;
    std::cout << "x  Project_11_alpha_V0.0x   x" << std::endl;
    std::cout << "x---------------------------x" << std::endl;
    std::cout << std::endl;
    std::cout
        << "1\t"
        << "Write sub-routine"
        << std::endl
        << "2\t"
        << "Execute sub-routine"
        << std::endl
        << "3\t"
        << "Compose batch"
        << std::endl
        << "4\t"
        << "Execute batch"
        << std::endl
        << "5\t"
        << "Help"
        << std::endl
        << "6\t"
        << "Exit"
        << std::endl;
    std::cin
        >> scelta;
    std::cout
        << std::endl;
    if (scelta > 0 && scelta <= 6) {
        return print_menu(scelta);
    }
    return get_choice();
}
void uix::print_menu(int n) {
    system("cls");
    std::cout << "x---------------------------x" << std::endl;
    std::cout << "x  Project_11_alpha_V0.0x   x" << std::endl;
    std::cout << "x---------------------------x" << std::endl;
    std::cout << std::endl;
    switch (n) {
    case 1:
        std::cout
            << "1\t"
            << "Write sub-routine"
            << std::endl;
        demo::edit_demo();
        get_choice();
        return;
    case 2:
        std::cout << "2\t"
            << "Execute sub-routine"
            << std::endl;
        map_macro();
        demo::exec_macro();
        get_choice();
        return;
    case 3:
        std::cout << "3\t"
            << "Compose batch"
            << std::endl;
        map_macro();
        demo::exec_macro();
        get_choice();
        return;
    case 4:
        std::cout << "4\t"
            << "Execute batch"
            << std::endl;
        map_macro();
        demo::exec_macro();
        get_choice();
        return;
    case 5:
        std::cout
            << "5\t"
            << "Help"
            << std::endl;
        system(std::string("start https://www.openvayu.com/Project_11_quick_references.pdf").c_str());
        get_choice();
        return;
    case 6:
        std::cout
            << "6\t"
            << "Exit"
            << std::endl;
        return;
    default:
        return;
    }
}
void uix::map_macro() {
    system("cls");
    std::filesystem::current_path();
    std::string path = "scripts";
    int index = 0;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        std::cout << ++index << "\t" << entry.path().filename() << std::endl;
    }
}