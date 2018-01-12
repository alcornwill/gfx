
#include "vec.h"


void vec_set(Vec2f *vec, float x, float y) {
    vec->x = x;
    vec->y = y;
}

Vec2f vec_add(Vec2f v1, Vec2f v2) {
    // adds the values of v2 to v1
    return (Vec2f){v1.x + v2.x, v1.y + v2.y};
}

Vec2f vec_adds(Vec2f vec, float v) {
    // add scalar
    return (Vec2f){vec.x + v, vec.y + v};
}

Vec2f vec_sub(Vec2f v1, Vec2f v2) {
    // subtracts the values of v2 from v1
    return (Vec2f){v1.x - v2.x, v1.y - v2.y};
}

Vec2f vec_subs(Vec2f vec, float v) {
    // subtract scalar
    return (Vec2f){vec.x - v, vec.y - v};
}

Vec2f vec_mul(Vec2f v1, Vec2f v2) {
    // multiplies the values of v1 by v2
    return (Vec2f){v1.x * v2.x, v1.y * v2.y};
}

Vec2f vec_muls(Vec2f vec, float v) {
    // multiply scalar
    return (Vec2f){vec.x * v, vec.y * v};
}

Vec2f vec_div(Vec2f v1, Vec2f v2) {
    // divides the values of v1 by v2
    return (Vec2f){v1.x / v2.x, v1.y / v2.y};
}

Vec2f vec_divs(Vec2f vec, float v) {
    // divide scalar
    return (Vec2f){vec.x / v, vec.y / v};
}

Vec2f vec_clamp(Vec2f vec, float max) {
    float mag = vec_mag(vec);
    if (mag > max) {
        return vec_set_mag(vec, max);
    } else {
        return vec;
    }
}

int vec_eql(Vec2f v1, Vec2f v2) {
    // todo shit because float
    return v1.x == v2.x && v1.y == v2.y;
}

float vec_dot(Vec2f v1, Vec2f v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

float vec_mag(Vec2f vec) {
    return sqrt(vec_dot(vec, vec));
}

Vec2f vec_set_mag(Vec2f vec, float v) {
    return vec_muls(vec_norm(vec), v);
}

float vec_angle(Vec2f v1, Vec2f v2) {
    if (vec_eql(v1, v2)) {
        return 0;
    }
   
    float dot = vec_dot(v1, v2);
    float m1 = vec_mag(v1);
    float m2 = vec_mag(v2);
    float amt = dot / (m1 * m2);
    if (amt <= -1) {
        return M_PI;
    } else if (amt >= 1) {
        return 0;
    }
    return acos(amt);
}

Vec2f vec_norm(Vec2f vec) {
    float m = vec_mag(vec);
    if (m > 0) {
        return vec_divs(vec, m);
    } else {
        return (Vec2f){0, 0};
    }
}

Vec2f vec_copy(Vec2f vec) {
    return (Vec2f){vec.x, vec.y};
}

Vec2f vec_neg(Vec2f vec) {
    return (Vec2f){-vec.x, -vec.y};
}

Vec2f vec_trns(Vec2f vec, float tx, float ty, float rotate, float scale) {
    vec.x *= scale;
    vec.y *= scale;
    
    float x = vec.x;
    float y = vec.y;
    float sn = sin(rotate);
    float cs = cos(rotate);
    vec.x = x * cs - y * sn;
    vec.y = x * sn + y * cs;
    
    vec.x += tx;
    vec.y += ty;
    
    return vec;
}
