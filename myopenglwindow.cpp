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
#include <QScopedArrayPointer>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

const char vertexShader[] =
"//#version 150\n"
"\n"
"attribute vec3 vertex;\n"
"attribute vec3 normal;\n"
"varying vec3 normalOut;\n"
"\n"
"uniform mat4 worldMatrix;\n"
"uniform mat4 projectionMatrix;\n"
"\n"
"void main()\n"
"{\n"
"   gl_Position = projectionMatrix * worldMatrix * vec4(vertex, 1.0);\n"
"   normalOut = (worldMatrix * vec4(normal, 0.0)).xyz;\n"
"}\n";

const char fragmentShader[] =
"//#version 150\n"
"varying vec3 normalOut;\n"
"//out vec4 fragColor;\n"
"\n"
"uniform vec3 lightDirection;\n"
"//precision mediump float;\n"
"\n"
"void main()\n"
"{\n"
"   float diffuseFactor = max(dot(normalize(normalOut), -lightDirection), 0.0);\n"
"   gl_FragColor = diffuseFactor * vec4(1.0, 1.0, 1.0, 1.0);\n"
"}\n";

MyOpenGLWindow::MyOpenGLWindow()
    : m_fluid(10, 10, 0.1f, 0.005f, 1.0f, 0.5f),
      m_vbo(0),
      m_projectionMatrixPosition(-1)
{
    //initial camera setup
    TVec3 at(0.5f, 0.5f, 0.0f);
    TVec3 eye(0.5f, -2.0f, -1.0f);
    TVec3 up(0.0f, 1.0f, 0.0f);

    m_camera.lookat(eye.vec_array, at.vec_array, up.vec_array);
    m_camera.setPerspectiveViewFOV(float(3.14 * 0.25), float(width()) / float(height()), 0.1f, 100.0f);

    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
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

    //total indices = (totalX * 2 + 2) * (totalZ - 1)

    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

    QScopedArrayPointer<GLshort> indexes(new GLshort[(10 * 2 + 2) * (10 - 1)]);

    int indexOffset = 0;
    for (int zindex = 0; zindex < 10 - 1; zindex++) {

        GLshort index = 0;

        for (int xindex = 0; xindex < 10; xindex++) {
            index = xindex + ((zindex + 1) * 10); //numX
            indexes[indexOffset] = index;
            indexOffset++;

            index = xindex + (zindex * 10);   //numX
            indexes[indexOffset] = index;
            indexOffset++;
        }

        //copy of previous one
        indexes[indexOffset] = index;
        indexOffset++;

        //place in first one for next row
        index = 0 + ((zindex + 1) * 10);     //numX
        indexes[indexOffset] = index;
        indexOffset++;
    }

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort) * (10 * 2 + 2) * (10 - 1), indexes.data(), GL_STATIC_DRAW);

    //shader
    m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader);
    m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader);
    m_program.link();
    m_program.bind();

    m_positionAttributeLocation =  m_program.attributeLocation("vertex");
    m_normalAttributeLocation = m_program.attributeLocation("normal");

    glEnableVertexAttribArray(m_positionAttributeLocation);
    glEnableVertexAttribArray(m_normalAttributeLocation);

    //uniforms setup
    m_worldMatrixPosition = m_program.uniformLocation("worldMatrix");
    m_projectionMatrixPosition = m_program.uniformLocation("projectionMatrix");

    int lightDirectionPosition = m_program.uniformLocation("lightDirection");
    glUniform3f(lightDirectionPosition, 0.0f, 0.0f, -1.0f);

    //initial matrix setup
    glUniformMatrix4fv(m_worldMatrixPosition, 1, GL_FALSE, m_camera.viewMatrix());
    glUniformMatrix4fv(m_projectionMatrixPosition, 1, GL_FALSE, m_camera.projectionMatrix());
}

void MyOpenGLWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 m_fluid.width() * m_fluid.height() * sizeof(Vertex),
                 m_fluid.bufferData(),
                 GL_STREAM_DRAW);

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

    glDrawElements(GL_TRIANGLE_STRIP, (10 * 2 + 2) * (10 - 1), GL_UNSIGNED_SHORT, 0);

    m_fluid.evaluate();
}

void MyOpenGLWindow::resizeGL(int w, int h)
{
    if (m_projectionMatrixPosition != -1) {
        m_camera.setPerspectiveViewFOV(float(3.14 * 0.25), float(w) / float(h), 0.1f, 100.0f);
        glUniformMatrix4fv(m_projectionMatrixPosition, 1, GL_FALSE, m_camera.projectionMatrix());

        glViewport(0, 0, w, h);
    }
}

