#include "Clipboard.h"
#include <string>
#include <functional>

namespace SL {
    namespace Clipboard_Lite {

        const UINT formatRTF = RegisterClipboardFormat(_T("Rich Text Format"));
        const UINT formatHTML = RegisterClipboardFormat(_T("HTML Format"));
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
        Clipboard_ManagerImpl::Clipboard_ManagerImpl() {
            Pasting = false;
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
                                    if (!Pasting && OpenClipboard(Hwnd) == TRUE) {
                                        std::string data;
                                        if (onRTF && LoadClip(data, formatRTF)) {
                                            onRTF(data);
                                        }
                                        else if (onHTML && LoadClip(data, formatHTML)) {
                                            onHTML(data);
                                        }
                                        else if (onText && LoadClip(data, CF_TEXT)) {
                                            onText(data);
                                        }
                                        CloseClipboard();
                                    }
                                    Pasting = false;
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
        void Clipboard_ManagerImpl::paste_HTML(const std::string& html) {
            Pasting = true;
            RestoreClip(html, formatHTML);
        }
        void Clipboard_ManagerImpl::paste_RTF(const std::string& rtf) {
            Pasting = true;
            RestoreClip(rtf, formatRTF);
        }
        void Clipboard_ManagerImpl::paste_Text(const std::string& text) {
            Pasting = true;
            RestoreClip(text, CF_TEXT);
        }

    };
}