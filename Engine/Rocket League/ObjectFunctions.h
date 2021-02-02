//#ifdef _MSC_VER 
//#define strncasecmp _strnicmp
//#define strcasecmp _stricmp
//#endif

#include "GameDefines.h"

/*
# ========================================================================================= #
# Object Functions																			#
# ========================================================================================= #
*/

TArray<UObject*>* UObject::GObjObjects()
{
	TArray<UObject*>* ObjectArray = (TArray<UObject*>*)GObjects;
	return ObjectArray;
}

char const* UObject::GetName()
{
	static char cOutBuffer[1024];
	memset(cOutBuffer, 0, sizeof cOutBuffer);
	std::string name = this->Name.GetName();
	sprintf_s(cOutBuffer, "%s", name.c_str());

	return cOutBuffer;
}

char const* UObject::GetNameCPP()
{
	static char cOutBuffer[1024];

	if (this->IsA(UClass::StaticClass()))
	{
		UClass* pClass = (UClass*)this;

		while (pClass)
		{
			std::string pClassName = pClass->GetName();

			if (pClassName == "Actor")
			{
				strcpy_s(cOutBuffer, "A");
				break;
			}
			else if (pClassName == "Object")
			{
				strcpy_s(cOutBuffer, "U");
				break;
			}

			pClass = (UClass*)pClass->SuperField;
		}
	}
	else
	{
		strcpy_s(cOutBuffer, "F");
	}

	strcat_s(cOutBuffer, this->GetName());

	return cOutBuffer;
}

char const* UObject::GetFullName()
{
	if (this->Class && this->Outer)
	{
		static char cOutBuffer[1024];
		char cTmpBuffer[1024];
		strcpy_s(cOutBuffer, this->GetName());

		for (UObject* pOuter = this->Outer; pOuter; pOuter = pOuter->Outer)
		{
			strcpy_s(cTmpBuffer, pOuter->GetName());
			strcat_s(cTmpBuffer, ".");

			size_t len1 = strlen(cTmpBuffer);
			size_t len2 = strlen(cOutBuffer);

			memmove(cOutBuffer + len1, cOutBuffer, len1 + len2 + 1);
			memcpy(cOutBuffer, cTmpBuffer, len1);
		}

		strcpy_s(cTmpBuffer, this->Class->GetName());
		strcat_s(cTmpBuffer, " ");

		size_t len1 = strlen(cTmpBuffer);
		size_t len2 = strlen(cOutBuffer);

		memmove(cOutBuffer + len1, cOutBuffer, len1 + len2 + 1);
		memcpy(cOutBuffer, cTmpBuffer, len1);

		return cOutBuffer;
	}

	return "(null)";
}

char const* UObject::GetPackageName()
{
	UObject* pPackage = this->GetPackageObj();

	if (pPackage)
	{
		static char cOutBuffer[256];
		strcpy_s(cOutBuffer, pPackage->GetName());
		return cOutBuffer;
	}
	
	return "(null)";
}

UObject* UObject::GetPackageObj()
{
	UObject* pPackage = nullptr;

	for (UObject* pOuter = this->Outer; pOuter; pOuter = pOuter->Outer)
		pPackage = pOuter;

	return pPackage;
}

template<typename T> T* UObject::FindObject(char const* objectFullName)
{
	for (int i = 0; i < UObject::GObjObjects()->Num(); i++)
	{
		UObject* Object = UObject::GObjObjects()->Data[i];

		if (!Object || !Object->IsA(T::StaticClass()))
			continue;

		if (Object->GetFullName() == objectFullName)
			return (T*)Object;
	}

	return nullptr;
}

template<typename T> static unsigned int UObject::CountObject(char const* objectName)
{
	static std::map<std::string, int> mCountCache;
	std::string sObjectName = objectName;

	if (mCountCache.find(sObjectName) == mCountCache.end())
	{
		mCountCache[sObjectName] = 0;

		for (int i = 0; i < UObject::GObjObjects()->Num(); i++)
		{
			UObject* pObject = UObject::GObjObjects()->Data[i];

			if (!pObject || !pObject->IsA(T::StaticClass()))
				continue;

			if (pObject->GetName() == objectName)
				mCountCache[std::string(pObject->GetName())]++;
		}
	}

	return mCountCache[sObjectName];
}

UClass* UObject::FindClass(char const* classFullName)
{
	while (!UObject::GObjObjects())
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	while (!FName::Names())
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	static bool initialized = false;
	static std::map<std::string, UClass*> loadedClasses{};

	if (!initialized)
	{
		for (int i = 0; i < UObject::GObjObjects()->Num(); i++)
		{
			UObject* object = UObject::GObjObjects()->Data[i];

			if (object)
			{
				std::string objectFullName = object->GetFullName();

				if (objectFullName.find("Class") != std::string::npos)
					loadedClasses[objectFullName] = (UClass*)object;
			}
		}

		initialized = true;
	}

	if (loadedClasses.find(classFullName) != loadedClasses.end())
		return loadedClasses[classFullName];

	return nullptr;
}

bool UObject::IsA(UClass* pClass)
{
	for (UClass* SuperClass = this->Class; SuperClass; SuperClass = (UClass*)SuperClass->SuperField)
	{
		if (SuperClass == pClass)
			return true;
	}

	return false;
}

bool UObject::IsA(int objInternalInteger)
{
	UClass* pClass = UObject::GObjObjects()->Data[objInternalInteger]->Class;

	if (pClass)
		return this->IsA(pClass);

	return false;
}
