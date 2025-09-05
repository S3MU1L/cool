#include "compiler.hpp"

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    /* TODO: additional argument for output file */
    if (argc != 2)
    {
        std::cerr << "Usage: coolc <filePath>\n";
        exit(1);
    }

    cool::compiler::Compiler          compiler{argv[1]};
    cool::compiler::CompilationResult result = compiler.compile();
}
