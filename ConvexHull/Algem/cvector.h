#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

typedef double pos;

class CVector {
private:
    string type;
public:
    pos x, y, z;

    CVector() : x(0), y(0), z(0) {};

    CVector(pos c_x, pos c_y, pos c_z) : x(c_x), y(c_y), z(c_z) {};

    bool operator == (const CVector& b) const;
    bool operator < (const CVector& b) const;

    friend ostream& operator << (ostream& os, const CVector& a);

    CVector operator + (const CVector& b) const;
    CVector operator - (const CVector& b) const;
    CVector operator * (const pos& c) const;

    pos operator * (const CVector& b) const;
    CVector operator ^ (const CVector& b) const;

    void SetTypeC(const char& s);
    void SetTypeS(const string& s);
    void AddToType(const char& s);
    string GetType() const;


};