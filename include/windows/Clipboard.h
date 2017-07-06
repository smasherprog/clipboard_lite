#pragma once
#include <string>
#include <functional>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

namespace SL {
    namespace Clipboard_Lite {

        class Clipboard_ManagerImpl {
            std::thread BackGroundWorker;
            std::atomic<bool> Copying;
            HWND Hwnd = nullptr;
            std::vector<unsigned char> Data;

        public:

            std::function<void(const char* data, size_t len)> onText;

            Clipboard_ManagerImpl();
            ~Clipboard_ManagerImpl();

            void run();
            void copy(const std::string& test);

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
            template<class T>void RestoreClip(const T& buffer, UINT format) {
                if (OpenClipboard(Hwnd) == TRUE) {
                    if (EmptyClipboard() == TRUE && buffer.size() > 0) {
                        auto hData = GlobalAlloc(GMEM_MOVEABLE, buffer.size());
                        if (hData) {
                            auto pData = GlobalLock(hData);
                            if (pData) {
                                memcpy(pData, buffer.data(), buffer.size());
                                GlobalUnlock(hData);
                                if (::SetClipboardData(format, hData)) {
                                    //clipboard takes ownership of the memory
                                    hData = nullptr;
                                }
                            }
                        }
                        if (hData) {
                            GlobalFree(hData);
                        }
                    }
                    CloseClipboard();
                }
            }
        };
    }
}