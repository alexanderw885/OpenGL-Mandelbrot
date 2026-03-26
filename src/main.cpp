#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "state.hpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void poll_inputs(GLFWwindow* window, Shader program, State* state);


int curr_width = 1980 / 2;
int curr_height = 1080 / 2;

int main()
{
     // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create the window
    GLFWwindow* window = glfwCreateWindow(curr_width, curr_height, "Can it truly be done?", NULL, NULL);
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


    Shader program = Shader("/src/vShade.vs", "/src/period_2.fs");

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
    program.use();

    // Initialize state
    State state;
    
    while(!glfwWindowShouldClose(window))
    {
        poll_inputs(window, program, &state);

        // Render
        float aspect_ratio = (float)(curr_width) / (float)(curr_height);
        
        program.setFloat("aspectRatio", aspect_ratio);
        state.update(program);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        // glfwSetWindowShouldClose(window, true);

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


void poll_inputs(GLFWwindow* window, Shader program, struct State *state)
{
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
}