import numpy as np
import matplotlib.pyplot as plt

def f_z(z):
    return 1.0 / z

r1, r2 = 1.0, np.e
theta1, theta2 = 0, np.pi / 2
N = 400

C1 = np.linspace(r1, r2, N) * np.exp(1j * theta1)
C2 = r2 * np.exp(1j * np.linspace(theta1, theta2, N))
C3 = np.linspace(r2, r1, N) * np.exp(1j * theta2)
C4 = r1 * np.exp(1j * np.linspace(theta2, theta1, N))

Z = [C1, C2, C3, C4]
W = [f_z(z) for z in Z]

fig, ax = plt.subplots(1, 2, figsize=(14, 6))
fig.suptitle(r'$w = 1/z$', fontsize=16)

for z in Z:
    ax[0].plot(z.real, z.imag, 'b', lw=2)
ax[0].set_aspect('equal')
ax[0].set_title('z-plane')
ax[0].grid(True, linestyle=':')
ax[0].set_xlim([-0.5, 3.5])
ax[0].set_ylim([-0.5, 3.5])

for w in W:
    ax[1].plot(w.real, w.imag, 'r', lw=2)
ax[1].set_aspect('equal')
ax[1].set_title('w-plane')
ax[1].grid(True, linestyle=':')
ax[1].set_xlim([-0.5, 1.5])
ax[1].set_ylim([-1.5, 0.5])

plt.tight_layout()
plt.savefig("map_1_over_z.png", dpi=300)
plt.close()
