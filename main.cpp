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

#include "bsp.hpp"
#include <iostream>
#include <string>

using namespace std;

// Função para ler os dados da entrada padrão
BSPData readInput() {
  BSPData data;
  int n, t, l;
  cin >> n >> t >> l;

  // Lê os pontos
  for (int i = 0; i < n; ++i) {
    int x, y, z;
    cin >> x >> y >> z;
    data.points.emplace_back(x, y, z);
  }

  // Lê os triângulos
  for (int i = 0; i < t; ++i) {
    int a, b, c;
    cin >> a >> b >> c;
    data.triangles.emplace_back(a, b, c);
  }

  // Lê os segmentos
  for (int i = 0; i < l; ++i) {
    int xa, ya, za, xb, yb, zb;
    cin >> xa >> ya >> za >> xb >> yb >> zb;
    data.segments.emplace_back(xa, ya, za, xb, yb, zb);
  }

  return data;
}

int main(int argc, char *argv[]) {
  bool verbose = false;
  // Processa argumentos de linha de comando
  for (int i = 1; i < argc; ++i) {
    string arg = argv[i];
    if (arg == "-v" || arg == "--verbose") {
      verbose = true;
    }
  }

  BSPData data = readInput();

  if (verbose) {
    // Imprime os dados lidos de forma resumida ou detalhada
    data.printPoints();
    data.printTriangles();
    data.printSegments();
  }

  // Processa os segmentos e obtém os triângulos interceptados
  vector<vector<int>> results = processSegments(data);

  // Imprime a saída conforme especificado
  for (const auto& tri_list : results) {
    cout << tri_list.size();
    for (int idx : tri_list) {
      cout << " " << idx;
    }
    cout << "\n";
  }
  
  return 0;
}
