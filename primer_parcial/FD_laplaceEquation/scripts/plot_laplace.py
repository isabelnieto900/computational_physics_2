import sys
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm
import os  # Importar la librería os

if len(sys.argv) != 2:
    print("Uso: python plot_laplace.py <nombre_archivo_datos>")
    sys.exit(1)

data_file = sys.argv[1]

try:
    data = np.loadtxt(data_file)
except FileNotFoundError:
    print(f"Error: No se pudo encontrar el archivo de datos: {data_file}")
    sys.exit(1)
except Exception as e:
    print(f"Error al leer el archivo de datos: {e}")
    sys.exit(1)

# Crear la malla de coordenadas
ny, nx = data.shape
x = np.arange(nx)
y = np.arange(ny)
X, Y = np.meshgrid(x, y)

# Crear la figura y los ejes 3D
fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')

# Graficar la superficie
surf = ax.plot_surface(X, Y, data, cmap=cm.viridis, rstride=1, cstride=1,
                       linewidth=0, antialiased=False)

# Añadir etiquetas y título
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Solución')
ax.set_title('Solución de la Ecuación de Laplace')

# Añadir barra de color
fig.colorbar(surf, shrink=0.5, aspect=5)

# Crear el directorio "graph" si no existe
graph_dir = "graph"
if not os.path.exists(graph_dir):
    os.makedirs(graph_dir)

# Guardar el gráfico como un archivo PNG
png_filename = os.path.join(graph_dir, f"solucion_{os.path.basename(data_file)}.png")  # Usar el nombre del archivo .dat
plt.savefig(png_filename)
print(f"Gráfico guardado en: {png_filename}")

# No mostrar el gráfico en pantalla
# plt.show()

