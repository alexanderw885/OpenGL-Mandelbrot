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
    // Initial setup
    vec2 uv = vec2((coords.x * aspectRatio), (coords.y));
    uv *= vec2((scale), (scale));
    uv += vec2((centerX), (centerY));

    // Set Z, C
    vec2 z = vec2(0,0);
    vec2 c = uv;

    // Values for colouring
    float bound = 4.0;
    int num_iter = 0;
    float dist = 0;

    // Period detection for early escape
    int max_period = 50;
    int curr_period = 50;
    vec2 oldZ = vec2(100, 100);

    for(int i = 0; i < maxIter; i++)
    {
        // Calculate new Z
        z = complex_mult(z, z);
        z += c;

        // Check for perodicity
        if (dot(z - oldZ, z - oldZ) <= 1e-8)
        {
            break;
        }
        curr_period++;
        if (curr_period >= max_period)
        {
            curr_period = 0;
            oldZ = z;
        }

        // Check out of bounds
        float d = float(dot(z,z));
        if (d >= bound)
        {
            num_iter = i;
            dist = d;
            break;
        }
    }
    float n = float(num_iter + 1) - log(log(sqrt(dist)))/log(2); // smooth colouring
    n = float(int(20*n) % 255) / 255;

    fragColor = vec4(n,n,n,1);
}

