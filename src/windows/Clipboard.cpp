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
                      if (info->bmiHeader.biBitCount != 32) {
                          std::cout << "Not 32 bits"<< "\n";
                          return;
                      }
                        Image img;
                        img.Width = info->bmiHeader.biWidth;
                        img.Height = info->bmiHeader.biHeight;
                 /*       BMP bmp = { 0 };
                        bmp.header.type = 0x4D42;
                        bmp.header.offset = 54;
                        bmp.header.bfSize = info->biSizeImage + bmp.header.offset;
                        bmp.dib = *info;

                        std::cout << "Type: " << std::hex << bmp.header.type << std::dec << "\n";
                        std::cout << "bfSize: " << bmp.header.bfSize << "\n";
                        std::cout << "Reserved: " << bmp.header.reserved << "\n";
                        std::cout << "Offset: " << bmp.header.offset << "\n";
                        std::cout << "biSize: " << bmp.dib.biSize << "\n";
                        std::cout << "Width: " << bmp.dib.biWidth << "\n";
                        std::cout << "Height: " << bmp.dib.biHeight << "\n";
                        std::cout << "Planes: " << bmp.dib.biPlanes << "\n";
                        std::cout << "Bits: " << bmp.dib.biBitCount << "\n";
                        std::cout << "Compression: " << bmp.dib.biCompression << "\n";
                        std::cout << "Size: " << bmp.dib.biSizeImage << "\n";
                        std::cout << "X-res: " << bmp.dib.biXPelsPerMeter << "\n";
                        std::cout << "Y-res: " << bmp.dib.biYPelsPerMeter << "\n";
                        std::cout << "ClrUsed: " << bmp.dib.biClrUsed << "\n";
                        std::cout << "ClrImportant: " << bmp.dib.biClrImportant << "\n";*/
                       
                        img.PixelStride = info->bmiHeader.biBitCount / 4;
                        img.Data = std::shared_ptr<unsigned char>(new unsigned char[info->bmiHeader.biSizeImage], [](auto p) {if (p) delete[] p; });
                        memcpy(img.Data.get(), (info + 1), info->bmiHeader.biSizeImage);
                        assert(info->bmiHeader.biSizeImage == img.Width* img.Height*4);
                        onImage(img);

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
                                   
                                        if (onText) {
                                            LoadClipText();
                                        }
                                        if (onImage) {
                                            LoadClipImage();
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