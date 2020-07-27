#include "system.h"

bool is_file_exist(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

void databaseInit() {
    if (!is_file_exist("Data"))
    system("mkdir Data");
}
