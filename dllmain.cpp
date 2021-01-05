#include "dllmain.h"
#include "Printers.h"

namespace Utils
{
    MODULEINFO GetModuleInfo(LPCTSTR lpModuleName)
    {
        MODULEINFO miInfos = { NULL };

        HMODULE hmModule = GetModuleHandle(lpModuleName);

        if (hmModule)
            GetModuleInformation(GetCurrentProcess(), hmModule, &miInfos, sizeof(MODULEINFO));

        return miInfos;
    }

    uintptr_t FindPattern(HMODULE module, const unsigned char* pattern, const char* mask)
    {
        MODULEINFO info = { };
        GetModuleInformation(GetCurrentProcess(), module, &info, sizeof(MODULEINFO));

        uintptr_t start = reinterpret_cast<uintptr_t>(module);
        size_t length = info.SizeOfImage;

        size_t pos = 0;
        auto maskLength = std::strlen(mask) - 1;
        auto startAdress = start;
        for (uintptr_t it = startAdress; it < startAdress + length; it++)
        {
            if (*reinterpret_cast<unsigned char*>(it) == pattern[pos] || mask[pos] == '?')
            {
                if (pos == maskLength)
                {
                    return it - maskLength;
                }
                pos++;
            }
            else
            {
                it -= pos;
                pos = 0;
            }
        }

        return -1;
    }

    uintptr_t FindPattern(uintptr_t startAddres, uintptr_t fileSize, const unsigned char* pattern, const char* mask)
    {
        size_t pos = 0;
        int searchLen = strlen(mask) - 1;

        for (uintptr_t retAddress = startAddres; retAddress < startAddres + fileSize; retAddress++)
        {
            if (*reinterpret_cast<unsigned char*>(retAddress) == pattern[pos] || mask[pos] == '?')
            {
                if (mask[pos + 1] == '\0')
                {
                    return (retAddress - searchLen);
                }
                pos++;
            }
            else
            {
                pos = 0;
            }
        }

        return NULL;
    }

    bool MapExists(std::multimap<std::string, std::string>& StrStrMm, std::string sKey, std::string sValue)
    {
        std::pair<std::multimap<std::string, std::string>::iterator, std::multimap<std::string, std::string>::iterator> prRange;
        prRange = StrStrMm.equal_range(sKey);

        for (std::multimap<std::string, std::string>::iterator it = prRange.first; it != prRange.second; ++it)
        {
            if ((*it).second == sValue)
                return true;
        }

        return false;
    }

    bool SortPropertyPair(std::pair<UProperty*, std::string> pPropertyA, std::pair<UProperty*, std::string> pPropertyB)
    {
        if (pPropertyA.first->Offset == pPropertyB.first->Offset && pPropertyA.first->IsA(UBoolProperty::StaticClass()) && pPropertyB.first->IsA(UBoolProperty::StaticClass()))
        {
            return (((UBoolProperty*)pPropertyA.first)->BitMask < ((UBoolProperty*)pPropertyB.first)->BitMask);
        }
        else
        {
            return (pPropertyA.first->Offset < pPropertyB.first->Offset);
        }
    }

    bool SortProperty(UProperty* pPropertyA, UProperty* pPropertyB)
    {
        if (pPropertyA->Offset == pPropertyB->Offset && pPropertyA->IsA(UBoolProperty::StaticClass()) && pPropertyB->IsA(UBoolProperty::StaticClass()))
        {
            return (((UBoolProperty*)pPropertyA)->BitMask < ((UBoolProperty*)pPropertyB)->BitMask);
        }
        else
        {
            return (pPropertyA->Offset < pPropertyB->Offset);
        }
    }

    bool IsBitField(EPropertyTypes propertyType)
    {
        if (propertyType == EPropertyTypes::TYPE_UnsignedChar
            || propertyType == EPropertyTypes::TYPE_UnsignedLong
            || propertyType == EPropertyTypes::TYPE_UnsignedLongLong)
            return true;

        return false;
    }

    bool IsBitField(uint32_t arrayDim)
    {
        if (arrayDim == static_cast<uint32_t>(EPropertyTypes::TYPE_UnsignedChar)
            || arrayDim == static_cast<uint32_t>(EPropertyTypes::TYPE_UnsignedLong)
            || arrayDim == static_cast<uint32_t>(EPropertyTypes::TYPE_UnsignedLongLong))
            return true;

        return false;
    }
}

namespace Retrievers
{
    void GetAllPropertyFlags(std::ostringstream& stream, int propertyFlags)
    {
        bool first = true;

        if (propertyFlags & EPropertyFlags::CPF_Edit) { stream << (first ? "(" : " | ") << "CPF_Edit";							first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Const) { stream << (first ? "(" : " | ") << "CPF_Const";						first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Input) { stream << (first ? "(" : " | ") << "CPF_Input";						first = false; }
        if (propertyFlags & EPropertyFlags::CPF_ExportObject) { stream << (first ? "(" : " | ") << "CPF_ExportObject";			first = false; }
        if (propertyFlags & EPropertyFlags::CPF_OptionalParm) { stream << (first ? "(" : " | ") << "CPF_OptionalParm";			first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Net) { stream << (first ? "(" : " | ") << "CPF_Net";							first = false; }
        if (propertyFlags & EPropertyFlags::CPF_EditConstArray) { stream << (first ? "(" : " | ") << "CPF_EditConstArray";		first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Parm) { stream << (first ? "(" : " | ") << "CPF_Parm";							first = false; }
        if (propertyFlags & EPropertyFlags::CPF_OutParm) { stream << (first ? "(" : " | ") << "CPF_OutParm";					first = false; }
        if (propertyFlags & EPropertyFlags::CPF_SkipParm) { stream << (first ? "(" : " | ") << "CPF_SkipParm";					first = false; }
        if (propertyFlags & EPropertyFlags::CPF_ReturnParm) { stream << (first ? "(" : " | ") << "CPF_ReturnParm";				first = false; }
        if (propertyFlags & EPropertyFlags::CPF_CoerceParm) { stream << (first ? "(" : " | ") << "CPF_CoerceParm";				first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Native) { stream << (first ? "(" : " | ") << "CPF_Native";						first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Transient) { stream << (first ? "(" : " | ") << "CPF_Transient";				first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Config) { stream << (first ? "(" : " | ") << "CPF_Config";						first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Localized) { stream << (first ? "(" : " | ") << "CPF_Localized";				first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Travel) { stream << (first ? "(" : " | ") << "CPF_Travel";						first = false; }
        if (propertyFlags & EPropertyFlags::CPF_EditConst) { stream << (first ? "(" : " | ") << "CPF_EditConst";				first = false; }
        if (propertyFlags & EPropertyFlags::CPF_GlobalConfig) { stream << (first ? "(" : " | ") << "CPF_GlobalConfig";			first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Component) { stream << (first ? "(" : " | ") << "CPF_Component";				first = false; }
        if (propertyFlags & EPropertyFlags::CPF_NeedCtorLink) { stream << (first ? "(" : " | ") << "CPF_NeedCtorLink";			first = false; }
        if (propertyFlags & EPropertyFlags::CPF_NoExport) { stream << (first ? "(" : " | ") << "CPF_NoExport";					first = false; }
        if (propertyFlags & EPropertyFlags::CPF_NoClear) { stream << (first ? "(" : " | ") << "CPF_NoClear";					first = false; }
        if (propertyFlags & EPropertyFlags::CPF_EditInline) { stream << (first ? "(" : " | ") << "CPF_EditInline";				first = false; }
        if (propertyFlags & EPropertyFlags::CPF_EdFindable) { stream << (first ? "(" : " | ") << "CPF_EdFindable";				first = false; }
        if (propertyFlags & EPropertyFlags::CPF_EditInlineUse) { stream << (first ? "(" : " | ") << "CPF_EditInlineUse";		first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Deprecated) { stream << (first ? " " : " | ") << "CPF_Deprecated";				first = false; }
        if (propertyFlags & EPropertyFlags::CPF_EditInlineNotify) { stream << (first ? "(" : " | ") << "CPF_EditInlineNotify";	first = false; }
        if (propertyFlags & EPropertyFlags::CPF_RepNotify) { stream << (first ? "(" : " | ") << "CPF_RepNotify";				first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Interp) { stream << (first ? "(" : " | ") << "CPF_Interp";						first = false; }
        if (propertyFlags & EPropertyFlags::CPF_NonTransactional) { stream << (first ? "(" : " | ") << "CPF_NonTransactional";	first = false; }
        if (propertyFlags & EPropertyFlags::CPF_EditorOnly) { stream << (first ? "(" : " | ") << "CPF_EditorOnly";	            first = false; }
        if (propertyFlags & EPropertyFlags::CPF_NoDestructor) { stream << (first ? "(" : " | ") << "CPF_NoDestructor";	        first = false; }
        if (propertyFlags & EPropertyFlags::CPF_AutoWeak) { stream << (first ? "(" : " | ") << "CPF_AutoWeak";	                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_ContainsInstancedReference) { stream << (first ? "(" : " | ") << "CPF_ContainsInstancedReference";	first = false; }
        if (propertyFlags & EPropertyFlags::CPF_SimpleDisplay) { stream << (first ? "(" : " | ") << "CPF_SimpleDisplay";	    first = false; }
        if (propertyFlags & EPropertyFlags::CPF_AdvancedDisplay) { stream << (first ? "(" : " | ") << "CPF_AdvancedDisplay";	first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Protected) { stream << (first ? "(" : " | ") << "CPF_Protected";	            first = false; }
        if (propertyFlags & EPropertyFlags::CPF_BlueprintCallable) { stream << (first ? "(" : " | ") << "CPF_BlueprintCallable"; first = false; }
        if (propertyFlags & EPropertyFlags::CPF_BlueprintAuthorityOnly) { stream << (first ? "(" : " | ") << "CPF_BlueprintAuthorityOnly"; first = false; }
        if (propertyFlags & EPropertyFlags::CPF_TextExportTransient) { stream << (first ? "(" : " | ") << "CPF_TextExportTransient"; first = false; }
        if (propertyFlags & EPropertyFlags::CPF_NonPIEDuplicateTransient) { stream << (first ? "(" : " | ") << "CPF_NonPIEDuplicateTransient";	first = false; }
        if (propertyFlags & EPropertyFlags::CPF_ExposeOnSpawn) { stream << (first ? "(" : " | ") << "CPF_ExposeOnSpawn";	    first = false; }
        if (propertyFlags & EPropertyFlags::CPF_PersistentInstance) { stream << (first ? "(" : " | ") << "CPF_PersistentInstance"; first = false; }
        if (propertyFlags & EPropertyFlags::CPF_UObjectWrapper) { stream << (first ? "(" : " | ") << "CPF_UObjectWrapper";	    first = false; }
        if (propertyFlags & EPropertyFlags::CPF_HasGetValueTypeHash) { stream << (first ? "(" : " | ") << "CPF_HasGetValueTypeHash"; first = false; }
        if (propertyFlags & EPropertyFlags::CPF_NativeAccessSpecifierPublic) { stream << (first ? "(" : " | ") << "CPF_NativeAccessSpecifierPublic"; first = false; }
        if (propertyFlags & EPropertyFlags::CPF_NativeAccessSpecifierProtected) { stream << (first ? "(" : " | ") << "CPF_NativeAccessSpecifierProtected"; first = false; }
        if (propertyFlags & EPropertyFlags::CPF_NativeAccessSpecifierPrivate) { stream << (first ? "(" : " | ") << "CPF_NativeAccessSpecifierPrivate"; first = false; }
        if (propertyFlags & EPropertyFlags::CPF_SkipSerialization) { stream << (first ? "(" : " | ") << "CPF_SkipSerialization"; first = false; }

        if (!first)
            stream << ")";
    }

