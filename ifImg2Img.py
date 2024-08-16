if generationType == "img2img":
    if not os.path.isfile(imagePath):
        print(f"Invalid image path : {imagePath}")
        return ""

    # Used if you want to encode the image being passed to base64
    with open(imagePath, "rb") as image_file:
        encoded_image = base64.b64encode(image_file.read()).decode("utf-8")
    
    payload.update({
        "init_images": [encoded_image],
        "sampler_name": sampler,
        "scheduler": scheduler,
    })