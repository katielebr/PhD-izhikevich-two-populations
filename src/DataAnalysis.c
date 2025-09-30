#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "functions.h"
#include "globals.h"
#include "constants.h" 

void DataAnalysis(void)
{
  nlinhas1 = itimefinal; 

  // Allocates memory for vectors that store data read from membrane potentials
  t1 = timeArray;
  vm1 = VmembArray1;
  vs1 = VmembArray2;

  N = nlinhas1 - 2 * janela;

  // These pointers are declared in globals.h but only allocated/used here
  double* timeFiltro = (double *)calloc(N, sizeof(double));
  double* mmFiltro   = (double *)calloc(N, sizeof(double));
  double* msFiltro   = (double *)calloc(N, sizeof(double));

  if (!timeFiltro || !mmFiltro || !msFiltro) {//|| !miFiltro
    printf("Allocation error in filter buffers.\n");
    return;
  }

  for (i = janela; i < N; i++){
    double acc_m = 0.0, acc_s = 0.0, acc_i = 0.0;
  
    // Sum for filtering (moving average)
    for (j = 1; j < janela + 1; j++){
      acc_m += vm1[i + j] + vm1[i - j];
      acc_s += vs1[i + j] + vs1[i - j];
    }

    acc_m += vm1[i];
    acc_s += vs1[i];

    double mean_m = acc_m / (2 * janela + 1);
    double mean_s = acc_s / (2 * janela + 1);

    timeFiltro[i - janela] = i * dt;
    mmFiltro[i - janela]   = mean_m;
    msFiltro[i - janela]   = mean_s;

    if (i >= it0print){
      fprintf(Filtro, "%lf %lf %lf\n", i*dt, mean_m, mean_s);//, mean_i
    }
  }
  fclose(Filtro);

  nlinhas2 = N;
  
  //////////////////////////////////////////////////////////////////////
  //////////////// Calculates mean and variance of periods /////////////
  //////////////////////////////////////////////////////////////////////

  t2  = timeFiltro;
  vm2 = mmFiltro;  
  vs2 = msFiltro; 
  //vi2 = miFiltro; 

  // sender ('m' for 'mestre' or master/sender)
  l = 0;
  nmaximo_m = 0;
  for (j = 0; j < nlinhas2; j++){
    v1 = vm2[j];
    v2 = vm2[j + 1];
    v3 = vm2[j + 2];
    if ((v1 < v2) && (v2 >= v3) && (v2 > limiarM)){
      if (t2[j + 1] - tmaxaux_m < largura){
        if (v2 > vaux){
          tmaxaux_m = t2[j + 1];
          vaux = v2;
        }
      }
      else{
        tmaxaux_m = t2[j + 1];
        vaux = v2;
        nmaximo_m++;
        fprintf(m, "%d %lf %lf \n", nmaximo_m, tmax_m, vmax_m);
      }
    }
    tmax_m = tmaxaux_m;
    vmax_m = vaux;
    l++;
  }
  fclose(m);

  // receiver ('s' for slave/receiver)
  l = 0;
  nmaximo_s = 0;
  for (j = 0; j < nlinhas2; j++){
    v1 = vs2[j];
    v2 = vs2[j + 1];
    v3 = vs2[j + 2];
    if ((v1 < v2) && (v2 >= v3) && (v2 > limiarS)){
      if (t2[j + 1] - tmaxaux_s < largura){ 
        if (v2 > vaux){
          tmaxaux_s = t2[j + 1];
          vaux = v2;
        }
      }
      else{
        tmaxaux_s = t2[j + 1];
        vaux = v2;
        nmaximo_s++;
        fprintf(s, "%d %lf %lf \n", nmaximo_s, tmax_s, vmax_s);
      }
    }
    tmax_s = tmaxaux_s;
    vmax_s = vaux;
    l++;
  }
  fclose(s);

  // sender (m)
  if (!(T_m = (double *)calloc(nmaximo_m, (size_t)(sizeof(double))))){
    printf("Allocation error in T_m.\n");
    return;
  }
  if (!(t_m = (double *)calloc(nmaximo_m, (size_t)(sizeof(double))))){
    printf("Allocation error in t_m.\n");
    return;
  }
  if (!(v_m = (double *)calloc(nmaximo_m, (size_t)(sizeof(double))))){
    printf("Allocation error in v_m.\n");
    return;
  }

  // receiver (s)
  if (!(T_s = (double *)calloc(nmaximo_s, (size_t)(sizeof(double))))){
    printf("Allocation error in T_s.\n");
    return;
  }
  if (!(t_s = (double *)calloc(nmaximo_s, (size_t)(sizeof(double))))){
    printf("Allocation error in t_s.\n");
    return;
  }
  if (!(v_s = (double *)calloc(nmaximo_s, (size_t)(sizeof(double))))){
    printf("Allocation error in v_s.\n");
    return;
  }

  //////// Opening read-only files ////////////////
  // sender (m)
  m = fopen(strm, "r");
  if (!m) { printf("Error opening file %s.\n", strm); return; }
  for (j = 0; j < nmaximo_m; j++){
    fscanf(m, "%d %lf %lf \n", &l, &t_m[j], &v_m[j]);
  }
  fclose(m);

  // receiver (s)
  s = fopen(strs, "r");
  if (!s) { printf("Error opening file %s.\n", strs); return; }
  for (j = 0; j < nmaximo_s; j++){
    fscanf(s, "%d %lf %lf \n", &l, &t_s[j], &v_s[j]);
  }
  fclose(s);

  //////////////////////////////////////////////////////////////////////
  //////////////// Calculating periods and average /////////////////////
  //////////////////////////////////////////////////////////////////////

  // sender (m)
  Soma_Tm = 0.0;
  for (j = 0; j < nmaximo_m - 2; j++){
    T_m[j] = t_m[j + 1] - t_m[j];
    if (j >= inicial){
      fprintf(Tm, "%d %lf %lf %lf \n", j, t_m[j], T_m[j], v_m[j]);
    }
  }
  for (j = inicial; j < nmaximo_m - 2; j++){
    Soma_Tm += T_m[j];
  }
  Media_Tm = Soma_Tm / (nmaximo_m - 2 - inicial);
  fclose(Tm);

  // receiver (s)
  Soma_Ts = 0.0;
  for (j = 0; j < nmaximo_s - 2; j++){
    T_s[j] = t_s[j + 1] - t_s[j];
    if (j >= inicial){
      fprintf(Ts, "%d %lf %lf %lf \n", j, t_s[j], T_s[j], v_s[j]);
    }
  }
  for (j = inicial; j < nmaximo_s - 2; j++){
    Soma_Ts += T_s[j];
  }
  Media_Ts = Soma_Ts / (nmaximo_s - 2 - inicial);
  fclose(Ts);

  //////////////////////////////////////////////////////////////////////
  //////////////// Calculating the variance of periods /////////////////
  //////////////////////////////////////////////////////////////////////

  // sender (m)
  Sigma_Tm = 0.0;
  for (i = inicial; i < nmaximo_m - 2; i++){
    Sigma_Tm += (T_m[i] - Media_Tm) * (T_m[i] - Media_Tm);
  }
  Sigma_Tm = Sigma_Tm / (nmaximo_m - 3 - inicial);
  Sigma_Tm = sqrt(Sigma_Tm);

  // receiver (s)
  Sigma_Ts = 0.0;
  for (i = inicial; i < nmaximo_s - 2; i++){
    Sigma_Ts += (T_s[i] - Media_Ts) * (T_s[i] - Media_Ts);
  }
  Sigma_Ts = Sigma_Ts / (nmaximo_s - 3 - inicial);
  Sigma_Ts = sqrt(Sigma_Ts);

  //////////////////////////////////////////////////////////////////////
  ///////// Calculates tau between master slave and return map /////////
  //////////////////////////////////////////////////////////////////////

  if (nmaximo_s > nmaximo_m){
    nmaximo = nmaximo_s;
  }
  else{
    nmaximo = nmaximo_m;
  }

  // ntau and TAU are needed for calculation
  if (!(ntau = (double *)calloc(nmaximo, (size_t)(sizeof(double))))){
    printf("Allocation error in ntau.\n");
    return;
  }
  if (!(TAU = (double *)calloc(nmaximo, (size_t)(sizeof(double))))){
    printf("Allocation error in TAU.\n");
    return;
  }

  g = 0;
  achei = 0;
  ntau[0] = 0.0;
  i = 1;
  TAU1 = 0.0;

  for (l = 0; l < nmaximo; l++){
    for (j = 0; j < nmaximo; j++){
      if ((t_s[l] > t_m[j]) && (t_s[l] < t_m[j + 1])){
        if (t_s[l] > ((t_m[j] + t_m[j + 1]) / 2)){
          TAU[l] = -t_m[j + 1] + t_s[l]; // positive if master before slave DS (negative if AS)
          ntau[i] = t_m[j + 1] - t_s[l];
          i++;
        }
        else{
          TAU[l] = -t_m[j] + t_s[l];
          ntau[i] = t_m[j] - t_s[l];
          i++;
        }
        if (l >= inicial){
          fprintf(Tau, "%d %d %lf %lf \n", j, l, TAU[l], TAU[l - 1]);
        }
        TAU1 = TAU[l];
        achei = l;
        break;
      }
    }
  }

  fclose(Tau);

  //////////////////////////////////////////////////////////////////////
  //////////////// Calculate Mean and Variance of Tau //////////////////
  //////////////////////////////////////////////////////////////////////

  if (!(Tau = fopen(strTau, "r"))){
    printf("Erro ao abrir arquivo Tau.\n");
    return;
  }

  // Checking if you read everything correctly
  nlinhasTau = 0;
  while (!feof(Tau)){
    if (fscanf(Tau, "%lf %lf %lf %lf", &temp1, &temp2, &temp3, &temp4) == 4)
      nlinhasTau++;
  }
  fclose(Tau);

  // calculation of the average
  Media_TAU = 0.0; 
  for (l = inicial; l < nlinhasTau - 1; l++){
    Media_TAU += TAU[l];
  }
  Media_TAU = Media_TAU / (nlinhasTau - 1 - inicial);

  // calculation of tau variance
  Sigma_TAU = 0.0;
  for (i = inicial; i < nlinhasTau - 1; i++){
    Sigma_TAU += (TAU[i] - Media_TAU) * (TAU[i] - Media_TAU);
  }
  Sigma_TAU = Sigma_TAU / (nlinhasTau - 2 - inicial);
  Sigma_TAU = sqrt(Sigma_TAU);
  
  // Free memory for TAU map
  free(ntau);
  ntau = NULL; 
  free(TAU);
  TAU = NULL;

  Razao = Media_TAU / Media_Tm;

  //////////////////////////////////////////////////////////////////////
  ////////////////////////// FINAL OUTPUT //////////////////////////////
  //////////////////////////////////////////////////////////////////////

  fprintf(Saida, "%lf %lf %lf %lf %lf %lf %lf %lf \n", X, Media_TAU, Sigma_TAU, Media_Tm, gI1, gEext01, gEpoisson, Xi);
  if (Sigma_TAU >= 15.0){
    fprintf(sSaida, "%lf %lf %lf 1000.0 %lf %lf %lf %lf %lf %lf %lf\n", gI1, gEext01, Media_TAU, Razao, Media_Tm, Sigma_Tm, Media_Ts, Sigma_Ts, Xi, X);
  }
  else{
    fprintf(sSaida, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n", gI1, gEext01, Media_TAU, Sigma_TAU, Razao, X, gEpoisson, Media_Tm, Sigma_Tm, Media_Ts, Sigma_Ts, Xi);
  }

  // Close remaining files
  fclose(Saida); fclose(sSaida); fclose(Varia);
  
  free(timeFiltro); free(mmFiltro); free(msFiltro);
  
  // Free memory allocated for period and peak data
  free(T_m); free(t_m); free(v_m); free(T_s); free(t_s); free(v_s);
}
