/*
* Copyright (c) 2010 Antonie Jovanoski <minimoog77_at_gmail.com>
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

#ifndef ARRAY2D_H
#define ARRAY2D_H

template<typename T>
class Array2D
{
public:
    Array2D(int width = 1, int height = 1)
        : w(width),
          h(height),
          data(new T[w * h])
    {
        clear(T());
    }

    Array2D(const Array2D<T> &t)
    {
        w = h = 0;
        data = 0;
        (*this) = t;
    }

    ~Array2D()
    {
        delete []data;
    }

    const Array2D & operator= (const Array2D<T> & t)
    {
        if (w != t.w || h != t.h)
            setSize(t.w, t.h);

        for (int i = 0; i < w * h; i++)
            data[i] = t.data[i];

        return *this;
    }

    void setSize(int width, int height)
    {
        if(w == width && h == height)
            return;

        delete []data;

        w = width;
        h = height;

        data = new T[w * h];
    }

    T & operator() (int i, int j)
    {
        return data[i + j * w];
    }

    const T & operator() (int i, int j) const
    {
        return data[i + j * w];
    }

    int width() const
    {
        return w;
    }

    int height() const
    {
        return h;
    }

    void clear(const T & val)
    {
        for(int i = 0; i < w * h; i++)
            data[i] = val;
    }

    void copy(const Array2D<T> & src, int iOffset = 0, int jOffset = 0, int width = 0, int height = 0)
    {
        if(width == 0)
            width = src.width();

        if(height == 0)
            height = src.height();

        if(iOffset + width > w)
            return;

        if(jOffset + height > h)
            return;

        for(int i = 0; i < width; i++)
            for(j = 0; j < height; j++)
                (*this)(iOffset + i, jOffset + j) = src(i, j);
    }

    T* pointer()
    {
        return data;
    }

    const T* pointer() const
    {
        return data;
    }

private:
    int w, h;
    T * data;
};

#endif
