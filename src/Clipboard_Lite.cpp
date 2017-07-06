#include "Clipboard_Lite.h"
#include "Clipboard.h"
#include <memory>
#include <assert.h>
#include <functional>

namespace SL {
    namespace Clipboard_Lite {

        void Clipboard_Manager::copy(const std::string& text) {
            Impl_->copy(text);
        }

        Clipboard_Configuration Clipboard_Configuration::onText(const std::function<void(const char* data, size_t len)>& handle) {
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


