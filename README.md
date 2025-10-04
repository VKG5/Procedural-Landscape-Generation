# Procedural Landscape Generation

> **MS Thesis Project** - Trinity College Dublin  
> A workflow for generating realistic landscapes using AI (Stable Diffusion) combined with contemporary procedural methods and real-time OpenGL rendering.

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Python](https://img.shields.io/badge/python-3.8+-blue.svg)](https://www.python.org/downloads/)
[![CMake](https://img.shields.io/badge/cmake-3.24+-green.svg)](https://cmake.org/)

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Building the Project](#building-the-project)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [API Reference](#api-reference)
- [Dataset](#dataset)
- [Contributing](#contributing)
- [Resources](#resources)
- [License](#license)

## Overview

This project aims to simplify landscape generation by overcoming the learning curve and limitations of traditional tools like World Machine, Gaea, and node-based workflows such as Blender Geometry Nodes and Houdini. It combines:

- **AI-Powered Generation**: Uses Stable Diffusion with custom-trained LoRA models to generate heightmaps
- **Real-Time Rendering**: OpenGL-based C++ application for visualizing and rendering terrains
- **User-Friendly GUI**: PyQt5 interface for easy interaction with the generation pipeline
- **Dataset Collection**: Scripts for gathering and processing Digital Elevation Models (DEMs) from OpenTopography

The project is designed to be accessible to hobbyists, companies, and the community while maintaining high-quality outputs. It can be extended in multiple directions, including generating textures for landscapes, training specialized LoRA models, and more.

## Features

- ✨ **Text-to-Image Generation**: Generate heightmaps from text prompts using Stable Diffusion
- 🔄 **Image-to-Image Generation**: Refine existing heightmaps with AI-guided modifications
- 🎨 **Real-Time 3D Visualization**: View generated terrains in an interactive OpenGL viewport
- 🖥️ **Intuitive GUI**: Easy-to-use PyQt5 interface for controlling generation parameters
- 📊 **DEM Dataset Support**: Integration with OpenTopography for real-world elevation data
- 🔧 **Customizable Parameters**: Control dimensions, steps, upscaling, and more
- 🎮 **Interactive Camera**: Navigate through generated landscapes in real-time
- 🌐 **Stable Diffusion API Integration**: Seamless connection to AUTOMATIC1111's WebUI

## Prerequisites

### Software Requirements

#### For Python Components (GUI & AI Generation):
- **Python 3.8+**
- **PyQt5** for GUI
- **Stable Diffusion WebUI** (AUTOMATIC1111)

#### For C++ Rendering Component:
- **CMake 3.24+**
- **C++17 Compatible Compiler**:
  - Visual Studio 2022 (Windows)
  - GCC 9+ (Linux)
  - Clang 10+ (macOS)
- **OpenGL 4.3+** compatible graphics driver

#### External Dependencies (Auto-installed via CMake):
- **GLFW 3.3.8** - Window and input handling
- **GLM 0.9.9.8** - Mathematics library
- **Assimp 5.3.1** - Model loading
- **GLAD** - OpenGL loader
- **ImGui** - Immediate mode GUI
- **Vulkan Headers 1.3.204** - Graphics API headers

### Hardware Requirements

- **GPU**: OpenGL 4.3+ capable graphics card (NVIDIA/AMD with updated drivers)
- **RAM**: 8GB minimum, 16GB recommended
- **Storage**: 5GB+ for dependencies and models
- **For AI Generation**: NVIDIA GPU with CUDA support (4GB+ VRAM recommended)

## Installation

### 1. Clone the Repository

```bash
git clone --recursive https://github.com/VKG5/Procedural-Landscape-Generation.git
cd Procedural-Landscape-Generation
```

**Note**: The `--recursive` flag is important to clone the Stable Diffusion WebUI submodule.

If you already cloned without `--recursive`, run:
```bash
git submodule update --init --recursive
```

### 2. Install Python Dependencies

```bash
pip install -r requirements.txt
```

**Required Python packages:**
- `beautifulsoup4==4.12.3`
- `imageio==2.33.1`
- `jax==0.4.25`
- `opencv-contrib-python==4.8.1.78`
- `opencv-python==4.8.1.78`
- `Pillow==10.0.1`
- `requests==2.32.2`
- `selenium==4.21.0`
- `torch==2.1.2`
- `urllib3==2.2.1`
- `webdriver-manager==4.0.1`
- `bmi-topography==2.0.1`
- `PyQt5` (install separately if needed)

### 3. Set Up Stable Diffusion WebUI

Navigate to the Stable Diffusion WebUI directory and follow its setup instructions:

```bash
cd stable-diffusion-webui
# Follow AUTOMATIC1111's installation instructions for your platform
# https://github.com/AUTOMATIC1111/stable-diffusion-webui
```

**Download Required Models:**
- Download the checkpoint from [CivitAI - Game Landscape Heightmap Generator](https://civitai.com/models/16826/game-landscape-heightmap-genrator)
- Download the LoRA from [CivitAI - Game Landscape Heightmap Generator LoRA](https://civitai.com/models/18621/game-landscape-heightmap-genrator-lora)
- Place models in the appropriate Stable Diffusion WebUI directories:
  - Checkpoints: `stable-diffusion-webui/models/Stable-diffusion/`
  - LoRA: `stable-diffusion-webui/models/Lora/`

### 4. Configure API Settings

Edit the `apiLink.py` file to match your Stable Diffusion WebUI configuration:

```python
# Change this URL according to your local host configuration
url = "http://localhost:7861/"  # Default API endpoint
```

## Building the Project

### Building the OpenGL C++ Application

#### On Windows (Visual Studio):

```bash
cd OpenGL
mkdir build
cd build

# Configure the project (Visual Studio 2022)
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_CXX_STANDARD=17 ..

# Build the project
cmake --build . --config Debug
# Or for Release build:
cmake --build . --config Release
```

The executable will be located at:
- Debug: `OpenGL/build/src/Rendering/Debug/Executable.exe`
- Release: `OpenGL/build/src/Rendering/Release/Executable.exe`

#### On Linux/macOS:

```bash
cd OpenGL
mkdir build
cd build

# Configure the project
cmake -DCMAKE_CXX_STANDARD=17 ..

# Build the project
cmake --build .

# Run the executable
./src/Rendering/Executable
```

#### Using Makefile (Linux/macOS):

```bash
cd OpenGL
make prepare  # Creates build directory
cd build
cmake ..
cmake --build .
```

### CMake Build Options

The CMake build system automatically fetches and builds the following dependencies:
- GLFW (Window management)
- GLM (Mathematics)
- Assimp (Model loading)
- Vulkan Headers

No manual installation of these libraries is required.

## Usage

### Starting the Application

#### 1. Start Stable Diffusion WebUI API

First, start the Stable Diffusion WebUI with API enabled:

```bash
cd stable-diffusion-webui
# On Windows:
./webui-user.bat --api --port 7861

# On Linux/macOS:
./webui.sh --api --port 7861
```

**Important**: Make sure the API port matches the one configured in `apiLink.py`.

#### 2. Launch the GUI Application

In a new terminal, from the project root directory:

```bash
python gui.py
```

### Using the GUI Interface

The **Yumi - Landscape Generator** GUI provides an intuitive interface:

1. **Generation Type**: Choose between:
   - `txt2img`: Generate heightmaps from text descriptions
   - `img2img`: Refine existing heightmaps

2. **Prompt Field**: Enter your landscape description
   - Example: "A mountain range with valleys"
   - Trigger word "gamelandscapeheightmap512" is automatically prepended

3. **Parameters**:
   - **Dimensions**: Resolution of the generated heightmap (e.g., 512, 1024)
   - **Steps**: Number of diffusion steps (higher = more detailed, slower)
   - **Upscale**: Enable to upscale the output
   - **Upscale Factor**: Multiplier for upscaling (e.g., 2x, 4x)

4. **Generate Image**: Click to generate the heightmap
   - Generated images are saved in the `outputs/` directory
   - Images are named with their seed value (e.g., `123456789.png`)

5. **Launch Application**: Click to open the OpenGL renderer with the generated heightmap

### Command Line Options

You can also use the API directly in Python:

```python
import apiLink as api

# Text-to-Image generation
image_path = api.runStableDiffusionAPI(
    generationType="txt2img",
    prompt="A mountain range with snow peaks",
    dimensions=1024,
    steps=50,
    isUpscale=True,
    upscaleFactor=2,
    seed=-1  # -1 for random seed
)

# Image-to-Image generation
image_path = api.runStableDiffusionAPI(
    generationType="img2img",
    prompt="Add more valleys",
    imagePath="outputs/existing_heightmap.png",
    dimensions=1024,
    steps=50
)
```

### OpenGL Renderer Controls

When the C++ rendering application launches:

- **Mouse**: Look around (right-click and drag)
- **W/A/S/D**: Move camera forward/left/backward/right
- **Q/E**: Move camera up/down
- **ESC**: Close application
- **ImGui Interface**: Adjust rendering parameters in real-time

## Project Structure

```
Procedural-Landscape-Generation/
├── README.md                          # This file
├── requirements.txt                   # Python dependencies
├── gui.py                            # Main PyQt5 GUI application
├── apiLink.py                        # Stable Diffusion API integration
├── dataset.py                        # OpenTopography data collection
├── test.py                           # Test utilities
├── ifImg2Img.py                      # Image-to-image helper functions
├── landscapePreview.blend            # Blender preview file
├── MSc_Dissertation.pdf              # Thesis document
├── slides_theory1_shaders_pipeline.pdf # Technical slides
│
├── OpenGL/                           # C++ rendering application
│   ├── CMakeLists.txt               # Main CMake configuration
│   ├── Makefile                     # Build helper
│   ├── include/                     # Custom library headers
│   │   └── custom_lib/
│   │       ├── Camera.h
│   │       ├── Window.h
│   │       ├── Shader.h
│   │       ├── Mesh.h
│   │       ├── Terrain.h
│   │       ├── Scene.h
│   │       └── ...
│   ├── src/                         # Source files
│   │   └── Rendering/
│   │       ├── main.cpp            # Entry point
│   │       ├── Scene.cpp           # Scene management
│   │       ├── Terrain.cpp         # Terrain rendering
│   │       ├── Mesh.cpp
│   │       ├── GUI.cpp             # ImGui implementation
│   │       ├── Shaders/            # GLSL shader files
│   │       ├── Textures/           # Texture assets
│   │       ├── Models/             # 3D model files
│   │       └── commons/            # Shared implementations
│   ├── extern/                      # External dependencies
│   │   ├── glad/                   # OpenGL loader
│   │   └── imgui/                  # ImGui library
│   └── tools/                       # Development tools
│       ├── run-clang-format.py
│       └── run-clang-tidy.py
│
├── stable-diffusion-webui/          # Submodule: AUTOMATIC1111's WebUI
├── outputs/                         # Generated heightmap images
├── images/                          # Sample/reference images
├── metadata/                        # Project metadata
└── tmp/                            # Temporary files
```

## API Reference

### apiLink.py

#### `runStableDiffusionAPI()`

Generates heightmaps using Stable Diffusion.

**Parameters:**
- `generationType` (str): `"txt2img"` or `"img2img"`
- `prompt` (str): Text description of the landscape
- `sampler` (str): Sampling method (e.g., "Euler a")
- `scheduler` (str): Scheduler type (e.g., "Automatic")
- `dimensions` (int): Image resolution (512, 1024, etc.)
- `steps` (int): Number of diffusion steps (20-150)
- `isUpscale` (bool): Enable upscaling
- `upscaleFactor` (int): Upscale multiplier (2, 4)
- `seed` (int): Seed for reproducibility (-1 for random)
- `imagePath` (str): Path to input image (for img2img)

**Returns:**
- `str`: Path to the generated image in `outputs/` directory

### dataset.py

#### OpenTopography Integration

Uses the `bmi-topography` library to fetch real-world Digital Elevation Models (DEMs).

**Supported DEM Types:**
- `SRTMGL3` - SRTM GL3 90m resolution
- `SRTMGL1` - SRTM GL1 30m resolution
- `AW3D30` - ALOS World 3D 30m
- `COP30` - Copernicus Global DSM 30m
- `COP90` - Copernicus Global DSM 90m

**Example Usage:**
```python
from bmi_topography import Topography

patch = Topography(
    dem_type="SRTMGL3",
    south=40.0,
    north=41.0,
    west=-105.0,
    east=-104.0,
    output_format="GTiff"
)
```

## Dataset

This project uses multiple data sources for training and evaluation:

### Real-World Data
- **Source**: [OpenTopography](https://opentopography.org/)
- **Type**: Digital Elevation Models (DEMs) from satellite imagery
- **Formats**: GeoTIFF, IMG
- **Resolution**: 30m to 90m per pixel
- **Coverage**: Global

### Procedural Data
- **Method**: Blender Geometry Nodes with Noise functions
- **Script**: `landscapePreview.blend`
- **Purpose**: Generate diverse synthetic training data
- **Output**: Heightmaps representing various terrain types

### Dataset Characteristics
- **Format**: Heightmaps/heightfields (grayscale images)
- **Resolution**: 512x512 to 2048x2048 pixels
- **Value Range**: 0-255 (8-bit) or normalized float
- **Use Cases**: Training LoRA models, testing generation quality

## Contributing

Contributions are welcome! Here's how you can help:

1. **Fork the repository**
2. **Create a feature branch**: `git checkout -b feature/amazing-feature`
3. **Commit your changes**: `git commit -m 'Add amazing feature'`
4. **Push to the branch**: `git push origin feature/amazing-feature`
5. **Open a Pull Request**

### Development Guidelines

- Follow the existing code style
- Use `clang-format` for C++ code formatting
- Test your changes thoroughly
- Update documentation as needed

### Code Formatting

For C++ code:
```bash
cd OpenGL
python tools/run-clang-format.py
python tools/run-clang-tidy.py
```

## Resources

### Useful Links

- **Checkpoint**: [Game Landscape Heightmap Generator](https://civitai.com/models/16826/game-landscape-heightmap-genrator)
- **LoRA**: [Game Landscape Heightmap Generator LoRA](https://civitai.com/models/18621/game-landscape-heightmap-genrator-lora)
- **SD API Documentation**: [AUTOMATIC1111 API Guide](https://github.com/AUTOMATIC1111/stable-diffusion-webui/wiki/API)
- **OpenTopography**: [DEM Data Access](https://opentopography.org/)

### Tutorials

- **Stable Diffusion Basics**: [freeCodeCamp Tutorial](https://www.youtube.com/watch?v=dMkiOex_cKU)
- **LoRA Training**: [How to train a LoRA](https://civitai.com/models/22530)
- **CMake Tutorial**: [Official CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
- **OpenGL Guide**: [LearnOpenGL](https://learnopengl.com/)

### Academic Resources

- **MSc Dissertation**: See `MSc_Dissertation.pdf` in the repository
- **Technical Slides**: See `slides_theory1_shaders_pipeline.pdf`

## License

This project is part of an MS thesis at Trinity College Dublin. Please check with the repository owner for specific licensing terms.

## Acknowledgments

- Trinity College Dublin for academic support
- AUTOMATIC1111 for the Stable Diffusion WebUI
- The open-source community for the various libraries used

---

**For questions, issues, or feature requests, please open an issue on GitHub.**

**Author**: VKG5  
**Institution**: Trinity College Dublin  
**Project Type**: MS Thesis
