//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  Cosmic muons generator
//
//  Author: P.Chimenti
//
//  30-03-2012 v0.1, 04-06-2013 v0.9 
//
//--------------------------------------------------------------
//--------------------------------------------------------------

#include "TMatrixD.h"
#include "TVectorD.h"
#include "TMath.h"


#ifndef __CINT__
#include "Math/Point3D.h"
#include "Math/Vector3D.h"
#include "Math/Vector4D.h"
#include "Math/Rotation3D.h"
#include "Math/EulerAngles.h"
#include "Math/AxisAngle.h"
#include "Math/Quaternion.h"
#include "Math/RotationX.h"
#include "Math/RotationY.h"
#include "Math/RotationZ.h"
#include "Math/RotationZYX.h"
#include "Math/LorentzRotation.h"
#include "Math/Boost.h"
#include "Math/BoostX.h"
#include "Math/BoostY.h"
#include "Math/BoostZ.h"
#include "Math/Transform3D.h"
#include "Math/Plane3D.h"
#include "Math/VectorUtil.h"

using namespace ROOT::Math;

#endif

#include "muonSpectrum.cpp"

void muons(int INI_RAND=9999,int MAXEVENTS=10000, TString file="event.hepev")
{
	
  // ROOT initialization
#ifdef __CINT__
  gSystem->Load("libMathCore");
  using namespace ROOT::Math;
#endif

bool write = false;

//   gROOT->ProcessLine(".L muons/muonSpectrum.cpp");
  

  delete gRandom;	       
  gRandom = new TRandom3(INI_RAND);


  // constant definitions
  const Double_t pi = TMath::Pi();

  //geometric definitions
  const Double_t SURF_HALF_LEN=2.5;//units: m 
  const Double_t SURF_DIST=4.0;//units: m

  //physical definitions
  const Double_t muonMass = 0.1056583666;

  //
  // model from D.Reyna (hep-ph/0604145)
  //

  // set spectrum histo 
  TH1F* histoFlux =new TH1F ("MuonFlux","MuonFlux",10000,0.0,100.);
  for(int i=0; i<10000; i++){
    histoFlux->SetBinContent(i+1,muonSpec(0.005+i/100.,1.,0,0));// just vertical intensity
  }

  // just check histo
//   histoFlux->Draw();

  // now azimuthal angle
  double thetaIndex=2.;
  TF1 *thetaDistr = new TF1("Theta Distribution","TMath::Sin(x)*(TMath::Cos(x))**[0]",0,pi/2);
  thetaDistr->SetParameter(0,thetaIndex);

  // open out file
  ofstream outfile;
  outfile.open(file);

  // start generating
  int ievent=0;
//   ROOT::Math::XYZVector v1();// why that? Misteries of cint!
    TVector3  momentum3d; // y is vertical, x and z horizonthal 
    TVector3  position;
    Double_t  momentum, ltheta, phi, initX,initY,initZ, muonEnergy;
    Double_t  h_det, h_s1, h_s2, t_s1, t_s2, px_s1, px_s2, pz_s1, pz_s2;
  
  
  for(;ievent<MAXEVENTS; ievent++){

    while(write == false) {
        // generate event variables
        momentum = histoFlux->GetRandom();
        ltheta = thetaDistr->GetRandom();
        phi = gRandom->Uniform(0.,2*pi);

        // generate verticla particle and then rotates
        // momentum first
        momentum3d = TVector3(0.0, -1.0*momentum, 0.0); // y is vertical, x and z horizonthal 
        momentum3d.RotateX(ltheta);
        momentum3d.RotateY(phi);
        
        // position
        initX = gRandom->Uniform(-1.*SURF_HALF_LEN,SURF_HALF_LEN);
        initZ = gRandom->Uniform(-1.*SURF_HALF_LEN,SURF_HALF_LEN);
        initY = SURF_DIST;
        position = TVector3(initX,initY,initZ);
        position.RotateX(ltheta);
        position.RotateY(phi);
        
        // energy
        muonEnergy = sqrt(momentum*momentum+muonMass*muonMass);

        h_det = 835 + 2*142;

        h_s1 =  h_det + 45;
        h_s2 = h_det + 475;

        t_s1 = (h_s1-position.Y()*1000)/momentum3d.Y();
        t_s2 = (h_s2-position.Y()*1000)/momentum3d.Y();

        px_s1 = momentum3d.X()*t_s1 + position.X()*1000;
        px_s2 = momentum3d.X()*t_s2 + position.X()*1000;
        pz_s1 = momentum3d.Z()*t_s1 + position.Z()*1000;
        pz_s2 = momentum3d.Z()*t_s2 + position.Z()*1000;

   if( px_s1 >= -700 && px_s1 <= 700 && t_s1 > 0 && px_s2 >= -700 && px_s2 <= 700 && t_s2 > 0){
      if(pz_s1 >= -700 && pz_s1 <= 700 && pz_s2 >= -700 && pz_s2 <= 700 ){
          write = true;
      }
   }
//        if(muonEnergy <= energ_necess2/3000 & muonEnergy >= energ_necess1/3000){
//            write = true;
//        }
    }
    write = false;
    outfile << ievent << " " << 1 << endl;

    if(gRandom->Uniform(0.,1.)<0.5){//muon
      outfile << 1 << " " << 13  << " " //
	      << 0  << " " << 0  << " "//
	      << 0  << " " << 0  << " "//
	      << momentum3d.X()  << " " << momentum3d.Y()  << " " //
	      << momentum3d.Z()  << " " << muonEnergy  << " " //
	      << muonMass  << " " << position.X()*1000  << " " // position in mm
	      << position.Y()*1000  << " " << position.Z()*1000 << " " // position in mm
	      << 0 << endl; 
    }
    else{ // antimuon
      outfile << 1 << " " << -13  << " " //
	      << 0  << " " << 0  << " "//
	      << 0  << " " << 0  << " "//
	      << momentum3d.X()  << " " << momentum3d.Y()  << " " //
	      << momentum3d.Z()  << " " << muonEnergy  << " " //
	      << muonMass  << " " << position.X()*1000  << " " // position in mm
	      << position.Y()*1000  << " " << position.Z()*1000 << " " // position in mm
	      << 0 << endl; 
    }
  }
  delete histoFlux;
  delete thetaDistr;  
}
