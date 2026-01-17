#include "transformer.h"

int main(void)
{
    enum { MAX_POINTS = 512 };

    vec4_t local_points[MAX_POINTS];
    vec4_t world_points[MAX_POINTS];
    size_t count;

    const vec3_t scale = { 1.f, 2.f, 1.f };
    const vec3_t rotation = { 60.f, -30.f, 15.f };
    const vec3_t translation = { 2.f, 0.f, 3.f };

    count = read_points(local_points, MAX_POINTS);


    run(world_points, local_points, count, &scale, &rotation, &translation);

    print_csv(world_points, count);

    return 0;
}