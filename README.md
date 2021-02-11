# UE3SDKGenerator v2.0.7

### About

A  modern C++17 remake of TheFeckless's UE3 SDK Generator.

This project is not fully complete yet, if you encounter any bugs please create a new issue [here](https://github.com/ItsBranK/UE3SDKGenerator/issues).

### Features

- **Accessibility**
Generated sdk is plug and play, just `#include "SdkHeaders.h"` in your project, initialize globals, and you're ready for modding.

- **Global offsets**
Generate an sdk using either offsets or patterns for GObjects and GNames.

- **Constant variables**
Have the option to use constant variables for classes and functions instead of FindClass/FindObject.

- **Process Event options**
Have the option of using virtual voids for Process Event, or just use an index number for UObject's VfTable instead.

- **Custom class alignment**
Full support for both x32 bit and x64 bit games, just change the `Alignment` value in `Configuration.cpp`.

- **Custom spacer widths**
Customize the spacing for comments, constants, structs, enums, functions, classes, and even the log file.

### Configuration

To get started in generating an sdk, copy  and paste the `Rocket League` folder included in the Engine folder and rename it to the game you would like to use. The `GameDefines.h` file will need to be reversed by hand for each game that you want to use. Once you have the necessary classes filled out all that's left to do is make the desired changes in the `Configuration.cpp` file and make sure you have the right `Engine` files included for your game in `dllmain.h`. When you inject compiled dll into your game you will be prompted with a message saying that sdk generation has started, do not close your game until you recieve another message confirming generation is completed.

By default there is one Engine file, Rocket League, included. To create your own just copy the folder and paste it in the Engine folder and modify the #includes in `dllmain.h` as needed. The `GameDefines.h` file is game specific and will need to be changed acordingly. After that you can make your own changes to `Configuration.cpp` and after that you're ready to generate an sdk! Just compile as

### Finalization

There is a [video tutorial](https://www.youtube.com/watch?v=e2igcsT-Yc8) for this project, but as of version 2.0.6 it is slightly outdated so keep that in mind.

Once your sdk has been generated you might need to make a minor change to it. Depending on the game the header files in `SdkHeaders.h` could be placed out of order, if they are make sure to swap it out in the order of `Core` first, then `Engine`; here is an example:

```cpp
#include "GameDefines.h"
#include "SDK_HEADERS\Core_structs.h"
#include "SDK_HEADERS\Core_classes.h"
#include "SDK_HEADERS\Core_parameters.h"
#include "SDK_HEADERS\Engine_structs.h"
#include "SDK_HEADERS\Engine_classes.h"
#include "SDK_HEADERS\Engine_parameters.h"
```

Here is an example of what a generated sdk looks like.

![](https://i.imgur.com/gQhmv34.png)
![](https://i.imgur.com/b3N6MvO.png)
