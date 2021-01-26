#pragma once

#include <vector>
#include <set>

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

    Seg() : p1(Point<T>()), p2(Point<T>()) {}
    Seg(const Point<T>& _p1, const Point<T>& _p2) : p1(_p1), p2(_p2) {}
    Seg(const T& x1, const T& y1, const T& x2, const T& y2) : p1(Point<T>(x1, y1)), p2(Point<T>(x2, y2)) {}

    friend bool operator==(const Seg& s1, const Seg& s2) {return (s1.p1 == s2.p1 && s1.p2 == s2.p2) || (s1.p1 == s2.p2 && s1.p2 == s2.p1);}
    friend bool operator!=(const Seg& s1, const Seg& s2) {return !(s1 == s2);}

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

    friend bool anyIntersect(vector<Seg> v) {
        vector<pair<Seg, int>> endpoints(2 * (int)v.size());

        for (int i = 0; i < (int)v.size(); i++) {
            if (v[i].p1.x > v[i].p2.x || (v[i].p1.x == v[i].p2.x && v[i].p1.y > v[i].p2.y))
                swap(v[i].p1, v[i].p2);
            endpoints[2 * i] = {v[i], 0};
            endpoints[2 * i + 1] = {v[i], 1};
        }
        sort(endpoints.begin(), endpoints.end(), [&](const auto& e1, const auto& e2) {
            Point pe1 = (e1.second == 0 ? e1.first.p1 : e1.first.p2);
            Point pe2 = (e2.second == 0 ? e2.first.p1 : e2.first.p2);
            if (pe1.x != pe2.x)
                return pe1.x < pe2.x;
            if (e1.second != e2.second)
                return e1.second < e2.second;
            return pe1.y < pe2.y;
        });

        bool res = false;
        auto cmp = [&](const Seg& s1, const Seg& s2) {
            if (s1 == s2) return false;
            if (min(s1.p2.x, s2.p2.x) < max(s1.p1.x, s2.p1.x))
                return true;
            T d1 = direction(s2.p1, s2.p2, s1.p1);
            T d2 = direction(s2.p1, s2.p2, s1.p2);
            T d3 = direction(s1.p1, s1.p2, s2.p1);
            T d4 = direction(s1.p1, s1.p2, s2.p2);
            if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
                ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) return res = true;
            if (d1 == 0 && s2.contains(s1.p1)) return res = true;
            if (d2 == 0 && s2.contains(s1.p2)) return res = true; 
            if (d3 == 0 && s1.contains(s2.p1)) return res = true;
            if (d4 == 0 && s1.contains(s2.p2)) return res = true;
            if (d1 == 0 && d2 == 0)
                return s1.p1.y > s2.p2.y;
            return (d1 <= 0 && d2 <= 0) || (d3 >= 0 && d4 >= 0);
        };
        set<Seg, decltype(cmp)> segs(cmp);
        
        for (const auto& endpoint : endpoints) {
            Seg s = endpoint.first;
            if (endpoint.second == 0) {
                segs.insert(s);
                if (res) return true;
                auto above = segs.upper_bound(s);
                auto cur = segs.find(s);
                if ((above != segs.end() && intersect(*above, s)) ||
                    (cur != segs.begin() && intersect(*(--cur), s)))
                    return true;
            } else {
                auto above = segs.upper_bound(s);
                auto cur = segs.find(s);
                if (above != segs.end() && intersect(*above, s) &&
                    cur != segs.begin() && intersect(*(--cur), s))
                    return true;
                segs.erase(s);
                if (res) return true;
            }
        }
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

    enum Position {Interior, Boundary, Exterior};

    Position where(const Point<T>& p) const {
        bool in = false;
        for (int i = 0; i < n; i++) {
            if (p == v[i]) return Boundary;
            int j = (i == n - 1 ? 0 : i + 1);
            if (direction(v[i], v[j], p) == 0 && Seg<T>(v[i], v[j]).contains(p)) 
                return Boundary;
            if ((v[i].y >= p.y && v[j].y < p.y && direction(v[i], v[j], p) > 0) ||
                (v[i].y < p.y && v[j].y >= p.y && direction(v[j], v[i], p) > 0))
                in = !in;
        }
        if (in) return Interior;
        return Exterior;
    }

};
