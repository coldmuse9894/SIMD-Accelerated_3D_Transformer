/***************************************************************************/
/* WARNING: DO NOT CHANGE. This file will be overwritten by Buildbot       */
/***************************************************************************/
#include "matrix.h"

#include <assert.h>
#include <math.h>
#include <string.h>

#define RADIAN(degree) ((degree) * 3.14159265358979323846f / 180.0f)

void mat_scale(mat4_t* mat, float x, float y, float z)
{
    assert(sizeof(mat4_t) == 64);
    assert(sizeof(vec4_t) == 16);

    memset(mat, 0, sizeof(mat4_t));

    mat->row0.x = x;
    mat->row1.y = y;
    mat->row2.z = z;
    mat->row3.w = 1.0f;
}

void mat_rotation_x(mat4_t* mat, float degree)
{
    const float rad = RADIAN(degree);
    const float sine = (float)sin(rad);
    const float cosine = (float)cos(rad);

    memset(mat, 0, sizeof(mat4_t));

    mat->row0.x = 1.0f;

    mat->row1.y = cosine;
    mat->row1.z = sine;

    mat->row2.y = -sine;
    mat->row2.z = cosine;

    mat->row3.w = 1.0f;
}

void mat_rotation_y(mat4_t* mat, float degree)
{
    const float rad = RADIAN(degree);
    const float sine = (float)sin(rad);
    const float cosine = (float)cos(rad);

    memset(mat, 0, sizeof(mat4_t));

    mat->row0.x = cosine;
    mat->row0.z = -sine;

    mat->row1.y = 1.0f;

    mat->row2.x = sine;
    mat->row2.z = cosine;

    mat->row3.w = 1.0f;
}

void mat_rotation_z(mat4_t* mat, float degree)
{
    const float rad = RADIAN(degree);
    const float sine = (float)sin(rad);
    const float cosine = (float)cos(rad);

    memset(mat, 0, sizeof(mat4_t));

    mat->row0.x = cosine;
    mat->row0.y = sine;

    mat->row1.x = -sine;
    mat->row1.y = cosine;

    mat->row2.z = 1.0f;

    mat->row3.w = 1.0f;
}

void mat_translation(mat4_t* mat, float x, float y, float z)
{
    memset(mat, 0, sizeof(mat4_t));

    mat->row0.x = 1.0f;
    mat->row1.y = 1.0f;
    mat->row2.z = 1.0f;

    mat->row3.x = x;
    mat->row3.y = y;
    mat->row3.z = z;
    mat->row3.w = 1.0f;
}