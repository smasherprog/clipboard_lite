#pragma once
#include <string>
#include <functional>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <thread>
#include <mutex>
#include <atomic>

namespace SL {
    namespace Clipboard_Lite {

        class Clipboard_ManagerImpl {
            std::thread BackGroundWorker;
            std::atomic<bool> Pasting;
            HWND Hwnd = nullptr;
        public:

            std::function<void(const std::string& html)> onHTML;
            std::function<void(const std::string& html)> onRTF;
            std::function<void(const std::string& text)> onText;

            Clipboard_ManagerImpl();
            ~Clipboard_ManagerImpl();

            void run();
            void paste_HTML(const std::string& html);
            void paste_RTF(const std::string& rtf);
            void paste_Text(const std::string& text);
        };
    }
}