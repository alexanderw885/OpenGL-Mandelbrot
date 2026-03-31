import numpy as np 
from PIL import Image 

start = np.array([0, 0, 1])
end = np.array([[0, 1, 0]])
num_colors = 255

def main():
    map = np.ndarray([1,num_colors, 3])
    for i in range(num_colors):
        map[0][i] = start + (end - start)*np.sin(np.pi*(i/num_colors))
    output = np.array(map*255, dtype=np.uint8)
    print(output.shape)

    Image.fromarray(output, "RGB").save("Red_Blue.jpg")
    pass


if __name__ == "__main__":
    main()