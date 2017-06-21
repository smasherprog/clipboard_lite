#pragma once
#include <memory>
#include <functional>

namespace SL {
    namespace Clipboard_Lite {

        class Clipboard_ManagerImpl;
        class Clipboard_Manager {
            std::shared_ptr<Clipboard_ManagerImpl> Impl_;
        public:
            Clipboard_Manager(const std::shared_ptr<Clipboard_ManagerImpl>& impl) : Impl_(impl) {}
            Clipboard_Manager() {}

            operator bool() const { return Impl_.operator bool(); }
            //will stop the library from processing frames and release all memory
            void destroy() { Impl_.reset(); }
        };
        class Clipboard_Configuration {
            std::shared_ptr<Clipboard_ManagerImpl> Impl_;
        public:
            Clipboard_Configuration(const std::shared_ptr<Clipboard_ManagerImpl>& impl) : Impl_(impl) {}
  
            Clipboard_Configuration onHTML(const std::function<void(const std::string& html)>& handle);
            Clipboard_Configuration onRTF(const std::function<void(const std::string& html)>& handle);
            Clipboard_Configuration onText(const std::function<void(const std::string& html)>& handle);

            Clipboard_Manager run();
        };
        Clipboard_Configuration CreateClipboard();
    }
}