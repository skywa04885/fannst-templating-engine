#include <iostream>
#include <fstream>

#include "src/lexer.hpp"
#include "src/errors.src.hpp"
#include "src/prepare.src.hpp"

using namespace std;
using namespace Fannst::TemplatingEngine;

int main(int argc, char **argv)
{
    TemplateErrorLog log{};
    const char *title = "Hello & WOrld !";
    bool a = false;

    char *test = nullptr;
    std::map<const char *, TemplateVariable> p{
        std::make_pair("title", TemplateVariable{
            TvType::TV_CHAR_POINTER,
            reinterpret_cast<const void *>(title),
        }),
        std::make_pair("bool", TemplateVariable{
                TvType::TV_BOOL,
                reinterpret_cast<const void *>(&a),
        })
    };
    render("../test.html", p, &test, log);
    std::cout << test << std::endl;

    free(test);

	return 0;
}
