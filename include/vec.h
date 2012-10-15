#pragma once



// Vector

template <class T>
class Vec2
{
public:
	T _x;
	T _y;

	Vec2();
	Vec2(T x,T y);

	friend Vec2<T> operator*(const Vec2<T> &op1,const Vec2<T> &op2)
    {	
		return Vec3<T>(op1._x * op2._x,op1._y * op2._y);
    };
    friend Vec2<T> operator*(const Vec2<T> &op1,const T &op2)
    {	
    	return Vec2<T>(op1._x * op2,op1._y * op2);
    };
    friend Vec2<T> operator*(const T &op1,const Vec2<T> &op2)
    {	
		return Vec2<T>(op1 * op2._x,op1 * op2._y);
    };
    friend Vec2<T> operator/(const Vec2<T> &op1,const Vec2<T> &op2)
    {	
    	return Vec2<T>(op1._x / op2._x,op1._y / op2._y);
    };
    friend Vec2<T> operator/(const Vec2<T> &op1,const T& op2)
    {	
    	return Vec2<T>(op1._x / op2,op1._y / op2);
    };
    friend Vec2<T> operator/(const T &op1,const Vec2<T> &op2)
    {	
    	return Vec2<T>(op1 / op2._x,op1 / op2._y);
    };
    friend Vec2<T> operator+(const Vec2<T> &op1,const Vec2<T> &op2)
    {	
    	return Vec2<T>(op1._x + op2._x,op1._y + op2._y);
    };
    friend Vec2<T> operator+(const Vec2<T> &op1,const T& op2)
    {	
    	return Vec2<T>(op1._x + op2,op1._y + op2);
    };
    friend Vec2<T> operator+(const T &op1,const Vec2<T> &op2)
    {	
    	return Vec2<T>(op1 + op2._x,op1 + op2._y);
    };
    friend Vec2<T> operator-(const Vec2<T> &op1,const Vec2<T> &op2)
    {	
    	return Vec2<T>(op1._x - op2._x,op1._y - op2._y);
    };
    friend Vec2<T> operator-(const Vec2<T> &op1,const T& op2)
    {	
    	return Vec2<T>(op1._x - op2,op1._y - op2);
    };
    friend Vec2<T> operator-(const T &op1,const Vec2<T> &op2)
    {	
    	return Vec2<T>(op1 - op2._x,op1 - op2._y);
    };
};


template <class T>
class Vec3
{
public:
	T _x;
	T _y;
	T _z;

	Vec3();
	Vec3(T x,T y,T z);

	T normal();

	Vec3<T> operator=(const Vec2<T> &op2)
    {	
		return Vec3<T>(op2._x,op2._y,this->_z);
    };

	//OPERATOR OVERLOADS

	friend Vec3<T> operator*(const Vec3<T> &op1,const Vec3<T> &op2)
    {	
		return Vec3<T>(op1._x * op2._x,op1._y * op2._y,op1._z * op2._z);
    };
    friend Vec3<T> operator*(const Vec3<T> &op1,const T& op2)
    {	
    	return Vec3<T>(op1._x * op2,op1._y * op2,op1._z * op2);
    };
    friend Vec3<T> operator*(const T &op1,const Vec3<T> &op2)
    {	
		return Vec3<T>(op1 * op2._x,op1 * op2._y,op1 * op2._z);
    };
    friend Vec3<T> operator/(const Vec3<T> &op1,const Vec3<T> &op2)
    {	
    	return Vec3<T>(op1._x / op2._x,op1._y / op2._y,op1._z / op2._z);
    };
    friend Vec3<T> operator/(const Vec3<T> &op1,const T& op2)
    {	
    	return Vec3<T>(op1._x / op2,op1._y / op2,op1._z / op2);
    };
    friend Vec3<T> operator/(const T &op1,const Vec3<T> &op2)
    {	
		return Vec3<T>(op1 / op2._x,op1 / op2._y,op1 / op2._z);
    };
    friend Vec3<T> operator+(const Vec3<T> &op1,const Vec3<T> &op2)
    {	
    	return Vec3<T>(op1._x + op2._x,op1._y + op2._y,op1._z + op2._z);
    };
    friend Vec3<T> operator+(const Vec3<T> &op1,const T& op2)
    {	
    	return Vec3<T>(op1._x + op2,op1._y + op2,op1._z + op2);
    };
    friend Vec3<T> operator+(const T &op1,const Vec3<T> &op2)
    {	
		return Vec3<T>(op1 + op2._x,op1 + op2._y,op1 + op2._z);
    };
    friend Vec3<T> operator-(const Vec3<T> &op1,const Vec3<T> &op2)
    {	
    	return Vec3<T>(op1._x - op2._x,op1._y - op2._y,op1._z - op2._z);
    };
    friend Vec3<T> operator-(const Vec3<T> &op1,const T& op2)
    {	
    	return Vec3<T>(op1._x - op2,op1._y - op2,op1._z - op2);
    };
	friend Vec3<T> operator-(const T &op1,const Vec3<T> &op2)
    {	
		return Vec3<T>(op1 - op2._x,op1 - op2._y,op1 - op2._z);
    };

	friend T dot(const Vec3<T> &op1,const Vec3<T> &op2)
    {	
		return (op1._x * op2._x + op1._y * op2._y + op1._z * op2._z);
    };
	friend Vec3<T> cross(const Vec3<T> &op1,const Vec3<T> &op2)
	{
		return Vec3<T>(op1._y*op2._z-op1._z-op2._y,
			           op1._z*op2._x-op1._x-op1._z,
					   op1._x*op2._y-op1._y-op2._x);
	};
};

template <class T>
Vec2<T>::Vec2()
{
	_x = 0;
	_y = 0;
}

template <class T>
Vec2<T>::Vec2(T x,T y)
{
	_x = x;
	_y = y;
}

template <class T>
Vec3<T>::Vec3()
{
	_x = 0;
	_y = 0;
	_z = 0;
}

template <class T>
Vec3<T>::Vec3(T x,T y,T z)
{
	_x = x;
	_y = y;
	_z = z;
}

template <class T>
T Vec3<T>::normal()
{
	return sqrt(_x*_x+_y*_y+_z*_z);
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

