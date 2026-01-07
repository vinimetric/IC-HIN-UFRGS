#ifndef OBSCONSTRUCTOR_H
#define OBSCONSTRUCTOR_H

#include "TString.h"
#include <string>

void ObsConstructor(float Eta_gap, float HFSET_Min, float HFSET_Max, float CentFluct, float pTr_Min, float pTr_Max, string Correction, TString Name, TString Savename, string TrackSelec, float pvZ_Cut);

#endif