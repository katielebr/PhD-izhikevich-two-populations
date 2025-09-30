///////////////////////////////////////////////////////////////////////////////////////  
//  This reference should be cited whenever this code is used.                       //
//                                                                                   //
//  NEURONAL MODEL: Izhikevich                                                       //
//  SYNAPTIC MODEL: Chemical excitatory and inhibitory                               //
//                                                                                   //
//  COMPILE: g++ -lm main.c InitialConditions.c Connectivity.c TimeIteration.c SynapticFunctions.c Integration.c SpikeFunctions.c DataAnalysis.c ran2.c -o saida.out		          	                       //
//  RUN: time ./saida.out gEpoisson Iext gI1 gEext01 seed X Xi     	        //
//  EX:  time ./saida.out 0.5 0.0 3.5 0.5 53408123 2.00 0.00                         //
//                                                                                   //
//  variar X de -5 a 10 e Xi de -0.04 a 0.04                                         //
//                                                                                   //
//  Writen by Fernanda S. Matias 2012                                                //
//  Updated by Katiele Valéria Pereira Brito 2022                                    //
///////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "ran2.h" 
//#include "save_data.h"
#include "globals.h" 
#include "functions.h"
#include "constants.h"

double gI1 , gEext01, X,Y,Xi;

void InitialConditions(void);
void InternalConnectivityMatrix(void);
void ExternalConnectivityMatrix(void);
void TimeIteration(void);
void Spike0(void);
void Spike1(void);
void IsynInt(void);
void IsynExt(void);
void Integration(void);
float ran2(long *idum);

double gEpoisson, Iext;
double nn;
double w[npop][n0][dim], a[npop][n0], b[npop][n0], c[npop][n0], d[npop][n0]; 
double vdet, udet;                                                       
double t0poisson[npop][n1];                                                  
long seed;
double IsynapticintE, IsynapticintI;
double IsynapticextE, IsynapticextI;
double syncurE[npop][n0], syncurI[npop][n0];
int preint0[intcon0][n0], spiketime0[n0];
int preint1[intcon1][n1], spiketime1[n1];
int preext10[extcon10][n0], preext01[extcon01][n1];
int synint0E[2][n0], synint0I[2][n0];
int synint1E[2][n1], synint1I[2][n1];
int synext01[delay0][n0], synext10[delay1][n1];
int ii, it, j, k, l, n, key, taux, itime, spikepoisson, resto;
int tauxmem0, tauxmem1;
int itmem0, itmem1;
double t, dt, lambda, auxrand, auxrand2;

FILE *Vmemb;
char strVmemb[130];

FILE *Varia;
char strVaria[130];

int i;
double temp1, temp2, temp3, temp4;
int nlinhas1, nlinhas2, N, nlinhasTau, nlinhasTaui;
double *t1, *vm1, *vs1; 
double *t2, *vm2, *vs2; 
double *timeArray, *VmembArray1, *VmembArray2, *VmembArray3; 
double *mm, *ms;
int nmaximo_m, nmaximo_s, nmaximo, achei, g;
double tmax_m, tmax_s;
double v1, v2, v3;
double *T_m, Soma_Tm, Media_Tm, Sigma_Tm;
double *T_s, Soma_Ts, Media_Ts, Sigma_Ts;
double *t_m, *t_s, *v_m, *v_s;
double *ntau, *ni;
double tmaxaux_m, vmax_m, vaux;
double tmaxaux_s, vmax_s;
double *TAU, TAU1, Media_TAU, Sigma_TAU;
double Razao;
double sig1, sig2;

char strFiltro[200], strm[200], strs[200], stri[200], strTau[200], strTm[200], strTs[200], strSaida[200], strsSaida[200];

FILE *arq1;
FILE *Filtro, *Tau;
FILE *m, *s;
FILE *Tm, *Ts;
FILE *Saida, *sSaida;

