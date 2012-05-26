//
// File: SchemeMemoryPool.h
// ========================
//

#ifndef __SCHEMEMEMORYPOOL_H__
#define __SCHEMEMEMORYPOOL_H__

#include "SchemeDef.h"

class MemoryPool
{
private:
	Variable *_pool;
	MEMORY_ADDR _now_alloc;

public:
	MemoryPool(void);
	~MemoryPool(void);

	bool initialize(void);
	void release(void);

	MEMORY_ADDR createVariable(void);
	MEMORY_ADDR createCopiedVariable(MEMORY_ADDR addr);
	void destroyVariable(MEMORY_ADDR addr);
	inline bool isValidAddress(MEMORY_ADDR addr) { return (addr > 0 && addr < _now_alloc); };

	bool setValue(MEMORY_ADDR addr, const void *content, unsigned int size);
	bool setValue(MEMORY_ADDR addr, long n);
	bool setValue(MEMORY_ADDR addr, float f);
	bool setValue(MEMORY_ADDR addr, const wchar_t *symbol);
	bool setValue(MEMORY_ADDR addr, SCHEME_PAIR pair);

	VARIABLE_TYPE getVariableType(MEMORY_ADDR addr);
	bool getIntegerValue(MEMORY_ADDR addr, long &n);
	bool getFloatValue(MEMORY_ADDR addr, float &f);
	bool getSymbolValue(MEMORY_ADDR addr, wchar_t *symbol, size_t buffer_size);
	bool getPairValue(MEMORY_ADDR addr, SCHEME_PAIR &pair);
	bool getCustomValue(MEMORY_ADDR addr, void **ptr);

	void printAllocationStatus(void);
};

#endif // __SCHEMEMEMORYPOOL_H__