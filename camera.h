/*
* Copyright (c) 2012 Antonie Jovanoski <minimoog77_at_gmail.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#ifndef CAMERA_H
#define CAMERA_H

#include "math/tmat4.h"
#include "math/tvec4.h"

class Camera
{
public:
    Camera();
    Camera(float eyeVec3[], float atVec3[], float upVec3[]);
    Camera(float eyex, float eyey, float eyez,
           float atx, float aty, float atz,
           float upx, float upy, float upz);
    ~Camera();

    void lookat(float eyeVec3[], float atVec3[], float upVec3[]);

    void moveForward(float f);
    void moveSide(float s);
    void moveUp(float u);
    void rotateX(float angle);
    void rotateXX(float angle);
    void rotateY(float angle);
    void rotateYY(float angle);

    void setPerspectiveViewFOV(float fovy, float aspect, float zn, float zf);
    void setPerspectiveViewWH(float w, float h, float zn, float zf);

    TVec3  eyePos();
    float* viewMatrix();
    float* projectionMatrix();
    float  widthNearPlane() const;
    float  heightNearPlane() const;
    float  zNearPlane() const;
    float  zFarPlane() const;
    TVec3  directionVector() const;
    TVec3  upVector() const;
    TVec3  rightVector() const;

private:
    TQuat m_rotQuat;
    TMat4 m_viewMatrix;
    TMat4 m_projMatrix;
    TVec3 m_eyePos;
    float m_wnearplane;
    float m_hnearplane;
    float m_znear;
    float m_zfar;
};

#endif // CAMERA_H
