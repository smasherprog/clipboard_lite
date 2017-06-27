#pragma once
#include <string>
#include <functional>

#include <thread>
#include <mutex>
#include <atomic>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
namespace SL {
    namespace Clipboard_Lite {

        class Clipboard_ManagerImpl {
            std::thread BackGroundWorker;
            std::atomic<bool> Pasting;

            Display* Display_;
            Window Window_=0;

        public:


            std::function<void(const std::string& text)> onText;

            Clipboard_ManagerImpl();
            ~Clipboard_ManagerImpl();
            void run();

            void paste(const std::string& text);
        };
    }
}