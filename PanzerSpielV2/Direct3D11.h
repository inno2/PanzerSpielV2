#pragma once

#ifdef _WIN64

#include "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.16299.0\\um\\d3d11.h"
#pragma comment (lib, "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.16299.0\\um\\x64\\d3d11.lib")

#else// _WIN64

#include <D3D11.h>
#pragma comment (lib, "d3d11.lib")

#endif