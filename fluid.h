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

#ifndef FLUID_H
#define FLUID_H

#include "math/tvec3.h"
#include "array2d.h"

struct Vertex
{
    TVec3 position;
    TVec3 normal;
};

class Fluid
{
	
public:
	
	Fluid(int n, int m, float distance, float timeStep, float velocity, float viscosity);
	~Fluid();
	
	void	evaluate(void);

    float	distance() const;
	void	setDistance(float distance);
	void	setTimeStep(float timeStep);
	void	setVelocity(float velocity);
	void	setViscosity(float viscosity);
    const Vertex* bufferData() const;
    int width() const { return m_width; }
    int height() const { return m_height; }

private:
	
    int		m_width;
    int		m_height;
    Array2D<Vertex> buffer0;
    Array2D<Vertex> buffer1;

	float	d, c, mu, t;
	
	int		renderBuffer; //0 ili 1 za koj bafer

	float k1, k2, k3;

	void calculateCoef();
};

#endif