////////////////////////////////////////////////////////////////
//                     MAIN STARTS HERE                       //
////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{ // Inputs
  gEpoisson = atof(argv[1]);
  Iext = atof(argv[2]);
  gI1 = atof(argv[3]);
  gEext01 = atof(argv[4]);
  seed = atoi(argv[5]);
  X = atof(argv[6]);
  Xi = atof(argv[7]);
  Y = 2 * X / 5;
  
  char basefolder[50];
  sprintf(basefolder, "x%.2f_xi%.5f", X, Xi);

  char command[100];
  sprintf(command, "mkdir -p \"%s\"", basefolder);
  system(command);

  // Files
  sprintf(strVmemb, "%s/Zvmedio%.1lf_%.1lf_gI1_%.2lf_gE%.3lf_X%.2lf_Xi%.5lf.dat",basefolder, gEpoisson, Iext, gI1, gEext01, X, Xi);
  remove(strVmemb);
  Vmemb = fopen(strVmemb, "w");

  sprintf(strFiltro, "%s/FiltroZv%.1lf_%.1lf_gI%.3lf_gE%.3lf_X%.2lf_Xi%.5lf.dat",basefolder, gEpoisson, Iext, gI1, gEext01, X, Xi);
  remove(strFiltro);
  Filtro = fopen(strFiltro, "w");

  sprintf(strTau, "%s/Tau_%.1lf_%.1lf_gI%.3lf_gE%.3lf_X%.2lf_Xi%.5lf.dat",basefolder, gEpoisson, Iext, gI1, gEext01, X, Xi);
  remove(strTau);
  Tau = fopen(strTau, "w");

  sprintf(strm, "%s/Zm_%.1lf_%.1lf_gI%.3lf_gE%.3lf_X%.2lf_Xi%.5lf.dat",basefolder, gEpoisson, Iext, gI1, gEext01, X, Xi);
  remove(strm);
  m = fopen(strm, "w");

  sprintf(strs, "%s/Zs_%.1lf_%.1lf_gI%.3lf_gE%.3lf_X%.2lf_Xi%.5lf.dat",basefolder, gEpoisson, Iext, gI1, gEext01, X, Xi);
  remove(strs);
  s = fopen(strs, "w");

  sprintf(strTm, "%s/Tm_%.1lf_%.1lf_gI%.3lf_gE%.3lf_X%.2lf_Xi%.5lf.dat",basefolder, gEpoisson, Iext, gI1, gEext01, X, Xi);
  remove(strTm);
  Tm = fopen(strTm, "w");

  sprintf(strTs, "%s/Ts_%.1lf_%.1lf_gI%.3lf_gE%.3lf_X%.2lf_Xi%.5lf.dat",basefolder, gEpoisson, Iext, gI1, gEext01, X, Xi);
  remove(strTs);
  Ts = fopen(strTs, "w");

  sprintf(strSaida, "%s/GSaida_%.1lf_%.1lf_gI%.3lf_gE%.3lf_X%.2lf_Xi%.5lf.dat",basefolder, gEpoisson, Iext, gI1, gEext01, X, Xi);
  remove(strSaida);
  Saida = fopen(strSaida, "w");

  sprintf(strsSaida, "%s/GsSaida_%.1lf_%.1lf_gI%.3lf_gE%.3lf_X%.2lf_Xi%.5lf.dat",basefolder, gEpoisson, Iext, gI1, gEext01, X, Xi);
  remove(strsSaida);
  sSaida = fopen(strsSaida, "w");

  sprintf(strVaria, "%s/GVariability_%.1lf_%.1lf_gI%.3lf_gE%.3lf_X%.2lf_Xi%.5lf.dat",basefolder, gEpoisson, Iext, gI1, gEext01, X, Xi);
  remove(strVaria);
  Varia = fopen(strVaria, "w");

  clock_t start_time, end_time;
  double elapsed_time;
  start_time = clock();

  // Functions
  InitialConditions();
  InternalConnectivityMatrix();
  ExternalConnectivityMatrix();
  TimeIteration();

  end_time = clock();
  elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
  printf("Simulation Time: %f minutes\n", elapsed_time/60.0);

  fclose(Vmemb);

  DataAnalysis();

  return 0;
} 
