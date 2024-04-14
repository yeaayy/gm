
#include <math.h>

void mat4_mulmm(float *dst, const float *a, const float *b){
    register float axx = a[ 0], axy = a[ 1], axz = a[ 2], axw = a[ 3];
    register float ayx = a[ 4], ayy = a[ 5], ayz = a[ 6], ayw = a[ 7];
    register float azx = a[ 8], azy = a[ 9], azz = a[10], azw = a[11];
    register float awx = a[12], awy = a[13], awz = a[14], aww = a[15];

    register float bxx = b[ 0], bxy = b[ 1], bxz = b[ 2], bxw = b[ 3];
    register float byx = b[ 4], byy = b[ 5], byz = b[ 6], byw = b[ 7];
    register float bzx = b[ 8], bzy = b[ 9], bzz = b[10], bzw = b[11];
    register float bwx = b[12], bwy = b[13], bwz = b[14], bww = b[15];

    dst[ 0] = axx * bxx + ayx * bxy + azx * bxz + awx * bxw;
    dst[ 1] = axy * bxx + ayy * bxy + azy * bxz + awy * bxw;
    dst[ 2] = axz * bxx + ayz * bxy + azz * bxz + awz * bxw;
    dst[ 3] = axw * bxx + ayw * bxy + azw * bxz + aww * bxw;
    dst[ 4] = axx * byx + ayx * byy + azx * byz + awx * byw;
    dst[ 5] = axy * byx + ayy * byy + azy * byz + awy * byw;
    dst[ 6] = axz * byx + ayz * byy + azz * byz + awz * byw;
    dst[ 7] = axw * byx + ayw * byy + azw * byz + aww * byw;
    dst[ 8] = axx * bzx + ayx * bzy + azx * bzz + awx * bzw;
    dst[ 9] = axy * bzx + ayy * bzy + azy * bzz + awy * bzw;
    dst[10] = axz * bzx + ayz * bzy + azz * bzz + awz * bzw;
    dst[11] = axw * bzx + ayw * bzy + azw * bzz + aww * bzw;
    dst[12] = axx * bwx + ayx * bwy + azx * bwz + awx * bww;
    dst[13] = axy * bwx + ayy * bwy + azy * bwz + awy * bww;
    dst[14] = axz * bwx + ayz * bwy + azz * bwz + awz * bww;
    dst[15] = axw * bwx + ayw * bwy + azw * bwz + aww * bww;
}

void mat4_mulmv4(float *dst, const float *m, const float *v){
    register float x = v[0], y = v[1], z = v[2], w = v[3];
    dst[ 0] = m[ 0] * x + m[ 4] * y + m[ 8] * z + m[12] * w;
    dst[ 1] = m[ 1] * x + m[ 5] * y + m[ 9] * z + m[13] * w;
    dst[ 2] = m[ 2] * x + m[ 6] * y + m[10] * z + m[14] * w;
    dst[ 3] = m[ 3] * x + m[ 7] * y + m[11] * z + m[15] * w;
}

void mat4_mulmv3_normalize(float *dst, const float *m, const float *v){
    register float x = v[0], y = v[1], z = v[2];
    register float iw = 1 / ( m[ 3] * x + m[ 7] * y + m[11] * z + m[15] ) ;
    dst[ 0] = ( m[ 0] * x + m[ 4] * y + m[ 8] * z + m[12] ) * iw;
    dst[ 1] = ( m[ 1] * x + m[ 5] * y + m[ 9] * z + m[13] ) * iw;
    dst[ 2] = ( m[ 2] * x + m[ 6] * y + m[10] * z + m[14] ) * iw;
}

float mat4_mulmxv3_normalize(const float *m, const float *v){
    register float x = v[0], y = v[1], z = v[2];
    return ( m[ 0] * x + m[ 4] * y + m[ 8] * z + m[12] ) / ( m[ 3] * x + m[ 7] * y + m[11] * z + m[15] );
}

float mat4_mulmyv3_normalize(const float *m, const float *v){
    register float x = v[0], y = v[1], z = v[2];
    return ( m[ 1] * x + m[ 5] * y + m[ 9] * z + m[13] ) / ( m[ 3] * x + m[ 7] * y + m[11] * z + m[15] );
}

