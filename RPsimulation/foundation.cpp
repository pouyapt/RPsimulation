#include "foundation.h"

bool is_file_exist(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

void databaseInit() {
    if (!is_file_exist("Data"))
    system("mkdir Data");
}

double round_(double var)
{
    char str[80];
    sprintf(str, "%.2lf", var);
    sscanf(str, "%lf", &var);
    return var;
}
