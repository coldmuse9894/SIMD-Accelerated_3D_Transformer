/***************************************************************************/
/* WARNING: DO NOT CHANGE. This file will be overwritten by Buildbot       */
/***************************************************************************/

#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include <stddef.h>
#include "matrix.h"

size_t read_points(vec4_t* points, const size_t count);
void print_csv(const vec4_t* points, const size_t count);

void transpose(mat4_t* mat);
void transform(vec4_t* dst, const vec4_t* src, const mat4_t* mat_tr);
void concatenate(mat4_t* dst, const mat4_t* m0, const mat4_t* m1_tr);

void run(vec4_t* world_pts, const vec4_t* local_pts, const size_t count, const vec3_t* scale, const vec3_t* rotation, const vec3_t* translation);

#endif /* TRANSFORMER_H */