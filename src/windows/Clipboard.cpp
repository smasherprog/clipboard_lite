#include "Clipboard.h"
#include "Clipboard_Lite.h"
#include <string>
#include <functional>
#include <memory>
#include <iostream>
#include <assert.h>

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

        void Clipboard_ManagerImpl::LoadClipImage() {
            auto c = ClipWrapper(Hwnd);
            if (!c)  return;
            if (IsClipboardFormatAvailable(CF_BITMAP) || IsClipboardFormatAvailable(CF_DIB) || IsClipboardFormatAvailable(CF_DIBV5))
            {
                HANDLE hClipboard = (BITMAPINFO*)GetClipboardData(CF_DIB);
                if (hClipboard)
                {
                    GlobalLockWrapper dib(GlobalLock(hClipboard));
                    if (dib)
                    {
                        auto info = reinterpret_cast<LPBITMAPINFO>(dib.Ptr);
                        Image img;
                        if ((info->bmiHeader.biBitCount == 24 || info->bmiHeader.biBitCount == 32) &&
                            info->bmiHeader.biCompression == BI_RGB && info->bmiHeader.biClrUsed == 0) {

                            img.Width = info->bmiHeader.biWidth;
                            img.Height = info->bmiHeader.biHeight;

                            img.PixelStride = info->bmiHeader.biBitCount / 4;
                            img.Data = std::shared_ptr<unsigned char>(new unsigned char[info->bmiHeader.biSizeImage], [](auto p) {if (p) delete[] p; });
                            memcpy(img.Data.get(), (info + 1), info->bmiHeader.biSizeImage);

                            auto linewidth = 0;
                            auto depth = info->bmiHeader.biBitCount / 8; 
                            if (depth == 3) linewidth = 4 * ((3 * img.Width + 3) / 4); 
                            else linewidth = 4 * img.Width;
                            img.Data = std::shared_ptr<unsigned char>(new unsigned char[img.Height *  img.Width * depth], [](auto p) {if (p) delete[] p; });

                            auto *p = img.Data.get();

                            for (int i = img.Height - 1; i >= 0; i--) { // for each row, from last to first
                                auto r = (unsigned char*)(info->bmiColors) + i*linewidth; // beginning of pixel data for the ith row
                                for (int j = 0; j < img.Width; j++) { // for each pixel in a row
                                    auto bb = *r++; // BGR is in DIB
                                    auto gg = *r++;
                                    auto rr = *r++;
                                    *p++ = rr; // we want RGB
                                    *p++ = gg;
                                    *p++ = bb;
                                    if (depth == 4) *p++ = *r++;
                                }
                            }
                            onImage(img);
                        }
                    }

                }
            }
        }
        void Clipboard_ManagerImpl::LoadClipText() {
            auto c = ClipWrapper(Hwnd);
            if (!c) return;
            if (IsClipboardFormatAvailable(CF_TEXT)) {
                auto h = ::GetClipboardData(CF_TEXT);
                if (h) {
                    auto pData = GlobalLock(h);
                    auto nLength = GlobalSize(h);
                    if (pData && nLength > 0) {
                        std::string buffer;
                        buffer.resize(nLength);
                        memcpy((void*)buffer.data(), pData, nLength);
                        GlobalUnlock(h);
                        onText(buffer);
                    }
                }
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
                                    if (!Copying) {
                                        if (onText) {
                                            LoadClipText();
                                        }
                                        if (onImage) {
                                            LoadClipImage();
                                        }
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