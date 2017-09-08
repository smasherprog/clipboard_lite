#pragma once
#include <functional>
#include <memory>

#if defined(WINDOWS) || defined(WIN32)
#if defined(CLIPBOARD_LITE_DLL)
#define CLIPBOARD_LITE_EXTERN __declspec(dllexport)
#define CLIPBOARD_EXPIMP_TEMPLATE
#else
#define CLIPBOARD_LITE_EXTERN
#define CLIPBOARD_EXPIMP_TEMPLATE extern
#endif
#else
#define CLIPBOARD_LITE_EXTERN
#define CLIPBOARD_EXPIMP_TEMPLATE
#endif

namespace SL {
namespace Clipboard_Lite {
    // image data will be in r, g, b format   or r, g, b, a format
    struct Image {
        std::shared_ptr<unsigned char> Data;
        int Height = 0;
        int Width = 0;
        int PixelStride = 0;
    };
    class Clipboard_ManagerImpl;
    class CLIPBOARD_LITE_EXTERN Clipboard_Manager {
        std::shared_ptr<Clipboard_ManagerImpl> Impl_;

      public:
        Clipboard_Manager(const std::shared_ptr<Clipboard_ManagerImpl> &impl) : Impl_(impl) {}
        // copy text into the clipboard
        void copy(const std::string &text);
        // copy an image into the clipboard... image data will be in r, g, b format   or r, g, b, a format
        void copy(const Image &img);
    };
    class CLIPBOARD_LITE_EXTERN IClipboard_Configuration {
      public:
        virtual ~IClipboard_Configuration() {}
        virtual std::shared_ptr<IClipboard_Configuration> onText(const std::function<void(const std::string &text)> &handle) = 0;
        virtual std::shared_ptr<IClipboard_Configuration> onImage(const std::function<void(const Image &image)> &handle) = 0;
        virtual std::shared_ptr<Clipboard_Manager> run() = 0;
    };

    CLIPBOARD_LITE_EXTERN std::shared_ptr<IClipboard_Configuration> CreateClipboard();
} // namespace Clipboard_Lite
} // namespace SL
CLIPBOARD_EXPIMP_TEMPLATE template class CLIPBOARD_LITE_EXTERN std::shared_ptr<SL::Clipboard_Lite::Clipboard_ManagerImpl>;