void mat4_setIdentity(float *dst){
    dst[ 0] = 1;
    dst[ 1] = 0;
    dst[ 2] = 0;
    dst[ 3] = 0;
    dst[ 4] = 0;
    dst[ 5] = 1;
    dst[ 6] = 0;
    dst[ 7] = 0;
    dst[ 8] = 0;
    dst[ 9] = 0;
    dst[10] = 1;
    dst[11] = 0;
    dst[12] = 0;
    dst[13] = 0;
    dst[14] = 0;
    dst[15] = 1;
}

void mat4_setOrtho(float *dst, float right, float left, float top, float bottom, float near, float far){
    register float lr = right - left;
    register float tb = top - bottom;
    register float nf = near - far;

    dst[ 0] = 2 / lr;
    dst[ 1] = 0;
    dst[ 2] = 0;
    dst[ 3] = 0;
    dst[ 4] = 0;
    dst[ 5] = 2 / tb;
    dst[ 6] = 0;
    dst[ 7] = 0;
    dst[ 8] = 0;
    dst[ 9] = 0;
    dst[10] = 2 / nf;
    dst[11] = 0;
    dst[12] = ( left + right ) / lr;
    dst[13] = ( top + bottom ) / tb;
    dst[14] = ( near + far ) / nf;
    dst[15] = 1;
}

void mat4_setFrustum(float *dst, float right, float left, float top, float bottom, float near, float far){
    register float lr = right - left;
    register float tb = top - bottom;
    register float nf = near - far;

    dst[ 0] = near / lr;
    dst[ 1] = 0;
    dst[ 2] = 0;
    dst[ 3] = 0;
    dst[ 4] = 0;
    dst[ 5] = near / tb;
    dst[ 6] = 0;
    dst[ 7] = 0;
    dst[ 8] = 0;
    dst[ 9] = 0;
    dst[10] = near / nf;
    dst[11] = (far-near)/nf;
    dst[12] = ( left + right ) / lr;
    dst[13] = ( top + bottom ) / tb;
    dst[14] = ( near + far ) / nf;
    dst[15] = 0;
}

void mat4_setRotateA(float *dst, float x, float y, float z, float rad){
    register float r = sqrtf(x * x + y * y + z * z);
    if(fabsf(r-1)>0.0001){
        x /= r;
        y /= r;
        z /= r;
    }

    register float s = sinf(rad), c = cosf(rad);
    register float t = 1 - c;
    dst[ 0] = x*x*t + c;
    dst[ 1] = x*y*t + z*s;
    dst[ 2] = x*z*t - y*s;

    dst[ 4] = y*x*t - z*s;
    dst[ 5] = y*y*t + c;
    dst[ 6] = y*z*t + x*s;

    dst[ 8] = z*x*t + y*s;
    dst[ 9] = z*y*t - x*s;
    dst[10] = z*z*t + c;

    dst[ 3] = 0;
    dst[ 7] = 0;
    dst[11] = 0;

    dst[12] = 0;
    dst[13] = 0;
    dst[14] = 0;
    dst[15] = 1;
}

void mat4_setRotateAtA(float *dst, float ax, float ay, float az, float Tx, float Ty, float Tz, float rad){
    register float r = sqrtf(ax * ax + ay * ay + az * az);
    if(fabsf(r-1)>0.0001){
        ax /= r;
        ay /= r;
        az /= r;
    }

    register float s = sinf(rad), c = cosf(rad);
    register float t = 1 - c;
    register float A, B, C, D, E, F, G, H, I;
    dst[ 0] = A = ax*ax*t + c;
    dst[ 1] = D = ax*ay*t + az*s;
    dst[ 2] = G = ax*az*t - ay*s;

    dst[ 4] = B = ay*ax*t - az*s;
    dst[ 5] = E = ay*ay*t + c;
    dst[ 6] = H = ay*az*t + ax*s;

    dst[ 8] = C = az*ax*t + ay*s;
    dst[ 9] = F = az*ay*t - ax*s;
    dst[10] = I = az*az*t + c;

    dst[ 3] = 0;
    dst[ 7] = 0;
    dst[11] = 0;

    dst[12] = Tx - A*Tx - B*Ty - C*Tz;
    dst[13] = Ty - D*Tx - E*Ty - F*Tz;
    dst[14] = Tz - G*Tx - H*Ty - I*Tz;
    dst[15] = 1;
}

