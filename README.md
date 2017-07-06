# clipboard_lite
<p>Linux/Mac <img src="https://travis-ci.org/smasherprog/clipboard_lite.svg?branch=master" /></p>
<p>Windows <img src="https://ci.appveyor.com/api/projects/status/pmm5snxdiv5pm0uy"/><p>
<h3>Project setup: https://github.com/smasherprog/Projects_Setup</h3>
<p>Cross-platform Clipboard Management library . . . this is made to be lightweight and fast.
see the Exmaple folder for a demo</p>
<h2>No External Dependencies</h2>
<h4>Platforms supported:</h4>
<ul>
<li>Windows XP and Up</li>
<li>MacOS</li>
<li>Linux</li>
</ul>


```c++

  auto clipboard = SL::Clipboard_Lite::CreateClipboard()
          .onText([](const char* string, size_t len) {
        std::cout << "onText" << len<< std::endl;
        std::cout << string << std::endl;
    })
        .run();
    std::string txt = "pasted text";
    clipboard.copy(txt);

```
