#include <stdio.h>
#include <stdlib.h>
#include "ran2.h"
#include "constants.h"
#include "functions.h" 
#include "globals.h"

void InitialConditions()
{
  dt = 0.05;
  for (k = 0; k < npop; k++){
    if (k == 0){
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
    }
    if (k == 1){
      for (ii = 0; ii < n1; ii++){
        t0poisson[k][ii] = 30.0 * 8.0 + 600. * ran2(&seed);
        syncurE[k][ii] = 0.0;
        syncurI[k][ii] = 0.0;
        auxrand = ran2(&seed);
        auxrand2 = ran2(&seed);
        if (ii >= n_exc_pop1 * n1){  // population 2 (inhibitory neurons)
          // varying a and b
          //a[k][ii] = 0.06 - Xi + ((0.04 + Xi) * auxrand * auxrand) - ((0.04 - Xi) * auxrand2 * auxrand2);
          //b[k][ii] = (-5 * a[k][ii] / 8) + 2.1 / 8;
          
          //FS
          a[k][ii] = 0.10;
          b[k][ii] = 0.20;
          //LTS
          //a[k][ii] = 0.02;
          //b[k][ii] = 0.25;

          c[k][ii] = -65.0;
          d[k][ii] = 2.0;
        }
        else{  //population 2 (excitatory neurons)
          a[k][ii] = 0.02;
          b[k][ii] = 0.2;
          // varying c and d
          c[k][ii] = -55.0 - X + ((5.0 + X) * auxrand * auxrand) - ((10 - X) * auxrand2 * auxrand2);
          d[k][ii] = 4.0 + Y - ((2.0 + Y) * auxrand * auxrand) + ((4 - Y) * auxrand2 * auxrand2);
        }
      }
    }
  }
  for (ii = 0; ii < n0; ii++){spiketime0[ii] = 0;}
  for (ii = 0; ii < n1; ii++){spiketime1[ii] = 0;}
}
