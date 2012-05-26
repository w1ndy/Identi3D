//
// File: SchemeSyntax.h
// ====================
//

#ifndef __SCHEMESYNTAX_H__
#define __SCHEMESYNTAX_H__

#include "SchemeDef.h"

#include <iterator>

// SYNTAX ELEMENT TYPE ID
typedef unsigned int SE_TYPE_ID;
#define SE_TYPE_SYMBOL	0x0001
#define SE_TYPE_TREE	0x0002

class SyntaxElement
{
public:
	SyntaxElement(void) {};
	virtual ~SyntaxElement(void) {};

	virtual SE_TYPE_ID getTypeId(void) const = 0;
};

struct SyntaxElementList
{
	SyntaxElement *elem;
	SyntaxElementList *next;
};

class SyntaxSymbol : public SyntaxElement
{
private:
	wchar_t name[SCHEME_SYMBOL_MAX_LEN];

public:
	SyntaxSymbol(void) {};
	SyntaxSymbol(const wchar_t *symbol);
	SyntaxSymbol(const SyntaxSymbol &symbol);
	~SyntaxSymbol(void) {};

	bool setSymbolName(const wchar_t *symbol);
	const wchar_t *getSymbolName(void);

	SyntaxSymbol &operator=(const SyntaxSymbol *rhs);
	SE_TYPE_ID getTypeId(void) const { return SE_TYPE_SYMBOL; }
};

class SyntaxTree : public SyntaxElement
{
private:
	SyntaxElementList *_list;

public:
	class const_iterator : public std::iterator<std::input_iterator_tag, SyntaxElement>
	{
		const SyntaxElementList *ptr;
	public:
		const_iterator(const SyntaxElementList *list) : ptr(list) {};
		const_iterator(const const_iterator &iter) : ptr(iter.ptr) {};
		bool is_valid(void) const { return (ptr != NULL); };
		const_iterator &operator++(void) { if(is_valid()) ptr = ptr->next; return *this; };
		const_iterator operator++(int) { const_iterator iter(*this); operator++(); return iter; };
		bool operator==(const const_iterator &iter) { return (ptr == iter.ptr); }
		bool operator!=(const const_iterator &iter) { return (ptr != iter.ptr); }
		const SyntaxElement &operator*(void) const { if(is_valid()) return *(ptr->elem); throw std::runtime_error("Dereference null pointer"); return *(ptr->elem); };
		const SyntaxElement &operator->(void) const { if(is_valid()) return *(ptr->elem); throw std::runtime_error("Dereference null pointer"); return *(ptr->elem); };
	};
	
	const const_iterator &begin(void) { return const_iterator(_list); };
	const const_iterator &end(void) { return const_iterator(NULL); };

public:
	SyntaxTree(void);
	SyntaxTree(const SyntaxTree &tree);
	~SyntaxTree(void);

	bool attachSyntaxSymbol(const SyntaxSymbol &symbol);
	bool attachSyntaxTree(const SyntaxTree &tree);
	void removeLastElement(void);
	void reset(void);

	SyntaxTree &operator=(const SyntaxTree &rhs);
	SE_TYPE_ID getTypeId(void) const { return SE_TYPE_TREE; };
};

#endif // __SCHEMESYNTAX_H__


