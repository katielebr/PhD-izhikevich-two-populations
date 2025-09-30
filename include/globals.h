#ifndef GLOBALS_H
#define GLOBALS_H
#include "constants.h"
#include <stdio.h>

// Variáveis de Simulação (TimeIteration.c)
extern double dt, Iext, gEpoisson;
extern int k, ii, j, jj, spiketime0[n0], spiketime1[n1];
extern long seed;
extern double syncurE[npop][n0], syncurI[npop][n0], t0poisson[npop][n1];
extern double a[npop][n0], b[npop][n0], c[npop][n0], d[npop][n0]; 
extern double auxrand, auxrand2;

// Variáveis de Conectividade
extern int preint0[intcon0][n0], preint1[intcon1][n1];
extern int preext01[extcon01][n1], preext10[extcon10][n0];
extern int tauxmem0, tauxmem1, taux;
extern int synext01[delay0][n0], synext10[delay1][n1];
extern int synint0E[2][n0], synint0I[2][n0], synint1E[2][n1], synint1I[2][n1];

extern double gI1 , gEext01, X, Y, Xi;
extern double lambda, vdet, udet;
extern double nn, w[npop][n0][dim];
extern int it, itime, itmem0, itmem1, key, spikepoisson;
extern double t, IsynapticintE, IsynapticintI, IsynapticextE, IsynapticextI;

// Ponteiros de dados da Simulação (Alocados em TimeIteration.c)
extern double *timeArray, *VmembArray1, *VmembArray2, *VmembArray3; 

// Contadores e Auxiliares
extern int i, l, g, achei;
extern int nlinhas1, nlinhas2, N, nlinhasTau, nlinhasTaui;
extern double temp1, temp2, temp3, temp4; 
extern int nmaximo_m, nmaximo_s, nmaximo, nmaximoi;

// Ponteiros para dados (após simulação/filtragem)
extern double *t1, *vm1, *vs1, *vi1;
extern double *t2, *vm2, *vs2, *vi2; 

// Arrays/Variáveis de Picos
extern double tmaxaux_m, vmax_m, vaux;
extern double tmaxaux_s, vmax_s;
extern double tmax_m, tmax_s;
extern double v1, v2, v3;

// Arrays/Variáveis de Períodos (T)
extern double *T_m, Soma_Tm, Media_Tm, Sigma_Tm;
extern double *T_s, Soma_Ts, Media_Ts, Sigma_Ts;
extern double *t_m, *t_s, *t_i, *v_m, *v_s;

// Arrays/Variáveis de Sincronia (TAU)
extern double *ntau, *ni;
extern double *TAU, TAU1, Media_TAU, Sigma_TAU, Razao;
extern double *timeFiltro, *mmFiltro, *msFiltro;

// Variáveis de Arquivo
extern FILE *Vmemb, *Filtro, *Tau, *m, *s, *in, *Tm, *Ts, *Saida, *sSaida, *Varia;
extern char strVmemb[], strFiltro[], strm[], strs[], strTau[], strTm[], strTs[], strSaida[], strsSaida[], strVaria[];

#endif 
