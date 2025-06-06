/***********************************************************************
 *
 * Autor: Richard Fernando Heise Ferreira
 * Matrícula: 201900121214
 * Data: 06/2025
 * Instituição: Universidade Federal do Paraná
 * Curso: Mestrado em Segurança da Computação - PPG-Inf
 * Motivo: Trabalho 3 da disciplina de Geometria Computacional
 *
 ************************************************************************/

#ifndef BSP_HPP
#define BSP_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <set>

using namespace std;

// ======================================================================================================================= //

/**
 * Vetor ou ponto no espaço tridimensional com coordenadas inteiras.
 * Suporta operações de subtração, produto vetorial (cross) e produto escalar (dot).
 */
struct Point3D {
  int x, y, z;

  Point3D() : x(0), y(0), z(0) {}
  Point3D(int x, int y, int z) : x(x), y(y), z(z) {}

  /**
   * Operador de subtração entre vetores. Retorna o vetor resultante de this - other.
   * @param other Vetor a ser subtraído
   * @return Vetor resultante da subtração
   */
  Point3D operator-(const Point3D& other) const {
    return Point3D(x - other.x, y - other.y, z - other.z);
  }

  
  /**
   * Calcula o produto vetorial (cross product) entre este vetor e outro.
   * O resultado é um vetor perpendicular aos dois vetores.
   * @param other Outro vetor
   * @return Vetor resultante do produto vetorial
  */
  Point3D cross(const Point3D& other) const {
    return Point3D(
      y * other.z - z * other.y,
      z * other.x - x * other.z,
      x * other.y - y * other.x
    );
  }

  int dot(const Point3D& other) const {
    return x * other.x + y * other.y + z * other.z;
  }
};

// ======================================================================================================================= //

/**
 * Representa um triângulo por meio de três índices que referenciam vértices em um vetor de pontos.
 * @param a Índice do primeiro vértice
 * @param b Índice do segundo vértice
 * @param c Índice do terceiro vértice
 */
struct Triangle {
  int a, b, c;

  Triangle(int a, int b, int c) : a(a), b(b), c(c) {}
};

// ======================================================================================================================= //

/**
 * Representa um segmento de reta definido por dois pontos 3D.
 * @param xa, ya, za Coordenadas do ponto inicial
 * @param xb, yb, zb Coordenadas do ponto final
 */
struct Segment {
  Point3D p1, p2;

  Segment(int xa, int ya, int za, int xb, int yb, int zb)
    : p1(xa, ya, za), p2(xb, yb, zb) {}
};

// ======================================================================================================================= //

/**
 * Representa um plano no espaço 3D por meio de um ponto sobre o plano e o vetor normal.
 * @param p Ponto sobre o plano
 * @param n Vetor normal ao plano
 */
struct Plane {
  Point3D point;
  Point3D normal;

  Plane() = default;
  Plane(const Point3D& p, const Point3D& n) : point(p), normal(n) {}
};

// ======================================================================================================================= //

/**
 * Enumeração para representar a posição relativa de um triângulo em relação a um plano.
 * - FRONT: completamente à frente do plano
 * - BACK: completamente atrás do plano
 * - SPANNING: cruza o plano (parte na frente, parte atrás)
 * - COPLANAR: está contido no plano
 */
enum class Position { FRONT, BACK, SPANNING, COPLANAR };

// ======================================================================================================================= //

/**
 * Estrutura que agrupa todos os dados de entrada utilizados na construção da BSP e na verificação de interseções.
 * Contém vetores de pontos (vértices), triângulos (faces) e segmentos.
 */
struct BSPData {
  vector<Point3D> points;
  vector<Triangle> triangles;
  vector<Segment> segments;

  /**
   * Imprime a lista de pontos para fins de depuração.
   */
  void printPoints() const {
    cout << "Points (count: " << points.size() << "):\n";
    for (const auto& p : points)
      cout << "  (" << p.x << ", " << p.y << ", " << p.z << ")\n";
  }

