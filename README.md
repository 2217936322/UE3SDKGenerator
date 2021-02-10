# UE3SDKGenerator v2.0.6
Internal SDK generator for Unreal Engine 3 games.

### About
UE3SDKGenerator is a modern remake of TheFeckless's UE3 SDK Generator. It includes options such as using constant variables, address offsets, enum classes, VfTable for process event or use virtual functions, and more. This generator is not fully complete in terms of being "100% user friendly", but the generation part of it is completed. See usage for more information.

### Usage
By default there is one Engine file, Rocket League, included. To create your own just copy the folder and paste it in the Engine folder and modify the #includes in `dllmain.h` as needed. The `GameDefines.h` file is game specific and will need to be changed acordingly. After that you can make your own changes to `Configuration.cpp` and you're ready to generate an SDK!

### Completed SDK

As of version v2.0.6 the tutorial video for this project is slightly outdated, so keep that in mind!
Tutorial video if you don't feel like reading: https://www.youtube.com/watch?v=e2igcsT-Yc8

Once your SDK is complete you need to make a few changes to the generated files.

If you're using virtual voids for process event `(UsingDetours = false)` you need to comment out "VfTableObject" from the UObject class.

You also need to swap the order of the includes in theSdkHeaders.h file. For example make sure the Core files are placed FIRST after GameDefines.h behind all the other includes like so:

```cpp
#include "GameDefines.h"
#include "SDK_HEADERS\Core_structs.h"
#include "SDK_HEADERS\Core_classes.h"
#include "SDK_HEADERS\Core_parameters.h"
#include "SDK_HEADERS\Engine_structs.h"
#include "SDK_HEADERS\Engine_classes.h"
#include "SDK_HEADERS\Engine_parameters.h"
```

To use your newly generated SDK in your own DLL all you have to do is include SdkHeaders.h and initialize the GObject and GName pointers that are located in the generated GameDefines.h file.

Here is an example of what a generated SDK looks like.

![](https://i.imgur.com/gQhmv34.png)
![](https://i.imgur.com/b3N6MvO.png)
