import numpy as np 
from PIL import Image 

start = np.array([0, 0, 1])
end = np.array([[0, 1, 0]])
num_colors = 256

def main():
    map = np.ndarray([1,num_colors // 2, 3])

    for i in range(num_colors // 2):
        map[0][i] = start + (end - start)*(2 * i / num_colors)

    map = np.concat([map, map[:,::-1]], axis=1)

    output = np.array(map*255, dtype=np.uint8)
    print(output.shape)

    Image.fromarray(output, "RGB").save("RBLinear.jpg")
    pass


if __name__ == "__main__":
    main()