#pragma once
#include <glm/glm.hpp>
#include "Colour.h"
#include <string>

using namespace glm;

class ModelTriangle
{
  public:
    vec3 vertices[3];
    Colour colour;

    ModelTriangle()
    {
    }

    ModelTriangle(vec3 v0, vec3 v1, vec3 v2, Colour trigColour)
    {
      vertices[0] = v0;
      vertices[1] = v1;
      vertices[2] = v2;
      colour = trigColour;
    }

    vec3 calculateNormal()
    {
      return cross((vertices[1] - vertices[0]), (vertices[2] - vertices[0]));
    }
};

inline std::ostream& operator<<(std::ostream& os, const ModelTriangle& triangle)
{
    os << "(" << triangle.vertices[0].x << ", " << triangle.vertices[0].y << ", " << triangle.vertices[0].z << ")" << std::endl;
    os << "(" << triangle.vertices[1].x << ", " << triangle.vertices[1].y << ", " << triangle.vertices[1].z << ")" << std::endl;
    os << "(" << triangle.vertices[2].x << ", " << triangle.vertices[2].y << ", " << triangle.vertices[2].z << ")" << std::endl;
    os << std::endl;
    return os;
}
