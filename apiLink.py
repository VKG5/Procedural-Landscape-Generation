# Libraries for sending requests to SD
import json
import requests
import io
import base64

from PIL import Image, PngImagePlugin

# Change this URL according to your local host on the browser - With webui
# url = "http://127.0.0.1:7860/"

# Set the API endpoint URL for --nowebui argument
url = "http://localhost:7861/"

# Payload is the prompt that will be passed to the server
# You can have multiple options, which can be parsed in a JSON format
## *Refer to the following link for more details about the API : http://127.0.0.1:7860/docs/

def runStableDiffusionAPI(generationType = "txt2img", prompt = "A mountain range", sampler = 'Euler a', scheduler = 'Automatic', dimesnions = 512, steps = 50, isUpscale = False, upscaleFactor = 2, seed = -1):
    # Make sure the prompt starts with the trigger word to make sure consistent results are produced
    triggerWords = "gamelandscapeheightmap512"
    finalPrompt = triggerWords + "\," + prompt
    
    # Add any negative prompt you want to add, for example, if you don't want colors, pass in color as the prompt
    negativePrompt = "Colors"
    payload = {
        # Main Prompt
        "prompt": finalPrompt,
        "negative_prompt": negativePrompt,

        # Steps and Batch properties
        "batch_size": 1,
        "n_iter": 2,
        "steps": steps,
        "cfg_scale": 7,
        "width": dimesnions,
        "height": dimesnions,

        # Setting the sampler name and scheduler type - Recommended 
        # "sampler_name": sampler,
        # "scheduler": scheduler,
        
        # Denoising strength for the image - [0, 1]
        "denoising_strength": 0.75,

        # Hi-Resolution
        "enable_hr": isUpscale,
        "hr_scale": upscaleFactor,
        "hr_prompt": "",
        "hr_negative_prompt": "",

        # Seed -1 means a random seed per generation
        "seed": seed
    }

    # Send defined payload to the URL through the API
    # You can send to different kinds of APIs
    # 1. /sdapi/v1/txt2img - Text to Image
    # 2. /sdapi/v1/img2img - Image to Image
    response = requests.post(url = f'{url}/sdapi/v1/{generationType}', json = payload)
    r = response.json()

    # Decode and save the image (We get the image in a Binary 64 format)
    '''
    After the backend does its thing, the API sends the response back in a variable that was assigned above: response. 
    The response contains three entries; images, parameters, and info, and I have to find some way to get the information from these entries.
    '''
    for i in r['images']:
        # Decoding the 64-bit binary image to a PNG format
        image = Image.open(io.BytesIO(base64.b64decode(i.split(",",1)[0])))

        # Getting the details about the images, such as the prompt, etc.
        png_payload = {
            "image": "data:image/png;base64," + i
        }

        response2 = requests.post(url = f'{url}/sdapi/v1/png-info', json = png_payload)

        pnginfo = PngImagePlugin.PngInfo()
        pnginfo.add_text("parameters", response2.json().get("info"))

        ## Debugging
        # Extracting Hash Code for saving image with unique name
        # There are 2 elements in the generated list, b'tEXt', and the details of the generation both of type 'byte'
        # Converting back to string before accessing the data
        byteData = pnginfo.chunks[0][1]
        stringData = byteData.decode('utf-8')

        ## Debugging
        # print(stringData)

        lines = stringData.split(',')
        imageHash = None
        for line in lines:
            if "Seed:" in line:
                imageHash = line.split("Seed:")[1].strip()
                break

        ## Debugging
        # print(imageHash)
    
        # Save the image at a custom path
        savePath = f'outputs/{imageHash}.png'
        image.save(savePath, pnginfo=pnginfo)
        
        ## Debugging
        print(f"Image saved at: {savePath}")

        return savePath