    void GetAllFunctionFlags(std::ostringstream& stream, int functionFlags)
    {
        bool first = true;

        if (functionFlags & EFunctionFlags::FUNC_None) { stream << (first ? "(" : " | ") << "FUNC_None";						first = false; }
        if (functionFlags & EFunctionFlags::FUNC_Final) { stream << (first ? "(" : " | ") << "FUNC_Final";						first = false; }
        if (functionFlags & EFunctionFlags::FUNC_RequiredAPI) { stream << (first ? "(" : " | ") << "FUNC_RequiredAPI";			first = false; }
        if (functionFlags & EFunctionFlags::FUNC_BlueprintAuthorityOnly) { stream << (first ? "(" : " | ") << "FUNC_BlueprintAuthorityOnly"; first = false; }
        if (functionFlags & EFunctionFlags::FUNC_BlueprintCosmetic) { stream << (first ? "(" : " | ") << "FUNC_BlueprintCosmetic"; first = false; }
        if (functionFlags & EFunctionFlags::FUNC_Net) { stream << (first ? "(" : " | ") << "FUNC_Net";					    	first = false; }
        if (functionFlags & EFunctionFlags::FUNC_NetReliable) { stream << (first ? "(" : " | ") << "FUNC_NetReliable";			first = false; }
        if (functionFlags & EFunctionFlags::FUNC_NetRequest) { stream << (first ? "(" : " | ") << "FUNC_NetRequest";			first = false; }
        if (functionFlags & EFunctionFlags::FUNC_Exec) { stream << (first ? "(" : " | ") << "FUNC_Exec";						first = false; }
        if (functionFlags & EFunctionFlags::FUNC_Native) { stream << (first ? "(" : " | ") << "FUNC_Native";					first = false; }
        if (functionFlags & EFunctionFlags::FUNC_Event) { stream << (first ? "(" : " | ") << "FUNC_Event";						first = false; }
        if (functionFlags & EFunctionFlags::FUNC_NetResponse) { stream << (first ? "(" : " | ") << "FUNC_NetResponse";			first = false; }
        if (functionFlags & EFunctionFlags::FUNC_Static) { stream << (first ? "(" : " | ") << "FUNC_Static";					first = false; }
        if (functionFlags & EFunctionFlags::FUNC_NetMulticast) { stream << (first ? "(" : " | ") << "FUNC_NetMulticast";		first = false; }
        if (functionFlags & EFunctionFlags::FUNC_UbergraphFunction) { stream << (first ? "(" : " | ") << "FUNC_UbergraphFunction"; first = false; }
        if (functionFlags & EFunctionFlags::FUNC_MulticastDelegate) { stream << (first ? "(" : " | ") << "FUNC_MulticastDelegate"; first = false; }
        if (functionFlags & EFunctionFlags::FUNC_Public) { stream << (first ? "(" : " | ") << "FUNC_Public";					first = false; }
        if (functionFlags & EFunctionFlags::FUNC_Private) { stream << (first ? "(" : " | ") << "FUNC_Private";					first = false; }
        if (functionFlags & EFunctionFlags::FUNC_Protected) { stream << (first ? "(" : " | ") << "FUNC_Protected";				first = false; }
        if (functionFlags & EFunctionFlags::FUNC_Delegate) { stream << (first ? "(" : " | ") << "FUNC_Delegate";				first = false; }
        if (functionFlags & EFunctionFlags::FUNC_NetServer) { stream << (first ? "(" : " | ") << "FUNC_NetServer";				first = false; }
        if (functionFlags & EFunctionFlags::FUNC_HasOutParms) { stream << (first ? "(" : " | ") << "FUNC_HasOutParms";			first = false; }
        if (functionFlags & EFunctionFlags::FUNC_HasDefaults) { stream << (first ? "(" : " | ") << "FUNC_HasDefaults";			first = false; }
        if (functionFlags & EFunctionFlags::FUNC_NetClient) { stream << (first ? "(" : " | ") << "FUNC_NetClient";				first = false; }
        if (functionFlags & EFunctionFlags::FUNC_DLLImport) { stream << (first ? "(" : " | ") << "FUNC_DLLImport";				first = false; }
        if (functionFlags & EFunctionFlags::FUNC_BlueprintCallable) { stream << (first ? "(" : " | ") << "FUNC_BlueprintCallable"; first = false; }
        if (functionFlags & EFunctionFlags::FUNC_BlueprintEvent) { stream << (first ? "(" : " | ") << "FUNC_BlueprintEvent";	first = false; }
        if (functionFlags & EFunctionFlags::FUNC_BlueprintPure) { stream << (first ? "(" : " | ") << "FUNC_BlueprintPure";		first = false; }
        if (functionFlags & EFunctionFlags::FUNC_EditorOnly) { stream << (first ? "(" : " | ") << "FUNC_EditorOnly";			first = false; }
        if (functionFlags & EFunctionFlags::FUNC_Const) { stream << (first ? "(" : " | ") << "FUNC_Const";						first = false; }
        if (functionFlags & EFunctionFlags::FUNC_NetValidate) { stream << (first ? "(" : " | ") << "FUNC_NetValidate";			first = false; }
        if (functionFlags & EFunctionFlags::FUNC_AllFlags) { stream << (first ? "(" : " | ") << "FUNC_AllFlags";				first = false; }

        if (!first)
            stream << ")";
    }

    EPropertyTypes GetPropertyType(UProperty* uProperty, std::string& sPropertyType, bool returnFunction)
    {
        if (!uProperty)
            return EPropertyTypes::TYPE_UnknownData;

        if (uProperty->IsA(UStructProperty::StaticClass()))
        {
            uint32_t propertyCount = UObject::CountObject<UScriptStruct>(((UStructProperty*)uProperty)->Struct->GetName());

            if (propertyCount > 1) {
                sPropertyType = "struct " + Generator::GenerateValidName(std::string(((UStructProperty*)uProperty)->Struct->Outer->GetNameCPP())) + "_" + Generator::GenerateValidName(std::string(((UStructProperty*)uProperty)->Struct->GetNameCPP()));
            }
            else
            {
                sPropertyType = "struct " + Generator::GenerateValidName(std::string(((UStructProperty*)uProperty)->Struct->GetNameCPP()));
            }

            return EPropertyTypes::TYPE_Struct;
        }
        else if (uProperty->IsA(UStrProperty::StaticClass())) { sPropertyType = "struct FString"; return EPropertyTypes::TYPE_FString; }
        else if (uProperty->IsA(UQWordProperty::StaticClass())) { sPropertyType = "unsigned long long"; return EPropertyTypes::TYPE_UnsignedLongLong; }
        /*else if (pProperty->IsA(UComponentProperty::StaticClass())) // I was just testing shit here, don't uncomment this.
        {
        std::string sPropertyValue1;
        std::string sPropertyValue2;

        if (GetPropertyType(((UComponentProperty*)pProperty)->Value1, sPropertyValue1) && GetPropertyType(((UComponentProperty*)pProperty)->Value2, sPropertyValue2))
        {
        sPropertyType = "Value1(" + sPropertyValue1 + "), Value2(" + sPropertyValue2 + ")";
        return TYPE_TMap;
        }

        return TYPE_UnknownData;
        }*/
        else if (uProperty->IsA(UObjectProperty::StaticClass())) { sPropertyType = "class " + Generator::GenerateValidName(std::string(((UObjectProperty*)uProperty)->PropertyClass->GetNameCPP())) + "*"; return EPropertyTypes::TYPE_Pointer; }
        else if (uProperty->IsA(UClassProperty::StaticClass())) { sPropertyType = "class " + Generator::GenerateValidName(std::string(((UClassProperty*)uProperty)->MetaClass->GetNameCPP())) + "*"; return EPropertyTypes::TYPE_Pointer; }
        else if (uProperty->IsA(UNameProperty::StaticClass())) { sPropertyType = "struct FName"; return EPropertyTypes::TYPE_FName; }
        else if (uProperty->IsA(UMapProperty::StaticClass())) {
            std::string sPropertyTypeKey;
            std::string sPropertyTypeValue;

            if (GetPropertyType(((UMapProperty*)uProperty)->Key, sPropertyTypeKey, returnFunction) != EPropertyTypes::TYPE_UnknownData && GetPropertyType(((UMapProperty*)uProperty)->Value, sPropertyTypeValue, returnFunction) != EPropertyTypes::TYPE_UnknownData)
            {
                sPropertyType = "TMap<" + sPropertyTypeKey + ", " + sPropertyTypeValue + ">";
                return EPropertyTypes::TYPE_TMap;
            }
            else
            {
                return EPropertyTypes::TYPE_UnknownData;
            }
        }
        else if (uProperty->IsA(UIntProperty::StaticClass())) { sPropertyType = "int"; return EPropertyTypes::TYPE_Int; }
        else if (uProperty->IsA(UInterfaceProperty::StaticClass())) { sPropertyType = "class " + Generator::GenerateValidName(std::string(((UInterfaceProperty*)uProperty)->InterfaceClass->GetNameCPP())) + "*"; return EPropertyTypes::TYPE_Pointer; }
        else if (uProperty->IsA(UFloatProperty::StaticClass())) { sPropertyType = "float"; return EPropertyTypes::TYPE_Float; }
        else if (uProperty->IsA(UDelegateProperty::StaticClass())) { sPropertyType = "struct FScriptDelegate"; return EPropertyTypes::TYPE_FScriptDelegate; }
        else if (uProperty->IsA(UByteProperty::StaticClass())) { sPropertyType = "unsigned char"; return EPropertyTypes::TYPE_UnsignedChar; }
        else if (uProperty->IsA(UBoolProperty::StaticClass()))
        {
            if (returnFunction)
            {
                sPropertyType = "bool";
                return EPropertyTypes::TYPE_Bool;
            }
            else
            {
                sPropertyType = "unsigned long";
                return EPropertyTypes::TYPE_UnsignedLong;
            }
        }
        else if (uProperty->IsA(UArrayProperty::StaticClass()))
        {
            std::string sPropertyTypeInner;

            if (GetPropertyType(((UArrayProperty*)uProperty)->Inner, sPropertyTypeInner, returnFunction) != EPropertyTypes::TYPE_UnknownData)
            {
                sPropertyType = "TArray<" + sPropertyTypeInner + ">";
                return EPropertyTypes::TYPE_TArray;
            }
            else
            {
                return EPropertyTypes::TYPE_UnknownData;
            }
        }

        else { return EPropertyTypes::TYPE_UnknownData; }
    }

    size_t GetPropertySize(UProperty* uProperty)
    {
        if (!uProperty)
            return 0;

        if (uProperty->IsA(UStructProperty::StaticClass())) { return uProperty->ElementSize; }
        else if (uProperty->IsA(UStrProperty::StaticClass())) { return sizeof(FString); }
        else if (uProperty->IsA(UQWordProperty::StaticClass())) { return sizeof(unsigned long long); }
        //else if (uProperty->IsA(UComponentProperty::StaticClass())) { return sizeof(void*); }
        else if (uProperty->IsA(UObjectProperty::StaticClass())) { return sizeof(void*); }
        else if (uProperty->IsA(UClassProperty::StaticClass())) { return sizeof(void*); }
        else if (uProperty->IsA(UNameProperty::StaticClass())) { return sizeof(FName); }
        else if (uProperty->IsA(UMapProperty::StaticClass())) { return 0x3C; }
        else if (uProperty->IsA(UIntProperty::StaticClass())) { return sizeof(int); }
        else if (uProperty->IsA(UInterfaceProperty::StaticClass())) { return sizeof(void*); }
        else if (uProperty->IsA(UFloatProperty::StaticClass())) { return sizeof(float); }
        else if (uProperty->IsA(UDelegateProperty::StaticClass())) { return sizeof(FScriptDelegate); }
        else if (uProperty->IsA(UByteProperty::StaticClass())) { return sizeof(unsigned char); }
        else if (uProperty->IsA(UBoolProperty::StaticClass())) { return sizeof(unsigned long); }
        else if (uProperty->IsA(UArrayProperty::StaticClass())) { return sizeof(TArray<void*>); }
        else { return 0; }
    }
}

namespace StructGenerator
{
    UScriptStruct* FindLargestStruct(std::string structFullName)
    {
        unsigned long propertySize = 0;
        UScriptStruct* largestStruct = nullptr;

        for (int i = 0; i < UObject::GObjObjects()->Num(); i++)
        {
            UObject* uObject = UObject::GObjObjects()->Data[i];
            if (!uObject || !uObject->IsA(UScriptStruct::StaticClass()))
                continue;

            if (structFullName == std::string(uObject->GetFullName()))
            {
                UScriptStruct* uStruct = (UScriptStruct*)uObject;
                if (uStruct->PropertySize >= propertySize)
                {
                    largestStruct = uStruct;
                    propertySize = uStruct->PropertySize;
                }
            }
        }

        return largestStruct;
    }

