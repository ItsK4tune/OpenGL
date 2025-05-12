#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <array>

struct Vertex
{
    float Position[3];
    float TexCoords[2];
    float Normal[3];
};

bool loadObj(const std::string &path, std::vector<Vertex> &outVertices, std::vector<unsigned int> &outIndices);

#endif