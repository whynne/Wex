#pragma once



// Vector

template <class T>
class Vec2
{
public:
	T x;
	T y;

	Vec2();
	Vec2(T x,T y);

	Vec2<T> rotate2(Vec2<T> pivot,float theta)
	{
		Vec2<T> result;
		result.x = pivot.x + (x - pivot.x) * cos(theta) - (y - pivot.y) * sin(theta);
		result.y = pivot.y + (x - pivot.x) * sin(theta) - (y - pivot.y) * cos(theta);
	}


	friend Vec2<T> operator*(const Vec2<T> &op1,const Vec2<T> &op2)
    {	
		return Vec3<T>(op1.x * op2.x,op1.y * op2.y);
    };
    friend Vec2<T> operator*(const Vec2<T> &op1,const T &op2)
    {	
    	return Vec2<T>(op1.x * op2,op1.y * op2);
    };
    friend Vec2<T> operator*(const T &op1,const Vec2<T> &op2)
    {	
		return Vec2<T>(op1 * op2.x,op1 * op2.y);
    };
    friend Vec2<T> operator/(const Vec2<T> &op1,const Vec2<T> &op2)
    {	
    	return Vec2<T>(op1.x / op2.x,op1.y / op2.y);
    };
    friend Vec2<T> operator/(const Vec2<T> &op1,const T& op2)
    {	
    	return Vec2<T>(op1.x / op2,op1.y / op2);
    };
    friend Vec2<T> operator/(const T &op1,const Vec2<T> &op2)
    {	
    	return Vec2<T>(op1 / op2.x,op1 / op2.y);
    };
    friend Vec2<T> operator+(const Vec2<T> &op1,const Vec2<T> &op2)
    {	
    	return Vec2<T>(op1.x + op2.x,op1.y + op2.y);
    };
    friend Vec2<T> operator+(const Vec2<T> &op1,const T& op2)
    {	
    	return Vec2<T>(op1.x + op2,op1.y + op2);
    };
    friend Vec2<T> operator+(const T &op1,const Vec2<T> &op2)
    {	
    	return Vec2<T>(op1 + op2.x,op1 + op2.y);
    };
    friend Vec2<T> operator-(const Vec2<T> &op1,const Vec2<T> &op2)
    {	
    	return Vec2<T>(op1.x - op2.x,op1.y - op2.y);
    };
    friend Vec2<T> operator-(const Vec2<T> &op1,const T& op2)
    {	
    	return Vec2<T>(op1.x - op2,op1.y - op2);
    };
    friend Vec2<T> operator-(const T &op1,const Vec2<T> &op2)
    {	
    	return Vec2<T>(op1 - op2.x,op1 - op2.y);
    };
};


template <class T>
class Vec3
{
public:
	T x;
	T y;
	T z;

	Vec3();
	Vec3(T x,T y,T z);

	T normal();

	Vec3<T> operator=(const Vec2<T> &op2)
    {	
		return Vec3<T>(op2.x,op2.y,this->z);
    };

	void rotate(Vec3<T> pivot,float theta)
	{
		x = pivot.x + (x - pivot.x) * cos(theta) - (y - pivot.y) * sin(theta);
		y = pivot.y + (x - pivot.x) * sin(theta) - (y - pivot.y) * cos(theta);
	}

	//OPERATOR OVERLOADS

	friend Vec3<T> operator*(const Vec3<T> &op1,const Vec3<T> &op2)
    {	
		return Vec3<T>(op1.x * op2.x,op1.y * op2.y,op1.z * op2.z);
    };
    friend Vec3<T> operator*(const Vec3<T> &op1,const T& op2)
    {	
    	return Vec3<T>(op1.x * op2,op1.y * op2,op1.z * op2);
    };
    friend Vec3<T> operator*(const T &op1,const Vec3<T> &op2)
    {	
		return Vec3<T>(op1 * op2.x,op1 * op2.y,op1 * op2.z);
    };
    friend Vec3<T> operator/(const Vec3<T> &op1,const Vec3<T> &op2)
    {	
    	return Vec3<T>(op1.x / op2.x,op1.y / op2.y,op1.z / op2.z);
    };
    friend Vec3<T> operator/(const Vec3<T> &op1,const T& op2)
    {	
    	return Vec3<T>(op1.x / op2,op1.y / op2,op1.z / op2);
    };
    friend Vec3<T> operator/(const T &op1,const Vec3<T> &op2)
    {	
		return Vec3<T>(op1 / op2.x,op1 / op2.y,op1 / op2.z);
    };
    friend Vec3<T> operator+(const Vec3<T> &op1,const Vec3<T> &op2)
    {	
    	return Vec3<T>(op1.x + op2.x,op1.y + op2.y,op1.z + op2.z);
    };
    friend Vec3<T> operator+(const Vec3<T> &op1,const T& op2)
    {	
    	return Vec3<T>(op1.x + op2,op1.y + op2,op1.z + op2);
    };
    friend Vec3<T> operator+(const T &op1,const Vec3<T> &op2)
    {	
		return Vec3<T>(op1 + op2.x,op1 + op2.y,op1 + op2.z);
    };
    friend Vec3<T> operator-(const Vec3<T> &op1,const Vec3<T> &op2)
    {	
    	return Vec3<T>(op1.x - op2.x,op1.y - op2.y,op1.z - op2.z);
    };
    friend Vec3<T> operator-(const Vec3<T> &op1,const T& op2)
    {	
    	return Vec3<T>(op1.x - op2,op1.y - op2,op1.z - op2);
    };
	friend Vec3<T> operator-(const T &op1,const Vec3<T> &op2)
    {	
		return Vec3<T>(op1 - op2.x,op1 - op2.y,op1 - op2.z);
    };

	friend T dot(const Vec3<T> &op1,const Vec3<T> &op2)
    {	
		return (op1.x * op2.x + op1.y * op2.y + op1.z * op2.z);
    };
	friend Vec3<T> cross(const Vec3<T> &op1,const Vec3<T> &op2)
	{
		return Vec3<T>(op1.y*op2.z-op1.z-op2.y,
			           op1.z*op2.x-op1.x-op1.z,
					   op1.x*op2.y-op1.y-op2.x);
	};
};

template <class T>
Vec2<T>::Vec2()
{
	x = 0;
	y = 0;
}

template <class T>
Vec2<T>::Vec2(T x,T y)
{
	x = x;
	y = y;
}

template <class T>
Vec3<T>::Vec3()
{
	x = 0;
	y = 0;
	z = 0;
}

template <class T>
Vec3<T>::Vec3(T x,T y,T z)
{
	x = x;
	y = y;
	z = z;
}

template <class T>
T Vec3<T>::normal()
{
	return sqrt(x*x+y*y+z*z);
}

typedef Vec3<int>    Point3i;
typedef Vec3<double> Point3d;
typedef Vec3<float>  Point3f;
typedef Vec2<int>    Point2i;
typedef Vec2<double> Point2d;
typedef Vec2<float>  Point2f;

typedef Vec3<int>    Vector3i;
typedef Vec3<double> Vector3d;
typedef Vec3<float>  Vector3f;
typedef Vec2<int>    Vector2i;
typedef Vec2<double> Vector2d;
typedef Vec2<float>  Vector2f;

typedef Vec3<int>    Vertex3i;
typedef Vec3<double> Vertex3d;
typedef Vec3<float>  Vertex3f;
typedef Vec2<int>    Vertex2i;
typedef Vec2<double> Vertex2d;
typedef Vec2<float>  Vertex2f;

