#include "Clipboard.h"
#include <functional>
#include <string>

namespace SL
{
namespace Clipboard_Lite
{

    Clipboard_ManagerImpl::Clipboard_ManagerImpl()
    {
    }
    Clipboard_ManagerImpl::~Clipboard_ManagerImpl()
    {
        if(BackGroundWorker.joinable()) {
            BackGroundWorker.join();
        }
    }
    void Clipboard_ManagerImpl::run()
    {
        BackGroundWorker = std::thread([&] {
            Display_ = XOpenDisplay(NULL);
            if(Display_) {
                Window_ = XCreateWindow(Display_, RootWindow(Display_, DefaultScreen(Display_)), 10, 10, 200, 200, 0,
                    CopyFromParent, InputOnly, 0, 0, NULL);
                if(Window_) {

                    XDestroyWindow(Display_, Window_);
                }
                XCloseDisplay(Display_);
            }
        });
    }
    void RestoreClip(const std::string& str)
    {
    }
    void Clipboard_ManagerImpl::paste_HTML(const std::string& html)
    {
        Pasting = true;
        RestoreClip(html);
    }
    void Clipboard_ManagerImpl::paste_RTF(const std::string& rtf)
    {
        Pasting = true;
        RestoreClip(rtf);
    }
    void Clipboard_ManagerImpl::paste_Text(const std::string& text)
    {
        Pasting = true;
        RestoreClip(text);
    }
}
}