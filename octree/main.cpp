#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <cmath>

#ifndef OCTREE_H
#define OCTREE_H


using namespace std;

struct Point {
    int x;
    int y;
    int z;

    Point() : x(0), y(0), z(0) {}
    Point(int a, int b, int c) : x(a), y(b), z(c) {}
};


class Octree {
private:
    Octree* children[8];
    Point* points;

    // bottomLeft y h definen el espacio(cubo más grande)
    Point bottomLeft = Point(-10,-10,-10);
    double height = 20;

    int nPoints; // puntos ingresados.

public:
    Octree() = default;
    bool exist(const Point&);
    void insert(const Point&);
    Point find_closest(const Point& punto, const Octree& octree) ;
};
Point Octree::find_closest(const Point& punto, const Octree& octree) {

    if (octree.nPoints == 0) {
        return Point();
    }

    double distaciac1 = numeric_limits<double>::max();

    if (octree.children[0] == nullptr) {
        Point puntoc1 = octree.points[0];
        double distanciac2 = sqrt((puntoc1.x - punto.x) * (puntoc1.x - punto.x) + (puntoc1.y - punto.y) * (puntoc1.y - punto.y) + (puntoc1.z - punto.z) * (puntoc1.z - punto.z));

        for (int i = 1; i < octree.nPoints; ++i) {
            double distance = sqrt((octree.points[i].x - punto.x) * (octree.points[i].x - punto.x) + (octree.points[i].y - punto.y) * (octree.points[i].y - punto.y) + (octree.points[i].z - punto.z) * (octree.points[i].z - punto.z));

            if (distance < distanciac2) {
                puntoc1 = octree.points[i];
                distanciac2 = distance;
            }
        }

        return puntoc1;
    }

    int octant = 0;

    if (punto.x >= octree.bottomLeft.x + octree.height / 2)
        octant |= 1;
    if (punto.y >= octree.bottomLeft.y + octree.height / 2)
        octant |= 2;
    if (punto.z >= octree.bottomLeft.z + octree.height / 2)
        octant |= 4;

    Point puntoc2 = find_closest(punto, *(octree.children[octant]));

    for (int i = 0; i < 8; ++i) {
        if (i != octant && octree.children[i] != nullptr) {
            Point puntoenc = find_closest(punto, *(octree.children[i]));
            double distaciapuntoenc = sqrt((puntoenc.x - punto.x) * (puntoenc.x - punto.x) + (puntoenc.y - punto.y) * (puntoenc.y - punto.y) + (puntoenc.z - punto.z) * (puntoenc.z - punto.z));

            if (distaciapuntoenc < distaciac1) {
                puntoc2 = puntoenc;
                distaciac1 = distaciapuntoenc;
            }
        }
    }
    return puntoc2;
}

bool Octree::exist(const Point& p) {

    if (p.x < bottomLeft.x || p.x >= bottomLeft.x + height || p.y < bottomLeft.y || p.y >= bottomLeft.y + height || p.z < bottomLeft.z || p.z >= bottomLeft.z + height) {
        return false;
    }

    if (nPoints > 0 && children[0] != nullptr) {

        int octant = 0;

        if (p.x >= bottomLeft.x + height / 2)
            octant |= 1;
        if (p.y >= bottomLeft.y + height / 2)
            octant |= 2;
        if (p.z >= bottomLeft.z + height / 2)
            octant |= 4;

        return children[octant]->exist(p);
    }

    if (nPoints > 0) {
        for (int i = 0; i < nPoints; ++i) {
            if (points[i].x == p.x && points[i].y == p.y && points[i].z == p.z) {
                return true;
            }
        }
    }
    return false;
}

void Octree::insert(const Point& p) {

    if (p.x < bottomLeft.x || p.x >= bottomLeft.x + height ||p.y < bottomLeft.y || p.y >= bottomLeft.y + height ||p.z < bottomLeft.z || p.z >= bottomLeft.z + height) {

    }

    if (nPoints == 0) {

        nPoints = 1;
        points = new Point[1];
        points[0] = p;
    }

    if (nPoints > 0 && nPoints < 8 && children[0] == nullptr) {
        Point* newPoints = new Point[nPoints + 1];

        for (int i = 0; i < nPoints; ++i) {
            newPoints[i] = points[i];
        }

        newPoints[nPoints] = p;
        delete[] points;
        points = newPoints;
        nPoints++;
    }

    if (nPoints == 8 && children[0] == nullptr) {

        for (int i = 0; i < 8; ++i) {
            children[i] = new Octree();
        }

        for (int i = 0; i < nPoints; ++i) {
            int octant = 0;
            if (points[i].x >= bottomLeft.x + height / 2) octant |= 1;
            if (points[i].y >= bottomLeft.y + height / 2) octant |= 2;
            if (points[i].z >= bottomLeft.z + height / 2) octant |= 4;
            children[octant]->insert(points[i]);
        }

        int octant = 0;

        if (p.x >= bottomLeft.x + height / 2) octant |= 1;
        if (p.y >= bottomLeft.y + height / 2) octant |= 2;
        if (p.z >= bottomLeft.z + height / 2) octant |= 4;

        children[octant]->insert(p);
        delete[] points;
        points = nullptr;
        nPoints = 0;
    }
    if (nPoints == 0 && children[0] != nullptr) {
        int octant = 0;

        if (p.x >= bottomLeft.x + height / 2)
            octant |= 1;
        if (p.y >= bottomLeft.y + height / 2)
            octant |= 2;
        if (p.z >= bottomLeft.z + height / 2)
            octant |= 4;

        children[octant]->insert(p);
    }
}
#endif
int main() {
    Octree octree;

    Point point1(-122, 177, -71);
    Point point2(260, 236, -8);
    Point point3(-135, 28, -41);
    Point point4(248, 283, -23);
    Point point5(214, 218, -5);


    octree.insert(point1);
    octree.insert(point2);
    octree.insert(point3);
    octree.insert(point4);
    octree.insert(point5);


    if (octree.exist(point2) == true) {
        cout << "existe" << endl;
    }
    else
        cout << "no existe" << endl;


    return 0;
      }