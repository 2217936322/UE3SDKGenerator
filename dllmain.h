#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <inttypes.h>
#include <stdint.h>
#include <direct.h>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <vector>

#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

#include "Engine/Rocket League/GameDefines.h"
#include "Engine/Rocket League/ObjectFunctions.h"
#include "Engine/Rocket League/PiecesOfCode.h"
#include "Engine/Rocket League/Configuration.h"

/*
	TO-DO:
	- More detailed log
	- memcpy_s instead of memcpy
	- Make it look less shit
	- reference for out params
*/

namespace Utils
{
	MODULEINFO GetModuleInfo(LPCTSTR lpModuleName);
	uintptr_t FindPattern(HMODULE module, const unsigned char* pattern, const char* mask);
	uintptr_t FindPattern(uintptr_t startAddres, uintptr_t fileSize, const unsigned char* pattern, const char* mask);
	bool MapExists(std::multimap<std::string, std::string>& StrStrMm, std::string sKey, std::string sValue);
	bool SortPropertyPair(std::pair<UProperty*, std::string> pPropertyA, std::pair<UProperty*, std::string> pPropertyB);
	bool SortProperty(UProperty* pPropertyA, UProperty* pPropertyB);
	bool IsBitField(EPropertyTypes propertyType);
	bool IsBitField(uint32_t arrayDim);
}

namespace Retrievers
{
	void GetAllPropertyFlags(std::ostringstream& stream, int propertyFlags);
	void GetAllFunctionFlags(std::ostringstream& stream, int functionFlags);
	EPropertyTypes GetPropertyType(UProperty* uProperty, std::string& sPropertyType, bool returnFunction);
	size_t GetPropertySize(UProperty* uProperty);
}

namespace StructGenerator
{
	UScriptStruct* FindLargestStruct(std::string structFullName);
	void GenerateStruct(FILE* file, UScriptStruct* uScriptStruct);
	void GenerateStructProperties(FILE* file, UScriptStruct* uScriptStruct, UObject* pPackageObj);
	void ProcessStructs(FILE* file, UObject* uPackageObj);
}

namespace ConstGenerator
{
	void GenerateConst(FILE* file, UConst* uConst);
	void ProcessConsts(FILE* file, UObject* uPackageObj);
}

namespace EnumGenerator
{
	void GenerateEnum(FILE* file, UEnum* uEnum);
	void ProcessEnums(FILE* file, UObject* uPackageObj);
}

namespace ClassGenerator
{
	void GenerateClass(FILE* file, UClass* uClass);
	void GenerateClassProperties(FILE* file, UClass* uClass, UObject* uPackageObj);
	void ProcessClasses(FILE* file, UObject* uPackageObj);
}

namespace ParameterGenerator
{
	void GenerateParameter(FILE* file,  UClass* uClass);
	void ProcessParameters(FILE* file, UObject* uPackageObj);
}

namespace FunctionGenerator
{
	void GenerateVirtualFunction(FILE* file, UClass* uClass);
	void GenerateFunctionCode(FILE* file,  UClass* uClass);
	void GenerateFunctionDescription(FILE* file,  UClass* uClass);
	void ProcessFunctions(FILE* file, UObject* uPackageObj);
}

namespace Generator
{
	extern FILE* logFile;
	extern std::vector<std::pair<std::string, int>> vConstants;
	extern std::vector<UObject*> vIncludes;

	std::string GenerateValidName(const std::string& invalidName);
	std::string GenerateUniqueName(UClass* uClass, bool useIndex);
	std::string GenerateUniqueName(UFunction* uFunction, UClass* uClass, bool useIndex);
	void GenerateConstants();
	void GenerateHeaders();
	void GenerateDefines();
	void ProcessPackages();
	void GenerateSDK();
	void DumpInstances();
	void Initialize(FILE* file);
}