#pragma once

#include <iostream>
#include <malloc.h>
#include <cstring>

namespace Fannst::TemplatingEngine
{
    void makeTextHTML5Tolerant(const char *raw, char **ret);
}