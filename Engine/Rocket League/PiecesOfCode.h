#pragma once
#include <string>

/*
# ========================================================================================= #
# Pieces Of Code																			#
# ========================================================================================= #
*/

std::string TArray_Struct =
"template<class T> struct TArray\n"
"{\n"
"public:\n"
"\tT* Data;\n"
"\tint32_t Count;\n"
"\tint32_t Max;\n"
"public:\n"
"\tTArray()\n"
"\t{\n"
"\t\tData = nullptr;\n"
"\t\tCount = Max = 0;\n"
"\t}\n"
"public:\n"
"\tT& operator[](int index)\n"
"\t{\n"
"\t\treturn Data[index];\n"
"\t}\n\n"

"\tconst T& operator[](int index) const\n"
"\t{\n"
"\t\treturn Data[index];\n"
"\t}\n\n"

"\tvoid Add(T data)\n"
"\t{\n"
"\t\tData = (T*)realloc(Data, sizeof(T) * (Count + 1));\n"
"\t\tData[Count++] = data;\n"
"\t\tMax = Count;\n"
"\t}\n\n"

"\tvoid Clear()\n"
"\t{\n"
"\t\tfree(Data);\n"
"\t\tCount = Max = 0;\n"
"\t}\n\n"

"\tint Num()\n"
"\t{\n"
"\t\treturn Count;\n"
"\t}\n"
"};\n";

std::string FNameEntry_Struct =
"struct FNameEntry\n"
"{\n"
"\tunsigned char\tUnknownData00[0x18];\n"
"\twchar_t\t\t\tName[0x100];\n"
"};\n";

std::string FName_Struct =
"struct FName\n"
"{\n"
"public:\n"
"\tint32_t\t\tIndex;\n"
"\tint32_t\t\t\Number;\n"
"public:\n"
"\tFName()\n"
"\t{\n"
	"\t\tIndex = Number = 0;\n"
"\t}\n\n"

"\tFName(int i)\n"
"\t{\n"
	"\t\tIndex = i;\n"
	"\t\tNumber = 0;\n"
"\t}\n\n"

"\t~FName() {};\n"
"public:\n"
"\tbool operator == (const FName& name) const\n"
"\t{\n"
"\t\treturn (Index == name.Index);\n"
"\t}\n\n"

"\tstatic TArray<FNameEntry*>* Names()\n"
"\t{\n"
"\t\tTArray<FNameEntry*>* GNamesArray = (TArray<FNameEntry*>*)GNames;\n"
"\t\treturn GNamesArray;\n"
"\t}\n\n"

"\tFName(const wchar_t* findName)\n"
"\t{\n"
"\t\tstatic TArray<int> nameCache;\n"
"\t\tfor (int i = 0; i < nameCache.Num(); i++)\n"
"\t\t{\n"
"\t\t\tif (Names()->Data[i])\n"
"\t\t\t{\n"
"\t\t\t\tif (!wcscmp(Names()->Data[nameCache[i]]->Name, findName))\n"
"\t\t\t\t{\n"
"\t\t\t\t\tIndex = nameCache[i];\n"
"\t\t\t\t\treturn;\n"
"\t\t\t\t}\n"
"\t\t\t}\n"
"\t\t}\n\n"

"\t\tfor (int i = 0; i < Names()->Num(); i++)\n"
"\t\t{\n"
"\t\t\tif (Names()->Data[i])\n"
"\t\t\t{\n"
"\t\t\t\tif (!wcscmp(Names()->Data[i]->Name, findName))\n"
"\t\t\t\t{\n"
"\t\t\t\t\tnameCache.Add(i);\n"
"\t\t\t\t\tIndex = i;\n"
"\t\t\t\t}\n"
"\t\t\t}\n"
"\t\t}\n"
"\t}\n\n"

"\tstd::string GetName()\n"
"\t{\n"
"\t\tif (Index < 0 || Index > Names()->Num())\n"
"\t\t{\n"
"\t\t\treturn std::string(\"UnknownName\");\n"
"\t\t}\n"
"\t\telse\n"
"\t\t{\n"
"\t\t\tstd::wstring ws(Names()->Data[Index]->Name);\n"
"\t\t\tstd::string str(ws.begin(), ws.end());\n"
"\t\t\treturn str;\n"
"\t\t}\n"
"\t}\n"
"};\n";

std::string FScriptDelegate_Struct =
"struct FScriptDelegate\n"
"{\n"
"\tunsigned char UnknownData00[0x18];\n"
"};\n";

std::string FPointer_Struct =
"struct FPointer\n"
"{\n"
"\tuintptr_t Dummy;\n"
"};\n";

