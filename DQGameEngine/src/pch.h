#pragma once

#include<memory>
#include<stdint.h>
#include<set>
#include<random>
#include<limits>
#include<vector>
#include<string>
#include<bitset>
#include<cassert>
#include<assert.h>
#include<sstream>
#include<istream>
#include<fstream>
#include<iostream>
#include<algorithm>
#include<functional>
#include<unordered_map>
#include<unordered_set>
#include<queue>
#include<map>
#include<stack>
#include <typeindex>

// Extenal libraries

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include "stb_image.h"

// GLM core
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

// Matrix transforms (translate, rotate, scale, ortho, perspective)
#include <glm/gtc/matrix_transform.hpp>

// Pointer conversion (glm::value_ptr)
#include <glm/gtc/type_ptr.hpp>

// Quaternion (camera / rotation nâng cao)
#include <glm/gtc/quaternion.hpp>

// Random utilities
#include <glm/gtc/random.hpp>

// Vector angle / projection utilities
#include <glm/gtx/vector_angle.hpp>

// String cast (debug glm)
#include <glm/gtx/string_cast.hpp>

template<typename T> using Shared = std::shared_ptr<T>;
template<typename T> using Unique = std::unique_ptr<T>; 
