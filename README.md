# Izhikevich Two-Population Cortical Model

This repository contains code originally created by Fernanda Selingardi Matias and later modified by Katiele Valéria Pereira Brito as part of her PhD research. The simulator models two neuron populations representing cortical areas, using the Izhikevich neuron model.

The current implementation connects the two populations unidirectionally. Within each population, neurons are modeled as either excitatory or inhibitory to probe how changes in cell type composition shape the dynamics of the receiver population. Synapses include AMPA (excitatory) and GABA_A (inhibitory) conductances, and background activity is driven by Poisson noise.

The code outputs the time evolution of the mean membrane potential for each population and provides analysis utilities for these signals, including measures of variability across excitatory and inhibitory neurons.



A computational neuroscience simulator modeling two cortical populations using Izhikevich neurons with unidirectional connectivity.

## 🧠 Model Overview

In our model we explore two cortical populations unidirectionally coupled in a Sender-Receiver configuration using the Izhikevich model. Each population consists of $500$ neurons, with $80\%$ being excitatory and $20\%$ inhibitory. All neurons are described by[1]:

\begin{align*}
\frac{dv}{dt} &= 0.04v^2 + 5v + 140 - u + I_{total} \\
\frac{du}{dt} &= a(bv - u)
\end{align*}
where, $v$ represents the membrane potential, and $u$ is the recovery variable, which accounts for the activation of K$^+$ and inactivation of Na$^+$ ionic currents. $I_x$ denotes the synaptic currents resulting from interactions with other neurons and external inputs. If $v\geq30$ mV, $v$ is reset to $c$ and $u$ to $u+d$, ensuring all neurons have the same magnitude.

Both populations exhibit neuronal heterogeneity, but in the receiver population, we vary the distribution of parameters that determine neuronal firing patterns. Specifically, we manipulate $X_i$ in the inhibitory neurons :

\begin{eqnarray}
a &=& 0.06 - X_i + [(0.04 + X_i) - (0.04 - X_i)]\sigma^2, \label{eq:a}\\
b &=& -0.625 a + 0.262. \label{eq:b}
\end{eqnarray}

And we also investigated the impact of neuronal variability within the recipient population, we utilize distinct values of (c, d) for the excitatory neurons:
\begin{eqnarray}
  c &=& -55-X_e+[(5+X_e)-(10-X_e)]\sigma^2, \label{eq:c} \\
  d &=& -0.4c -18. \label{eq:d}
\end{eqnarray}

The connections between populations is modulated by excitatory conductance mediated by AMPA($g_E$), and inside the populations the conncetions are mediated by AMPA to excitatory neurons and GABA_A to inhibitory neurons. In the receiver population, the inhibition is controlled by synaptic conductance $g_I$.

Each neuron receives external stimulation via Poisson noise. The dynamics are solved numerically using the Euler method with a fixed step of 0.05.
