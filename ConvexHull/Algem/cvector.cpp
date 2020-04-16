#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include "cvector.h"

using namespace std;

typedef double pos;

bool CVector::operator==(const CVector& b) const {
    return x == b.x && y == b.y && z == b.z;
}

bool CVector::operator<(const CVector& b) const {
    if (x != b.x)
        return x < b.x;
    if (y != b.y)
        return y < b.y;
    return z < b.z;
}

ostream& operator<<(ostream& os, const CVector& b) {
    os << ((fabs(b.x) < 0.000001) ? 0.0 : b.x)
        << " " << ((fabs(b.y) < 0.000001) ? 0.0 : b.y)
        << " " << ((fabs(b.z) < 0.000001) ? 0.0 : b.z);
    return os;
}

CVector CVector::operator+(const CVector& b) const {
    return CVector(x + b.x, y + b.y, z + b.z);
}

CVector CVector::operator-(const CVector& b) const {
    return CVector(x - b.x, y - b.y, z - b.z);
}

CVector CVector::operator*(const pos& c) const {
    return CVector(x * c, y * c, z * c);
}


pos CVector::operator*(const CVector& b) const {
    return x * b.x + y * b.y + z * b.z;
}

CVector CVector::operator^(const CVector& b) const {
    return CVector(
        y * b.z - z * b.y,
        -x * b.z + z * b.x,
        x * b.y - y * b.x
    );
}

string CVector::GetType() const {
    return type;
}

void CVector::SetTypeC(const char& s) {
    type.clear();
    type.push_back(s);
}
void CVector::SetTypeS(const string& s) {
    type = s;
}

void CVector::AddToType(const char& s) {
    type.push_back(s);
}
