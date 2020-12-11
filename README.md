# UE3SDKGenerator
Internal SDK generator for Unreal Engine 3 games.

### About
UE3SDKGenerator is a more modernized & modular remake of TheFeckless's UE3 SDK Generator. It includes options such as using constant variables, address offsets instead of patterns, using enum classes instead of enums, use the VfTable for process event or use virtual functions instead, etc. This generator is not fully complete in terms of being "100% user friendly", but the generation part of it is completed. See usage for more information.

### Usage
By default there is one Engine file, Rocket League, included. To create your own just copy the folder and paste it in the Engine folder. The `GameDefines.h` file is game specific and will need to be changed acordingly. After that you can make your own changes to `Configuration.cpp` as needed, and you're ready to generate an SDK!

Once your SDK is complete you need to make a few changes to the generated files.

If you're using virtual voids for process event (UsingDetours = false) you need to comment out "VfTableObject" from the UObject class.

In your Core_structs.h file you need to delete the generated FPointer struct (as this is already defined in the generated GameDefines.h file), and you also need to swap the order of the includes in the SdkHeaders.h file. For example make sure the Core files are placed FIRST after GameDefines.h behind all the other includes like so:

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
