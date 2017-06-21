#include "Clipboard.h"
#include <string>
#include <functional>

namespace SL {
    namespace Clipboard_Lite {

        const UINT formatRTF = RegisterClipboardFormat(_T("Rich Text Format"));
        const UINT formatHTML = RegisterClipboardFormat(_T("HTML Format"));

        Clipboard_ManagerImpl::Clipboard_ManagerImpl() {

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
                                    if (!IgnoreClipUpdateNotice) {
                                        std::lock_guard<std::mutex> l(ClipboardLock);
                                        if (!IgnoreClipUpdateNotice) {
                                            IgnoreClipUpdateNotice = true;
                                            if (OpenClipboard(Hwnd) == TRUE) {

                                                std::string data;
                                                if (onRTF && LoadClip(data, formatRTF)) {
                                                    onRTF(data);
                                                } else if (onHTML && LoadClip(data, formatHTML)) {
                                                    onHTML(data);
                                                } else if (onText && LoadClip(data, CF_TEXT)) {
                                                    onText(data);
                                                }
                                                CloseClipboard();
                                            }
                                        }

                                        IgnoreClipUpdateNotice = false;
                                    }
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

    };
}