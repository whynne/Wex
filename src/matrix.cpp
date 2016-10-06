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

#include "matrix.h"

Mat4::Mat4() {
	Mat4::Clear(*this);
}

void Mat4::Multiply(Mat4& out, const Mat4& other) {
	Mat4::Clear(out);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				out.v[i][j] += this->v[i][k] * other.v[k][j];
			}
		}
	}
}

void Mat4::Identity(Mat4& out) {
	Mat4::Clear(out);
	out.v[0][0] = 1.0f;
	out.v[1][1] = 1.0f;
	out.v[2][2] = 1.0f;
	out.v[3][3] = 1.0f;
}

void Mat4::Ortho(Mat4& out, float left, float top, float right, float bottom, float znear, float zfar) {
	Mat4::Clear(out);
	out.v[0][0] = 2  / (right - left);
	out.v[1][1] = 2  / (top - bottom);
	out.v[2][2] = -2 / (zfar - znear);
	out.v[3][3] = 1.0f;
	out.v[3][0] = -(right + left) / (right - left);
	out.v[3][1] = -(top + bottom) / (top - bottom);
	out.v[3][2] = -(zfar + znear) / (zfar - znear);
}

Mat4::Mat4(float m1,float m2,float m3,float m4,float m5,float m6,float m7,float m8,float m9,float m10,float m11,float m12,float m13,float m14,float m15,float m16)
{
	v[0][0] = m1;    v[0][1] = m2;    v[0][2] = m3; v[0][3] = m4; 
	v[1][0] = m5;    v[1][1] = m6;    v[1][2] = m7; v[1][3] = m8; 
	v[2][0] = m9;    v[2][1] = m10;    v[2][2] = m11; v[2][3] = m12; 
	v[3][0] = m13;    v[3][1] = m14;    v[3][2] = m15; v[3][3] = m16; 
}

Mat4 operator+(const Mat4& op1,const Mat4& op2)
{	
	Mat4 result;
	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++) 
		{
			result.v[i][j] += op1.v[i][j] + op2.v[i][j];
		}
	}
	return result;
};

Mat4 operator*(const Mat4& op1,const Mat4& op2)
{	
	Mat4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result.v[i][j] += op1.v[i][k] * op2.v[k][j];
			}
		}
	}
	return result;
};
