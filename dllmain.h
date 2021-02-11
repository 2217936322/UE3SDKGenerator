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
#include <filesystem>

#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

#include "Engine/Rocket League/GameDefines.h"
#include "Engine/Rocket League/ObjectFunctions.h"
#include "Engine/Rocket League/PiecesOfCode.h"
#include "Engine/Rocket League/Configuration.h"

/*
	TO-DO:
	- Double check all property sizes before writing them, there seems to be a mismatch sometimes and I can't figure out why.
	- Figure out better method for GenerateVirtualFunctions.
	- Add an option to use ProcessEventOffset to generate virtual functions.
*/

namespace Utils
{
	MODULEINFO GetModuleInfo(LPCTSTR lpModuleName);
	uintptr_t FindPattern(HMODULE module, const unsigned char* pattern, const char* mask);
	uintptr_t FindPattern(uintptr_t startAddres, uintptr_t fileSize, const unsigned char* pattern, const char* mask);
	bool MapExists(std::multimap<std::string, std::string>& map, std::string& key, std::string& value);
	bool SortPropertyPair(std::pair<UProperty*, std::string> uPropertyA, std::pair<UProperty*, std::string> pPropertyB);
	bool SortProperty(UProperty* uPropertyA, UProperty* pPropertyB);
	bool IsBitField(EPropertyTypes propertyType);
	bool IsBitField(uint32_t arrayDim);
	bool AreGObjectsValid();
	bool AreGNamesValid();
}

namespace Retrievers
{
	void GetAllPropertyFlags(std::ostringstream& stream, int propertyFlags);
	void GetAllFunctionFlags(std::ostringstream& stream, int functionFlags);
	EPropertyTypes GetPropertyType(UProperty* uProperty, std::string& uPropertyType, bool returnFunction);
	size_t GetPropertySize(UProperty* uProperty);
}

namespace StructGenerator
{
	UScriptStruct* FindLargestStruct(std::string& structFullName);
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
	void GenerateVirtualFunctions(FILE* file);
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
	std::string GenerateUniqueName(UClass* uClass);
	std::string GenerateUniqueName(UFunction* uFunction, UClass* uClass);
	std::string GenerateIndexName(UObject* uObject, bool pushBack);

	void GenerateConstants();
	void GenerateHeaders();
	void GenerateDefines();
	void ProcessPackages();
	void GenerateSDK();
	void DumpInstances();
	void Initialize(FILE* file);
}