#include "Matrix4.h"
#include <cmath>
#include <iomanip>

using namespace std;

/** Default constructor */
Matrix4::Matrix4()
{
}

/** Copy constructor */
Matrix4::Matrix4(const Matrix4 &M)
{
    for(unsigned int row = 0; row < 4; row++)
    {
        for(unsigned int col = 0; col < 4; col++)
        {
            entries[row][col] = M[row][col];
        }
    }
}

/** Instantiate a matrix given 3 initial vectors (the 4th column will be set to 0,0,0,1)
 * @param vn the (n+1)th column of the matrix */
Matrix4::Matrix4(const Vector3D &v0, const Vector3D &v1, const Vector3D &v2)
{
    entries[0][0] = v0[0];
    entries[0][1] = v0[1];
    entries[0][2] = v0[2];
    entries[0][3] = 0.0f;

    entries[1][0] = v1[0];
    entries[1][1] = v1[1];
    entries[1][2] = v1[2];
    entries[1][3] = 0.0f;

    entries[2][0] = v2[0];
    entries[2][1] = v2[1];
    entries[2][2] = v2[2];
    entries[2][3] = 0.0f;

    entries[3][0] = 0.0f;
    entries[3][1] = 0.0f;
    entries[3][2] = 0.0f;
    entries[3][3] = 1.0f;
}

