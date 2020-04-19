#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <set>
#include <map>
#include <fstream>
#include <algorithm>
#include "cvector.h"
#include "surface.h"
#include "draw.h"

using namespace std;

typedef double pos;


vector<CVector> scan_polygon();
vector<CVector> scan_vertices();
vector<CVector> get_vertices(vector<Surface>& sf, int output_allowed);
vector<Surface> scan_surfaces();
vector<Surface> build_convex_hull(vector<CVector>& a, int output_allowed);
map<pair<string, string>, int> compose_graph(vector<Surface>& sf, const vector<CVector>& vr);
bool detect_collision(vector<CVector>& m1, vector<CVector>& m2);
bool fits_single(const Surface& s, CVector t);
bool fits(vector<Surface>& sf, CVector t);
pos len(const CVector& a);
Surface compose_surface_equation(CVector a, CVector b, CVector c);
pair<CVector, bool> get_cross(const Surface& a, const Surface& b, const Surface& c);

ifstream d;

int main(int argc, char** args) {
	char type, type1;
	vector<Surface> faces;
	map<pair<string, string>, int> mat1;
	//cout << args[0] << " " << args[1] << " " << args[2] << " " << endl;
	if (argc == 3) {
		d.open(args[1], ios::in);
		//freopen(args[1], "r", stdin);
		d >> type;
		vector<CVector> v1 = scan_polygon();
		d.close();
		d.open(args[2], ios::in);
		//freopen(args[2], "r+", stdin);
		d >> type1;
		vector<CVector> v2 = scan_polygon();
		d.close();
		if (v1.empty() || v2.empty())
			return 0;
		cout << "COLLUSION" << (detect_collision(v1, v2) ? " DETECTED" : "WAS NOT DETECT") << endl;	
		return 0;
	} 
	if (argc == 2)
		d.open(args[1], ios::in);

	d >> type;

	if (type == 'V') {
		vector<CVector> vertices = scan_vertices();
		if (vertices.empty())
			return 0;
		faces = build_convex_hull(vertices, 1);
	}
	else if (type == 'H') {
		faces = scan_surfaces();
		if (faces.empty())
			return 0;
	}
	else {
		cout << "Unknown operation, aborting..." << endl;
		return 0;
	}

	vector<CVector> vertices = get_vertices(faces, 1);

	mat1 = compose_graph(faces, vertices);
	vector<CVector> out;
	out.clear();
	draw(mat1, vertices);

	return 0;
}

vector<CVector> scan_polygon() {
	int n;
	d >> n;
	vector<CVector> ans(n);
	for (int i = 0; i < n; i++) {
		if (!(d >> ans[i].x >> ans[i].y >> ans[i].z)) {
			cout << "Smth`s wrong with coords" << endl;
			ans.clear();
			return ans;
		}
	}
	return ans;
}

bool detect_collision(vector<CVector>& m1, vector<CVector>& m2) {
	for (int i = 0; i < m2.size(); i++) {
		m2[i] = m2[i] * -1;
	}

	vector<CVector> sum_vector;
	for (int i = 0; i < m1.size(); i++) {
		for (int j = 0; j < m2.size(); j++) {
			sum_vector.push_back(m1[i] + m2[j]);
		}
	}

	vector<Surface> faces = build_convex_hull(sum_vector, 1);
	vector<CVector> vertices = get_vertices(faces, 1);
	CVector zero = CVector(0, 0, 0);
	return fits(faces, zero);
}

vector<CVector> scan_vertices() {
	int n;
	d >> n;
	vector<CVector> a(n);
	for (int i = 0; i < n; i++) {
		if (!(d >> a[i].x >> a[i].y >> a[i].z)){
			cout << "Smth`s wrong with coords" << endl;
			a.clear();
			return a;
		}
	}
	return a;
}

