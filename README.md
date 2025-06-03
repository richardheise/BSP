# BSP com Triângulos 3D

Este projeto implementa uma estrutura de BSP (Binary Space Partitioning) para triângulos no espaço 3D, com suporte à verificação de interseções com segmentos de reta. O código é escrito em C++ e contém ferramentas para testes automatizados.

## Organização do Projeto

```plaintext
.
├── bsp.cpp / bsp.hpp       # Implementação da árvore BSP
├── main.cpp                # Função principal e leitura de entrada
├── Makefile                # Compilação
├── run_tests.sh            # Script de execução dos testes
├── README.md               # (Este arquivo)
├── tests/
│   ├── inputs/             # Casos de teste (.in)
│   ├── answers/            # Saídas esperadas (.out)
│   ├── outputs/            # Saídas geradas
│   └── imgs/               # Visualizações dos testes
```

## Compilação

Requisitos: `g++`, `make`

Para compilar o projeto, execute:

```bash
make
```

Isso gera o executável `bsp`.

## Execução Manual

Você pode rodar o programa com:

```bash
./bsp < tests/inputs/entrada.in > tests/outputs/saida.out
```

Use a flag `--verbose` para imprimir os dados lidos:

```bash
./bsp --verbose < entrada.in
```
## Execução de Testes

Para rodar todos os testes automaticamente:

```bash
./run_tests.sh
```

Esse script compara a saída gerada com os gabaritos e indica sucesso ou erro para cada teste.

## Observações sobre a BSP

- Cada nó da árvore representa um plano de divisão (definido por um triângulo).
- A árvore é construída recursivamente, subdividindo triângulos se necessário.
- A busca por interseções percorre apenas os ramos relevantes da árvore.
- Segmentos coplanares são tratados com projeções e coordenadas baricêntricas.
