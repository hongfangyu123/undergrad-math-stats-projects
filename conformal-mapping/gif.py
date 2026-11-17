import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

def f_z(z):
    return 1.0 / z

r1, r2 = 1.0, np.e
theta1, theta2 = 0, np.pi / 2

GRID_R = 5
GRID_T = 5
PTS = 400

Z_grid = []

for r in np.linspace(r1, r2, GRID_R):
    Z_grid.append(r * np.exp(1j * np.linspace(theta1, theta2, PTS)))

for t in np.linspace(theta1, theta2, GRID_T):
    Z_grid.append(np.linspace(r1, r2, PTS) * np.exp(1j * t))

fig, ax = plt.subplots(figsize=(6, 6))
ax.set_aspect('equal')
ax.set_xlim([-2.5, 2.5])
ax.set_ylim([-2.5, 2.5])
ax.grid(True, linestyle=':')

lines = []
for i in range(len(Z_grid)):
    style = 'b-' if i < GRID_R else 'r--'
    line, = ax.plot([], [], style, lw=1)
    lines.append(line)

FRAMES = 80

def animate(i):
    t = np.sin(np.pi * i / (FRAMES - 1))**2
    for k, Z in enumerate(Z_grid):
        W = (1 - t) * Z + t * f_z(Z)
        lines[k].set_data(W.real, W.imag)
    ax.set_title(rf'$w_t=(1-t)z+t/z,\ t={t:.2f}$')
    return lines

ani = FuncAnimation(fig, animate, frames=FRAMES, blit=True)
ani.save("map_1_over_z.gif", writer="pillow", fps=25)
plt.close()
