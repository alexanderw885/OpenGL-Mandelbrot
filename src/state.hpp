#include <glad/glad.h>
#include <rapidjson/include/rapidjson/document.h>
#include <iostream>
#include <fstream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include "shader.hpp"

class State
{
public:
    double center[2] = {0,0};
    float scale = 1.5;
    int max_iter = 50;
    float color_scale = 0.05;
    bool image_pressed = false;

    int pic_width = 1920;
    int pic_height = 1080;

    Shader programs[2];
    unsigned int texture;
    
    State(const char* config_path)
    {
        rapidjson::Document cfg;

        std::fstream cfg_file(config_path);
        std::string json_cfg(
            (std::istreambuf_iterator<char>(cfg_file)),
            std::istreambuf_iterator<char>()
        );


        rapidjson::ParseResult ok = cfg.Parse(json_cfg.c_str());
        if (!ok)
        {
            std::cout << "Error code " << ok.Code() << std::endl;
            exit(EXIT_FAILURE);
        }
        cfg_file.close();

        scale = cfg["scale"].GetFloat();
        max_iter = cfg["iterations"].GetInt();
        center[0] = cfg["center"][0].GetDouble();
        center[1] = cfg["center"][1].GetDouble();
        pic_width = cfg["pic_width"].GetInt();
        pic_height = cfg["pic_height"].GetInt();
        texture = set_colormap(cfg["colormap"].GetString());
    }

    unsigned int set_colormap(const char* filename)
    {
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_1D, texture);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int tWidth, tHeight, nrChannels;
        unsigned char* data = stbi_load((std::string(ASSET_PATH) + "colormaps/" + filename).c_str(), &tWidth, &tHeight, &nrChannels, 0);
        if(!data)
        {
            std::cout << "Error loading texture" << std::endl;
        }

        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB8, tWidth, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        std::cout << (std::string(ASSET_PATH) + "colormaps/" + filename).c_str()<< std::endl;
        return texture;
    }

    void set_shader(const char* fragmentShader)
    {
        Shader prog;
        prog.init("vertex.vs", fragmentShader);
        programs[0] = prog;
        if (num_programs == 0) num_programs = 1;
        set_current_program(0);
    }
    void set_double_shader(const char* fragmentShader)
    {
        Shader prog;
        prog.init("vertex.vs", fragmentShader);
        programs[1] = prog;
        if (num_programs < 2) num_programs = 2;
    }


    void update(float aspect_ratio){
        
        programs[curr_program].setFloat("scale", scale);
        programs[curr_program].setInt("maxIter", max_iter);
        programs[curr_program].setFloat("aspectRatio", aspect_ratio);
        programs[curr_program].setFloat("colorScale", color_scale);

        if(curr_program == 0)
        {
            programs[curr_program].setFloat("centerX", (float)(center[0]));
            programs[curr_program].setFloat("centerY", (float)(center[1]));
        }
        else 
        {
            programs[curr_program].setDouble("centerX", center[0]);
            programs[curr_program].setDouble("centerY", center[1]);
        }
    }

    void center_right(){center[0] += (scale*d_center);}
    void center_left(){center[0] -= (scale*d_center);}
    void center_up(){center[1] += (scale*d_center);}
    void center_down(){center[1] -= (scale*d_center);}

    void color_up(){
        color_scale /= d_color;
        std::cout << max_iter << "   " << scale << "   " << color_scale << std::endl;
    }
    void color_down(){
        color_scale *= d_color;
        std::cout << max_iter << "   " << scale << "   " << color_scale << std::endl;
    }

    void scale_in(){
        scale *= d_scale;
        std::cout << max_iter << "   " << scale << "   " << color_scale << std::endl;
        if (num_programs > 1 && curr_program == 0 && scale < 1e-4)
        {
            set_current_program(1);
        }
    }
    void scale_out(){
        scale /= d_scale;
        std::cout << max_iter << "   " << scale << "   " << color_scale << std::endl;
        if (num_programs > 1 && curr_program == 1 && scale >= 1e-4)
        {
            set_current_program(0);
        }
    }


    void iter_up()
    {
        max_iter /= d_iter;
        if(max_iter <= 50) max_iter += 1;
        std::cout << max_iter << "   " << scale << std::endl;
    }
    void iter_down(){
        max_iter *= d_iter;
        if (max_iter <= 50) max_iter -= 1;
        if(max_iter <= 10) max_iter = 10;
        std::cout << max_iter << "   " << scale << std::endl;
    }

private:
    float d_center = 0.05;
    float d_scale = 0.95;
    float d_iter = 0.98;
    float d_color = 0.97;

    int curr_program = 0;
    int num_programs = 0;

    void set_current_program(int i)
    {
        curr_program = i;
        std::cout << "changing to program " << i << std::endl;
        programs[i].use();
    }
};