# Laigter

Laigter is a tool that automatically generates maps for dynamic lighting effects in games. It allows normal, parallax, specular and occlusion map generation. Also, it allows the user to preview how the effect will look ingame.
You can get binaries from itch.io: https://azagaya.itch.io/laigter

Laigter is available in english and spanish. It should automatically detect your PC language.

## Prerequisites

You need Qt framework (online installer is the recommended way to get it) and opencv 3.2 or above. Opencv is not meant for tools like this, but is the one i'm used to. In the future it would be nice to do without it.

## Contributing

Easiest way to contribute is trying laigter and giving me your feedback. Please open an issue when you find a bug, and try to specify the way to reproduce it.
Also, suggestions, feature requests, and new ideas are welcome. Please open issues for all that!

On itch.io, you can download the tool for free, or choose to pay any amount to support the project.

You can also support the development on [patreon](https://www.patreon.com/azagaya), 
or support it by paypal: [![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://paypal.me/PabloFonovich).

Now you can also [![ko-fi](https://www.ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/O5O110W22)

If you wan't to contribute by coding, please contact me and we will see how can we work together! Many things are needed.

## Screenshots

![specular2](https://user-images.githubusercontent.com/46932830/60845793-cbaf6c00-a1b3-11e9-999b-404670185b4c.png)
![captura-piedra-5](https://user-images.githubusercontent.com/46932830/60845822-dcf87880-a1b3-11e9-879c-e909fbd83469.png)
![captura-piedras-9](https://user-images.githubusercontent.com/46932830/60845823-dcf87880-a1b3-11e9-8772-a42180f3abdc.png)
![specular4](https://user-images.githubusercontent.com/46932830/60845825-dcf87880-a1b3-11e9-9f32-45ccc27abe0f.png)


## Compiling on Windows

Compiling on linux is pretty straightforward, just use your package manager to install qtcreator, libopencv-dev and thats it, you can open the .pro file with qtcreator and build or debug laigter. For windows, unfortunately, there are more steps to be done.

First you will need to install qt. I strongly discourage the offline installer, use the web installer instead. In the components selection window you will need to select  qt creator under "qt >> developer and designers tools" and the mingw 7.3 32bit toolchain under "qt >> developer and designers tools >> mingw 7.3.0 32bit" and "qt >> qt 5.13.0 >> mingw 7.3.0 32bit". Do not choose the 64 bit version or chose the msvc toolchain or any other thing.

Then you will need to download and compile opencv 3.2. It must be version 3.2, it cannot be any other version. be warned. But to compile opencv you will need cmake, so download and install that too.

Once you have installed cmake and downloaded opencv 3.2, folow this tutorial here:

https://wiki.qt.io/How_to_setup_Qt_and_openCV_on_Windows

But before clicking "generate" on cmake-gui, be sure to uncheck BUILD_opencv_python3 in case you have this flag and it is checked. I could not compile until I unchecked this. And also change EXECUTABLE_OUTPUT_PATH to C:\opencv-build\install\x86\mingw\bin wich is the path set on laigter.pro (or you could change the path on your laigter.pro to match your EXECUTABLE_OUTPUT_PATH)

After several minutes, fortunately your compilation will be sucessfull. If that's the case, you can open the project on qt-creator, compile and run it. Once you want to deploy the application for windows, you will need the windeployqt command. Something like the following should be enough:

```
windeployqt --dir .\deploy ..\build-laigter-Desktop_Qt_5_13_0_MinGW_32_bit-Release\release\laigter.exe
copy /Y ..\build-laigter-Desktop_Qt_5_13_0_MinGW_32_bit-Release\release\laigter.exe .\deploy\laigter.exe
```