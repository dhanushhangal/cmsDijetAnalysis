#include "trackingEfficiency2017pp.h"

inline bool TrkEff2017pp::checkBounds(float pt, float eta){
  if( TMath::Abs(eta) > 2.4 ){
    if( ! isQuiet) std::cout << "TrkEff2017pp: track outside |eta|<2.4, please apply this cut!  I am returning a correction factor of 0 for this track for now." << std::endl;
    return false;
  }
  
  if( pt< 0 || pt > 500 ){
    if( ! isQuiet) std::cout << "TrkEff2017pp: pT is outside the range [0,500].  I am returning a correction factor of 0 for this track for now." << std::endl;
    return false;
  }

  return true;
}

float TrkEff2017pp::getCorrection(float pt, float eta, int hiBin){
  return getCorrection(pt, eta);
}

float TrkEff2017pp::getCorrection(float pt, float eta){
  if( !checkBounds(pt, eta) ) return 0;

  float efficiency = getEfficiency(pt, eta, true);
  float fake = getFake(pt, eta, true);
  float secondary = getSecondary(pt, eta, true);

  //protect against dividing by 0
  if(efficiency > 0.001){
    return (1-fake)*(1-secondary)/efficiency;
  } else {
    if( ! isQuiet ) std::cout << "TrkEff2017pp: Warning! Tracking efficiency is very low for this track (close to dividing by 0).  Returning correction factor of 0 for this track for now." << std::endl;
    return 0;
  }
}

float TrkEff2017pp::getEfficiency( float pt, float eta, bool passesCheck){
  if( !passesCheck){
    if(  !checkBounds(pt, eta) ) return 0;
  }

  return eff->GetBinContent( eff->FindBin(eta, pt) );
}

float TrkEff2017pp::getFake( float pt, float eta,  bool passesCheck){
  if( !passesCheck){
    if(  !checkBounds(pt, eta) ) return 0;
  }

  return fake->GetBinContent( fake->FindBin(eta, pt) );
}

float TrkEff2017pp::getSecondary( float pt, float eta, bool passesCheck){
  if( !passesCheck){
    if(  !checkBounds(pt, eta) ) return 0;
  }

  return sec->GetBinContent( sec->FindBin(eta, pt) );
}


TrkEff2017pp::TrkEff2017pp(bool isQuiet_, std::string filePath){
  isQuiet = isQuiet_;
  
  std::cout << "Searching tracking corrections from folder: " << filePath << std::endl;
  
    if(!isQuiet) std::cout << "TrkEff2017pp class opening in general tracks mode!  WARNING, THESE ARE STILL PRELIMINARY!" << std::endl;
    
    trkEff = TFile::Open( (filePath + "2017pp_TrkCorr_Aug16_Prelim.root").c_str(),"open");
    
    if( !(trkEff->IsOpen() ) ){
      std::cout << "WARNING, COULD NOT FIND TRACK EFFICIENCY FILE FOR GENERAL TRACKS!" << std::endl;
    } else {
      eff = (TH2F*) trkEff->Get("rEff");
      fake = (TH2F*) trkEff->Get("rFak");
      sec = (TH2F*) trkEff->Get("rSec");
    }
}

TrkEff2017pp::~TrkEff2017pp(){
  trkEff->Close();
}
