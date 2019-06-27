//
// Win32 utilities library
// Utilities for testing.
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include <string>


bool verify(bool cond, const std::string& label, const std::string& condStr,
            const std::string& fileName, int lineNum);

#define VERIFY(cond, label) (verify(cond, label, #cond, __FILE__, __LINE__))
