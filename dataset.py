from bmi_topography import Topography
from bmi_topography.api_key import ApiKey
import matplotlib.pyplot as plt
import numpy as np
import os
from PIL import Image

'''
OpenTopography
API key: a068b49917f1a936e8511b8f003b8f7d
'''
apiKey = ApiKey("a068b49917f1a936e8511b8f003b8f7d")

'''
SRTMGL3 (SRTM GL3 90m)
SRTMGL1 (SRTM GL1 30m)
SRTMGL1_E (SRTM GL1 Ellipsoidal 30m)
AW3D30 (ALOS World 3D 30m)
AW3D30_E (ALOS World 3D Ellipsoidal, 30m)
SRTM15Plus (Global Bathymetry SRTM15+ V2.1)
*NASADEM (NASADEM Global DEM) - Only available in GTiff fomrat and not IMG
COP30 (Copernicus Global DSM 30m)
COP90 (Copernicus Global DSM 90m)

*NASADEM - https://portal.opentopography.org/datasetMetadata?otCollectionID=OT.032021.4326.2
Dataset Spatial Bounds: North: 61.0001388888889°     
                        South: -56.0001388888889°     
                        East: 179.000138888889°     
                        West: -179.000138888889°

class bmi_topography.Topography(dem_type=None, 
                                south=None, 
                                north=None, 
                                west=None, 
                                east=None, 
                                output_format=None, 
                                cache_dir=None, 
                                api_key=None)

For more information, please visit the following link : https://pypi.org/project/bmi-topography/

* Error: The maximum area for NASADEM is 450,000 km2. The selected area is 1,588,116 km2. Each patch/DEM has to be below this limit
'''

# Plot the extracted DEM using matplotlib
def displayDEM(patch, title):
    ## Plot the data - Not necessary
    patch.da.plot(cmap = 'gray')
    plt.title(title)
    plt.show()

# Function to save the data onto disk
# TODO : FIX THE FUNCTIONALITY AND MAKE IT MORE EFFICIENT
def saveDEM(patch, patchSize, patchIndex, longitude, latitude):
    da_resampled = patch.da.interp(
        x=np.linspace(patch.da.x.min(), patch.da.x.max(), 512),
        y=np.linspace(patch.da.y.min(), patch.da.y.max(), 512),
        method="linear"
    )

    # Ensure data shape is compatible with PIL
    # Squeeze is used to remove extra dimensions
    data = da_resampled.values.squeeze() 

    # Normalize the data for image representation
    data_min = np.min(data)
    data_max = np.max(data)
    normalizedData = 255 * (data - data_min) / (data_max - data_min)
    normalizedData = normalizedData.astype(np.uint8)

    ## Debugging
    print(normalizedData, normalizedData.shape)

    # Save the image using Pillow
    image = Image.fromarray(normalizedData)
    imgFile = f'./images/patch_{patchIndex}.png'  # or use .jpg for JPG format
    image.save(imgFile)
    
    # Save metadata to a .txt file
    metadataCount = "Himalaya, Mountain, Mount Everest, Mountain Range, Steep mountains, steep, erosion, rock, solid, cliffs, snowy"
    metadataFile = f'./images/patch_{patchIndex}.txt'
    
    with open(metadataFile, 'w') as file:
        file.write(metadataCount)

    ## Debugging
    print(f"Saved image to {imgFile} and metadata to {metadataFile}")

