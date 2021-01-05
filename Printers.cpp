#include "Printers.h"
#include "Engine/Rocket League/Configuration.h"

namespace Printers
{
    void Print(FILE* file, std::ostringstream& stream)
    {
        fprintf(file, "%s", stream.str().c_str());
        stream.str(std::string());
    }

    void Print(FILE* file, const std::string& string)
    {
        fprintf(file, "%s", string.c_str());
    }

    void EmptyStream(std::ostringstream& stream)
    {
        stream.str(std::string());
    }

    void MakeSpacer(std::ostringstream& stream, uint32_t size)
    {
        stream << std::setfill(' ') << std::setw(size) << std::left;
    }

    void MakeSpacer(std::ostringstream& stream, std::string string, uint32_t size)
    {
        stream << std::setfill(' ') << std::setw(size) << std::left << string;
    }

    void MakeSpacer(std::ostringstream& streamIn, std::ostringstream& streamOut, uint32_t size)
    {
        streamIn << std::setfill(' ') << std::setw(size) << std::left << streamOut.str();
    }

    void MakeDecimal(std::ostringstream& streamIn, uintptr_t address, uint32_t size)
    {
        streamIn << std::dec << std::setfill('0') << std::setw(size) << std::right << address;
    }

    void MakeHex(std::ostringstream& stream, uintptr_t address, uint32_t size)
    {
        stream << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(size) << std::right << address << std::nouppercase;
    }

    void PrintHeader(FILE* file, const std::string& fileName, const std::string& fileExtension, bool pragmaPackPush)
    {
        fprintf(file, "/*\n");
        fprintf(file, "#############################################################################################\n");
        fprintf(file, "# %s (%s) SDK\n", Configuration::GameName.c_str(), Configuration::GameVersion.c_str());
        fprintf(file, "# Generated with the UE3SDKGenerator %s\n", Configuration::GeneratorVersion.c_str());
        fprintf(file, "# ========================================================================================= #\n");
        fprintf(file, "# File: %s.%s\n", fileName.c_str(), fileExtension.c_str());
        fprintf(file, "# ========================================================================================= #\n");
        fprintf(file, "# Credits: %s\n", Configuration::GeneratorCredits.c_str());
        fprintf(file, "# Links: %s\n", Configuration::GeneratorLinks.c_str());
        fprintf(file, "#############################################################################################\n");
        fprintf(file, "*/\n");

        if (fileName != "SdkHeaders")
        {
            if (fileExtension == "h" && fileName != "GameDefines")
            {
                fprintf(file, "#pragma once\n");
                fprintf(file, "#include \"../SdkConstants.h\"\n");
            }
            else if (fileExtension == "cpp" && fileName != "GameDefines")
            {
                fprintf(file, "#include \"../SdkHeaders.h\"\n");
            }
        }

        if (pragmaPackPush)
        {
            fprintf(file, "\n#ifdef _MSC_VER\n");
            fprintf(file, "\t#pragma pack(push, 0x%X)\n", Configuration::Alignment);
            fprintf(file, "#endif\n");
        }
    }


    void PrintSection(FILE* file, const std::string& sectionName)
    {
        fprintf(file, "\n/*\n");
        fprintf(file, "# ========================================================================================= #\n");
        fprintf(file, "# %s\n", sectionName.c_str());
        fprintf(file, "# ========================================================================================= #\n");
        fprintf(file, "*/\n\n");
    }

    void Printers::PrintFooter(FILE* file, bool pragmaPackPop)
    {
        fprintf(file, "/*\n");
        fprintf(file, "# ========================================================================================= #\n");
        fprintf(file, "#\n");
        fprintf(file, "# ========================================================================================= #\n");
        fprintf(file, "*/\n");

        if (pragmaPackPop)
        {
            fprintf(file, "\n#ifdef _MSC_VER\n");
            fprintf(file, "\t#pragma pack(pop)\n");
            fprintf(file, "#endif\n");
        }
    }
}