    void GenerateStruct(FILE* file, UScriptStruct* uScriptStruct)
    {
        std::ostringstream structStream;
        std::ostringstream propertyStream;
        std::ostringstream flagStream;

        std::string structFullName = std::string(uScriptStruct->GetFullName());
        std::string structName = Generator::GenerateValidName(std::string(uScriptStruct->GetName()));
        std::string structNameCPP = Generator::GenerateValidName(std::string(uScriptStruct->GetNameCPP()));
        std::string structOuterNameCPP = Generator::GenerateValidName(std::string(uScriptStruct->Outer->GetNameCPP()));

        //fprintf(pLog, "ScriptStruct: %-50s - Instance: 0x%" PRIxPTR "\n", sSStructName.c_str(), uScriptStruct);

        structStream << "// " << structFullName << "\n";

        uintptr_t size = 0;
        uintptr_t lastOffset = 0;
        uintptr_t missedOffset = 0;

        UScriptStruct* uSuperField = (UScriptStruct*)uScriptStruct->SuperField;

        char charBuffer[256] = { NULL };
        strcpy_s(charBuffer, structName.c_str());
        uint32_t structCount = UObject::CountObject<UScriptStruct>(charBuffer);

        if (uSuperField && uSuperField != uScriptStruct)
        {
            size = uScriptStruct->PropertySize - uSuperField->PropertySize;
            lastOffset = uSuperField->PropertySize;

            std::string fieldName = Generator::GenerateValidName(std::string(uSuperField->GetName()));
            std::string fieldNameCPP = Generator::GenerateValidName(std::string(uSuperField->GetNameCPP()));
            std::string fieldOuterNameCPP = Generator::GenerateValidName(std::string(uSuperField->Outer->GetNameCPP()));

            strcpy_s(charBuffer, fieldName.c_str());
            uint32_t fieldStructCount = UObject::CountObject<UScriptStruct>(charBuffer);

            structStream << "// ";
            Printers::MakeHex(structStream, size, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
            structStream << "(";
            Printers::MakeHex(structStream, uScriptStruct->PropertySize, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
            structStream << " - ";
            Printers::MakeHex(structStream, uSuperField->PropertySize, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
            structStream << ")\n";

            if (structCount > 1)
            {
                structStream << "struct " << structOuterNameCPP << "_" << structNameCPP << " : ";
            }
            else
            {
                structStream << "struct " << structNameCPP << " : ";
            }

            if (fieldStructCount > 1)
            {
                structStream << fieldOuterNameCPP << "_" << fieldNameCPP << "\n";
            }
            else
            {
                structStream << fieldNameCPP << "\n";
            }
        }
        else
        {
            size = uScriptStruct->PropertySize;

            structStream << "// ";
            Printers::MakeHex(structStream, size, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
            structStream << "\n";

            if (structCount > 1)
            {
                structStream << "struct " << structOuterNameCPP << "_" << structNameCPP << "\n";
            }
            else
            {
                structStream << "struct " << structNameCPP << "\n";
            }
        }

        structStream << "{\n";

        std::vector<UProperty*> vProperty;
        for (UProperty* uProperty = (UProperty*)uScriptStruct->Children; uProperty; uProperty = (UProperty*)uProperty->Next)
        {
            if (uProperty->ElementSize > 0 && !uProperty->IsA(UScriptStruct::StaticClass()))
                vProperty.push_back(uProperty);
        }

        std::sort(vProperty.begin(), vProperty.end(), Utils::SortProperty);

        std::map<std::string, uint32_t> propertyNameMap;
        uint32_t unknownDataIndex = 0;

        for (size_t i = 0; i < vProperty.size(); i++)
        {
            UProperty* uProperty = vProperty[i];

            if (lastOffset < uProperty->Offset)
            {
                missedOffset = uProperty->Offset - lastOffset;

                if (missedOffset >= Configuration::Alignment)
                {
                    propertyStream << "UnknownData";
                    Printers::MakeDecimal(propertyStream, unknownDataIndex, static_cast<uint32_t>(EWidthTypes::WIDTH_Decimal));
                    propertyStream << "[";
                    Printers::MakeHex(propertyStream, missedOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_SpecialCase));
                    propertyStream << "];";

                    structStream << "\t";
                    Printers::MakeSpacer(structStream, "unsigned char", Configuration::StructSpacer);
                    structStream << " ";
                    Printers::MakeSpacer(structStream, propertyStream, Configuration::StructSpacer);
                    structStream << "\t\t// ";
                    Printers::MakeHex(structStream, lastOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                    structStream << " (";
                    Printers::MakeHex(structStream, missedOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                    structStream << ") MISSED OFFSET\n";

                    Printers::EmptyStream(propertyStream);

                    unknownDataIndex++;
                }
            }

            std::string propertyType;

            if (Retrievers::GetPropertyType(uProperty, propertyType, false) != EPropertyTypes::TYPE_UnknownData)
            {
                size_t dwCorrectElementSize = Retrievers::GetPropertySize(uProperty);

                std::string propertyName = Generator::GenerateValidName(std::string(uProperty->GetName()));

                if (propertyNameMap.count(propertyName) == 0) {
                    propertyNameMap[propertyName] = 1;
                    propertyStream << propertyName;
                }
                else
                {
                    propertyStream << propertyName;
                    Printers::MakeDecimal(propertyStream, propertyNameMap[propertyName], static_cast<uint32_t>(EWidthTypes::WIDTH_Decimal));
                    propertyNameMap[propertyName]++;
                }

                if (uProperty->ArrayDim > 1)
                {
                    propertyStream << "[";
                    Printers::MakeHex(propertyStream, uProperty->ArrayDim, static_cast<uint32_t>(EWidthTypes::WIDTH_SpecialCase));
                    propertyStream << "]";
                }

                if (uProperty->IsA(UBoolProperty::StaticClass()))
                    propertyStream << " : 1";

                propertyStream << ";";

                Retrievers::GetAllPropertyFlags(flagStream, uProperty->PropertyFlags.A);

                structStream << "\t";
                Printers::MakeSpacer(structStream, propertyType, Configuration::StructSpacer);
                structStream << " ";
                Printers::MakeSpacer(structStream, propertyStream, Configuration::StructSpacer);
                structStream << "\t\t// ";
                Printers::MakeHex(structStream, uProperty->Offset, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                structStream << " (";
                Printers::MakeHex(structStream, (uProperty->ElementSize* uProperty->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                structStream << ") [";
                Printers::MakeHex(structStream, uProperty->PropertyFlags.A, static_cast<uint32_t>(EWidthTypes::WIDTH_PropertyFlags));
                structStream << "] ";

                if (uProperty->IsA(UBoolProperty::StaticClass()))
                {
                    structStream << "[";
                    Printers::MakeHex(structStream, ((UBoolProperty*)uProperty)->BitMask, static_cast<uint32_t>(EWidthTypes::WIDTH_BitMask));
                    structStream << "] ";
                }
                else
                {
                    Printers::MakeSpacer(structStream, 14);
                }

                structStream << flagStream.str() << "\n";

                Printers::EmptyStream(propertyStream);
                Printers::EmptyStream(flagStream);

                int offsetError = (uProperty->ElementSize * uProperty->ArrayDim) - (dwCorrectElementSize * uProperty->ArrayDim);

                if (offsetError > 0)
                {
                    propertyStream << "UnknownData";
                    Printers::MakeDecimal(propertyStream, unknownDataIndex, static_cast<uint32_t>(EWidthTypes::WIDTH_Decimal));
                    propertyStream << "[";
                    Printers::MakeHex(propertyStream, offsetError, static_cast<uint32_t>(EWidthTypes::WIDTH_SpecialCase));
                    propertyStream<< "];";

                    structStream << "\t";
                    Printers::MakeSpacer(structStream, "unsigned char", Configuration::StructSpacer);
                    structStream << " ";
                    Printers::MakeSpacer(structStream, propertyStream, Configuration::StructSpacer);
                    structStream << "\t\t// ";
                    Printers::MakeHex(structStream, (uProperty->Offset + offsetError), static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                    structStream << " (";
                    Printers::MakeHex(structStream, offsetError, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                    structStream << ") FIX WRONG SIZE OF PREVIOUS PROPERTY\n";

                    Printers::EmptyStream(propertyStream);

                    unknownDataIndex++;
                }
            }
            else
            {
                propertyStream << "UnknownData";
                Printers::MakeDecimal(propertyStream, unknownDataIndex, static_cast<uint32_t>(EWidthTypes::WIDTH_Decimal));
                propertyStream << "[";
                Printers::MakeHex(propertyStream, (uProperty->ElementSize * uProperty->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_SpecialCase));
                propertyStream << "];";

                structStream << "\t";
                Printers::MakeSpacer(structStream, "unsigned char", Configuration::StructSpacer);
                structStream << " ";
                Printers::MakeSpacer(structStream, propertyStream, Configuration::StructSpacer);
                structStream << "\t\t//";
                Printers::MakeHex(structStream, uProperty->Offset, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                structStream << " (";
                Printers::MakeHex(structStream, (uProperty->ElementSize * uProperty->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                structStream << ") UNKNOWN PROPERTY: " << std::string(uProperty->GetFullName()) << "\n";

                Printers::EmptyStream(propertyStream);

                unknownDataIndex++;
            }

            uintptr_t offsetValue = uProperty->Offset + (uProperty->ElementSize * uProperty->ArrayDim);
            lastOffset = offsetValue;
        }

        if (lastOffset < uScriptStruct->PropertySize)
        {
            missedOffset = uScriptStruct->PropertySize - lastOffset;

            if (missedOffset >= Configuration::Alignment)
            {
                propertyStream << "UnknownData";
                Printers::MakeDecimal(propertyStream, unknownDataIndex, static_cast<uint32_t>(EWidthTypes::WIDTH_Decimal));
                propertyStream << "[";
                Printers::MakeHex(propertyStream, missedOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_SpecialCase));
                propertyStream << "];";

                structStream << "\t";
                Printers::MakeSpacer(structStream, "unsigned char", Configuration::StructSpacer);
                structStream << " ";
                Printers::MakeSpacer(structStream, propertyStream, Configuration::StructSpacer);
                structStream << "\t\t// ";
                Printers::MakeHex(propertyStream, lastOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                structStream << " (";
                Printers::MakeHex(structStream, missedOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                structStream << ") MISSED OFFSET\n";

                Printers::EmptyStream(propertyStream);
            }
        }

        structStream << "};\n\n";

        Printers::Print(file, structStream);
    }

    void GenerateStructProperties(FILE* file, UScriptStruct* uScriptStruct, UObject* uPackageObj)
    {
        UObject* uPackageObject = uScriptStruct->GetPackageObj();

        if (!uPackageObject || uPackageObject != uPackageObj)
            return;

        static std::vector<std::string> generatedStructs;

        std::string structName = Generator::GenerateValidName(std::string(uScriptStruct->GetName()));
        std::string structFullName = std::string(uScriptStruct->GetFullName());

        if (structName.find("Default__") != std::string::npos)
            return;

        if (structName.find("<uninitialized>") != std::string::npos)
            return;

        std::string propertyType;

        if (find(generatedStructs.begin(), generatedStructs.end(), structFullName) == generatedStructs.end())
        {
            uScriptStruct = FindLargestStruct(structFullName);

            if (uScriptStruct->SuperField && uScriptStruct->SuperField != uScriptStruct && std::find(generatedStructs.begin(), generatedStructs.end(), std::string(((UScriptStruct*)(uScriptStruct->SuperField))->GetFullName())) == generatedStructs.end())
                GenerateStructProperties(file, (UScriptStruct*)(uScriptStruct->SuperField), uPackageObj);

            for (UProperty* pStructProperty = (UProperty*)uScriptStruct->Children; pStructProperty; pStructProperty = (UProperty*)pStructProperty->Next)
            {
                EPropertyTypes iTypeResult = Retrievers::GetPropertyType(pStructProperty, propertyType, false);

                if (iTypeResult == EPropertyTypes::TYPE_Struct
                    && (UScriptStruct*)(((UStructProperty*)pStructProperty)->Struct) != uScriptStruct
                    && std::find(generatedStructs.begin(), generatedStructs.end(), std::string(((UScriptStruct*)(((UStructProperty*)pStructProperty)->Struct))->GetFullName())) == generatedStructs.end()
                    )
                {
                    GenerateStructProperties(file, (UScriptStruct*)(((UStructProperty*)pStructProperty)->Struct), uPackageObj);
                }

                if (iTypeResult == EPropertyTypes::TYPE_TArray
                    && Retrievers::GetPropertyType(((UArrayProperty*)pStructProperty)->Inner, propertyType, false) == EPropertyTypes::TYPE_Struct
                    && (UScriptStruct*)(((UStructProperty*)((UArrayProperty*)pStructProperty)->Inner)->Struct) != uScriptStruct
                    && find(generatedStructs.begin(), generatedStructs.end(), std::string(((UScriptStruct*)(((UStructProperty*)((UArrayProperty*)pStructProperty)->Inner)->Struct))->GetFullName())) == generatedStructs.end()
                    )
                {
                    GenerateStructProperties(file, (UScriptStruct*)(((UStructProperty*)((UArrayProperty*)pStructProperty)->Inner)->Struct), uPackageObj);
                }
            }

            GenerateStruct(file, uScriptStruct);
            generatedStructs.push_back(structFullName);
        }
    }

    void ProcessStructs(FILE* file, UObject* uPackageObj)
    {
        for (int i = 0; i < UObject::GObjObjects()->Num(); i++)
        {
            UObject* uObject = UObject::GObjObjects()->Data[i];
            if (!uObject)
                continue;

            UObject* uPackageObject = uObject->GetPackageObj();
            if (!uPackageObject)
                continue;

            if (uPackageObject == uPackageObj && uObject->IsA(UScriptStruct::StaticClass()))
                GenerateStructProperties(file, (UScriptStruct*)uObject, uPackageObject);
        }
    }
}

namespace ConstGenerator
{
    void GenerateConst(FILE* file, UConst* uConst)
    {
        std::ostringstream constStream;
        std::ostringstream valueStream;

        static std::multimap<std::string, std::string> nameValueMap;

        std::string constName = Generator::GenerateValidName(std::string(uConst->GetName()));

        if (constName.find("Default__") != std::string::npos)
            return;

        //fprintf(pLog, "Const:        %-50s - Instance: 0x%" PRIxPTR "\n", constName.c_str(), uConst);

        std::string constValue = uConst->Value.ToString();
        size_t mapSize = nameValueMap.count(constName);

        if (mapSize == 0)
        {
            nameValueMap.insert(std::make_pair(constName, constValue));

            constStream << "#define CONST_";
            Printers::MakeSpacer(constStream, constName, Configuration::ConstSpacer);
            constStream << " " << constValue << "\n";

            Printers::Print(file, constStream);
        }
        else if (!Utils::MapExists(nameValueMap, constName, constValue))
        {
            nameValueMap.insert(std::make_pair(constName, constValue));

            valueStream << constName;
            Printers::MakeDecimal(valueStream, mapSize, static_cast<uint32_t>(EWidthTypes::WIDTH_Decimal));

            constStream << "#define CONST_";
            Printers::MakeSpacer(constStream, valueStream, Configuration::ConstSpacer);
            constStream << " " << constValue << "\n";

            Printers::Print(file, constStream);
            Printers::EmptyStream(valueStream);
        }
    }

    void ProcessConsts(FILE* file, UObject* uPackageObj)
    {
        for (int i = 0; i < UObject::GObjObjects()->Num(); i++)
        {
            UObject* uObject = UObject::GObjObjects()->Data[i];
            if (!uObject)
                continue;

            UObject* uPackageObject = uObject->GetPackageObj();
            if (!uPackageObject)
                continue;

            if (uPackageObject == uPackageObj && uObject->IsA(UConst::StaticClass()))
                GenerateConst(file, (UConst*)uObject);
        }
    }
}

namespace EnumGenerator
{
    void GenerateEnum(FILE* file, UEnum* uEnum)
    {
        std::ostringstream enumStream;
        std::ostringstream propertyStream;

        std::string sEnumName = Generator::GenerateValidName(std::string(uEnum->GetName()));
        std::string sEnumFullName = std::string(uEnum->GetFullName());
        std::string sEnumOuterName = Generator::GenerateValidName(std::string(uEnum->Outer->GetNameCPP()));

        if (sEnumName.find("Default__") != std::string::npos)
            return;

        //fprintf(pLog, "Enum:         %-50s - Instance: 0x%" PRIxPTR "\n", sEnumName.c_str(), pEnum);

        uint32_t enumCount = UObject::CountObject<UEnum>(uEnum->GetName());

        if (enumCount > 1)
        {
            if (Configuration::UsingEnumClasses)
            {
                enumStream << "// " << sEnumFullName << "\n" << "enum class " << sEnumOuterName << "_" << sEnumName << " : int\n{\n";
            }
            else
            {
                enumStream << "// " << sEnumFullName << "\n" << "enum " << sEnumOuterName << "_" << sEnumName << "\n{\n";
            }
        }
        else
        {
            if (Configuration::UsingEnumClasses)
            {
                enumStream << "// " << sEnumFullName << "\n" << "enum class " << sEnumName << " : int\n{\n";
            }
            else
            {
                enumStream << "// " << sEnumFullName << "\n" << "enum " << sEnumName << "\n{\n";
            }
        }

        std::map<std::string, uint32_t> propertiesMap;

        for (int i = 0; i < uEnum->Names.Num(); i++)
        {
            std::string propertyName = Generator::GenerateValidName(uEnum->Names[i].GetName());

            if (propertyName.find("_MAX") != std::string::npos)
                propertyName += "X";

            if (propertiesMap.count(propertyName) == 0)
            {
                propertiesMap[propertyName] = 1;
                propertyStream << propertyName;
            }
            else
            {
                propertyStream << propertyName;
                Printers::MakeDecimal(propertyStream, propertiesMap[propertyName], static_cast<uint32_t>(EWidthTypes::WIDTH_Decimal));
                propertiesMap[propertyName]++;
            }

            enumStream << "\t";
            Printers::MakeSpacer(enumStream, propertyStream, Configuration::EnumSpacer);
            enumStream << " = " << i;

            if (i != uEnum->Names.Num() - 1)
            {
                enumStream << ",\n";
            }
            else
            {
                enumStream << "\n";
            }

            Printers::EmptyStream(propertyStream);
        }

        enumStream << "};\n\n";

        std::string finishedEnum = enumStream.str();
        Printers::Print(file, finishedEnum);
    }

    void ProcessEnums(FILE* file, UObject* uPackageObj)
    {
        for (int i = 0; i < UObject::GObjObjects()->Num(); i++)
        {
            UObject* uObject = UObject::GObjObjects()->Data[i];
            if (!uObject)
                continue;

            UObject* uPackageObject = uObject->GetPackageObj();
            if (!uPackageObject)
                continue;

            if (uPackageObject == uPackageObj && uObject->IsA(UEnum::StaticClass()))
                GenerateEnum(file, (UEnum*)uObject);
        }
    }
}

namespace ClassGenerator
{
    void GenerateClass(FILE* file, UClass* uClass)
    {
        std::ostringstream classStream;
        std::ostringstream propertyStream;
        std::ostringstream flagStream;

        uintptr_t size = 0;
        uintptr_t lastOffset = 0;
        uintptr_t missedOffset = 0;

        UClass* uSuperClass = (UClass*)uClass->SuperField;

        std::string className = Generator::GenerateValidName(std::string(uClass->GetName()));
        std::string classNameCPP = Generator::GenerateValidName(std::string(uClass->GetNameCPP()));
        std::string classFullName = std::string(uClass->GetFullName());

        //fprintf(pLog, "Class:        %-50s - Instance: 0x%" PRIxPTR "\n", sClassName.c_str(), uClass);

        std::vector<UProperty*> vProperty;
        for (UProperty* uProperty = (UProperty*)uClass->Children; uProperty; uProperty = (UProperty*)uProperty->Next)
        {
            if (!uProperty->IsA(UFunction::StaticClass())
                && !uProperty->IsA(UConst::StaticClass())
                && !uProperty->IsA(UEnum::StaticClass())
                && !uProperty->IsA(UScriptStruct::StaticClass())
                && uProperty->ElementSize > 0
                && (!uSuperClass || (uSuperClass && uSuperClass != uClass && uProperty->Offset >= uSuperClass->PropertySize)))
            {
                vProperty.push_back(uProperty);
            }
        }

        std::sort(vProperty.begin(), vProperty.end(), Utils::SortProperty);

        classStream << "// " << classFullName << "\n";

        if (uSuperClass && uSuperClass != uClass)
        {
            size = uClass->PropertySize - uSuperClass->PropertySize;
            lastOffset = uSuperClass->PropertySize;

            std::string superClassNameCPP = Generator::GenerateValidName(std::string(uSuperClass->GetNameCPP()));

            classStream << "// ";
            Printers::MakeHex(classStream, size, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
            classStream << " (";
            Printers::MakeHex(classStream, uClass->PropertySize, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
            classStream << " - ";
            Printers::MakeHex(classStream, uSuperClass->PropertySize, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
            classStream << ")\n" << "class " << classNameCPP << " : public " << superClassNameCPP;
        }
        else
        {
            size = uClass->PropertySize;

            classStream << "// ";
            Printers::MakeHex(classStream, size, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
            classStream << "\n" << "class " << classNameCPP;
        }

        classStream << "\n{\npublic:\n";

        if (uClass == UObject::FindClass("Class Core.Field")) { classStream << UClass_Fields; }
        else if (uClass == UObject::FindClass("Class Core.Struct")) { classStream << UStruct_Fields; }
        else if (uClass == UObject::FindClass("Class Core.Function")) { classStream << UFunction_Fields; }
        else
        {
            std::map<std::string, uint32_t> propertyNameMap;
            uint32_t unknownDataIndex = 0;

            for (size_t i = 0; i < vProperty.size(); i++)
            {
                UProperty* uProperty = vProperty[i];

                if (lastOffset < uProperty->Offset)
                {
                    missedOffset = uProperty->Offset - lastOffset;

                    if (missedOffset >= Configuration::Alignment)
                    {
                        propertyStream << "UnknownData";
                        Printers::MakeDecimal(propertyStream, unknownDataIndex, static_cast<uint32_t>(EWidthTypes::WIDTH_Decimal));
                        propertyStream << "[";
                        Printers::MakeHex(propertyStream, missedOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_SpecialCase));
                        propertyStream << "];";

                        classStream << "\t";
                        Printers::MakeSpacer(classStream, "unsigned char", Configuration::ClassSpacer);
                        classStream << " ";
                        Printers::MakeSpacer(classStream, propertyStream, Configuration::ClassSpacer);
                        classStream << "\t\t// ";
                        Printers::MakeHex(classStream, lastOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                        classStream << " (";
                        Printers::MakeHex(classStream, missedOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                        classStream << ") MISSED OFFSET\n";

                        Printers::EmptyStream(propertyStream);

                        unknownDataIndex++;
                    }
                }

                std::string sPropertyType;
                if (Retrievers::GetPropertyType(uProperty, sPropertyType, false) != EPropertyTypes::TYPE_UnknownData)
                {
                    size_t correctElementSize = Retrievers::GetPropertySize(uProperty);
                    std::string propertyName = Generator::GenerateValidName(std::string(uProperty->GetName()));

                    if (propertyNameMap.count(propertyName) == 0)
                    {
                        propertyNameMap[propertyName] = 1;
                        propertyStream << propertyName;
                    }
                    else
                    {
                        propertyStream << propertyName;
                        Printers::MakeDecimal(propertyStream, propertyNameMap[propertyName], static_cast<uint32_t>(EWidthTypes::WIDTH_Decimal));
                        propertyNameMap[propertyName]++;
                    }

                    if (uProperty->ArrayDim > 1)
                    {
                        propertyStream << "[";
                        Printers::MakeHex(propertyStream, uProperty->ArrayDim, static_cast<uint32_t>(EWidthTypes::WIDTH_SpecialCase));
                        propertyStream << "] ";
                    }

                    if (uProperty->IsA(UBoolProperty::StaticClass()))
                        propertyStream << " : 1";

                    propertyStream << ";";

                    Retrievers::GetAllPropertyFlags(flagStream, uProperty->PropertyFlags.A);

                    classStream << "\t";
                    Printers::MakeSpacer(classStream, sPropertyType, Configuration::ClassSpacer);
                    classStream << " ";
                    Printers::MakeSpacer(classStream, propertyStream, Configuration::ClassSpacer);
                    classStream << "\t\t// ";
                    Printers::MakeHex(classStream, uProperty->Offset, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                    classStream << " (";
                    Printers::MakeHex(classStream, (uProperty->ElementSize* uProperty->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                    classStream << ") [";
                    Printers::MakeHex(classStream, uProperty->PropertyFlags.A, static_cast<uint32_t>(EWidthTypes::WIDTH_PropertyFlags));
                    classStream << "] ";

                    if (uProperty->IsA(UBoolProperty::StaticClass()))
                    {
                        classStream << "[";
                        Printers::MakeHex(classStream, ((UBoolProperty*)uProperty)->BitMask, static_cast<uint32_t>(EWidthTypes::WIDTH_BitMask));
                        classStream << "] ";
                    }
                    else
                    {
                        classStream;
                        Printers::MakeSpacer(classStream, 14);
                    }

                    classStream << flagStream.str() << "\n";

                    Printers::EmptyStream(propertyStream);
                    Printers::EmptyStream(flagStream);

                    int offserError = (uProperty->ElementSize * uProperty->ArrayDim) - (correctElementSize * uProperty->ArrayDim);

                    if (offserError > 0)
                    {
                        propertyStream << "UnknownData";
                        Printers::MakeDecimal(propertyStream, unknownDataIndex, static_cast<uint32_t>(EWidthTypes::WIDTH_Decimal));
                        propertyStream << "[";
                        Printers::MakeHex(propertyStream, offserError, static_cast<uint32_t>(EWidthTypes::WIDTH_SpecialCase));
                        propertyStream << "];";

                        classStream << "\t";
                        Printers::MakeSpacer(classStream, "unsigned char", Configuration::ClassSpacer);
                        classStream << " ";
                        Printers::MakeSpacer(classStream, propertyStream, Configuration::ClassSpacer);
                        classStream << "\t\t// ";
                        Printers::MakeHex(classStream, (uProperty->Offset + offserError), static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                        classStream << " (";
                        Printers::MakeHex(classStream, offserError, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                        classStream << ") FIX WRONG SIZE OF PREVIOUS PROPERTY\n";

                        Printers::EmptyStream(propertyStream);

                        unknownDataIndex++;
                    }
                }
                else
                {
                    propertyStream << "UnknownData";
                    Printers::MakeDecimal(propertyStream, unknownDataIndex, static_cast<uint32_t>(EWidthTypes::WIDTH_Decimal));
                    propertyStream << "[";
                    Printers::MakeHex(propertyStream, (uProperty->ElementSize* uProperty->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_SpecialCase));
                    propertyStream << "];";

                    classStream << "\t";
                    Printers::MakeSpacer(classStream, "unsigned char", Configuration::ClassSpacer);
                    classStream << " ";
                    Printers::MakeSpacer(classStream, propertyStream, Configuration::ClassSpacer);
                    classStream << "\t\t// ";
                    Printers::MakeHex(classStream, uProperty->Offset, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                    classStream << " (";
                    Printers::MakeHex(classStream, (uProperty->ElementSize * uProperty->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                    classStream << ") UNKNOWN PROPERTY: " << std::string(uProperty->GetFullName()) << "\n";

                    Printers::EmptyStream(propertyStream);

                    unknownDataIndex++;
                }

                lastOffset = uProperty->Offset + (uProperty->ElementSize * uProperty->ArrayDim);
            }

            if (lastOffset < uClass->PropertySize)
            {
                missedOffset = uClass->PropertySize - lastOffset;

                if (missedOffset >= Configuration::Alignment)
                {
                    propertyStream << "UnknownData";
                    Printers::MakeDecimal(propertyStream, unknownDataIndex, static_cast<uint32_t>(EWidthTypes::WIDTH_Decimal));
                    propertyStream << "[";
                    Printers::MakeHex(propertyStream, missedOffset, 0);
                    propertyStream << "];";

                    classStream << "\t";
                    Printers::MakeSpacer(classStream, "unsigned char", Configuration::ClassSpacer);
                    classStream << " ";
                    Printers::MakeSpacer(classStream, propertyStream, Configuration::ClassSpacer);
                    classStream << "\t\t// ";
                    Printers::MakeHex(classStream, lastOffset, 4);
                    classStream << " (";
                    Printers::MakeHex(classStream, missedOffset, 4);
                    classStream << ") MISSED OFFSET\n";

                    Printers::EmptyStream(propertyStream);
                }
            }
        }

        classStream << "public:\n";

        if (uClass == UObject::FindClass("Class Core.Object")) classStream << UObject_FunctionDescriptions;
        else if (uClass == UObject::FindClass("Class Core.Function")) classStream << UFunction_FunctionDescriptions;

        if (Configuration::UsingConstants)
        {
            classStream << "\tstatic UClass* StaticClass()\n"
                << "\t{\n"
                << "\t\tstatic UClass* pClassPointer = nullptr;\n\n"
                << "\t\tif (!pClassPointer)\n"
                << "\t\t\tpClassPointer = (UClass*)UObject::GObjObjects()->Data[" << Generator::GenerateUniqueName(uClass, true) << "];\n\n"
                << "\t\treturn pClassPointer;\n"
                << "\t};\n\n";
        }
        else
        {
            classStream << "\tstatic UClass* StaticClass()\n"
                << "\t{\n"
                << "\t\tstatic UClass* pClassPointer = nullptr;\n\n"
                << "\t\tif (!pClassPointer)\n"
                << "\t\t\tpClassPointer = UObject::FindClass(\"" << classFullName << "\");\n\n"
                << "\t\treturn pClassPointer;\n"
                << "\t};\n\n";
        }

        Printers::Print(file, classStream);

        FunctionGenerator::GenerateFunctionDescription(file, uClass);

        if (uClass == UObject::FindClass("Class Core.Object"))
        {
            if (Configuration::UsingDetours)
            {
                classStream << "\tvoid ProcessEvent(class UFunction* pFunction, void* pParms, void* pResult);\n";
            }
            else
            {
                FunctionGenerator::GenerateVirtualFunction(file, uClass);
            }
        }

        classStream << "};\n\n";

        Printers::Print(file, classStream);
    }

    void GenerateClassProperties(FILE* file, UClass* uClass, UObject* uPackageObj)
    {
        UObject* uPackageObject = uClass->GetPackageObj();

        if (!uPackageObject)
            return;

        if (std::find(Generator::vIncludes.begin(), Generator::vIncludes.end(), uPackageObj) == Generator::vIncludes.end())
            Generator::vIncludes.push_back(uPackageObj);

        if (uPackageObject != uPackageObj)
        {
            std::vector<UObject*>::iterator itPO = std::find(Generator::vIncludes.begin(), Generator::vIncludes.end(), uPackageObject);
            std::vector<UObject*>::iterator itPTP = std::find(Generator::vIncludes.begin(), Generator::vIncludes.end(), uPackageObj);

            if (itPO == Generator::vIncludes.end())
            {
                Generator::vIncludes.insert(itPTP, uPackageObject);
            }
            else if (itPO >= itPTP)
            {
                Generator::vIncludes.insert(itPTP, uPackageObject);
                Generator::vIncludes.erase(itPO);
            }

            return;
        }

        static std::map<std::string, int> mClassNames;

        std::string className = Generator::GenerateValidName(std::string(uClass->GetName()));
        std::string classFullName = std::string(uClass->GetFullName());

        if (className.find("Default__") != std::string::npos)
            return;

        if (mClassNames.find(classFullName) == mClassNames.end())
        {
            if (uClass->SuperField && uClass->SuperField != uClass)
            {
                if (mClassNames.find(std::string(uClass->SuperField->GetFullName())) == mClassNames.end())
                    GenerateClassProperties(file, (UClass*)uClass->SuperField, uPackageObj);
            }
            
            GenerateClass(file, uClass);
            mClassNames.insert(std::make_pair(classFullName, uClass->ObjectInternalInteger));
        }
    }

    void ProcessClasses(FILE* file, UObject* uPackageObj)
    {
        for (int i = 0; i < UObject::GObjObjects()->Num(); i++)
        {
            UObject* uObject = UObject::GObjObjects()->Data[i];
            if (!uObject)
                continue;

            UObject* uPackageObject = uObject->GetPackageObj();
            if (!uPackageObject)
                continue;

            if (uPackageObject == uPackageObj && uObject->IsA(UClass::StaticClass()))
                GenerateClassProperties(file, (UClass*)uObject, uPackageObject);
        }
    }
}

namespace ParameterGenerator
{
    void GenerateParameter(FILE* file, UClass* uClass)
    {
        std::ostringstream parameterStream;
        std::ostringstream propertyStream;
        std::ostringstream flagStream;

        std::vector<UFunction*> vFunction;

        for (UProperty* uProperty = (UProperty*)uClass->Children; uProperty; uProperty = (UProperty*)uProperty->Next)
        {
            if (uProperty->IsA(UFunction::StaticClass()))
                vFunction.push_back((UFunction*)uProperty);
        }

        for (size_t i = 0; i < vFunction.size(); i++)
        {
            UFunction* uFunction = vFunction[i];

            std::string functionFullName = std::string(uFunction->GetFullName());
            std::string functionName = Generator::GenerateValidName(std::string(uFunction->GetName()));
            std::string classNameCPP = Generator::GenerateValidName(std::string(uClass->GetNameCPP()));

            parameterStream << "// " << functionFullName << "\n" << "// [";
            Printers::MakeHex(parameterStream, uFunction->FunctionFlags, static_cast<uint32_t>(EWidthTypes::WIDTH_FunctionFlags));
            parameterStream << "] ";

            if ((uFunction->FunctionFlags & EFunctionFlags::FUNC_Native) && uFunction->iNative)
            {
                parameterStream << " iNative [";
                Printers::MakeHex(parameterStream, uFunction->iNative, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                parameterStream << "]";
            }

            if (uFunction->FunctionFlags & EFunctionFlags::FUNC_Exec) { propertyStream << "exec"; }
            else if (uFunction->FunctionFlags & EFunctionFlags::FUNC_Event) { propertyStream << "event"; }
            else { propertyStream << "exec"; }

            parameterStream << "\nstruct " << classNameCPP << "_" << propertyStream.str() << functionName << "_Parms\n" << "{\n";

            Printers::EmptyStream(propertyStream);

            std::vector<UProperty*> vProperty;

            for (UProperty* uProperty = (UProperty*)uFunction->Children; uProperty; uProperty = (UProperty*)uProperty->Next)
            {
                if (uProperty->ElementSize > 0)
                    vProperty.push_back(uProperty);		
            }

            std::sort(vProperty.begin(), vProperty.end(), Utils::SortProperty);

            std::map<std::string, uint32_t> propertyNameMap;

            for (size_t i = 0; i < vProperty.size(); i++)
            {
                UProperty* uProperty = vProperty[i];

                std::string propertyType;

                if (Retrievers::GetPropertyType(uProperty, propertyType, true) != EPropertyTypes::TYPE_UnknownData)
                {
                    std::string propertyName = Generator::GenerateValidName(std::string(uProperty->GetName()));

                    if (propertyNameMap.count(propertyName) == 0)
                    {
                        propertyNameMap[propertyName] = 1;
                        propertyStream << propertyName;
                    }
                    else
                    {
                        propertyStream << propertyName;
                        Printers::MakeDecimal(propertyStream, propertyNameMap[propertyName], static_cast<uint32_t>(EWidthTypes::WIDTH_Decimal));
                        propertyNameMap[propertyName]++;
                    }

                    if (uProperty->ArrayDim > 1)
                    {
                        propertyStream << "[";
                        Printers::MakeHex(propertyStream, uProperty->ArrayDim, static_cast<uint32_t>(EWidthTypes::WIDTH_SpecialCase));
                        propertyStream << "]";
                    }

                    if (uProperty->IsA(UBoolProperty::StaticClass()))
                        propertyStream << " : 1";

                    propertyStream << ";";

                    Retrievers::GetAllPropertyFlags(flagStream, uProperty->PropertyFlags.A);

                    if (uProperty->PropertyFlags.A & EPropertyFlags::CPF_Parm)
                    {
                        parameterStream << "\t";
                        Printers::MakeSpacer(parameterStream, propertyType, Configuration::FunctionSpacer);
                        parameterStream << " ";
                        Printers::MakeSpacer(parameterStream, propertyStream, Configuration::FunctionSpacer);
                        parameterStream << "\t\t// ";
                        Printers::MakeHex(parameterStream, uProperty->Offset, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                        parameterStream << " (";
                        Printers::MakeHex(parameterStream, (uProperty->ElementSize * uProperty->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                        parameterStream << ") [";
                        Printers::MakeHex(parameterStream, uProperty->PropertyFlags.A, static_cast<uint32_t>(EWidthTypes::WIDTH_PropertyFlags));
                        parameterStream << "] ";
                    }
                    else
                    {
                        parameterStream << "\t// ";
                        Printers::MakeSpacer(parameterStream, propertyType, (Configuration::FunctionSpacer - 3));
                        parameterStream << " ";
                        Printers::MakeSpacer(parameterStream, propertyStream, Configuration::FunctionSpacer);
                        parameterStream << "\t\t// ";
                        Printers::MakeHex(parameterStream, uProperty->Offset, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                        parameterStream << " (";
                        Printers::MakeHex(parameterStream, (uProperty->ElementSize * uProperty->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                        parameterStream << ") [";
                        Printers::MakeHex(parameterStream, uProperty->PropertyFlags.A, static_cast<uint32_t>(EWidthTypes::WIDTH_PropertyFlags));
                        parameterStream << "] ";
                    }

                    if (uProperty->IsA(UBoolProperty::StaticClass()))
                    {
                        parameterStream << "[";
                        Printers::MakeHex(parameterStream, ((UBoolProperty*)uProperty)->BitMask, static_cast<uint32_t>(EWidthTypes::WIDTH_BitMask));
                        parameterStream << "] ";
                    }
                    else
                    {
                        Printers::MakeSpacer(parameterStream, 14);
                    }

                    parameterStream << flagStream.str() << "\n";

                    Printers::EmptyStream(propertyStream);
                    Printers::EmptyStream(flagStream);
                }
                else
                {
                    std::string sPropertyFullName = std::string(uProperty->GetFullName());

                    parameterStream << "\t// UNKNOWN PROPERTY: " << sPropertyFullName << "\n";
                }
            }

            parameterStream << "};\n\n";
        }

        Printers::Print(file, parameterStream);
    }

    void ProcessParameters(FILE* file, UObject* uPackageObj)
    {
        for (int i = 0; i < UObject::GObjObjects()->Num(); i++)
        {
            UObject* uObject = UObject::GObjObjects()->Data[i];
            if (!uObject)
                continue;

            UObject* uPackageObject = uObject->GetPackageObj();
            if (!uPackageObject)
                continue;

            if (uPackageObject == uPackageObj && uObject->IsA(UClass::StaticClass()))
                GenerateParameter(file, (UClass*)uObject);
        }
    }
}

namespace FunctionGenerator
{
    void GenerateVirtualFunction(FILE* file, UClass* uClass)
    {
        std::ostringstream functionStream;

        functionStream << "\n\t// Virtual Functions\n";

        uintptr_t VfTable = uClass->VfTableObject.Dummy;

        for (uintptr_t i = 0x0; i < 0x400; i += 0x8)
        {
            if (Utils::FindPattern(*(uintptr_t*)(VfTable + i), 0x200, Configuration::ProcessEventPattern, Configuration::ProcessEventMask))
            {
                functionStream << "\tvirtual void ProcessEvent(class UFunction* pFunction, void* pParms, void* pResult = nullptr);\t\t\t\t// ";
                Printers::MakeHex(functionStream, (*(uintptr_t*)(VfTable + i)), 8);
                functionStream << " (";
                Printers::MakeHex(functionStream, i, 2);
                functionStream << ")\n";
                break;
            }
            else
            {
                functionStream << "\tvirtual void VirtualFunction";
                Printers::MakeDecimal(functionStream, (i / 0x8), static_cast<uint32_t>(EWidthTypes::WIDTH_Decimal));
                functionStream << " ();\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t// ";
                Printers::MakeHex(functionStream, (*(uintptr_t*)(VfTable + i)), 8);
                functionStream << " (";
                Printers::MakeHex(functionStream, i, static_cast<uint32_t>(EWidthTypes::WIDTH_Decimal));
                functionStream << ")\n";
            }
        }

        Printers::Print(file, functionStream);
    }

    void GenerateFunctionCode(FILE* file, UClass* uClass)
    {
        std::ostringstream codeStream;
        std::ostringstream functionStream;
        std::vector<UFunction*> vFunction;

        static bool objectFunctions = false;
        static bool functionFunctions = false;

        if (!objectFunctions && uClass == UObject::FindClass("Class Core.Object"))
        {
            codeStream << UObject_Functions;

            if (Configuration::UsingDetours)
            {
                codeStream << "template<typename T>\n"
                << "T GetVFunction(const void* instance, std::size_t index)\n"
                << "{\n"
                << "\tauto vtable = *static_cast<const void***>(const_cast<void*>(instance));\n"
                << "\treturn reinterpret_cast<T>(vtable[index]);\n"
                << "}\n\n";

                codeStream << "void UObject::ProcessEvent(class UFunction* pFunction, void* pParms, void* pResult = nullptr)\n"
                << "{\n"
                << "\tGetVFunction<void(*)(UObject*, class UFunction*, void*)>(this, 67)(this, pFunction, pParms);\n"
                << "}\n\n";
            }

            objectFunctions = true;
        }

        if (!objectFunctions && uClass == UObject::FindClass("Class Core.Function"))
        {
            codeStream << UFunction_FunctionDescriptions;

            functionFunctions = true;
        }

        for (UProperty* uProperty = (UProperty*)uClass->Children; uProperty; uProperty = (UProperty*)uProperty->Next)
        {
            if (uProperty->IsA(UFunction::StaticClass()))
                vFunction.push_back((UFunction*)uProperty);
        }

        for (size_t i = 0; i < vFunction.size(); i++)
        {
            UFunction* pFunction = vFunction[i];

            std::string sFunctionFullName = std::string(pFunction->GetFullName());
            std::string sFunctionName = Generator::GenerateValidName(std::string(pFunction->GetName()));
            std::string sClassNameCPP = Generator::GenerateValidName(std::string(uClass->GetNameCPP()));

            Retrievers::GetAllFunctionFlags(functionStream, pFunction->FunctionFlags);
            codeStream << "// " << sFunctionFullName << "\n" << "// [";
            Printers::MakeHex(codeStream, pFunction->FunctionFlags, static_cast<uint32_t>(EWidthTypes::WIDTH_FunctionFlags));
            codeStream << "] "<< functionStream.str();
            Printers::EmptyStream(functionStream);

            if ((pFunction->FunctionFlags & EFunctionFlags::FUNC_Native) && pFunction->iNative)
            {
                codeStream << " iNative [";
                Printers::MakeHex(codeStream, pFunction->iNative, static_cast<uint32_t>(EWidthTypes::WIDTH_Size));
                codeStream << "]";
            }

            std::vector<std::pair<UProperty*, std::string>> vProperty_Parms;
            std::vector<std::pair<UProperty*, std::string>> vProperty_OutParms;
            std::pair<UProperty*, std::string> uProperty_ReturnParm;

            std::map<std::string, uint32_t> propertyNameMap;

            for (UProperty* uProperty = (UProperty*)pFunction->Children; uProperty; uProperty = (UProperty*)uProperty->Next)
            {
                if (uProperty->ElementSize == 0)
                    continue;

                std::string sPropertyNameBuffer = Generator::GenerateValidName(std::string(uProperty->GetName()));
                std::string sPropertyNameUnique;

                if (propertyNameMap.count(sPropertyNameBuffer) == 0)
                {
                    propertyNameMap[sPropertyNameBuffer] = 1;
                    sPropertyNameUnique = sPropertyNameBuffer;
                }
                else
                {
                    functionStream << sPropertyNameBuffer;
                    Printers::MakeDecimal(functionStream, propertyNameMap[sPropertyNameBuffer], static_cast<uint32_t>(EWidthTypes::WIDTH_Decimal));
                    sPropertyNameUnique = functionStream.str();
                    Printers::EmptyStream(functionStream);
                    propertyNameMap[sPropertyNameBuffer]++;
                }

                if (uProperty->PropertyFlags.A & EPropertyFlags::CPF_ReturnParm) { uProperty_ReturnParm = make_pair(uProperty, sPropertyNameUnique); }
                else if (uProperty->PropertyFlags.A & EPropertyFlags::CPF_OutParm) { vProperty_OutParms.push_back(make_pair(uProperty, sPropertyNameUnique)); }
                else if (uProperty->PropertyFlags.A & EPropertyFlags::CPF_Parm) { vProperty_Parms.push_back(make_pair(uProperty, sPropertyNameUnique)); }
            }

            sort(vProperty_Parms.begin(), vProperty_Parms.end(), Utils::SortPropertyPair);
            sort(vProperty_OutParms.begin(), vProperty_OutParms.end(), Utils::SortPropertyPair);

            codeStream << "\n// Parameter info:\n";

            std::string propertyType;

            if (uProperty_ReturnParm.first && Retrievers::GetPropertyType(uProperty_ReturnParm.first, propertyType, true) != EPropertyTypes::TYPE_UnknownData)
            {
                Retrievers::GetAllPropertyFlags(functionStream, uProperty_ReturnParm.first->PropertyFlags.A);
                codeStream << "// ";
                Printers::MakeSpacer(codeStream, propertyType, Configuration::CommentSpacer);
                codeStream << " ";
                Printers::MakeSpacer(codeStream, uProperty_ReturnParm.second, Configuration::CommentSpacer);
                codeStream << " " << functionStream.str() << "\n";
                Printers::EmptyStream(functionStream);
            }

            for (size_t i = 0; i < vProperty_Parms.size(); i++)
            {
                std::pair<UProperty*, std::string> uProperty(vProperty_Parms[i]);

                if (Retrievers::GetPropertyType(uProperty.first, propertyType, false) != EPropertyTypes::TYPE_UnknownData)
                {
                    Retrievers::GetAllPropertyFlags(functionStream, uProperty.first->PropertyFlags.A);
                    codeStream << "// ";
                    Printers::MakeSpacer(codeStream, propertyType, Configuration::CommentSpacer);
                    codeStream << " ";
                    Printers::MakeSpacer(codeStream, uProperty.second, Configuration::CommentSpacer);
                    codeStream << " " << functionStream.str() << "\n";
                    Printers::EmptyStream(functionStream);
                }
            }

            for (size_t i = 0; i < vProperty_OutParms.size(); i++)
            {
                std::pair<UProperty*, std::string> uProperty(vProperty_OutParms[i]);

                if (Retrievers::GetPropertyType(uProperty.first, propertyType, false) != EPropertyTypes::TYPE_UnknownData)
                {
                    Retrievers::GetAllPropertyFlags(functionStream, uProperty.first->PropertyFlags.A);
                    codeStream << "// ";
                    Printers::MakeSpacer(codeStream, propertyType, Configuration::CommentSpacer);
                    codeStream << " ";
                    Printers::MakeSpacer(codeStream, uProperty.second, Configuration::CommentSpacer);
                    codeStream << " " << functionStream.str() << "\n";
                    Printers::EmptyStream(functionStream);
                }
            }

            if (uProperty_ReturnParm.first && Retrievers::GetPropertyType(uProperty_ReturnParm.first, propertyType, true) != EPropertyTypes::TYPE_UnknownData)
            {
                codeStream << "\n" << propertyType;
            }
            else
            {
                codeStream << "\nvoid";
            }

            if (pFunction->FunctionFlags & EFunctionFlags::FUNC_Exec) { codeStream << " " << sClassNameCPP << "::" << sFunctionName << "("; }
            else if (pFunction->FunctionFlags & EFunctionFlags::FUNC_Event) { codeStream << " " << sClassNameCPP << "::event" << sFunctionName << "("; }
            //else if (pFunction->FunctionFlags & EFunctionFlags::FUNC_Event) { codeStream << " " << sClassNameCPP << "::" << sFunctionName << "("; }
            else { codeStream << " " << sClassNameCPP << "::" << sFunctionName << "("; }

            bool printComma = false;

            for (size_t i = 0; i < vProperty_Parms.size(); i++)
            {
                std::pair<UProperty*, std::string> uProperty(vProperty_Parms[i]);

                if (Retrievers::GetPropertyType(uProperty.first, propertyType, false) != EPropertyTypes::TYPE_UnknownData)
                {
                    if (printComma)
                        codeStream << ", ";

                    codeStream << propertyType << " " << uProperty.second;

                    printComma = true;
                }
            }

            for (size_t i = 0; i < vProperty_OutParms.size(); i++)
            {
                std::pair<UProperty*, std::string> uProperty(vProperty_OutParms[i]);

                if (Retrievers::GetPropertyType(uProperty.first, propertyType, false) != EPropertyTypes::TYPE_UnknownData)
                {
                    if (uProperty.first->PropertyFlags.A & EPropertyFlags::CPF_Parm)
                    {
                        if (printComma)
                            codeStream << ", ";

                        codeStream << propertyType << "& " << uProperty.second;

                        printComma = true;
                    }
                }
            }

            if (Configuration::UsingConstants)
            {
                codeStream << ")\n{\n"
                << "\tstatic UFunction* pFn" << sFunctionName << " = nullptr;\n\n"
                << "\tif (!pFn" << sFunctionName << ")\n"
                << "\t\tpFn" << sFunctionName << " = (UFunction*)UObject::GObjObjects()->Data[" << Generator::GenerateUniqueName(pFunction, uClass, true) << "];\n\n"
                << "\t" << sClassNameCPP << "_";
            }
            else
            {
                codeStream << ")\n{\n"
                << "\tstatic UFunction* pFn" << sFunctionName << " = nullptr;\n\n"
                << "\tif (!pFn" << sFunctionName << ")\n"
                << "\t\tpFn" << sFunctionName << " = (UFunction*)UFunction::FindFunction(\"" << sFunctionFullName << "\");\n\n"
                << "\t" << sClassNameCPP << "_";
            }

            if (pFunction->FunctionFlags & EFunctionFlags::FUNC_Exec) { codeStream << "exec"; }
            else if (pFunction->FunctionFlags & EFunctionFlags::FUNC_Event) { codeStream << "event"; }
            else { codeStream << "exec"; }

            codeStream << sFunctionName << "_Parms " << sFunctionName << "_Parms;\n";

            for (size_t i = 0; i < vProperty_Parms.size(); i++)
            {
                std::pair<UProperty*, std::string> uProperty(vProperty_Parms[i]);

                EPropertyTypes propertyTypeResult = Retrievers::GetPropertyType(uProperty.first, propertyType, false);

                if (!Utils::IsBitField(propertyTypeResult) || !Utils::IsBitField(uProperty.first->ArrayDim))
                {
                    codeStream << "\tmemcpy(&" << sFunctionName << "_Parms." << uProperty.second << ", &" << uProperty.second << ", ";
                    Printers::MakeHex(codeStream, (uProperty.first->ElementSize* uProperty.first->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_SpecialCase));
                    codeStream << ");\n";
                }
                else if (propertyTypeResult != EPropertyTypes::TYPE_UnknownData && propertyTypeResult < EPropertyTypes::TYPE_FPointer)
                {
                    codeStream << "\t" << sFunctionName << "_Parms." << uProperty.second << " = " << uProperty.second << ";\n";
                }
            }

            if ((pFunction->FunctionFlags & EFunctionFlags::FUNC_Native) && pFunction->iNative)
                codeStream << "\n\tunsigned short NativeIndex = pFn" << sFunctionName << "->iNative;\n\tpFn" << sFunctionName << "->iNative = 0;\n";

            if (pFunction->FunctionFlags & EFunctionFlags::FUNC_Native)
            {
                codeStream << "\n\tpFn" << sFunctionName << "->FunctionFlags |= ~";
                Printers::MakeHex(codeStream, EFunctionFlags::FUNC_Native, static_cast<uint32_t>(EWidthTypes::WIDTH_SpecialCase));
                codeStream << ";\n";
            }

            codeStream << "\n\tthis->ProcessEvent(pFn" << sFunctionName << ", &" << sFunctionName << "_Parms, nullptr);\n";

            if (pFunction->FunctionFlags & EFunctionFlags::FUNC_Native)
            {
                codeStream << "\n\tpFn" << sFunctionName << "->FunctionFlags |= ";
                Printers::MakeHex(codeStream, EFunctionFlags::FUNC_Native, static_cast<uint32_t>(EWidthTypes::WIDTH_SpecialCase));
                codeStream << ";\n";
            }

            if ((pFunction->FunctionFlags & EFunctionFlags::FUNC_Native) && pFunction->iNative)
                codeStream << "\n\tpFn" << sFunctionName << "->iNative = NativeIndex;\n";

            for (size_t i = 0; i < vProperty_OutParms.size(); i++)
            {
                std::pair<UProperty*, std::string> uProperty(vProperty_OutParms[i]);

                if (uProperty.first->PropertyFlags.A & EPropertyFlags::CPF_Parm)
                {
                    EPropertyTypes propertyTypeResult = Retrievers::GetPropertyType(uProperty.first, propertyType, false);

                    if (!Utils::IsBitField(propertyTypeResult)|| !Utils::IsBitField(uProperty.first->ArrayDim))
                    {
                        codeStream << "\tmemcpy(&" << uProperty.second << ", &" << sFunctionName << "_Parms." << uProperty.second << ", ";
                        Printers::MakeHex(codeStream, (uProperty.first->ElementSize* uProperty.first->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_SpecialCase));
                        codeStream << ");\n";
                    }
                    else if (propertyTypeResult != EPropertyTypes::TYPE_UnknownData && propertyTypeResult < EPropertyTypes::TYPE_FPointer)
                    {
                        codeStream << "\t" << uProperty.second << " = " << sFunctionName << "_Parms." << uProperty.second << ";\n";
                    }
                }
            }

            if (uProperty_ReturnParm.first && Retrievers::GetPropertyType(uProperty_ReturnParm.first, propertyType, false) != EPropertyTypes::TYPE_UnknownData)
                codeStream << "\n\treturn " << sFunctionName << "_Parms." << uProperty_ReturnParm.second << ";\n";

            codeStream << "};\n\n";
        }

        Printers::Print(file, codeStream);
    }

    void GenerateFunctionDescription(FILE* file, UClass* uClass)
    {
        std::ostringstream functionStream;
        std::ostringstream propertyStream;

        std::vector<UFunction*> vFunction;

        for (UProperty* uProperty = (UProperty*)uClass->Children; uProperty; uProperty = (UProperty*)uProperty->Next)
        {
            if (uProperty->IsA(UFunction::StaticClass()))
                vFunction.push_back((UFunction*)uProperty);
        }

        for (size_t i = 0; i < vFunction.size(); i++)
        {
            UFunction* uFunction = vFunction[i];

            std::string functionName = Generator::GenerateValidName(std::string(uFunction->GetName()));

            std::vector<std::pair<UProperty*, std::string>> vProperty_Parms;
            std::vector<std::pair<UProperty*, std::string>> vProperty_OutParms;
            std::pair<UProperty*, std::string> uProperty_ReturnParm;

            std::map<std::string, uint32_t> propertyNameMap;

            for (UProperty* uProperty = (UProperty*)uFunction->Children; uProperty; uProperty = (UProperty*)uProperty->Next)
            {
                if (uProperty->ElementSize == 0)
                    continue;

                std::string propertyNameBuffer = Generator::GenerateValidName(std::string(uProperty->GetName()));
                std::string propertyNameUnique;

                if (propertyNameMap.count(propertyNameBuffer) == 0)
                {
                    propertyNameMap[propertyNameBuffer] = 1;
                    propertyNameUnique = propertyNameBuffer;
                }
                else
                {
                    propertyStream << propertyNameBuffer;
                    Printers::MakeDecimal(propertyStream, propertyNameMap[propertyNameBuffer], static_cast<uint32_t>(EWidthTypes::WIDTH_Decimal));

                    propertyNameUnique = propertyStream.str();
                    Printers::EmptyStream(propertyStream);
                    propertyNameMap[propertyNameBuffer]++;
                }

                if (uProperty->PropertyFlags.A & EPropertyFlags::CPF_ReturnParm) { uProperty_ReturnParm = std::make_pair(uProperty, propertyNameUnique); }
                else if (uProperty->PropertyFlags.A & EPropertyFlags::CPF_OutParm) { vProperty_OutParms.push_back(std::make_pair(uProperty, propertyNameUnique)); }
                else if (uProperty->PropertyFlags.A & EPropertyFlags::CPF_Parm) { vProperty_Parms.push_back(std::make_pair(uProperty, propertyNameUnique)); }
            }

            std::sort(vProperty_Parms.begin(), vProperty_Parms.end(), Utils::SortPropertyPair);
            std::sort(vProperty_OutParms.begin(), vProperty_OutParms.end(), Utils::SortPropertyPair);

            std::string propertyType;

            if (uProperty_ReturnParm.first && Retrievers::GetPropertyType(uProperty_ReturnParm.first, propertyType, true) != EPropertyTypes::TYPE_UnknownData) {
                functionStream << "\t" << propertyType;
            }
            else
            {
                functionStream << "\tvoid";
            }

            //functionStream << " " << functionName << "(";

            if (uFunction->FunctionFlags & EFunctionFlags::FUNC_Exec) { functionStream << " " << functionName << "("; }
            else if (uFunction->FunctionFlags & EFunctionFlags::FUNC_Event) { functionStream << " event" << functionName << "("; }
            else { functionStream << " " << functionName << "("; }

            bool printComma = false;

            for (size_t i = 0; i < vProperty_Parms.size(); i++ )
            {
                std::pair<UProperty*, std::string> uProperty(vProperty_Parms[i]);

                if (Retrievers::GetPropertyType(uProperty.first, propertyType, false) != EPropertyTypes::TYPE_UnknownData)
                {
                    if (printComma)
                        functionStream << ", ";

                    functionStream << propertyType << " " << uProperty.second;

                    printComma = true;
                }
            }

            for (size_t i = 0; i < vProperty_OutParms.size(); i++ )
            {
                std::pair<UProperty*, std::string> uProperty(vProperty_OutParms[i]);

                if (Retrievers::GetPropertyType(uProperty.first, propertyType, false) != EPropertyTypes::TYPE_UnknownData)
                {
                    if (uProperty.first->PropertyFlags.A & EPropertyFlags::CPF_Parm)
                    {
                        if (printComma)
                            functionStream << ", ";

                        functionStream << propertyType << "& " << uProperty.second;

                        printComma = true;
                    }
                }
            }

            functionStream << ");\n";
        }

        Printers::Print(file, functionStream);
    }

    void ProcessFunctions(FILE* file, UObject* uPackageObj)
    {
        for (int i = 0; i < UObject::GObjObjects()->Num(); i++)
        {
            UObject* uObject = UObject::GObjObjects()->Data[i];
            if (!uObject)
                continue;

            UObject* uPackageObject = uObject->GetPackageObj();
            if (!uPackageObject)
                continue;

            if (uPackageObject == uPackageObj && uObject->IsA(UClass::StaticClass()))
                GenerateFunctionCode(file, (UClass*)uObject);
        }
    }
}

namespace Generator
{
    FILE* logFile = nullptr;
    std::vector<std::pair<std::string, int>> vConstants{};
    std::vector<UObject*> vIncludes{};

    std::string GenerateValidName(const std::string& invalidName)
    {
        std::string newName = invalidName;

        for (size_t i = 0; i < newName.length(); ++i)
        {
            if (newName[i] == ' '
                || newName[i] == '?'
                || newName[i] == '+'
                || newName[i] == '-'
                || newName[i] == ':'
                || newName[i] == '/'
                || newName[i] == '^'
                || newName[i] == '('
                || newName[i] == ')'
                || newName[i] == '['
                || newName[i] == ']'
                || newName[i] == '<'
                || newName[i] == '>'
                || newName[i] == '&'
                || newName[i] == '.'
                || newName[i] == '#'
                || newName[i] == '\''
                || newName[i] == '"'
                || newName[i] == '%')
            {
                newName[i] = '_';
            }
        }

        return newName;
    }

    std::string GenerateUniqueName(UClass* uClass, bool useIndex)
    {
        std::ostringstream nameBuffer;

        UClass* superClass = (UClass*)uClass->SuperField;
        std::string classNameCPP = GenerateValidName(std::string(uClass->GetNameCPP()));

        for (size_t i = 0; i < classNameCPP.size(); i++)
            classNameCPP[i] = std::toupper(classNameCPP[i]);

        if (useIndex)
        {
            nameBuffer << "IDX_" << classNameCPP;
        }
        else
        {
            nameBuffer << classNameCPP;
        }

        if (superClass)
        {
            std::string superClassNameCPP = GenerateValidName(std::string(superClass->GetNameCPP()));

            for (size_t i = 0; i < superClassNameCPP.size(); i++)
                superClassNameCPP[i] = std::toupper(superClassNameCPP[i]);

            nameBuffer << "_" << superClassNameCPP;
        }

        std::string name = nameBuffer.str();

        if (useIndex && Configuration::UsingConstants)
        {
            std::pair<std::string, int> pConstant = std::make_pair(name, uClass->ObjectInternalInteger);

            if (std::find(vConstants.begin(), vConstants.end(), pConstant) == vConstants.end())
                vConstants.push_back(pConstant);
        }

        return name;
    }

    std::string GenerateUniqueName(UFunction* uFunction, UClass* uClass, bool useIndex)
    {
        std::ostringstream nameBuffer;

        std::string functionName = GenerateValidName(std::string(uFunction->GetName()));
        std::string classNameCPP = GenerateValidName(std::string(uClass->GetNameCPP()));

        for (size_t i = 0; i < functionName.size(); i++)
            functionName[i] = std::toupper(functionName[i]);

        for (size_t i = 0; i < classNameCPP.size(); i++)
            classNameCPP[i] = std::toupper(classNameCPP[i]);

        if (useIndex)
        {
            nameBuffer << "IDX_" << classNameCPP << "_" << functionName;
        }
        else
        {
            nameBuffer << classNameCPP << "_" << functionName;
        }

        std::string name = nameBuffer.str();

        if (useIndex && Configuration::UsingConstants)
        {
            std::pair<std::string, int> pConstant = std::make_pair(name, uFunction->ObjectInternalInteger);

            if (std::find(vConstants.begin(), vConstants.end(), pConstant) == vConstants.end())
                vConstants.push_back(pConstant);
        }

        return name;
    }

    void GenerateConstants()
    {
        if (Configuration::UsingConstants)
        {
            FILE* cFile = nullptr;
            char charBuffer[256] = { NULL };

            sprintf_s(charBuffer, "%s\\%s\\SdkConstants.h", Configuration::GeneratorDirectory.c_str(), Configuration::GameNameShort.c_str());
            fopen_s(&cFile, charBuffer, "w+");

            fprintf(cFile, "#pragma once");

            for (int i = 0; i < UObject::GObjObjects()->Num(); i++)
            {
                UObject* uObject = UObject::GObjObjects()->Data[i];

                if (uObject && uObject->IsA(UFunction::StaticClass()))
                {
                    UFunction* uFunction = (UFunction*)uObject;
                    std::string name = GenerateUniqueName(uFunction, (UClass*)uFunction->Outer, true);

                    std::pair<std::string, int> pConstant = std::make_pair(name, uObject->ObjectInternalInteger);

                    if (std::find(vConstants.begin(), vConstants.end(), pConstant) == vConstants.end())
                        vConstants.push_back(pConstant);
                }
            }

            for (size_t i = 0; i < vConstants.size(); i++)
                fprintf(cFile, "\n#define %-150s %d", vConstants[i].first.c_str(), vConstants[i].second);

            fclose(cFile);
        }
    }

    void GenerateHeaders()
    {
        FILE* hFile = nullptr;
        char charBuffer[256] = { NULL };

        sprintf_s(charBuffer, "%s\\%s\\SdkHeaders.h", Configuration::GeneratorDirectory.c_str(), Configuration::GameNameShort.c_str());
        fopen_s(&hFile, charBuffer, "w+");

        Printers::PrintHeader(hFile, "SdkHeaders", "h", false);
        fprintf(hFile, "#pragma once\n");

        Printers::PrintSection(hFile, "Includes");

        fprintf(hFile, "#include \"GameDefines.h\"\n");

        for (size_t i = 0; i < vIncludes.size(); i++)
        {
            fprintf(hFile, "#include \"SDK_HEADERS\\%s_structs.h\"\n", vIncludes[i]->GetName());
            fprintf(hFile, "#include \"SDK_HEADERS\\%s_classes.h\"\n", vIncludes[i]->GetName());
            fprintf(hFile, "#include \"SDK_HEADERS\\%s_parameters.h\"\n", vIncludes[i]->GetName());
        }

        fprintf(hFile, "\n");

        Printers::PrintFooter(hFile, false);

        fclose(hFile);
    }

    void GenerateDefines()
    {
        FILE* dFile = nullptr;
        char charBuffer[256] = { NULL };

        sprintf_s(charBuffer, "%s\\%s\\GameDefines.h", Configuration::GeneratorDirectory.c_str(), Configuration::GameNameShort.c_str());
        fopen_s(&dFile, charBuffer, "w+");

        Printers::PrintHeader(dFile, "GameDefines", "h", false);

        fprintf(dFile, "#pragma once\n");
        fprintf(dFile, "#include <algorithm>\n");
        fprintf(dFile, "#include <locale>\n");
        fprintf(dFile, "#include <stdlib.h>\n");
        fprintf(dFile, "#include <xlocale>\n");
        fprintf(dFile, "#include <ctype.h>\n");
        fprintf(dFile, "#include <chrono>\n");
        fprintf(dFile, "#include <thread>\n");
        fprintf(dFile, "#include <map>\n\n");

        fprintf(dFile, "// GObjects\n");
        fprintf(dFile, "#define GObjects_Pattern		  (const unsigned char*)\"%s\"\n", Configuration::GObjectsPattern);
        fprintf(dFile, "#define GObjects_Mask 			  (const char*)\"%s\"\n", Configuration::GObjectsMask);
        fprintf(dFile, "#define GObjects_Offset 		  (uintptr_t)0x%" PRIxPTR "\n", Configuration::GObjectsOffset);
        fprintf(dFile, "// GNames\n");
        fprintf(dFile, "#define GNames_Pattern 			  (const unsigned char*)\"%s\"\n", Configuration::GNamesPattern);
        fprintf(dFile, "#define GNames_Mask 			  (const char*)\"%s\"\n", Configuration::GNamesMask);
        fprintf(dFile, "#define GNames_Offset 			  (uintptr_t)0x%" PRIxPTR "\n", Configuration::GNamesOffset);

        Printers::PrintSection(dFile, "Defines");

        fprintf(dFile, "%s", TArray_Struct.c_str());

        Printers::PrintSection(dFile, "Globals");

        fprintf(dFile, "extern TArray<class UObject*>* GObjects;\n");
        fprintf(dFile, "extern TArray<struct FNameEntry*>* GNames;\n");

        Printers::PrintSection(dFile, "Structs");

        fprintf(dFile, "%s\n", FNameEntry_Struct.c_str());
        fprintf(dFile, "%s\n", FName_Struct.c_str());
        fprintf(dFile, "%s\n", FString_Struct.c_str());
        fprintf(dFile, "%s\n", FScriptDelegate_Struct.c_str());
        fprintf(dFile, "%s\n", FPointer_Struct.c_str());

        Printers::PrintFooter(dFile, false);

        fclose(dFile);

        sprintf_s(charBuffer, "%s\\%s\\GameDefines.cpp", Configuration::GeneratorDirectory.c_str(), Configuration::GameNameShort.c_str());
        fopen_s(&dFile, charBuffer, "w+");

        Printers::PrintHeader(dFile, "GameDefines", "cpp", false);
        fprintf(dFile, "#include \"GameDefines.h\"\n\n");
       
        Printers::PrintSection(dFile, "Initialize Global Variables");

        fprintf(dFile, "TArray<class UObject*>* GObjects{};\n");
        fprintf(dFile, "TArray<struct FNameEntry*>* GNames{};");

        fclose(dFile);
    }

    void ProcessPackages()
    {
        std::vector<UObject*> vPackages;

        for (int i = 0; i < UObject::GObjObjects()->Num(); i++)
        {
            UObject* uObject = UObject::GObjObjects()->Data[i];

            if (!uObject)
                continue;

            if (uObject->IsA(UClass::StaticClass()))
            {
                UObject* uPackageObject = uObject->GetPackageObj();

                if (!uPackageObject)
                    continue;

                if (find(vPackages.begin(), vPackages.end(), uPackageObject) == vPackages.end())
                {
                    vPackages.push_back(uPackageObject);

                    FILE* currentFile = nullptr;
                    char charBuffer[256] = { NULL };

                    // Structs
                    sprintf_s(charBuffer, "%s\\%s\\SDK_HEADERS\\%s_structs.h", Configuration::GeneratorDirectory.c_str(), Configuration::GameNameShort.c_str(), uPackageObject->GetName());
                    fopen_s(&currentFile, charBuffer, "w+");

                    sprintf_s(charBuffer, "%s_structs", uPackageObject->GetName());
                    Printers::PrintHeader(currentFile, charBuffer, "h", true);

                    Printers::PrintSection(currentFile, "Script Structs");
                    StructGenerator::ProcessStructs(currentFile, uPackageObject);

                    Printers::PrintFooter(currentFile, true);
                    fclose(currentFile);

                    // Classes
                    sprintf_s(charBuffer, "%s\\%s\\SDK_HEADERS\\%s_classes.h", Configuration::GeneratorDirectory.c_str(), Configuration::GameNameShort.c_str(), uPackageObject->GetName());
                    fopen_s(&currentFile, charBuffer, "w+");

                    sprintf_s(charBuffer, "%s_classes", uPackageObject->GetName());
                    Printers::PrintHeader(currentFile, charBuffer, "h", true);

                    Printers::PrintSection(currentFile, "Constants");
                    ConstGenerator::ProcessConsts(currentFile, uPackageObject);

                    Printers::PrintSection(currentFile, "Enums");
                    EnumGenerator::ProcessEnums(currentFile, uPackageObject);

                    Printers::PrintSection(currentFile, "Classes");
                    ClassGenerator::ProcessClasses(currentFile, uPackageObject);

                    Printers::PrintFooter(currentFile, true);
                    fclose(currentFile);

                    // Parameters
                    sprintf_s(charBuffer, "%s\\%s\\SDK_HEADERS\\%s_parameters.h", Configuration::GeneratorDirectory.c_str(), Configuration::GameNameShort.c_str(), uPackageObject->GetName());
                    fopen_s(&currentFile, charBuffer, "w+");

                    sprintf_s(charBuffer, "%s_parameters", uPackageObject->GetName());
                    Printers::PrintHeader(currentFile, charBuffer, "h", true);

                    Printers::PrintSection(currentFile, "Function Parameters");
                    ParameterGenerator::ProcessParameters(currentFile, uPackageObject);

                    Printers::PrintFooter(currentFile, true);
                    fclose(currentFile);

                    // Functions
                    sprintf_s(charBuffer, "%s\\%s\\SDK_HEADERS\\%s_classes.cpp", Configuration::GeneratorDirectory.c_str(), Configuration::GameNameShort.c_str(), uPackageObject->GetName());
                    fopen_s(&currentFile, charBuffer, "w+");

                    sprintf_s(charBuffer, "%s_classes", uPackageObject->GetName());
                    Printers::PrintHeader(currentFile, charBuffer, "cpp", true);

                    Printers::PrintSection(currentFile, "Functions");
                    FunctionGenerator::ProcessFunctions(currentFile, uPackageObject);

                    Printers::PrintFooter(currentFile, true);
                    fclose(currentFile);
                }
            }
        }
    }

    void GenerateSDK()
    {
        SYSTEMTIME stST, stET;
        FILETIME ftST, ftET;
        ULARGE_INTEGER nST, nET;

        float fDiff;
        char charBuffer[256] = { NULL };

        _mkdir(Configuration::GeneratorDirectory.c_str());
        sprintf_s(charBuffer, "%s\\%s", Configuration::GeneratorDirectory.c_str(), Configuration::GameNameShort.c_str());
        _mkdir(charBuffer);
        sprintf_s(charBuffer, "%s\\%s\\SDK_HEADERS", Configuration::GeneratorDirectory.c_str(), Configuration::GameNameShort.c_str());
        _mkdir(charBuffer);
        sprintf_s(charBuffer, "%s\\%s\\UE3SDKGenerator.log", Configuration::GeneratorDirectory.c_str(), Configuration::GameNameShort.c_str());
        fopen_s(&logFile, charBuffer, "w+");

        GetSystemTime(&stST);
        MessageBox(NULL, (LPCWSTR)L"SDK generation has started, do not close the game until prompted to do so!", (LPCWSTR)L"UE3SDKGenerator", MB_ICONINFORMATION | MB_OK);
        Initialize(logFile);
        ProcessPackages();
        GenerateConstants();
        GenerateHeaders();
        GenerateDefines();

        GetSystemTime(&stET);
        SystemTimeToFileTime(&stST, &ftST);
        nST.HighPart = ftST.dwHighDateTime;
        nST.LowPart = ftST.dwLowDateTime;
        SystemTimeToFileTime(&stET, &ftET);
        nET.HighPart = ftET.dwHighDateTime;
        nET.LowPart = ftET.dwLowDateTime;

        fDiff = (nET.QuadPart - nST.QuadPart) / 10000.0f / 1000.0f;

        fprintf(logFile, "\nSDK Generated in %.3f seconds", fDiff);
        fclose(logFile);

        MessageBox(NULL, (LPCWSTR)L"Finished! It is now safe to close the game.", (LPCWSTR)L"UE3SDKGenerator", MB_ICONINFORMATION | MB_OK);
    }

    void DumpInstances()
    {
        Initialize(NULL);

        uintptr_t baseAddress = (uintptr_t)GetModuleHandle(NULL);

        char buffer[256] = { NULL };
        _mkdir(Configuration::GeneratorDirectory.c_str());
        sprintf_s(buffer, "%s\\%s", Configuration::GeneratorDirectory.c_str(), Configuration::GameNameShort.c_str());
        _mkdir(buffer);

        if (true)
        {
            FILE* oFile = nullptr;
            char cBuffer[256] = { NULL };
            sprintf_s(cBuffer, "%s\\%s\\ObjectDump.txt", Configuration::GeneratorDirectory.c_str(), Configuration::GameNameShort.c_str());
            fopen_s(&oFile, cBuffer, "w+");
            fprintf(oFile, "Base: 0x%" PRIxPTR "\n", baseAddress);
            fprintf(oFile, "GObjects: 0x%" PRIxPTR "\n\n", GObjects);

            for (int i = 0; i < UObject::GObjObjects()->Num(); i++)
            {
                if (!UObject::GObjObjects()->Data[i])
                    continue;

                fprintf(oFile, "UObject[%06i] %-50s 0x%" PRIxPTR "\n", i, GObjects->Data[i]->GetName(), GObjects->Data[i]);
            }

            fclose(oFile);
        }

        if (true)
        {
            FILE* nFile = nullptr;
            char nBuffer[256] = { NULL };
            sprintf_s(nBuffer, "%s\\%s\\NameDump.txt", Configuration::GeneratorDirectory.c_str(), Configuration::GameNameShort.c_str());
            fopen_s(&nFile, nBuffer, "w+");
            fprintf(nFile, "Base: 0x%" PRIxPTR "\n", baseAddress);
            fprintf(nFile, "GNames: 0x%" PRIxPTR "\n\n", GNames);

            for (int i = 0; i < FName::Names()->Num(); i++)
            {
                if (!FName::Names()->Data[i])
                    continue;

                std::wstring ws(FName::Names()->Data[i]->Name);
                std::string str(ws.begin(), ws.end());

                fprintf(nFile, "Name[%06i] %-50s 0x%" PRIxPTR "\n", i, str.c_str(), FName::Names()->Data[i]);
            }

            fclose(nFile);
        }

        if (true)
        {
            FILE* fFile = nullptr;
            char fBuffer[256] = { NULL };
            sprintf_s(fBuffer, "%s\\%s\\FullNameDump.txt", Configuration::GeneratorDirectory.c_str(), Configuration::GameNameShort.c_str());
            fopen_s(&fFile, fBuffer, "w+");
            fprintf(fFile, "Base: 0x%" PRIxPTR "\n", baseAddress);
            fprintf(fFile, "GObjects: 0x%" PRIxPTR "\n\n", GObjects);

            for (int i = 0; i < UObject::GObjObjects()->Num(); i++)
            {
                if (!UObject::GObjObjects()->Data[i])
                    continue;

                fprintf(fFile, "UObject[%06i] %-50s 0x%" PRIxPTR "\n", i, UObject::GObjObjects()->Data[i]->GetFullName(), UObject::GObjObjects()->Data[i]);
            }

            fclose(fFile);
        }
    }

    void Initialize(FILE* file)
    {
        static bool initialized = false;

        if (!initialized)
        {
            uintptr_t baseAddress = (uintptr_t)GetModuleHandle(NULL);

            if (!Configuration::UsingOffset)
            {
                uintptr_t GObjectsAddress = Utils::FindPattern(GetModuleHandle(NULL), Configuration::GObjectsPattern, Configuration::GObjectsMask);
                uintptr_t GNamesAddress = Utils::FindPattern(GetModuleHandle(NULL), Configuration::GNamesPattern, Configuration::GNamesMask);
                GObjects = (TArray<UObject*>*)GObjectsAddress;
                GNames = (TArray<FNameEntry*>*)GNamesAddress;
            } 
            else
            {
                uintptr_t baseAddress = (uintptr_t)GetModuleHandle(NULL);
                uintptr_t GObjectsAddress = baseAddress + Configuration::GObjectsOffset;
                uintptr_t GNamesAddress = baseAddress + Configuration::GNamesOffset;
                GObjects = (TArray<UObject*>*)GObjectsAddress;
                GNames = (TArray<FNameEntry*>*)GNamesAddress;
            }

            if (file)
            {
                fprintf(file, "Base: 0x%" PRIxPTR "\n", baseAddress);
                fprintf(file, "GObjects: 0x%" PRIxPTR "\n", GObjects);
                fprintf(file, "GNames: 0x%" PRIxPTR "\n\n", GNames);
            }

            initialized = true;
        }
    }
}

void onAttach(HMODULE hModule)
{
    DisableThreadLibraryCalls(hModule);
    //Generator::DumpInstances();
    Generator::GenerateSDK();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(onAttach), nullptr, 0, nullptr);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}