//
// File: SchemeInterpreter.h
// =========================
//

#ifndef __SCHEMEINTERPRETER_H__
#define __SCHEMEINTERPRETER_H__

#include <cstdlib>
#include <cctype>

typedef unsigned int VARIABLETYPE;
#define VARIABLETYPE_INVALID	0xFFFF
#define VARIABLETYPE_INTEGER	0x0001
#define VARIABLETYPE_FLOAT		0x0002
#define VARIABLETYPE_SYMBOL		0x0003
#define VARIABLETYPE_CONS		0x0005

typedef unsigned long MEMORY_ADDR;
#define MEMORY_INVALID	0xFFFFFFFF
#define MEMORYPOOL_SIZE	65535

struct SchemeCons
{
	MEMORY_ADDR a;
	MEMORY_ADDR b;
};

struct Variable
{
	void *ptr;
	size_t size;
	VARIABLETYPE type;
	unsigned int ref_count;
};

class MemoryPool
{
private:
	Variable *_list[MEMORYPOOL_SIZE];

public:
	MemoryPool(void);
	~MemoryPool(void);

	MEMORY_ADDR createVariable(void);
	MEMORY_ADDR createCopiedVariable(MEMORY_ADDR addr);

	bool setValue(MEMORY_ADDR addr, const void *content, size_t size, VARIABLETYPE type);
	bool setValue(MEMORY_ADDR addr, int n);
	bool setValue(MEMORY_ADDR addr, float f);
	bool setValue(MEMORY_ADDR addr, const wchar_t *symbol);
	bool setValue(MEMORY_ADDR addr, bool b);
	bool setValue(MEMORY_ADDR addr, SchemeCons *cons);

	VARIABLETYPE getVariableType(MEMORY_ADDR addr);
	bool getIntegerValue(MEMORY_ADDR addr, int &n);
	bool getFloatValue(MEMORY_ADDR addr, float &f);
	bool getSymbolValue(MEMORY_ADDR addr, wchar_t *symbol, size_t buffer_size);
	bool getBoolValue(MEMORY_ADDR addr, bool &b);
	bool getSchemeConsValue(MEMORY_ADDR addr, SchemeCons **cons);

	void destroyVariable(MEMORY_ADDR addr);
};

class SchemeList
{
private:
	MEMORY_ADDR _addr;

public:
	SchemeList(MEMORY_ADDR a);
	~SchemeList(void);

	void addItem(MEMORY_ADDR addr);
	void removeLastItem(void);
	void reset(MEMORY_ADDR a);
};

class SchemeInterpreter
{
private:
	MemoryPool _mempool;

private:
	VARIABLETYPE determineElementType(const wchar_t *str)
	{
		bool floating = false;
		while(str)
		{
			if(!isdigit(*str)) {
				if(*str == '.') {
					floating = true;
				} else {
					return VARIABLETYPE_SYMBOL;
				}
			}
			++str;
		}
		if(floating) return VARIABLETYPE_FLOAT;
		return VARIABLETYPE_INTEGER;
	}

	bool addElementToList(const wchar_t *element, VARIABLETYPE type, SchemeList *list)
	{
		if(element == NULL || list == NULL) return false;
		MEMORY_ADDR addr = _mempool.createVariable(void);
		if(addr == MEMORY_INVALID) return false;
		switch(type)
		{
		case VARIABLETYPE_SYMBOL:
			if(!_mempool.setValue(addr, element)) return false;
			break;
		case VARIABLETYPE_INTEGER:
			if(!_mempool.setValue(addr, _wtoi(element))) return false;
			break;
		ca