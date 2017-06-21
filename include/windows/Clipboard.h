#pragma once
#include <string>
#include <functional>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <thread>
#include <mutex>

namespace SL {
    namespace Clipboard_Lite {

        class Clipboard_ManagerImpl {
            std::thread BackGroundWorker;
            std::mutex ClipboardLock;
            bool Running = false;

            bool IgnoreClipUpdateNotice = false;

            HWND Hwnd = nullptr;
        public:

            std::function<void(const std::string& html)> onHTML;
            std::function<void(const std::string& html)> onRTF;
            std::function<void(const std::string& text)> onText;

            Clipboard_ManagerImpl();
            ~Clipboard_ManagerImpl();
            template<class T>bool LoadClip(T& buffer, UINT format) {
                if (IsClipboardFormatAvailable(format)) {
                    auto h = ::GetClipboardData(format);
                    if (h) {
                        auto pData = GlobalLock(h);
                        auto nLength = GlobalSize(h);
                        if (pData && nLength > 0) {
                            buffer.resize(nLength);
                            memcpy((void*)buffer.data(), pData, nLength);
                            GlobalUnlock(h);
                            return true;
                        }
                    }
                }
                return false;
            }
            void run();

        };
    }
}