def generateData(southBound = 26.0, northBound = 30.0, eastBound = 90.0, westBound = 85.0, saveImage = True, viewImage = False, isPatch = True, pSize = 0.5, outputFormat = 'GTiff', demType = 'NASADEM'):
    ## Accessing the API key defined globally
    global apiKey

    # Passing the overall boundary of the required region
    holisticArea = {
        "dem_type" : demType,
        "south" : southBound,
        "north" : northBound,
        "west" : westBound,
        "east" : eastBound,
        "output_format" : outputFormat,
        "cache_dir" : './metadata'
    }

    # Defining the patch size
    patchSize = pSize

    # Create output directories if they don't exist
    os.makedirs('./metadata', exist_ok=True)
    os.makedirs('./images', exist_ok=True)

    ## Debugging
    # print(holisticArea)

    # Iterate over the larger area
    south = holisticArea["south"]
    north = holisticArea["north"]
    west = holisticArea["west"]
    east = holisticArea["east"]

    ## Debugging
    # print(south, north, west, east, patchSize)

    patchIndex = 0
    patch = None

    # Processing the data for patches
    if(isPatch):
        for latitude in np.arange(south, north, patchSize):
            for longitude in np.arange(west, east, patchSize):
                patchParams = {
                    "dem_type" : demType,
                    "south" : latitude,
                    "north" : min(latitude + patchSize, northBound),
                    "west" : longitude,
                    "east" : min(longitude + patchSize, eastBound),
                    "output_format" : outputFormat,
                    "cache_dir" : './metadata'
                }

                ## Debugging
                print(f"Fetching data for {patchIndex}: {patchParams}")

                patch = Topography(**patchParams, api_key=apiKey)

                # Download and locally store topography data
                patch.fetch()

                # Load a cached topography data file into an xarray DataArray
                patch.load()

                # Checking if Patch is valid data or not
                if(patch):
                    print(f"Successfully retrieved data for ({latitude}, {longitude}) using {demType}")
                    # Viewing the scraped data
                    if(viewImage):
                        title = f"Patch {patchIndex}"

                        ## Debugging
                        print(f"Showing image for ({latitude}, {longitude}) using {demType} as {title}")
                        
                        displayDEM(patch, title)

                    # Resample the data to 512x512 pixels
                    # Saving the scarped data
                    if(saveImage):
                        ## Debugging
                        print(f"Saving image for ({latitude}, {longitude}) using {demType}")
                        
                        saveDEM(patch, patchSize, patchIndex, longitude, latitude)

                else:
                    print(f"Error while retrieving data for {patchIndex}, ({latitude}, {longitude}) using {demType}")

                patchIndex += 1

    # Getting a holistic data
    else:
        patchParams = {
            "dem_type" : demType,
            "south" : south,
            "north" : north,
            "west" : west,
            "east" : east,
            "output_format" : outputFormat,
            "cache_dir" : './metadata'
        }

        ## Debugging
        print(f"Fetching data for {patchIndex}: {patchParams}")

        patch = Topography(**patchParams, api_key=apiKey)

        # Download and locally store topography data
        patch.fetch()

        # Load a cached topography data file into an xarray DataArray
        patch.load()

        # Checking if Patch is valid data or not
        if(patch):
            print(f"Successfully retrieved data for ({latitude}, {longitude}) using {demType}")
            # Viewing the scraped data
            if(viewImage):
                title = f"({latitude}, {longitude})"
                
                ## Debugging
                print(f"Showing image for ({latitude}, {longitude}) using {demType} as {title}")
                
                displayDEM(patch, title)

            # Resample the data to 512x512 pixels
            # Saving the scarped data
            if(saveImage):
                ## Debugging
                print(f"Saving image for ({latitude}, {longitude}) using {demType}")

                saveDEM(patch, patchSize, patchIndex, longitude, latitude)

        else:
            print(f"Error while retrieving data for ({latitude}, {longitude}) using {demType}")

# Spatial Bounds: North: 61.0001388888889°     
#                 South: -56.0001388888889°     
#                 East: 179.000138888889°     
#                 West: -179.000138888889°
# A higer patch size : 5, requires a lot of memory. 24 GB was not enough for this
generateData(southBound = 26.0, northBound = 36.0, eastBound = 92.0, westBound = 77.0, saveImage=True, viewImage=False, isPatch=True, pSize=2.5)