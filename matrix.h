/***************************************************************************/
/* WARNING: DO NOT CHANGE. This file will be overwritten by Buildbot       */
/***************************************************************************/

/* Matrix helper for left-handed coordinate system */
#ifndef MATRIX_H
#define MATRIX_H

#define ALIGN16 __declspec(align(16))

typedef struct vec3 {
    float x;
    float y;
    float z;
} vec3_t;

ALIGN16 typedef struct vec4 {
    float x;
    float y;
    float z;
    float w;
} vec4_t;

ALIGN16 typedef struct mat4 {
    vec4_t row0;
    vec4_t row1;
    vec4_t row2;
    vec4_t row3;
} mat4_t;

void mat_scale(mat4_t* mat, float x, float y, float z);
void mat_rotation_x(mat4_t* mat, float degree);
void mat_rotation_y(mat4_t* mat, float degree);
void mat_rotation_z(mat4_t* mat, float degree);
void mat_translation(mat4_t* mat, float x, float y, float z);

#endif /* MATRIX_H */