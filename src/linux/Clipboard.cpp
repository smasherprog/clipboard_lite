#include "Clipboard.h"

#include <string>
#include <functional>
#include <assert.h>
#include <iostream>


namespace SL
{
namespace Clipboard_Lite
{

    Clipboard_ManagerImpl::Clipboard_ManagerImpl()
    {

        Pasting = false;
        Display_ = XOpenDisplay(NULL);
        assert(Display_);
        Window_ = XCreateWindow(Display_,
                                RootWindow(Display_, XDefaultScreen(Display_)),
                                10,
                                10,
                                200,
                                200,
                                0,
                                CopyFromParent,
                                InputOnly,
                                0,
                                0,
                                NULL);
        assert(Window_);
    }
    Clipboard_ManagerImpl::~Clipboard_ManagerImpl()
    {

        if(BackGroundWorker.joinable()) {
            BackGroundWorker.join();
        }
        if(Display_) {
            XDestroyWindow(Display_, Window_);
            if(Window_) {
                XCloseDisplay(Display_);
            }
        }
    }

    void Clipboard_ManagerImpl::run()
    {

        BackGroundWorker = std::thread([&] {

            const Atom TIMESTAMP = XInternAtom(Display_, "TIMESTAMP", 0);
            const Atom PRIMARY_TIMESTAMP = XInternAtom(Display_, "PRIMARY_TIMESTAMP", 0);
            XConvertSelection(Display_, XA_PRIMARY, TIMESTAMP, PRIMARY_TIMESTAMP, Window_, CurrentTime);

            while(true) {
                XEvent xevent;
                XNextEvent(Display_, &xevent);
                switch(xevent.type) {
                case SelectionNotify:
                    std::cout << "SelectionNotify" << std::endl;
                case SelectionClear:

                    std::cout << "SelectionClear" << std::endl;
                case SelectionRequest:

                    std::cout << "SelectionRequest" << std::endl;
                default:
                    std::cout << "Default " << xevent.type << std::endl;
                }
                // handle vent here
            }
        });
    }
    void Clipboard_ManagerImpl::paste(const std::string& text)
    {
        Pasting = true;
        auto targets = XInternAtom(Display_, "TARGETS", 0);
        auto CLIPBOARD = XInternAtom(Display_, "CLIPBOARD", 0);
        XConvertSelection(Display_, CLIPBOARD, targets, CLIPBOARD, Window_, CurrentTime);
    }
}
}