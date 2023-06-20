#pragma once

#include <cstdint>


struct vec2
{
    float x; // offset (ou adresse relative) = 0
    float y; // offset = sizeof(x) = 4
};

struct vec3
{
    float x; // offset = 0
    float y; // offset = sizeof(x) = 4
    float z; // offset = sizeof(x) + sizeof(y) = 8
};

using Color = vec3; // equivalent de typedef vec3 color;

struct Vertex
{
    vec3 position;
    Color color; // todo, vec3 normal
    vec2 uv;
};
