#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <sys/stat.h>

#include "shader.hpp"
#include "state.hpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void poll_inputs(GLFWwindow* window, State* state);
void render_image(State* state);


int curr_width = 1980 / 2;
int curr_height = 1080 / 2;

const char* fragmentShader = "fTexture.fs";
const char* doubleShader = "dTexture.fs";
const char* colormap = "romaO.png";

int main()
{
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create the window
    GLFWwindow* window = glfwCreateWindow(curr_width, curr_height, "Zoooooom", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }  

    // Set OpenGL viewport
    glViewport(0, 0, curr_width, curr_height);

    // Reset the viewport whenever the window is resized
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // Shader program;
    // program.init("vertex.vs", fragmentShader);

    // // Set up texture
    // unsigned int texture;
    // glGenTextures(1, &texture);
    // glBindTexture(GL_TEXTURE_1D, texture);
    // glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // int tWidth, tHeight, nrChannels;
    // unsigned char* data = stbi_load((std::string(ASSET_PATH)+"/colormaps/" + colormap).c_str(), &tWidth, &tHeight, &nrChannels, 0);
    // if(!data)
    // {
    //     std::cout << "Error loading texture" << std::endl;
    // }

    // glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB8, tWidth, 0, GL_RGB, GL_UNSIGNED_BYTE, data);


    // Set up background triangles
    float vertices[] = {
        -1, -1, 0,
        -1,  1, 0,
         1,  1, 0,
         1, -1, 0
    };
    int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // Only using one vertex array and program, start outside of loop
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // program.use();

    // Initialize state
    std::string cfg_path = std::string(CFG_PATH) + "cfg.json";

    // State state;
    State state(cfg_path.c_str());

    state.set_shader(fragmentShader);
    if (doubleShader != NULL)
    {
        state.set_double_shader(doubleShader);
    }
    
    while(!glfwWindowShouldClose(window))
    {
        poll_inputs(window, &state);

        // Render
        float aspect_ratio = (float)(curr_width) / (float)(curr_height);
        state.update(aspect_ratio);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Handle events
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    curr_width = width;
    curr_height = height;
}


void poll_inputs(GLFWwindow* window, State* state)
{
    // Exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    // Pan
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        state->center_right();
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        state->center_left();
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        state->center_up();
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        state->center_down();
    }
    // Zoom
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        state->scale_in();
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        state->scale_out();
    }
    // Iterations
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        state->iter_up();
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
        state->iter_down();
    }
    // Color scale
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
        state->color_up();
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS){
        state->color_down();
    }

    // take image
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && !state->image_pressed)
    {
        state->image_pressed = true;
        GLint oldVP[4];
        glGetIntegerv(GL_VIEWPORT, oldVP);
        std::cout << "Taking image\n";

        render_image(state);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(oldVP[0], oldVP[1], oldVP[2], oldVP[3]);
    }
    if (state->image_pressed && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
    {
        std::cout << "Release key\n";
        state->image_pressed = false;
    }
}


void render_image(State* state) {
    int pWidth = state->pic_width;
    int pHeight = state->pic_height;
    state->update((float)(pWidth) / (float)(pHeight));

    unsigned int FBO, tex;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glViewport(0, 0, pWidth, pHeight);

    // Texture
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pWidth, pHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    {

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // read buffer into array
        unsigned char* data = new unsigned char[pWidth * pHeight * 4];
        glReadPixels(0, 0, state->pic_width, state->pic_height, GL_RGBA, GL_UNSIGNED_BYTE, data);

        // flip data
        for(int y = 0; y < pHeight / 2; ++y) {
            for(int x = 0; x < pWidth * 4; ++x) {
                std::swap(data[y * pWidth * 4 + x], data[(pHeight - 1 - y) * pWidth * 4 + x]);
            }
        }

        std::string last = std::string(OUTPUT_PATH) + "last.png";
        stbi_write_png(last.c_str(), pWidth, pHeight, 4, data, pWidth*4);

        double x = state->center[0];
        double y = state->center[1];
        double zoom = state->scale;
        std::string output_name = std::string(OUTPUT_PATH)
            + "x" + std::to_string(x)
            + "y" + std::to_string(y)
            + "scale" + std::to_string(zoom)
            + ".png";

        stbi_write_png(output_name.c_str(), pWidth, pHeight, 4, data, pWidth*4);
        std::cout << "Saved image to " << output_name << std::endl;
        delete[] data;

    } else {
        std::cout << "Error completing framebuffer, picture not taken\n";
    }
}