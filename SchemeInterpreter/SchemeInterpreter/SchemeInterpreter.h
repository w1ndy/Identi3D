//
// File: SchemeInterpreter.h
// =========================
//

#ifndef __SCHEMEINTERPRETER_H__
#define __SCHEMEINTERPRETER_H__

#include "SchemeDef.h"
#include "SchemeMemoryPool.h"
#include "SchemeSyntax.h"

class SchemeInterpreter
{
private:
	bool preprocess(const wchar_t *input, SyntaxTree &tree);
	bool evaluate(const SyntaxTree &tree);

public:
	bool execute(const wchar_t *input);
};

#endif // __SCHEMEINTERPRETER_H__