void mat4_setScaleA(float *dst, float sx, float sy, float sz){
    dst[ 0] = sx;
    dst[ 1] = 0;
    dst[ 2] = 0;
    dst[ 3] = 0;
    dst[ 4] = 0;
    dst[ 5] = sy;
    dst[ 6] = 0;
    dst[ 7] = 0;
    dst[ 8] = 0;
    dst[ 9] = 0;
    dst[10] = sz;
    dst[11] = 0;
    dst[12] = 0;
    dst[13] = 0;
    dst[14] = 0;
    dst[15] = 1;
}

void mat4_setTranslateA(float *dst, float tx, float ty, float tz){
    dst[ 0] = 1;
    dst[ 1] = 0;
    dst[ 2] = 0;
    dst[ 3] = 0;
    dst[ 4] = 0;
    dst[ 5] = 1;
    dst[ 6] = 0;
    dst[ 7] = 0;
    dst[ 8] = 0;
    dst[ 9] = 0;
    dst[10] = 1;
    dst[11] = 0;
    dst[12] = tx;
    dst[13] = ty;
    dst[14] = tz;
    dst[15] = 1;
}

void mat4_postTranslateA(float *dst, const float *src, float x, float y, float z){
    register float axx = src[ 0], axy = src[ 1], axz = src[ 2], axw = src[ 3];
    register float ayx = src[ 4], ayy = src[ 5], ayz = src[ 6], ayw = src[ 7];
    register float azx = src[ 8], azy = src[ 9], azz = src[10], azw = src[11];
    register float awx = src[12], awy = src[13], awz = src[14], aww = src[15];

    dst[12] = axx * x + ayx * y + azx * z + awx;
    dst[13] = axy * x + ayy * y + azy * z + awy;
    dst[14] = axz * x + ayz * y + azz * z + awz;
    dst[15] = axw * x + ayw * y + azw * z + aww;
    if(src!=dst){
        dst[ 0] = axx;
        dst[ 1] = axy;
        dst[ 2] = axz;
        dst[ 3] = axw;
        dst[ 4] = ayx;
        dst[ 5] = ayy;
        dst[ 6] = ayz;
        dst[ 7] = ayw;
        dst[ 8] = azx;
        dst[ 9] = azy;
        dst[10] = azz;
        dst[11] = azw;
    }
}

void mat4_translateA(float *dst, const float *src, float x, float y, float z){
    register float bxx = src[ 0], bxy = src[ 1], bxz = src[ 2], bxw = src[ 3];
    register float byx = src[ 4], byy = src[ 5], byz = src[ 6], byw = src[ 7];
    register float bzx = src[ 8], bzy = src[ 9], bzz = src[10], bzw = src[11];
    register float bwx = src[12], bwy = src[13], bwz = src[14], bww = src[15];

    dst[ 0] = bxx + x * bxw;
    dst[ 1] = bxy + y * bxw;
    dst[ 2] = bxz + z * bxw;
    dst[ 4] = byx + x * byw;
    dst[ 5] = byy + y * byw;
    dst[ 6] = byz + z * byw;
    dst[ 8] = bzx + x * bzw;
    dst[ 9] = bzy + y * bzw;
    dst[10] = bzz + z * bzw;
    dst[12] = bwx + x * bww;
    dst[13] = bwy + y * bww;
    dst[14] = bwz + z * bww;
    if(src != dst) {
        dst[ 3] = src[ 3];
        dst[ 7] = src[ 7];
        dst[11] = src[11];
        dst[15] = src[15];
    }
}

