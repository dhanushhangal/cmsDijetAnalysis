// Histograms needed in the dijet analysis

// C++ includes
#include <assert.h>

// Root includes
#include <TFile.h>
#include <TMath.h>

// Own includes
#include "DijetHistograms.h"

/*
 * Default constructor
 */
DijetHistograms::DijetHistograms() :
  fhVertexZ(0),
  fhVertexZWeighted(0),
  fhEvents(0),
  fhTrackCuts(0),
  fhTrackCutsInclusive(0),
  fhCentrality(0),
  fhCentralityWeighted(0),
  fhCentralityDijet(0),
  fhPtHat(0),
  fhPtHatWeighted(0),
  fhPtLeadingJet(0),
  fhLeadingJet(0),
  fhSubleadingJet(0),
  fhDijet(0),
  fhAnyJet(0),
  fhTrack(0),
  fhTrackInclusive(0),
  fhTrackLeadingJet(0),
  fhTrackSubleadingJet(0),
  fhTrackUncorrected(0),
  fhTrackInclusiveUncorrected(0),
  fhTrackLeadingJetUncorrected(0),
  fhTrackSubleadingJetUncorrected(0),
  fhTrackLeadingJetPtWeighted(0),
  fhTrackSubleadingJetPtWeighted(0),
  fhTrackJetInclusive(0),
  fhTrackJetInclusivePtWeighted(0),
  fCard(0)
{
  // Default constructor
}

/*
 * Custom constructor
 */
DijetHistograms::DijetHistograms(ConfigurationCard *newCard) :
  fhVertexZ(0),
  fhVertexZWeighted(0),
  fhEvents(0),
  fhTrackCuts(0),
  fhTrackCutsInclusive(0),
  fhCentrality(0),
  fhCentralityWeighted(0),
  fhCentralityDijet(0),
  fhPtHat(0),
  fhPtHatWeighted(0),
  fhPtLeadingJet(0),
  fhLeadingJet(0),
  fhSubleadingJet(0),
  fhDijet(0),
  fhAnyJet(0),
  fhTrack(0),
  fhTrackInclusive(0),
  fhTrackLeadingJet(0),
  fhTrackSubleadingJet(0),
  fhTrackUncorrected(0),
  fhTrackInclusiveUncorrected(0),
  fhTrackLeadingJetUncorrected(0),
  fhTrackSubleadingJetUncorrected(0),
  fhTrackLeadingJetPtWeighted(0),
  fhTrackSubleadingJetPtWeighted(0),
  fhTrackJetInclusive(0),
  fhTrackJetInclusivePtWeighted(0),
  fCard(newCard)
{
  // Custom constructor
}

/*
 * Copy constructor
 */
DijetHistograms::DijetHistograms(const DijetHistograms& in) :
  fhVertexZ(in.fhVertexZ),
  fhVertexZWeighted(in.fhVertexZWeighted),
  fhEvents(in.fhEvents),
  fhTrackCuts(in.fhTrackCuts),
  fhTrackCutsInclusive(in.fhTrackCutsInclusive),
  fhCentrality(in.fhCentrality),
  fhCentralityWeighted(in.fhCentralityWeighted),
  fhCentralityDijet(in.fhCentralityDijet),
  fhPtHat(in.fhPtHat),
  fhPtHatWeighted(in.fhPtHatWeighted),
  fhPtLeadingJet(in.fhPtLeadingJet),
  fhLeadingJet(in.fhLeadingJet),
  fhSubleadingJet(in.fhSubleadingJet),
  fhDijet(in.fhDijet),
  fhAnyJet(in.fhAnyJet),
  fhTrack(in.fhTrack),
  fhTrackInclusive(in.fhTrackInclusive),
  fhTrackLeadingJet(in.fhTrackLeadingJet),
  fhTrackSubleadingJet(in.fhTrackSubleadingJet),
  fhTrackUncorrected(in.fhTrackUncorrected),
  fhTrackInclusiveUncorrected(in.fhTrackInclusiveUncorrected),
  fhTrackLeadingJetUncorrected(in.fhTrackLeadingJetUncorrected),
  fhTrackSubleadingJetUncorrected(in.fhTrackSubleadingJetUncorrected),
  fhTrackLeadingJetPtWeighted(in.fhTrackLeadingJetPtWeighted),
  fhTrackSubleadingJetPtWeighted(in.fhTrackSubleadingJetPtWeighted),
  fhTrackJetInclusive(in.fhTrackJetInclusive),
  fhTrackJetInclusivePtWeighted(in.fhTrackJetInclusivePtWeighted),
  fCard(in.fCard)
{
  // Copy constructor
}

