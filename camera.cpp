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

#include "camera.h"

Camera::Camera()
{
    TMat3 matRot(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    m_rotQuat.fromMatrix(matRot);

    m_eyePos.x = 0.0f;
    m_eyePos.y = 0.0f;
    m_eyePos.z = 0.0f;

    m_viewMatrix.set_rot(m_rotQuat);
    m_viewMatrix.set_translation(m_eyePos);
    m_viewMatrix.invert_rot_trans();
}

/*
 * construct camera with lookat
 *
 * eyeVec3 - position of camera
 * atVec3 - where is camera looking
 * upVec3 - up vector of camera
 */
Camera::Camera(float eyeVec3[], float atVec3[], float upVec3[])
{
    m_eyePos.x = eyeVec3[0];
    m_eyePos.y = eyeVec3[1];
    m_eyePos.z = eyeVec3[2];

    TVec3 forward;
    TVec3 at(atVec3);

    forward = at - m_eyePos;
    forward.normalize();

    TVec3 up(upVec3);
    up.normalize();

    TVec3 side;
    cross(side, up, forward);
    side.normalize();
    cross(up, forward, side);

    TMat3 rotMatrix;
    rotMatrix.set_row(0, side);
    rotMatrix.set_row(1, up);
    rotMatrix.set_row(2, forward);

    m_rotQuat.fromMatrix(rotMatrix);

    m_viewMatrix.set_rot(m_rotQuat);
    m_viewMatrix.set_translation(m_eyePos);
    m_viewMatrix.invert_rot_trans();
}

/*
 * construct camera with lookat
 *
 * eyex, eyey, eyez - position of camera
 * atx, aty, atz - looking position
 * upx, upy, upz - up vector of camera
 */

Camera::Camera(float eyex, float eyey, float eyez,
               float atx, float aty, float atz,
               float upx, float upy, float upz)
{
    m_eyePos.x = eyex;
    m_eyePos.y = eyey;
    m_eyePos.z = eyez;

    TVec3 forward(atx - eyex, aty - eyey, atz - eyez);
    forward.normalize();

    TVec3 up(upx, upy, upz);
    up.normalize();

    TVec3 side;
    cross(side, up, forward);
    side.normalize();
    cross(up, forward, side);

    TMat3 rotMatrix;
    rotMatrix.set_row(0, side);
    rotMatrix.set_row(1, up);
    rotMatrix.set_row(2, forward);

    m_rotQuat.fromMatrix(rotMatrix);

    m_viewMatrix.set_rot(m_rotQuat);
    m_viewMatrix.set_translation(m_eyePos);
    m_viewMatrix.invert_rot_trans();
}

Camera::~Camera()
{
}

/*
 * Sets camera
 *
 * eyeVec3 - position of camera
 * atVec3 - looking position
 * upVec3 - up vector of camera
 */

void Camera::lookat(float eyeVec3[], float atVec3[], float upVec3[])
{
    m_eyePos.x = eyeVec3[0];
    m_eyePos.y = eyeVec3[1];
    m_eyePos.z = eyeVec3[2];

    TVec3 forward;
    TVec3 at(atVec3);

    forward = at - m_eyePos;

    forward.normalize();

    TVec3 up(upVec3);
    up.normalize();

    TVec3 side;
    cross(side, up, forward);
    side.normalize();
    cross(up, forward, side);

    TMat3 rotMatrix;
    rotMatrix.set_row(0, side);
    rotMatrix.set_row(1, up);
    rotMatrix.set_row(2, forward);

    m_rotQuat.fromMatrix(rotMatrix);

    m_viewMatrix.set_rot(m_rotQuat);
    m_viewMatrix.set_translation(m_eyePos);
    m_viewMatrix.invert_rot_trans();
}

void Camera::moveForward(float f)
{
    m_eyePos.x += f * m_viewMatrix.a02;
    m_eyePos.y += f * m_viewMatrix.a12;
    m_eyePos.z += f * m_viewMatrix.a22;

    //just update the translation
    m_viewMatrix.a30 = -(m_viewMatrix.a00 * m_eyePos.x + m_viewMatrix.a10 * m_eyePos.y + m_viewMatrix.a20 * m_eyePos.z);
    m_viewMatrix.a31 = -(m_viewMatrix.a01 * m_eyePos.x + m_viewMatrix.a11 * m_eyePos.y + m_viewMatrix.a21 * m_eyePos.z);
    m_viewMatrix.a32 = -(m_viewMatrix.a02 * m_eyePos.x + m_viewMatrix.a12 * m_eyePos.y + m_viewMatrix.a22 * m_eyePos.z);
}

void Camera::moveSide(float s)
{
    m_eyePos.x += s * m_viewMatrix.a00;
    m_eyePos.y += s * m_viewMatrix.a10;
    m_eyePos.z += s * m_viewMatrix.a20;

    m_viewMatrix.a30 = -(m_viewMatrix.a00 * m_eyePos.x + m_viewMatrix.a10 * m_eyePos.y + m_viewMatrix.a20 * m_eyePos.z);
    m_viewMatrix.a31 = -(m_viewMatrix.a01 * m_eyePos.x + m_viewMatrix.a11 * m_eyePos.y + m_viewMatrix.a21 * m_eyePos.z);
    m_viewMatrix.a32 = -(m_viewMatrix.a02 * m_eyePos.x + m_viewMatrix.a12 * m_eyePos.y + m_viewMatrix.a22 * m_eyePos.z);
}

void Camera::moveUp(float u)
{
    m_eyePos.x += u * m_viewMatrix.a01;
    m_eyePos.y += u * m_viewMatrix.a11;
    m_eyePos.z += u * m_viewMatrix.a21;

    m_viewMatrix.a30 = -(m_viewMatrix.a00 * m_eyePos.x + m_viewMatrix.a10 * m_eyePos.y + m_viewMatrix.a20 * m_eyePos.z);
    m_viewMatrix.a31 = -(m_viewMatrix.a01 * m_eyePos.x + m_viewMatrix.a11 * m_eyePos.y + m_viewMatrix.a21 * m_eyePos.z);
    m_viewMatrix.a32 = -(m_viewMatrix.a02 * m_eyePos.x + m_viewMatrix.a12 * m_eyePos.y + m_viewMatrix.a22 * m_eyePos.z);
}

void Camera::rotateX(float angle)
{
    // Fix it
    TVec3 up(m_viewMatrix.a01, m_viewMatrix.a11, m_viewMatrix.a21);
    TQuat q(up, angle);

    m_rotQuat *= q;

    m_viewMatrix.set_rot(m_rotQuat);
    m_viewMatrix.set_translation(m_eyePos);
    m_viewMatrix.invert_rot_trans();
}

void Camera::rotateXX(float angle)
{
    TVec3 xx(1.0f, 0.0f, 0.0f);
    TQuat q(xx, angle);

    m_rotQuat *= q;

    m_viewMatrix.set_rot(m_rotQuat);
    m_viewMatrix.set_translation(m_eyePos);
    m_viewMatrix.invert_rot_trans();
}

void Camera::rotateY(float angle)
{
    //FIX IT
    TVec3 side(m_viewMatrix.a00, m_viewMatrix.a10, m_viewMatrix.a20);
    TQuat q(side, angle);

    m_rotQuat *= q;
    m_viewMatrix.set_rot(m_rotQuat);
    m_viewMatrix.set_translation(m_eyePos);
    m_viewMatrix.invert_rot_trans();
}

void Camera::rotateYY(float angle)
{
    TVec3 yy(0.0f, 1.0f, 0.0f);
    TQuat q(yy, angle);

    m_rotQuat *= q;

    m_viewMatrix.set_rot(m_rotQuat);
    m_viewMatrix.set_translation(m_eyePos);
    m_viewMatrix.invert_rot_trans();
}

float* Camera::viewMatrix()
{
    return m_viewMatrix.mat_array;
}

TVec3 Camera::eyePos()
{
    return m_eyePos;
}

void Camera::setPerspectiveViewFOV(float fovy, float aspect, float zn, float zf)
{
    float yScale = 1.0f / tan(fovy / 2.0f);
    float xScale = yScale / aspect;
    float zz = zf / (zf - zn);
    float ww = -zn * zf / (zf - zn);

    //m_wnearplane = 2.0f * zn * tan(fovy / 2.0f);
    //m_hnearplane = m_wnearplane / aspect;
    m_hnearplane = 2.0f * zn * tan(fovy/ 2.0f);
    m_wnearplane = m_hnearplane * aspect;
    m_znear = zn;
    m_zfar = zf;

    m_projMatrix.a00 = xScale;	m_projMatrix.a01 = 0.0f;	m_projMatrix.a02 = 0.0f;	m_projMatrix.a03 = 0.0f;
    m_projMatrix.a10 = 0.0f;	m_projMatrix.a11 = yScale;	m_projMatrix.a12 = 0.0f;	m_projMatrix.a13 = 0.0f;
    m_projMatrix.a20 = 0.0f;	m_projMatrix.a21 = 0.0f;	m_projMatrix.a22 = zz;		m_projMatrix.a23 = 1.0f;
    m_projMatrix.a30 = 0.0f;	m_projMatrix.a31 = 0.0f;	m_projMatrix.a32 = ww;		m_projMatrix.a33 = 0.0f;
}

void Camera::setPerspectiveViewWH(float w, float h, float zn, float zf)
{
    float xScale = 2.0f * zn / w;
    float yScale = 2.0f * zn / h;
    float zz = zf / (zf - zn);
    float ww = zn * zf / (zn - zf);

    m_wnearplane = w;
    m_hnearplane = h;
    m_znear = zn;
    m_zfar = zf;

    m_projMatrix.a00 = xScale;	m_projMatrix.a01 = 0.0f;	m_projMatrix.a02 = 0.0f;	m_projMatrix.a03 = 0.0f;
    m_projMatrix.a10 = 0.0f;	m_projMatrix.a11 = yScale;	m_projMatrix.a12 = 0.0f;	m_projMatrix.a13 = 0.0f;
    m_projMatrix.a20 = 0.0f;	m_projMatrix.a21 = 0.0f;	m_projMatrix.a22 = zz;		m_projMatrix.a23 = 1.0f;
    m_projMatrix.a30 = 0.0f;	m_projMatrix.a31 = 0.0f;	m_projMatrix.a32 = ww;		m_projMatrix.a33 = 0.0f;
}

float* Camera::projectionMatrix()
{
    return m_projMatrix.mat_array;
}

float Camera::widthNearPlane() const
{
    return m_wnearplane;
}

float Camera::heightNearPlane() const
{
    return m_hnearplane;
}

float Camera::zNearPlane() const
{
    return m_znear;
}

float Camera::zFarPlane() const
{
    return m_zfar;
}

TVec3 Camera::directionVector() const
{
    return TVec3(m_viewMatrix.a02, m_viewMatrix.a12, m_viewMatrix.a22);
}

TVec3 Camera::upVector() const
{
    return TVec3(m_viewMatrix.a01, m_viewMatrix.a11, m_viewMatrix.a21);
}

TVec3 Camera::rightVector() const
{
    return TVec3(m_viewMatrix.a00, m_viewMatrix.a10, m_viewMatrix.a20);
}
