#include "functions.h"
#include "globals.h"

void Integration(void)
{
  syncurE[k][ii] = syncurE[k][ii] + dt * (1.0 / TalsynAMPA) * (-1.0 * syncurE[k][ii] + IsynapticextE + IsynapticintE + (double)gEpoisson * key * spikepoisson);
  syncurI[k][ii] = syncurI[k][ii] + dt * (1.0 / TalsynGABA) * (-1.0 * syncurI[k][ii] + IsynapticextI + IsynapticintI);

  vdet = vdet + dt * (0.04 * vdet * vdet + 5.0 * vdet + 140. - udet + (double)Iext * key + syncurE[k][ii] * (0.0 - vdet) + syncurI[k][ii] * (-65.0 - vdet));
  udet = udet + dt * (a[k][ii] * (b[k][ii] * vdet - udet));
}
