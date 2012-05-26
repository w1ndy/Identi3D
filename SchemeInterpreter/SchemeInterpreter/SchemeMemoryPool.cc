//
// File: SchemeMemoryPool.cc
// =========================
//

#include "SchemeMemoryPool.h"

#include <iostream>
#include <cstdlib>
#include <cstring>

MemoryPool::MemoryPool(void)
{
	_pool = NULL;
	_now_alloc = MEMORY_ADDR_INVALID;
}

MemoryPool::~MemoryPool(void)
{
	release();
}

bool MemoryPool::initialize(void)
{
	if(_pool) {
		std::cerr << "memory pool has already been initialized." << std::endl;
		return false;
	}
	_pool = (Variable *)::operator new(sizeof(Variable) * MEMORYPOOL_SIZE);
	if(_pool == NULL) {
		std::cerr << "insufficient memory while acquiring " << 
			sizeof(Variable) * MEMORYPOOL_SIZE << " bytes space." << std::endl;
		return false;
	}
	_now_alloc = 0;
	return true;
}

void MemoryPool::release(void)
{
	if(_pool) ::operator delete(_pool);
}

MEMORY_ADDR MemoryPool::createVariable(void)
{
	if(!_pool) {
		std::cerr << "memory pool uninitialized." << std::endl;
		return MEMORY_ADDR_INVALID;
	}
	if(_now_alloc >= MEMORYPOOL_SIZE) {
		std::cerr << "insufficient heap space while allocating variable." << std::endl;
		return MEMORY_ADDR_INVALID;
	}
	memset(_pool + _now_alloc, 0, sizeof(Variable));
	return _now_alloc++;
}

MEMORY_ADDR MemoryPool::createCopiedVariable(MEMORY_ADDR addr)
{
	if(!isValidAddress(addr)) {
		std::cerr << "unable to make a copy of undefined variable." << std::endl;
		return MEMORY_ADDR_INVALID;
	}
	MEMORY_ADDR new_addr = createVariable(), addr1, addr2;
	unsigned long pair;
	if(!isValidAddress(new_addr)) return MEMORY_ADDR_INVALID;

	switch(getVariableType(addr))
	{
	case VARIABLE_TYPE_INTEGER:
		setValue(new_addr, *(_pool + addr)->data);
		break;
	case VARIABLE_TYPE_FLOAT:
		setValue(new_addr, (float)(*(_pool + addr)->data));
		break;
	case VARIABLE_TYPE_SYMBOL:
		setValue(new_addr, (wchar_t *)(*(_pool + addr)->ptr));
		break;
	case VARIABLE_TYPE_PAIR:
		pair = (SCHEME_PAIR)(*(_pool + addr)->data);
		addr1 = createCopiedVariable(SCHEME_PAIR_FORMER_ADDR(pair));
		addr2 = createCopiedVariable(SCHEME_PAIR_LATTER_ADDR(pair));
		if(!isValidAddress(addr1) || !isValidAddress(addr2)) {
			std::cerr << "bad pair." << std::endl;
			destroyVariable(new_addr);
			return false;
		}
		setValue(new_addr, MAKE_PAIR(addr1, addr2));
		break;
	default:
		setValue(new_addr, *(_pool + addr)->ptr, VARIABLE_GET_SIZE(*(_pool + addr)->info));
	}
	return new_addr;
}