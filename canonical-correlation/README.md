# Canonical correlation analysis of pulp fibre characteristics

How strongly do the properties of pulp fibres relate to the strength of the paper made from them? Using four paper properties (breaking length, elastic modulus, stress at failure, burst strength) and four fibre properties (arithmetic fibre length, long- and fine-fibre fractions, zero-span tensile), each of the six team members ran a canonical correlation analysis on a different split of the eight variables, in R.

**My case.** Set 1 contained the four paper properties plus arithmetic fibre length, and set 2 contained the remaining three fibre properties. The first three canonical correlations were 0.96, 0.79 and 0.46. The first pair shows a very strong association. Stress at failure dominates the paper-side variate, while the long- and zero-span fibre measures dominate the fibre-side variate.

**Takeaway.** The weight a variable receives in a canonical variate depends heavily on which other variables share its set, so interpreting CCA coefficients needs care.

The full report and R code are not included here.
