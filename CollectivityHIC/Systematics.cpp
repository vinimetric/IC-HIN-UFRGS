#include "ObsConstructor.h"
#include <cmath>
#include <filesystem>

double StatisticalUncertainties(double sigma_A, double sigma_B){
    double unc = sqrt(abs(pow(sigma_A, 2) - pow(sigma_B, 2)));
    return unc;
}

double QuadSum(vector<Double_t> x){
    int N = x.size();
    double acc = 0.0;
    for (int i=0; i<N; i++) acc += pow(x[i], 2);
    double result = sqrt(acc);
    return result;
}

vector<Double_t> VecQuadSum(vector<vector<Double_t>> x){
    int nVar = x.size();
    int N = x[0].size();
    vector<Double_t> v(N, 0.0);
    Double_t acc;
    
    for (int j=0; j<N; j++){
        acc = 0.0;
        for (int i=0; i<nVar; i++){
            acc += pow(x[i][j], 2);
        }
        v[j] = sqrt(acc);
    }
    return v;
}

void TrackSelection(){
    
    if (!filesystem::exists("./Data/Systematics/TrackSelectionData.root")){
        cout << "Evaluating nominal 50-60%" << endl; // Calculating nominal 50-60%
        ObsConstructor(1.0, 210.0, 375.0, 0.0, 0.5, 2.0, "noCorrec", "nominal", "./Data/Systematics/TrackSelectionData.root", "Nominal", 15.0);
        cout << "Evaluating nominal 60-70%" << endl; // Calculating nominal 60-70%
        ObsConstructor(1.0, 100.0, 210.0, 0.0, 0.5, 2.0, "noCorrec", "nominal", "./Data/Systematics/TrackSelectionData.root", "Nominal", 15.0);

        cout << "Evaluating loose 50-60%" << endl; // Calculating loose 50-60%
        ObsConstructor(1.0, 210.0, 375.0, 0.0, 0.5, 2.0, "noCorrec", "loose", "./Data/Systematics/TrackSelectionData.root", "Loose", 15.0);
        cout << "Evaluating loose 60-70%" << endl;// Calculating loose 60-70%
        ObsConstructor(1.0, 100.0, 210.0, 0.0, 0.5, 2.0, "noCorrec", "loose", "./Data/Systematics/TrackSelectionData.root", "Loose", 15.0);

        cout << "Evaluating tight 50-60%" << endl; // Calculating tight 50-60%
        ObsConstructor(1.0, 210.0, 375.0, 0.0, 0.5, 2.0, "noCorrec", "tight", "./Data/Systematics/TrackSelectionData.root", "Tight", 15.0);
        cout << "Evaluating tight 60-70%" << endl; // Calculating tight 60-70%
        ObsConstructor(1.0, 100.0, 210.0, 0.0, 0.5, 2.0, "noCorrec", "tight", "./Data/Systematics/TrackSelectionData.root", "Tight", 15.0);
    }

    TFile *f = TFile::Open("./Data/Systematics/TrackSelectionData.root", "READ");

    // Calculating v0 difference
        TGraphErrors *g_v0_55_nominal = (TGraphErrors*)f->Get("v0_55_nominal");
        TGraphErrors *g_v0_55_loose = (TGraphErrors*)f->Get("v0_55_loose");
        TGraphErrors *g_v0_55_tight = (TGraphErrors*)f->Get("v0_55_tight");
        TGraphErrors *g_v0_65_nominal = (TGraphErrors*)f->Get("v0_65_nominal");
        TGraphErrors *g_v0_65_loose = (TGraphErrors*)f->Get("v0_65_loose");
        TGraphErrors *g_v0_65_tight = (TGraphErrors*)f->Get("v0_65_tight");

        Int_t N_v0 = 2;
        Double_t *y_v0_55_nominal = g_v0_55_nominal->GetY();
        Double_t *y_v0_65_nominal = g_v0_65_nominal->GetY();
        Double_t *y_v0_55_loose = g_v0_55_loose->GetY();
        Double_t *y_v0_65_loose = g_v0_65_loose->GetY();
        Double_t *y_v0_55_tight = g_v0_55_tight->GetY();
        Double_t *y_v0_65_tight = g_v0_65_tight->GetY();
        Double_t *y_unc_v0_55_nominal = g_v0_55_nominal->GetEY();
        Double_t *y_unc_v0_65_nominal = g_v0_65_nominal->GetEY();
        Double_t *y_unc_v0_55_loose = g_v0_55_loose->GetEY();
        Double_t *y_unc_v0_65_loose = g_v0_65_loose->GetEY();
        Double_t *y_unc_v0_55_tight = g_v0_55_tight->GetEY();
        Double_t *y_unc_v0_65_tight = g_v0_65_tight->GetEY();

        vector<Double_t> vec_x_v0(N_v0, 0.0);
        vector<Double_t> vec_x_unc_v0(N_v0, 0.0);
        vector<Double_t> vec_y_v0_loose(N_v0, 0.0);
        vector<Double_t> vec_y_v0_tight(N_v0, 0.0);
        vector<Double_t> vec_y_unc_v0_loose(N_v0, 0.0);
        vector<Double_t> vec_y_unc_v0_tight(N_v0, 0.0);

        vec_x_v0[0] = 55.0; vec_x_v0[1] = 65.0;
        vec_y_v0_loose[0] = abs(y_v0_55_loose[0] - y_v0_55_nominal[0]); vec_y_v0_loose[1] = abs(y_v0_65_loose[0] - y_v0_65_nominal[0]);
        vec_y_v0_tight[0] = abs(y_v0_55_tight[0] - y_v0_55_nominal[0]); vec_y_v0_tight[1] = abs(y_v0_65_tight[0] - y_v0_65_nominal[0]);
        vec_y_unc_v0_loose[0] = StatisticalUncertainties(y_unc_v0_55_loose[0], y_unc_v0_55_nominal[0]); vec_y_unc_v0_loose[1] = StatisticalUncertainties(y_unc_v0_65_loose[0], y_unc_v0_65_nominal[0]);
        vec_y_unc_v0_tight[0] = StatisticalUncertainties(y_unc_v0_55_tight[0], y_unc_v0_55_nominal[0]); vec_y_unc_v0_tight[1] = StatisticalUncertainties(y_unc_v0_65_tight[0], y_unc_v0_65_nominal[0]);

        TGraph* gd_v0_loose = new TGraphErrors(N_v0, vec_x_v0.data(), vec_y_v0_loose.data(), vec_x_unc_v0.data(), vec_y_unc_v0_loose.data());
        TGraph* gd_v0_tight = new TGraphErrors(N_v0, vec_x_v0.data(), vec_y_v0_tight.data(), vec_x_unc_v0.data(), vec_y_unc_v0_tight.data());

    // Calculating v0(pT) difference
        TGraphErrors *g_v0pt_5060_nominal = (TGraphErrors*)f->Get("v0pt_ptref_nominal_5060");
        TGraphErrors *g_v0pt_6070_nominal = (TGraphErrors*)f->Get("v0pt_ptref_nominal_6070");
        TGraphErrors *g_v0pt_5060_loose = (TGraphErrors*)f->Get("v0pt_ptref_loose_5060");
        TGraphErrors *g_v0pt_6070_loose = (TGraphErrors*)f->Get("v0pt_ptref_loose_6070");
        TGraphErrors *g_v0pt_5060_tight = (TGraphErrors*)f->Get("v0pt_ptref_tight_5060");
        TGraphErrors *g_v0pt_6070_tight = (TGraphErrors*)f->Get("v0pt_ptref_tight_6070");

        Int_t N_v0pt = g_v0pt_5060_nominal->GetN();
        Double_t *x_v0pt = g_v0pt_5060_nominal->GetX();
        Double_t *y_v0pt_5060_nominal = g_v0pt_5060_nominal->GetY();
        Double_t *y_v0pt_6070_nominal = g_v0pt_6070_nominal->GetY();
        Double_t *y_v0pt_5060_loose = g_v0pt_5060_loose->GetY();
        Double_t *y_v0pt_6070_loose = g_v0pt_6070_loose->GetY();
        Double_t *y_v0pt_5060_tight = g_v0pt_5060_tight->GetY();
        Double_t *y_v0pt_6070_tight = g_v0pt_6070_tight->GetY();
        Double_t *y_unc_v0pt_5060_nominal = g_v0pt_5060_nominal->GetEY();
        Double_t *y_unc_v0pt_6070_nominal = g_v0pt_6070_nominal->GetEY();
        Double_t *y_unc_v0pt_5060_loose = g_v0pt_5060_loose->GetEY();
        Double_t *y_unc_v0pt_6070_loose = g_v0pt_6070_loose->GetEY();
        Double_t *y_unc_v0pt_5060_tight = g_v0pt_5060_tight->GetEY();
        Double_t *y_unc_v0pt_6070_tight = g_v0pt_6070_tight->GetEY();

        vector<Double_t> vec_x_v0pt(N_v0pt, 0.0);
        vector<Double_t> vec_x_unc_v0pt(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_5060_loose(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_6070_loose(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_5060_tight(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_6070_tight(N_v0pt, 0.0);
        vector<Double_t> vec_y_unc_v0pt_5060_loose(N_v0pt, 0.0);
        vector<Double_t> vec_y_unc_v0pt_6070_loose(N_v0pt, 0.0);
        vector<Double_t> vec_y_unc_v0pt_5060_tight(N_v0pt, 0.0);
        vector<Double_t> vec_y_unc_v0pt_6070_tight(N_v0pt, 0.0);

        for (int i=0; i<N_v0pt; i++){
            vec_x_v0pt[i] = x_v0pt[i];
            vec_y_v0pt_5060_loose[i] = abs(y_v0pt_5060_loose[i] - y_v0pt_5060_nominal[i]);
            vec_y_v0pt_6070_loose[i] = abs(y_v0pt_6070_loose[i] - y_v0pt_6070_nominal[i]);
            vec_y_v0pt_5060_tight[i] = abs(y_v0pt_5060_tight[i] - y_v0pt_5060_nominal[i]);
            vec_y_v0pt_6070_tight[i] = abs(y_v0pt_6070_tight[i] - y_v0pt_6070_nominal[i]);
            vec_y_unc_v0pt_5060_loose[i] = StatisticalUncertainties(y_unc_v0pt_5060_loose[i], y_unc_v0pt_5060_nominal[i]);
            vec_y_unc_v0pt_6070_loose[i] = StatisticalUncertainties(y_unc_v0pt_6070_loose[i], y_unc_v0pt_6070_nominal[i]);
            vec_y_unc_v0pt_5060_tight[i] = StatisticalUncertainties(y_unc_v0pt_5060_tight[i], y_unc_v0pt_5060_nominal[i]);
            vec_y_unc_v0pt_6070_tight[i] = StatisticalUncertainties(y_unc_v0pt_6070_tight[i], y_unc_v0pt_6070_nominal[i]);
        }

        TGraph* gd_v0pt_5060_loose = new TGraphErrors(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_5060_loose.data(), vec_x_unc_v0pt.data(), vec_y_unc_v0pt_5060_loose.data());
        TGraph* gd_v0pt_6070_loose = new TGraphErrors(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_6070_loose.data(), vec_x_unc_v0pt.data(), vec_y_unc_v0pt_6070_loose.data());
        TGraph* gd_v0pt_5060_tight = new TGraphErrors(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_5060_tight.data(), vec_x_unc_v0pt.data(), vec_y_unc_v0pt_5060_tight.data());
        TGraph* gd_v0pt_6070_tight = new TGraphErrors(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_6070_tight.data(), vec_x_unc_v0pt.data(), vec_y_unc_v0pt_6070_tight.data());

    // Calculating v0(pT)/v0 difference
        TGraphErrors *g_sv0pt_5060_nominal = (TGraphErrors*)f->Get("sv0pt_ptref_nominal_5060");
        TGraphErrors *g_sv0pt_6070_nominal = (TGraphErrors*)f->Get("sv0pt_ptref_nominal_6070");
        TGraphErrors *g_sv0pt_5060_loose = (TGraphErrors*)f->Get("sv0pt_ptref_loose_5060");
        TGraphErrors *g_sv0pt_6070_loose = (TGraphErrors*)f->Get("sv0pt_ptref_loose_6070");
        TGraphErrors *g_sv0pt_5060_tight = (TGraphErrors*)f->Get("sv0pt_ptref_tight_5060");
        TGraphErrors *g_sv0pt_6070_tight = (TGraphErrors*)f->Get("sv0pt_ptref_tight_6070");

        Int_t N_sv0pt = g_sv0pt_5060_nominal->GetN();
        Double_t *x_sv0pt = g_sv0pt_5060_nominal->GetX();
        Double_t *y_sv0pt_5060_nominal = g_sv0pt_5060_nominal->GetY();
        Double_t *y_sv0pt_6070_nominal = g_sv0pt_6070_nominal->GetY();
        Double_t *y_sv0pt_5060_loose = g_sv0pt_5060_loose->GetY();
        Double_t *y_sv0pt_6070_loose = g_sv0pt_6070_loose->GetY();
        Double_t *y_sv0pt_5060_tight = g_sv0pt_5060_tight->GetY();
        Double_t *y_sv0pt_6070_tight = g_sv0pt_6070_tight->GetY();
        Double_t *y_unc_sv0pt_5060_nominal = g_sv0pt_5060_nominal->GetEY();
        Double_t *y_unc_sv0pt_6070_nominal = g_sv0pt_6070_nominal->GetEY();
        Double_t *y_unc_sv0pt_5060_loose = g_sv0pt_5060_loose->GetEY();
        Double_t *y_unc_sv0pt_6070_loose = g_sv0pt_6070_loose->GetEY();
        Double_t *y_unc_sv0pt_5060_tight = g_sv0pt_5060_tight->GetEY();
        Double_t *y_unc_sv0pt_6070_tight = g_sv0pt_6070_tight->GetEY();

        vector<Double_t> vec_x_sv0pt(N_sv0pt, 0.0);
        vector<Double_t> vec_x_unc_sv0pt(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_5060_loose(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_6070_loose(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_5060_tight(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_6070_tight(N_sv0pt, 0.0);
        vector<Double_t> vec_y_unc_sv0pt_5060_loose(N_sv0pt, 0.0);
        vector<Double_t> vec_y_unc_sv0pt_6070_loose(N_sv0pt, 0.0);
        vector<Double_t> vec_y_unc_sv0pt_5060_tight(N_sv0pt, 0.0);
        vector<Double_t> vec_y_unc_sv0pt_6070_tight(N_sv0pt, 0.0);

        for (int i=0; i<N_sv0pt; i++){
            vec_x_sv0pt[i] = x_sv0pt[i];
            vec_y_sv0pt_5060_loose[i] = abs(y_sv0pt_5060_loose[i] - y_sv0pt_5060_nominal[i]);
            vec_y_sv0pt_6070_loose[i] = abs(y_sv0pt_6070_loose[i] - y_sv0pt_6070_nominal[i]);
            vec_y_sv0pt_5060_tight[i] = abs(y_sv0pt_5060_tight[i] - y_sv0pt_5060_nominal[i]);
            vec_y_sv0pt_6070_tight[i] = abs(y_sv0pt_6070_tight[i] - y_sv0pt_6070_nominal[i]);
            vec_y_unc_sv0pt_5060_loose[i] = StatisticalUncertainties(y_unc_sv0pt_5060_loose[i], y_unc_sv0pt_5060_nominal[i]);
            vec_y_unc_sv0pt_6070_loose[i] = StatisticalUncertainties(y_unc_sv0pt_6070_loose[i], y_unc_sv0pt_6070_nominal[i]);
            vec_y_unc_sv0pt_5060_tight[i] = StatisticalUncertainties(y_unc_sv0pt_5060_tight[i], y_unc_sv0pt_5060_nominal[i]);
            vec_y_unc_sv0pt_6070_tight[i] = StatisticalUncertainties(y_unc_sv0pt_6070_tight[i], y_unc_sv0pt_6070_nominal[i]);
        }

        TGraph* gd_sv0pt_5060_loose = new TGraphErrors(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_5060_loose.data(), vec_x_unc_sv0pt.data(), vec_y_unc_sv0pt_5060_loose.data());
        TGraph* gd_sv0pt_6070_loose = new TGraphErrors(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_6070_loose.data(), vec_x_unc_sv0pt.data(), vec_y_unc_sv0pt_6070_loose.data());
        TGraph* gd_sv0pt_5060_tight = new TGraphErrors(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_5060_tight.data(), vec_x_unc_sv0pt.data(), vec_y_unc_sv0pt_5060_tight.data());
        TGraph* gd_sv0pt_6070_tight = new TGraphErrors(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_6070_tight.data(), vec_x_unc_sv0pt.data(), vec_y_unc_sv0pt_6070_tight.data());

    f->Close();

    // Saving TGraphs
    TFile *sf = new TFile("./Data/Systematics/TrackSelectionUnc.root", "UPDATE");

    // v0
    gd_v0_loose->SetName("gd_v0_loose"); gd_v0_loose->Write();
    gd_v0_tight->SetName("gd_v0_tight"); gd_v0_tight->Write();
    // v0(pT)
    gd_v0pt_5060_loose->SetName("gd_v0pt_5060_loose"); gd_v0pt_5060_loose->Write();
    gd_v0pt_6070_loose->SetName("gd_v0pt_6070_loose"); gd_v0pt_6070_loose->Write();
    gd_v0pt_5060_tight->SetName("gd_v0pt_5060_tight"); gd_v0pt_5060_tight->Write();
    gd_v0pt_6070_tight->SetName("gd_v0pt_6070_tight"); gd_v0pt_6070_tight->Write();
    // v0(pT)/v0
    gd_sv0pt_5060_loose->SetName("gd_sv0pt_5060_loose"); gd_sv0pt_5060_loose->Write();
    gd_sv0pt_6070_loose->SetName("gd_sv0pt_6070_loose"); gd_sv0pt_6070_loose->Write();
    gd_sv0pt_5060_tight->SetName("gd_sv0pt_5060_tight"); gd_sv0pt_5060_tight->Write();
    gd_sv0pt_6070_tight->SetName("gd_sv0pt_6070_tight"); gd_sv0pt_6070_tight->Write();
    
    sf->Close();
}

void CentralityFluctuations(){

    if (!filesystem::exists("./Data/Systematics/CentralityFluctuationData.root")){
        cout << "Evaluating zero fluctuation 50-60%" << endl; 
        ObsConstructor(1.0, 210.0, 375.0, 0.0, 0.5, 2.0, "Correc", "zero", "./Data/Systematics/CentralityFluctuationData.root", "Nominal", 15.0);
        cout << "Evaluating zero fluctuation 60-70%" << endl; 
        ObsConstructor(1.0, 100.0, 210.0, 0.0, 0.5, 2.0, "Correc", "zero", "./Data/Systematics/CentralityFluctuationData.root", "Nominal", 15.0);

        cout << "Evaluating positive fluctuation 50-60%" << endl; 
        ObsConstructor(1.0, 210.0, 375.0, 0.01, 0.5, 2.0, "Correc", "positive", "./Data/Systematics/CentralityFluctuationData.root", "Nominal", 15.0);
        cout << "Evaluating positive fluctuation 60-70%" << endl;
        ObsConstructor(1.0, 100.0, 210.0, 0.01, 0.5, 2.0, "Correc", "positive", "./Data/Systematics/CentralityFluctuationData.root", "Nominal", 15.0);

        cout << "Evaluating negative fluctuation 50-60%" << endl; 
        ObsConstructor(1.0, 210.0, 375.0, -0.01, 0.5, 2.0, "Correc", "negative", "./Data/Systematics/CentralityFluctuationData.root", "Nominal", 15.0);
        cout << "Evaluating negative fluctuation 60-70%" << endl;
        ObsConstructor(1.0, 100.0, 210.0, -0.01, 0.5, 2.0, "Correc", "negative", "./Data/Systematics/CentralityFluctuationData.root", "Nominal", 15.0);
    }

    TFile *f = TFile::Open("./Data/Systematics/CentralityFluctuationData.root", "READ");

    // Calculating v0 difference
        TGraphErrors *g_v0_55_zero = (TGraphErrors*)f->Get("v0_55_zero");
        TGraphErrors *g_v0_55_positive = (TGraphErrors*)f->Get("v0_55_positive");
        TGraphErrors *g_v0_55_negative = (TGraphErrors*)f->Get("v0_55_negative");
        TGraphErrors *g_v0_65_zero = (TGraphErrors*)f->Get("v0_65_zero");
        TGraphErrors *g_v0_65_positive = (TGraphErrors*)f->Get("v0_65_positive");
        TGraphErrors *g_v0_65_negative = (TGraphErrors*)f->Get("v0_65_negative");

        Int_t N_v0 = 2;
        Double_t *y_v0_55_zero = g_v0_55_zero->GetY();
        Double_t *y_v0_65_zero = g_v0_65_zero->GetY();
        Double_t *y_v0_55_positive = g_v0_55_positive->GetY();
        Double_t *y_v0_65_positive = g_v0_65_positive->GetY();
        Double_t *y_v0_55_negative = g_v0_55_negative->GetY();
        Double_t *y_v0_65_negative = g_v0_65_negative->GetY();
        Double_t *y_unc_v0_55_zero = g_v0_55_zero->GetEY();
        Double_t *y_unc_v0_65_zero = g_v0_65_zero->GetEY();
        Double_t *y_unc_v0_55_positive = g_v0_55_positive->GetEY();
        Double_t *y_unc_v0_65_positive = g_v0_65_positive->GetEY();
        Double_t *y_unc_v0_55_negative = g_v0_55_negative->GetEY();
        Double_t *y_unc_v0_65_negative = g_v0_65_negative->GetEY();

        vector<Double_t> vec_x_v0(N_v0, 0.0);
        vector<Double_t> vec_x_unc_v0(N_v0, 0.0);
        vector<Double_t> vec_y_v0_positive(N_v0, 0.0);
        vector<Double_t> vec_y_v0_negative(N_v0, 0.0);
        vector<Double_t> vec_y_unc_v0_positive(N_v0, 0.0);
        vector<Double_t> vec_y_unc_v0_negative(N_v0, 0.0);

        vec_x_v0[0] = 55.0; vec_x_v0[1] = 65.0;
        vec_y_v0_positive[0] = abs(y_v0_55_positive[0] - y_v0_55_zero[0]); vec_y_v0_positive[1] = abs(y_v0_65_positive[0] - y_v0_65_zero[0]);
        vec_y_v0_negative[0] = abs(y_v0_55_negative[0] - y_v0_55_zero[0]); vec_y_v0_negative[1] = abs(y_v0_65_negative[0] - y_v0_65_zero[0]);
        vec_y_unc_v0_positive[0] = StatisticalUncertainties(y_unc_v0_55_positive[0], y_unc_v0_55_zero[0]); vec_y_unc_v0_positive[1] = StatisticalUncertainties(y_unc_v0_65_positive[0], y_unc_v0_65_zero[0]);
        vec_y_unc_v0_negative[0] = StatisticalUncertainties(y_unc_v0_55_negative[0], y_unc_v0_55_zero[0]); vec_y_unc_v0_negative[1] = StatisticalUncertainties(y_unc_v0_65_negative[0], y_unc_v0_65_zero[0]);

        TGraph* gd_v0_positive = new TGraphErrors(N_v0, vec_x_v0.data(), vec_y_v0_positive.data(), vec_x_unc_v0.data(), vec_y_unc_v0_positive.data());
        TGraph* gd_v0_negative = new TGraphErrors(N_v0, vec_x_v0.data(), vec_y_v0_negative.data(), vec_x_unc_v0.data(), vec_y_unc_v0_negative.data());

    // Calculating v0(pT) difference
        TGraphErrors *g_v0pt_5060_zero = (TGraphErrors*)f->Get("v0pt_ptref_zero_5060");
        TGraphErrors *g_v0pt_6070_zero = (TGraphErrors*)f->Get("v0pt_ptref_zero_6070");
        TGraphErrors *g_v0pt_5060_positive = (TGraphErrors*)f->Get("v0pt_ptref_positive_5060");
        TGraphErrors *g_v0pt_6070_positive = (TGraphErrors*)f->Get("v0pt_ptref_positive_6070");
        TGraphErrors *g_v0pt_5060_negative = (TGraphErrors*)f->Get("v0pt_ptref_negative_5060");
        TGraphErrors *g_v0pt_6070_negative = (TGraphErrors*)f->Get("v0pt_ptref_negative_6070");

        Int_t N_v0pt = g_v0pt_5060_zero->GetN();
        Double_t *x_v0pt = g_v0pt_5060_zero->GetX();
        Double_t *y_v0pt_5060_zero = g_v0pt_5060_zero->GetY();
        Double_t *y_v0pt_6070_zero = g_v0pt_6070_zero->GetY();
        Double_t *y_v0pt_5060_positive = g_v0pt_5060_positive->GetY();
        Double_t *y_v0pt_6070_positive = g_v0pt_6070_positive->GetY();
        Double_t *y_v0pt_5060_negative = g_v0pt_5060_negative->GetY();
        Double_t *y_v0pt_6070_negative = g_v0pt_6070_negative->GetY();
        Double_t *y_unc_v0pt_5060_zero = g_v0pt_5060_zero->GetEY();
        Double_t *y_unc_v0pt_6070_zero = g_v0pt_6070_zero->GetEY();
        Double_t *y_unc_v0pt_5060_positive = g_v0pt_5060_positive->GetEY();
        Double_t *y_unc_v0pt_6070_positive = g_v0pt_6070_positive->GetEY();
        Double_t *y_unc_v0pt_5060_negative = g_v0pt_5060_negative->GetEY();
        Double_t *y_unc_v0pt_6070_negative = g_v0pt_6070_negative->GetEY();

        vector<Double_t> vec_x_v0pt(N_v0pt, 0.0);
        vector<Double_t> vec_x_unc_v0pt(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_5060_positive(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_6070_positive(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_5060_negative(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_6070_negative(N_v0pt, 0.0);
        vector<Double_t> vec_y_unc_v0pt_5060_positive(N_v0pt, 0.0);
        vector<Double_t> vec_y_unc_v0pt_6070_positive(N_v0pt, 0.0);
        vector<Double_t> vec_y_unc_v0pt_5060_negative(N_v0pt, 0.0);
        vector<Double_t> vec_y_unc_v0pt_6070_negative(N_v0pt, 0.0);

        for (int i=0; i<N_v0pt; i++){
            vec_x_v0pt[i] = x_v0pt[i];
            vec_y_v0pt_5060_positive[i] = abs(y_v0pt_5060_positive[i] - y_v0pt_5060_zero[i]);
            vec_y_v0pt_6070_positive[i] = abs(y_v0pt_6070_positive[i] - y_v0pt_6070_zero[i]);
            vec_y_v0pt_5060_negative[i] = abs(y_v0pt_5060_negative[i] - y_v0pt_5060_zero[i]);
            vec_y_v0pt_6070_negative[i] = abs(y_v0pt_6070_negative[i] - y_v0pt_6070_zero[i]);
            vec_y_unc_v0pt_5060_positive[i] = StatisticalUncertainties(y_unc_v0pt_5060_positive[i], y_unc_v0pt_5060_zero[i]);
            vec_y_unc_v0pt_6070_positive[i] = StatisticalUncertainties(y_unc_v0pt_6070_positive[i], y_unc_v0pt_6070_zero[i]);
            vec_y_unc_v0pt_5060_negative[i] = StatisticalUncertainties(y_unc_v0pt_5060_negative[i], y_unc_v0pt_5060_zero[i]);
            vec_y_unc_v0pt_6070_negative[i] = StatisticalUncertainties(y_unc_v0pt_6070_negative[i], y_unc_v0pt_6070_zero[i]);
        }

        TGraph* gd_v0pt_5060_positive = new TGraphErrors(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_5060_positive.data(), vec_x_unc_v0pt.data(), vec_y_unc_v0pt_5060_positive.data());
        TGraph* gd_v0pt_6070_positive = new TGraphErrors(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_6070_positive.data(), vec_x_unc_v0pt.data(), vec_y_unc_v0pt_6070_positive.data());
        TGraph* gd_v0pt_5060_negative = new TGraphErrors(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_5060_negative.data(), vec_x_unc_v0pt.data(), vec_y_unc_v0pt_5060_negative.data());
        TGraph* gd_v0pt_6070_negative = new TGraphErrors(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_6070_negative.data(), vec_x_unc_v0pt.data(), vec_y_unc_v0pt_6070_negative.data());

    // Calculating v0(pT)/v0 difference
        TGraphErrors *g_sv0pt_5060_zero = (TGraphErrors*)f->Get("sv0pt_ptref_zero_5060");
        TGraphErrors *g_sv0pt_6070_zero = (TGraphErrors*)f->Get("sv0pt_ptref_zero_6070");
        TGraphErrors *g_sv0pt_5060_positive = (TGraphErrors*)f->Get("sv0pt_ptref_positive_5060");
        TGraphErrors *g_sv0pt_6070_positive = (TGraphErrors*)f->Get("sv0pt_ptref_positive_6070");
        TGraphErrors *g_sv0pt_5060_negative = (TGraphErrors*)f->Get("sv0pt_ptref_negative_5060");
        TGraphErrors *g_sv0pt_6070_negative = (TGraphErrors*)f->Get("sv0pt_ptref_negative_6070");

        Int_t N_sv0pt = g_sv0pt_5060_zero->GetN();
        Double_t *x_sv0pt = g_sv0pt_5060_zero->GetX();
        Double_t *y_sv0pt_5060_zero = g_sv0pt_5060_zero->GetY();
        Double_t *y_sv0pt_6070_zero = g_sv0pt_6070_zero->GetY();
        Double_t *y_sv0pt_5060_positive = g_sv0pt_5060_positive->GetY();
        Double_t *y_sv0pt_6070_positive = g_sv0pt_6070_positive->GetY();
        Double_t *y_sv0pt_5060_negative = g_sv0pt_5060_negative->GetY();
        Double_t *y_sv0pt_6070_negative = g_sv0pt_6070_negative->GetY();
        Double_t *y_unc_sv0pt_5060_zero = g_sv0pt_5060_zero->GetEY();
        Double_t *y_unc_sv0pt_6070_zero = g_sv0pt_6070_zero->GetEY();
        Double_t *y_unc_sv0pt_5060_positive = g_sv0pt_5060_positive->GetEY();
        Double_t *y_unc_sv0pt_6070_positive = g_sv0pt_6070_positive->GetEY();
        Double_t *y_unc_sv0pt_5060_negative = g_sv0pt_5060_negative->GetEY();
        Double_t *y_unc_sv0pt_6070_negative = g_sv0pt_6070_negative->GetEY();

        vector<Double_t> vec_x_sv0pt(N_sv0pt, 0.0);
        vector<Double_t> vec_x_unc_sv0pt(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_5060_positive(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_6070_positive(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_5060_negative(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_6070_negative(N_sv0pt, 0.0);
        vector<Double_t> vec_y_unc_sv0pt_5060_positive(N_sv0pt, 0.0);
        vector<Double_t> vec_y_unc_sv0pt_6070_positive(N_sv0pt, 0.0);
        vector<Double_t> vec_y_unc_sv0pt_5060_negative(N_sv0pt, 0.0);
        vector<Double_t> vec_y_unc_sv0pt_6070_negative(N_sv0pt, 0.0);

        for (int i=0; i<N_sv0pt; i++){
            vec_x_sv0pt[i] = x_sv0pt[i];
            vec_y_sv0pt_5060_positive[i] = abs(y_sv0pt_5060_positive[i] - y_sv0pt_5060_zero[i]);
            vec_y_sv0pt_6070_positive[i] = abs(y_sv0pt_6070_positive[i] - y_sv0pt_6070_zero[i]);
            vec_y_sv0pt_5060_negative[i] = abs(y_sv0pt_5060_negative[i] - y_sv0pt_5060_zero[i]);
            vec_y_sv0pt_6070_negative[i] = abs(y_sv0pt_6070_negative[i] - y_sv0pt_6070_zero[i]);
            vec_y_unc_sv0pt_5060_positive[i] = StatisticalUncertainties(y_unc_sv0pt_5060_positive[i], y_unc_sv0pt_5060_zero[i]);
            vec_y_unc_sv0pt_6070_positive[i] = StatisticalUncertainties(y_unc_sv0pt_6070_positive[i], y_unc_sv0pt_6070_zero[i]);
            vec_y_unc_sv0pt_5060_negative[i] = StatisticalUncertainties(y_unc_sv0pt_5060_negative[i], y_unc_sv0pt_5060_zero[i]);
            vec_y_unc_sv0pt_6070_negative[i] = StatisticalUncertainties(y_unc_sv0pt_6070_negative[i], y_unc_sv0pt_6070_zero[i]);
        }

        TGraph* gd_sv0pt_5060_positive = new TGraphErrors(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_5060_positive.data(), vec_x_unc_sv0pt.data(), vec_y_unc_sv0pt_5060_positive.data());
        TGraph* gd_sv0pt_6070_positive = new TGraphErrors(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_6070_positive.data(), vec_x_unc_sv0pt.data(), vec_y_unc_sv0pt_6070_positive.data());
        TGraph* gd_sv0pt_5060_negative = new TGraphErrors(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_5060_negative.data(), vec_x_unc_sv0pt.data(), vec_y_unc_sv0pt_5060_negative.data());
        TGraph* gd_sv0pt_6070_negative = new TGraphErrors(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_6070_negative.data(), vec_x_unc_sv0pt.data(), vec_y_unc_sv0pt_6070_negative.data());

    f->Close();

    // Saving TGraphs
    TFile *sf = new TFile("./Data/Systematics/CentralityFluctuationUnc.root", "UPDATE");

    // v0
    gd_v0_positive->SetName("gd_v0_positive"); gd_v0_positive->Write();
    gd_v0_negative->SetName("gd_v0_negative"); gd_v0_negative->Write();
    // v0(pT)
    gd_v0pt_5060_positive->SetName("gd_v0pt_5060_positive"); gd_v0pt_5060_positive->Write();
    gd_v0pt_6070_positive->SetName("gd_v0pt_6070_positive"); gd_v0pt_6070_positive->Write();
    gd_v0pt_5060_negative->SetName("gd_v0pt_5060_negative"); gd_v0pt_5060_negative->Write();
    gd_v0pt_6070_negative->SetName("gd_v0pt_6070_negative"); gd_v0pt_6070_negative->Write();
    // v0(pT)/v0
    gd_sv0pt_5060_positive->SetName("gd_sv0pt_5060_positive"); gd_sv0pt_5060_positive->Write();
    gd_sv0pt_6070_positive->SetName("gd_sv0pt_6070_positive"); gd_sv0pt_6070_positive->Write();
    gd_sv0pt_5060_negative->SetName("gd_sv0pt_5060_negative"); gd_sv0pt_5060_negative->Write();
    gd_sv0pt_6070_negative->SetName("gd_sv0pt_6070_negative"); gd_sv0pt_6070_negative->Write();
    
    sf->Close();
}

void CorrectionApplication(){

    if (!filesystem::exists("./Data/Systematics/CorrectionApplicationData.root")){
        cout << "Evaluating with correction 50-60%" << endl; 
        ObsConstructor(1.0, 210.0, 375.0, 0.0, 0.5, 2.0, "Correc", "correc", "./Data/Systematics/CorrectionApplicationData.root", "Nominal", 15.0);
        cout << "Evaluating with correction 60-70%" << endl; // Calculating nominal 60-70%
        ObsConstructor(1.0, 100.0, 210.0, 0.0, 0.5, 2.0, "Correc", "correc", "./Data/Systematics/CorrectionApplicationData.root", "Nominal", 15.0);

        cout << "Evaluating without correction 50-60%" << endl; 
        ObsConstructor(1.0, 210.0, 375.0, 0.0, 0.5, 2.0, "noCorrec", "nocorrec", "./Data/Systematics/CorrectionApplicationData.root", "Nominal", 15.0);
        cout << "Evaluating without correction 60-70%" << endl;
        ObsConstructor(1.0, 100.0, 210.0, 0.0, 0.5, 2.0, "noCorrec", "nocorrec", "./Data/Systematics/CorrectionApplicationData.root", "Nominal", 15.0);
    }

    TFile *f = TFile::Open("./Data/Systematics/CorrectionApplicationData.root", "READ");

    // Calculating v0 difference
        TGraphErrors *g_v0_55_correc = (TGraphErrors*)f->Get("v0_55_correc");
        TGraphErrors *g_v0_55_nocorrec = (TGraphErrors*)f->Get("v0_55_nocorrec");
        TGraphErrors *g_v0_65_correc = (TGraphErrors*)f->Get("v0_65_correc");
        TGraphErrors *g_v0_65_nocorrec = (TGraphErrors*)f->Get("v0_65_nocorrec");

        Int_t N_v0 = 2;
        Double_t *y_v0_55_correc = g_v0_55_correc->GetY();
        Double_t *y_v0_65_correc = g_v0_65_correc->GetY();
        Double_t *y_v0_55_nocorrec = g_v0_55_nocorrec->GetY();
        Double_t *y_v0_65_nocorrec = g_v0_65_nocorrec->GetY();

        vector<Double_t> vec_x_v0(N_v0, 0.0);
        vector<Double_t> vec_y_v0_nocorrec(N_v0, 0.0);

        vec_x_v0[0] = 55.0; vec_x_v0[1] = 65.0;
        vec_y_v0_nocorrec[0] = abs(y_v0_55_nocorrec[0] - y_v0_55_correc[0]); vec_y_v0_nocorrec[1] = abs(y_v0_65_nocorrec[0] - y_v0_65_correc[0]);

        TGraph* gd_v0_nocorrec = new TGraph(N_v0, vec_x_v0.data(), vec_y_v0_nocorrec.data());

    // Calculating v0(pT) difference
        TGraphErrors *g_v0pt_5060_correc = (TGraphErrors*)f->Get("v0pt_ptref_correc_5060");
        TGraphErrors *g_v0pt_6070_correc = (TGraphErrors*)f->Get("v0pt_ptref_correc_6070");
        TGraphErrors *g_v0pt_5060_nocorrec = (TGraphErrors*)f->Get("v0pt_ptref_nocorrec_5060");
        TGraphErrors *g_v0pt_6070_nocorrec = (TGraphErrors*)f->Get("v0pt_ptref_nocorrec_6070");

        Int_t N_v0pt = g_v0pt_5060_correc->GetN();
        Double_t *x_v0pt = g_v0pt_5060_correc->GetX();
        Double_t *y_v0pt_5060_correc = g_v0pt_5060_correc->GetY();
        Double_t *y_v0pt_6070_correc = g_v0pt_6070_correc->GetY();
        Double_t *y_v0pt_5060_nocorrec = g_v0pt_5060_nocorrec->GetY();
        Double_t *y_v0pt_6070_nocorrec = g_v0pt_6070_nocorrec->GetY();

        vector<Double_t> vec_x_v0pt(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_5060_nocorrec(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_6070_nocorrec(N_v0pt, 0.0);

        for (int i=0; i<N_v0pt; i++){
            vec_x_v0pt[i] = x_v0pt[i];
            vec_y_v0pt_5060_nocorrec[i] = abs(y_v0pt_5060_nocorrec[i] - y_v0pt_5060_correc[i]);
            vec_y_v0pt_6070_nocorrec[i] = abs(y_v0pt_6070_nocorrec[i] - y_v0pt_6070_correc[i]);
        }

        TGraph* gd_v0pt_5060_nocorrec = new TGraph(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_5060_nocorrec.data());
        TGraph* gd_v0pt_6070_nocorrec = new TGraph(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_6070_nocorrec.data());

    // Calculating v0(pT)/v0 difference
        TGraphErrors *g_sv0pt_5060_correc = (TGraphErrors*)f->Get("sv0pt_ptref_correc_5060");
        TGraphErrors *g_sv0pt_6070_correc = (TGraphErrors*)f->Get("sv0pt_ptref_correc_6070");
        TGraphErrors *g_sv0pt_5060_nocorrec = (TGraphErrors*)f->Get("sv0pt_ptref_nocorrec_5060");
        TGraphErrors *g_sv0pt_6070_nocorrec = (TGraphErrors*)f->Get("sv0pt_ptref_nocorrec_6070");

        Int_t N_sv0pt = g_sv0pt_5060_correc->GetN();
        Double_t *x_sv0pt = g_sv0pt_5060_correc->GetX();
        Double_t *y_sv0pt_5060_correc = g_sv0pt_5060_correc->GetY();
        Double_t *y_sv0pt_6070_correc = g_sv0pt_6070_correc->GetY();
        Double_t *y_sv0pt_5060_nocorrec = g_sv0pt_5060_nocorrec->GetY();
        Double_t *y_sv0pt_6070_nocorrec = g_sv0pt_6070_nocorrec->GetY();

        vector<Double_t> vec_x_sv0pt(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_5060_nocorrec(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_6070_nocorrec(N_sv0pt, 0.0);

        for (int i=0; i<N_sv0pt; i++){
            vec_x_sv0pt[i] = x_sv0pt[i];
            vec_y_sv0pt_5060_nocorrec[i] = abs(y_sv0pt_5060_nocorrec[i] - y_sv0pt_5060_correc[i]);
            vec_y_sv0pt_6070_nocorrec[i] = abs(y_sv0pt_6070_nocorrec[i] - y_sv0pt_6070_correc[i]);
        }

        TGraph* gd_sv0pt_5060_nocorrec = new TGraph(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_5060_nocorrec.data());
        TGraph* gd_sv0pt_6070_nocorrec = new TGraph(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_6070_nocorrec.data());

    f->Close();

    // Saving TGraphs
    TFile *sf = new TFile("./Data/Systematics/CorrectionApplicationUnc.root", "UPDATE");

    // v0
    gd_v0_nocorrec->SetName("gd_v0_nocorrec"); gd_v0_nocorrec->Write();
    // v0(pT)
    gd_v0pt_5060_nocorrec->SetName("gd_v0pt_5060_nocorrec"); gd_v0pt_5060_nocorrec->Write();
    gd_v0pt_6070_nocorrec->SetName("gd_v0pt_6070_nocorrec"); gd_v0pt_6070_nocorrec->Write();
    // v0(pT)/v0
    gd_sv0pt_5060_nocorrec->SetName("gd_sv0pt_5060_nocorrec"); gd_sv0pt_5060_nocorrec->Write();
    gd_sv0pt_6070_nocorrec->SetName("gd_sv0pt_6070_nocorrec"); gd_sv0pt_6070_nocorrec->Write();
    
    sf->Close();
}

void ZvertexPosition(){

    if (!filesystem::exists("./Data/Systematics/ZvertexPositionData.root")){
        cout << "Evaluating |pvZ| < 15 cm 50-60%" << endl; // default
        ObsConstructor(1.0, 210.0, 375.0, 0.0, 0.5, 2.0, "noCorrec", "pvZ_15", "./Data/Systematics/ZvertexPositionData.root", "Nominal", 15.0);
        cout << "Evaluating |pvZ| < 15 cm 60-70%" << endl; // default
        ObsConstructor(1.0, 100.0, 210.0, 0.0, 0.5, 2.0, "noCorrec", "pvZ_15", "./Data/Systematics/ZvertexPositionData.root", "Nominal", 15.0);

        cout << "Evaluating |pvZ| < 3 cm 50-60%" << endl;
        ObsConstructor(1.0, 210.0, 375.0, 0.0, 0.5, 2.0, "noCorrec", "pvZ_3", "./Data/Systematics/ZvertexPositionData.root", "Nominal", 3.0);
        cout << "Evaluating |pvZ| < 3 cm 60-70%" << endl;
        ObsConstructor(1.0, 100.0, 210.0, 0.0, 0.5, 2.0, "noCorrec", "pvZ_3", "./Data/Systematics/ZvertexPositionData.root", "Nominal", 3.0);

        cout << "Evaluating 3 < |pvZ| < 15 cm 50-60%" << endl;
        ObsConstructor(1.0, 210.0, 375.0, 0.0, 0.5, 2.0, "noCorrec", "pvZ_3_15", "./Data/Systematics/ZvertexPositionData.root", "Nominal", 3.15);
        cout << "Evaluating 3 <|pvZ| < 15 cm 60-70%" << endl;
        ObsConstructor(1.0, 100.0, 210.0, 0.0, 0.5, 2.0, "noCorrec", "pvZ_3_15", "./Data/Systematics/ZvertexPositionData.root", "Nominal", 3.15);
    }

    TFile *f = TFile::Open("./Data/Systematics/ZvertexPositionData.root", "READ");

    // Calculating v0 difference
        TGraphErrors *g_v0_55_pvZ_15 = (TGraphErrors*)f->Get("v0_55_pvZ_15");
        TGraphErrors *g_v0_55_pvZ_3 = (TGraphErrors*)f->Get("v0_55_pvZ_3");
        TGraphErrors *g_v0_55_pvZ_3_15 = (TGraphErrors*)f->Get("v0_55_pvZ_3_15");
        TGraphErrors *g_v0_65_pvZ_15 = (TGraphErrors*)f->Get("v0_65_pvZ_15");
        TGraphErrors *g_v0_65_pvZ_3 = (TGraphErrors*)f->Get("v0_65_pvZ_3");
        TGraphErrors *g_v0_65_pvZ_3_15 = (TGraphErrors*)f->Get("v0_65_pvZ_3_15");

        Int_t N_v0 = 2;
        Double_t *y_v0_55_pvZ_15 = g_v0_55_pvZ_15->GetY();
        Double_t *y_v0_65_pvZ_15 = g_v0_65_pvZ_15->GetY();
        Double_t *y_v0_55_pvZ_3 = g_v0_55_pvZ_3->GetY();
        Double_t *y_v0_65_pvZ_3 = g_v0_65_pvZ_3->GetY();
        Double_t *y_v0_55_pvZ_3_15 = g_v0_55_pvZ_3_15->GetY();
        Double_t *y_v0_65_pvZ_3_15 = g_v0_65_pvZ_3_15->GetY();
        Double_t *y_unc_v0_55_pvZ_15 = g_v0_55_pvZ_15->GetEY();
        Double_t *y_unc_v0_65_pvZ_15 = g_v0_65_pvZ_15->GetEY();
        Double_t *y_unc_v0_55_pvZ_3 = g_v0_55_pvZ_3->GetEY();
        Double_t *y_unc_v0_65_pvZ_3 = g_v0_65_pvZ_3->GetEY();
        Double_t *y_unc_v0_55_pvZ_3_15 = g_v0_55_pvZ_3_15->GetEY();
        Double_t *y_unc_v0_65_pvZ_3_15 = g_v0_65_pvZ_3_15->GetEY();

        vector<Double_t> vec_x_v0(N_v0, 0.0);
        vector<Double_t> vec_x_unc_v0(N_v0, 0.0);
        vector<Double_t> vec_y_v0_pvZ_3(N_v0, 0.0);
        vector<Double_t> vec_y_v0_pvZ_3_15(N_v0, 0.0);
        vector<Double_t> vec_y_unc_v0_pvZ_3(N_v0, 0.0);
        vector<Double_t> vec_y_unc_v0_pvZ_3_15(N_v0, 0.0);

        vec_x_v0[0] = 55.0; vec_x_v0[1] = 65.0;
        vec_y_v0_pvZ_3[0] = abs(y_v0_55_pvZ_3[0] - y_v0_55_pvZ_15[0]); vec_y_v0_pvZ_3[1] = abs(y_v0_65_pvZ_3[0] - y_v0_65_pvZ_15[0]);
        vec_y_v0_pvZ_3_15[0] = abs(y_v0_55_pvZ_3_15[0] - y_v0_55_pvZ_15[0]); vec_y_v0_pvZ_3_15[1] = abs(y_v0_65_pvZ_3_15[0] - y_v0_65_pvZ_15[0]);
        vec_y_unc_v0_pvZ_3[0] = StatisticalUncertainties(y_unc_v0_55_pvZ_3[0], y_unc_v0_55_pvZ_15[0]); vec_y_unc_v0_pvZ_3[1] = StatisticalUncertainties(y_unc_v0_65_pvZ_3[0], y_unc_v0_65_pvZ_15[0]);
        vec_y_unc_v0_pvZ_3_15[0] = StatisticalUncertainties(y_unc_v0_55_pvZ_3_15[0], y_unc_v0_55_pvZ_15[0]); vec_y_unc_v0_pvZ_3_15[1] = StatisticalUncertainties(y_unc_v0_65_pvZ_3_15[0], y_unc_v0_65_pvZ_15[0]);

        TGraph* gd_v0_pvZ_3 = new TGraphErrors(N_v0, vec_x_v0.data(), vec_y_v0_pvZ_3.data(), vec_x_unc_v0.data(), vec_y_unc_v0_pvZ_3.data());
        TGraph* gd_v0_pvZ_3_15 = new TGraphErrors(N_v0, vec_x_v0.data(), vec_y_v0_pvZ_3_15.data(), vec_x_unc_v0.data(), vec_y_unc_v0_pvZ_3_15.data());

    // Calculating v0(pT) difference
        TGraphErrors *g_v0pt_5060_pvZ_15 = (TGraphErrors*)f->Get("v0pt_ptref_pvZ_15_5060");
        TGraphErrors *g_v0pt_6070_pvZ_15 = (TGraphErrors*)f->Get("v0pt_ptref_pvZ_15_6070");
        TGraphErrors *g_v0pt_5060_pvZ_3 = (TGraphErrors*)f->Get("v0pt_ptref_pvZ_3_5060");
        TGraphErrors *g_v0pt_6070_pvZ_3 = (TGraphErrors*)f->Get("v0pt_ptref_pvZ_3_6070");
        TGraphErrors *g_v0pt_5060_pvZ_3_15 = (TGraphErrors*)f->Get("v0pt_ptref_pvZ_3_15_5060");
        TGraphErrors *g_v0pt_6070_pvZ_3_15 = (TGraphErrors*)f->Get("v0pt_ptref_pvZ_3_15_6070");

        Int_t N_v0pt = g_v0pt_5060_pvZ_15->GetN();
        Double_t *x_v0pt = g_v0pt_5060_pvZ_15->GetX();
        Double_t *y_v0pt_5060_pvZ_15 = g_v0pt_5060_pvZ_15->GetY();
        Double_t *y_v0pt_6070_pvZ_15 = g_v0pt_6070_pvZ_15->GetY();
        Double_t *y_v0pt_5060_pvZ_3 = g_v0pt_5060_pvZ_3->GetY();
        Double_t *y_v0pt_6070_pvZ_3 = g_v0pt_6070_pvZ_3->GetY();
        Double_t *y_v0pt_5060_pvZ_3_15 = g_v0pt_5060_pvZ_3_15->GetY();
        Double_t *y_v0pt_6070_pvZ_3_15 = g_v0pt_6070_pvZ_3_15->GetY();
        Double_t *y_unc_v0pt_5060_pvZ_15 = g_v0pt_5060_pvZ_15->GetEY();
        Double_t *y_unc_v0pt_6070_pvZ_15 = g_v0pt_6070_pvZ_15->GetEY();
        Double_t *y_unc_v0pt_5060_pvZ_3 = g_v0pt_5060_pvZ_3->GetEY();
        Double_t *y_unc_v0pt_6070_pvZ_3 = g_v0pt_6070_pvZ_3->GetEY();
        Double_t *y_unc_v0pt_5060_pvZ_3_15 = g_v0pt_5060_pvZ_3_15->GetEY();
        Double_t *y_unc_v0pt_6070_pvZ_3_15 = g_v0pt_6070_pvZ_3_15->GetEY();

        vector<Double_t> vec_x_v0pt(N_v0pt, 0.0);
        vector<Double_t> vec_x_unc_v0pt(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_5060_pvZ_3(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_6070_pvZ_3(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_5060_pvZ_3_15(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_6070_pvZ_3_15(N_v0pt, 0.0);
        vector<Double_t> vec_y_unc_v0pt_5060_pvZ_3(N_v0pt, 0.0);
        vector<Double_t> vec_y_unc_v0pt_6070_pvZ_3(N_v0pt, 0.0);
        vector<Double_t> vec_y_unc_v0pt_5060_pvZ_3_15(N_v0pt, 0.0);
        vector<Double_t> vec_y_unc_v0pt_6070_pvZ_3_15(N_v0pt, 0.0);

        for (int i=0; i<N_v0pt; i++){
            vec_x_v0pt[i] = x_v0pt[i];
            vec_y_v0pt_5060_pvZ_3[i] = abs(y_v0pt_5060_pvZ_3[i] - y_v0pt_5060_pvZ_15[i]);
            vec_y_v0pt_6070_pvZ_3[i] = abs(y_v0pt_6070_pvZ_3[i] - y_v0pt_6070_pvZ_15[i]);
            vec_y_v0pt_5060_pvZ_3_15[i] = abs(y_v0pt_5060_pvZ_3_15[i] - y_v0pt_5060_pvZ_15[i]);
            vec_y_v0pt_6070_pvZ_3_15[i] = abs(y_v0pt_6070_pvZ_3_15[i] - y_v0pt_6070_pvZ_15[i]);
            vec_y_unc_v0pt_5060_pvZ_3[i] = StatisticalUncertainties(y_unc_v0pt_5060_pvZ_3[i], y_unc_v0pt_5060_pvZ_15[i]);
            vec_y_unc_v0pt_6070_pvZ_3[i] = StatisticalUncertainties(y_unc_v0pt_6070_pvZ_3[i], y_unc_v0pt_6070_pvZ_15[i]);
            vec_y_unc_v0pt_5060_pvZ_3_15[i] = StatisticalUncertainties(y_unc_v0pt_5060_pvZ_3_15[i], y_unc_v0pt_5060_pvZ_15[i]);
            vec_y_unc_v0pt_6070_pvZ_3_15[i] = StatisticalUncertainties(y_unc_v0pt_6070_pvZ_3_15[i], y_unc_v0pt_6070_pvZ_15[i]);
        }

        TGraph* gd_v0pt_5060_pvZ_3 = new TGraphErrors(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_5060_pvZ_3.data(), vec_x_unc_v0pt.data(), vec_y_unc_v0pt_5060_pvZ_3.data());
        TGraph* gd_v0pt_6070_pvZ_3 = new TGraphErrors(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_6070_pvZ_3.data(), vec_x_unc_v0pt.data(), vec_y_unc_v0pt_6070_pvZ_3.data());
        TGraph* gd_v0pt_5060_pvZ_3_15 = new TGraphErrors(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_5060_pvZ_3_15.data(), vec_x_unc_v0pt.data(), vec_y_unc_v0pt_5060_pvZ_3_15.data());
        TGraph* gd_v0pt_6070_pvZ_3_15 = new TGraphErrors(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_6070_pvZ_3_15.data(), vec_x_unc_v0pt.data(), vec_y_unc_v0pt_6070_pvZ_3_15.data());

    // Calculating v0(pT)/v0 difference
        TGraphErrors *g_sv0pt_5060_pvZ_15 = (TGraphErrors*)f->Get("sv0pt_ptref_pvZ_15_5060");
        TGraphErrors *g_sv0pt_6070_pvZ_15 = (TGraphErrors*)f->Get("sv0pt_ptref_pvZ_15_6070");
        TGraphErrors *g_sv0pt_5060_pvZ_3 = (TGraphErrors*)f->Get("sv0pt_ptref_pvZ_3_5060");
        TGraphErrors *g_sv0pt_6070_pvZ_3 = (TGraphErrors*)f->Get("sv0pt_ptref_pvZ_3_6070");
        TGraphErrors *g_sv0pt_5060_pvZ_3_15 = (TGraphErrors*)f->Get("sv0pt_ptref_pvZ_3_15_5060");
        TGraphErrors *g_sv0pt_6070_pvZ_3_15 = (TGraphErrors*)f->Get("sv0pt_ptref_pvZ_3_15_6070");

        Int_t N_sv0pt = g_sv0pt_5060_pvZ_15->GetN();
        Double_t *x_sv0pt = g_sv0pt_5060_pvZ_15->GetX();
        Double_t *y_sv0pt_5060_pvZ_15 = g_sv0pt_5060_pvZ_15->GetY();
        Double_t *y_sv0pt_6070_pvZ_15 = g_sv0pt_6070_pvZ_15->GetY();
        Double_t *y_sv0pt_5060_pvZ_3 = g_sv0pt_5060_pvZ_3->GetY();
        Double_t *y_sv0pt_6070_pvZ_3 = g_sv0pt_6070_pvZ_3->GetY();
        Double_t *y_sv0pt_5060_pvZ_3_15 = g_sv0pt_5060_pvZ_3_15->GetY();
        Double_t *y_sv0pt_6070_pvZ_3_15 = g_sv0pt_6070_pvZ_3_15->GetY();
        Double_t *y_unc_sv0pt_5060_pvZ_15 = g_sv0pt_5060_pvZ_15->GetEY();
        Double_t *y_unc_sv0pt_6070_pvZ_15 = g_sv0pt_6070_pvZ_15->GetEY();
        Double_t *y_unc_sv0pt_5060_pvZ_3 = g_sv0pt_5060_pvZ_3->GetEY();
        Double_t *y_unc_sv0pt_6070_pvZ_3 = g_sv0pt_6070_pvZ_3->GetEY();
        Double_t *y_unc_sv0pt_5060_pvZ_3_15 = g_sv0pt_5060_pvZ_3_15->GetEY();
        Double_t *y_unc_sv0pt_6070_pvZ_3_15 = g_sv0pt_6070_pvZ_3_15->GetEY();

        vector<Double_t> vec_x_sv0pt(N_sv0pt, 0.0);
        vector<Double_t> vec_x_unc_sv0pt(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_5060_pvZ_3(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_6070_pvZ_3(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_5060_pvZ_3_15(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_6070_pvZ_3_15(N_sv0pt, 0.0);
        vector<Double_t> vec_y_unc_sv0pt_5060_pvZ_3(N_sv0pt, 0.0);
        vector<Double_t> vec_y_unc_sv0pt_6070_pvZ_3(N_sv0pt, 0.0);
        vector<Double_t> vec_y_unc_sv0pt_5060_pvZ_3_15(N_sv0pt, 0.0);
        vector<Double_t> vec_y_unc_sv0pt_6070_pvZ_3_15(N_sv0pt, 0.0);

        for (int i=0; i<N_sv0pt; i++){
            vec_x_sv0pt[i] = x_sv0pt[i];
            vec_y_sv0pt_5060_pvZ_3[i] = abs(y_sv0pt_5060_pvZ_3[i] - y_sv0pt_5060_pvZ_15[i]);
            vec_y_sv0pt_6070_pvZ_3[i] = abs(y_sv0pt_6070_pvZ_3[i] - y_sv0pt_6070_pvZ_15[i]);
            vec_y_sv0pt_5060_pvZ_3_15[i] = abs(y_sv0pt_5060_pvZ_3_15[i] - y_sv0pt_5060_pvZ_15[i]);
            vec_y_sv0pt_6070_pvZ_3_15[i] = abs(y_sv0pt_6070_pvZ_3_15[i] - y_sv0pt_6070_pvZ_15[i]);
            vec_y_unc_sv0pt_5060_pvZ_3[i] = StatisticalUncertainties(y_unc_sv0pt_5060_pvZ_3[i], y_unc_sv0pt_5060_pvZ_15[i]);
            vec_y_unc_sv0pt_6070_pvZ_3[i] = StatisticalUncertainties(y_unc_sv0pt_6070_pvZ_3[i], y_unc_sv0pt_6070_pvZ_15[i]);
            vec_y_unc_sv0pt_5060_pvZ_3_15[i] = StatisticalUncertainties(y_unc_sv0pt_5060_pvZ_3_15[i], y_unc_sv0pt_5060_pvZ_15[i]);
            vec_y_unc_sv0pt_6070_pvZ_3_15[i] = StatisticalUncertainties(y_unc_sv0pt_6070_pvZ_3_15[i], y_unc_sv0pt_6070_pvZ_15[i]);
        }

        TGraph* gd_sv0pt_5060_pvZ_3 = new TGraphErrors(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_5060_pvZ_3.data(), vec_x_unc_sv0pt.data(), vec_y_unc_sv0pt_5060_pvZ_3.data());
        TGraph* gd_sv0pt_6070_pvZ_3 = new TGraphErrors(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_6070_pvZ_3.data(), vec_x_unc_sv0pt.data(), vec_y_unc_sv0pt_6070_pvZ_3.data());
        TGraph* gd_sv0pt_5060_pvZ_3_15 = new TGraphErrors(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_5060_pvZ_3_15.data(), vec_x_unc_sv0pt.data(), vec_y_unc_sv0pt_5060_pvZ_3_15.data());
        TGraph* gd_sv0pt_6070_pvZ_3_15 = new TGraphErrors(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_6070_pvZ_3_15.data(), vec_x_unc_sv0pt.data(), vec_y_unc_sv0pt_6070_pvZ_3_15.data());

    f->Close();

    // Saving TGraphs
    TFile *sf = new TFile("./Data/Systematics/ZvertexPositionUnc.root", "UPDATE");

    // v0
    gd_v0_pvZ_3->SetName("gd_v0_pvZ_3"); gd_v0_pvZ_3->Write();
    gd_v0_pvZ_3_15->SetName("gd_v0_pvZ_3_15"); gd_v0_pvZ_3_15->Write();
    // v0(pT)
    gd_v0pt_5060_pvZ_3->SetName("gd_v0pt_5060_pvZ_3"); gd_v0pt_5060_pvZ_3->Write();
    gd_v0pt_6070_pvZ_3->SetName("gd_v0pt_6070_pvZ_3"); gd_v0pt_6070_pvZ_3->Write();
    gd_v0pt_5060_pvZ_3_15->SetName("gd_v0pt_5060_pvZ_3_15"); gd_v0pt_5060_pvZ_3_15->Write();
    gd_v0pt_6070_pvZ_3_15->SetName("gd_v0pt_6070_pvZ_3_15"); gd_v0pt_6070_pvZ_3_15->Write();
    // v0(pT)/v0
    gd_sv0pt_5060_pvZ_3->SetName("gd_sv0pt_5060_pvZ_3"); gd_sv0pt_5060_pvZ_3->Write();
    gd_sv0pt_6070_pvZ_3->SetName("gd_sv0pt_6070_pvZ_3"); gd_sv0pt_6070_pvZ_3->Write();
    gd_sv0pt_5060_pvZ_3_15->SetName("gd_sv0pt_5060_pvZ_3_15"); gd_sv0pt_5060_pvZ_3_15->Write();
    gd_sv0pt_6070_pvZ_3_15->SetName("gd_sv0pt_6070_pvZ_3_15"); gd_sv0pt_6070_pvZ_3_15->Write();
    
    sf->Close();
}

void Evaluate_v0(){
    int nVar = 4; // No. of different unc types
    int N_v0 = 2; // No. of points

    // Opening diff. files
    TFile *f_corr = TFile::Open("./Data/Systematics/CorrectionApplicationUnc.root");
    TFile *f_trk = TFile::Open("./Data/Systematics/TrackSelectionUnc.root");
    TFile *f_cent = TFile::Open("./Data/Systematics/CentralityFluctuationUnc.root");
    TFile *f_pvZ = TFile::Open("./Data/Systematics/ZvertexPositionUnc.root");

    // Loading TGraphs from TFiles, each one has the two points
    TGraph *gr_v0_correc = (TGraph*)f_corr->Get("gd_v0_nocorrec");
    TGraphErrors *gr_v0_trk_loose = (TGraphErrors*)f_trk->Get("gd_v0_loose");
    TGraphErrors *gr_v0_trk_tight = (TGraphErrors*)f_trk->Get("gd_v0_tight");
    TGraphErrors *gr_v0_cent_positive = (TGraphErrors*)f_cent->Get("gd_v0_positive");
    TGraphErrors *gr_v0_cent_negative = (TGraphErrors*)f_cent->Get("gd_v0_negative");
    TGraphErrors *gr_v0_pvZ_3 = (TGraphErrors*)f_pvZ->Get("gd_v0_pvZ_3");
    TGraphErrors *gr_v0_pvZ_3_15 = (TGraphErrors*)f_pvZ->Get("gd_v0_pvZ_3_15");

    // Getting diff. values of each TGraph
    Double_t *v0_corr = gr_v0_correc->GetY();
    Double_t *v0_trk_loose = gr_v0_trk_loose->GetY();
    Double_t *v0_trk_tight = gr_v0_trk_tight->GetY();
    Double_t *v0_cent_positive = gr_v0_cent_positive->GetY();
    Double_t *v0_cent_negative = gr_v0_cent_negative->GetY();
    Double_t *v0_pvZ_3 = gr_v0_pvZ_3->GetY();
    Double_t *v0_pvZ_3_15 = gr_v0_pvZ_3_15->GetY();

    // Putting diff. values into vectors, I'll divide into 2 vectors, each containing one variation of each unc. type
    // The strategy is comparing between them and select the greater value, putting into vector containing all uncs. of each point and from the containing each unc. type
    // (Correction application is an exception, since it has only one option)
    vector<vector<Double_t>> vec_uncs_v0_holder_one(nVar-1, vector<Double_t>(N_v0, 0.0)); // One: loose, positive, 3
        vec_uncs_v0_holder_one[0].assign(v0_trk_loose, v0_trk_loose + N_v0);
        vec_uncs_v0_holder_one[1].assign(v0_cent_positive, v0_cent_positive + N_v0);
        vec_uncs_v0_holder_one[2].assign(v0_pvZ_3, v0_pvZ_3 + N_v0);
    vector<vector<Double_t>> vec_uncs_v0_holder_two(nVar-1, vector<Double_t>(N_v0, 0.0)); // Two: tight, negative, 3_15
        vec_uncs_v0_holder_two[0].assign(v0_trk_tight, v0_trk_tight + N_v0);
        vec_uncs_v0_holder_two[1].assign(v0_cent_negative, v0_cent_negative + N_v0);
        vec_uncs_v0_holder_two[2].assign(v0_pvZ_3_15, v0_pvZ_3_15 + N_v0);

    // Putting correction application unc. in the first slot of the vectors of each point
    vector<Double_t> vec_uncs_v0_55(nVar, 0.0);
        vec_uncs_v0_55[0] = v0_corr[0];
    vector<Double_t> vec_uncs_v0_65(nVar, 0.0);
        vec_uncs_v0_65[0] = v0_corr[1];

    // Vectors that will contain the greatest value from your unc. type of both points
    vector<Double_t> vec_uncs_trk(N_v0, 0.0);
    vector<Double_t> vec_uncs_cent(N_v0, 0.0);
    vector<Double_t> vec_uncs_pvZ(N_v0, 0.0);
    vector<Double_t> vec_uncs_corr(N_v0, 0.0);
    vec_uncs_corr[0] = v0_corr[0]; vec_uncs_corr[1] = v0_corr[1];

    // Taking greater value of each uncertainty type and including in the vector of each point
    // and the in the vector of each unc. type
    for (int i=0; i<nVar-1; i++){
        // Centrality = 55%
        if (vec_uncs_v0_holder_one[i][0] > vec_uncs_v0_holder_two[i][0]){
            vec_uncs_v0_55[i+1] = vec_uncs_v0_holder_one[i][0];
            if (i == 0) vec_uncs_trk[0] = vec_uncs_v0_holder_one[i][0];
            if (i == 1) vec_uncs_cent[0] = vec_uncs_v0_holder_one[i][0];
            if (i == 2) vec_uncs_pvZ[0] = vec_uncs_v0_holder_one[i][0];
        } 
        if (vec_uncs_v0_holder_one[i][0] < vec_uncs_v0_holder_two[i][0]){
            vec_uncs_v0_55[i+1] = vec_uncs_v0_holder_two[i][0];
            if (i == 0) vec_uncs_trk[0] = vec_uncs_v0_holder_two[i][0];
            if (i == 1) vec_uncs_cent[0] = vec_uncs_v0_holder_two[i][0];
            if (i == 2) vec_uncs_pvZ[0] = vec_uncs_v0_holder_two[i][0];
        }
        // Centrality = 65%
        if (vec_uncs_v0_holder_one[i][1] > vec_uncs_v0_holder_two[i][1]){
            vec_uncs_v0_65[i+1] = vec_uncs_v0_holder_one[i][1];
            if (i == 0) vec_uncs_trk[1] = vec_uncs_v0_holder_one[i][1];
            if (i == 1) vec_uncs_cent[1] = vec_uncs_v0_holder_one[i][1];
            if (i == 2) vec_uncs_pvZ[1] = vec_uncs_v0_holder_one[i][1];
        }
        if (vec_uncs_v0_holder_one[i][1] < vec_uncs_v0_holder_two[i][1]){
            vec_uncs_v0_65[i+1] = vec_uncs_v0_holder_two[i][1];
            if (i == 0) vec_uncs_trk[1] = vec_uncs_v0_holder_two[i][1];
            if (i == 1) vec_uncs_cent[1] = vec_uncs_v0_holder_two[i][1];
            if (i == 2) vec_uncs_pvZ[1] = vec_uncs_v0_holder_two[i][1];
        }
    }

    // Taking the value of total uncertainty by summing them as sqrt(unc1^2 + unc2^2)
    double unc_v0_55 = QuadSum(vec_uncs_v0_55);
    double unc_v0_65 = QuadSum(vec_uncs_v0_65);

    // Putting the total uncertainty into a single vector containg both points
    vector<double> vec_unc_v0(2, 0.0);
    vec_unc_v0[0] = unc_v0_55; vec_unc_v0[1] = unc_v0_65;

    f_corr->Close(); f_trk->Close(); f_cent->Close(); f_pvZ->Close();

    TFile *f_main = TFile::Open("./Data/Figures/main.root");

    // Opening main file to take the v0 values
    TGraphErrors *gr_v0_55 = (TGraphErrors*)f_main->Get("v0_55_1");
    TGraphErrors *gr_v0_65 = (TGraphErrors*)f_main->Get("v0_65_1");

    Double_t *v0_55 = gr_v0_55->GetY();
    Double_t *v0_65 = gr_v0_65->GetY();

    vector<Double_t> vec_x_v0(N_v0, 0.0);
    vector<Double_t> vec_v0(N_v0, 0.0);
    vector<Double_t> vec_ex(N_v0, 0.0);

    vec_v0[0] = v0_55[0]; vec_v0[1] = v0_65[0];
    vec_x_v0[0] = 55.0; vec_x_v0[1] = 65.0;
    vec_ex[0] = 0.1; vec_ex[1] = 0.1;

    // Creating a new TGraphErrors containing v0 values from the main file and the systematic uncertainties in EY
    TGraphErrors *gr_v0_total = new TGraphErrors(N_v0, vec_x_v0.data(), vec_v0.data(), vec_ex.data(), vec_unc_v0.data());
    TGraphErrors *gr_v0_corr = new TGraphErrors(N_v0, vec_x_v0.data(), vec_v0.data(), vec_ex.data(), vec_uncs_corr.data());
    TGraphErrors *gr_v0_trk = new TGraphErrors(N_v0, vec_x_v0.data(), vec_v0.data(), vec_ex.data(), vec_uncs_trk.data());
    TGraphErrors *gr_v0_cent = new TGraphErrors(N_v0, vec_x_v0.data(), vec_v0.data(), vec_ex.data(), vec_uncs_cent.data());
    TGraphErrors *gr_v0_pvZ = new TGraphErrors(N_v0, vec_x_v0.data(), vec_v0.data(), vec_ex.data(), vec_uncs_pvZ.data());

    TFile *sf = new TFile("./Data/Systematics/SystUncs.root", "UPDATE");
    gr_v0_total->SetName("gr_v0_total"); gr_v0_total->Write();
    gr_v0_corr->SetName("gr_v0_corr"); gr_v0_corr->Write();
    gr_v0_trk->SetName("gr_v0_trk"); gr_v0_trk->Write();
    gr_v0_cent->SetName("gr_v0_cent"); gr_v0_cent->Write();
    gr_v0_pvZ->SetName("gr_v0_pvZ"); gr_v0_pvZ->Write();
    sf->Close();
}

void Evaluate_v0pT(){

    // Opening diff. files
    TFile *f_corr = TFile::Open("./Data/Systematics/CorrectionApplicationUnc.root");
    TFile *f_trk = TFile::Open("./Data/Systematics/TrackSelectionUnc.root");
    TFile *f_cent = TFile::Open("./Data/Systematics/CentralityFluctuationUnc.root");
    TFile *f_pvZ = TFile::Open("./Data/Systematics/ZvertexPositionUnc.root");

    // Loading TGraphs from TFiles, each one has N points
    TGraph *gr_v0pt_5060_correc = (TGraph*)f_corr->Get("gd_v0pt_5060_nocorrec"); // 50-60%
    TGraphErrors *gr_v0pt_5060_trk_loose = (TGraphErrors*)f_trk->Get("gd_v0pt_5060_loose");
    TGraphErrors *gr_v0pt_5060_trk_tight = (TGraphErrors*)f_trk->Get("gd_v0pt_5060_tight");
    TGraphErrors *gr_v0pt_5060_cent_positive = (TGraphErrors*)f_cent->Get("gd_v0pt_5060_positive");
    TGraphErrors *gr_v0pt_5060_cent_negative = (TGraphErrors*)f_cent->Get("gd_v0pt_5060_negative");
    TGraphErrors *gr_v0pt_5060_pvZ_3 = (TGraphErrors*)f_pvZ->Get("gd_v0pt_5060_pvZ_3");
    TGraphErrors *gr_v0pt_5060_pvZ_3_15 = (TGraphErrors*)f_pvZ->Get("gd_v0pt_5060_pvZ_3_15");
    TGraph *gr_v0pt_6070_correc = (TGraph*)f_corr->Get("gd_v0pt_6070_nocorrec"); // 60-70%
    TGraphErrors *gr_v0pt_6070_trk_loose = (TGraphErrors*)f_trk->Get("gd_v0pt_6070_loose");
    TGraphErrors *gr_v0pt_6070_trk_tight = (TGraphErrors*)f_trk->Get("gd_v0pt_6070_tight");
    TGraphErrors *gr_v0pt_6070_cent_positive = (TGraphErrors*)f_cent->Get("gd_v0pt_6070_positive");
    TGraphErrors *gr_v0pt_6070_cent_negative = (TGraphErrors*)f_cent->Get("gd_v0pt_6070_negative");
    TGraphErrors *gr_v0pt_6070_pvZ_3 = (TGraphErrors*)f_pvZ->Get("gd_v0pt_6070_pvZ_3");
    TGraphErrors *gr_v0pt_6070_pvZ_3_15 = (TGraphErrors*)f_pvZ->Get("gd_v0pt_6070_pvZ_3_15");

    int N = gr_v0pt_5060_correc->GetN();
    int nVar = 4;

    gStyle->SetOptFit(1111);
    
    auto PlotAndFit = [&](TCanvas* c, int pad, TGraph* g, TString title) {
        c->cd(pad);
        gPad->SetBottomMargin(0.12);
        gPad->SetLeftMargin(0.12);
        g->SetTitle(title);
        g->SetMarkerStyle(20);
        g->SetMarkerSize(0.8);
        g->Fit("pol1", "Q");
        g->Draw("AP");
    };

    TCanvas *c_5060 = new TCanvas("c_5060", "Sistematicos v0pT 50-60%", 1200, 800);
    c_5060->Divide(3, 2);

    PlotAndFit(c_5060, 1, gr_v0pt_5060_trk_loose,    "50-60% Trk Loose");
    PlotAndFit(c_5060, 2, gr_v0pt_5060_trk_tight,    "50-60% Trk Tight");
    PlotAndFit(c_5060, 3, gr_v0pt_5060_cent_positive,"50-60% Cent Positive");
    PlotAndFit(c_5060, 4, gr_v0pt_5060_cent_negative,"50-60% Cent Negative");
    PlotAndFit(c_5060, 5, gr_v0pt_5060_pvZ_3,        "50-60% pvZ < 3");
    PlotAndFit(c_5060, 6, gr_v0pt_5060_pvZ_3_15,     "50-60% 3 < pvZ < 15");

    c_5060->SaveAs("./Plots/Systematics/Systematics_Fits_v0pT_5060.pdf");
    delete c_5060;

 
    TCanvas *c_6070 = new TCanvas("c_6070", "Sistematicos v0pT 60-70%", 1200, 800);
    c_6070->Divide(3, 2); 

    PlotAndFit(c_6070, 1, gr_v0pt_6070_trk_loose,    "60-70% Trk Loose");
    PlotAndFit(c_6070, 2, gr_v0pt_6070_trk_tight,    "60-70% Trk Tight");
    PlotAndFit(c_6070, 3, gr_v0pt_6070_cent_positive,"60-70% Cent Positive");
    PlotAndFit(c_6070, 4, gr_v0pt_6070_cent_negative,"60-70% Cent Negative");
    PlotAndFit(c_6070, 5, gr_v0pt_6070_pvZ_3,        "60-70% pvZ < 3");
    PlotAndFit(c_6070, 6, gr_v0pt_6070_pvZ_3_15,     "60-70% 3 < pvZ < 15");

    c_6070->SaveAs("./Plots/Systematics/Systematics_Fits_v0pT_6070.pdf");
    delete c_6070;

    // Getting fit parameters
    TF1 *fit_v0pt_5060_trk_loose = gr_v0pt_5060_trk_loose->GetFunction("pol1"); // 50-60%
    TF1 *fit_v0pt_5060_trk_tight = gr_v0pt_5060_trk_tight->GetFunction("pol1");
    TF1 *fit_v0pt_5060_cent_positive = gr_v0pt_5060_cent_positive->GetFunction("pol1");
    TF1 *fit_v0pt_5060_cent_negative = gr_v0pt_5060_cent_negative->GetFunction("pol1");
    TF1 *fit_v0pt_5060_pvZ_3 = gr_v0pt_5060_pvZ_3->GetFunction("pol1");
    TF1 *fit_v0pt_5060_pvZ_3_15 = gr_v0pt_5060_pvZ_3_15->GetFunction("pol1");
    TF1 *fit_v0pt_6070_trk_loose = gr_v0pt_6070_trk_loose->GetFunction("pol1"); // 60-70%
    TF1 *fit_v0pt_6070_trk_tight = gr_v0pt_6070_trk_tight->GetFunction("pol1");
    TF1 *fit_v0pt_6070_cent_positive = gr_v0pt_6070_cent_positive->GetFunction("pol1");
    TF1 *fit_v0pt_6070_cent_negative = gr_v0pt_6070_cent_negative->GetFunction("pol1");
    TF1 *fit_v0pt_6070_pvZ_3 = gr_v0pt_6070_pvZ_3->GetFunction("pol1");
    TF1 *fit_v0pt_6070_pvZ_3_15 = gr_v0pt_6070_pvZ_3_15->GetFunction("pol1");

    vector<TF1*> vec_fits_5060_one(nVar-1, nullptr); // One: loose, positive, 3
        vec_fits_5060_one[0] = fit_v0pt_5060_trk_loose;
        vec_fits_5060_one[1] = fit_v0pt_5060_cent_positive;
        vec_fits_5060_one[2] = fit_v0pt_5060_pvZ_3;
    vector<TF1*> vec_fits_5060_two(nVar-1, nullptr); // Two: tight, negative, 3_15
        vec_fits_5060_two[0] = fit_v0pt_5060_trk_tight;
        vec_fits_5060_two[1] = fit_v0pt_5060_cent_negative;
        vec_fits_5060_two[2] = fit_v0pt_5060_pvZ_3_15;
    vector<TF1*> vec_fits_6070_one(nVar-1, nullptr); // One: loose, positive, 3
        vec_fits_6070_one[0] = fit_v0pt_6070_trk_loose;
        vec_fits_6070_one[1] = fit_v0pt_6070_cent_positive;
        vec_fits_6070_one[2] = fit_v0pt_6070_pvZ_3;
    vector<TF1*> vec_fits_6070_two(nVar-1, nullptr); // Two: tight, negative, 3_15
        vec_fits_6070_two[0] = fit_v0pt_6070_trk_tight;
        vec_fits_6070_two[1] = fit_v0pt_6070_cent_negative;
        vec_fits_6070_two[2] = fit_v0pt_6070_pvZ_3_15;

     // Getting diff. values of each TGraph
    Double_t *v0pt_5060_corr = gr_v0pt_5060_correc->GetY(); // 50-60%
    Double_t *v0pt_5060_trk_loose = gr_v0pt_5060_trk_loose->GetY();
    Double_t *v0pt_5060_trk_tight = gr_v0pt_5060_trk_tight->GetY();
    Double_t *v0pt_5060_cent_positive = gr_v0pt_5060_cent_positive->GetY();
    Double_t *v0pt_5060_cent_negative = gr_v0pt_5060_cent_negative->GetY();
    Double_t *v0pt_5060_pvZ_3 = gr_v0pt_5060_pvZ_3->GetY();
    Double_t *v0pt_5060_pvZ_3_15 = gr_v0pt_5060_pvZ_3_15->GetY();
    Double_t *v0pt_6070_corr = gr_v0pt_6070_correc->GetY(); // 60-70%
    Double_t *v0pt_6070_trk_loose = gr_v0pt_6070_trk_loose->GetY();
    Double_t *v0pt_6070_trk_tight = gr_v0pt_6070_trk_tight->GetY();
    Double_t *v0pt_6070_cent_positive = gr_v0pt_6070_cent_positive->GetY();
    Double_t *v0pt_6070_cent_negative = gr_v0pt_6070_cent_negative->GetY();
    Double_t *v0pt_6070_pvZ_3 = gr_v0pt_6070_pvZ_3->GetY();
    Double_t *v0pt_6070_pvZ_3_15 = gr_v0pt_6070_pvZ_3_15->GetY();
    
    vector<Double_t> vec_unc_v0pt_5060_corr(N, 0.0); // 50-60%
        vec_unc_v0pt_5060_corr.assign(v0pt_5060_corr, v0pt_5060_corr + N);
    vector<Double_t> vec_unc_v0pt_6070_corr(N, 0.0); // 60-70%
        vec_unc_v0pt_6070_corr.assign(v0pt_6070_corr, v0pt_6070_corr + N);
    
    f_corr->Close(); f_trk->Close(); f_cent->Close(); f_pvZ->Close(); 

    TFile *f_main = TFile::Open("./Data/Figures/main.root");
    TGraphErrors *gr_v0pt_5060 = (TGraphErrors*)f_main->Get("v0pt_ptref_1_5060");
    TGraphErrors *gr_v0pt_6070 = (TGraphErrors*)f_main->Get("v0pt_ptref_1_6070");

    Double_t *x_v0pt = gr_v0pt_5060->GetX();
    Double_t *y_v0pt_5060 = gr_v0pt_5060->GetY();
    Double_t *y_v0pt_6070 = gr_v0pt_6070->GetY();

    vector<Double_t> vec_x_v0pt(N, 0.0); vec_x_v0pt.assign(x_v0pt, x_v0pt + N);
    vector<Double_t> vec_y_v0pt_5060(N, 0.0); vec_y_v0pt_5060.assign(y_v0pt_5060, y_v0pt_5060 + N);
    vector<Double_t> vec_y_v0pt_6070(N, 0.0); vec_y_v0pt_6070.assign(y_v0pt_6070, y_v0pt_6070 + N);

    // Creating v0pt vector to take the greater unc. type value of each point and putting correction application unc. in the first slot 
    vector<vector<Double_t>> vec_uncs_v0pt_5060(nVar, vector<Double_t>(N, 0.0));
    vector<vector<Double_t>> vec_uncs_v0pt_6070(nVar, vector<Double_t>(N, 0.0));

    for (int i=0; i<nVar; i++){
        for (int j=0; j<N; j++){
            if (i==0){
                vec_uncs_v0pt_5060[i][j] = vec_unc_v0pt_5060_corr[j];
                vec_uncs_v0pt_6070[i][j] = vec_unc_v0pt_6070_corr[j];
            } else{
                if (vec_fits_5060_one[i-1]->Eval(vec_x_v0pt[j]) > vec_fits_5060_two[i-1]->Eval(vec_x_v0pt[j])) vec_uncs_v0pt_5060[i][j] = vec_fits_5060_one[i-1]->Eval(vec_x_v0pt[j]);
                if (vec_fits_5060_one[i-1]->Eval(vec_x_v0pt[j]) < vec_fits_5060_two[i-1]->Eval(vec_x_v0pt[j])) vec_uncs_v0pt_5060[i][j] = vec_fits_5060_two[i-1]->Eval(vec_x_v0pt[j]);
                if (vec_fits_6070_one[i-1]->Eval(vec_x_v0pt[j]) > vec_fits_6070_two[i-1]->Eval(vec_x_v0pt[j])) vec_uncs_v0pt_6070[i][j] = vec_fits_6070_one[i-1]->Eval(vec_x_v0pt[j]);
                if (vec_fits_6070_one[i-1]->Eval(vec_x_v0pt[j]) < vec_fits_6070_two[i-1]->Eval(vec_x_v0pt[j])) vec_uncs_v0pt_6070[i][j] = vec_fits_6070_two[i-1]->Eval(vec_x_v0pt[j]);
            }
        }
    }

    vector<Double_t> vec_ex_left_v0pt(N, 0.0); for (int i=0; i<N; i++) vec_ex_left_v0pt[i] = vec_x_v0pt[i] - (vec_x_v0pt[i] / 1.025);
    vector<Double_t> vec_ex_right_v0pt(N, 0.0); for (int i=0; i<N; i++) vec_ex_right_v0pt[i] = (vec_x_v0pt[i] * 1.025) - vec_x_v0pt[i];
    vector<Double_t> vec_unc_v0pt_5060_total = VecQuadSum(vec_uncs_v0pt_5060);
    vector<Double_t> vec_unc_v0pt_6070_total = VecQuadSum(vec_uncs_v0pt_6070);

    TGraphAsymmErrors *gr_v0pt_5060_total = new TGraphAsymmErrors(N, vec_x_v0pt.data(), vec_y_v0pt_5060.data(), vec_ex_left_v0pt.data(), vec_ex_right_v0pt.data(), vec_unc_v0pt_5060_total.data(), vec_unc_v0pt_5060_total.data()); // 50-60%
    TGraphAsymmErrors *gr_v0pt_5060_corr = new TGraphAsymmErrors(N, vec_x_v0pt.data(), vec_y_v0pt_5060.data(), vec_ex_left_v0pt.data(), vec_ex_right_v0pt.data(), vec_uncs_v0pt_5060[0].data(), vec_uncs_v0pt_5060[0].data());
    TGraphAsymmErrors *gr_v0pt_5060_trk = new TGraphAsymmErrors(N, vec_x_v0pt.data(), vec_y_v0pt_5060.data(), vec_ex_left_v0pt.data(), vec_ex_right_v0pt.data(), vec_uncs_v0pt_5060[1].data(), vec_uncs_v0pt_5060[1].data());
    TGraphAsymmErrors *gr_v0pt_5060_cent = new TGraphAsymmErrors(N, vec_x_v0pt.data(), vec_y_v0pt_5060.data(), vec_ex_left_v0pt.data(), vec_ex_right_v0pt.data(), vec_uncs_v0pt_5060[2].data(), vec_uncs_v0pt_5060[2].data());
    TGraphAsymmErrors *gr_v0pt_5060_pvZ = new TGraphAsymmErrors(N, vec_x_v0pt.data(), vec_y_v0pt_5060.data(), vec_ex_left_v0pt.data(), vec_ex_right_v0pt.data(), vec_uncs_v0pt_5060[3].data(), vec_uncs_v0pt_5060[3].data());
    TGraphAsymmErrors *gr_v0pt_6070_total = new TGraphAsymmErrors(N, vec_x_v0pt.data(), vec_y_v0pt_6070.data(), vec_ex_left_v0pt.data(), vec_ex_right_v0pt.data(), vec_unc_v0pt_6070_total.data(), vec_unc_v0pt_6070_total.data()); // 60-70%
    TGraphAsymmErrors *gr_v0pt_6070_corr = new TGraphAsymmErrors(N, vec_x_v0pt.data(), vec_y_v0pt_6070.data(), vec_ex_left_v0pt.data(), vec_ex_right_v0pt.data(), vec_uncs_v0pt_6070[0].data(), vec_uncs_v0pt_6070[0].data());
    TGraphAsymmErrors *gr_v0pt_6070_trk = new TGraphAsymmErrors(N, vec_x_v0pt.data(), vec_y_v0pt_6070.data(), vec_ex_left_v0pt.data(), vec_ex_right_v0pt.data(), vec_uncs_v0pt_6070[1].data(), vec_uncs_v0pt_6070[1].data());
    TGraphAsymmErrors *gr_v0pt_6070_cent = new TGraphAsymmErrors(N, vec_x_v0pt.data(), vec_y_v0pt_6070.data(), vec_ex_left_v0pt.data(), vec_ex_right_v0pt.data(), vec_uncs_v0pt_6070[2].data(), vec_uncs_v0pt_6070[2].data());
    TGraphAsymmErrors *gr_v0pt_6070_pvZ = new TGraphAsymmErrors(N, vec_x_v0pt.data(), vec_y_v0pt_6070.data(), vec_ex_left_v0pt.data(), vec_ex_right_v0pt.data(), vec_uncs_v0pt_6070[3].data(), vec_uncs_v0pt_6070[3].data());

    TFile *sf = new TFile("./Data/Systematics/SystUncs.root", "UPDATE");
    gr_v0pt_5060_total->SetName("gr_v0pt_5060_total"); gr_v0pt_5060_total->Write(); // 50-60%
    gr_v0pt_5060_corr->SetName("gr_v0pt_5060_corr"); gr_v0pt_5060_corr->Write();
    gr_v0pt_5060_trk->SetName("gr_v0pt_5060_trk"); gr_v0pt_5060_trk->Write();
    gr_v0pt_5060_cent->SetName("gr_v0pt_5060_cent"); gr_v0pt_5060_cent->Write();
    gr_v0pt_5060_pvZ->SetName("gr_v0pt_5060_pvZ"); gr_v0pt_5060_pvZ->Write();
    gr_v0pt_6070_total->SetName("gr_v0pt_6070_total"); gr_v0pt_6070_total->Write(); // 60-70%
    gr_v0pt_6070_corr->SetName("gr_v0pt_6070_corr"); gr_v0pt_6070_corr->Write();
    gr_v0pt_6070_trk->SetName("gr_v0pt_6070_trk"); gr_v0pt_6070_trk->Write();
    gr_v0pt_6070_cent->SetName("gr_v0pt_6070_cent"); gr_v0pt_6070_cent->Write();
    gr_v0pt_6070_pvZ->SetName("gr_v0pt_6070_pvZ"); gr_v0pt_6070_pvZ->Write();
    sf->Close();
}

void Evaluate_sv0pT(){

    // Opening diff. files
    TFile *f_corr = TFile::Open("./Data/Systematics/CorrectionApplicationUnc.root");
    TFile *f_trk = TFile::Open("./Data/Systematics/TrackSelectionUnc.root");
    TFile *f_cent = TFile::Open("./Data/Systematics/CentralityFluctuationUnc.root");
    TFile *f_pvZ = TFile::Open("./Data/Systematics/ZvertexPositionUnc.root");

    // Loading TGraphs from TFiles, each one has N points
    TGraph *gr_sv0pt_5060_correc = (TGraph*)f_corr->Get("gd_sv0pt_5060_nocorrec"); // 50-60%
    TGraphErrors *gr_sv0pt_5060_trk_loose = (TGraphErrors*)f_trk->Get("gd_sv0pt_5060_loose");
    TGraphErrors *gr_sv0pt_5060_trk_tight = (TGraphErrors*)f_trk->Get("gd_sv0pt_5060_tight");
    TGraphErrors *gr_sv0pt_5060_cent_positive = (TGraphErrors*)f_cent->Get("gd_sv0pt_5060_positive");
    TGraphErrors *gr_sv0pt_5060_cent_negative = (TGraphErrors*)f_cent->Get("gd_sv0pt_5060_negative");
    TGraphErrors *gr_sv0pt_5060_pvZ_3 = (TGraphErrors*)f_pvZ->Get("gd_sv0pt_5060_pvZ_3");
    TGraphErrors *gr_sv0pt_5060_pvZ_3_15 = (TGraphErrors*)f_pvZ->Get("gd_sv0pt_5060_pvZ_3_15");
    TGraph *gr_sv0pt_6070_correc = (TGraph*)f_corr->Get("gd_sv0pt_6070_nocorrec"); // 60-70%
    TGraphErrors *gr_sv0pt_6070_trk_loose = (TGraphErrors*)f_trk->Get("gd_sv0pt_6070_loose");
    TGraphErrors *gr_sv0pt_6070_trk_tight = (TGraphErrors*)f_trk->Get("gd_sv0pt_6070_tight");
    TGraphErrors *gr_sv0pt_6070_cent_positive = (TGraphErrors*)f_cent->Get("gd_sv0pt_6070_positive");
    TGraphErrors *gr_sv0pt_6070_cent_negative = (TGraphErrors*)f_cent->Get("gd_sv0pt_6070_negative");
    TGraphErrors *gr_sv0pt_6070_pvZ_3 = (TGraphErrors*)f_pvZ->Get("gd_sv0pt_6070_pvZ_3");
    TGraphErrors *gr_sv0pt_6070_pvZ_3_15 = (TGraphErrors*)f_pvZ->Get("gd_sv0pt_6070_pvZ_3_15");

    int N = gr_sv0pt_5060_correc->GetN();
    int nVar = 4;

    gStyle->SetOptFit(1111);
    
    auto PlotAndFitS = [&](TCanvas* c, int pad, TGraph* g, TString title) {
        c->cd(pad);
        gPad->SetBottomMargin(0.12);
        gPad->SetLeftMargin(0.12);
        g->SetTitle(title);
        g->SetMarkerStyle(20);
        g->SetMarkerSize(0.8);
        g->Fit("pol1", "Q");
        g->Draw("AP");
    };

    TCanvas *cs_5060 = new TCanvas("cs_5060", "Sistematics sv0pT 50-60%", 1200, 800);
    cs_5060->Divide(3, 2); 

    PlotAndFitS(cs_5060, 1, gr_sv0pt_5060_trk_loose,    "sv0pT 50-60% Trk Loose");
    PlotAndFitS(cs_5060, 2, gr_sv0pt_5060_trk_tight,    "sv0pT 50-60% Trk Tight");
    PlotAndFitS(cs_5060, 3, gr_sv0pt_5060_cent_positive,"sv0pT 50-60% Cent Positive");
    PlotAndFitS(cs_5060, 4, gr_sv0pt_5060_cent_negative,"sv0pT 50-60% Cent Negative");
    PlotAndFitS(cs_5060, 5, gr_sv0pt_5060_pvZ_3,        "sv0pT 50-60% pvZ < 3");
    PlotAndFitS(cs_5060, 6, gr_sv0pt_5060_pvZ_3_15,     "sv0pT 50-60% 3 < pvZ < 15");

    cs_5060->SaveAs("./Plots/Systematics/Systematics_Fits_sv0pT_5060.pdf");
    delete cs_5060;

    TCanvas *cs_6070 = new TCanvas("cs_6070", "Sistematics sv0pT 60-70%", 1200, 800);
    cs_6070->Divide(3, 2);

    PlotAndFitS(cs_6070, 1, gr_sv0pt_6070_trk_loose,    "sv0pT 60-70% Trk Loose");
    PlotAndFitS(cs_6070, 2, gr_sv0pt_6070_trk_tight,    "sv0pT 60-70% Trk Tight");
    PlotAndFitS(cs_6070, 3, gr_sv0pt_6070_cent_positive,"sv0pT 60-70% Cent Positive");
    PlotAndFitS(cs_6070, 4, gr_sv0pt_6070_cent_negative,"sv0pT 60-70% Cent Negative");
    PlotAndFitS(cs_6070, 5, gr_sv0pt_6070_pvZ_3,        "sv0pT 60-70% pvZ < 3");
    PlotAndFitS(cs_6070, 6, gr_sv0pt_6070_pvZ_3_15,     "sv0pT 60-70% 3 < pvZ < 15");

    cs_6070->SaveAs("./Plots/Systematics/Systematics_Fits_sv0pT_6070.pdf");
    delete cs_6070;

    // Getting fit parameters
    TF1 *fit_sv0pt_5060_trk_loose = gr_sv0pt_5060_trk_loose->GetFunction("pol1"); // 50-60%
    TF1 *fit_sv0pt_5060_trk_tight = gr_sv0pt_5060_trk_tight->GetFunction("pol1");
    TF1 *fit_sv0pt_5060_cent_positive = gr_sv0pt_5060_cent_positive->GetFunction("pol1");
    TF1 *fit_sv0pt_5060_cent_negative = gr_sv0pt_5060_cent_negative->GetFunction("pol1");
    TF1 *fit_sv0pt_5060_pvZ_3 = gr_sv0pt_5060_pvZ_3->GetFunction("pol1");
    TF1 *fit_sv0pt_5060_pvZ_3_15 = gr_sv0pt_5060_pvZ_3_15->GetFunction("pol1");
    TF1 *fit_sv0pt_6070_trk_loose = gr_sv0pt_6070_trk_loose->GetFunction("pol1"); // 60-70%
    TF1 *fit_sv0pt_6070_trk_tight = gr_sv0pt_6070_trk_tight->GetFunction("pol1");
    TF1 *fit_sv0pt_6070_cent_positive = gr_sv0pt_6070_cent_positive->GetFunction("pol1");
    TF1 *fit_sv0pt_6070_cent_negative = gr_sv0pt_6070_cent_negative->GetFunction("pol1");
    TF1 *fit_sv0pt_6070_pvZ_3 = gr_sv0pt_6070_pvZ_3->GetFunction("pol1");
    TF1 *fit_sv0pt_6070_pvZ_3_15 = gr_sv0pt_6070_pvZ_3_15->GetFunction("pol1");

    vector<TF1*> vec_fits_5060_one(nVar-1, nullptr); // One: loose, positive, 3
        vec_fits_5060_one[0] = fit_sv0pt_5060_trk_loose;
        vec_fits_5060_one[1] = fit_sv0pt_5060_cent_positive;
        vec_fits_5060_one[2] = fit_sv0pt_5060_pvZ_3;
    vector<TF1*> vec_fits_5060_two(nVar-1, nullptr); // Two: tight, negative, 3_15
        vec_fits_5060_two[0] = fit_sv0pt_5060_trk_tight;
        vec_fits_5060_two[1] = fit_sv0pt_5060_cent_negative;
        vec_fits_5060_two[2] = fit_sv0pt_5060_pvZ_3_15;
    vector<TF1*> vec_fits_6070_one(nVar-1, nullptr); // One: loose, positive, 3
        vec_fits_6070_one[0] = fit_sv0pt_6070_trk_loose;
        vec_fits_6070_one[1] = fit_sv0pt_6070_cent_positive;
        vec_fits_6070_one[2] = fit_sv0pt_6070_pvZ_3;
    vector<TF1*> vec_fits_6070_two(nVar-1, nullptr); // Two: tight, negative, 3_15
        vec_fits_6070_two[0] = fit_sv0pt_6070_trk_tight;
        vec_fits_6070_two[1] = fit_sv0pt_6070_cent_negative;
        vec_fits_6070_two[2] = fit_sv0pt_6070_pvZ_3_15;

     // Getting diff. values of each TGraph
    Double_t *sv0pt_5060_corr = gr_sv0pt_5060_correc->GetY(); // 50-60%
    Double_t *sv0pt_5060_trk_loose = gr_sv0pt_5060_trk_loose->GetY();
    Double_t *sv0pt_5060_trk_tight = gr_sv0pt_5060_trk_tight->GetY();
    Double_t *sv0pt_5060_cent_positive = gr_sv0pt_5060_cent_positive->GetY();
    Double_t *sv0pt_5060_cent_negative = gr_sv0pt_5060_cent_negative->GetY();
    Double_t *sv0pt_5060_pvZ_3 = gr_sv0pt_5060_pvZ_3->GetY();
    Double_t *sv0pt_5060_pvZ_3_15 = gr_sv0pt_5060_pvZ_3_15->GetY();
    Double_t *sv0pt_6070_corr = gr_sv0pt_6070_correc->GetY(); // 60-70%
    Double_t *sv0pt_6070_trk_loose = gr_sv0pt_6070_trk_loose->GetY();
    Double_t *sv0pt_6070_trk_tight = gr_sv0pt_6070_trk_tight->GetY();
    Double_t *sv0pt_6070_cent_positive = gr_sv0pt_6070_cent_positive->GetY();
    Double_t *sv0pt_6070_cent_negative = gr_sv0pt_6070_cent_negative->GetY();
    Double_t *sv0pt_6070_pvZ_3 = gr_sv0pt_6070_pvZ_3->GetY();
    Double_t *sv0pt_6070_pvZ_3_15 = gr_sv0pt_6070_pvZ_3_15->GetY();
    
    vector<Double_t> vec_unc_sv0pt_5060_corr(N, 0.0); // 50-60%
        vec_unc_sv0pt_5060_corr.assign(sv0pt_5060_corr, sv0pt_5060_corr + N);
    vector<Double_t> vec_unc_sv0pt_6070_corr(N, 0.0); // 60-70%
        vec_unc_sv0pt_6070_corr.assign(sv0pt_6070_corr, sv0pt_6070_corr + N);
    
    f_corr->Close(); f_trk->Close(); f_cent->Close(); f_pvZ->Close(); 

    TFile *f_main = TFile::Open("./Data/Figures/main.root");
    TGraphErrors *gr_sv0pt_5060 = (TGraphErrors*)f_main->Get("sv0pt_ptref_1_5060");
    TGraphErrors *gr_sv0pt_6070 = (TGraphErrors*)f_main->Get("sv0pt_ptref_1_6070");

    Double_t *x_sv0pt = gr_sv0pt_5060->GetX();
    Double_t *y_sv0pt_5060 = gr_sv0pt_5060->GetY();
    Double_t *y_sv0pt_6070 = gr_sv0pt_6070->GetY();

    vector<Double_t> vec_x_sv0pt(N, 0.0); vec_x_sv0pt.assign(x_sv0pt, x_sv0pt + N);
    vector<Double_t> vec_y_sv0pt_5060(N, 0.0); vec_y_sv0pt_5060.assign(y_sv0pt_5060, y_sv0pt_5060 + N);
    vector<Double_t> vec_y_sv0pt_6070(N, 0.0); vec_y_sv0pt_6070.assign(y_sv0pt_6070, y_sv0pt_6070 + N);

    // Creating sv0pt vector to take the greater unc. type value of each point and putting correction application unc. in the first slot 
    vector<vector<Double_t>> vec_uncs_sv0pt_5060(nVar, vector<Double_t>(N, 0.0));
    vector<vector<Double_t>> vec_uncs_sv0pt_6070(nVar, vector<Double_t>(N, 0.0));

    for (int i=0; i<nVar; i++){
        for (int j=0; j<N; j++){
            if (i==0){
                vec_uncs_sv0pt_5060[i][j] = vec_unc_sv0pt_5060_corr[j];
                vec_uncs_sv0pt_6070[i][j] = vec_unc_sv0pt_6070_corr[j];
            } else{
                if (vec_fits_5060_one[i-1]->Eval(vec_x_sv0pt[j]) > vec_fits_5060_two[i-1]->Eval(vec_x_sv0pt[j])) vec_uncs_sv0pt_5060[i][j] = vec_fits_5060_one[i-1]->Eval(vec_x_sv0pt[j]);
                if (vec_fits_5060_one[i-1]->Eval(vec_x_sv0pt[j]) < vec_fits_5060_two[i-1]->Eval(vec_x_sv0pt[j])) vec_uncs_sv0pt_5060[i][j] = vec_fits_5060_two[i-1]->Eval(vec_x_sv0pt[j]);
                if (vec_fits_6070_one[i-1]->Eval(vec_x_sv0pt[j]) > vec_fits_6070_two[i-1]->Eval(vec_x_sv0pt[j])) vec_uncs_sv0pt_6070[i][j] = vec_fits_6070_one[i-1]->Eval(vec_x_sv0pt[j]);
                if (vec_fits_6070_one[i-1]->Eval(vec_x_sv0pt[j]) < vec_fits_6070_two[i-1]->Eval(vec_x_sv0pt[j])) vec_uncs_sv0pt_6070[i][j] = vec_fits_6070_two[i-1]->Eval(vec_x_sv0pt[j]);
            }
        }
    }

    vector<Double_t> vec_ex_left_sv0pt(N, 0.0); for (int i=0; i<N; i++) vec_ex_left_sv0pt[i] = vec_x_sv0pt[i] - (vec_x_sv0pt[i] / 1.025);
    vector<Double_t> vec_ex_right_sv0pt(N, 0.0); for (int i=0; i<N; i++) vec_ex_right_sv0pt[i] = (vec_x_sv0pt[i] * 1.025) - vec_x_sv0pt[i];
    vector<Double_t> vec_unc_sv0pt_5060_total = VecQuadSum(vec_uncs_sv0pt_5060);
    vector<Double_t> vec_unc_sv0pt_6070_total = VecQuadSum(vec_uncs_sv0pt_6070);

    TGraphAsymmErrors *gr_sv0pt_5060_total = new TGraphAsymmErrors(N, vec_x_sv0pt.data(), vec_y_sv0pt_5060.data(), vec_ex_left_sv0pt.data(), vec_ex_right_sv0pt.data(), vec_unc_sv0pt_5060_total.data(), vec_unc_sv0pt_5060_total.data()); // 50-60%
    TGraphAsymmErrors *gr_sv0pt_5060_corr = new TGraphAsymmErrors(N, vec_x_sv0pt.data(), vec_y_sv0pt_5060.data(), vec_ex_left_sv0pt.data(), vec_ex_right_sv0pt.data(), vec_uncs_sv0pt_5060[0].data(), vec_uncs_sv0pt_5060[0].data());
    TGraphAsymmErrors *gr_sv0pt_5060_trk = new TGraphAsymmErrors(N, vec_x_sv0pt.data(), vec_y_sv0pt_5060.data(), vec_ex_left_sv0pt.data(), vec_ex_right_sv0pt.data(), vec_uncs_sv0pt_5060[1].data(), vec_uncs_sv0pt_5060[1].data());
    TGraphAsymmErrors *gr_sv0pt_5060_cent = new TGraphAsymmErrors(N, vec_x_sv0pt.data(), vec_y_sv0pt_5060.data(), vec_ex_left_sv0pt.data(), vec_ex_right_sv0pt.data(), vec_uncs_sv0pt_5060[2].data(), vec_uncs_sv0pt_5060[2].data());
    TGraphAsymmErrors *gr_sv0pt_5060_pvZ = new TGraphAsymmErrors(N, vec_x_sv0pt.data(), vec_y_sv0pt_5060.data(), vec_ex_left_sv0pt.data(), vec_ex_right_sv0pt.data(), vec_uncs_sv0pt_5060[3].data(), vec_uncs_sv0pt_5060[3].data());
    TGraphAsymmErrors *gr_sv0pt_6070_total = new TGraphAsymmErrors(N, vec_x_sv0pt.data(), vec_y_sv0pt_6070.data(), vec_ex_left_sv0pt.data(), vec_ex_right_sv0pt.data(), vec_unc_sv0pt_6070_total.data(), vec_unc_sv0pt_6070_total.data()); // 60-70%
    TGraphAsymmErrors *gr_sv0pt_6070_corr = new TGraphAsymmErrors(N, vec_x_sv0pt.data(), vec_y_sv0pt_6070.data(), vec_ex_left_sv0pt.data(), vec_ex_right_sv0pt.data(), vec_uncs_sv0pt_6070[0].data(), vec_uncs_sv0pt_6070[0].data());
    TGraphAsymmErrors *gr_sv0pt_6070_trk = new TGraphAsymmErrors(N, vec_x_sv0pt.data(), vec_y_sv0pt_6070.data(), vec_ex_left_sv0pt.data(), vec_ex_right_sv0pt.data(), vec_uncs_sv0pt_6070[1].data(), vec_uncs_sv0pt_6070[1].data());
    TGraphAsymmErrors *gr_sv0pt_6070_cent = new TGraphAsymmErrors(N, vec_x_sv0pt.data(), vec_y_sv0pt_6070.data(), vec_ex_left_sv0pt.data(), vec_ex_right_sv0pt.data(), vec_uncs_sv0pt_6070[2].data(), vec_uncs_sv0pt_6070[2].data());
    TGraphAsymmErrors *gr_sv0pt_6070_pvZ = new TGraphAsymmErrors(N, vec_x_sv0pt.data(), vec_y_sv0pt_6070.data(), vec_ex_left_sv0pt.data(), vec_ex_right_sv0pt.data(), vec_uncs_sv0pt_6070[3].data(), vec_uncs_sv0pt_6070[3].data());

    TFile *sf = new TFile("./Data/Systematics/SystUncs.root", "UPDATE");
    gr_sv0pt_5060_total->SetName("gr_sv0pt_5060_total"); gr_sv0pt_5060_total->Write(); // 50-60%
    gr_sv0pt_5060_corr->SetName("gr_sv0pt_5060_corr"); gr_sv0pt_5060_corr->Write();
    gr_sv0pt_5060_trk->SetName("gr_sv0pt_5060_trk"); gr_sv0pt_5060_trk->Write();
    gr_sv0pt_5060_cent->SetName("gr_sv0pt_5060_cent"); gr_sv0pt_5060_cent->Write();
    gr_sv0pt_5060_pvZ->SetName("gr_sv0pt_5060_pvZ"); gr_sv0pt_5060_pvZ->Write();
    gr_sv0pt_6070_total->SetName("gr_sv0pt_6070_total"); gr_sv0pt_6070_total->Write(); // 60-70%
    gr_sv0pt_6070_corr->SetName("gr_sv0pt_6070_corr"); gr_sv0pt_6070_corr->Write();
    gr_sv0pt_6070_trk->SetName("gr_sv0pt_6070_trk"); gr_sv0pt_6070_trk->Write();
    gr_sv0pt_6070_cent->SetName("gr_sv0pt_6070_cent"); gr_sv0pt_6070_cent->Write();
    gr_sv0pt_6070_pvZ->SetName("gr_sv0pt_6070_pvZ"); gr_sv0pt_6070_pvZ->Write();
    sf->Close();
}