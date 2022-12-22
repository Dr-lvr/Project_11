#include "uix.h"
#include "editor.h"
#include "demo.h"
#include <iostream>
#include <string>
void uix::get_choice() {
    int scelta;
    system("cls");
    std::cout
        << "1\t"
        << "Scrivi una macro"
        << std::endl
        << "2\t"
        << "Esegui una macro"
        << std::endl
        << "3\t"
        << "Help"
        << std::endl
        << "4\t"
        << "Exit"
        << std::endl;
    std::cin
        >> scelta;
    std::cout
        << std::endl;
    if (scelta > 0 && scelta <= 4) {
        return print_menu(scelta);
    }
    return get_choice();
}
void uix::print_menu(int n) {
    system("cls");
    std::cout << "x--------------------x" << std::endl;
    switch (n) {
    case 1:
        std::cout
            << "1\t"
            << "Scrivi una macro"
            << std::endl;
        demo::edit_demo();
        return;
    case 2:
        std::cout << "2\t"
            << "Esegui una macro"
            << std::endl;
        demo::exec_macro();
        get_choice();
        return;
    case 3:
        std::cout
            << "3\t"
            << "Help"
            << std::endl;
        system(std::string("start http://openvayu/").c_str());
        get_choice();
        return;
    case 4:
        std::cout
            << "4\t"
            << "Exit"
            << std::endl;
        return;
    default:
        return;
    }
}