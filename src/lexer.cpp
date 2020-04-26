#include "lexer.hpp"

namespace Fannst::TemplatingEngine
{
	void prepareCommand(char **target)
	{
		// Creates the start buffer
		char *buffer = reinterpret_cast<char *>(malloc(strlen(&(*target)[0]) + 1));
		std::size_t bufferLen = 0;

		// Starts the processing
		bool lww = false;
		for (char *p = &(*target)[0]; *p != '\0'; p++)
		{
			if (*p == ' ')
			{
				if (!lww) lww = true;
				else if (lww) continue;
			} else lww = false;

			*(&buffer[bufferLen]) = *p;
			bufferLen++;
		}

		// Removes the spaces at start and end
		if (*(&buffer[0]) == ' ') memmove(&buffer[0], &buffer[1], bufferLen);
		if (*(&buffer[bufferLen-1]) == ' ') *(&buffer[bufferLen-1]) = '\0';

		// Stores the result
		free(*target);
		*target = reinterpret_cast<char *>(malloc(bufferLen + 1));
		memcpy(&(*target)[0], &buffer[0], bufferLen + 1);

		// ----
		// Free the memory
		// ----

		// Free's the buffer
		free(buffer);
	}

    int performLexicalAnalysisCommand(char **raw,
		std::vector<LexPart> &result)
	{
        int rc = 0;
		char *buffer = reinterpret_cast<char *>(malloc(strlen(&(*raw)[0]) + 1));
		std::size_t bufferLen = 0;

		// ----
		// Preprocess the data
		// ----

		// Removes the whitespace
		prepareCommand(raw);

		// ----
		// Starts the analysis
		// ----

		bool stringStarted = false;
		bool symbolStarted = false;
		for (char *p = &(*raw)[0]; *p != '\0'; p++)
		{
		    // Performs the symbol stuff
            if (symbolStarted && isalpha(*p) != 0)
            { // It is an valid symbol
                // Appends the char
                *(&buffer[bufferLen]) = *p;
                // Increments the buffer len
                bufferLen++;
                // Checks if it almost is the end
                if (*(p+1) != '\0') continue;
            }

            if (symbolStarted || *(p+1) == '\0')
            { // End the symbol
                // Sets the null termination char
                *(&buffer[bufferLen]) = '\0';
                // Allocate the memory for the copy
                char *t = reinterpret_cast<char *>(malloc(bufferLen + 1));
                // Copies the memory
                memcpy(&t[0], &buffer[0], bufferLen + 1);
                // Pushes it to the result
                result.emplace_back(LexPart{
                    LexType::LT_NAME,
                    &t[0]
                });
                // Resets the buffer
                bufferLen = 0;
                *(&buffer[0]) = '\0';
                // Ends the symbol
                symbolStarted = false;
                // Skips
                continue;
            }

			// If whitespace, just continue, and store result
			if (*p == ' ' && !stringStarted)
			{
				continue;
			}

			// Handles the current char
			if (!stringStarted)
			{
				if (*p == '$' || *p == ')' || *p == '(')
				{
					// Allocates two octets
					char *t = reinterpret_cast<char *>(malloc(2));
					// Sets the null termination char, and the char it self
					*(&t[1]) = '\0';
					// Copy the char
					memcpy(&t[0], p, 1);
					// Stores the result
					result.emplace_back(LexPart{
						LexType::LT_SYMBOL,
						&t[0]
					});
				} else if (*p == ':')
				{
					result.emplace_back(LexPart{
						LexType::LT_MODE_FUNC,
						nullptr
					});
				} else if (*p == '=')
				{
					result.emplace_back(LexPart{
						LexType::LT_MODE_INSERT_TXT,
						nullptr
					});
				} else if (*p == '-')
				{
					result.emplace_back(LexPart{
						LexType::LT_MODE_INSERT_HTML,
						nullptr
					});
				} else if (*p == '"')
				{
					stringStarted = true;
				} else if (isalpha(*p) != 0)
				{
                    // Appends the char
                    *(&buffer[bufferLen]) = *p;
                    // Increments the buffer len
                    bufferLen++;
                    // Sets the symbol started to true
                    symbolStarted = true;
				} else
				{
					rc = -1;
					goto PLAC_End;
				}
			}
		}

	PLAC_End:

	    // ----
	    // Free the memory
	    // ----

	    // Free's the buffer
	    free(buffer);

		return rc;
	}

	int getVariableFromMap(const char *key, std::map<const char *, TemplateVariable> &map, TemplateVariable **target)
    {
	    for (std::map<const char *, TemplateVariable>::iterator it = map.begin(); it != map.end(); it++)
        {
	        if (strcmp(&it->first[0], &key[0]) == 0)
            {
	            *target = &it->second;
	            return 0;
            }
        }

	    return -1;
    }

