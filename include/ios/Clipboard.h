#pragma once
#include "Clipboard_Lite.h"
#include <string>
#include <functional>
#include <thread>
#include <atomic>


namespace SL {
    namespace Clipboard_Lite {
        struct Image;
        class Clipboard_Manager : public IClipboard_Manager {
            std::thread BackGroundWorker;
            std::atomic<bool> Copying;
            bool KeepRunning = false;
            int ChangeCount =-1;
        public:

            std::function<void(const std::string& text)> onText;
            std::function<void(const Image& image)> onImage;

            Clipboard_Manager();
           virtual ~Clipboard_Manager();

            void run();
			virtual void copy(const std::string &test) override;
			virtual void copy(const Image &image) override;
        };
    }
}
