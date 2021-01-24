#pragma once

#include <vector>
using namespace std;

template <typename T>
struct Point {

    T x, y;

    Point() : x(0), y(0) {}
    Point(T _x, T _y) : x(_x), y(_y) {}

    friend bool operator==(const Point& p1, const Point& p2) {return p1.x == p2.x && p1.y == p2.y;}
    friend bool operator!=(const Point& p1, const Point& p2) {return !(p1 == p2);}

    friend Point operator+(const Point& p1, const Point& p2) {return Point(p1.x + p2.x, p1.y + p2.y);}
    friend Point operator-(const Point& p1, const Point& p2) {return Point(p1.x - p2.x, p1.y - p2.y);}
    friend Point operator*(const T& t, const Point& p) {return Point(t * p.x, t * p.y);}
    friend Point operator*(const Point& p, const T& t) {return Point(p.x * t, p.y * t);}
    friend Point operator/(const Point& p, const T& t) {return Point(p.x / t, p.y / t);}

    Point& operator+=(const Point& p) {x += p.x, y += p.y; return *this;}
    Point& operator-=(const Point& p) {x -= p.x, y -= p.y; return *this;}
    Point& operator*=(const T& t) {x *= t, y *= t; return *this;}
    Point& operator/=(const T& t) {x /= t, y /= t; return *this;}
    
    friend T cross(const Point& p1, const Point& p2) {return p1.x * p2.y - p2.x * p1.y;}
    friend T direction(const Point& p1, const Point& p2, const Point& p3) {return cross(p3 - p1, p2 - p1);}

};

template <typename T>
struct Seg {

    Point<T> p1, p2;

    Seg(const Point<T>& _p1, const Point<T>& _p2) : p1(_p1), p2(_p2) {}
    Seg(const T& x1, const T& y1, const T& x2, const T& y2) : p1(Point<T>(x1, y1)), p2(Point<T>(x2, y2)) {}

    bool contains(const Point<T>& p) const {return min(p1.x, p2.x) <= p.x && p.x <= max(p1.x, p2.x) && min(p1.y, p2.y) <= p.y && p.y <= max(p1.y, p2.y);}

    friend bool intersect(const Seg& s1, const Seg& s2) {
        T d1 = direction(s2.p1, s2.p2, s1.p1);
        T d2 = direction(s2.p1, s2.p2, s1.p2);
        T d3 = direction(s1.p1, s1.p2, s2.p1);
        T d4 = direction(s1.p1, s1.p2, s2.p2);
        if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
            ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) return true;
        if (d1 == 0 && s2.contains(s1.p1)) return true;
        if (d2 == 0 && s2.contains(s1.p2)) return true; 
        if (d3 == 0 && s1.contains(s2.p1)) return true;
        if (d4 == 0 && s1.contains(s2.p2)) return true;
        return false;
    }

};

template <typename T>
struct Polygon {

    int n;
    vector<Point<T>> v;
    
    Polygon(const vector<Point<T>>& _v) : n((int)_v.size()), v(_v) {}
    
    T area() const {
        T res = 0;
        for (int i = 2; i < n; i++) {
            res -= direction(v[0], v[i - 1], v[i]);
        }
        return abs(res);
    }

};