vector<Surface> build_convex_hull(vector<CVector>& a, int output_allowed) {
	int n = a.size();
	vector<Surface> answer;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			for (int k = j + 1; k < n; k++) {
				CVector v1 = a[j] - a[i];
				CVector v2 = a[k] - a[i];
				if (len(v1 ^ v2) < 1e-9) 
					continue;
				Surface sf = compose_surface_equation(a[i], a[j], a[k]);
				int sm = 0, bg = 0;
				for (int t = 0; t < n; t++) {
					if (t == i || t == j || t == k) {
						continue;
					}
					pos cval = a[t].x * sf.c.x +
						a[t].y * sf.c.y +
						a[t].z * sf.c.z + sf.n;
					if (cval < 0) {
						sm++;
					}
					else if (cval > 0) {
						bg++;
					}
				}
				if (sm > 0 && bg > 0) {
					continue;
				}
				if (bg > 0) {
					sf.c = sf.c * -1;
					sf.n *= -1;
				}
				int t = 0;
				for (; t < answer.size(); t++) {
					if (sf == answer[t]) {
						break;
					}
				}
				if (t == answer.size()) {
					answer.push_back(sf);
				}
			}
		}
	}
	if (!output_allowed)
		return answer;
	cout << "Number of faces: " << answer.size() << endl;
	for (int i = 0; i < answer.size(); i++) 
		cout << answer[i] << endl;
	return answer;
}

Surface compose_surface_equation(CVector a, CVector b, CVector c) {
	CVector v1 = (b - a), v2 = (c - a);
	CVector norm = v1 ^ v2;
	pos d = -(norm.x * a.x + norm.y * a.y + norm.z * a.z);
	return Surface { 
		norm * -1, 
		d * -1 
	};
}

