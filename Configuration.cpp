#include "Configuration.h"

namespace Configuration
{
    bool UsingConstants = true; // If you want to use constant variables for everything, instead of FindClass (very efficient, but need to generate new sdk everytime the game updates)
    bool UsingOffset = true; // If you are using offsets or FindPattern to find GObjects & GNames
    bool UsingDetours = true; // If you're goning to be detouring/using the VfTable for process event change this to true, if you're using virtual voids change this to false
    bool UsingEnumClasses = true; // If you want to use enum classes over just normal enums

    int LogSpacer = 15;
    int CommentSpacer = 30;
    int ConstSpacer = 50;
    int EnumSpacer = 50;
    int StructSpacer = 50;
    int FunctionSpacer = 50;
    int ClassSpacer = 50;
    const unsigned char Alignment = 0x8; // 0x4 for x32 bit, 0x8 for x64 bit

    //int ProcessEventIndex = 67;
    const unsigned char* ProcessEventPattern = (const unsigned char*)"(null)";
    const char* ProcessEventMask = (const char*)"(null)";
    //uintptr_t ProcessEventOffset = (uintptr_t)0x327FDA;

    const unsigned char* GObjectsPattern = (const unsigned char*)"(null)";
    const char* GObjectsMask = (const char*)"(null)";
    uintptr_t GObjectsOffset = (uintptr_t)0x2418B68;

    const unsigned char* GNamesPattern = (const unsigned char*)"(null)";
    const char* GNamesMask = (const char*)"(null)";
    uintptr_t GNamesOffset = (uintptr_t)0x2418B20;

    std::string GameName = "Rocket League";
    std::string GameNameShort = "RLSDK";
    std::string GameVersion = "201009.61933.295428";
    std::string GeneratorVersion = "v2.0.1";
    std::string GeneratorCredits = "TheFeckless, ItsBranK";
    std::string GeneratorLinks = "www.github.com/ItsBranK/UE3SDKGenerator, www.twitter.com/ItsBranK";
    std::string GeneratorDirectory = "UE3SDKGenerator";
}