    int useCommand(const std::vector<LexPart> &parts, char **result, std::map<const char *, TemplateVariable> &variables, TemplateErrorLog &log)
    {
        int rc = 0;

	    // Checks the command type, if it is not valid, return error code
	    if (parts.at(0).l_LexType == LexType::LT_MODE_INSERT_TXT)
        {
	        // Checks if it is an insertion command
	        if (parts.at(1).l_LexType == LexType::LT_SYMBOL && strcmp(&parts.at(1).l_Content[0], "$") == 0)
            { // It is an insert command
	            // Gets the variable
	            TemplateVariable *var = nullptr;

                // Gets the variable names
	            if (getVariableFromMap(parts.at(2).l_Content, variables, &var))
                {
	                // Sets the error message
	                log.t_TotalErrors++;
	                log.t_Errors.emplace_back(TemplateError{
	                    "Variable not defined !"
	                });

	                // Returns the error message
                    generateHtmlError("Syntax Error", "Undefined Variable", result);

	                // ----
	                // Prints the console error, and finishes
	                // ----

	                rc = -1;
	                goto UC_End;
                }

	            // Prepares the data, so it is html acceptable
	            char *ret = nullptr;
                makeTextHTML5Tolerant(reinterpret_cast<char *>(const_cast<void *>(var->t_Val)), &ret);

	            // Inserts the variable
	            *result = ret;

	            // Sets the return code to -1, so we need to free memory
	            rc = -1;
            }
        } else if (parts.at(0).l_LexType == LexType::LT_MODE_FUNC)
        {

        } else if (parts.at(0).l_LexType == LexType::LT_MODE_INSERT_HTML)
        {

        } else
        { // It is invalid
	        rc = -1;
        }

	UC_End:

	    return rc;
    }

    int performLexicalAnalysis(const char *raw,
		std::vector<LexPart> &result)
	{
		char *buffer = reinterpret_cast<char *>(malloc(strlen(&raw[0]) + 1));
		*(&buffer[0]) = '\0';
		std::size_t bufferLen = 0;

        int rc = 0;

		// ----
		// Starts the loop
		// ----

		bool commandStarted = false;
		for (const char *p = &raw[0]; *p != '\0'; p++)
		{
			// ----
			// Checks the content
			// ----

			// Checks if a command has started \
			by checking if the previous char was an '{' \
			and the current char is an '%'
			if (*(p - 1) == '{' && *p == '%')
			{
				// Checks if there is non-command data to append, if so push it \
				to the result vector as LT_CONTENT
				if (bufferLen > 0)
				{
					char *contentPtr = nullptr;

					// Check if we need to copy, with or without the '}'
					if (*(&buffer[0]) == '}')
					{
						// Allocates the required memory for the buffer copy, without \
						the first char
						contentPtr = reinterpret_cast<char *>(malloc(bufferLen));
						// Copies the content, without the last char
						memcpy(&contentPtr[0], &buffer[1], bufferLen);
						// Removes the '{' if there
						if (*(&contentPtr[bufferLen-2]) == '{') *(&contentPtr[bufferLen-2]) = '\0';
					} else
					{
						// Allocates the required memory for the buffer copy
						contentPtr = reinterpret_cast<char *>(malloc(bufferLen + 1));
						// Copies the content
						memcpy(&contentPtr[0], &buffer[0], bufferLen + 1);
						// Removes the '{' if there
						if (*(&contentPtr[bufferLen-1]) == '{') *(&contentPtr[bufferLen-1]) = '\0';
					}
					// Pushes it to the result
					result.emplace_back(LexPart{
						LexType::LT_CONTENT,
						&contentPtr[0]
					});

					// Resets the buffer len
					bufferLen = 0;

					// Resets the buffer
					*(&buffer[0]) = '\0';
				}

				// Sets the commandStarted to true
				commandStarted = true;
			}

			// Checks if an command has ended, if not append data
			if (commandStarted && *(p - 1) == '%' && *p == '}')
			{
				// Allocates the memory for the command itself
				char *contentPtr = reinterpret_cast<char *>(malloc(bufferLen + 1));
				*(&contentPtr[bufferLen]) = '\0';

				// Copies the memory
				memcpy(&contentPtr[0], &buffer[1], bufferLen - 2);

				// Append it to the result
				result.emplace_back(LexPart{
					LexType::LT_COMMAND,
					&contentPtr[0]
				});

				// Resets the buffer
				*(&buffer[0]) = '\0';

				// Sets the buffer len
				bufferLen = 0;

				// Sets command started to false
				commandStarted = false;
			} else if (commandStarted)
			{
				// Appends the char
				strncat(&buffer[0], p, 1);

				// Increments the size
				bufferLen++;
			}

			// ----
			// Appends to the buffer if required
			// ----

			if (!commandStarted) {
				strncat(&buffer[0], p, 1);
				bufferLen++;
			}
		}

		// ----
		// Checks if there still is other data left
		// ----

		if (bufferLen > 0)
		{
			char *contentPtr = nullptr;

			// Checks if we need to remove an '}' at the start
			if (*(&buffer[0]) == '}')
			{
				// Allocate the memory without the first char
				contentPtr = reinterpret_cast<char *>(malloc(bufferLen));
				// Copies the data
				memcpy(&contentPtr[0], &buffer[1], bufferLen);
				// Removes the '{' if there
				if (*(&contentPtr[bufferLen-2]) == '{') *(&contentPtr[bufferLen-2]) = '\0';
			} else
			{
				// Allocates the memory
				contentPtr = reinterpret_cast<char *>(malloc(bufferLen + 1));
				// Copies the data
				memcpy(&contentPtr[0], &buffer[0], bufferLen + 1);
				// Removes the '{' if there
				if (*(&contentPtr[bufferLen-1]) == '{') *(&contentPtr[bufferLen-1]) = '\0';
			}

			// Pushes it to the result
			result.emplace_back(LexPart{
				LexType::LT_CONTENT,
				&contentPtr[0]
			});
		}

		// ----
		// Frees the memory
		// ----

		free(buffer);

		return rc;
	}