pos len(const CVector& a) {
	return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

vector<Surface> scan_surfaces() {
	int n;
	d >> n;
	vector<Surface> a;
	for (int i = 0; i < n; i++) {
		Surface s;
		if (!(d >> s.c.x >> s.c.y >> s.c.z >> s.n)) {
			cout << "Smth`s wrong with coords" << endl;
			a.clear();
			return a;
		}
		s.n *= -1;
		int j = 0;
		for (; j < a.size(); j++) 
			if (s == a[j]) 
				break;
		if (j == a.size()) 
			a.push_back(s);
	}
	cout << "Number of faces: " << a.size() << endl;
	for (auto s : a) 
		cout << s << endl;
	return a;
}

vector<CVector> get_vertices(vector<Surface>& sf, int output_allowed) {
	vector<CVector> ans;
	for (int i = 0; i < sf.size(); i++) {
		for (int j = i + 1; j < sf.size(); j++) {
			for (int k = j + 1; k < sf.size(); k++) {
				Surface a = sf[i], b = sf[j], c = sf[k];
				if (parallel(a, b) || parallel(b, c) || parallel(a, c)) {
					continue;
				}
				pair<CVector, bool> t = get_cross(a, b, c);
				if (t.second == false) {
					continue;
				}
				if (!fits(sf, t.first)) {
					continue;
				}
				int v = 0;
				for (; v < ans.size(); v++) {
					if (ans[v] == t.first) {
						t.first.SetTypeS(ans[v].GetType());
						sf[i].edge_points.insert(t.first);
						sf[j].edge_points.insert(t.first);
						sf[k].edge_points.insert(t.first);
						break;
					}
				}
				if (v == ans.size()) {
					if (v < 26) {
						t.first.SetTypeC(static_cast<char>(v + 'A'));
					}
					else {
						t.first.SetTypeC((char)(v / 26 + 'A' - 1));
						t.first.AddToType((char)(v % 26 + 'A'));
					}
					sf[i].edge_points.insert(t.first);
					sf[j].edge_points.insert(t.first);
					sf[k].edge_points.insert(t.first);
					ans.push_back(t.first);
				}
			}
		}
	}
	if (!output_allowed) {
		return ans;
	}
	cout << "Number of vertices: " << ans.size() << endl;
	for (int i = 0; i < ans.size(); i++) {
		cout << ans[i].GetType() << ": " << ans[i] << endl;
	}
	return ans;
}

map<pair<string, string>, int> compose_graph(vector<Surface>& sf,const vector<CVector>& vr) {
	map<pair<string, string>, int> graph;
	for (int i = 0; i < sf.size(); i++) {
		set<CVector> t = sf[i].edge_points;
		vector<CVector> points;
		while (!t.empty()) {
			points.push_back(*(t.begin()));
			t.erase(t.begin());
		}
		for (int j = 0; j < points.size(); j++) {
			for (int k = j + 1; k < points.size(); k++) {
				CVector dir = points[k] - points[j];
				int sm = 0, bg = 0;
				for (int v = 0; v < points.size(); v++) {
					if (v == j || v == k) 
						continue;
					CVector cur = points[v] - points[j];
					CVector cross = dir ^ cur;
					CVector a = dir, b = cur;
					double len_cross = a.y * b.z - a.z * b.y
						- a.x * b.z + a.z * b.x +
						a.x * b.y - a.y * b.x;
					double angle = atan2(len_cross, cur * dir);
					if (angle < 0) {
						sm++;
					}
					else if (angle > 0) {
						bg++;
					}
				}
				if (sm == 0 || bg == 0) { 
					graph.insert(make_pair(make_pair(points[j].GetType(), points[k].GetType()), 1));
					graph.insert(make_pair(make_pair(points[k].GetType(), points[j].GetType()), 1));
					sf[i].edges.insert(points[j].GetType() + points[k].GetType());
				}
			}
		}
	}
	cout << "Adjacency matrix" << endl;
	size_t name_len = 0;
	for (int i = 0; i < vr.size(); i++) {
		name_len = max(name_len, vr[i].GetType().length());
	}
	name_len++;

	for (int i = 0; i < name_len; i++) 
		cout << " ";
	for (int i = 0; i < vr.size(); i++) {
		for (int j = 0; j < name_len - vr[i].GetType().length(); j++) {
			cout << " ";
		}
		cout << vr[i].GetType();
	}
	cout << endl;

	for (int i = 0; i < vr.size(); i++) {
		for (int j = 0; j < name_len - vr[i].GetType().length(); j++) {
			cout << " ";
		}
		cout << vr[i].GetType();
		for (int j = 0; j < vr.size(); j++) {
			for (int k = 0; k < name_len - 1; k++) {
				cout << " ";
			}
			cout << (graph.count(make_pair(vr[i].GetType(), vr[j].GetType())) ? 1 : 0);
		}
		cout << endl;
	}
	cout << endl;
	for (int i = 0; i < sf.size(); i++) {
		cout << "Face: " << sf[i] << endl;
		cout << "Edges: ";
		for (auto edge : sf[i].edges) {
			cout << edge << " ";
		}
		cout << endl << "Vertices: ";
		for (auto s : sf[i].edge_points) 
			cout << s.GetType() << " ";
		cout << endl << endl;
	}
	return graph;
}

double det(vector<Surface> const& t, int which) {
	vector<vector<pos>> m(4);
	for (int i = 0; i < 3; i++) {
		m[i].push_back(t[i].c.x);
		m[i].push_back(t[i].c.y);
		m[i].push_back(t[i].c.z);
		m[i].push_back(t[i].n);
	}
	for (int i = 0; i < 3; i++)
		m[i][which] = -t[i].n;
	return (m[0][0] * m[1][1] * m[2][2] +
		m[1][0] * m[0][2] * m[2][1] +
		m[2][0] * m[0][1] * m[1][2]) -
		(m[2][0] * m[1][1] * m[0][2] +
			m[0][0] * m[2][1] * m[1][2] +
			m[1][0] * m[0][1] * m[2][2]);
}

pair<CVector, bool> get_cross(const Surface& a, const Surface& b, const Surface& c) {
	vector<Surface> tmp = { a, b, c };
	double d0 = det(tmp, 3),
		d1 = det(tmp, 0),
		d2 = det(tmp, 1),
		d3 = det(tmp, 2);
	if (d0 == 0) {
		return make_pair(CVector(0, 0, 0), false);
	}
	return make_pair(CVector(d1 / d0, d2 / d0, d3 / d0), true);
}

bool fits_single(const Surface& s, CVector t) {
	return (s.c.x * t.x + s.c.y * t.y + s.c.z * t.z + s.n) <= 0;
}

bool fits(vector<Surface>& sf, CVector t) {
	bool ans = true;
	for (auto s : sf) {
		if (!fits_single(s, t)) {
			ans = false;
			break;
		}
	}
	return ans;
}