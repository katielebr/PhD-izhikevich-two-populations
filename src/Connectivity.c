#include "functions.h"
#include "ran2.h"
#include "globals.h"

void InternalConnectivityMatrix(void)
{
  int j, jj;

  for (j = 0; j < n0; j++){ // jj pre-synaptic neurons of neuron j (in the same pop 0)
    for (jj = 0; jj < intcon0; jj++){
      do{
        preint0[jj][j] = (int)(n0 * ran2(&seed));
      } while (preint0[jj][j] == j);
    }
  }

  for (j = 0; j < n1; j++){
    for (jj = 0; jj < intcon1; jj++){
      do{
        preint1[jj][j] = (int)(n1 * ran2(&seed));
      } while (preint1[jj][j] == j);
    }
  }
}

void ExternalConnectivityMatrix(){
  int j, jj;

  for (j = 0; j < n1; j++){ // jj pre-synaptic excitatory neurons from pop0 which send synpases to neuron j in pop 1
    for (jj = 0; jj < extcon01; jj++){
      preext01[jj][j] = (int)(n_exc_pop0 * n0 * ran2(&seed));
    }
  }

  for (j = 0; j < n0; j++){
    for (jj = 0; jj < extcon10; jj++){
      preext10[jj][j] = (int)(n_exc_pop1 * n1 * ran2(&seed));
    }
  }

  //for (j=0;j<n1;j++){ //  if there are inhibitory connections from pop 0 to pop 1
  // for (jj=0;jj<extcon01;jj++){
  //     preext01[jj][j]=(int)(n_exc_pop0*n0+(1.0-n_exc_pop0)*n0*ran2(&seed));
  //  }
  //}

}
