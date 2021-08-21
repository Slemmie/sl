#pragma once

#include <sl/point.h>

#include <vector>
#include <iostream>

//-//

template <typename A> class Convex_hull {
	
public:
	
	int size, size_pts;
	std::vector <Point <A>> pts, hull;
	
	Convex_hull(const std::vector <Point <A>>& _pts, bool _init = true) :
	size(0),
	size_pts(_pts.size()),
	pts(_pts)
	{
		if (!_init) {
			return;
		}
		init();
	}
	
	void init() {
		size_pts = pts.size();
		if (size_pts < 2) {
			hull = pts;
			size = hull.size();
			return;
		}
		std:sort(pts.begin(), pts.end());
		int ptr = 0, s = 0;
		hull = std::vector <Point <A>> (size_pts + 1);
		for (int i = 2; i--; s = --ptr, std::reverse(pts.begin(), pts.end())) {
			for (const Point <A>& p : pts) {
				while (ptr >= s + 2 && hull[ptr - 2].cross(hull[ptr - 1], p) <= 0) {
					ptr--;
				}
				hull[ptr++] = p;
			}
		}
		hull = { hull.begin(), hull.begin() + ptr - (ptr == 2 && hull[0] == hull[1]) };
		size = hull.size();
	}
	
	friend std::ostream& operator << (std::ostream& stream, const Convex_hull& hull) {
		stream << "{";
		for (int i = 0; i < (int)hull.pts.size(); i++) {
			stream << hull.pts[i];
			if (i + 1 != (int)hull.pts.size()) {
				stream << ", ";
			}
		}
		stream << "}";
		return stream;
	}
	
};
