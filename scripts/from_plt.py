import numpy as np
import matplotlib.pyplot as plt
from PIL import Image

num_colors = 256

def main():
    colname = "hsv"


    map = plt.colormaps.get_cmap(colname)

    colors = np.ndarray([1,num_colors,3])
    colors[0] = map(np.linspace(0,1,num_colors))[:,:3]
    
    output = np.array(colors * num_colors, dtype=np.uint8)
    Image.fromarray(output, "RGB").save(colname+".png")

    



if __name__ == "__main__":
    main()