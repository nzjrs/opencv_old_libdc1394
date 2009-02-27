#pragma once

#include <vector>
#include <string>
#include <windows.h>

bool FilenamesDialog(HWND parent, int num_filenames, std::vector<std::string> &filenames);
