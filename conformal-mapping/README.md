# Conformal mapping: w = 1/z

Visualises how the inversion w = 1/z transforms the annular sector 1 ≤ |z| ≤ e, 0 ≤ arg z ≤ π/2. Each boundary is tracked in polar form: with z = re^{iθ}, the image is w = (1/r)e^{−iθ}, so radii are inverted and the sector is reflected into the fourth quadrant.

- [`png.py`](png.py) draws the region and its image side by side.
- [`gif.py`](gif.py) animates a polar grid deforming continuously from z to 1/z, via w_t = (1 − t)z + t/z.

Run with `python png.py` / `python gif.py` (needs numpy, matplotlib, pillow).
