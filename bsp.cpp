#include "bsp.hpp"
#include <algorithm>
#include <cmath>

using namespace std;

// ======================================================================================================================= //

Plane computePlane(const Vec3& p1, const Vec3& p2, const Vec3& p3) {
  Vec3 u = p2 - p1;
  Vec3 v = p3 - p1;
  Vec3 normal = u.cross(v);
  return Plane(p1, normal);
}

// ======================================================================================================================= //

int classifyPointToPlane(const Plane& plane, const Vec3& point) {
  Vec3 diff = point - plane.point;
  int dotProduct = plane.normal.dot(diff);
  if (dotProduct > 0) return 1;    // Front
  if (dotProduct < 0) return -1;   // Back
  return 0;                        // Coplanar
}

// ======================================================================================================================= //

Position classifyTriangle(const Plane& plane, const Triangle& tri, const vector<Vec3>& points) {
  int aSide = classifyPointToPlane(plane, points[tri.a - 1]);
  int bSide = classifyPointToPlane(plane, points[tri.b - 1]);
  int cSide = classifyPointToPlane(plane, points[tri.c - 1]);

  if (aSide == 0 && bSide == 0 && cSide == 0) return Position::COPLANAR;
  if (aSide >= 0 && bSide >= 0 && cSide >= 0) return Position::FRONT;
  if (aSide <= 0 && bSide <= 0 && cSide <= 0) return Position::BACK;
  return Position::SPANNING;
}

// ======================================================================================================================= //

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

// ======================================================================================================================= //

bool segmentTriangleCoplanarIntersect(
    const Vec3& a, const Vec3& b,
    const Vec3& p0, const Vec3& p1, const Vec3& p2,
    const Vec3& normal) {

  // Escolhe o plano de projeção com base no maior componente do vetor normal
  int axis = 0; // 0 = X, 1 = Y, 2 = Z
  if (abs(normal.y) > abs(normal.x)) axis = 1;
  if (abs(normal.z) > abs(normal.y) && abs(normal.z) > abs(normal.x)) axis = 2;

  // Função lambda para projetar um ponto em 2D
  auto project = [&](const Vec3& v) -> pair<int, int> {
    if (axis == 0) return {v.y, v.z}; // projeta em YZ
    if (axis == 1) return {v.x, v.z}; // projeta em XZ
    return {v.x, v.y};                // projeta em XY
  };

  auto a2 = project(a), b2 = project(b);
  auto p02 = project(p0), p12 = project(p1), p22 = project(p2);

  // Verifica se algum ponto do segmento está dentro do triângulo
  if (pointInTriangle2D(a2, p02, p12, p22)) return true;
  if (pointInTriangle2D(b2, p02, p12, p22)) return true;

  // Verifica interseção do segmento com as arestas do triângulo
  if (segmentsIntersect2D(a2, b2, p02, p12)) return true;
  if (segmentsIntersect2D(a2, b2, p12, p22)) return true;
  if (segmentsIntersect2D(a2, b2, p22, p02)) return true;

  return false;
}
// ======================================================================================================================= //

int orientation(pair<int, int> p, pair<int, int> q, pair<int, int> r) {
  int val = (q.second - p.second) * (r.first - q.first) -
            (q.first - p.first) * (r.second - q.second);
  if (val == 0) return 0; // colinear
  return (val > 0) ? 1 : 2; // clockwise or counterclockwise
}

bool onSegment(pair<int, int> p, pair<int, int> q, pair<int, int> r) {
  return min(p.first, r.first) <= q.first && q.first <= max(p.first, r.first) &&
         min(p.second, r.second) <= q.second && q.second <= max(p.second, r.second);
}

// ======================================================================================================================= //

bool segmentsIntersect2D(pair<int, int> p1, pair<int, int> q1,
                         pair<int, int> p2, pair<int, int> q2) {
  int o1 = orientation(p1, q1, p2);
  int o2 = orientation(p1, q1, q2);
  int o3 = orientation(p2, q2, p1);
  int o4 = orientation(p2, q2, q1);

  if (o1 != o2 && o3 != o4) return true;

  // Casos especiais
  if (o1 == 0 && onSegment(p1, p2, q1)) return true;
  if (o2 == 0 && onSegment(p1, q2, q1)) return true;
  if (o3 == 0 && onSegment(p2, p1, q2)) return true;
  if (o4 == 0 && onSegment(p2, q1, q2)) return true;

  return false;
}

// ======================================================================================================================= //

bool pointInTriangle2D(pair<int, int> p, pair<int, int> a,
                       pair<int, int> b, pair<int, int> c) {
  auto sign = [](pair<int, int> p1, pair<int, int> p2, pair<int, int> p3) {
    return (p1.first - p3.first) * (p2.second - p3.second) -
           (p2.first - p3.first) * (p1.second - p3.second);
  };

  bool b1 = sign(p, a, b) < 0;
  bool b2 = sign(p, b, c) < 0;
  bool b3 = sign(p, c, a) < 0;

  return (b1 == b2) && (b2 == b3);
}

// ======================================================================================================================= //

bool segmentIntersectsPlane(const Vec3& a, const Vec3& b, const Plane& plane) {
  int sideA = classifyPointToPlane(plane, a);
  int sideB = classifyPointToPlane(plane, b);
  return (sideA * sideB <= 0); // Um em cada lado ou um é coplanar
}

// ======================================================================================================================= //

bool segmentIntersectsTriangle(const Vec3& a, const Vec3& b, const Triangle& tri, const vector<Vec3>& points) {
  const Vec3& p0 = points[tri.a - 1];
  const Vec3& p1 = points[tri.b - 1];
  const Vec3& p2 = points[tri.c - 1];

  // Plano do triângulo
  Vec3 normal = (p1 - p0).cross(p2 - p0);

  Vec3 ab = b - a;
  float denom = normal.dot(ab);

  // Segmento é paralelo ao plano
  if (denom == 0) {
    // Verifica se o segmento está contido no plano
    if (normal.dot(a - p0) != 0) return false;

    // Segmento está no plano: checar interseção 2D
    return segmentTriangleCoplanarIntersect(a, b, p0, p1, p2, normal);
  }

  // Calcula o ponto de interseção com o plano
  float t = normal.dot(p0 - a) / denom;

  // Se t está fora do segmento [0,1], não intersecta
  if (t < 0 || t > 1) return false;

  // Define a interseção como ponto com coordenadas reais
  double ix = a.x + (b.x - a.x) * t;
  double iy = a.y + (b.y - a.y) * t;
  double iz = a.z + (b.z - a.z) * t;
  Vec3 intersection = Vec3(static_cast<int>(round(ix)), static_cast<int>(round(iy)), static_cast<int>(round(iz)));

  // Verifica se o ponto está dentro do triângulo usando coordenadas baricêntricas
  Vec3 v0 = p1 - p0;
  Vec3 v1 = p2 - p0;
  Vec3 v2 = intersection - p0;

  float d00 = v0.dot(v0);
  float d01 = v0.dot(v1);
  float d02 = v0.dot(v2);
  float d11 = v1.dot(v1);
  float d12 = v1.dot(v2);

  float denomBary = d00 * d11 - d01 * d01;
  if (denomBary == 0) return false;

  float u = (d11 * d02 - d01 * d12) / denomBary;
  float v = (d00 * d12 - d01 * d02) / denomBary;

  return (u >= 0) && (v >= 0) && (u + v <= 1);
}


// ======================================================================================================================= //

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

// ======================================================================================================================= //

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
