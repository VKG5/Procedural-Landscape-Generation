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
def generatePatchData(southBound = 26.0, northBound = 30.0, eastBound = 85.0, westBound = 90.0, saveImage = True, viewImage = False, pSize = 0.5, outputFormat = 'GTiff', demType = 'NASADEM'):
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
    # print(south, north, west, east)

    patchIndex = 0

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

            if(viewImage):
                ## Plot the data - Not necessary
                patch.da.plot(cmap = 'gray')
                plt.title(f"Patch {patchIndex}")
                plt.show()

            # Resample the data to 512x512 pixels
            if(saveImage):
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

            patchIndex += 1

generatePatchData(saveImage=False, viewImage=True)