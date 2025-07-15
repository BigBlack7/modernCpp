#pragma once

// output&input
#include <iostream>
#include <fstream>

// utility
#include <exception>
#include <utility>
#include <typeinfo>
#include <thread>
#include <mutex>

// stl
#include <string>
#include <list>
#include <deque>
#include <utility>

// core
#include "ptr/zSharedPtr.hpp"

#include "stl/zArray.hpp"
#include "stl/zList.hpp"
#include "stl/zDeque.hpp"
#include "stl/zBSTMap.hpp"
#include "stl/zAVLMap.hpp"
#include "stl/zHashMap.hpp"

#include "memory/zMemoryPool.hpp"

#include "modern/sfinae.hpp"

#include "designPattern/singleton.hpp"

#include "logger/logger.hpp"

// external
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>