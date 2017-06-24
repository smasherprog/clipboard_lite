#pragma once
#include <string>
#include <functional>
#include <thread>
#include <atomic>

#include <X11/Xlib.h>

namespace SL {
    namespace Clipboard_Lite {

        class Clipboard_ManagerImpl {
            std::thread BackGroundWorker;
            std::atomic<bool> Pasting;
            Window Window_;
            Display* Display_;
            
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