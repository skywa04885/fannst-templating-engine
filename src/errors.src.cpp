#include "errors.src.hpp"

namespace Fannst::TemplatingEngine
{
    /**
     * Generates an HTML Error
     * @param title
     * @param description
     * @param ret
     */
    void generateHtmlError(const char *title, const char *description, char **ret)
    {
        // Free's ret, if it is not an null ptr
        if (*ret != nullptr) free(*ret);

        // ----
        // Allocates the required memory
        // ----

        // Allocates the memory based on the ret size
        *ret = reinterpret_cast<char *>(malloc(
                strlen(&title[0]) + strlen(&title[0]) + 256 + 1));
        // Sets the null termination char
        *(&(*ret)[0]) = '\0';

        // ----
        // Starts generating the result
        // ----

        // Creates the div, with paragraph
        strcat(&(*ret)[0], "<div style=\"color:#0505ff;background-color:rgba(10,10,255,0.2);padding:5px;"
                           "border:1px solid rgba(10,10,255,0.2);\"><p style=\"margin:0;\">");
        // Creates the title
        strcat(&(*ret)[0], "<strong>");
        strcat(&(*ret)[0], &title[0]);
        strcat(&(*ret)[0], ": </strong>");
        // Creates the description
        strcat(&(*ret)[0], description);
        // Creates the closing div, and closing paragraph
        strcat(&(*ret)[0], "</p></div>");
    }

    /**
     * Generates an HTML Error log
     * @param err
     * @param ret
     */
    void generateFullPageFailure(TemplateErrorLog &err, char **ret)
    {
        // Free's ret, if it is not an null ptr
        if (*ret != nullptr) free(*ret);
    }
}