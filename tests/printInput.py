import os
import sys
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection

def read_input(file):
    """Lê o arquivo de entrada no formato especificado."""
    try:
        n, t, l = map(int, file.readline().split())
        points = [tuple(map(int, file.readline().split())) for _ in range(n)]
        triangles = [tuple(map(int, file.readline().split())) for _ in range(t)]
        segments = [tuple(map(int, file.readline().split())) for _ in range(l)]
        return points, triangles, segments
    except Exception as e:
        print("Erro ao ler a entrada:", e)
        sys.exit(1)

def plot_view(points, triangles, segments, elev, azim, output_path, title):
    """Plota uma única vista e salva a imagem em output_path."""
    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111, projection='3d')

    # Pontos
    xs, ys, zs = zip(*points)
    ax.scatter(xs, ys, zs, c='black', marker='o', s=20, label='Points')

    # Triângulos
    for a, b, c in triangles:
        verts = [points[a - 1], points[b - 1], points[c - 1]]
        tri = Poly3DCollection([verts], alpha=0.3, facecolor='cyan', edgecolor='gray')
        ax.add_collection3d(tri)

    # Segmentos
    for xa, ya, za, xb, yb, zb in segments:
        ax.plot([xa, xb], [ya, yb], [za, zb], color='red', linewidth=2)

    # Configura visualização
    ax.view_init(elev=elev, azim=azim)
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.set_box_aspect([1, 1, 1])

    plt.title(title)
    plt.tight_layout()
    plt.savefig(output_path)
    print(f"Salvo: {output_path}")
    plt.close()

def main():
    if len(sys.argv) != 2 or sys.argv[1] in ('-h', '--help'):
        print("""\nUso:
    python3 printInput.py <arquivo_entrada.in>

Descrição:
    Este script lê um arquivo de entrada no formato BSP (pontos, triângulos e segmentos),
    gera 4 imagens em diferentes perspectivas (isométrica, eixo X, eixo Y, eixo Z)
    e salva tudo na pasta ./imgs/<nome_input_base>/.

Exemplo:
    python plot_bsp.py default.in

Saída:
    imgs/default/default.png   (isométrica)
    imgs/default/defaultX.png  (vista do eixo X)
    imgs/default/defaultY.png  (vista do eixo Y)
    imgs/default/defaultZ.png  (vista do eixo Z)
""")
        sys.exit(0)

    input_path = sys.argv[1]

    if not os.path.isfile(input_path):
        print(f"Erro: arquivo '{input_path}' não encontrado.")
        sys.exit(1)

    base_name = os.path.splitext(os.path.basename(input_path))[0]
    output_dir = os.path.join("imgs", base_name)
    os.makedirs(output_dir, exist_ok=True)

    with open(input_path) as f:
        points, triangles, segments = read_input(f)

    # Gera e salva as 4 imagens
    plot_view(points, triangles, segments, elev=30, azim=45,
              output_path=os.path.join(output_dir, f"{base_name}.png"),
              title="Vista Isométrica")

    plot_view(points, triangles, segments, elev=0, azim=0,
              output_path=os.path.join(output_dir, f"{base_name}X.png"),
              title="Vista do Eixo X")

    plot_view(points, triangles, segments, elev=0, azim=90,
              output_path=os.path.join(output_dir, f"{base_name}Y.png"),
              title="Vista do Eixo Y")

    plot_view(points, triangles, segments, elev=90, azim=0,
              output_path=os.path.join(output_dir, f"{base_name}Z.png"),
              title="Vista do Eixo Z")

if __name__ == "__main__":
    main()
