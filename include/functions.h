#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "constants.h"

// Declarações de todas as funções
void InitialConditions();
void InternalConnectivityMatrix();
void ExternalConnectivityMatrix();
void TimeIteration();
void IsynInt();
void IsynExt();
void Spike0();
void Spike1();
void Integration();
void DataAnalysis();

#endif // FUNCTIONS_H
