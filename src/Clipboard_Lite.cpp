#include "Clipboard.h"
#include "Clipboard_Lite.h"
#include <assert.h>
#include <functional>
#include <memory>

namespace SL {
namespace Clipboard_Lite {

    class Clipboard_Configuration : public IClipboard_Configuration {
        std::shared_ptr<Clipboard_Manager> Impl_;

      public:
        Clipboard_Configuration(const std::shared_ptr<Clipboard_Manager> &impl) : Impl_(impl) {}
        virtual ~Clipboard_Configuration() {}
        virtual std::shared_ptr<IClipboard_Configuration> onText(const std::function<void(const std::string &text)> &handle) override
        {
            assert(!Impl_->onText);
            Impl_->onText = handle;
            return std::make_shared<Clipboard_Configuration>(Impl_);
        }
        virtual std::shared_ptr<IClipboard_Configuration> onImage(const std::function<void(const Image &image)> &handle) override
        {
            assert(!Impl_->onImage);
            Impl_->onImage = handle;
            return std::make_shared<Clipboard_Configuration>(Impl_);
        }
        virtual std::shared_ptr<IClipboard_Manager> run() override
        {
            Impl_->run();
            return Impl_;
        }
    };

    std::shared_ptr<IClipboard_Configuration> CreateClipboard()
    {
        return std::make_shared<Clipboard_Configuration>(std::make_shared<Clipboard_Manager>());
    }
} // namespace Clipboard_Lite
} // namespace SL
