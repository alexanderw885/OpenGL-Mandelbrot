#version 400 core
in vec2 coords;
out vec4 fragColor;
uniform float aspectRatio;
uniform float centerX;
uniform float centerY;
uniform float scale;
uniform int maxIter;

vec2 complex_mult(vec2 u, vec2 v)
{
    return vec2(u.x*v.x - u.y*v.y, u.x*v.y + u.y*v.x);
}

void main()
{
    vec2 uv = vec2((coords.x * aspectRatio), (coords.y));
    uv *= vec2((scale), (scale));
    uv += vec2((centerX), (centerY));

    // vec2 z = uv;
    // vec2 c = vec2(-0.77146, -0.10119);
    vec2 z = vec2(0.0,0.0);
    vec2 c = uv;

    float bound = 4.0;
    int num_iter = 0;
    float dist = 0;
    int max_period = 20;
    int curr_period = 20;
    int period_out = -1;
    int period_iter = -1;
    vec2 zOld = vec2(10,10);

    for(int i = 0; i < maxIter; i++)
    {
        z = complex_mult(z, z);
        z += c;

        if(dot(z-zOld, z-zOld) < 1e-6)
        {
            period_out = curr_period;
            period_iter = i;
            break;
        }

        curr_period += 1;
        if(curr_period > max_period)
        {
            curr_period = 0;
            zOld = z;
        }

        float d = float(dot(z,z));
        if (d >= bound)
        {
            num_iter = i;
            dist = d;
            break;
        }
    }
    float n = float(num_iter) / float(maxIter);

    // fragColor = vec4(0,1-n,n,1);
    fragColor = vec4(0,0,0,1);
    if(num_iter > 0)
    {
        if ((num_iter & 1) != 0){fragColor = vec4(0,0,0,1);}
        else {fragColor = vec4(1,1,1,1);}
    }
    if(period_out >= 0)
    {
        if((num_iter & 1) != 0){fragColor.r = 0.6;}
        else {fragColor.b = 0.6;}
    }
}