/** Return the inverse of this matrix */
Matrix4 Matrix4::Inverse() const
{
    //
    // Inversion by Cramer's rule.  Code taken from an Intel publication
    //
    double result[4][4];
    double tmp[12]; /* temp array for pairs */
    double src[16]; /* array of transpose source matrix */
    double det; /* determinant */
    /* transpose matrix */
    for (unsigned int i = 0; i < 4; i++)
    {
        src[i + 0 ] = (*this)[i][0];
        src[i + 4 ] = (*this)[i][1];
        src[i + 8 ] = (*this)[i][2];
        src[i + 12] = (*this)[i][3];
    }
    /* calculate pairs for first 8 elements (cofactors) */
    tmp[0] = src[10] * src[15];
    tmp[1] = src[11] * src[14];
    tmp[2] = src[9] * src[15];
    tmp[3] = src[11] * src[13];
    tmp[4] = src[9] * src[14];
    tmp[5] = src[10] * src[13];
    tmp[6] = src[8] * src[15];
    tmp[7] = src[11] * src[12];
    tmp[8] = src[8] * src[14];
    tmp[9] = src[10] * src[12];
    tmp[10] = src[8] * src[13];
    tmp[11] = src[9] * src[12];
    /* calculate first 8 elements (cofactors) */
    result[0][0] = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
    result[0][0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
    result[0][1] = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
    result[0][1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
    result[0][2] = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
    result[0][2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
    result[0][3] = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
    result[0][3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
    result[1][0] = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
    result[1][0] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
    result[1][1] = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
    result[1][1] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
    result[1][2] = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
    result[1][2] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
    result[1][3] = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
    result[1][3] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];
    /* calculate pairs for second 8 elements (cofactors) */
    tmp[0] = src[2]*src[7];
    tmp[1] = src[3]*src[6];
    tmp[2] = src[1]*src[7];
    tmp[3] = src[3]*src[5];
    tmp[4] = src[1]*src[6];
    tmp[5] = src[2]*src[5];

    tmp[6] = src[0]*src[7];
    tmp[7] = src[3]*src[4];
    tmp[8] = src[0]*src[6];
    tmp[9] = src[2]*src[4];
    tmp[10] = src[0]*src[5];
    tmp[11] = src[1]*src[4];

    /* calculate second 8 elements (cofactors) */
    result[2][0] = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
    result[2][0] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
    result[2][1] = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
    result[2][1] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
    result[2][2] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
    result[2][2] -= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
    result[2][3] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
    result[2][3] -= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
    result[3][0] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
    result[3][0] -= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
    result[3][1] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
    result[3][1] -= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
    result[3][2] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
    result[3][2] -= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
    result[3][3] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
    result[3][3] -= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];
    /* calculate determinant */
    det=src[0]*result[0][0]+src[1]*result[0][1]+src[2]*result[0][2]+src[3]*result[0][3];
    /* calculate matrix inverse */
    det = 1.0f / det;

    Matrix4 floatResult;
    for (unsigned int i = 0; i < 4; i++)
    {
        for (unsigned int j = 0; j < 4; j++)
        {
            floatResult[i][j] = float(result[i][j] * det);
        }
    }
    return floatResult;
}

/** Return the transpose of this matrix */
Matrix4 Matrix4::Transpose() const
{
    Matrix4 result;
    for(unsigned int i = 0; i < 4; i++)
    {
        for(unsigned int i2 = 0; i2 < 4; i2++)
        {
            result[i2][i] = entries[i][i2];
        }
    }
    return result;
}

/** Returns the matrix identity */
Matrix4 Matrix4::Identity()
{
    Matrix4 result;
    for(unsigned int i = 0; i < 4; i++)
    {
        for(unsigned int i2 = 0; i2 < 4; i2++)
        {
            if(i == i2)
            {
                result[i][i2] = 1.0f;
            }
            else
            {
                result[i][i2] = 0.0f;
            }
        }
    }
    return result;
}

/*
Matrix4 Matrix4::Rotation(const Vector3D &Axis, float Angle)
{
    float c = cos(Angle);
    float s = sin(Angle);
    float t = 1.0f - c;

    Vector3D NormalizedAxis = Axis;
    NormalizedAxis.normalize();

    float x = NormalizedAxis[0];
    float y = NormalizedAxis[1];
    float z = NormalizedAxis[2];

    Matrix4 Result;
    Result[0][0] = 1 + t*(x*x-1);
    Result[0][1] = z*s+t*x*y;
    Result[0][2] = -y*s+t*x*z;
    Result[0][3] = 0.0f;

    Result[1][0] = -z*s+t*x*y;
    Result[1][1] = 1+t*(y*y-1);
    Result[1][2] = x*s+t*y*z;
    Result[1][3] = 0.0f;

    Result[2][0] = y*s+t*x*z;
    Result[2][1] = -x*s+t*y*z;
    Result[2][2] = 1+t*(z*z-1);
    Result[2][3] = 0.0f;

    Result[3][0] = 0.0f;
    Result[3][1] = 0.0f;
    Result[3][2] = 0.0f;
    Result[3][3] = 1.0f;
    return Result;
}*/

/** Rotate matrix (angles are in radians) */
Matrix4 Matrix4::Rotation(float yaw, float pitch, float roll)
{
    return RotationY(yaw) * RotationX(pitch) * RotationZ(roll);
}

/*
Matrix4 Matrix4::Rotation(const Vector3D &Axis, float Angle, const Vector3D &Center)
{
    return Translation(-Center) * Rotation(Axis, Angle) * Translation(Center);
}*/

/** Rotate about the X axis (angle in radians) */
Matrix4 Matrix4::RotationX(float theta)
{
    float cosT = cos(theta);
    float sinT = sin(theta);

    Matrix4 result = Identity();
    result[1][1] = cosT;
    result[1][2] = sinT;
    result[2][1] = -sinT;
    result[2][2] = cosT;
    return result;
}

/** Rotate about the Y axis (angle in radians) */
Matrix4 Matrix4::RotationY(float theta)
{
    float cosT = cos(theta);
    float sinT = sin(theta);

    Matrix4 result = Identity();
    result[0][0] = cosT;
    result[0][2] = sinT;
    result[2][0] = -sinT;
    result[2][2] = cosT;
    return result;
}

/** Rotate about the Z axis (angle in radians) */
Matrix4 Matrix4::RotationZ(float theta)
{
    float cosT = cos(theta);
    float sinT = sin(theta);

    Matrix4 result = Identity();
    result[0][0] = cosT;
    result[0][1] = sinT;
    result[1][0] = -sinT;
    result[1][1] = cosT;
    return result;
}

float Matrix4::Determinant() const
{
    //
    // Inversion by Cramer's rule.  Code taken from an Intel publication
    //
    double result[4][4];
    double tmp[12]; /* temp array for pairs */
    double src[16]; /* array of transpose source matrix */
    double det; /* determinant */
    /* transpose matrix */
    for (unsigned int i = 0; i < 4; i++)
    {
        src[i + 0 ] = (*this)[i][0];
        src[i + 4 ] = (*this)[i][1];
        src[i + 8 ] = (*this)[i][2];
        src[i + 12] = (*this)[i][3];
    }
    /* calculate pairs for first 8 elements (cofactors) */
    tmp[0] = src[10] * src[15];
    tmp[1] = src[11] * src[14];
    tmp[2] = src[9] * src[15];
    tmp[3] = src[11] * src[13];
    tmp[4] = src[9] * src[14];
    tmp[5] = src[10] * src[13];
    tmp[6] = src[8] * src[15];
    tmp[7] = src[11] * src[12];
    tmp[8] = src[8] * src[14];
    tmp[9] = src[10] * src[12];
    tmp[10] = src[8] * src[13];
    tmp[11] = src[9] * src[12];
    /* calculate first 8 elements (cofactors) */
    result[0][0] = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
    result[0][0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
    result[0][1] = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
    result[0][1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
    result[0][2] = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
    result[0][2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
    result[0][3] = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
    result[0][3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
    result[1][0] = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
    result[1][0] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
    result[1][1] = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
    result[1][1] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
    result[1][2] = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
    result[1][2] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
    result[1][3] = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
    result[1][3] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];
    /* calculate pairs for second 8 elements (cofactors) */
    tmp[0] = src[2]*src[7];
    tmp[1] = src[3]*src[6];
    tmp[2] = src[1]*src[7];
    tmp[3] = src[3]*src[5];
    tmp[4] = src[1]*src[6];
    tmp[5] = src[2]*src[5];

    tmp[6] = src[0]*src[7];
    tmp[7] = src[3]*src[4];
    tmp[8] = src[0]*src[6];
    tmp[9] = src[2]*src[4];
    tmp[10] = src[0]*src[5];
    tmp[11] = src[1]*src[4];
    /* calculate second 8 elements (cofactors) */
    result[2][0] = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
    result[2][0] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
    result[2][1] = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
    result[2][1] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
    result[2][2] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
    result[2][2] -= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
    result[2][3] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
    result[2][3] -= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
    result[3][0] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
    result[3][0] -= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
    result[3][1] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
    result[3][1] -= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
    result[3][2] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
    result[3][2] -= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
    result[3][3] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
    result[3][3] -= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];
    /* calculate determinant */
    det=src[0]*result[0][0]+src[1]*result[0][1]+src[2]*result[0][2]+src[3]*result[0][3];
    return det;
}

Vector3D Matrix4::multiply(const Vector3D & vector, float w) const
{
    return Vector3D(vector[0] * entries[0][0] + vector[1] * entries[1][0] + vector[2] * entries[2][0] + w * entries[3][0],
					vector[0] * entries[0][1] + vector[1] * entries[1][1] + vector[2] * entries[2][1] + w * entries[3][1],
					vector[0] * entries[0][2] + vector[1] * entries[1][2] + vector[2] * entries[2][2] + w * entries[3][2]);
}

/** Scale the matrix by the given factors (x,y,z) */
Matrix4 Matrix4::Scale(const Vector3D &scaleFactors)
{
    Matrix4 result;
    result[0][0] = scaleFactors[0];
    result[1][0] = 0.0f;
    result[2][0] = 0.0f;
    result[3][0] = 0.0f;

    result[0][1] = 0.0f;
    result[1][1] = scaleFactors[1];
    result[2][1] = 0.0f;
    result[3][1] = 0.0f;

    result[0][2] = 0.0f;
    result[1][2] = 0.0f;
    result[2][2] = scaleFactors[2];
    result[3][2] = 0.0f;

    result[0][3] = 0.0f;
    result[1][3] = 0.0f;
    result[2][3] = 0.0f;
    result[3][3] = 1.0f;
    return result;
}

/** Get a translated matrix */
Matrix4 Matrix4::Translation(const Vector3D &pos)
{
    Matrix4 result;
    result[0][0] = 1.0f;
    result[1][0] = 0.0f;
    result[2][0] = 0.0f;
    result[3][0] = pos[0];

    result[0][1] = 0.0f;
    result[1][1] = 1.0f;
    result[2][1] = 0.0f;
    result[3][1] = pos[1];

    result[0][2] = 0.0f;
    result[1][2] = 0.0f;
    result[2][2] = 1.0f;
    result[3][2] = pos[2];

    result[0][3] = 0.0f;
    result[1][3] = 0.0f;
    result[2][3] = 0.0f;
    result[3][3] = 1.0f;
    return result;
}

/** @return normal multiplied by the current matrix */
Vector3D Matrix4::TransformNormal(const Vector3D &normal) const
{
    return Vector3D(normal[0] * entries[0][0] + normal[1] * entries[1][0] + normal[2] * entries[2][0],
                 normal[0] * entries[0][1] + normal[1] * entries[1][1] + normal[2] * entries[2][1],
                 normal[0] * entries[0][2] + normal[1] * entries[1][2] + normal[2] * entries[2][2]);
}

ostream& operator << (ostream &os, const Matrix4 &m)
{
    os << setprecision(8) << setiosflags(ios::right | ios::showpoint);
    for(unsigned int i=0; i<4; i++)
    {
        for(unsigned int j=0; j<4; j++)
        {
            os << setw(12) << m[j][i];
        }
        os << endl;
    }
    return os;
}

istream& operator >> (istream &is, Matrix4 &m)
{
    for(unsigned int i=0;i<4;i++)
    {
        for(unsigned int j=0; j<4; j++)
        {
            is >> m[i][j];
        }
    }
    return is;
}

Matrix4& Matrix4::operator = (const Matrix4 &M)
{
    for(unsigned int row = 0; row < 4; row++)
    {
        for(unsigned int col = 0; col < 4; col++)
        {
            entries[row][col] = M[row][col];
        }
    }
    return (*this);
}

Matrix4 operator * (const Matrix4 &left, const Matrix4 &right)
{
    Matrix4 result;
    for(unsigned int i = 0; i < 4; i++)
    {
        for(unsigned int j = 0; j < 4; j++)
        {
            float sum = 0.0f;
            for(unsigned int i3 = 0; i3 < 4; i3++)
            {
                sum += left[i][i3] * right[i3][j];
            }
            result[i][j] = sum;
        }
    }
    return result;
}

Matrix4 operator * (const Matrix4 &left, float &right)
{
    Matrix4 result;
    for(unsigned int i = 0; i < 4; i++)
    {
        for(unsigned int j = 0; j < 4; j++)
        {
            result[i][j] = left[i][j] * right;
        }
    }
    return result;
}

Matrix4 operator * (float &left, const Matrix4 &right)
{
    Matrix4 result;
    for(unsigned int i = 0; i < 4; i++)
    {
        for(unsigned int j = 0; j < 4; j++)
        {
            result[i][j] = right[i][j] * left;
        }
    }
    return result;
}

Matrix4 operator + (const Matrix4 &left, const Matrix4 &right)
{
    Matrix4 result;
    for(unsigned int i = 0; i < 4; i++)
    {
        for(unsigned int j = 0; j < 4; j++)
        {
            result[i][j] = left[i][j] + right[i][j];
        }
    }
    return result;
}

Matrix4 operator - (const Matrix4 &left, const Matrix4 &right)
{
    Matrix4 result;
    for(unsigned int i = 0; i < 4; i++)
    {
        for(unsigned int j = 0; j < 4; j++)
        {
            result[i][j] = left[i][j] - right[i][j];
        }
    }
    return result;
}
