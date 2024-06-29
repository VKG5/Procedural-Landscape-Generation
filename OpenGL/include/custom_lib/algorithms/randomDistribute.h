#pragma once

// Necessary libraries for procedural generation
// cstdlib - Random
#include <iostream>
#include <vector>
#include <cstdlib>

// Adding for more modern random value generation
#include <random>

// GLM Files - Math Library
#include <glm/glm.hpp>

// Function to check collisions between buildings and edges of the grid
bool collide(const std::vector<std::pair<int, int>>& points, int x, int y, int size);

// Generate random buildings on the grid, making sure no two buildings collide
void generateRandomPoints(std::vector<std::pair<int, int>>& points, int gridSize, int pointSize, int numPoints, int seed = 69);

// Generate random scales for the points
void generateRandomScales(std::vector<glm::vec3>& scales, int numPoints, int seed = 69);

// Generate random heights for the points
void generateRandomHeights(std::vector<int>& heights, int numPoints, int seed = 69);

// Debugging, to display the grid in a 2D view on the console/terminal
void displayGrid(const std::vector<std::pair<int, int>>& points, const int gridSize, const int pointSize, const int numPoints);
