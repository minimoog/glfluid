/*
* Copyright (c) 2014 Antonie Jovanoski <minimoog77_at_gmail.com>
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

#ifndef MYOPENGLWINDOW_H
#define MYOPENGLWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include "fluid.h"

class MyOpenGLWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
public:
    MyOpenGLWindow();
    ~MyOpenGLWindow();

    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;

private:
    Fluid m_fluid;
    GLuint m_vbo;
    GLuint m_ibo;
    //GLuint m_vertexShader;
    //GLuint m_fragmentShader;
    QOpenGLShaderProgram m_program;
    int m_positionAttributeLocation;
    int m_normalAttributeLocation;
};

#endif // MYOPENGLWINDOW_H
