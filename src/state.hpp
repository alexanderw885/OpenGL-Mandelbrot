#include <glad/glad.h>
#include "shader.hpp"

class State
{
public:
    double center[2] = {0,0};
    float scale = 1.5;
    int max_iter = 50;

    Shader programs[2];
    

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

    void scale_in(){
        scale *= d_scale;
        std::cout << max_iter << "   " << scale << std::endl;
        if (num_programs > 1 && curr_program == 0 && scale < 1e-4)
        {
            set_current_program(1);
        }
    }
    void scale_out(){
        scale /= d_scale;
        std::cout << max_iter << "   " << scale << std::endl;
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

    int curr_program = 0;
    int num_programs = 0;

    void set_current_program(int i)
    {
        curr_program = i;
        std::cout << "changing to program " << i << std::endl;
        programs[i].use();
    }
};