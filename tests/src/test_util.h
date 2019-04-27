#pragma once
#include <string>


bool verify(bool cond, const std::string& label, const std::string& condStr,
            const std::string& fileName, int lineNum);

#define VERIFY(cond, label) (verify(cond, label, #cond, __FILE__, __LINE__))
