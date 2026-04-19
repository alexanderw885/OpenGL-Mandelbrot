#version 400 core

#ifdef DOUBLE
    #define dynf double
    #define dfvec2 dvec2
#else
    #define dynf float
    #define dfvec2 vec2
#endif

in vec2 coords;
out vec4 fragColor;
uniform float aspectRatio;
uniform double centerX;
uniform double centerY;
uniform float scale;
uniform int maxIter;

dfvec2 complex_mult(dfvec2 u, dfvec2 v)
{
    return dfvec2(u.x*v.x - u.y*v.y, u.x*v.y + u.y*v.x);
}

void main()
{
    dfvec2 uv = vec2((coords.x * aspectRatio), (coords.y));
    uv *= vec2((scale), (scale));
    uv += dfvec2((centerX), (centerY));

    dfvec2 z = uv;
    dfvec2 c = vec2(-0.77146, -0.10119);

    float bound = 4.0;
    int num_iter = 0;
    float dist = 0;

    int period_start = -1;
    dfvec4 old = dfvec4(0,0,0,0);

    for(int i = 0; i < maxIter; i++)
    {
        old.xy = old.zw;
        old.zw = z;

        z = complex_mult(z, z);
        z += c;

        if(i > 20 && dot(z - old.xy, z - old.xy) < 1e-8)
        {
            period_start = i;
            break;
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
    else if (period_start >= 0)
    {
        // if(period_start % 2 == 0){fragColor.r = 1;}
        // else {fragColor.b = 1;}

        float n = float(period_start) / 50000.;
        fragColor.b = n;
    }
}

