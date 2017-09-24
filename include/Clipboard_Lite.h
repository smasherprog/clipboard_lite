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
    class CLIPBOARD_LITE_EXTERN IClipboard_Manager {
      public:
        virtual ~IClipboard_Manager() {}
        // copy text into the clipboard
        virtual void copy(const std::string &text) = 0;
        // copy an image into the clipboard... image data will be in r, g, b format   or r, g, b, a format
        virtual void copy(const Image &img) = 0;
    };
    class CLIPBOARD_LITE_EXTERN IClipboard_Configuration {
      public:
        virtual ~IClipboard_Configuration() {}
        virtual std::shared_ptr<IClipboard_Configuration> onText(const std::function<void(const std::string &text)> &handle) = 0;
        virtual std::shared_ptr<IClipboard_Configuration> onImage(const std::function<void(const Image &image)> &handle) = 0;
        virtual std::shared_ptr<IClipboard_Manager> run() = 0;
    };

    CLIPBOARD_LITE_EXTERN std::shared_ptr<IClipboard_Configuration> CreateClipboard();
} // namespace Clipboard_Lite
} // namespace SL
