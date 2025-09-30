#include "functions.h"
#include "globals.h"

void Spike0()
{
  synint0E[it][ii] = 0;
  synint0I[it][ii] = 0;
  synext01[itmem0][ii] = 0;

  if (w[k][ii][0] > 30.0 && spiketime0[ii] == 0){
    spiketime0[ii] = 1;
    if (n0 == 1 || ii < n_exc_pop0 * n0){
      synint0E[it][ii] = 1;
      synint0I[it][ii] = 0;
      synext01[itmem0][ii] = 1;
    }
    else{
      synint0I[it][ii] = 1;
      synint0E[it][ii] = 0;
    }
  }

  if (w[k][ii][0] < 30.0 && spiketime0[ii] == 1){
    spiketime0[ii] = 0;
  }
} 

void Spike1()
{
  synint1E[it][ii] = 0;
  synint1I[it][ii] = 0;
  synext10[itmem1][ii] = 0;

  if (w[k][ii][0] > 30.0 && spiketime1[ii] == 0){
    spiketime1[ii] = 1;
    if (n1 == 1 || ii < n_exc_pop1 * n1){
      synint1E[it][ii] = 1;
      synint1I[it][ii] = 0;
      synext10[itmem1][ii] = 1;
    }
    else{
      synint1I[it][ii] = 1;
      synint1E[it][ii] = 0;
    }
  }

  if (w[k][ii][0] < 30.0 && spiketime1[ii] == 1){
    spiketime1[ii] = 0;
  }
} 