std::string FQWord_Struct =
"struct FQWord\n"
"{\n"
"\tint A;\n"
"\tint B;\n"
"};\n";

std::string FString_Struct =
"struct FString : public TArray<const wchar_t>\n"
"{\n"
"public:\n"
"\tFString() {};\n\n"

"\t~FString() {};\n"
"public:\n"
"\tFString(const wchar_t* other)\n"
"\t{\n"
"\t\tthis->Max = this->Count = *other ? (wcslen(other) + 1) : 0;\n\n"
"\t\tif (this->Count)\n"
"\t\t\tthis->Data = other;\n"
"\t}\n\n"

"\tFString operator = (const wchar_t* other)\n"
"\t{\n"
"\t\tif (this->Data != other)\n"
"\t\t{\n"
"\t\t\tthis->Max = this->Count = *other ? (wcslen(other) + 1) : 0;\n\n"
"\t\t\tif (this->Count)\n"
"\t\t\t\tthis->Data = other;\n"
"\t\t}\n\n"
"\t\treturn *this;\n"
"\t}\n\n"

"\tstd::string ToString() const\n"
"\t{\n"
"\t\tif (this->Data)\n"
"\t\t{\n"
"\t\t\tstd::wstring ws(this->Data);\n"
"\t\t\tstd::string str(ws.begin(), ws.end());\n"
"\t\t\treturn str;\n"
"\t\t}\n"
"\t\telse\n"
"\t\t{\n"
"\t\t\treturn std::string(\"null\");\n"
"\t\t}\n"
"\t};\n"
"};\n";

std::string UClass_Fields =
"\tclass UField*\tNext;\t// 0x0060 (0x08) NOT AUTO-GENERATED PROPERTY\n"
"\tunsigned char\tUnknownData00[0x8];\t// 0x0068 (0x08) NOT AUTO-GENERATED PROPERTY\n";

std::string UStruct_Fields =
"\tunsigned char\tUnknownData00[0x10];\t// 0x0070 (0x10) NOT AUTO-GENERATED PROPERTY\n"
"\tclass UField*\tSuperField;\t// 0x0080 (0x08) NOT AUTO-GENERATED PROPERTY\n"
"\tclass UField*\tChildren;\t// 0x0088 (0x08) NOT AUTO-GENERATED PROPERTY\n"
"\tunsigned long\tPropertySize;\t// 0x0090 (0x08) NOT AUTO-GENERATED PROPERTY\n"
"\tunsigned char\tUnknownData01[0x98];\t// 0x0094 (0x9C) NOT AUTO-GENERATED PROPERTY\n";

std::string UFunction_Fields =
"\tunsigned long\tFunctionFlags;\t// NOT AUTO-GENERATED PROPERTY\n"
"\tunsigned short\tiNative;\t// NOT AUTO-GENERATED PROPERTY\n"
"\tunsigned short\tRepOffset;\t// NOT AUTO-GENERATED PROPERTY\n"
"\tstruct FName\tFriendlyName;\t// NOT AUTO-GENERATED PROPERTY\n"
"\tunsigned short\tNumParms;\t// NOT AUTO-GENERATED PROPERTY\n"
"\tunsigned short\tParmsSize;\t// NOT AUTO-GENERATED PROPERTY\n"
"\tunsigned long\tReturnValueOffset;\t// NOT AUTO-GENERATED PROPERTY\n"
"\tunsigned char\tUnknownData00[0x8];\t// NOT AUTO-GENERATED PROPERTY\n"
"\tvoid*\tFunc;\t// NOT AUTO-GENERATED PROPERTY \n";

std::string UObject_FunctionDescriptions =
"\tstatic TArray<UObject*>* GObjObjects();\n\n"

"\tchar const* GetName();\n"
"\tchar const* GetNameCPP();\n"
"\tchar const* GetFullName();\n"
"\t//char const* GetPackageName();\n"
"\tUObject* GetPackageObj();\n\n"

"\ttemplate<class T> static T* FindObject(char const* objectFullName);\n"
"\ttemplate<class T> static unsigned int CountObject(char const* objectName);\n"
"\tstatic UClass* FindClass(char const* classFullName);\n\n"

"\tbool IsA(UClass* pClass);\n"
"\tbool IsA(int objInternalInteger);\n\n";

std::string UFunction_FunctionDescriptions =
"\tstatic UFunction* FindFunction(char const* functionFullName);\n";

std::string UObject_Functions =
"TArray<UObject*>* UObject::GObjObjects()\n"
"{\n"
"\tTArray<UObject*>* ObjectArray = (TArray<UObject*>*)GObjects;\n"
"\treturn ObjectArray;\n"
"}\n\n"

