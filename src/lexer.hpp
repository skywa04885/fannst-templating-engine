#pragma once

#include <iostream>
#include <cstring>
#include <vector>
#include <malloc.h>
#include <map>

#include "errors.src.hpp"
#include "prepare.src.hpp"

using namespace std;

#define PIC_SP_FLAG     0b00000001      // Flag for special flag check
#define PIC_SCM_FLAG    0b00000010      // Flag to indicate heap allocation should be freed
#define PIC_ERR_FLAG    0b00000100      // The flag for an possible error

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

    int useCommand(const std::vector<LexPart> &parts, char **result, std::map<const char *, TemplateVariable> &variables,
            TemplateErrorLog &log, uint8_t *flags, const char *file);

	int render(const char *filename, std::map<const char *, TemplateVariable> &variables, char **ret, TemplateErrorLog &log);

    int getVariableFromMap(const char *key, std::map<const char *, TemplateVariable> &map, TemplateVariable **target);
}
