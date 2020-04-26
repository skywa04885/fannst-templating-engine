#pragma once

#include <map>
#include <vector>
#include <cstring>

namespace Fannst::TemplatingEngine
{
    typedef struct
    {
        const char *t_Message{nullptr};
    } TemplateError;

    typedef struct
    {
        std::vector<TemplateError> t_Errors;
        int t_TotalErrors;
        const char *filename{nullptr};
    } TemplateErrorLog;

    /**
     * Generates an HTML Error
     * @param title
     * @param description
     * @param ret
     */
    void generateHtmlError(const char *title, const char *description, char **ret);

    /**
     * Generates an HTML Error log
     * @param err
     * @param ret
     */
    void generateFullPageFailure(TemplateErrorLog &err, char **ret);
}