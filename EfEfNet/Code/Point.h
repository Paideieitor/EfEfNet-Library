#ifndef _POINT_
#define _POINT_

template<class data>
struct Point
{
	Point() : x(0), y(0) {}
	Point(data x, data y) : x(x), y(y) {}
	Point(data d) : x(d), y(d) {}

	~Point() {}

	void operator=(const Point& p)
	{
		x = p.x;
		y = p.y;
	}

	bool operator==(const Point& p)
	{
		if (x == p.x && y == p.y)
			return true;
		return false;
	}

	Point<data> operator+(const Point& p)
	{
		return Point<data>(x + p.x, y + p.y);
	}
	Point<data> operator-(const Point& p)
	{
		return Point<data>(x - p.x, y - p.y);
	}

	template<class T>
	void operator+=(const T& t)
	{
		x += t;
		y += t;
	}
	template<class T>
	void operator-=(const T& t)
	{
		x -= t;
		y -= t;
	}
	template<class T>
	Point<data> operator*(const T& t)
	{
		return Point<data>(x * t, y * t);
	}
	template<class T>
	Point<data> operator/(const T& t)
	{
		return Point<data>(x / t, y / t);
	}

	bool IsZero()
	{
		if (x == 0 && y == 0)
			return true;
		return false;
	}

	data x;
	data y;
};

typedef Point<int> iPoint;
typedef Point<float> fPoint;

#endif //_POINT_