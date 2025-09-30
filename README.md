# Izhikevich Two-Population Cortical Model

This repository contains code originally created by **Fernanda Selingardi Matias** and later modified by **Katiele Valéria Pereira Brito** as part of her PhD research. The simulator models two neuronal populations representing cortical areas using the Izhikevich neuron model.

The current implementation couples the two populations **unidirectionally** (Sender → Receiver). Within each population, neurons are either **excitatory** or **inhibitory**, allowing us to probe how cell-type composition shapes the dynamics of the receiver population. Synapses include $AMPA$ (excitatory) and $GABA_A$ (inhibitory) conductances, and background activity is driven by **Poisson noise**.

The code outputs the time evolution of the **mean membrane potential** for each population and provides analysis utilities for these signals, including variability across excitatory and inhibitory neurons.

---

## 🧠 Model Overview

The model is composed by two cortical populations in a Sender–Receiver configuration. Each population contains 500 neurons, with $80\%$ excitatory and $20\%$ inhibitory. All neurons follow the standard Izhikevich dynamics [1]:

$$
\frac{dv}{dt} = 0.04v^2 + 5v + 140 - u + I_{\text{x}}, \qquad
\frac{du}{dt} = a(bv - u).
$$

Here, $v$ is the membrane potential, $u$ is a recovery variable (capturing $K^+$ activation and $Na^+$ inactivation), and $I_{\text{x}}$ aggregates synaptic and external inputs. When $v \ge 30~\text{mV}$, the reset is applied:

$$
v \leftarrow c, \qquad u \leftarrow u + d.
$$

### Neuronal heterogeneity (Sender population)

In the **sender** population, excitatory neurons have parameters distributed as:

$$
\begin{aligned}
(a,b) &= (0.02, 0.2),\\
(c,d) &= (-65, 8) + (15,-6) \sigma^{2},
\end{aligned}
$$

and inhibitory neurons as:

$$
\begin{aligned}
(a,b) &= (0.02, 0.25) + (0.08, -0.05) \sigma,\\
(c,d) &= (-65, 2).
\end{aligned}
$$

Here, $\sigma$ is sampled independently for each neuron from a uniform distribution $\sigma \sim \mathcal{U}(0,1)$.  

### Neuronal heterogeneity (Receiver population)

We introduce heterogeneity in both inhibitory and excitatory neurons of the **receiver** population.

**Inhibitory neurons** — varied via $X_i$:

$$
a = 0.06 - X_i + \big[(0.04 + X_i) - (0.04 - X_i)\big]\sigma^2, \qquad
b = -0.625 a + 0.262.
$$

**Excitatory neurons** — varied via $X_e$ in the reset parameters:

$$
c = -55 - X_e + \big[(5 + X_e) - (10 - X_e)\big]\sigma^2, \qquad
d = -0.4 c - 18.
$$

$\sigma^2$ controls the variance of the heterogeneity; $X_i$ and $X_e$ set the heterogeneity scale for inhibitory and excitatory cells, respectively.

### Synaptic coupling

Across populations, coupling is mediated by **$AMPA$** (excitatory) conductance $g_E$. Within each population, synapses are **$AMPA$** onto excitatory neurons and **$GABA\_A$** onto inhibitory neurons; inhibitory control in the receiver is governed by synaptic conductance $g_I$.

### External drive and numerical scheme

Each neuron receives external **Poisson** input. Dynamics are integrated with the **explicit Euler** method using a fixed step of $\Delta t = 0.05~\text{ms}$.

## 🗂️ Project Structure

Izhikevich-TwoPopulation-Model/
├── src/                   # Source files
│   ├── InitialConditions.c
│   ├── Connectivity.c
│   ├── TimeIteration.c
│   ├── SynapticFunctions.c
│   ├── Integration.c
│   ├── SpikeFunctions.c
│   └── DataAnalysis.c
├── include/               # Header files
│   ├── constants.h
│   ├── functions.h
│   └── globals.h
├── main.c                 # Main program entry point
├── LICENSE                # MIT License
└── README.md             # Documentation

## 📎 Dependencies
- **Numerical Recipes** - `ran2` function for pseudo-random number generation.

## 📋 Input Parameters

| Parameter | Description | Range |
|-----------|-------------|-------|
| `gEpoisson` | Poisson input conductance | 0.0 - 1.0 |
| `Iext` | External current | 0.0 |
| `gI1` | Inhibitory conductance (receiver) | 0.5 - 9.0 |
| `gEext01` | Sender→Receiver excitatory conductance | 0.0 - 0.5 |
| `seed` | Random number generator seed | integer |
| `X` | Excitatory heterogeneity scale | -5.0 to 10.0 |
| `Xi` | Inhibitory heterogeneity scale | -0.04 to 0.04 |

## 🎮 How to Run
### 1. Compile the simulator
The code can be compiled using the g++ compiler. In the terminal, run:

```bash
g++ -I./include -lm main.c src/*.c -o saida.out
```

### 2. Execute with parameters
After compilation, the simulation is r'un with:

```bash
time ./saida.out gEpoisson Iext gI1 gEext01 seed X Xi
```
Example:

```bash
time ./saida.out 0.5 0.0 3.5 0.5 53408123 2.00 0.00
```

## 📄 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.


## 📚 References 
[1] E. M. Izhikevich (2003). *Simple model of spiking neurons*. IEEE Transactions on Neural Networks, 14(6), 1569–1572.

To use this code, please cite one of the references below:

```bibtex
@article{brito2025role,
  title={The role of inhibitory neuronal variability in modulating phase diversity between coupled networks},
  author={Brito, Katiele VP and Silva, Joana MGL and Mirasso, Claudio R and Matias, Fernanda S},
  journal={Chaos: An Interdisciplinary Journal of Nonlinear Science},
  volume={35},
  number={9},
  year={2025},
  publisher={AIP Publishing}
}

## 👥 Authors

Fernanda Selingardi Matias - Original implementation (2012)

Katiele Valéria Pereira Brito - Modifications and extensions (2022)
