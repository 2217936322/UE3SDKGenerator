#pragma once
#include "GameDefines.h"

/*
# ========================================================================================= #
# Object Functions																			#
# ========================================================================================= #
*/

TArray<class UObject*>* UObject::GObjObjects()
{
	TArray<class UObject*>* objectArray = reinterpret_cast<TArray<class UObject*>*>(GObjects);
	return objectArray;
}

char const* UObject::GetName()
{
	static char charBuffer[512];
	memset(charBuffer, 0, sizeof charBuffer);
	std::string name = this->Name.ToString();
	sprintf_s(charBuffer, "%s", name.c_str());

	return charBuffer;
}

char const* UObject::GetNameCPP()
{
	static char charBuffer[512];

	if (this->IsA(UClass::StaticClass()))
	{
		UClass* uClass = reinterpret_cast<UClass*>(this);

		while (uClass)
		{
			std::string className = uClass->GetName();

			if (className == "Actor")
			{
				strcpy_s(charBuffer, "A");
				break;
			}
			else if (className == "Object")
			{
				strcpy_s(charBuffer, "U");
				break;
			}

			uClass = reinterpret_cast<UClass*>(uClass->SuperField);
		}
	}
	else
	{
		strcpy_s(charBuffer, "F");
	}

	strcat_s(charBuffer, this->GetName());

	return charBuffer;
}

char const* UObject::GetFullName()
{
	if (this->Class && this->Outer)
	{
		static char charBuffer[1024];
		char tempBuffer[1024];
		strcpy_s(charBuffer, this->GetName());

		for (UObject* uOuter = this->Outer; uOuter; uOuter = uOuter->Outer)
		{
			strcpy_s(tempBuffer, uOuter->GetName());
			strcat_s(tempBuffer, ".");

			size_t len1 = strlen(tempBuffer);
			size_t len2 = strlen(charBuffer);

			memmove(charBuffer + len1, charBuffer, len1 + len2 + 1);
			memcpy(charBuffer, tempBuffer, len1);
		}

		strcpy_s(tempBuffer, this->Class->GetName());
		strcat_s(tempBuffer, " ");

		size_t len1 = strlen(tempBuffer);
		size_t len2 = strlen(charBuffer);

		memmove(charBuffer + len1, charBuffer, len1 + len2 + 1);
		memcpy(charBuffer, tempBuffer, len1);

		return charBuffer;
	}

	return "null";
}

char const* UObject::GetPackageName()
{
	UObject* uPackageObj = this->GetPackageObj();

	if (uPackageObj)
	{
		static char charBuffer[512];
		strcpy_s(charBuffer, uPackageObj->GetName());
		return charBuffer;
	}
	
	return "null";
}

UObject* UObject::GetPackageObj()
{
	UObject* uPackage = nullptr;

	for (UObject* uOuter = this->Outer; uOuter; uOuter = uOuter->Outer)
	{
		uPackage = uOuter;
	}

	return uPackage;
}

template<typename T> T* UObject::FindObject(char const* objectFullName)
{
	for (UObject* uObject : *UObject::GObjObjects())
	{
		if (uObject && uObject->IsA(T::StaticClass()))
		{
			if (uObject->GetFullName() == objectFullName)
			{
				return reinterpret_cast<T*>(uObject);
			}
		}
	}

	return nullptr;
}

template<typename T> static unsigned int UObject::CountObject(char const* objectName)
{
	static std::map<std::string, int> countCache;

	if (countCache.find(objectName) == countCache.end())
	{
		countCache[objectName] = 0;

		for (UObject* uObject : *UObject::GObjObjects())
		{
			if (uObject && uObject->IsA(T::StaticClass()))
			{
				if (uObject->GetName() == objectName)
				{
					countCache[std::string(uObject->GetName())]++;
				}
			}
		}
	}

	return countCache[objectName];
}

UClass* UObject::FindClass(char const* classFullName)
{
	static bool initialized = false;
	static std::map<std::string, UClass*> foundClasses{};

	if (!initialized)
	{
		for (UObject* uObject : *UObject::GObjObjects())
		{
			if (uObject)
			{
				std::string objectFullName = uObject->GetFullName();

				if (objectFullName.find("Class") == 0)
				{
					foundClasses[objectFullName] = reinterpret_cast<UClass*>(uObject);
				}
			}
		}

		initialized = true;
	}

	if (foundClasses.find(classFullName) != foundClasses.end())
	{
		return foundClasses[classFullName];
	}

	return nullptr;
}

bool UObject::IsA(UClass* uClass)
{
	for (UClass* uSuperClass = this->Class; uSuperClass; uSuperClass = reinterpret_cast<UClass*>(uSuperClass->SuperField))
	{
		if (uSuperClass == uClass)
		{
			return true;
		}
	}

	return false;
}

bool UObject::IsA(int objInternalInteger)
{
	UClass* uClass = UObject::GObjObjects()->At(objInternalInteger)->Class;

	if (uClass)
	{
		return this->IsA(uClass);
	}

	return false;
}

UFunction* UFunction::FindFunction(char const* functionFullName)
{
	static bool initialized = false;
	static std::map<std::string, UFunction*> foundFunctions{};

	if (!initialized)
	{
		for (UObject* uObject : *UObject::GObjObjects())
		{
			if (uObject)
			{
				std::string objectFullName = uObject->GetFullName();

				if (objectFullName.find("Function") == 0)
				{
					foundFunctions[objectFullName] = reinterpret_cast<UFunction*>(uObject);
				}
			}
		}

		initialized = true;
	}

	if (foundFunctions.find(functionFullName) != foundFunctions.end())
	{
		return foundFunctions[functionFullName];
	}

	return nullptr;
}