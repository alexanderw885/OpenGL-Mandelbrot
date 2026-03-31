import numpy as np
import matplotlib.pyplot as plt
from PIL import Image

num_colors = 256

def main():
    map = plt.colormaps.get_cmap("twilight")

    colors = np.ndarray([1,num_colors,3])
    colors[0] = map(np.linspace(0,1,num_colors))[:,:3]
    
    output = np.array(colors * num_colors, dtype=np.uint8)
    Image.fromarray(output, "RGB").save("twilight.png")

    



if __name__ == "__main__":
    main()