void mat4_rotateA(float *dst, const float *src, float x, float y, float z, float rad){
    register float r = sqrtf(x*x + y*y + z*z);
    if(fabsf(r-1)>0.0001){
        x /= r;
        y /= r;
        z /= r;
    }

    register float s = sinf(rad), c = cosf(rad);
    register float t = 1 - c;

    register float axx = x*x*t + c, axy = x*y*t + z*s, axz = x*z*t - y*s;
    register float ayx = y*x*t - z*s, ayy = y*y*t + c, ayz = y*z*t + x*s;
    register float azx = z*x*t + y*s, azy = z*y*t - x*s, azz = z*z*t + c;

    register float bxx = src[ 0], bxy = src[ 1], bxz = src[ 2];
    register float byx = src[ 4], byy = src[ 5], byz = src[ 6];
    register float bzx = src[ 8], bzy = src[ 9], bzz = src[10];
    register float bwx = src[12], bwy = src[13], bwz = src[14];

    dst[ 0] = axx * bxx + ayx * bxy + azx * bxz;
    dst[ 1] = axy * bxx + ayy * bxy + azy * bxz;
    dst[ 2] = axz * bxx + ayz * bxy + azz * bxz;
    dst[ 4] = axx * byx + ayx * byy + azx * byz;
    dst[ 5] = axy * byx + ayy * byy + azy * byz;
    dst[ 6] = axz * byx + ayz * byy + azz * byz;
    dst[ 8] = axx * bzx + ayx * bzy + azx * bzz;
    dst[ 9] = axy * bzx + ayy * bzy + azy * bzz;
    dst[10] = axz * bzx + ayz * bzy + azz * bzz;
    dst[12] = axx * bwx + ayx * bwy + azx * bwz;
    dst[13] = axy * bwx + ayy * bwy + azy * bwz;
    dst[14] = axz * bwx + ayz * bwy + azz * bwz;

    if(src!=dst){
        dst[ 3] = src[ 3];
        dst[ 7] = src[ 7];
        dst[11] = src[11];
        dst[15] = src[15];
    }
}

void mat4_scaleA(float *dst, const float *src, float sx, float sy, float sz){
    dst[ 0] = src[ 0] * sx;
    dst[ 1] = src[ 1] * sy;
    dst[ 2] = src[ 2] * sz;
    dst[ 4] = src[ 4] * sx;
    dst[ 5] = src[ 5] * sy;
    dst[ 6] = src[ 6] * sy;
    dst[ 8] = src[ 8] * sz;
    dst[ 9] = src[ 9] * sx;
    dst[10] = src[10] * sz;
    dst[12] = src[12] * sx;
    dst[13] = src[13] * sy;
    dst[14] = src[14] * sz;
    if(src!=dst){
        dst[ 3] = src[ 3];
        dst[ 7] = src[ 7];
        dst[11] = src[11];
        dst[15] = src[15];
    }
}

void mat4_setRotateV(float *dst, const float *v, float rad){
    mat4_setRotateA(dst, v[0], v[1], v[2], rad);
}

void mat4_setRotateAtV(float *dst, const float *axis, const float *p, float rad){
    mat4_setRotateAtA(dst, axis[0], axis[1], axis[2], p[0], p[1], p[2], rad);
}

void mat4_setScaleV(float *dst, const float *scale){
    mat4_setScaleA(dst, scale[0], scale[1], scale[2]);
}

void mat4_setTranslateV(float *dst, const float *v){
    mat4_setTranslateA(dst, v[0], v[1], v[2]);
}

void mat4_postTranslateV(float *dst, const float *src, const float *v){
    mat4_postTranslateA(dst, src, v[0], v[1], v[2]);
}

void mat4_translateV(float *dst, const float *src, const float *v){
    mat4_translateA(dst, src, v[0], v[1], v[2]);
}

void mat4_rotateV(float *dst, const float *src, const float *v, float rad){
    mat4_rotateA(dst, src, v[0], v[1], v[2], rad);
}

void mat4_scaleV(float *dst, const float *src, const float *scale){
    mat4_scaleA(dst, src, scale[0], scale[1], scale[2]);
}

