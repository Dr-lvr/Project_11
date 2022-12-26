#include <Windows.h>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <codecvt>
#include <locale>
#include <map>

#include "kernel.h"
void kernel::move(int x, int y) {
	SetCursorPos(x, y);
	Sleep(1000);
}
void kernel::echo(std::string str) {
    std::wstring msg;
    std::wstring wsTmp(str.begin(), str.end());
    msg = wsTmp;
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

void kernel::c_key(std::string str) {
    INPUT ip = { 0 };
    // Set up a generic keyboard event.
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
    std::cout << std::hex << static_cast<WORD>(c_key_map[str]) << std::endl;
    // Press the key
    ip.ki.wVk = static_cast<WORD>(c_key_map[str]);
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
    Sleep(500);
}