#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "functions.h"
#include "globals.h"
#include "ran2.h"

void TimeIteration(void)
{
  timeArray = (double *)calloc(itimefinal, (size_t)(sizeof(double))); 
  VmembArray1 = (double *)calloc(itimefinal, (size_t)(sizeof(double))); 
  VmembArray2 = (double *)calloc(itimefinal, (size_t)(sizeof(double))); 
  VmembArray3 = (double *)calloc(itimefinal, (size_t)(sizeof(double))); 
  double vmedio[npop];

  // Initial conditions
  lambda = 1. - exp(-rsyn);
  vdet = -60.;
  udet = 0.2 * vdet;
  for (k = 0; k < npop; k++){
    if (k == 0){
      nn = n0;
    }
    if (k == 1){
      nn = n1;
    }

    for (ii = 0; ii < nn; ii++){
      w[k][ii][0] = vdet;
      w[k][ii][1] = udet;
    }
  }

//  for (ii = 0; ii < 400; ii++) // only exc
//  {
//    fprintf(Varia, "%lf %lf %lf %lf \n", c[0][ii], d[0][ii], c[1][ii], d[1][ii]);
//  }
  for (ii = 400; ii < 500; ii++){ //only inh
    fprintf(Varia, "%lf %lf %lf %lf \n", a[0][ii], b[0][ii], a[1][ii], b[1][ii]);
  }

  for (itime = 0; itime < itimefinal; itime++){ // Time iterarion
    it = itime % 2;
    itmem0 = (itime) % delay0;
    itmem1 = (itime) % delay1;
    t = (double)(1.0 * itime * dt);

    for (k = 0; k < npop; k++){ // For k in each population
      vmedio[k] = 0.0;
      for (ii = 0; ii < nn; ii++){ // For ii in each neuron
        // Synaptic inputs
        IsynapticintE = 0.0;
        IsynapticintI = 0.0;
        IsynapticextE = 0.0;
        IsynapticextI = 0.0;
        key = 0;
        if (t > t0poisson[k][ii]){
          key = 1;
        }

        if (itime > it0synint){
          if (k == 0){
            Spike0();
          }
          if (k == 1){
            Spike1();
          }
          if (itime > it0synext){
            IsynExt();
          }
          IsynInt();
        }

        // Poisson synaptic input
        if (ran2(&seed) < lambda){
          spikepoisson = 1;
        }
        else{
          spikepoisson = 0;
        }

        // TO CALCULATE THE ODE FOR EACH NEURON
        vdet = w[k][ii][0];
        udet = w[k][ii][1];

        if (vdet > 30.0){
          vdet = c[k][ii];
          udet = udet + d[k][ii];
        }
        Integration();

        w[k][ii][0] = vdet;
        w[k][ii][1] = udet;

        // Mean membrane potential
        if (n1 == 1){
          vmedio[k] = vdet;
        }
        else{
          vmedio[k] += vdet;
        }
      } // end of for ii
    }   // end of for k

    for (k = 0; k < npop; k++){
      if (k == 0){
        vmedio[k] = vmedio[k] / (1.0 * n0);
      }
      if (k == 1){
        vmedio[k] = vmedio[k] / (1.0 * n1);
      }
    }
    VmembArray1[itime] = vmedio[0];
    VmembArray2[itime] = vmedio[1];
    VmembArray3[itime] = vmedio[1];
    timeArray[itime] = t;

    if (itime >= it0print){
      fprintf(Vmemb, "%lf %lf %lf %lf \n", t, vmedio[0], vmedio[1], vmedio[1]);
    }
  } 
} 
