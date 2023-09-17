#pragma once
#include <inttypes.h>

#include <condition_variable>
#include <functional>
#include <mutex>

#include "figures.h"
#include "materials.h"

#define WIDTH 600
#define HEIGHT 600

void render(uint32_t* scene);