  /**
   * Imprime a lista de triângulos para fins de depuração.
   */
  void printTriangles() const {
    cout << "Triangles (count: " << triangles.size() << "):\n";
    for (const auto& t : triangles)
      cout << "  [" << t.a << ", " << t.b << ", " << t.c << "]\n";
  }

  /**
   * Imprime a lista de segmentos para fins de depuração.
   */
  void printSegments() const {
    cout << "Segments (count: " << segments.size() << "):\n";
    for (const auto& s : segments)
      cout << "  (" << s.p1.x << ", " << s.p1.y << ", " << s.p1.z << ") -> ("
           << s.p2.x << ", " << s.p2.y << ", " << s.p2.z << ")\n";
  }
};

// ======================================================================================================================= //

/**
 * Representa um nó da árvore BSP (Binary Space Partitioning).
 * Cada nó armazena um índice de triângulo usado para dividir o espaço e o plano associado,
 * além de ponteiros para as subárvores da frente e de trás.
 */
struct BSPNode {
  int triangle_index;               // Índice do triângulo usado como divisor
  Plane plane;                      // Plano que divide o espaço neste nó
  unique_ptr<BSPNode> front;        // Subárvore do lado da frente
  unique_ptr<BSPNode> back;         // Subárvore do lado de trás
};

// ======================================================================================================================= //

/**
 * Calcula o plano definido por três pontos de um triângulo.
 * @param p1 Primeiro ponto
 * @param p2 Segundo ponto
 * @param p3 Terceiro ponto
 * @return O plano correspondente ao triângulo
 */
Plane computePlane(const Point3D& p1, const Point3D& p2, const Point3D& p3);

/**
 * Classifica um ponto em relação a um plano.
 * @param plane O plano de referência
 * @param point O ponto a ser classificado
 * @return 1 (frente), -1 (trás), 0 (coplanar)
 */
int classifyPointToPlane(const Plane& plane, const Point3D& point);

/**
 * Classifica um triângulo em relação a um plano.
 * @param plane O plano de referência
 * @param tri O triângulo a ser classificado
 * @param points Vetor de pontos do espaço
 * @return Posição do triângulo: FRONT, BACK, COPLANAR ou SPANNING
 */
Position classifyTriangle(const Plane& plane, const Triangle& tri, const vector<Point3D>& points);

/**
 * Constrói uma árvore BSP recursivamente a partir de triângulos.
 * @param triangles Vetor de triângulos
 * @param points Vetor de pontos
 * @param triangle_indices Índices dos triângulos a serem inseridos
 * @return Ponteiro para o nó raiz da árvore BSP construída
 */
unique_ptr<BSPNode> buildBSP(const vector<Triangle>& triangles, const vector<Point3D>& points, vector<int> triangle_indices);

/**
 * Verifica se um segmento cruza um plano.
 * @param a Ponto inicial do segmento
 * @param b Ponto final do segmento
 * @param plane O plano a ser verificado
 * @return true se o segmento intersecta o plano, false caso contrário
 */
bool segmentIntersectsPlane(const Point3D& a, const Point3D& b, const Plane& plane);

/**
 * Verifica se um segmento intersecta um triângulo.
 * @param a Ponto inicial do segmento
 * @param b Ponto final do segmento
 * @param tri Triângulo a ser testado
 * @param points Vetor de pontos
 * @return true se o segmento intersecta o triângulo
 */
bool segmentIntersectsTriangle(const Point3D& a, const Point3D& b, const Triangle& tri, const vector<Point3D>& points);

/**
 * Percorre a BSP para encontrar interseções entre um segmento e triângulos.
 * @param node Nó atual da BSP
 * @param a Ponto inicial do segmento
 * @param b Ponto final do segmento
 * @param triangles Vetor de triângulos
 * @param points Vetor de pontos
 * @param result Conjunto onde os índices dos triângulos intersectados serão inseridos
 */
void queryBSP(const BSPNode* node, const Point3D& a, const Point3D& b, const vector<Triangle>& triangles, const vector<Point3D>& points, set<int>& result);

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
bool segmentTriangleCoplanarIntersect(const Point3D& a, const Point3D& b, const Point3D& p0, const Point3D& p1, const Point3D& p2, const Point3D& normal);

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
