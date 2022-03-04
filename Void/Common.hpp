#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

#include <vector>
#include <map>

#include <filesystem>

#include <fstream>

#include <optional>

#include <any>

#include <stdarg.h>

#include <mutex>

#include <functional>

#include <tchar.h>
#include <system_error>

#define NOMINMAX
#include <windows.h>

#include "vni.h"

#define STRING std::string
#define WSTRING std::wstring

#define LIST std::vector<std::string>
#define LIST_T std::vector
#define CONTAINS(list, element) \
	std::find(list.begin(), list.end(), element) != list.end()

#define MAP std::map

#define PATH std::filesystem::path

#define DIR_ITERATOR std::filesystem::directory_iterator
#define DIR_ENTRY std::filesystem::directory_entry
#define IS_DIRECTORY std::filesystem::is_directory
#define EXISTS std::filesystem::exists

#define FILE_INPUT_STREAM std::ifstream
#define FILE_OUTPUT_STREAM std::ofstream

#define GET_LINE std::getline

#define OPTIONAL_T std::optional

#define SIZE_T size_t

#define ANY std::any

#define ANY_CAST std::any_cast

#define MUTEX std::recursive_mutex

#define LOCK std::unique_lock

#define DEFER_LOCK std::defer_lock

#define STRING_STREAM std::stringstream

#define UNDERLYING_TYPE std::underlying_type

#define TO_STRING std::to_string

#define SET_PRECISION std::setprecision

#define FUNCTION std::function

#define UPPERCASE(x) \
	std::toupper(x, std::locale())

#define LOWERCASE(x) \
	std::tolower(x, std::locale())

#define println(x) \
	std::cout << x << '\n'

#define print(x) \
	std::cout << x

#define error(x) \
	std::cout << x << '\n'; exit(-1);

//#define instanceof(obj, T) \
//	T* x = dynamic_cast<T*>(obj)

#define instanceof(obj, T) \
	((bool) (dynamic_cast<T*>(obj) != nullptr))

#define GET_LINE std::getline

#define CONSOLE_IN std::cin

#define SYSTEM_CATEGORY std::system_category

#define OUTPUT_STREAM std::ostream

#define INDENT std::setw