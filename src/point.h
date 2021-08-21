#pragma once

#include <tuple>
#include <cmath>
#include <iostream>

//-//

template <typename A> class Point {
	
public:
	
	A x, y;
	
	explicit Point(A _x = 0, A _y = 0) : x(_x), y(_y) { }
	explicit Point(const std::pair <A, A>& p) : x(p.first), y(p.second) { }
	
	bool operator < (const Point& oth) const {
		return std::tie(x, y) < std::tie(oth.x, oth.y);
	}
	bool operator > (const Point& oth) const {
		return std::tie(x, y) > std::tie(oth.x, oth.y);
	}
	bool operator <= (const Point& oth) const {
		return std::tie(x, y) <= std::tie(oth.x, oth.y);
	}
	bool operator >= (const Point& oth) const {
		return std::tie(x, y) >= std::tie(oth.x, oth.y);
	}
	bool operator == (const Point& oth) const {
		return std::tie(x, y) == std::tie(oth.x, oth.y);
	}
	bool operator != (const Point& oth) const {
		return !((*this) == oth);
	}
	Point operator - () const {
		return Point(-x, -y);
	}
	Point& operator += (const Point& oth) {
		x += oth.x;
		y += oth.y;
		return *this;
	}
	Point& operator -= (const Point& oth) {
		x -= oth.x;
		y -= oth.y;
		return *this;
	}
	Point& operator *= (A val) {
		x *= val;
		y *= val;
		return *this;
	}
	Point& operator /= (A val) {
		x /= val;
		y /= val;
		return *this;
	}
	friend Point operator + (Point a, const Point& b) {
		return a += b;
	}
	friend Point operator - (Point a, const Point& b) {
		return a -= b;
	}
	friend Point operator * (const Point& p, A val) {
		return Point(p.x * val, p.y * val);
	}
	friend Point operator / (const Point& p, A val) {
		return Point(p.x / val, p.y / val);
	}
	A dot(const Point& oth) const {
		return x * oth.x + y * oth.y;
	}
	A cross(const Point& oth) const {
		return x * oth.x - y * oth.y;
	}
	A cross(const Point& a, const Point& b) const {
		return (a - (*this)).cross(b - (*this));
	}
	A dist_sq() const {
		return x * x + y * y;
	}
	double dist() const {
		return sqrt(dist_sq());
	}
	A dist_sq(const Point& oth) const {
		return ((*this) - oth).dist_sq();
	}
	double dist(const Point& oth) const {
		return sqrt(dist_sq(oth));
	}
	double angle() const {
		return atan2(y, x);
	}
	Point unit() const {
		return (*this) / dist();
	}
	Point perp() const {
		return Point(-y, x);
	}
	Point normal() const {
		return perp().unit();
	}
	Point rotate(double rads) const {
		return Point(x * cos(rads) - y * sin(rads), x * sin(rads) + y * cos(rads));
	}
	friend std::istream& operator >> (std::istream& stream, Point& p) {
		stream >> p.x >> p.y;
		return stream;
	}
	friend std::ostream& operator << (std::ostream& stream, const Point& p) {
		stream << "(" << p.x << ", " << p.y << ")";
		return stream;
	}
	
};

typedef Point <double> ptd;
typedef Point <long long> ptll;
