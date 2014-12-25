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

#include "myopenglwindow.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

const char vertexShader[] =
"attribute vec3 vertex;\n"
"attribute vec3 normal;\n"
"\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(vertex, 1.0);\n"
"}\n";

const char fragmentShader[] =
"//precision mediump float;\n"
"\n"
"void main()\n"
"{\n"
"   gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);\n"
"}\n";

MyOpenGLWindow::MyOpenGLWindow()
    : m_fluid(10, 10, 0.1, 0.1, 1.0, 0.0),
      m_vbo(0)
{
}

MyOpenGLWindow::~MyOpenGLWindow()
{

}

void MyOpenGLWindow::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, 10 * 10 * sizeof(Vertex), NULL, GL_STREAM_DRAW);

    //shader
    m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader);
    m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader);
    m_program.link();
    m_program.bind();

    m_positionAttributeLocation =  m_program.attributeLocation("vertex");
    m_normalAttributeLocation = m_program.attributeLocation("normal");

    glEnableVertexAttribArray(m_positionAttributeLocation);
    glEnableVertexAttribArray(m_normalAttributeLocation);
}

void MyOpenGLWindow::paintGL()
{
    glViewport(0, 0, width(), height());

    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    // ### TODO: fill buffer
    glVertexAttribPointer(m_positionAttributeLocation,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          0);

    glVertexAttribPointer(m_normalAttributeLocation,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          BUFFER_OFFSET(sizeof(TVec3)));

    for (int i = 0; i < 10; ++i) {
        // ### TODO: glDrawArrays(GL_TRIANGLE_STRIP, 0, 10);
    }
}

void MyOpenGLWindow::resizeGL(int w, int h)
{
}

