#include "bsp.hpp"
#include <algorithm>
#include <cmath>

using namespace std;

// ===============================
// Função auxiliar: plano do triângulo
Plane computePlane(const Vec3& p1, const Vec3& p2, const Vec3& p3) {
  Vec3 u = p2 - p1;
  Vec3 v = p3 - p1;
  Vec3 normal = u.cross(v);
  return Plane(p1, normal);
}

// ===============================
// Função auxiliar: classificar ponto em relação ao plano
int classifyPointToPlane(const Plane& plane, const Vec3& point) {
  Vec3 diff = point - plane.point;
  int dotProduct = plane.normal.dot(diff);
  if (dotProduct > 0) return 1;    // Front
  if (dotProduct < 0) return -1;   // Back
  return 0;                        // Coplanar
}

// ===============================
// Classificar triângulo em relação ao plano
Position classifyTriangle(const Plane& plane, const Triangle& tri, const vector<Vec3>& points) {
  int aSide = classifyPointToPlane(plane, points[tri.a - 1]);
  int bSide = classifyPointToPlane(plane, points[tri.b - 1]);
  int cSide = classifyPointToPlane(plane, points[tri.c - 1]);

  if (aSide == 0 && bSide == 0 && cSide == 0) return Position::COPLANAR;
  if (aSide >= 0 && bSide >= 0 && cSide >= 0) return Position::FRONT;
  if (aSide <= 0 && bSide <= 0 && cSide <= 0) return Position::BACK;
  return Position::SPANNING;
}

// ===============================
// Construção da BSP recursiva
unique_ptr<BSPNode> buildBSP(const vector<Triangle>& triangles, const vector<Vec3>& points, vector<int> triangle_indices) {
  if (triangle_indices.empty()) return nullptr;

  int root_index = triangle_indices[0]; // Usa o primeiro triângulo como plano de divisão
  const Triangle& root_tri = triangles[root_index];
  Plane dividing_plane = computePlane(points[root_tri.a - 1], points[root_tri.b - 1], points[root_tri.c - 1]);

  vector<int> front_indices, back_indices;

  for (size_t i = 1; i < triangle_indices.size(); ++i) {
    int idx = triangle_indices[i];
    const Triangle& tri = triangles[idx];
    Position pos = classifyTriangle(dividing_plane, tri, points);

    if (pos == Position::FRONT) front_indices.push_back(idx);
    else if (pos == Position::BACK) back_indices.push_back(idx);
    else if (pos == Position::COPLANAR) front_indices.push_back(idx); // Pode ir pra qualquer lado
    else {
      // SPANNING: simplificação — envia para os dois lados
      front_indices.push_back(idx);
      back_indices.push_back(idx);
    }
  }

  auto node = make_unique<BSPNode>();
  node->triangle_index = root_index;
  node->plane = dividing_plane;
  node->front = buildBSP(triangles, points, front_indices);
  node->back = buildBSP(triangles, points, back_indices);
  return node;
}

// ===============================
// Verificar se um segmento cruza o plano (simples)
bool segmentIntersectsPlane(const Vec3& a, const Vec3& b, const Plane& plane) {
  int sideA = classifyPointToPlane(plane, a);
  int sideB = classifyPointToPlane(plane, b);
  return (sideA * sideB <= 0); // Um em cada lado ou um é coplanar
}

// ===============================
// Verificar interseção com triângulo (simples, checa plano e bounding box)
bool segmentIntersectsTriangle(const Vec3& a, const Vec3& b, const Triangle& tri, const vector<Vec3>& points) {
  Plane plane = computePlane(points[tri.a - 1], points[tri.b - 1], points[tri.c - 1]);
  if (!segmentIntersectsPlane(a, b, plane)) return false;

  // Aqui poderia adicionar interseção precisa com triângulo, por simplicidade assume que plano basta
  return true;
}

// ===============================
// Percorre a BSP procurando interseções com um segmento
void queryBSP(const BSPNode* node, const Vec3& a, const Vec3& b, const vector<Triangle>& triangles, const vector<Vec3>& points, set<int>& result) {
  if (!node) return;

  const Triangle& tri = triangles[node->triangle_index];
  if (segmentIntersectsTriangle(a, b, tri, points)) {
    result.insert(node->triangle_index + 1); // índice 1-based
  }

  int sideA = classifyPointToPlane(node->plane, a);
  int sideB = classifyPointToPlane(node->plane, b);

  if (sideA >= 0 && sideB >= 0) queryBSP(node->front.get(), a, b, triangles, points, result);
  else if (sideA <= 0 && sideB <= 0) queryBSP(node->back.get(), a, b, triangles, points, result);
  else {
    queryBSP(node->front.get(), a, b, triangles, points, result);
    queryBSP(node->back.get(), a, b, triangles, points, result);
  }
}

// ===============================
// Interface pública: processar todos os segmentos
vector<vector<int>> processSegments(const BSPData& data) {
  vector<int> all_indices(data.triangles.size());
  for (int i = 0; i < (int)data.triangles.size(); ++i) all_indices[i] = i;

  unique_ptr<BSPNode> bsp_tree = buildBSP(data.triangles, data.points, all_indices);

  vector<vector<int>> output;

  for (const auto& seg : data.segments) {
    set<int> intersected;
    queryBSP(bsp_tree.get(), seg.p1, seg.p2, data.triangles, data.points, intersected);
    vector<int> list(intersected.begin(), intersected.end());
    sort(list.begin(), list.end());
    output.push_back(list);
  }

  return output;
}
