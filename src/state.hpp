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

using namespace std;

struct ShaderList {
    Shader shader;
    float start_scale;
};


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

    vector<ShaderList> programs;

    Shader programs_array[2];
    unsigned int texture;
    
    State(const char* config_path)
    {
        rapidjson::Document cfg;

        fstream cfg_file(config_path);
        string json_cfg(
            (istreambuf_iterator<char>(cfg_file)),
            istreambuf_iterator<char>()
        );


        rapidjson::ParseResult ok = cfg.Parse(json_cfg.c_str());
        if (!ok)
        {
            cout << "Error code " << ok.Code() << endl;
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

        // Load shaders
        for (rapidjson::Value::ConstValueIterator iter = cfg["shaders"].Begin(); iter != cfg["shaders"].End(); ++iter) 
        {
            const char* filename = (*iter)["filename"].GetString();
            const char* type = (*iter)["type"].GetString();
            
            Shader prog;
            prog.init("vertex.vs", filename, type);

            ShaderList list;
            list.shader = prog;
            list.start_scale = (*iter)["start"].GetFloat();
            programs.push_back(list);
        }
        // Sort vector by scale, descending
        std::sort(
            programs.begin(), 
            programs.end(),
            [](const ShaderList& a, const ShaderList& b) {
                return a.start_scale > b.start_scale;
            });

        // activate current shader
        curr_program = programs.size() - 1;
        for(int i = 1; i < programs.size(); i++)
        {
            if (programs[i].start_scale < scale)
            {
                cout << "selected\n";
                curr_program = i - 1;
                break;
            }
        }
        std::cout << "using program " << curr_program << " with id " << programs[curr_program].shader.id << endl;
        programs[curr_program].shader.use();
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
        unsigned char* data = stbi_load((string(ASSET_PATH) + "colormaps/" + filename).c_str(), &tWidth, &tHeight, &nrChannels, 0);
        if(!data)
        {
            cout << "Error loading texture" << endl;
        }

        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB8, tWidth, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        cout << (string(ASSET_PATH) + "colormaps/" + filename).c_str()<< endl;
        return texture;
    }


    void update(float aspect_ratio){
        
        programs[curr_program].shader.setFloat("scale", scale);
        programs[curr_program].shader.setInt("maxIter", max_iter);
        programs[curr_program].shader.setFloat("aspectRatio", aspect_ratio);
        programs[curr_program].shader.setFloat("colorScale", color_scale);
        programs[curr_program].shader.setDouble("centerX", center[0]);
        programs[curr_program].shader.setDouble("centerY", center[1]);
    }

    void center_right(){center[0] += (scale*d_center);}
    void center_left(){center[0] -= (scale*d_center);}
    void center_up(){center[1] += (scale*d_center);}
    void center_down(){center[1] -= (scale*d_center);}

    void color_up(){
        color_scale /= d_color;
        cout << max_iter << "   " << scale << "   " << color_scale << endl;
    }
    void color_down(){
        color_scale *= d_color;
        cout << max_iter << "   " << scale << "   " << color_scale << endl;
    }

    void scale_in(){
        scale *= d_scale;
        cout << max_iter << "   " << scale << "   " << color_scale << endl;
        if (curr_program < programs.size() -1 && scale < programs[curr_program+1].start_scale)
        {
            set_current_program(curr_program+1);
        }
    }
    void scale_out(){
        scale /= d_scale;
        cout << max_iter << "   " << scale << "   " << color_scale << endl;
        if (curr_program > 0 && scale > programs[curr_program].start_scale)
        {
            set_current_program(curr_program - 1);
        }
    }


    void iter_up()
    {
        max_iter /= d_iter;
        if(max_iter <= 50) max_iter += 1;
        cout << max_iter << "   " << scale << endl;
    }
    void iter_down(){
        max_iter *= d_iter;
        if (max_iter <= 50) max_iter -= 1;
        if(max_iter <= 10) max_iter = 10;
        cout << max_iter << "   " << scale << endl;
    }

private:
    float d_center = 0.05;
    float d_scale = 0.95;
    float d_iter = 0.98;
    float d_color = 0.97;

    int curr_program = 0;

    void set_current_program(int i)
    {
        curr_program = i;
        cout << "changing to program " << i << endl;
        programs[i].shader.use();
    }
};