#include "Clipboard.h"
#include <string>
#include <functional>

namespace SL {
    namespace Clipboard_Lite {


        Clipboard_ManagerImpl::Clipboard_ManagerImpl() {
            Copying = false;
        }
        Clipboard_ManagerImpl::~Clipboard_ManagerImpl() {
            if (Hwnd) {
                PostMessage(Hwnd, WM_QUIT, 0, 0);
            }
            if (BackGroundWorker.joinable()) {
                BackGroundWorker.join();
            }
        }
        void Clipboard_ManagerImpl::run() {
            BackGroundWorker = std::thread([&] {
                WNDCLASSEX wndclass = {};
                memset(&wndclass, 0, sizeof(wndclass));
                wndclass.cbSize = sizeof(WNDCLASSEX);
                wndclass.lpfnWndProc = DefWindowProc;
                wndclass.lpszClassName = _T("myclass");
                if (RegisterClassEx(&wndclass))
                {
                    Hwnd = CreateWindowEx(0, wndclass.lpszClassName, _T("clipwatcher"), 0, 0, 0, 0, 0, HWND_MESSAGE, 0, 0, 0);
                    if (Hwnd) {
                        if (AddClipboardFormatListener(Hwnd)) {
                            MSG msg;
                            while (GetMessage(&msg, Hwnd, 0, 0) != 0) {
                                if (msg.message == WM_CLIPBOARDUPDATE) {
                                    if (!Copying && OpenClipboard(Hwnd) == TRUE) {

                                        if (onText && LoadClip(Data, CF_TEXT)) {
                                            onText(reinterpret_cast<char*>(Data.data()), Data.size());
                                        }
                                        CloseClipboard();
                                    }
                                    Copying = false;
                                }
                                else
                                {
                                    TranslateMessage(&msg);
                                    DispatchMessage(&msg);
                                }
                            }
                            RemoveClipboardFormatListener(Hwnd);
                        }
                        UnregisterClass(wndclass.lpszClassName, nullptr);
                    }
                }
            });
        }

        void Clipboard_ManagerImpl::copy(const std::string& text) {
            Copying = true;
            RestoreClip(text, CF_TEXT);
        }

    };
}