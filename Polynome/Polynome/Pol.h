#pragma once

#include <vector>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cassert>
#include <set>

using namespace std;

const int MAX_DEG = 100;

class Pol {
private:
	struct polynome {
		vector<long double> p;
		int deg;
	} t;
public:
	Pol() {
		t.p.assign(MAX_DEG, 0);
		t.deg = -1;
	}

	int getDeg() {
		return t.deg;
	}

	void setDeg(int deg) {
		t.deg = deg;
	}

	bool is_digit(char c) {
		return c >= '0' && c <= '9';
	}

	void print_p() {
		for (int i = t.deg; i > 0; i--) {
			if (t.p[i] == 0)
				continue;

			if (i != t.deg && t.p[i] > 0)
				cout << "+";
			if (t.p[i] == -1)
				cout << "-";
			else if (t.p[i] != 1)
				cout << t.p[i];

			if (i > 0)
				cout << "x";
			if (i > 1)
				cout << "^" << i;
		}
		if (t.p[0] > 0 && t.deg > 0)
			cout << "+";
		if (t.p[0] != 0) {
			cout << t.p[0];
		}
		cout << endl;
	}

	int to_value(int& loc, string& s) {
		if (!is_digit(s[loc]))
			return 1;
		int ans = 0;
		while (loc < s.size() && is_digit(s[loc])) {
			ans = ans * 10 + (int)(s[loc] - '0');
			loc++;
		}
		return ans;
	}

	int transform(string& s) {
		int loc = 0;
		while (loc < s.size()) {
			while (loc < s.size() && s[loc] == ' ')
				loc++;
			int cf = 1;
			if (loc < s.size() && s[loc] == '-') {
				cf = -1;
				loc++;
			}
			else if (loc < s.size() && s[loc] == '+')
				loc++;
			cf *= to_value(loc, s);

			if (loc == s.size()) {
				t.p[0] = cf;
				if (t.deg == -1)
					t.deg = 0;
				break;
			}
			else {
				if (s[loc] != 'x')
					return -1;
				loc++;
				int u = 1;
				if (loc < s.size() && s[loc] == '^') {
					loc++;
					u = to_value(loc, s);
				}
				if (t.deg == -1)
					t.deg = u;
				t.p[u] = cf;
			}
		}
	}


	Pol derivate() {
		Pol	temp;
		temp.t.deg = max(t.deg - 1, 0);
		for (int i = t.deg; i > 0; i--) {
			temp.t.p[i - 1] = i * t.p[i];
		}
		return	temp;
	}

	long double get_value(long double x) {
		long double initial = 1.0;
		long double ans = 0;
		for (int i = 0; i <= t.deg; i++) {
			ans += initial * t.p[i];
			initial *= x;
		}
		return ans;
	}

	pair<Pol, Pol> divide(Pol a, Pol b) {
		Pol answer;
		answer.t.deg = 0;

		if (a.t.deg < b.t.deg) {
			return make_pair(answer, a);
		}

		if (b.t.deg == 0) {
			return make_pair(a, answer);
		}

		while (a.t.deg >= b.t.deg) {

			int shift = a.t.deg - b.t.deg;

			long double cf = a.t.p[a.t.deg] / b.t.p[b.t.deg];

			Pol temp;
			answer.t.p[shift] = cf;
			answer.t.deg = max(answer.t.deg, shift);

			for (int i = 0; i <= b.t.deg; i++) {
				temp.t.p[i + shift] = cf * b.t.p[i];
			}

			for (int i = 0; i <= a.t.deg; i++) {
				a.t.p[i] -= temp.t.p[i];
			}

			a.t.p[a.t.deg] = 0;
			int i;
			for (i = a.t.deg; i > 0 && a.t.p[i] == 0; i--);
			a.t.deg = i;
		}
		return make_pair(answer, a);
	}

	Pol take_gcd(Pol a, Pol b) {
		while (true) {
			pair<Pol, Pol> res = divide(a, b);
			if (res.second.t.deg == 0 && res.second.t.p[0] == 0) {
				int i;

				for (i = a.t.deg; i > 0 && b.t.p[i] == 0; i--);
				b.t.deg = i;

				return b;
			}
			a = b;
			b = res.second;
		}
	}

	Pol negate() {
		Pol ans;
		ans.setDeg(t.deg);
		for (int i = 0; i <= t.deg; i++) {
			ans.t.p[i] = -1 * t.p[i];
		}
		return ans;
	}

	int get_sign(long double val) {
		long double curdegree = 1;
		long double res = 0;
		for (int i = 0; i <= t.deg; i++) {
			res += t.p[i] * curdegree;
			curdegree *= val;
		}
		if (res > 0)
			return 1;
		else
			return 0;
	}

};










