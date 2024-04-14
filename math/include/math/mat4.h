
#ifndef __MATH_MAT4_H
#define __MATH_MAT4_H

#ifdef __cplusplus
extern "C" {
#endif

void mat4_mulmm(float *dst, const float *a, const float *b);
void mat4_mulmv4(float *dst, const float *m, const float *v);
void mat4_mulmv3_normalize(float *dst, const float *m, const float *v);
float mat4_mulmxv3_normalize(const float *m, const float *v);
float mat4_mulmyv3_normalize(const float *m, const float *v);

void mat4_setIdentity(float *dst);
void mat4_setOrtho(float *dst, float right, float left, float top, float bottom, float near, float far);
void mat4_setFrustum(float *dst, float right, float left, float top, float bottom, float near, float far);

void mat4_setRotateV(float *dst, const float *v, float rad);
void mat4_setRotateAtV(float *dst, const float *axis, const float *p, float rad);
void mat4_setScaleV(float *dst, const float *scale);
void mat4_setTranslateV(float *dst, const float *v);
void mat4_postTranslateV(float *dst, const float *src, const float *v);
void mat4_translateV(float *dst, const float *src, const float *v);
void mat4_rotateV(float *dst, const float *src, const float *v, float rad);
void mat4_scaleV(float *dst, const float *src, const float *scale);

void mat4_setRotateA(float *dst, float x, float y, float z, float rad);
void mat4_setRotateAtA(float *dst, float ax, float ay, float az, float Tx, float Ty, float Tz, float rad);
void mat4_setScaleA(float *dst, float sx, float sy, float sz);
void mat4_setTranslateA(float *dst, float tx, float ty, float tz);
void mat4_postTranslateA(float *dst, const float *src, float x, float y, float z);
void mat4_translateA(float *dst, const float *src, float x, float y, float z);
void mat4_rotateA(float *dst, const float *src, float ax, float ay, float az, float rad);
void mat4_scaleA(float *dst, const float *src, float sx, float sy, float sz);

void mat4_transpose(float *dst, const float *src);
float mat4_determinant(const float *M);
void mat4_invert(float *dst, const float *src);

#ifdef __cplusplus
}
#endif

#endif /* __MATH_MAT4_H */