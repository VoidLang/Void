#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

#include <chrono>

#include <vector>
#include <map>

#include <filesystem>

#include <fstream>

#include <optional>

#include <any>

#include <stdarg.h>

#include <mutex>

#include <functional>

#include <ctime>
#include <cstdlib>

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

#define LONG __int64

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

#define MILLISECONDS std::chrono::milliseconds
#define NANOSECONDS std::chrono::nanoseconds
#define DURATION_CAST std::chrono::duration_cast
#define SYSTEM_CLOCK std::chrono::system_clock

#define currentTimeMillis() \
    (DURATION_CAST<MILLISECONDS>(SYSTEM_CLOCK::now().time_since_epoch()).count())

#define nanoTime() \
    (DURATION_CAST<NANOSECONDS>(SYSTEM_CLOCK::now().time_since_epoch()).count())

// this will generate a number from 0.0 to 1.0, inclusive
#define random() \
    (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))

// this will generate a number from 0.0 to some arbitrary float, max
#define randomTo(max) \
    (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/max)))

// this will generate a number from some arbitrary min to some arbitrary max
#define rangomRange(min, max) \
    (min + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX/(max-min))))