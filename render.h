#pragma once
#include <inttypes.h>

#include <condition_variable>
#include <functional>
#include <mutex>

#include "figures.h"
#include "materials.h"

#define WIDTH 1280
#define HEIGHT 720

void render(uint32_t* scene);
