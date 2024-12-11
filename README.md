# Procedural-Landscape-Generation
Thesis for MS at Trinity College Dublin. A workflow for generating landscapes using AI (Stable Diffusion) with contemporary Procedural Methods.

## Overview
The project aims to simplify landscape generation by overcoming the learning curve and limitations of traditional tools like World Machine, Gaea, and node-based workflows such as Blender Geometry Nodes and Houdini. It is designed to be accessible to hobbyists, companies, and the community while maintaining high-quality outputs. This can be extended in multiple directions, including generating textures for the landscapes, training a LoRA such that it only generates a particular type of map and much more. Some useful links are shared below:

- Checkpoint - https://civitai.com/models/16826/game-landscape-heightmap-genrator
- LoRA (Low Rank Adaptation) - https://civitai.com/models/18621/game-landscape-heightmap-genrator-lora
- SD API - https://github.com/AUTOMATIC1111/stable-diffusion-webui/wiki/API

## Tutorial
The following tutorial by [freeCodeCamp](https://www.youtube.com/watch?v=dMkiOex_cKU) gives a good run-down about Stable Diffusion and it's use. Instead of using the full Stable Diffusion model, I have trained a LoRA for our specific usecase. [How to train a LoRA](https://civitai.com/models/22530), this link goes over the details of how to train a LoRA and use it with your SD models.

## Dataset
For the dataset, I am using [OpenTopography](https://opentopography.org/) for satellite images. Further, a script for generating varied landscapes used Geometry Nodes and Noise (Procedural Landscape Generation) was used. The script was written for Blender Python API, and used in conjunction with Geometry Nodes to give unique results. The dataset is comprised of Digital Elevation Models (DEMs) which represent the landscape as a heightmap/heightfield.
