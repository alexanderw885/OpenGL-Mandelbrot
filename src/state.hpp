#include <glad/glad.h>
#include "shader.hpp"

class State
{
public:
    float center[2] = {0,0};
    float scale = 1.5;
    int max_iter = 50;

    void update(Shader program){
        program.setFloat("centerX", center[0]);
        program.setFloat("centerY", center[1]);
        program.setFloat("scale", scale);
        program.setInt("maxIter", max_iter);
    }

    void center_right(){center[0] += (scale*d_center);}
    void center_left(){center[0] -= (scale*d_center);}
    void center_up(){center[1] += (scale*d_center);}
    void center_down(){center[1] -= (scale*d_center);}

    void scale_in(){
        scale *= d_scale;
        std::cout << max_iter << "   " << scale << std::endl;
    }
    void scale_out(){
        scale /= d_scale;
        std::cout << max_iter << "   " << scale << std::endl;
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
};