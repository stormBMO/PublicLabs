#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cassert>
#include <set>
#include "Pol.h"


using namespace std;

int greatest(int a, int b) {
	if (b == 0)
		return a;
	return greatest(b, a % b);
}

int shtur_value(long double val, vector<Pol>& shtur) {
	int ans = 0;
	int prevsign = shtur[0].get_sign(val);
	for (int i = 1; i < shtur.size(); i++) {
		int cursign = shtur[i].get_sign(val);
		if (cursign != prevsign) {
			ans++;
			prevsign = cursign;
		}
	}
	return ans;
}


void find_roots(long double L, long double R, vector<Pol>& shtur, set<long double>& roots) {
	int WL = shtur_value(L, shtur), WR = shtur_value(R, shtur);
	if (WL - WR) {
		for (int i = 0; i < 100; i++) {
			long double M = (L + R) / 2;
			if (shtur[0].get_sign(L) != shtur[0].get_sign(M))
				R = M;
			else
				L = M;
		}

		for (long double cval : roots) {
			if (abs(L - cval) < 1e-10) {
				return;
			}
		}

		cout << L << endl;
		return;
	}
	long double M = (L + R) / 2;
	int WM = shtur_value(M, shtur);
	if (WL - WM > 0) {
		find_roots(L, M, shtur, roots);
	}
	if (WM - WR > 0) {
		find_roots(M, R, shtur, roots);
	}
}

int main() {
	setlocale(0, "");
	Pol main;

	set<long double> other_roots;
	set<pair<int, int>> target_answer;

	vector<Pol> shtur;

	string mn;
	int ap, l;
	double L = -100, R = 100;

	cout << "Введите многочлен:" << endl;
	cin >> mn;

	if (main.transform(mn) < 0) {
		cout << "\nВы неправильно ввели многочлен, проверьте коректность введенных данных и повторите попытку." << endl;
		return 0;
	}

	cout << "Введите параметр точности аппроксимации:" << endl;
	if (!(cin >> ap)) {
		cout << "Парамент должен быть числом, проверьте коректность введенных данных и повторите попытку.";
		return 0;
	}

	for (int n = -1000; n <= 1000; n++) {
		for (int d = 1; d <= 10000; d++) {
			if (greatest(abs(n), d) == 1 && abs(main.get_value((long double)n / d)) < 1e-30) {
				target_answer.insert(make_pair(n, d));
				other_roots.insert((long double)n / d);
			}
		}
	}

	Pol der = der.derivate();
	Pol gcd = gcd.take_gcd(main, der);

	Pol temp; //отсюда может быть ошибка с темпом, а может и нет, потом проверю

	if (gcd.getDeg() != 0)
		shtur.push_back(temp.divide(main, gcd).first);
	else
		shtur.push_back(main);
	shtur.push_back(shtur[0].derivate());


	while (shtur[shtur.size() - 1].getDeg() > 0) {
		Pol t = shtur[shtur.size() - 2].negate();
		Pol last = shtur[shtur.size() - 1];

		shtur.push_back(temp.divide(t, last).second);
	}

	cout << "Вы ввели следующий многочлен:" << endl;
	main.print_p();

	cout << "Ряд Штурмана:" << endl;
	for (auto i : shtur) i.print_p();

	cout << fixed << setprecision(ap);

	cout << "Рациональные корни:" << endl;
	if (target_answer.size() != 0) {
		int j = 1;
		for (auto i : target_answer) {
			cout << "x" << j << " = ";
			cout << i.first;
			if (i.second != 1)
				cout << "/" << i.second;
			j++;
			cout << endl;
		}
	}
	else
		cout << "Их нет..." << endl;

	cout << "Действительные корни:" << endl;
	find_roots(L, R, shtur, other_roots);
	return 0;
}