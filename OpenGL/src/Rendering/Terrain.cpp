#include "Terrain.h"

Terrain::Terrain(Window &window, GUI &gui) : mainWindow(window), mainGUI(gui) {
    // Add any local initializations here
}

bool Terrain::createTerrainFromHeightmap(const char* fileLoc) {
    // Loading the texture from provided path
    heightmapTex = Texture(fileLoc);
    bool texLoaded = heightmapTex.loadTexture();

    // Checking if the texture loaded successfully or not
    if(!texLoaded) {
        return false;
    }

    height = heightmapTex.getHeight();
    width = heightmapTex.getWidth();
    bitDepth = heightmapTex.getBitDepth();

    // Debugging
    // std::string message = "Successfully loaded heightmap!\nThe dimesions are: (" + std::to_string(height) + "," + std::to_string(width) + ")\n";
    // printf(message.c_str());

    return true;
}

Terrain::~Terrain() {
    height = 0;
    width = 0;
    bitDepth = 0;

    heightmapTex.cleanTexture();
}
