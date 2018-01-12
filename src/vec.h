

#ifndef VEC_HEADER
#define VEC_HEADER

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define CLAMP(A, B, C) (MAX(MIN(A, C), B))

typedef struct Vec2f Vec2f;
struct Vec2f {
    float x;
    float y;
};

void vec_set(Vec2f *vec, float x, float y);
Vec2f vec_add(Vec2f v1, Vec2f v2);
Vec2f vec_adds(Vec2f vec, float v);
Vec2f vec_sub(Vec2f v1, Vec2f v2);
Vec2f vec_subs(Vec2f vec, float v);
Vec2f vec_mul(Vec2f v1, Vec2f v2);
Vec2f vec_muls(Vec2f vec, float v);
Vec2f vec_div(Vec2f v1, Vec2f v2);
Vec2f vec_divs(Vec2f vec, float v);
Vec2f vec_clamp(Vec2f vec, float max);
int vec_eql(Vec2f v1, Vec2f v2);
float vec_dot(Vec2f v1, Vec2f v2);
float vec_mag(Vec2f vec);
Vec2f vec_set_mag(Vec2f vec, float v);
float vec_angle(Vec2f v1, Vec2f v2);
Vec2f vec_norm(Vec2f vec);
Vec2f vec_copy(Vec2f vec);
Vec2f vec_neg(Vec2f vec);
Vec2f vec_trns(Vec2f vec, float tx, float ty, float rotate, float scale);

#endif