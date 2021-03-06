#include "hadrons/protons.cpp"

void gen_protons(){

  Int_t N        = 2.1e3;
  Int_t numFiles = 2;
  Int_t seed     = 20131000;
  Int_t localseed;
  
  TString fileName   = "protons_21k_";
  TString filePrefix = "data/";
  TString fileAfix   = ".hepevt";
  TString file;
  
  for (Int_t i = 1; i<=numFiles; i++){
    file = filePrefix + fileName;
    localseed = seed + i;
    file += seed;
    file += "_";
    file += i;
    file += fileAfix;
    cout << file << endl;
    protons(localseed,N,file);
  }

}
