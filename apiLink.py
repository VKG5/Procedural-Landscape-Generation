# Libraries for sending requests to SD
import json
import requests
import io
import base64

from PIL import Image, PngImagePlugin

# Change this URL according to your local host on the browser
url = "http://127.0.0.1:7860/"

# Payload is the prompt that will be passed to the server
# You can have multiple options, which can be parsed in a JSON format
## *Refer to the following link for more details about the API : http://127.0.0.1:7860/docs/

# The prompt start with the trigger word to make sure consistent results are produced
prompt = "LandscapeAI"

payload = {
    # Main Prompt
    "prompt": prompt,
    "negative_prompt": "",

    # Steps and Batch properties
    "batch_size": 1,
    "n_iter": 2,
    "steps": 99,
    "cfg_scale": 7,
    "width": 512,
    "height": 512,

    # Hi-Resolution
    "enable_hr": False,
    "hr_scale": 2,
    "hr_prompt": "",
    "hr_negative_prompt": "",

    # Seed -1 means a random seed per generation
    "seed": -1
}

# Send defined payload to the URL through the API
# You can send to different kinds of APIs
# 1. /sdapi/v1/txt2img - Text to Image
# 2. /sdapi/v1/img2img - Image to Image
response = requests.post(url = f'{url}/sdapi/v1/txt2img', json = payload)
r = response.json()

# Decode and save the image (We get the image in a Binary 64 format)
'''
After the backend does its thing, the API sends the response back in a variable that was assigned above: response. 
The response contains three entries; images, parameters, and info, and I have to find some way to get the information from these entries.
'''
for i in r['images']:
    image = Image.open(io.BytesIO(base64.b64decode(i.split(",",1)[0])))

    # Getting the details about the images, such as the prompt, etc.
    png_payload = {
        "image": "data:image/png;base64," + i
    }

    response2 = requests.post(url = f'{url}/sdapi/v1/png-info', json = png_payload)

    pnginfo = PngImagePlugin.PngInfo()
    pnginfo.add_text("parameters", response2.json().get("info"))

    # Save the image
    image.save('Outputs/output.png', pnginfo=pnginfo)