void mat4_transpose(float *dst, const float *src){
    register float axy = src[ 1], axz = src[ 2], axw = src[ 3];
    register float ayx = src[ 4], ayz = src[ 6], ayw = src[ 7];
    register float azx = src[ 8], azy = src[ 9], azw = src[11];
    register float awx = src[12], awy = src[13], awz = src[14];
    dst[ 1] = ayx;
    dst[ 2] = azx;
    dst[ 3] = awx;
    dst[ 4] = axy;
    dst[ 6] = azy;
    dst[ 7] = awy;
    dst[ 8] = axz;
    dst[ 9] = ayz;
    dst[11] = awz;
    dst[12] = axw;
    dst[13] = ayw;
    dst[14] = azw;
    if(src!=dst){
        dst[ 0] = src[ 0];
        dst[ 5] = src[ 5];
        dst[10] = src[10];
        dst[15] = src[15];
    }
}

float mat4_determinant(const float *M){
    register float a = M[ 0], b = M[ 1], c = M[ 2], d = M[ 3];
    register float e = M[ 4], f = M[ 5], g = M[ 6], h = M[ 7];
    register float i = M[ 8], j = M[ 9], k = M[10], l = M[11];
    register float m = M[12], n = M[13], o = M[14], p = M[15];
    register float A00 = a*f - b*e;
    register float A01 = k*p - l*o;
    register float A02 = c*h - d*g;
    register float A03 = i*n - j*m;
    register float A04 = a*g - c*e;
    register float A05 = l*n - j*p;
    register float A06 = b*h - d*f;
    register float A07 = k*m - i*o;
    register float A08 = a*h - d*e;
    register float A09 = j*o - k*n;
    register float A10 = b*g - c*f;
    register float A11 = i*p - l*m;
    return A00*A01 + A02*A03 + A04*A05 + A06*A07 + A08*A09 + A10*A11;
}

void mat4_invert(float *dst, const float *src){
    register float a = src[ 0], b = src[ 1], c = src[ 2], d = src[ 3];
    register float e = src[ 4], f = src[ 5], g = src[ 6], h = src[ 7];
    register float i = src[ 8], j = src[ 9], k = src[10], l = src[11];
    register float m = src[12], n = src[13], o = src[14], p = src[15];
    register float A00 = a*f - b*e;
    register float A01 = k*p - l*o;
    register float A02 = c*h - d*g;
    register float A03 = i*n - j*m;
    register float A04 = a*g - c*e;
    register float A05 = l*n - j*p;
    register float A06 = b*h - d*f;
    register float A07 = k*m - i*o;
    register float A08 = a*h - d*e;
    register float A09 = j*o - k*n;
    register float A10 = b*g - c*f;
    register float A11 = i*p - l*m;
    register float det = 1/(A00*A01 + A02*A03 + A04*A05 + A06*A07 + A08*A09 + A10*A11);
    dst[ 0] = ( + h*A09 + g*A05 + f*A01 ) * det;
    dst[ 1] = ( - d*A09 - c*A05 - b*A01 ) * det;
    dst[ 2] = ( - o*A06 + n*A02 + p*A10 ) * det;
    dst[ 3] = ( + k*A06 - j*A02 - l*A10 ) * det;
    dst[ 4] = ( + h*A07 + g*A11 - e*A01 ) * det;
    dst[ 5] = ( - d*A07 - c*A11 + a*A01 ) * det;
    dst[ 6] = ( + o*A08 - p*A04 - m*A02 ) * det;
    dst[ 7] = ( - k*A08 + l*A04 + i*A02 ) * det;
    dst[ 8] = ( - f*A11 - e*A05 + h*A03 ) * det;
    dst[ 9] = ( + b*A11 + a*A05 - d*A03 ) * det;
    dst[10] = ( - n*A08 + m*A06 + p*A00 ) * det;
    dst[11] = ( + j*A08 - i*A06 - l*A00 ) * det;
    dst[12] = ( - f*A07 - e*A09 - g*A03 ) * det;
    dst[13] = ( + b*A07 + a*A09 + c*A03 ) * det;
    dst[14] = ( + n*A04 - m*A10 - o*A00 ) * det;
    dst[15] = ( - j*A04 + i*A10 + k*A00 ) * det;
}