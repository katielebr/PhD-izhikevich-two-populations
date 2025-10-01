#ifndef CONSTANTS_H
#define CONSTANTS_H

#define dim 2     	   // neuronal model dimension (4 if HH)
#define npop 2      	   // # of neuronal populations
#define n0 500     	   // # of neurons in population 0
#define n1 500 	   // # of neurons in population 1
#define itimefinal 960000 //38656000 final itime
#define it0print 00 	   //38636000	  initial printing time
#define it0synint 0	   // initial time for internal synapses
#define it0synext 0       // initial time for external synapses
#define n_exc_pop0 0.8     // % of excitatory neurons in population 0
#define n_exc_pop1 0.8     // % of excitatory neurons in population 1
#define intcon0 50 	   // # of pre-synaptic neurons (in the same pop) of each neuron in population 0
#define intcon1 50 	   // # of pre-synaptic neurons (in the same pop) of each neuron in population 1
#define extcon01 20 	   // # of pre-synaptic neurons (from pop 0) of each neuron in population 1
#define extcon10 0 	   // # of pre-synaptic neurons (from pop 1) of each neuron in population 0
#define gE0 0.5 	   // excitatory conductance in pop 0
#define gE1 0.5            // excitatory conductance in pop 1
#define gI0 4.0            // inhibitory conductance in pop 0
//#define gI1 2.0		   // inhibitory conductance in pop 1
//#define gEext01 0.5        // excitatory conductance from pop 0 to pop 1
#define gEext10 0.5        // excitatory conductance from pop 1 to pop 0
#define rsyn 0.12  	   // Poissonian rate R=rsyn/dt (for dt=0.05ms -> R=2400Hz)
#define TalsynGABA 5.60//5.60    // time decay of inhibitory synpases
#define TalsynAMPA 5.26//5.26    // time decay of excitatory synpases
#define delay0 2	   // synaptic delay from pop 0 to 1
#define delay1 2	   // synaptic delay from pop 1 to 0
#define janela 80    //4ms se //80  //8ms
#define limiarM -62
#define limiarS -62
#define largura 40.0
#define inicial 30

#endif // CONSTANTS_H
