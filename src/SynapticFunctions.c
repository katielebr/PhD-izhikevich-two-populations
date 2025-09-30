#include "functions.h"
#include "globals.h"
#include <math.h>

void IsynExt(void)
{
  int nsyntE, nsyntI, n;
  tauxmem1 = (itime + 1) % delay1;
  tauxmem0 = (itime + 1) % delay0;
  nsyntE = 0;
  nsyntI = 0;

  if (k == 0){
    for (n = 0; n < extcon10; n++){
      nsyntE = nsyntE + synext10[tauxmem1][preext10[n][ii]];
    }
    IsynapticextE = nsyntE * gEext10;
  }

  if (k == 1){
    for (n = 0; n < extcon01; n++){
      nsyntE = nsyntE + synext01[tauxmem0][preext01[n][ii]];
    }
    IsynapticextE = nsyntE * gEext01;
  }
} 

void IsynInt(void)
{
  int nsyntE, nsyntI, n;
  taux = (itime + 1) % 2;
  nsyntE = 0;
  nsyntI = 0;

  if (k == 0){
    for (n = 0; n < intcon0; n++){
      nsyntE = nsyntE + synint0E[taux][preint0[n][ii]];
      nsyntI = nsyntI + synint0I[taux][preint0[n][ii]];
    }
    IsynapticintI = nsyntI * gI0;
    IsynapticintE = nsyntE * gE0;
  }

  if (k == 1){
    for (n = 0; n < intcon1; n++){
      nsyntE = nsyntE + synint1E[taux][preint1[n][ii]];
      nsyntI = nsyntI + synint1I[taux][preint1[n][ii]];
    }
    IsynapticintI = nsyntI * gI1;
    IsynapticintE = nsyntE * gE1;
  }
}
