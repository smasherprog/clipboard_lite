#include "Clipboard_Lite.h"
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

int main(int argc, char* argv[]) {


    std::cout << "Clipboard Example running" << std::endl;
    auto clipboard = SL::Clipboard_Lite::CreateClipboard()
        .onText([](const char* string, size_t len) {
        std::cout << "onText" << len<< std::endl;
        std::cout << string << std::endl;
    })
        .run();

    std::this_thread::sleep_for(1s);
    std::cout << "Copying Text to clipboard" << std::endl;
    std::string txt = "pasted text";
    clipboard.copy(txt);



    auto i = 0;
    while (true) {
        std::this_thread::sleep_for(10s);
        std::cout << "Copying Text to clipboard a " << i++ << " time" << std::endl;
        txt = "pasted text a time ";
        txt += std::to_string(i);
        clipboard.copy(txt);
    }
    std::this_thread::sleep_for(10s);

    return 0;
}
