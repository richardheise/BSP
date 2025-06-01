#ifndef BSP_HPP
#define BSP_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <set>

using namespace std;

// ======================================================================================================================= //

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

// ======================================================================================================================= //

// Estrutura para representar um triângulo por índices de vértices
struct Triangle {
  int a, b, c;

  Triangle(int a, int b, int c) : a(a), b(b), c(c) {}
};

// ======================================================================================================================= //

// Estrutura para representar um segmento de reta
struct Segment {
  Vec3 p1, p2;

  Segment(int xa, int ya, int za, int xb, int yb, int zb)
    : p1(xa, ya, za), p2(xb, yb, zb) {}
};

// ======================================================================================================================= //

// Estrutura para representar um plano
struct Plane {
  Vec3 point;
  Vec3 normal;

  Plane() = default;
  Plane(const Vec3& p, const Vec3& n) : point(p), normal(n) {}
};

// ======================================================================================================================= //

// Enum class para representar posição de um triângulo em relação ao plano
enum class Position { FRONT, BACK, SPANNING, COPLANAR };

// ======================================================================================================================= //

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

// ======================================================================================================================= //

// Estrutura da árvore BSP
struct BSPNode {
  int triangle_index;               // Índice do triângulo usado para dividir
  Plane plane;                      // Plano que divide o espaço
  unique_ptr<BSPNode> front;
  unique_ptr<BSPNode> back;
};

// ======================================================================================================================= //

/**
 * Calcula o plano definido por três pontos de um triângulo.
 * @param p1 Primeiro ponto
 * @param p2 Segundo ponto
 * @param p3 Terceiro ponto
 * @return O plano correspondente ao triângulo
 */
Plane computePlane(const Vec3& p1, const Vec3& p2, const Vec3& p3);

/**
 * Classifica um ponto em relação a um plano.
 * @param plane O plano de referência
 * @param point O ponto a ser classificado
 * @return 1 (frente), -1 (trás), 0 (coplanar)
 */
int classifyPointToPlane(const Plane& plane, const Vec3& point);

/**
 * Classifica um triângulo em relação a um plano.
 * @param plane O plano de referência
 * @param tri O triângulo a ser classificado
 * @param points Vetor de pontos do espaço
 * @return Posição do triângulo: FRONT, BACK, COPLANAR ou SPANNING
 */
Position classifyTriangle(const Plane& plane, const Triangle& tri, const vector<Vec3>& points);

/**
 * Constrói uma árvore BSP recursivamente a partir de triângulos.
 * @param triangles Vetor de triângulos
 * @param points Vetor de pontos
 * @param triangle_indices Índices dos triângulos a serem inseridos
 * @return Ponteiro para o nó raiz da árvore BSP construída
 */
unique_ptr<BSPNode> buildBSP(const vector<Triangle>& triangles, const vector<Vec3>& points, vector<int> triangle_indices);

/**
 * Verifica se um segmento cruza um plano.
 * @param a Ponto inicial do segmento
 * @param b Ponto final do segmento
 * @param plane O plano a ser verificado
 * @return true se o segmento intersecta o plano, false caso contrário
 */
bool segmentIntersectsPlane(const Vec3& a, const Vec3& b, const Plane& plane);

/**
 * Verifica se um segmento intersecta um triângulo.
 * @param a Ponto inicial do segmento
 * @param b Ponto final do segmento
 * @param tri Triângulo a ser testado
 * @param points Vetor de pontos
 * @return true se o segmento intersecta o triângulo
 */
bool segmentIntersectsTriangle(const Vec3& a, const Vec3& b, const Triangle& tri, const vector<Vec3>& points);

/**
 * Percorre a BSP para encontrar interseções entre um segmento e triângulos.
 * @param node Nó atual da BSP
 * @param a Ponto inicial do segmento
 * @param b Ponto final do segmento
 * @param triangles Vetor de triângulos
 * @param points Vetor de pontos
 * @param result Conjunto onde os índices dos triângulos intersectados serão inseridos
 */
void queryBSP(const BSPNode* node, const Vec3& a, const Vec3& b, const vector<Triangle>& triangles, const vector<Vec3>& points, set<int>& result);

/**
 * Processa todos os segmentos e determina os triângulos que cada um intersecta.
 * @param data Estrutura contendo pontos, triângulos, segmentos e BSP construída
 * @return Vetor de vetores contendo os índices dos triângulos interceptados por cada segmento
 */
vector<vector<int>> processSegments(const BSPData& data);

/**
 * Verifica se dois segmentos 2D se intersectam.
 * @param p1 Início do primeiro segmento
 * @param q1 Fim do primeiro segmento
 * @param p2 Início do segundo segmento
 * @param q2 Fim do segundo segmento
 * @return true se há interseção
 */
bool segmentsIntersect2D(pair<int, int> p1, pair<int, int> q1, pair<int, int> p2, pair<int, int> q2);

/**
 * Verifica se o ponto q está sobre o segmento pr.
 * @param p Início do segmento
 * @param q Ponto a ser verificado
 * @param r Fim do segmento
 * @return true se q está sobre o segmento pr
 */
bool onSegment(pair<int, int> p, pair<int, int> q, pair<int, int> r);

/**
 * Retorna a orientação entre três pontos.
 * @param p Primeiro ponto
 * @param q Segundo ponto
 * @param r Terceiro ponto
 * @return 0 se colinear, 1 se horário, 2 se anti-horário
 */
int orientation(pair<int, int> p, pair<int, int> q, pair<int, int> r);

/**
 * Verifica se um segmento coplanar intersecta um triângulo.
 * @param a Ponto inicial do segmento
 * @param b Ponto final do segmento
 * @param p0 Primeiro vértice do triângulo
 * @param p1 Segundo vértice do triângulo
 * @param p2 Terceiro vértice do triângulo
 * @param normal Vetor normal do plano do triângulo
 * @return true se o segmento intersecta o triângulo no plano
 */
bool segmentTriangleCoplanarIntersect(const Vec3& a, const Vec3& b, const Vec3& p0, const Vec3& p1, const Vec3& p2, const Vec3& normal);

/**
 * Verifica se um ponto 2D está dentro de um triângulo 2D.
 * @param p Ponto a ser testado
 * @param a Primeiro vértice do triângulo
 * @param b Segundo vértice do triângulo
 * @param c Terceiro vértice do triângulo
 * @return true se o ponto está dentro do triângulo
 */
bool pointInTriangle2D(pair<int, int> p, pair<int, int> a, pair<int, int> b, pair<int, int> c);

#endif // BSP_HPP