"char const* UObject::GetName()\n"
"{\n"
"\tstatic char cOutBuffer[256];\n"
"\tmemset(cOutBuffer, 0, sizeof cOutBuffer);\n"
"\tstd::string name = this->Name.GetName();\n"
"\tsprintf_s(cOutBuffer, \"%s\", name.c_str());\n\n"

"\treturn cOutBuffer;\n"
"}\n\n"

"char const* UObject::GetNameCPP()\n"
"{\n"
	"\tstatic char cOutBuffer[256];\n\n"

	"\tif (this->IsA(UClass::StaticClass()))\n"
	"\t{\n"
		"\t\tUClass* pClass = (UClass*)this;\n\n"

		"\t\twhile (pClass)\n"
		"\t\t{\n"
			"\t\t\tstd::string pClassName = pClass->GetName();\n\n"

			"\t\t\tif (pClassName == \"Actor\")\n"
			"\t\t\t{\n"
				"\t\t\t\tstrcpy_s(cOutBuffer, \"A\");\n"
				"\t\t\t\tbreak;\n"
			"\t\t\t}\n"
			"\t\t\telse if (pClassName == \"Object\")\n"
			"\t\t\t{\n"
				"\t\t\t\tstrcpy_s(cOutBuffer, \"U\");\n"
				"\t\t\t\tbreak;\n"
			"\t\t\t}\n\n"

			"\t\t\tpClass = (UClass*)pClass->SuperField;\n"
		"\t\t}\n"
	"\t}\n"
	"\telse\n"
	"\t{\n"
		"\t\tstrcpy_s(cOutBuffer, \"F\");\n"
	"\t}\n\n"

	"\tstrcat_s(cOutBuffer, this->GetName());\n\n"

	"\treturn cOutBuffer;\n"
"}\n\n"

"char const* UObject::GetFullName()\n"
"{\n"
	"\tif (this->Class && this->Outer)\n"
	"\t{\n"
		"\t\tstatic char cOutBuffer[512];\n"
		"\t\tchar cTmpBuffer[512];\n"
		"\t\tstrcpy_s(cOutBuffer, this->GetName());\n\n"

		"\t\tfor (UObject* pOuter = this->Outer; pOuter; pOuter = pOuter->Outer)\n"
		"\t\t{\n"
			"\t\t\tstrcpy_s(cTmpBuffer, pOuter->GetName());\n"
			"\t\t\tstrcat_s(cTmpBuffer, \".\");\n\n"

			"\t\t\tsize_t len1 = strlen(cTmpBuffer);\n"
			"\t\t\tsize_t len2 = strlen(cOutBuffer);\n\n"

			"\t\t\tmemmove(cOutBuffer + len1, cOutBuffer, len1 + len2 + 1);\n"
			"\t\t\tmemcpy(cOutBuffer, cTmpBuffer, len1);\n"
		"\t\t}\n\n"

		"\t\tstrcpy_s(cTmpBuffer, this->Class->GetName());\n"
		"\t\tstrcat_s(cTmpBuffer, \" \");\n\n"

		"\t\tsize_t len1 = strlen(cTmpBuffer);\n"
		"\t\tsize_t len2 = strlen(cOutBuffer);\n\n"

		"\t\tmemmove(cOutBuffer + len1, cOutBuffer, len1 + len2 + 1);\n"
		"\t\tmemcpy(cOutBuffer, cTmpBuffer, len1);\n\n"

		"\t\treturn cOutBuffer;\n"
	"\t}\n\n"

	"\treturn \"(null)\";\n"
"}\n\n"

"UObject* UObject::GetPackageObj()\n"
"{\n"
	"\tUObject* pPackage = nullptr;\n\n"

	"\tfor (UObject* pOuter = this->Outer; pOuter; pOuter = pOuter->Outer)\n"
		"\t\tpPackage = pOuter;\n\n"

	"\treturn pPackage;\n"
"}\n\n"

"template<class T> T* UObject::FindObject(char const* objectFullName)\n"
"{\n"
	"\tfor (int i = 0; i < UObject::GObjObjects()->Num(); i++)\n"
	"\t{\n"
		"\t\tUObject* Object = UObject::GObjObjects()->Data[i];\n\n"

		"\t\tif (!Object || !Object->IsA(T::StaticClass()))\n"
			"\t\t\tcontinue;\n\n"

		"\t\tif (Object->GetFullName() == objectFullName)\n"
			"\t\t\treturn (T*)Object;\n"
	"\t}\n\n"

	"\treturn nullptr;\n"
"}\n\n"

