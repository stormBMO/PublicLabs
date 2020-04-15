#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include "cvector.h"

using namespace std;

class Surface {
public:
    CVector c;
    pos n;

    set<CVector> edge_points;
    set<string> edges;

    bool operator==(const Surface& s);

    friend ostream& operator<<(ostream& os, Surface& s);
    friend bool parallel(const Surface& a, const Surface& b);
};