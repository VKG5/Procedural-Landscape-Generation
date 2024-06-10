from bmi_topography import Topography
from bmi_topography.api_key import ApiKey
import matplotlib.pyplot as plt
import numpy as np
import os
import rasterio
from rasterio.transform import from_origin

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

    # Ensure data shape is compatible with rasterio (2D array)
    data = da_resampled.values

    # Save the resampled data to a GeoTIFF file
    transform = from_origin(longitude, latitude + patchSize, (patch.da.x.max() - patch.da.x.min()) / 512, (patch.da.y.max() - patch.da.y.min()) / 512)
    image = f'./images/patch_{patchIndex}.tif'

    with rasterio.open(
        image,
        'w',
        driver='GTiff',
        height=data.shape[0],
        width=data.shape[1],
        count=1,
        dtype=data.dtype,
        crs='+proj=latlong',
        transform=transform,
    ) as dst:
        dst.write(data, 1)

    # Save metadata to a .txt file
    metadata_content = "Himalaya, Mountain, Mount Everest, Mountain Range, Steep mountains, steep, erosion, rock, solid, cliffs, snowy"
    metadata_file = f'./metadata/patch_{patchIndex}.txt'
    
    with open(metadata_file, 'w') as file:
        file.write(metadata_content)

    ## Debugging
    print(f"Saved image to {image} and metadata to {metadata_file}")

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
                    "north" : latitude + patchSize,
                    "west" : longitude,
                    "east" : longitude + patchSize,
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
                        displayDEM(patch, title)

                    # Resample the data to 512x512 pixels
                    # Saving the scarped data
                    if(saveImage):
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
                displayDEM(patch, title)

            # Resample the data to 512x512 pixels
            # Saving the scarped data
            if(saveImage):
                saveDEM(patch, patchSize, patchIndex, longitude, latitude)

        else:
            print(f"Error while retrieving data for ({latitude}, {longitude}) using {demType}")

# Spatial Bounds: North: 61.0001388888889°     
#                 South: -56.0001388888889°     
#                 East: 179.000138888889°     
#                 West: -179.000138888889°
# A higer patch size : 5, requires a lot of memory. 24 GB was not enough for this
generateData(southBound = 26.0, northBound = 36.0, eastBound = 92.0, westBound = 77.0, saveImage=False, viewImage=True, isPatch=True, pSize=0.5)