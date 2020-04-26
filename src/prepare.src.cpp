#include "prepare.src.hpp"

namespace Fannst::TemplatingEngine
{
    void makeTextHTML5Tolerant(const char *raw, char **ret)
    {
        std::size_t resLen, resEncodingLenUsed;

        // ----
        // Prepares the loop
        // ----

        // Gets the length of raw, plus null termination char
        resLen = strlen(&raw[0]) + 1;
        // Allocates the buffer memory, plus 24 extra for possible encoding
        resLen += 24;
        resEncodingLenUsed = 0;
        *ret = reinterpret_cast<char *>(malloc(resLen));

        // ----
        // Starts the encoding
        // ----

        const char *append = nullptr;
        std::size_t appendSize = 0;
        for (const char *p = &raw[0]; *p != '\0'; p++)
        {
            // Checks if it needs to be encoded
            if (isalpha(*p) != 0)
            { // Is char, just append
                strncat(&(*ret)[0], p, 1);
            } else
            {
                // Checks which char to append
                switch (*p)
                {
                    case '<':
                    { // &lt;
                        append = "&lt;";
                        appendSize = 4;
                        break;
                    }
                    case '>':
                    { // &gt;
                        append = "&gt;";
                        appendSize = 4;
                        break;
                    }
                    case '"':
                    { // &quot;
                        append = "&quot;";
                        appendSize = 6;
                        break;
                    }
                    case '\'':
                    { // &apos;
                        append = "&apos;";
                        appendSize = 6;
                        break;
                    }
                    case '&':
                    { // &amp;
                        append = "&amp;";
                        appendSize = 5;
                        break;
                    }
                    default:
                    { // Just append the char, and continue
                        // Appends the char
                        strncat(&(*ret)[0], p, 1);
                        // Skips next operations
                        continue;
                    }
                }

                // Adds the append size
                resEncodingLenUsed += appendSize;

                // Checks if resize is required
                if (resEncodingLenUsed >= 24)
                { // Is larger, perform re-allocation before continue
                    // Adds 24 chars to the buffer
                    resLen += 24;
                    // Re-allocates the buffer
                    *ret = reinterpret_cast<char *>(realloc(&(*ret)[0], resLen));
                    // Resets the encoding used
                    resEncodingLenUsed = 0;
                }

                // Appends the chars
                strcat(&(*ret)[0], append);
                // Resets the append size
                appendSize = 0;
            }
        }
    }
}