#ifndef BSP_HPP
#define BSP_HPP

#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

// Define a estrutura de um ponto 3D
typedef tuple<int, int, int> Point;

// Define a estrutura de um triângulo (índices dos vértices)
typedef tuple<int, int, int> Triangle;

// Define a estrutura de um segmento de reta (dois pontos extremos)
typedef tuple<int, int, int, int, int, int> Segment;

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
      cout << "  (" << xa << ", " << ya << ", " << za << ") -> (" << xb << ", "
           << yb << ", " << zb << ")\n";
    }
  }
};

#endif // BSP_HPP
