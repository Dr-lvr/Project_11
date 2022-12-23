#include <Windows.h>
#include <string>
#include <map>

#include "kernel.h"

void kernel::move(int x, int y) {
	SetCursorPos(x, y);
	Sleep(1000);
}
void kernel::prompt(WORD letter) {
    INPUT ip={0};
    // Set up a generic keyboard event.
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    // Press the key
    ip.ki.wVk = letter; // virtual-key code for the "a" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
    Sleep(500);
}
void kernel::echo(std::string str) {
    for (auto c : str) {
        prompt(static_cast<WORD>(alphamap[c]));
    }
}
void kernel::click() {
    mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); // Left click
    Sleep(1000);
}