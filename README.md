# clipboard_lite

<p>Cross-platform Clipboard Management library . . . this is made to be lightweight and fast.
see the Test folder for a demo https://github.com/smasherprog/clipboard_lite/blob/master/Test/main.cpp</p>
<h2>No External Dependencies</h2>
<h4>Platforms supported:</h4>
<ul>
<li>Windows XP and Up</li>
<li>MacOS</li>
<li>Linux</li>
</ul>


```c++

  auto clipboard = SL::Clipboard_Lite::CreateClipboard()
    ->onText([](const std::string& text) {
      std::cout << text << std::endl;
    })->onImage([&](const SL::Clipboard_Lite::Image& image) {
        std::cout << "onImage Height=" << image.Height << " Width=" << image.Width << std::endl;
    })->run();
        
    std::string txt = "pasted text";
    clipboard->copy(txt);

```
