#pragma once

// System imports
#include <string>
#include <map>
#include <iostream>
#include <fstream>

// Project imports
#include <spdlog/spdlog.h>
#include <Eigen/Core>


#if !defined(NAMESPACE_BEGIN)
#define NAMESPACE_BEGIN(name) namespace name {
#endif
#if !defined(NAMESPACE_END)
#define NAMESPACE_END(name) }
#endif