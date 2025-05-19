import numpy as np
import matplotlib.pyplot as plt
import os

# Cambia el path si ejecutas desde otro directorio
dataA_path = os.path.join(os.path.dirname(__file__), '..', 'dataA.dat')
dataN_path = os.path.join(os.path.dirname(__file__), '..', 'dataN.dat')

# Leer datos analíticos
xA, yA = np.loadtxt(dataA_path, unpack=True)
# Leer datos numéricos
xN, yN = np.loadtxt(dataN_path, unpack=True)

plt.figure(figsize=(8, 5))
plt.plot(xA, yA, label='Analítica', color='blue', linestyle='--')
plt.plot(xN, yN, label='Numérica', color='red')
plt.title('Onda analítica vs numérica')
plt.xlabel('x')
plt.ylabel('y(x, t)')
plt.xlim([0, 4])
plt.ylim([-2.5, 2.5])
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()