/*
 * Assingment operator
 */
DijetHistograms& DijetHistograms::operator=(const DijetHistograms& in){
  // Assingment operator
  
  if (&in==this) return *this;
  
  fhVertexZ = in.fhVertexZ;
  fhVertexZWeighted = in.fhVertexZWeighted;
  fhEvents = in.fhEvents;
  fhTrackCuts = in.fhTrackCuts;
  fhTrackCutsInclusive = in.fhTrackCutsInclusive;
  fhCentrality = in.fhCentrality;
  fhCentralityWeighted = in.fhCentralityWeighted;
  fhCentralityDijet = in.fhCentralityDijet;
  fhPtHat = in.fhPtHat;
  fhPtHatWeighted = in.fhPtHatWeighted;
  fhPtLeadingJet = in.fhPtLeadingJet;
  fhLeadingJet = in.fhLeadingJet;
  fhSubleadingJet = in.fhSubleadingJet;
  fhDijet = in.fhDijet;
  fhAnyJet = in.fhAnyJet;
  fhTrack = in.fhTrack;
  fhTrackInclusive = in.fhTrackInclusive;
  fhTrackLeadingJet = in.fhTrackLeadingJet;
  fhTrackSubleadingJet = in.fhTrackSubleadingJet;
  fhTrackUncorrected = in.fhTrackUncorrected;
  fhTrackInclusiveUncorrected = in.fhTrackInclusiveUncorrected;
  fhTrackLeadingJetUncorrected = in.fhTrackLeadingJetUncorrected;
  fhTrackSubleadingJetUncorrected = in.fhTrackSubleadingJetUncorrected;
  fhTrackLeadingJetPtWeighted = in.fhTrackLeadingJetPtWeighted;
  fhTrackSubleadingJetPtWeighted = in.fhTrackSubleadingJetPtWeighted;
  fhTrackJetInclusive = in.fhTrackJetInclusive;
  fhTrackJetInclusivePtWeighted = in.fhTrackJetInclusivePtWeighted;
  fCard = in.fCard;
  
  return *this;
}

/*
 * Destructor
 */
DijetHistograms::~DijetHistograms(){
  // destructor
  delete fhVertexZ;
  delete fhVertexZWeighted;
  delete fhEvents;
  delete fhTrackCuts;
  delete fhTrackCutsInclusive;
  delete fhCentrality;
  delete fhCentralityWeighted;
  delete fhCentralityDijet;
  delete fhPtHat;
  delete fhPtHatWeighted;
  delete fhPtLeadingJet;
  delete fhLeadingJet;
  delete fhSubleadingJet;
  delete fhDijet;
  delete fhAnyJet;
  delete fhTrack;
  delete fhTrackInclusive;
  delete fhTrackLeadingJet;
  delete fhTrackSubleadingJet;
  delete fhTrackUncorrected;
  delete fhTrackInclusiveUncorrected;
  delete fhTrackLeadingJetUncorrected;
  delete fhTrackSubleadingJetUncorrected;
  delete fhTrackLeadingJetPtWeighted;
  delete fhTrackSubleadingJetPtWeighted;
  delete fhTrackJetInclusive;
  delete fhTrackJetInclusivePtWeighted;
}

/*
 * Set the configuration card used for the histogram class
 */
void DijetHistograms::SetCard(ConfigurationCard *newCard){  
  fCard = newCard;
}

/*
 * Create the necessary histograms
 */
