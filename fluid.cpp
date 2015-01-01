/*
* Copyright (c) 2008 Antonie Jovanoski <minimoog77_at_gmail.com>
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

#include "fluid.h"

Fluid::Fluid(int n, int m, float distance, float timeStep, float velocity, float viscosity)
    :	m_width(n),
      m_height(m),
      renderBuffer(0),
      d(distance),
      t(timeStep),
      c(velocity),
      mu(viscosity)
{
    calculateCoef();

    buffer[0].setSize(n, m);
    buffer[1].setSize(n, m);

    for(int j = 0; j < m; j++){
        for(int i = 0; i < n; i++){
            buffer[0](i, j).position.x = d * i;
            buffer[0](i, j).position.y = d * j;
            buffer[0](i, j).position.z = 0.0f;

            buffer[1](i, j).position.x = buffer[0](i, j).position.x;
            buffer[1](i, j).position.y = buffer[0](i, j).position.y;
            buffer[1](i, j).position.z = buffer[0](i, j).position.z;

            buffer[0](i, j).normal.x = 0.0f;
            buffer[0](i, j).normal.y = 0.0f;
            buffer[0](i, j).normal.z = 2.0f * d;

            buffer[1](i, j).normal.x = buffer[0](i, j).normal.x;
            buffer[1](i, j).normal.y = buffer[0](i, j).normal.y;
            buffer[1](i, j).normal.z = buffer[0](i, j).normal.z;
        }
    }

    buffer[0](1, 1).position.z = 1.0f;
    //buffer[1](5, 5).position.z = 1.0f;
}

Fluid::~Fluid()
{
}

void Fluid::calculateCoef()
{
    float f1 = c * c * t * t / (d * d);
    float f2 = 1.0f / (mu * t + 2);
    k1 = (4.0f - 8.0f * f1) * f2;
    k2 = (mu * t - 2) * f2;
    k3 = 2.0f * f1 * f2;
}

void Fluid::evaluate(void)
{
    for(int j = 1; j < m_height - 1; j++){
        for(int i = 1; i < m_width - 1; i++){
            buffer[1 - renderBuffer](i, j).position.z = k1 * buffer[renderBuffer](i, j).position.z +
                    k2 * buffer[1 - renderBuffer](i, j).position.z +
                    k3 * (buffer[renderBuffer](i + 1, j).position.z +
                    buffer[renderBuffer](i - 1, j).position.z +
                    buffer[renderBuffer](i, j + 1).position.z +
                    buffer[renderBuffer](i, j - 1).position.z);
        }
    }

    renderBuffer = 1 - renderBuffer;

    for(int j = 1; j < m_height - 1; j++){
        for(int i = 1; i < m_width - 1; i++){
            buffer[renderBuffer](i, j).normal.x = (buffer[renderBuffer](i - 1, j).position.z - buffer[renderBuffer](i + 1, j).position.z) / d;
            buffer[renderBuffer](i, j).normal.y = (buffer[renderBuffer](i, j - 1).position.z - buffer[renderBuffer](i, j + 1).position.z) / d;
            buffer[renderBuffer](i, j).normal.z = 2.0f;
            buffer[renderBuffer](i, j).normal.normalize();

            buffer[1 - renderBuffer](i, j).normal.x = buffer[renderBuffer](i, j).normal.x;
            buffer[1 - renderBuffer](i, j).normal.y = buffer[renderBuffer](i, j).normal.y;
            buffer[1 - renderBuffer](i, j).normal.z = buffer[renderBuffer](i, j).normal.z;
        }
    }
}

float Fluid::distance() const
{
    return d;
}

void Fluid::setDistance(float distance)
{
    d = distance;
    calculateCoef();
}

void Fluid::setTimeStep(float timeStep)
{
    t = timeStep;
    calculateCoef();
}

void Fluid::setVelocity(float velocity)
{
    //fali proverka na stabilnost

    c = velocity;
    calculateCoef();
}

void Fluid::setViscosity(float viscosity)
{
    mu = viscosity;
    calculateCoef();
}

const Vertex *Fluid::bufferData() const
{
    return buffer[renderBuffer].pointer();
}
