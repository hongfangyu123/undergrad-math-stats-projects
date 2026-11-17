# Cafeteria queueing simulation

How long do students wait to pay for lunch, and how many give up when the queue gets too long? A discrete-event simulation of a university cafeteria, with every customer's arrival, food selection, waiting and departure modelled explicitly.

**Part A – baseline.** Two cashiers, Poisson arrivals (2 per minute over 2 hours), lognormal food-selection time (mean 8 min), exponential service time.

**Part B – realistic lunch peak.** Three cashiers with different speeds, a non-homogeneous Poisson process (7 arrivals/min from 12:00 to 12:40, then 1.5/min), regular vs large-order customers, and balking: customers may leave when they see 10, 15 or 20+ people queueing.

Each scenario is repeated 1,000 times to get a Monte Carlo estimate with a 95% confidence interval.

| | Mean wait of served customers | Customers lost to balking |
|---|---|---|
| Part A | 5.16 min (95% CI 4.94–5.37) | – |
| Part B | 5.38 min (95% CI 5.33–5.44) | 58.2% (95% CI 57.9–58.5%) |

The average wait in Part B looks moderate only because more than half of the peak-hour crowd walks away. Served and lost customers are therefore reported separately.

**Run:** open [`cafeteria_queue_simulation.ipynb`](cafeteria_queue_simulation.ipynb) (needs numpy, pandas, matplotlib, seaborn).