void DijetHistograms::CreateHistograms(){
  
  // ======== Common binning information for histograms =========
  
  // Centrality
  const Double_t minCentrality = -0.75;   // Minimum centrality bin, is negative since hiBin is -1 for pp
  const Double_t maxCentrality = 100.25;  // Maximum centrality bin
  const Int_t nCentralityBins = 202;      // Number of centrality bins
  
  // Jet pT
  const Double_t minPtJet = 0;     // Minimum jet pT
  const Double_t maxPtJet = 300;   // Maximum jet pT
  const Int_t nPtBinsJet = 150;    // Number of jet pT bins
  
  //Track pT
  const Double_t minPtTrack = 0;   // Minimum track pT for track histograms
  const Double_t maxPtTrack = 20;  // Maximum track pT for track histograms (Hallie's analysis = 20)
  const Int_t nPtBinsTrack = 500;  // Number of track pT bins for track histograms (Hallie's analysis = 500)
  
  // Phi
  const Double_t minPhi = -TMath::Pi();  // Minimum phi
  const Double_t maxPhi = TMath::Pi();   // Maximum phi
  const Int_t nPhiBins = 64;             // Number of phi bins
  
  // Eta
  const Double_t minEta = -2.5;    // Minimum eta (current eta cut for tracks = 2.4)
  const Double_t maxEta = 2.5;     // Maximum eta (current eta cut for tracks = 2.4)
  const Int_t nEtaBins = 50;       // Number of eta bins
  
  // DeltaPhi in [0,pi]
  const Double_t minDeltaPhi = 0;             // Minimum deltaPhi
  const Double_t maxDeltaPhi = TMath::Pi();   // Maximum deltaPhi
  const Int_t nDeltaPhiBins = 32;             // Number of deltaPhi bins
  
  // DeltaPhi in [-pi/2,3pi/2]
  const Double_t minDeltaPhiJetTrack = -TMath::Pi()/2.0;    // Minimum deltaPhi for jet-track correlations
  const Double_t maxDeltaPhiJetTrack = 3.0*TMath::Pi()/2.0; // Maximum deltaPhi for jet-track correlations
  const Int_t nDeltaPhiBinsJetTrack = 200;                  // Number of deltaPhi bins for jet-track correlations (match the common number in UIC group)
  
  // DeltaEta
  const Double_t minDeltaEtaJetTrack = -5.0;   // Minimum deltaEta for jet-track correlations
  const Double_t maxDeltaEtaJetTrack = 5.0;    // Maximum deltaEta for jet-track correlations
  const Int_t nDeltaEtaBinsJetTrack = 500;     // Number of deltaEta bins for jet-track correlations (match the common number in UIC group)
  
  // Dijet asymmetry
  const Double_t minAsymmetry = 0;     // Minimum asymmetry
  const Double_t maxAsymmetry = 0.75;  // Maximum asymmetry
  const Int_t nAsymmetryBins = 25;     // Number of asymmetry bins
  
  // Vertex z-position
  const Double_t minVz = -20;   // Minimum vz
  const Double_t maxVz = 20;    // Maximum vz
  const Int_t nVzBins = 80;     // Number of vz bins
  
  // pT hat
  const Double_t minPtHat = 0;     // Minimum pT hat
  const Double_t maxPtHat = 460;   // Maximum pT hat
  const Int_t nFinePtHatBins = 230; // Number of fine pT hat bins
  
  // Correlation types
  const Double_t minCorrelationType = -0.5;                   // Correlation type indexing starts from zero
  const Double_t maxCorrelationType = knCorrelationTypes-0.5; // Maximum correlation type index
  const Int_t nCorrelationTypeBins = knCorrelationTypes;      // Make a bin with width of 1 for each correlation type
  
  // Centrality bins for THnSparses (We run into memory issues, if have all the bins)
  const Int_t nWideCentralityBins = fCard->GetNBin("CentralityBinEdges");
  Double_t wideCentralityBins[nWideCentralityBins+1];
  for(Int_t iCentrality = 0; iCentrality < nWideCentralityBins+1; iCentrality++){
    wideCentralityBins[iCentrality] = fCard->Get("CentralityBinEdges",iCentrality);
  }
  
  // Track pT bins for THnSparses (We run into memory issues, if have all the bins)
  const Int_t nWideTrackPtBins = fCard->GetNBin("TrackPtBinEdges");
  Double_t wideTrackPtBins[nWideTrackPtBins+1];
  for(Int_t iTrackPt = 0; iTrackPt < nWideTrackPtBins+1; iTrackPt++){
    wideTrackPtBins[iTrackPt] = fCard->Get("TrackPtBinEdges",iTrackPt);
  }
  
  // Dijet asymmetry bins for THnSparses (We run into memory issues, if have all the bins)
  const Int_t nWideAsymmetryBins = fCard->GetNBin("AsymmetryBinEdges");
  Double_t wideAsymmetryBins[nWideAsymmetryBins+1];
  for(Int_t iAsymmetryBin = 0; iAsymmetryBin < nWideAsymmetryBins+1; iAsymmetryBin++){
    wideAsymmetryBins[iAsymmetryBin] = fCard->Get("AsymmetryBinEdges",iAsymmetryBin);
  }
  
  // Bins for the pT hat histogram
  const Int_t nPtHatBins = fCard->GetNBin("PtHatBinEdges");
  Double_t ptHatBins[nPtHatBins+1];
  for(Int_t iPtHat = 0; iPtHat < nPtHatBins+1; iPtHat++){
    ptHatBins[iPtHat] = fCard->Get("PtHatBinEdges",iPtHat);
  }
  
  // Arrays for creating THnSparses
  Int_t nBins4D[4];
  Int_t nBins5D[5];
  Int_t nBins6D[6];
  Double_t lowBinBorder4D[4];
  Double_t lowBinBorder5D[5];
  Double_t lowBinBorder6D[6];
  Double_t highBinBorder4D[4];
  Double_t highBinBorder5D[5];
  Double_t highBinBorder6D[6];
  
  // ======== Plain TH1 histograms ========
  
  fhVertexZ = new TH1F("vertexZ","vertexZ",nVzBins,minVz,maxVz); fhVertexZ->Sumw2();
  fhVertexZWeighted = new TH1F("vertexZweighted","vertexZweighted",nVzBins,minVz,maxVz); fhVertexZWeighted->Sumw2();
  fhEvents = new TH1F("nEvents","nEvents",knEventTypes,-0.5,knEventTypes-0.5); fhEvents->Sumw2();
  fhTrackCuts = new TH1F("trackCuts","trackCuts",knTrackCuts,-0.5,knTrackCuts-0.5); fhTrackCuts->Sumw2();
  fhTrackCutsInclusive = new TH1F("trackCutsInclusive","trackCutsInclusive",knTrackCuts,-0.5,knTrackCuts-0.5); fhTrackCutsInclusive->Sumw2();
  fhCentrality = new TH1F("centrality","centrality",nCentralityBins,minCentrality,maxCentrality); fhCentrality->Sumw2();
  fhCentralityWeighted = new TH1F("centralityWeighted","centralityWeighted",nCentralityBins,minCentrality,maxCentrality); fhCentralityWeighted->Sumw2();
  fhCentralityDijet = new TH1F("centralityDijet","centralityDijet",nCentralityBins,minCentrality,maxCentrality); fhCentralityDijet->Sumw2();
  fhPtHat = new TH1F("pthat","pthat",nPtHatBins,ptHatBins); fhPtHat->Sumw2();
  fhPtHatWeighted = new TH1F("pthatWeighted","pthatWeighted",nFinePtHatBins,minPtHat,maxPtHat); fhPtHatWeighted->Sumw2();
  fhPtLeadingJet = new TH1F("ptLeadingJet","ptLeadingJet",nPtBinsJet,minPtJet,maxPtJet); fhPtLeadingJet->Sumw2();
  
  // For the event histogram, label each bin corresponding to an event cut
  for(Int_t i = 0; i < knEventTypes; i++){
    fhEvents->GetXaxis()->SetBinLabel(i+1,kEventTypeStrings[i]);
  }
  
  // If we are using PF jets, change the axis label for that
  if(fCard->Get("JetType") == 1) fhEvents->GetXaxis()->SetBinLabel(kCaloJet+1,"PFJet");
  
  // For the track cut histogram, label each bin corresponding to a track cut
  for(Int_t i = 0; i < knTrackCuts; i++){
    fhTrackCuts->GetXaxis()->SetBinLabel(i+1,kTrackCutStrings[i]);
    fhTrackCutsInclusive->GetXaxis()->SetBinLabel(i+1,kTrackCutStrings[i]);
  }
  
  // ======== THnSparses for leading and subleading jets ========
  
  // Axis 0 for the jet histogram: leading/subleading jet pT
  nBins5D[0] = nPtBinsJet;         // nBins for leading/subleading jet pT
  lowBinBorder5D[0] = minPtJet;    // low bin border for leading/subleading jet pT
  highBinBorder5D[0] = maxPtJet;   // high bin border for leading/subleading jet pT
  
  // Axis 1 for the jet histogram: leading/subleading jet phi
  nBins5D[1] = nPhiBins;        // nBins for leading/subleading jet phi
  lowBinBorder5D[1] = minPhi;   // low bin border for leading/subleading jet phi
  highBinBorder5D[1] = maxPhi;  // high bin border for leading/subleading jet phi
  
  // Axis 2 for the jet histogram: leading/subleading jet eta
  nBins5D[2] = nEtaBins;        // nBins for leading/subleading jet eta
  lowBinBorder5D[2] = minEta;   // low bin border for leading/subleading jet eta
  highBinBorder5D[2] = maxEta;  // high bin border for leading/subleading jet eta
  
  // Axis 3 for the jet histogram: asymmetry
  nBins5D[3] = nWideAsymmetryBins;     // nBins for wide asymmetry bins
  lowBinBorder5D[3] = minAsymmetry;    // low bin border for asymmetry
  highBinBorder5D[3] = maxAsymmetry;   // high bin border for asymmetry
  
  // Axis 4 for the jet histogram: centrality
  nBins5D[4] = nWideCentralityBins;   // nBins for wide centrality bins
  lowBinBorder5D[4] = minCentrality;  // low bin border for centrality
  highBinBorder5D[4] = maxCentrality; // high bin border for centrality
  
  // Create the histograms for leading and subleading jets using the above binning information
  fhLeadingJet = new THnSparseF("leadingJet","leadingJet",5,nBins5D,lowBinBorder5D,highBinBorder5D); fhLeadingJet->Sumw2();
  fhSubleadingJet = new THnSparseF("subleadingJet","subleadingJet",5,nBins5D,lowBinBorder5D,highBinBorder5D); fhSubleadingJet->Sumw2();
  
  // Set custom dijet asymmetry bins for histograms
  fhLeadingJet->SetBinEdges(3,wideAsymmetryBins);
  fhSubleadingJet->SetBinEdges(3,wideAsymmetryBins);
  
  // Set custom centrality bins for histograms
  fhLeadingJet->SetBinEdges(4,wideCentralityBins);
  fhSubleadingJet->SetBinEdges(4,wideCentralityBins);

  // ========= THnSparse for dijets =========
  
  // Axis 0 for the dijet histogram: leading jet pT
  nBins5D[0] = nPtBinsJet;         // nBins for leading jet pT
  lowBinBorder5D[0] = minPtJet;    // low bin border for leading jet pT
  highBinBorder5D[0] = maxPtJet;   // high bin border for leading jet pT
  
  // Axis 1 for the dijet histogram: subleading jet pT
  nBins5D[1] = nPtBinsJet;         // nBins for subleading jet pT
  lowBinBorder5D[1] = minPtJet;    // low bin border for subleading jet pT
  highBinBorder5D[1] = maxPtJet;   // high bin border for subleading jet pT
  
  // Axis 2 for the dijet histogram: deltaPhi
  nBins5D[2] = nDeltaPhiBins;       // nBins for deltaPhi
  lowBinBorder5D[2] = minDeltaPhi;  // low bin border for deltaPhi
  highBinBorder5D[2] = maxDeltaPhi; // high bin border for deltaPhi
  
  // Axis 3 for the dijet histogram: asymmetry
  nBins5D[3] = nAsymmetryBins;         // nBins for asymmetry
  lowBinBorder5D[3] = minAsymmetry;    // low bin border for asymmetry
  highBinBorder5D[3] = maxAsymmetry;   // high bin border for asymmetry
  
  // Axis 4 for the dijet histogram: centrality
  nBins5D[4] = nWideCentralityBins;   // nBins for wide centrality bins
  lowBinBorder5D[4] = minCentrality;  // low bin border for centrality
  highBinBorder5D[4] = maxCentrality; // high bin border for centrality
  
  // Create the dijet histogram using the above binning information
  fhDijet = new THnSparseF("dijet","dijet",5,nBins5D,lowBinBorder5D,highBinBorder5D); fhDijet->Sumw2();
  
  // Set custom centrality bins for histograms
  fhDijet->SetBinEdges(4,wideCentralityBins);
  
  // ======== THnSparse for all jets ========
  
  // Axis 0 for the any jet histogram: jet pT
  nBins4D[0] = nPtBinsJet;         // nBins for any jet pT
  lowBinBorder4D[0] = minPtJet;    // low bin border for any jet pT
  highBinBorder4D[0] = maxPtJet;   // high bin border for any jet pT
  
  // Axis 1 for the any jet histogram: jet phi
  nBins4D[1] = nPhiBins;        // nBins for any jet phi
  lowBinBorder4D[1] = minPhi;   // low bin border for any jet phi
  highBinBorder4D[1] = maxPhi;  // high bin border for any jet phi
  
  // Axis 2 for the any jet histogram: jet eta
  nBins4D[2] = nEtaBins;        // nBins for any jet eta
  lowBinBorder4D[2] = minEta;   // low bin border for any jet eta
  highBinBorder4D[2] = maxEta;  // high bin border for any jet eta
  
  // Axis 3 for the any jet histogram: centrality
  nBins4D[3] = nWideCentralityBins;   // nBins for wide centrality bins
  lowBinBorder4D[3] = minCentrality;  // low bin border for centrality
  highBinBorder4D[3] = maxCentrality; // high bin border for centrality
  
  // Create the histogram for all jets using the above binning information
  fhAnyJet = new THnSparseF("anyJet","anyJet",4,nBins4D,lowBinBorder4D,highBinBorder4D); fhAnyJet->Sumw2();
  
  // Set custom centrality bins for histograms
  fhAnyJet->SetBinEdges(3,wideCentralityBins);
  
  // ======== THnSparses for tracks and uncorrected tracks ========
  
  // Axis 0 for the track histogram: track pT
  nBins5D[0] = nPtBinsTrack;         // nBins for track pT
  lowBinBorder5D[0] = minPtTrack;    // low bin border for track pT
  highBinBorder5D[0] = maxPtTrack;   // high bin border for track pT
  
  // Axis 1 for the track histogram: track phi
  nBins5D[1] = nPhiBins;         // nBins for track phi
  lowBinBorder5D[1] = minPhi;    // low bin border for track phi
  highBinBorder5D[1] = maxPhi;   // high bin border for track phi
  
  // Axis 2 for the track histogram: track eta
  nBins5D[2] = nEtaBins;         // nBins for track eta
  lowBinBorder5D[2] = minEta;    // low bin border for track eta
  highBinBorder5D[2] = maxEta;   // high bin border for track eta
  
  // Axis 3 for the track histogram: centrality
  nBins5D[3] = nWideCentralityBins;   // nBins for wide centrality bins
  lowBinBorder5D[3] = minCentrality;  // low bin border for centrality
  highBinBorder5D[3] = maxCentrality; // high bin border for centrality
  
  // Axis 4 for the track histogram: correlation type
  nBins5D[4] = nCorrelationTypeBins;        // nBins for correlation types
  lowBinBorder5D[4] = minCorrelationType;   // low bin border for correlation types
  highBinBorder5D[4] = maxCorrelationType;  // high bin border for correlation types
  
  // Create the histograms for tracks and uncorrected tracks using the above binning information
  fhTrack = new THnSparseF("track","track",5,nBins5D,lowBinBorder5D,highBinBorder5D); fhTrack->Sumw2();
  fhTrackInclusive = new THnSparseF("trackInclusive","trackInclusive",4,nBins5D,lowBinBorder5D,highBinBorder5D); fhTrackInclusive->Sumw2();
  fhTrackUncorrected = new THnSparseF("trackUncorrected","trackUncorrected",5,nBins5D,lowBinBorder5D,highBinBorder5D); fhTrackUncorrected->Sumw2();
  fhTrackInclusiveUncorrected = new THnSparseF("trackInclusiveUncorrected","trackInclusiveUncorrected",4,nBins5D,lowBinBorder5D,highBinBorder5D); fhTrackInclusiveUncorrected->Sumw2();

  // Set custom centrality bins for histograms
  fhTrack->SetBinEdges(3,wideCentralityBins);
  fhTrackInclusive->SetBinEdges(3,wideCentralityBins);
  fhTrackUncorrected->SetBinEdges(3,wideCentralityBins);
  fhTrackInclusiveUncorrected->SetBinEdges(3,wideCentralityBins);
  
  // ======== THnSparses for correlation between tracks and leading or subleading jets ========
  
  // Axis 0 for the track-jet correlation histogram: track pT
  nBins6D[0] = nWideTrackPtBins;     // nBins for wide track pT bins
  lowBinBorder6D[0] = minPtTrack;    // low bin border for track pT
  highBinBorder6D[0] = maxPtTrack;   // high bin border for track pT
  
  // Axis 1 for the track-jet correlation histogram: deltaPhi between track and jet
  nBins6D[1] = nDeltaPhiBinsJetTrack;       // nBins for deltaPhi between track and jet
  lowBinBorder6D[1] = minDeltaPhiJetTrack;  // low bin border for deltaPhi between track and jet
  highBinBorder6D[1] = maxDeltaPhiJetTrack; // high bin border for deltaPhi between track and jet
  
  // Axis 2 for the track-jet correlation histogram: deltaEta between track and jet
  nBins6D[2] = nDeltaEtaBinsJetTrack;         // nBins for deltaEta between track and jet
  lowBinBorder6D[2] = minDeltaEtaJetTrack;    // low bin border deltaEta between track and jet
  highBinBorder6D[2] = maxDeltaEtaJetTrack;   // high bin border deltaEta between track and jet
  
  // Axis 3 for the track-jet correlation histogram: dijet asymmetry
  nBins6D[3] = nWideAsymmetryBins;     // nBins for wide dijet asymmetry bins
  lowBinBorder6D[3] = minAsymmetry;    // low bin border for dijet asymmetry
  highBinBorder6D[3] = maxAsymmetry;   // high bin border for dijet asymmetry
  
  // Axis 4 for the track-jet correlation histogram: centrality
  nBins6D[4] = nWideCentralityBins;     // nBins for centrality
  lowBinBorder6D[4] = minCentrality;    // low bin border for centrality
  highBinBorder6D[4] = maxCentrality;   // high bin border for centrality
  
  // Axis 5 for the track-jet correlation histogram: correlation type
  nBins6D[5] = nCorrelationTypeBins;        // nBins for correlation types
  lowBinBorder6D[5] = minCorrelationType;   // low bin border for correlation types
  highBinBorder6D[5] = maxCorrelationType;  // high bin border for correlation types
  
  // Create histograms for track-jet correlations
  fhTrackLeadingJet = new THnSparseF("trackLeadingJet","trackLeadingJet",6,nBins6D,lowBinBorder6D,highBinBorder6D); fhTrackLeadingJet->Sumw2();
  fhTrackSubleadingJet = new THnSparseF("trackSubleadingJet","trackSubleadingJet",6,nBins6D,lowBinBorder6D,highBinBorder6D); fhTrackSubleadingJet->Sumw2();
  
  // Set custom centrality, asymmetry and track pT bins for histograms
  fhTrackLeadingJet->SetBinEdges(0,wideTrackPtBins);
  fhTrackSubleadingJet->SetBinEdges(0,wideTrackPtBins);
  fhTrackLeadingJet->SetBinEdges(3,wideAsymmetryBins);
  fhTrackSubleadingJet->SetBinEdges(3,wideAsymmetryBins);
  fhTrackLeadingJet->SetBinEdges(4,wideCentralityBins);
  fhTrackSubleadingJet->SetBinEdges(4,wideCentralityBins);

  // Create histograms for uncorrected track-jet correlations
  fhTrackLeadingJetUncorrected = new THnSparseF("trackLeadingJetUncorrected","trackLeadingJetUncorrected",6,nBins6D,lowBinBorder6D,highBinBorder6D); fhTrackLeadingJetUncorrected->Sumw2();
  fhTrackSubleadingJetUncorrected = new THnSparseF("trackSubleadingJetUncorrected","trackSubleadingJetUncorrected",6,nBins6D,lowBinBorder6D,highBinBorder6D); fhTrackSubleadingJetUncorrected->Sumw2();
  
  // Set custom centrality, asymmetry and track pT bins for histograms
  fhTrackLeadingJetUncorrected->SetBinEdges(0,wideTrackPtBins);
  fhTrackSubleadingJetUncorrected->SetBinEdges(0,wideTrackPtBins);
  fhTrackLeadingJetUncorrected->SetBinEdges(3,wideAsymmetryBins);
  fhTrackSubleadingJetUncorrected->SetBinEdges(3,wideAsymmetryBins);
  fhTrackLeadingJetUncorrected->SetBinEdges(4,wideCentralityBins);
  fhTrackSubleadingJetUncorrected->SetBinEdges(4,wideCentralityBins);
  
  // Create histograms for pT weighted track-jet correlations
  fhTrackLeadingJetPtWeighted = new THnSparseF("trackLeadingJetPtWeighted","trackLeadingJetPtWeighted",6,nBins6D,lowBinBorder6D,highBinBorder6D); fhTrackLeadingJetPtWeighted->Sumw2();
  fhTrackSubleadingJetPtWeighted = new THnSparseF("trackSubleadingJetPtWeighted","trackSubleadingJetPtWeighted",6,nBins6D,lowBinBorder6D,highBinBorder6D); fhTrackSubleadingJetPtWeighted->Sumw2();
  
  // Set custom centrality, asymmetry and track pT bins for histograms
  fhTrackLeadingJetPtWeighted->SetBinEdges(0,wideTrackPtBins);
  fhTrackSubleadingJetPtWeighted->SetBinEdges(0,wideTrackPtBins);
  fhTrackLeadingJetPtWeighted->SetBinEdges(3,wideAsymmetryBins);
  fhTrackSubleadingJetPtWeighted->SetBinEdges(3,wideAsymmetryBins);
  fhTrackLeadingJetPtWeighted->SetBinEdges(4,wideCentralityBins);
  fhTrackSubleadingJetPtWeighted->SetBinEdges(4,wideCentralityBins);
  
  // ======== THnSparses for correlation between tracks and inclusive jets ========
  
  // Axis 0 for the track-inclusive jet correlation histogram: track pT
  nBins5D[0] = nWideTrackPtBins;     // nBins for wide track pT bins
  lowBinBorder5D[0] = minPtTrack;    // low bin border for track pT
  highBinBorder5D[0] = maxPtTrack;   // high bin border for track pT
  
  // Axis 1 for the track-inclusive jet correlation histogram: deltaPhi between track and jet
  nBins5D[1] = nDeltaPhiBinsJetTrack;       // nBins for deltaPhi between track and jet
  lowBinBorder5D[1] = minDeltaPhiJetTrack;  // low bin border for deltaPhi between track and jet
  highBinBorder5D[1] = maxDeltaPhiJetTrack; // high bin border for deltaPhi between track and jet
  
  // Axis 2 for the track-inclusive jet correlation histogram: deltaEta between track and jet
  nBins5D[2] = nDeltaEtaBinsJetTrack;         // nBins for deltaEta between track and jet
  lowBinBorder5D[2] = minDeltaEtaJetTrack;    // low bin border deltaEta between track and jet
  highBinBorder5D[2] = maxDeltaEtaJetTrack;   // high bin border deltaEta between track and jet
  
  // Axis 3 for the track-inclusive jet correlation histogram: centrality
  nBins5D[3] = nWideCentralityBins;     // nBins for centrality
  lowBinBorder5D[3] = minCentrality;    // low bin border for centrality
  highBinBorder5D[3] = maxCentrality;   // high bin border for centrality
  
  // Axis 4 for the track-inclusive jet correlation histogram: correlation type
  nBins5D[4] = nCorrelationTypeBins;        // nBins for correlation types
  lowBinBorder5D[4] = minCorrelationType;   // low bin border for correlation types
  highBinBorder5D[4] = maxCorrelationType;  // high bin border for correlation types
  
  // Create histograms for track-inclusive jet correlations
  fhTrackJetInclusive = new THnSparseF("trackJetInclusive","trackJetInclusive",5,nBins5D,lowBinBorder5D,highBinBorder5D); fhTrackJetInclusive->Sumw2();
  fhTrackJetInclusivePtWeighted = new THnSparseF("trackJetInclusivePtWeighted","trackJetInclusivePtWeighted",5,nBins5D,lowBinBorder5D,highBinBorder5D); fhTrackJetInclusivePtWeighted->Sumw2();
  
  // Set custom centrality and track pT bins for histograms
  fhTrackJetInclusive->SetBinEdges(0,wideTrackPtBins);
  fhTrackJetInclusivePtWeighted->SetBinEdges(0,wideTrackPtBins);
  fhTrackJetInclusive->SetBinEdges(3,wideCentralityBins);
  fhTrackJetInclusivePtWeighted->SetBinEdges(3,wideCentralityBins);
  
}

