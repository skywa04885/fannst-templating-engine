#include <iostream>
#include <fstream>

#include "src/lexer.hpp"

using namespace std;
using namespace Fannst::TemplatingEngine;

int main(int argc, char **argv)
{
    const char *title = "Hello WOrld !";

    char *test = nullptr;
    std::map<const char *, TemplateVariable> p{
        std::make_pair("title", TemplateVariable{
            TvType::TV_CHAR_POINTER,
            reinterpret_cast<const void *>(title),
        })
    };
    render("../test.html", p, &test);
    std::cout << test << std::endl;

    free(test);

	return 0;
}