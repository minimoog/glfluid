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
    :	width(n),
      height(m),
      buffer0(n, m),
      buffer1(n, m),
      renderBuffer(0),
      d(distance),
      t(timeStep),
      c(velocity),
      mu(viscosity)
{
    calculateCoef();

    for(int j = 0; j < m; j++){
        for(int i = 0; i < n; i++){
            buffer0(i, j).position.x = d * i;
            buffer0(i, j).position.y = 0.0f;
            buffer0(i, j).position.z = d * j;

            buffer1(i, j).position.x = buffer0(i, j).position.x;
            buffer1(i, j).position.y = buffer0(i, j).position.y;
            buffer1(i, j).position.z = buffer0(i, j).position.z;

            buffer0(i, j).normal.x = 0.0f;
            buffer0(i, j).normal.y = 2.0f * d;
            buffer0(i, j).normal.z = 0.0f;

            buffer0(i, j).normal.x = buffer1(i, j).normal.x;
            buffer0(i, j).normal.y = buffer1(i, j).normal.y;
            buffer0(i, j).normal.z = buffer1(i, j).normal.z;
        }
    }
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
    Array2D<Vertex>& current = buffer0;
    Array2D<Vertex>& previous = buffer1;

    if (renderBuffer == 1) {
        current = buffer1;
        previous = buffer0;
    }

    for(int j = 1; j < height - 1; j++){
        for(int i = 1; i < width - 1; i++){
            previous(i, j).position.y = k1 * current(i, j).position.y +
                                        k2 * previous(i, j).position.y +
                                        k3 * (current(i + 1, j).position.y +
                                              current(i - 1, j).position.y +
                                              current(i, j + 1).position.y +
                                              current(i, j - 1).position.y);
        }
    }

    renderBuffer = 1 - renderBuffer;

    for(int j = 1; j < height - 1; j++){
        for(int i = 1; i < width - 1; i++){
            previous(i, j).normal.x = previous(i - 1, j).position.y - previous(i + 1, j).position.y;
            previous(i, j).normal.z = previous(i, j - 1).position.y - previous(i, j + 1).position.y;
            previous(i, j).normal.normalize();
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
