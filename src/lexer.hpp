#pragma once

#include <iostream>
#include <cstring>
#include <vector>
#include <malloc.h>
#include <map>

#include "errors.src.hpp"
#include "prepare.src.hpp"

using namespace std;

namespace Fannst::TemplatingEngine
{
	typedef enum
	{
		// Types of commands
		LT_MODE_INSERT_TXT = 0,
		LT_MODE_FUNC,
		LT_MODE_INSERT_HTML,

		// Syntax types
		LT_NAME,
		LT_SYMBOL,

		// Stuff like strings
		LT_STRING,

		// Other stuff
		LT_INVALID,
		LT_CONTENT,
		LT_COMMAND
	} LexType;

	typedef enum
    {
	    TV_NONE = 0,
	    TV_BOOL,
	    TV_CHAR_POINTER,
	    TV_INT,
	    TV_DOUBLE,
	    TV_FLOAT,
	    TV_LONG
    } TvType;

	typedef struct
    {
	    TvType t_Type{TvType::TV_NONE};
	    const void *t_Val{nullptr};
    } TemplateVariable;

	typedef struct
	{
		LexType l_LexType{LT_INVALID};
		char *l_Content{nullptr};
	} LexPart;

    int performLexicalAnalysisCommand(const char **raw, std::vector<LexPart> &result);

    int performLexicalAnalysis(const char *raw, std::vector<LexPart> &result);

    int useCommand(const std::vector<LexPart> &parts, char **result, std::map<const char *, TemplateVariable> &variables, TemplateErrorLog &log);

	int render(const char *filename, std::map<const char *, TemplateVariable> &variables, char **ret, TemplateErrorLog &log);

    int getVariableFromMap(const char *key, std::map<const char *, TemplateVariable> &map, TemplateVariable **target);
}
