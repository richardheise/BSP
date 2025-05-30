#include "bsp.hpp"

using namespace std;

// Função para subtrair dois pontos
Point subtract(const Point &a, const Point &b) {
  return Point(get<0>(a) - get<0>(b),
               get<1>(a) - get<1>(b),
               get<2>(a) - get<2>(b));
}

// Produto vetorial entre dois vetores
Point cross(const Point &a, const Point &b) {
  return Point(get<1>(a) * get<2>(b) - get<2>(a) * get<1>(b),
               get<2>(a) * get<0>(b) - get<0>(a) * get<2>(b),
               get<0>(a) * get<1>(b) - get<1>(a) * get<0>(b));
}

// Produto escalar
int dot(const Point &a, const Point &b) {
  return get<0>(a) * get<0>(b) +
         get<1>(a) * get<1>(b) +
         get<2>(a) * get<2>(b);
}

// Calcula o plano definido por um triângulo
Plane makePlane(const Triangle &tri, const vector<Point> &points) {
  Point p0 = points[get<0>(tri) - 1];
  Point p1 = points[get<1>(tri) - 1];
  Point p2 = points[get<2>(tri) - 1];
  Point v1 = subtract(p1, p0);
  Point v2 = subtract(p2, p0);
  Point n = cross(v1, v2); // normal
  return Plane{p0, n};
}

// Classifica um triângulo em relação a um plano
Position classifyTriangle(const Triangle &tri, const Plane &plane, const vector<Point> &points) {
  int front = 0, back = 0;

  for (int i = 0; i < 3; ++i) {
    Point p = points[get<0>(tri) - 1 + i];
    Point vec = subtract(p, plane.point);
    int d = dot(vec, plane.normal);
    if (d > 0) front++;
    else if (d < 0) back++;
  }

  if (front > 0 && back > 0) return SPANNING;
  if (front > 0) return FRONT;
  if (back > 0) return BACK;
  return COPLANAR;
}

// Constrói recursivamente a árvore BSP
unique_ptr<BSPNode> buildBSPTree(vector<Triangle> triangles, const vector<Point> &points) {
  if (triangles.empty()) return nullptr;

  Triangle pivot = triangles[0];
  Plane pivotPlane = makePlane(pivot, points);

  vector<Triangle> coplanar, front, back;

  for (const Triangle &tri : triangles) {
    Position pos = classifyTriangle(tri, pivotPlane, points);
    if (pos == COPLANAR)
      coplanar.push_back(tri);
    else if (pos == FRONT)
      front.push_back(tri);
    else if (pos == BACK)
      back.push_back(tri);
    else {
      // SPANNING não é tratado para simplicidade
      // Pode-se particionar o triângulo nesse caso
      coplanar.push_back(tri); // por ora considera coplanar
    }
  }

  auto node = make_unique<BSPNode>();
  node->plane_triangle = pivot;
  node->plane = pivotPlane;
  node->coplanar = coplanar;
  node->front = buildBSPTree(front, points);
  node->back = buildBSPTree(back, points);

  return node;
}

// Função auxiliar para imprimir a árvore BSP
void printBSPTree(const unique_ptr<BSPNode> &node, int depth) {
  if (!node) return;
  string indent(depth * 2, ' ');
  cout << indent << "Node at depth " << depth << " with " << node->coplanar.size() << " coplanar triangles\n";
  printBSPTree(node->front, depth + 1);
  printBSPTree(node->back, depth + 1);
}
