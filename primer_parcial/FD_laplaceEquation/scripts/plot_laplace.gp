# Configuración del terminal para PNG
set terminal png
# Crear el directorio "graph" si no existe (esto es un workaround y no es ideal)
system "mkdir -p graph"
# Configuración del archivo de salida en el directorio "graph"
set output "graph/solucion.png" #Sobreescribe el mismo archivo, habría que cambiar el nombre.
# Configuración del título
set title "Solución de la Ecuación de Laplace"

# Etiquetas de los ejes
set xlabel "X"
set ylabel "Y"
set zlabel "Solución"

# Configuración de la vista 3D
set view 60, 30, 1, 1

# Graficar la superficie
splot data_file with pm3d title "Solución"

