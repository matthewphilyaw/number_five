# Platform for Number Five.

Perhaps a few may get where "number five" comes from. I loved Short Circuit as a kid.

This is a first pass at making a custom robotics platform utilizing an RTOS. I'm using FreeRTOS as it's already provided by ST. I'm sure some who will read this will go why "RTOS" and wouldn't see the need.

However, this project is intended to serve as a playfround that I can try out different takes on how to do handle concurrency in a single core MCU. My background is largely in .NET, I'm used to threads and all the "joys" that come along with, so that aspect of an RTOS doesn't bother me. I know there are pain points around sharring resources and things like that and you have code defesensively for it.

Thing is it seems like at somepoint you would be trying to reinvent the wheel, these problems don't go away they just get shifted and the point of this project is to see if FreeRTOS makes easier to split up code into tass. Fully expecting lots of gotchas.

## A few things:
1. CMake is used to generate build system
2. CLion IDE from jetbreans i the primary IDE
3. The ssd1306 code is based on the library from adafruit
    1. Library was ported to straight C
    2. Turned some divides into right shifts
    3. Supports the bear minimum needed to draw text to screen
4. gfx code is based on the Adafruit GFX library
    1. Stripped out everything I did not want
    2. Left the bare minimum to write characters to screen
    3. implement a few functions to print strings and numbers
    
## To build using cmake

```shell
# ensure you have the gnu arm gcc compiler
# then open up var.settings.cmake and replace
# the TOOL_PATH dirictory with the full path to the
# bin directory of the tools.

#in root of repo
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=arm.cmake ..
cd ..
cmake --build build
```
