#ifndef BSP_HPP
#define BSP_HPP

#include <iostream>
#include <tuple>
#include <vector>
#include <memory>
#include <cmath>

using namespace std;

// Define a estrutura de um ponto 3D
typedef tuple<int, int, int> Point;

// Define a estrutura de um triângulo (índices dos vértices)
typedef tuple<int, int, int> Triangle;

// Define a estrutura de um segmento de reta (dois pontos extremos)
typedef tuple<int, int, int, int, int, int> Segment;

// Define um plano a partir de um ponto e um vetor normal (ambos inteiros)
struct Plane {
  Point point;           // Ponto no plano
  Point normal;          // Vetor normal ao plano
};

// Enum para classificar posição de triângulo em relação a um plano
enum Position { FRONT, BACK, SPANNING, COPLANAR };

// Estrutura principal para armazenar toda a entrada e opções de impressão
struct BSPData {
  vector<Point> points;
  vector<Triangle> triangles;
  vector<Segment> segments;

  // Método para imprimir pontos
  void printPoints() const {
    cout << "Points (count: " << points.size() << "):\n";
    for (const auto &[x, y, z] : points) {
      cout << "  (" << x << ", " << y << ", " << z << ")\n";
    }
  }

  // Método para imprimir triângulos
  void printTriangles() const {
    cout << "Triangles (count: " << triangles.size() << "):\n";
    for (const auto &[a, b, c] : triangles) {
      cout << "  [" << a << ", " << b << ", " << c << "]\n";
    }
  }

  // Método para imprimir segmentos
  void printSegments() const {
    cout << "Segments (count: " << segments.size() << "):\n";
    for (const auto &[xa, ya, za, xb, yb, zb] : segments) {
      cout << "  (" << xa << ", " << ya << ", " << za << ") -> ("
           << xb << ", " << yb << ", " << zb << ")\n";
    }
  }
};

// Estrutura da árvore BSP
struct BSPNode {
  Triangle plane_triangle;        // Triângulo que define o plano do nó
  Plane plane;                    // Plano calculado
  vector<Triangle> coplanar;     // Triângulos no mesmo plano
  unique_ptr<BSPNode> front;     // Subárvore à frente do plano
  unique_ptr<BSPNode> back;      // Subárvore atrás do plano
};

void printBSPTree(const unique_ptr<BSPNode> &node, int depth = 0);
unique_ptr<BSPNode> buildBSPTree(vector<Triangle> triangles, const vector<Point> &points);
Position classifyTriangle(const Triangle &tri, const Plane &plane, const vector<Point> &points);
Plane makePlane(const Triangle &tri, const vector<Point> &points);
int dot(const Point &a, const Point &b);
Point cross(const Point &a, const Point &b);
Point subtract(const Point &a, const Point &b);


#endif // BSP_HPP
