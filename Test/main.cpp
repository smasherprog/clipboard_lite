#include "Clipboard_Lite.h"
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

int main(int argc, char* argv[]) {


    std::cout << "Clipboard Example running" << std::endl;
    auto clipboard = SL::Clipboard_Lite::CreateClipboard()
        .onText([](const std::string& text) {
        std::cout << "onText" << std::endl;
        std::cout << text << std::endl;
    }).onImage([](const SL::Clipboard_Lite::Image& image) {
        std::cout << "onImage Height=" << image.Height << " Width=" << image.Width << std::endl;
    })
        .run();

    std::this_thread::sleep_for(1s);
    std::cout << "Copying Text to clipboard" << std::endl;
    std::string txt = "pasted text";
    clipboard.copy(txt);


    std::this_thread::sleep_for(2s);
    auto i = 0;
    while (i++ < 10) {

        std::cout << "Copying Text to clipboard a " << i++ << " time" << std::endl;
        txt = "pasted text a time ";
        txt += std::to_string(i);
       // clipboard.copy(txt);
        std::this_thread::sleep_for(10s);
    }


    return 0;
}