/*
 * Write the histograms to file
 */
void DijetHistograms::Write() const{
  
  // Write the histograms to file
  fhVertexZ->Write();
  fhVertexZWeighted->Write();
  fhEvents->Write();
  fhTrackCuts->Write();
  fhTrackCutsInclusive->Write();
  fhCentrality->Write();
  fhCentralityWeighted->Write();
  fhCentralityDijet->Write();
  fhPtHat->Write();
  fhPtHatWeighted->Write();
  fhPtLeadingJet->Write();
  fhLeadingJet->Write();
  fhSubleadingJet->Write();
  fhDijet->Write();
  fhAnyJet->Write();
  fhTrack->Write();
  fhTrackInclusive->Write();
  fhTrackLeadingJet->Write();
  fhTrackSubleadingJet->Write();
  fhTrackUncorrected->Write();
  fhTrackInclusiveUncorrected->Write();
  fhTrackLeadingJetUncorrected->Write();
  fhTrackSubleadingJetUncorrected->Write();
  fhTrackLeadingJetPtWeighted->Write();
  fhTrackSubleadingJetPtWeighted->Write();
  fhTrackJetInclusive->Write();
  fhTrackJetInclusivePtWeighted->Write();
  
}

/*
 * Write the histograms to a given file
 */
void DijetHistograms::Write(TString outputFileName) const{
  
  // Define the output file
  TFile *outputFile = new TFile(outputFileName, "RECREATE");
  
  // Write the histograms to file
  Write();
  
  // Close the file after everything is written
  outputFile->Close();
  
  // Delete the outputFile object
  delete outputFile;
}