    int render(const char *filename, std::map<const char *, TemplateVariable> &variables, char **ret, TemplateErrorLog &log)
    {
	    char *buffer = reinterpret_cast<char *>(malloc(1024));
	    char *file = reinterpret_cast<char *>(malloc(1));
	    std::size_t fileLen = 1, i, retSize, buffSize;
	    *(&file[0]) = '\0';

	    // ----
	    // Reads the file
	    // ----

	    // Opens the file
	    FILE *fp = fopen(filename, "rt");

	    // Starts reading
	    while (fgets(&buffer[0], 1024, fp) != nullptr)
        {
	        // Checks how much whitespace o remove
	        i = 0;
            for (const char *c = &buffer[0]; (*c == ' ' || *c == '\t') && *c != '\0'; c++) i++;
            // Gets the buffer size
            buffSize = strlen(&buffer[0]);
            // Removes the '\n'
            if (buffer[buffSize-1] == '\n') buffer[buffSize-1] = '\0';

	        // Adds the length of the buffer, to the fileLen
            fileLen += buffSize - i;

	        // Resize the buffer
	        file = reinterpret_cast<char *>(realloc(&file[0], fileLen));

	        // Concat the buffer
	        strcat(&file[0], &buffer[i]);
        }

	    // ----
	    // Starts removing the '\r\n'
	    // ----

	    // ----
	    // Performs the analysis
	    // ----

	    // Creates the result vector
	    std::vector<LexPart> resultVec{};
	    // Performs the analysis
	    performLexicalAnalysis(file, resultVec);

	    // ----
	    // Executes the command
	    // ----

        std::vector<LexPart> tempParts{};
        retSize = 1;
        int ucRet;
        *ret = reinterpret_cast<char *>(malloc(retSize));
        *(&(*ret)[0]) = '\0';
        for (auto &a : resultVec)
        {
            if (a.l_LexType == LexType::LT_COMMAND)
            {
                char *result = nullptr;

                // ----
                // Prepares the data
                // ----

                // Clears the old values
                tempParts.clear();
                // Performs the analysis
                uint8_t stat = performLexicalAnalysisCommand(&a.l_Content, tempParts);
                // Executes the command
                ucRet = useCommand(tempParts, &result, variables, log);

                // ----
                // Concatenates the data
                // ----

                // Sets the new buffer size
                retSize += strlen(&result[0]);
                // Performs the re-allocation
                *ret = reinterpret_cast<char *>(realloc(&(*ret)[0], retSize));
                // Concatenates the result
                strcat(&(*ret)[0], &result[0]);

                // ----
                // Frees the memory
                // ----

                if (ucRet == -1)
                {
                    free(result);
                }

                // Clears the temp parts
                for (LexPart &l : tempParts)
                {
                    free(l.l_Content);
                }
            } else if (a.l_LexType == LexType::LT_CONTENT)
            {
                // Sets the new result size
                retSize += strlen(&a.l_Content[0]);
                // Performs the re-allocation
                *ret = reinterpret_cast<char *>(realloc(&(*ret)[0], retSize));
                // Concatenates the new data
                strcat(&(*ret)[0], &a.l_Content[0]);
            }
        }

        // ----
        // Frees the memory
        // ----

        // Free's the lex parts
        for (LexPart &p : resultVec)
        {
            free(p.l_Content);
        }

        // Free's the char pointers
        free(buffer);
        free(file);

        return 0;
    }
}
