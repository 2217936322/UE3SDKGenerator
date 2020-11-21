#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

namespace Printers
{
	void Print(FILE* file, std::ostringstream& stream);
	void Print(FILE* file, const std::string& string);
	void EmptyStream(std::ostringstream& stream);
	void MakeSpacer(std::ostringstream& stream, uint32_t size);
	void MakeSpacer(std::ostringstream& stream, std::string string, uint32_t size);
	void MakeSpacer(std::ostringstream& streamIn, std::ostringstream& streamOut, uint32_t size);
	void MakeDecimal(std::ostringstream& streamIn, uintptr_t address, uint32_t size);
	void MakeHex(std::ostringstream& stream, uintptr_t address, uint32_t size);
	void PrintHeader(FILE* file, const std::string& fileName, const std::string& fileExtension, bool pragmaPackPush);
	void PrintSection(FILE* file, const std::string& sectionName);
	void PrintFooter(FILE* file, bool pragmaPackPop);
}