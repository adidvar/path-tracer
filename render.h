#pragma once
#include <inttypes.h>
#include "materials.h"
#include "figures.h"

#include <functional>
#include <mutex>
#include <condition_variable>

#define WIDTH 600
#define HEIGHT 600

void render(uint32_t* scene);

