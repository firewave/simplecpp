/*
* simplecpp - A simple and high-fidelity C/C++ preprocessor library
 * Copyright (C) 2016-2025 simplecpp team
 */

#include "simplecpp.h"

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>

#ifdef NO_FUZZ
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#endif

static int doProcess(const uint8_t *data, size_t dataSize)
{
    const std::string s = std::string{reinterpret_cast<const char*>(data), dataSize};
    try {
        (void)simplecpp::characterLiteralToLL(s);
    } catch (const std::runtime_error& e) {
        if (strcmp(e.what(), "expected a character literal") == 0)
            return 0;
        return -1;
    }
    return -1;
}

#ifndef NO_FUZZ
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t dataSize);

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t dataSize)
{
    return doProcess(data, dataSize);
}
#else
int main(int argc, char * argv[])
{
    if (argc < 2 || argc > 3)
        return EXIT_FAILURE;

    std::ifstream f(argv[1]);
    if (!f.is_open())
        return EXIT_FAILURE;

    std::ostringstream oss;
    oss << f.rdbuf();

    if (!f.good())
        return EXIT_FAILURE;

    const int cnt = (argc == 3) ? std::stoi(argv[2]) : 1;

    const std::string code = oss.str();
    for (int i = 0; i < cnt; ++i)
        doProcess(reinterpret_cast<const uint8_t*>(code.data()), code.size());

    return EXIT_SUCCESS;
}
#endif
