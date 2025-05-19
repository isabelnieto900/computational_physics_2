# Simulación de la Ecuación de Onda 1D

Este proyecto resuelve numéricamente la ecuación de onda unidimensional usando el método de diferencias finitas explícitas y compara la solución numérica con la solución analítica. Además, permite visualizar los resultados y generar animaciones de la evolución de la onda.

---

## 📂 Estructura de Carpetas

```
waveEquation/
│
├── include/
│   └── waveEquation.h         # Declaraciones de funciones y constantes globales
│
├── src/
│   ├── waveEquation.cpp       # Implementación de funciones principales
│   └── waveEquationMain.cpp   # Función main y flujo principal del programa
│
├── scripts/
│   └── plot_wave.py           # Script Python para graficar resultados
|   └── plot_wave.gp           # Script gnuplot para graficar resultados
│
├── Makefile                   # Compilación y automatización de tareas
├── dataA.dat                  # (Se genera) Datos de la solución analítica
├── dataN.dat                  # (Se genera) Datos de la solución numérica
├── onda_anim.dat              # (Se genera) Datos para animación analítica
├── onda_animN.dat             # (Se genera) Datos para animación numérica
└── ...otros archivos generados
```

---

## 📈 Problema que resuelve este código

El programa resuelve la siguiente **ecuación de onda** con condiciones iniciales y de frontera:

\[
\begin{cases}
\displaystyle \frac{\partial^2 y}{\partial t^2} = 9\, \frac{\partial^2 y}{\partial x^2} & \quad 0 < x < 4,\; t > 0 \\[2ex]
y(0, t) = y(4, t) = 0 & \quad \text{para } t \geq 0 \\[2ex]
y(x, 0) = 2 \sin(\pi x) & \quad \text{para } 0 \leq x \leq 4 \\[2ex]
\displaystyle \frac{\partial y}{\partial t}(x, 0) = 0 & \quad \text{para } 0 \leq x \leq 4
\end{cases}
\]

---

## 🚀 ¿Cómo compilar y ejecutar?

1. **Compilar:**
   ```sh
   make
   ```
   Esto genera el ejecutable `waveEquation`.

2. **Ejecutar:**
   ```sh
   ./waveEquation
   ```
   El programa te pedirá un tiempo `t` y luego:
   - Calcula la solución numérica y analítica.
   - Guarda los datos en archivos.
   - Te pregunta si quieres graficar con Gnuplot o Python.
   - Genera archivos para animaciones y crea GIFs de la evolución de la onda.

3. **Limpiar archivos generados:**
   ```sh
   make clean
   ```

---

## 🧩 Archivos principales

- **include/waveEquation.h**  
  Declaraciones de funciones, constantes globales y parámetros del problema.

- **src/waveEquation.cpp**  
  Implementa:
  - Lectura de tiempo desde consola.
  - Solución analítica.
  - Método de diferencias finitas explícitas (`solve_fdm`).
  - Guardado de datos y generación de scripts para animaciones.
  - Función para graficar resultados (elige entre Gnuplot o Python).

- **src/waveEquationMain.cpp**  
  Controla el flujo principal del programa:
  1. Solicita el tiempo.
  2. Resuelve la ecuación numéricamente.
  3. Guarda y grafica los resultados.
  4. Genera archivos y GIFs animados.

- **scripts/plot_wave.py**  
  Script en Python para graficar los datos de la solución analítica y numérica usando Matplotlib.

---

## 📊 Visualización

- **Gnuplot:**  
  El programa puede generar scripts para visualizar los resultados y crear animaciones GIF de la evolución de la onda.

- **Python:**  
  Puedes visualizar los resultados con el script `scripts/plot_wave.py`:
  ```sh
  python scripts/plot_wave.py
  ```

---

## 📝 Explicación del método numérico

Se utiliza el método explícito de diferencias finitas para aproximar la ecuación de onda.  
La fórmula de actualización es:

\[
y_i^{n+1} = 2y_i^n - y_i^{n-1} + r^2 (y_{i+1}^n - 2y_i^n + y_{i-1}^n)
\]
donde \( r = \frac{c \Delta t}{\Delta x} \).

Se imponen condiciones de frontera (bordes en cero) y se parte de una condición inicial tipo seno.

---

## 🛠️ Parámetros importantes

- `c`: velocidad de la onda (por defecto 3.0)
- `L`: longitud del dominio (por defecto 4.0)
- `Nn`: número de nodos para la malla numérica (por defecto 10)
- `Na`: número de nodos para la malla analítica (por defecto 100)
- `t_max`: tiempo máximo para animaciones (por defecto 5.0)

Puedes modificar estos valores en `waveEquation.h`.

---

## 📚 Requisitos

- **Compilador C++** (g++ recomendado)
- **Gnuplot** (opcional, para visualizar y animar)
- **Python 3** con `matplotlib` y `numpy` (opcional, para graficar con Python)

Instala dependencias de Python con:
```sh
pip install matplotlib numpy
```

---

## 👩‍💻 Autores

- Isabel Nieto
- Camilo Huertas

---

