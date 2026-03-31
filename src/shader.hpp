#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

class Shader
{
public:
    unsigned int id;

    // constructor
    Shader(const char* vertexPath, const char* fragmentPath)
    {
    // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try 
        {
            // open files
            vShaderFile.open(std::string(ASSET_PATH)+"/shaders/" + vertexPath);
            fShaderFile.open(std::string(ASSET_PATH)+"/shaders/" + fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode   = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();
        
        // 2. compile shaders
        unsigned int vShader = compile_shader(vShaderCode, GL_VERTEX_SHADER);
        unsigned int fShader = compile_shader(fShaderCode, GL_FRAGMENT_SHADER);
        
        id = glCreateProgram();
        glAttachShader(id, vShader);
        glAttachShader(id, fShader);
        glLinkProgram(id);
        
        int success;
        char infoLog[512];
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        glDeleteShader(vShader);
        glDeleteShader(fShader);
    }
    void use(){glUseProgram(id);}

    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value); 
    }

    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(id, name.c_str()), value); 
    }

    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(id, name.c_str()), value); 
    }


private:
    unsigned int compile_shader(const char* source, GLenum type)
    {
        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        return shader;
    }

};