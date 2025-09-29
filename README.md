# Izhikevich Two-Population Cortical Model

This repository contains code originally created by Fernanda Selingardi Matias and later modified by Katiele Valéria Pereira Brito as part of her PhD research. The simulator models two neuron populations representing cortical areas, using the Izhikevich neuron model.

The current implementation connects the two populations unidirectionally. Within each population, neurons are modeled as either excitatory or inhibitory to probe how changes in cell type composition shape the dynamics of the receiver population. Synapses include AMPA (excitatory) and GABA_A (inhibitory) conductances, and background activity is driven by Poisson noise.

The code outputs the time evolution of the mean membrane potential for each population and provides analysis utilities for these signals, including measures of variability across excitatory and inhibitory neurons.



A computational neuroscience simulator modeling two cortical populations using Izhikevich neurons with unidirectional connectivity.

## 🧠 Model Overview

- **Neuronal Model**: Izhikevich
- **Synaptic Models**: AMPA (excitatory) and GABA_A (inhibitory)
- **Connectivity**: Unidirectional between populations coupled in a Sender-Receiver configuration 
- **Populations**: Two cortical areas, each population consists of $500$ neurons, with $80\%$ being excitatory and $20\%$ inhibitory.