"template<class T> static unsigned int UObject::CountObject(char const* objectName)\n"
"{\n"
	"\tstatic std::map<std::string, int> mCountCache;\n"
	"\tstd::string sObjectName = objectName;\n\n"

	"\tif (mCountCache.find(sObjectName) == mCountCache.end())\n"
	"\t{\n"
		"\t\tmCountCache[sObjectName] = 0;\n\n"

		"\t\tfor (int i = 0; i < UObject::GObjObjects()->Num(); i++)\n"
		"\t\t{\n"
			"\t\t\tUObject* pObject = UObject::GObjObjects()->Data[i];\n\n"

			"\t\t\tif (!pObject || !pObject->IsA(T::StaticClass()))\n"
				"\t\t\t\tcontinue;\n\n"

			"\t\t\tif (pObject->GetName() == objectName)\n"
				"\t\t\t\tmCountCache[std::string(pObject->GetName())]++;\n"
		"\t\t}\n"
	"\t}\n\n"

	"\treturn mCountCache[sObjectName];\n"
"}\n\n"

"UClass* UObject::FindClass(char const* classFullName)\n"
"{\n"
	"\twhile (!UObject::GObjObjects())\n"
		"\t\tstd::this_thread::sleep_for(std::chrono::milliseconds(100));\n\n"

	"\twhile (!FName::Names())\n"
		"\t\tstd::this_thread::sleep_for(std::chrono::milliseconds(100));\n\n"

	"\tstatic bool initialized = false;\n"
	"\tstatic std::map<std::string, UClass*> loadedClasses{};\n\n"

	"\tif (!initialized)\n"
	"\t{\n"
		"\t\tfor (int i = 0; i < UObject::GObjObjects()->Num(); i++)\n"
		"\t\t{\n"
			"\t\t\tUObject* object = UObject::GObjObjects()->Data[i];\n\n"

			"\t\t\tif (object)\n"
			"\t\t\t{\n"
				"\t\t\t\tstd::string objectFullName = object->GetFullName();\n\n"

				"\t\t\t\tif (objectFullName.find(\"Class\") != std::string::npos)\n"
					"\t\t\t\t\tloadedClasses[objectFullName] = (UClass*)object;\n"
			"\t\t\t}\n"
		"\t\t}\n\n"

		"\t\tinitialized = true;\n"
	"\t}\n\n"

	"\tif (loadedClasses.find(classFullName) != loadedClasses.end())\n"
		"\t\treturn loadedClasses[classFullName];\n\n"

	"\treturn nullptr;\n"
"\t}\n\n"

"bool UObject::IsA(UClass* pClass)\n"
"{\n"
	"\tfor (UClass* SuperClass = this->Class; SuperClass; SuperClass = (UClass*)SuperClass->SuperField)\n"
	"\t{\n"
		"\t\tif (SuperClass == pClass)\n"
			"\t\treturn true;\n"
	"\t}\n\n"

	"\treturn false;\n"
"}\n\n"

"bool UObject::IsA(int objInternalInteger)\n"
"{\n"
	"\tUClass* pClass = UObject::GObjObjects()->Data[objInternalInteger]->Class;\n\n"

	"\tif (pClass)\n"
		"\t\treturn this->IsA(pClass);\n\n"

	"\treturn false;\n"
"}\n\n";

std::string UFunction_Functions =
"UFunction* UFunction::FindFunction(char const* functionFullName)\n"
"{\n"
	"\twhile (!UObject::GObjObjects())\n"
		"\t\tstd::this_thread::sleep_for(std::chrono::milliseconds(100));\n\n"

	"\twhile (!FName::Names())\n"
		"\t\tstd::this_thread::sleep_for(std::chrono::milliseconds(100));\n\n"

	"\tstatic bool initialized = false;\n"
	"\tstatic std::map<std::string, UClass*> loadedFunctions{};\n\n"

	"\tif (!initialized)\n"
	"\t{\n"
		"\t\tfor (int i = 0; i < UObject::GObjObjects()->Num(); i++)\n"
		"\t\t{\n"
				"\t\t\tUObject* object = UObject::GObjObjects()->Data[i];\n\n"

				"\t\t\tif (object)\n"
				"\t\t\t{\n"
				"\t\t\t\tstd::string objectFullName = object->GetFullName();\n\n"

				"\t\t\t\tif (objectFullName.find(\"Function\") != std::string::npos)\n"
				"\t\t\t\t\loadedFunctions[objectFullName] = (UClass*)object;\n"
				"\t\t\t}\n"
			"\t\t}\n\n"

		"\t\tinitialized = true;\n"
	"\t}\n\n"

	"\tif (loadedFunctions.find(functionFullName) != loadedFunctions.end())\n"
		"\t\treturn loadedFunctions[functionFullName];\n\n"

	"\treturn nullptr;\n"
"\t}\n\n";