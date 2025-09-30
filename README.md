# Izhikevich Two-Population Cortical Model
**Citation Request**: If you use this code in your research, please cite the article.
## 📚 Associated Publication
This code accompanies the following research article:
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
```

This repository contains the computational code developed for the PhD research of Katiele Valéria Pereira Brito conducted under a cotutelle agreement between the Federal University of Alagoas (UFAL, Brazil) and the University of the Balearic Islands (UIB, Spain) in 2025. The original codebase was created by Fernanda Selingardi Matias and substantially extended for this research.

The simulator implements a two-population cortical network using the Izhikevich neuron model, specifically investigating how inhibitory neuronal variability modulates phase relationships between coupled neural populations.

The current implementation couples the two populations unidirectionally (Sender → Receiver). Within each population, neurons are either excitatory or inhibitory, allowing us to probe how cell-type composition shapes the dynamics of the receiver population. Synapses include $AMPA$ (excitatory) and $GABA_A$ (inhibitory) conductances, and background activity is driven by Poisson noise.

The code outputs the time evolution of the mean membrane potential for each population and provides analysis utilities for these signals, including variability across excitatory and inhibitory neurons.

## 🗂️ Project Structure

```
PhD-izhikevich-two-populations/
    src/                           # Source files
        InitialConditions.c        # Set initial neuronal states
        Connectivity.c             # Network connectivity
        TimeIteration.c            # Simulation loop
        SynapticFunctions.c        # Synaptic currents
        Integration.c              # ODE integration
        SpikeFunctions.c           # Spike detection & reset
        DataAnalysis.c             # Post-processing
    include/                       # Header files
        constants.h                # Model parameters
        functions.h                # Function declarations
        globals.h                  # Global variables
    main.c                         # Main program entry point
    LICENSE                        # MIT License
    .gitignore                     # Git ignore rules
    README.md                      # Documentation
```

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

## 🧠 Model Overview

The model is composed by two cortical populations in a Sender–Receiver configuration. Each population contains 500 neurons, with $80\%$ excitatory and $20\%$ inhibitory. All neurons follow the standard Izhikevich dynamics [1].

### Neuronal heterogeneity (Sender population)

In the **sender** population, inhbitory and excitatory neurons have parameters distributed as:
```bash
      for (ii = 0; ii < n0; ii++){
        t0poisson[k][ii] = 30.0 * 8.0 + 600. * ran2(&seed);
        syncurE[k][ii] = 0.0;
        syncurI[k][ii] = 0.0;
        auxrand = ran2(&seed);
        if (ii >= n_exc_pop0 * n0){  // population 1 (inhibitory neurons)
          a[k][ii] = 0.02 + (0.08 * auxrand);
          b[k][ii] = 0.25 - (0.05 * auxrand);
          c[k][ii] = -65.0;
          d[k][ii] = 2.0;
        }
        else{ // population 1 (excitatory neurons)
          a[k][ii] = 0.02;
          b[k][ii] = 0.2;
          c[k][ii] = -65.0 + (15.0 * auxrand * auxrand);
          d[k][ii] = 8.0 - (6.0 * auxrand * auxrand);
        }
      }
```
### Neuronal heterogeneity (Receiver population)

We introduce heterogeneity in both inhibitory and excitatory neurons of the **receiver** population.

```bash
for (ii = 0; ii < n1; ii++) {
    // [...] initialization code
    
    if (ii >= n_exc_pop1 * n1) {  // INHIBITORY NEURONS
        // Fast Spiking (FS) - CURRENTLY ACTIVE
        a[k][ii] = 0.10;
        b[k][ii] = 0.20;
        
        // Low-Threshold Spiking (LTS) - COMMENTED OUT
        // a[k][ii] = 0.02;
        // b[k][ii] = 0.25;
        
        // Heterogeneity Model - COMMENTED OUT  
        // a[k][ii] = 0.06 - Xi + ((0.04 + Xi) * auxrand * auxrand) - ((0.04 - Xi) * auxrand2 * auxrand2);
        // b[k][ii] = (-5 * a[k][ii] / 8) + 2.1 / 8;
        
        c[k][ii] = -65.0;
        d[k][ii] = 2.0;
    }
    else {  // EXCITATORY NEURONS
        a[k][ii] = 0.02;
        b[k][ii] = 0.2;
        
        // Heterogeneous excitatory neurons
        c[k][ii] = -55.0 - X + ((5.0 + X) * auxrand * auxrand) - ((10 - X) * auxrand2 * auxrand2);
        d[k][ii] = 4.0 + Y - ((2.0 + Y) * auxrand * auxrand) + ((4 - Y) * auxrand2 * auxrand2);
    }
}
```
### Modifications
To change inhibitory neuron type:
```bash
// For Fast Spiking (FS) neurons:
a[k][ii] = 0.10; b[k][ii] = 0.20;

// For Low-Threshold Spiking (LTS) neurons:  
a[k][ii] = 0.02; b[k][ii] = 0.25;

// For heterogeneous inhibitory neurons:
a[k][ii] = 0.06 - Xi + ((0.04 + Xi) * auxrand * auxrand) - ((0.04 - Xi) * auxrand2 * auxrand2);
b[k][ii] = (-5 * a[k][ii] / 8) + 2.1 / 8;
```
To change excitatory neuron type:
```bash
// For Intrinsically Bursting (IB) neurons:
c[k][ii] = -55.0; d[k][ii] = 4.0;

// For Chattering (CH) neurons:
c[k][ii] = -50.0; d[k][ii] = 2.0;

// For Regular Spiking (RS) neurons:
c[k][ii] = -65.0; d[k][ii] = 8.0;

// For heterogeneous excitatory neurons:
c[k][ii] = -55.0 - X + ((5.0 + X) * auxrand * auxrand) - ((10 - X) * auxrand2 * auxrand2);
d[k][ii] = 4.0 + Y - ((2.0 + Y) * auxrand * auxrand) + ((4 - Y) * auxrand2 * auxrand2);
```

### Synaptic coupling

Across populations, coupling is mediated by **$AMPA$** (excitatory) conductance $g_E$. Within each population, synapses are **$AMPA$** onto excitatory neurons and **$GABA\_A$** onto inhibitory neurons; inhibitory control in the receiver is governed by synaptic conductance $g_I$.

### External drive and numerical scheme

Each neuron receives external **Poisson** input. Dynamics are integrated with the **explicit Euler** method using a fixed step of $\Delta t = 0.05~\text{ms}$.

## 🔧 Customizing Neuron Types
**Location**: src/InitialConditions.c - Receiver Population Setup

## 📎 Dependencies
**Numerical Recipes** - `ran2` function for pseudo-random number generation.

## 📄 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🔗 Reference
[1] E. M. Izhikevich (2003). *Simple model of spiking neurons*. IEEE Transactions on Neural Networks, 14(6), 1569–1572.


## 👥 Authors

Fernanda Selingardi Matias - Original implementation (2012)

Katiele Valéria Pereira Brito - Modifications and extensions (2022)
