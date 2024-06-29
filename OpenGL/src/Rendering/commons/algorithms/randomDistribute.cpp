#include "randomDistribute.h"

bool collide(const std::vector<std::pair<int, int>>& points, int x, int y, int size) {
    for (const auto& point : points) {
        int px = point.first;
        int py = point.second;

        if (x < px + size && x + size > px && y < py + size && y + size > py) {
            return true; // Collision detected
        }
    }

    return false; // No collision
}

// Using srand() - Old Method
void generateRandomPoints(std::vector<std::pair<int, int>>& points, int gridSize, int pointSize, int numPoints, int seed) {
    srand(seed);

    // Clearing the vector
    points.clear();

    for(size_t i=0; i < gridSize; i++) {
        int x, y;

        // Generating points until collision is detected
        do {
            x = rand() % (gridSize - pointSize + 1);
            y = rand() % (gridSize - pointSize + 1);
        } while(collide(points, x, y, pointSize));

        points.push_back({x, y});
    }
}

// Using gen() - New Method
void generateRandomScales(std::vector<glm::vec3>& scales, int numPoints, int seed) {
    // Setting parameters
    float min = 0.7f, max = 1.5f;

    // Clearing for any previous scales that were set
    scales.clear();

    // Use the seed to initialize the generator
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> scaleDistribution(min, max);

    for (size_t i = 0; i < numPoints; ++i) {
        float scaleX = scaleDistribution(gen);
        float scaleY = scaleDistribution(gen);
        float scaleZ = scaleDistribution(gen);

        scales.push_back(glm::vec3(scaleX, scaleY, scaleZ));
    }
}

void generateRandomHeights(std::vector<int>& heights, int numPoints, int seed) {
    // Setting parameters
    int min = 0, max = 3;

    // Clearing for any previous scales that were set
    heights.clear();

    // Use the seed to initialize the generator
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> heightDistribution(min, max);

    for (size_t i = 0; i < numPoints; i++) {
        heights.push_back(heightDistribution(gen));
    }
}

void displayGrid(const std::vector<std::pair<int, int>>& points, const int gridSize, const int pointSize, const int numPoints) {
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            bool isPoint = false;

            // Check if the current position is a point
            for (const auto& point : points) {
                if (i >= point.first && i < point.first + pointSize &&
                    j >= point.second && j < point.second + pointSize) {
                    isPoint = true;
                    break;
                }
            }

            // Display '-' for borders and 'x' for points
            std::cout << (isPoint ? 'x' : '-');
        }

        std::cout << std::endl;
    }
}
