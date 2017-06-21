#include "Clipboard_Lite.h"
#include "Clipboard.h"
#include <memory>
#include <assert.h>
#include <functional>

namespace SL {
    namespace Clipboard_Lite {

        void Clipboard_Manager::paste_HTML(const std::string& html) {
            Impl_->paste_HTML(html);
        }
        void Clipboard_Manager::paste_RTF(const std::string& rtf) {
            Impl_->paste_RTF(rtf);
        }
        void Clipboard_Manager::paste_Text(const std::string& text) {
            Impl_->paste_Text(text);
        }
    
        Clipboard_Configuration Clipboard_Configuration::onHTML(const std::function<void(const std::string& html)>& handle) {
            assert(!Impl_->onHTML);
            Impl_->onHTML = handle;
            return Clipboard_Configuration(Impl_);

        }
        Clipboard_Configuration Clipboard_Configuration::onRTF(const std::function<void(const std::string& rtf)>& handle) {
            assert(!Impl_->onRTF);
            Impl_->onRTF = handle;
            return Clipboard_Configuration(Impl_);
        }
        Clipboard_Configuration Clipboard_Configuration::onText(const std::function<void(const std::string& text)>& handle) {
            assert(!Impl_->onText);
            Impl_->onText = handle;
            return Clipboard_Configuration(Impl_);
        }
        Clipboard_Manager Clipboard_Configuration::run()
        {
            Impl_->run();
            return Clipboard_Manager(Impl_);
        }    
        Clipboard_Configuration CreateClipboard()
        {
            return Clipboard_Configuration(std::make_shared<Clipboard_ManagerImpl>());
        }
    }
}


