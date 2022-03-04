#pragma once

#include <any>

#define VNIEXPORT __declspec(dllexport)
#define VNIIMPORT __declspec(dllimport)
#define VNICALL __stdcall

#define ANY std::any
#define CAST std::any_cast

#define TYPE type_info

class VNIEnv
{
};

class VObject
{
};

class VNIParams
{
public:
	ANY* elements;
	int size;

	VNIParams(ANY* elements, int size) : elements(elements), size(size) {}

	template <typename T>
	T get(int index)
	{
		return CAST<T>(elements[index]);
	}

	ANY raw(int index)
	{
		return elements[index];
	}

	const TYPE& type(int index)
	{
		return elements[index].type();
	}
};


typedef void(__stdcall* VOID_CALL_STATIC)(VNIEnv* env, VNIParams* params);
typedef int (__stdcall* INT_CALL_STATIC) (VNIEnv* env, VNIParams* params);

typedef void(__stdcall* VOID_CALL)(VNIEnv* env, VObject* self, VNIParams* params);
typedef int (__stdcall* INT_CALL) (VNIEnv* env, VObject* self, VNIParams* params);
