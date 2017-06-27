#include "Clipboard_Lite.h"
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

int main(int argc, char* argv[]) {

    bool done = false;
    std::cout << "The example will exit if you copy or cut rtf to the clipboard" << std::endl;
    auto clipboard = SL::Clipboard_Lite::CreateClipboard()
    .onHTML([](const std::string& html) {
        std::cout << "onHTML" << std::endl;
        std::cout << html << std::endl;
    }).onRTF([&](const std::string& rtf) {
        std::cout << "onRTF" << std::endl;
        std::cout << rtf << std::endl;
        done = true;
    }).onText([](const std::string& text) {
        std::cout << "onText" << std::endl;
        std::cout << text << std::endl;
    }).run();

    std::this_thread::sleep_for(1s);
    std::string txt = "pasted text";
    clipboard.paste_Text(txt);

    while (!done) {
        std::this_thread::sleep_for(1s);
    }
    return 0;
}
