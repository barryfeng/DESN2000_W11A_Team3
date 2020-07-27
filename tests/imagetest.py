from PIL import Image
import numpy as np

im = Image.open('image.png')

pixels = list(im.getdata())
width, height = im.size 
pixels = [pixels[i * width:(i + 1) * width] for i in xrange(height)]
np.savetxt("pixel_data.txt", pixels, delimiter=" ") 