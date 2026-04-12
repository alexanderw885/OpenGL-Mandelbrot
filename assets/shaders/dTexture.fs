#version 400 core
in vec2 coords;
out vec4 fragColor;
uniform float aspectRatio;
uniform double centerX;
uniform double centerY;
uniform float scale;
uniform int maxIter;
uniform float colorScale;
uniform sampler1D tex;

dvec2 complex_mult(dvec2 u, dvec2 v)
{
    return dvec2(u.x*v.x - u.y*v.y, u.x*v.y + u.y*v.x);
}

void main()
{
    // Initial setup
    vec2 temp = vec2((coords.x * aspectRatio), (coords.y));
    temp *= vec2((scale), (scale));
    dvec2 uv = temp + dvec2((centerX), (centerY));

    // Set Z, C

    // Mandelbrot
    dvec2 c = uv;
    dvec2 z = dvec2(0,0);

    // Broken mandelbrot
    // dvec2 c = uv;
    // dvec2 z = dvec2(0.2, -0.5);

    // Julia set 1
    // dvec2 z = uv;
    // dvec2 c = dvec2(-0.5123, 0.5125);

    // Julia set 2
    // dvec2 z = uv;
    // dvec2 c = dvec2(-0.4, 0.6);

    // Test
    // dvec2 c = uv;
    // dvec2 z = dvec2(-0.1528, 1.0397);

    // for more efficient computation
    double zx_sq = z.x * z.x;
    double zy_sq = z.y * z.y;    

    // Values for colouring
    float bound = 255.0;
    int num_iter = -1;
    float dist = 0;

    // Period detection for early escape
    int max_period = 50;
    int curr_period = 50;
    dvec2 oldZ = dvec2(100, 100);

    for(int i = 0; i < maxIter; i++)
    {
        // Calculate new Z
        z = dvec2(zx_sq - zy_sq, 2 * z.x * z.y) + c;
        zx_sq = z.x * z.x;
        zy_sq = z.y * z.y;

        // // Check for perodicity
        // if (dot(z - oldZ, z - oldZ) <= 1e-8)
        // {
        //     break;
        // }
        // curr_period++;
        // if (curr_period >= max_period)
        // {
        //     curr_period = 0;
        //     oldZ = z;
        // }

        // Check out of bounds
        float d = float(zx_sq + zy_sq);
        if (d >= bound)
        {
            num_iter = i;
            dist = d;
            break;
        }
    }
    float n = float(num_iter + 1) - log(log(dist) / 2)/log(2); // smooth colouring
    n = fract(colorScale * n);

    // Colour for whole image
    fragColor = texture(tex, n);

    // Colour for if pixel is in fractal
    if (num_iter < 0)
    {
        fragColor = vec4(0,0,0,1);
    }
}

