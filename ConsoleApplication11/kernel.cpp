#include <Windows.h>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <map>
#include <codecvt>
#include <locale>

#include "kernel.h"
void kernel::move(int x, int y) {
	SetCursorPos(x, y);
	Sleep(1000);
}
void kernel::echo(std::string str) {
    std::wstring msg;
    std::wstring wsTmp(str.begin(), str.end());
    msg = wsTmp;
    //or std::string msg = "ABCD - abcd - 1234";

    std::vector<INPUT> vec;
    for (auto ch : msg)
    {
        INPUT input = { 0 };
        input.type = INPUT_KEYBOARD;
        input.ki.dwFlags = KEYEVENTF_UNICODE;
        input.ki.wScan = ch;
        vec.push_back(input);

        input.ki.dwFlags |= KEYEVENTF_KEYUP;
        vec.push_back(input);
    }
    SendInput(vec.size(), vec.data(), sizeof(INPUT));
}
void kernel::click() {
    mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); // Left click
    Sleep(1000);
}