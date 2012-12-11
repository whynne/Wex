// ============================================================================
// Copyright (c) 2011-2012 J.C. Moyer
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ============================================================================

#ifndef MATRIX_H
#define MATRIX_H

// Dependencies
#include <cstring>

class Mat4 {
public:
	float v[4][4];
public:
	Mat4();
	Mat4(float m1,float m2,float m3,float m4,float m5,float m6,float m7,float m8,float m9,float m10,float m11,float m12,float m13,float m14,float m15,float m16);
	void Multiply(Mat4& out, const Mat4& other);
	static void Identity(Mat4& out);
	static void Ortho(Mat4& out, float left, float top, float right, float bottom, float znear, float zfar);
	inline static void Clear(Mat4& out);
	
	friend Mat4 operator+(const Mat4& op1,const Mat4& op2);
	friend Mat4 operator*(const Mat4& op1,const Mat4& op2);
};

inline void Mat4::Clear(Mat4& out) {
	memset(&out, 0, sizeof(float) * 16);
}


#endif
