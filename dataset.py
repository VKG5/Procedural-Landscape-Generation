from bmi_topography import Topography
from bmi_topography.api_key import ApiKey
import matplotlib.pyplot as plt
import xarray as xr
import numpy as np
import rasterio
from rasterio.transform import from_origin

'''
OpenTopography
API key: a068b49917f1a936e8511b8f003b8f7d
'''
api_key = ApiKey("a068b49917f1a936e8511b8f003b8f7d")

'''
SRTMGL3 (SRTM GL3 90m)
SRTMGL1 (SRTM GL1 30m)
SRTMGL1_E (SRTM GL1 Ellipsoidal 30m)
AW3D30 (ALOS World 3D 30m)
AW3D30_E (ALOS World 3D Ellipsoidal, 30m)
SRTM15Plus (Global Bathymetry SRTM15+ V2.1)
NASADEM (NASADEM Global DEM)
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
params = Topography.DEFAULT.copy()
params["dem_type"] = 'NASADEM'
params["south"] = 39.93
params["north"] = 40.00
params["west"] = -105.33
params["east"] = -105.26
params["output_format"] = 'GTiff'
params["cache_dir"] = './metadata'

## Debugging
print(params)

boulder = Topography(**params, api_key=api_key)

# Download and locally store topography data
boulder.fetch()

# Load a cached topography data file into an xarray DataArray
boulder.load()

# Resample the data to 512x512 pixels
da_resampled = boulder.da.interp(
    x=np.linspace(boulder.da.x.min(), boulder.da.x.max(), 512),
    y=np.linspace(boulder.da.y.min(), boulder.da.y.max(), 512),
    method="linear"
)

# Plot the data
boulder.da.plot(cmap = 'gray')
plt.show()