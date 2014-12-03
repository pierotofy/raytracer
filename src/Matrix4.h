#ifndef MATRIX4_H
#define MATRIX4_H

#include <iostream>
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include <string>

using namespace std;
using namespace sivelab;


class Matrix4
{
public:
    Matrix4();
    Matrix4(const Matrix4 &M);
    Matrix4(const Vector3D &V0, const Vector3D &V1, const Vector3D &V2);

    Matrix4& operator = (const Matrix4 &M);

    float Determinant() const;
    Matrix4 Transpose() const;
    Matrix4 Inverse() const;
    Vector3D TransformNormal(const Vector3D &normal) const;

    Vector3D multiply(const Vector3D &vector, float w) const;

    float* operator [] (int Row)
    {
        return entries[Row];
    }

    const float* operator [] (int Row) const
    {
        return entries[Row];
    }

    static Matrix4 Identity();
    static Matrix4 Scale(const Vector3D &scaleFactors);
    static Matrix4 Scale(float scaleFactor)
    {
        return Scale(Vector3D(scaleFactor, scaleFactor, scaleFactor));
    }
    static Matrix4 Translation(const Vector3D &Pos);
    //static Matrix4 Rotation(const Vector3D &Axis, float Angle, const Vector3D &Center);
    //static Matrix4 Rotation(const Vector3D &Axis, float Angle);
    static Matrix4 Rotation(float yaw, float pitch, float roll);
    static Matrix4 RotationX(float theta);
    static Matrix4 RotationY(float theta);
    static Matrix4 RotationZ(float theta);

private:
    float entries[4][4];
};

ostream& operator << (ostream &os, const Matrix4 &m);
istream& operator >> (istream &os, Matrix4 &m);

Matrix4 operator * (const Matrix4 &left, const Matrix4 &right);
Matrix4 operator * (const Matrix4 &left, float &right);
Matrix4 operator * (float &Left, const Matrix4 &right);
Matrix4 operator + (const Matrix4 &left, const Matrix4 &right);
Matrix4 operator - (const Matrix4 &left, const Matrix4 &right);


#endif
