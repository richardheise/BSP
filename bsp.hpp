#ifndef BSP_HPP
#define BSP_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <set>

using namespace std;

// Estrutura para representar um vetor ou ponto 3D
struct Vec3 {
  int x, y, z;

  Vec3() : x(0), y(0), z(0) {}
  Vec3(int x, int y, int z) : x(x), y(y), z(z) {}

  Vec3 operator-(const Vec3& other) const {
    return Vec3(x - other.x, y - other.y, z - other.z);
  }

  Vec3 cross(const Vec3& other) const {
    return Vec3(
      y * other.z - z * other.y,
      z * other.x - x * other.z,
      x * other.y - y * other.x
    );
  }

  int dot(const Vec3& other) const {
    return x * other.x + y * other.y + z * other.z;
  }
};

// Estrutura para representar um triângulo por índices de vértices
struct Triangle {
  int a, b, c;

  Triangle(int a, int b, int c) : a(a), b(b), c(c) {}
};

// Estrutura para representar um segmento de reta
struct Segment {
  Vec3 p1, p2;

  Segment(int xa, int ya, int za, int xb, int yb, int zb)
    : p1(xa, ya, za), p2(xb, yb, zb) {}
};

// Estrutura para representar um plano
struct Plane {
  Vec3 point;
  Vec3 normal;

  Plane() = default;
  Plane(const Vec3& p, const Vec3& n) : point(p), normal(n) {}
};

// Enum class para representar posição de um triângulo em relação ao plano
enum class Position { FRONT, BACK, SPANNING, COPLANAR };

// Estrutura principal para armazenar os dados de entrada
struct BSPData {
  vector<Vec3> points;
  vector<Triangle> triangles;
  vector<Segment> segments;

  void printPoints() const {
    cout << "Points (count: " << points.size() << "):\n";
    for (const auto& p : points)
      cout << "  (" << p.x << ", " << p.y << ", " << p.z << ")\n";
  }

  void printTriangles() const {
    cout << "Triangles (count: " << triangles.size() << "):\n";
    for (const auto& t : triangles)
      cout << "  [" << t.a << ", " << t.b << ", " << t.c << "]\n";
  }

  void printSegments() const {
    cout << "Segments (count: " << segments.size() << "):\n";
    for (const auto& s : segments)
      cout << "  (" << s.p1.x << ", " << s.p1.y << ", " << s.p1.z << ") -> ("
                << s.p2.x << ", " << s.p2.y << ", " << s.p2.z << ")\n";
  }
};

// Estrutura da árvore BSP
struct BSPNode {
  int triangle_index;               // Índice do triângulo usado para dividir
  Plane plane;                      // Plano que divide o espaço
  unique_ptr<BSPNode> front;
  unique_ptr<BSPNode> back;
};

// Função para calcular o plano de um triângulo
Plane computePlane(const Vec3& p1, const Vec3& p2, const Vec3& p3);

// Classifica um ponto em relação a um plano (frente, trás ou coplanar)
int classifyPointToPlane(const Plane& plane, const Vec3& point);

// Classifica um triângulo em relação a um plano
Position classifyTriangle(const Plane& plane, const Triangle& tri, const vector<Vec3>& points);

// Constrói a árvore BSP recursivamente
unique_ptr<BSPNode> buildBSP(const vector<Triangle>& triangles, const vector<Vec3>& points, vector<int> triangle_indices);

// Verifica se um segmento intersecta um plano
bool segmentIntersectsPlane(const Vec3& a, const Vec3& b, const Plane& plane);

// Verifica se um segmento intersecta um triângulo (versão simplificada)
bool segmentIntersectsTriangle(const Vec3& a, const Vec3& b, const Triangle& tri, const vector<Vec3>& points);

// Percorre a BSP coletando interseções entre um segmento e triângulos
void queryBSP(const BSPNode* node, const Vec3& a, const Vec3& b, const vector<Triangle>& triangles, const vector<Vec3>& points, set<int>& result);

// Função principal: processa todos os segmentos e retorna os índices dos triângulos interceptados
vector<vector<int>> processSegments(const BSPData& data);

#endif // BSP_HPP
