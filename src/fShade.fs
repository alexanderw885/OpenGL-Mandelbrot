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

    vec2 z = uv;
    vec2 c = vec2(-0.77146, -0.10119);

    float bound = 4.0;
    int num_iter = 0;
    float dist = 0;

    for(int i = 0; i < maxIter; i++)
    {
        z = complex_mult(z, z);
        z += c;

        float d = float(dot(z,z));
        if (d >= bound)
        {
            num_iter = i;
            dist = d;
        }
    }
    float n = float(num_iter) / float(maxIter);

    fragColor = vec4(n,n,n,1);
}

