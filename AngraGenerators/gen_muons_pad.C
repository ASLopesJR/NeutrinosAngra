#include "muons/muons_pad.cpp"

void gen_muons_pad(int N){

  Int_t numFiles = 1;
  Int_t seed     = 20191029;
  Int_t localseed;
  
  TString fileName   = "muons_130k_";
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
    muons(localseed,N,file);
  }

}
