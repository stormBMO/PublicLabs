#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include "cvector.h"
#include "surface.h"

using namespace std;

typedef double pos;



int greatest(const int& value) {
	return value;
}

template<typename Head, typename... Tail>
int greatest(const Head& head, const Tail&... tail) {
	int value = head;
	Head greatest_tail = greatest(tail...);
	if(value < greatest_tail)
		swap(value, greatest_tail);
	while (greatest_tail != 0) {
		swap(value, greatest_tail);
		greatest_tail = value % greatest_tail;
	}
	return value;
}

void format(pos val, char var, int suppress_plus) {
	if (val == 0) {
		return;
	}
	if (val > 0) {
		if (!suppress_plus) {
			cout << "+ ";
		}
	}
	else {
		cout << "- ";
	}
	if (abs(val) != 1) {
		cout << abs(val) << " * ";
	}
	cout << var << " ";
}

bool mult(pos a, pos b, pos c, pos d) {
	return a * d == c * b;
}

bool Surface::operator==(const Surface& s) {
	return
		mult(c.x, s.c.x, c.y, s.c.y) &&
		mult(c.x, s.c.x, c.z, s.c.z) &&
		mult(c.x, s.c.x, n, s.n) &&
		mult(c.y, s.c.y, c.z, s.c.z) &&
		mult(c.y, s.c.y, n, s.n) &&
		mult(c.z, s.c.z, n, s.n);
}

ostream& operator<<(ostream& os, Surface& s) {
	int x = abs((int)s.c.x), y = abs((int)s.c.y), z = abs((s.c.z)), d = abs((int)s.n);
	if (abs(s.c.x) == x && abs(s.c.y) == y && abs(s.c.z) == z && abs(s.n) == d) {
		int t = greatest(x, y, z, d); // может быть трабл
		s.c.x /= t;
		s.c.y /= t;
		s.c.z /= t;
		s.n /= t;
	}


	format(s.c.x, 'x', 1);
	format(s.c.y, 'y', s.c.x == 0);
	format(s.c.z, 'z', s.c.x == 0 && s.c.y == 0);
	cout << "<= " << -s.n;
	return os;
}

bool parallel(const Surface& a, const Surface& b) {
	return
		mult(a.c.x, b.c.x, a.c.y, b.c.y) &&
		mult(a.c.x, b.c.x, a.c.z, b.c.z) &&
		mult(a.c.y, b.c.y, a.c.z, b.c.z);
}