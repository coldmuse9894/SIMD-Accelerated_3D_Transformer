#include "transformer.h"

#include <stddef.h>
#include <stdio.h>

#define TRUE (1)

size_t read_points(vec4_t* points, const size_t count)
{
    vec4_t* points_ptr = points;
    size_t points_read;

    for (points_read = 0; points_read < count; ++points_read) {
        if (scanf("%f %f %f", &(points_ptr->x), &(points_ptr->y), &(points_ptr->z)) == EOF) {
            break;
        }

        points_ptr->w = 1.f;

        ++points_ptr;
    }
 
    return points_read;
}

void print_csv(const vec4_t* points, const size_t count)
{
    size_t i;
    for (i = 0; i < count; ++i) {
        printf("%.4f, %.4f, %.4f\n", points[i].x, points[i].y, points[i].z);
    }
}

void transpose(mat4_t* mat)
{
    __asm {
        mov eax, mat

        movaps xmm0, [eax]
        movaps xmm1, [eax+4*4]
        movaps xmm2, [eax+4*8]
        movaps xmm3, [eax+4*12]

        movaps xmm4, xmm0
        shufps xmm4, xmm1, 01000100b

        movaps xmm5, xmm2
        shufps xmm5, xmm3, 01000100b

        movaps xmm6, xmm0
        shufps xmm6, xmm1, 11101110b

        movaps xmm7, xmm2
        shufps xmm7, xmm3, 11101110b


        movaps xmm0, xmm4
        shufps xmm0, xmm5, 10001000b

        movaps xmm1, xmm4
        shufps xmm1, xmm5, 11011101b

        movaps xmm2, xmm6
        shufps xmm2, xmm7, 10001000b

        movaps xmm3, xmm6
        shufps xmm3, xmm7, 11011101b

        movaps [eax], xmm0
        movaps [eax+4*4], xmm1
        movaps [eax+4*8], xmm2
        movaps [eax+4*12], xmm3

    }
}

void transform(vec4_t* dst, const vec4_t* src, const mat4_t* mat_tr)
{
    __asm {
        mov eax, mat_tr
        mov ebx, src

        movaps xmm0, [ebx]
        movaps xmm1, xmm0
        movaps xmm2, xmm0
        movaps xmm3, xmm0

        movaps xmm4, [eax]
        movaps xmm5, [eax+4*4]
        movaps xmm6, [eax+4*8]
        movaps xmm7, [eax+4*12]

        dpps xmm0, xmm4, 11110001b
        dpps xmm1, xmm5, 11110010b
        dpps xmm2, xmm6, 11110100b
        dpps xmm3, xmm7, 11111000b

        addps xmm0, xmm1
        addps xmm2, xmm3
        addps xmm0, xmm2

        mov eax, dst
        movaps [eax], xmm0
    }
}

void concatenate(mat4_t* dst, const mat4_t* m0, const mat4_t* m1_tr)
{
    __asm {
        mov ebx, dst
        mov eax, m1_tr
        movaps xmm0, [eax]
        movaps xmm1, [eax+4*4]
        movaps xmm2, [eax+4*8]
        movaps xmm3, [eax+4*12]

        mov eax, m0
        movaps xmm4, [eax]

        movaps xmm5, xmm4
        dpps xmm5, xmm0, 11110001b

        movaps xmm6, xmm4
        dpps xmm6, xmm1, 11110010b

        addps xmm5, xmm6

        movaps xmm6, xmm4
        dpps xmm6, xmm2, 11110100b

        dpps xmm4, xmm3, 11111000b

        addps xmm4, xmm6
        addps xmm4, xmm5

        movaps [ebx], xmm4

        ; dot product row1
        movaps xmm4, [eax+4*4]

        movaps xmm5, xmm4
        dpps xmm5, xmm0, 11110001b

        movaps xmm6, xmm4
        dpps xmm6, xmm1, 11110010b

        addps xmm5, xmm6

        movaps xmm6, xmm4
        dpps xmm6, xmm2, 11110100b

        dpps xmm4, xmm3, 11111000b

        addps xmm4, xmm6
        addps xmm4, xmm5

        movaps [ebx+4*4], xmm4

        ; dot product row2
        movaps xmm4, [eax+4*8]

        movaps xmm5, xmm4
        dpps xmm5, xmm0, 11110001b

        movaps xmm6, xmm4
        dpps xmm6, xmm1, 11110010b

        addps xmm5, xmm6

        movaps xmm6, xmm4
        dpps xmm6, xmm2, 11110100b

        dpps xmm4, xmm3, 11111000b

        addps xmm4, xmm6
        addps xmm4, xmm5

        movaps [ebx+4*8], xmm4

        ; dot product row3
        movaps xmm4, [eax+4*12]

        movaps xmm5, xmm4
        dpps xmm5, xmm0, 11110001b

        movaps xmm6, xmm4
        dpps xmm6, xmm1, 11110010b

        addps xmm5, xmm6

        movaps xmm6, xmm4
        dpps xmm6, xmm2, 11110100b

        dpps xmm4, xmm3, 11111000b

        addps xmm4, xmm6
        addps xmm4, xmm5

        movaps [ebx+4*12], xmm4
    }
}

void run(vec4_t* world_pts, const vec4_t* local_pts, const size_t count, const vec3_t* scale, const vec3_t* rotation, const vec3_t* translation)
{
    mat4_t concat_mat;
    mat4_t tmp_mat1;
    mat4_t tmp_mat2;
    size_t i;

    mat_scale(&tmp_mat1, scale->x, scale->y, scale->z);
    mat_rotation_x(&tmp_mat2, rotation->x);
    transpose(&tmp_mat2);
    concatenate(&concat_mat, &tmp_mat1, &tmp_mat2);

    mat_rotation_y(&tmp_mat2, rotation->y);
    transpose(&tmp_mat2);
    concatenate(&concat_mat, &concat_mat, &tmp_mat2);

    mat_rotation_z(&tmp_mat2, rotation->z);
    transpose(&tmp_mat2);
    concatenate(&concat_mat, &concat_mat, &tmp_mat2);

    mat_translation(&tmp_mat2, translation->x, translation->y, translation->z);
    transpose(&tmp_mat2);
    concatenate(&concat_mat, &concat_mat, &tmp_mat2);

    transpose(&concat_mat);
    for (i = 0; i < count; ++i) {
        transform(world_pts + i, local_pts + i, &concat_mat);
    }

}