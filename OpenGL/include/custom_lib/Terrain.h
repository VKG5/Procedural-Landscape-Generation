// We need to load the heightmap into memory for height/width calculations
#include "Texture.h"
#include "GUI.h"
#include "Window.h"

#include <string>

class Terrain {
private:
    Texture heightmapTex;
    GLuint height, width, bitDepth;

    // This ensures that the the references are stored only for the objects lifetime (reference), and not after that
    // Our main window
    Window& mainWindow;

    // Our main GUI
    GUI& mainGUI;

public:
    // Constructor
    Terrain(Window& window, GUI& gui);

    bool createTerrainFromHeightmap(const char* fileLoc);

    // Getters=========================================================================================================
    // Getter that returns a reference to the Texture
    const Texture& getHeightmapTexture() const { return heightmapTex; }

    // Destructor
    ~Terrain();
};
