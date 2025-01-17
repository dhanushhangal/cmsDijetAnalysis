/*
 * Implementation of DijetComparingDrawer
 */

// Root includes
#include <THnSparse.h>
#include <TPad.h>
#include <TF1.h>

// Own includes
#include "DijetComparingDrawer.h"

/*
 * Constructor
 */
DijetComparingDrawer::DijetComparingDrawer(DijetHistogramManager *fBaseHistograms) :
  fBaseHistograms(fBaseHistograms),
  fnAddedHistograms(0),
  fMainHistogram(0),
  fDrawDijets(false),
  fDrawJetTrackDeltaPhi(false),
  fDrawJetTrackDeltaEta(false),
  fDrawJetTrackDeltaEtaDeltaPhi(false),
  fDrawEventMixingCheck(false),
  fSaveFigures(false),
  fFigureFormat("pdf"),
  fFigureComment(""),
  fApplyScaling(false),
  fLogPt(true),
  fLogCorrelation(true),
  fLogJetShape(true),
  fUseDifferenceInsteadOfRatio(false),
  fRatioZoomMin(0.6),
  fRatioZoomMax(1.4),
  fRatioLabel(""),
  fEventMixingZoom(false),
  fEventMixingDistribution(3),
  fColorPalette(kRainBow),
  fStyle2D("colz"),
  fStyle3D("surf1"),
  fFirstDrawnCentralityBin(0),
  fLastDrawnCentralityBin(0),
  fFirstDrawnTrackPtBin(0),
  fLastDrawnTrackPtBin(0),
  fAsymmetryBin(DijetHistogramManager::kMaxAsymmetryBins)
{
  
  // Create a new drawer
  fDrawer = new JDrawer();
  
  for(int iRatios = 0; iRatios < knMaxRatios; iRatios++){
    fAddedHistograms[iRatios] = NULL;
    fComparisonHistogram[iRatios] = NULL;
    fRatioHistogram[iRatios] = NULL;
  }
  
  // Do not draw anything by default
  for(int iJetTrack = 0; iJetTrack < DijetHistogramManager::knJetTrackCorrelations; iJetTrack++){
    fDrawJetTrackCorrelations[iJetTrack] = false;
  }
  for(int iTrackType = 0; iTrackType < DijetHistogramManager::knTrackCategories; iTrackType++){
    fDrawTracks[iTrackType] = false;
  }
  for(int iJetType = 0; iJetType < DijetHistogramManager::knSingleJetCategories; iJetType++){
    fDrawSingleJets[iJetType] = false;
  }
  for(int iCorrelationType = 0; iCorrelationType < DijetHistogramManager::knCorrelationTypes; iCorrelationType++){
    fDrawCorrelationType[iCorrelationType] = false;
  }
  for(int iJetShape = 0; iJetShape < DijetHistogramManager::knJetShapeTypes; iJetShape++){
    fDrawJetShape[iJetShape] = false;
  }
  for(int iComment = 0; iComment < knMaxRatios+1; iComment++){
    fLegendComment[iComment] = "";
    fScalingFactors[iComment] = 1;
  }
  
}

/*
 * Destructor
 */
DijetComparingDrawer::~DijetComparingDrawer(){
  delete fDrawer;
}

/*
 * Add histograms to draw together with base histograms
 *
 *  Arguments:
 *   DijetHistogramManager *additionalHistogram = Histogram manager containing the set of histograms for this dataset
 */
void DijetComparingDrawer::AddHistogramToDraw(DijetHistogramManager *additionalHistogram){
  if(fnAddedHistograms == knMaxRatios){
    cout << "Already at maximum amount of histograms (" << knMaxRatios << "), cannot add more!" << endl;
    return;
  }
  
  fAddedHistograms[fnAddedHistograms++] = additionalHistogram;
}

/*
 * Add comment to legend
 *
 *  Arguments:
 *   TString comment = Comment given to the legend
 */
void DijetComparingDrawer::AddLegendComment(TString comment){
  if(fnAddedHistograms == knMaxRatios){
    cout << "Already at maximum amount of histograms (" << knMaxRatios << "), cannot add more!" << endl;
    return;
  }
  
  fLegendComment[fnAddedHistograms] = comment;
}

/*
 * Draw all the selected histograms using JDrawer
 */
void DijetComparingDrawer::DrawHistograms(){
  
  // Draw dijet histograms
  DrawDijetHistograms();
  
  // Draw the single jet histograms
  DrawSingleJetHistograms();
  
  // Draw the track histograms
  DrawTrackHistograms();
  
  // Draw the jet-track correlation histograms
  DrawJetTrackCorrelationHistograms();
  
  // Draw the jet shape histograms
  DrawJetShapeHistograms();
  DrawJetShapeMCComparison();
  
  // Draw the event mixing check
  DrawEventMixingCheck();
  
}

/*
 * Draw dijet histograms
 *
 *  For dijet asymmetry comparison, two files are required. The first file must have the dijet asymmetry for pp
 *  and the second file the dijet asymmetries in different centrality bins for PbPb.
 */
void DijetComparingDrawer::DrawDijetHistograms(){
  
  if(!fDrawDijets) return;
  
  // For the event mixing check, there will be one added histogram for each centrality bin
  fnAddedHistograms = fBaseHistograms->GetNCentralityBins();
  
  // Legend helper variable
  TLegend *legend = new TLegend(0.5,0.48,0.8,0.83);
  legend->SetFillStyle(0);legend->SetBorderSize(0);legend->SetTextSize(0.05);legend->SetTextFont(62);
  
  // Helper variables for centrality naming in figures
  TString centralityString;
  char namer[100];
  
  // Main histogram is the dijet asymmetry from pp file
  fMainHistogram = (TH1D*)fBaseHistograms->GetOneDimensionalHistogram("dijetasymmetry",0)->Clone();
  fMainHistogram->Scale(1.0/fBaseHistograms->GetPtIntegral(0));
  legend->AddEntry(fMainHistogram,"pp","l");
  
  // Loop over centrality
  for(int iCentrality = 0; iCentrality < fBaseHistograms->GetNCentralityBins(); iCentrality++){
    
    centralityString = Form("PbPb Cent: %.0f-%.0f%%",fAddedHistograms[0]->GetCentralityBinBorder(iCentrality),fAddedHistograms[0]->GetCentralityBinBorder(iCentrality+1));
    
    // Comparison histograms are the dijet asymmetries in different centrality bins from the PbPb file
    fComparisonHistogram[iCentrality] = (TH1D*)fAddedHistograms[0]->GetOneDimensionalHistogram("dijetAsymmetry",iCentrality)->Clone();
    fComparisonHistogram[iCentrality]->Scale(1.0/fAddedHistograms[0]->GetPtIntegral(iCentrality));
    legend->AddEntry(fComparisonHistogram[iCentrality],centralityString.Data(),"l");
    
    // Ratio is taken with respect to the pp reference
    sprintf(namer,"%sRatio%d",fMainHistogram->GetName(),iCentrality);
    fRatioHistogram[iCentrality] = (TH1D*)fMainHistogram->Clone(namer);
    fRatioHistogram[iCentrality]->Divide(fComparisonHistogram[iCentrality]);
    
    
  } // Centrality loop
  
  // Draw the jet pT distributions to the upper panel of a split canvas plot
  DrawToUpperPad("A_{J}","#frac{1}{N_{dijet}} #frac{dN}{dA_{J}}");
  legend->Draw();
  
  // Draw the ratios to the lower portion of the split canvas
  DrawToLowerPad("A_{J}","#frac{PbPb}{pp}",fRatioZoomMin,fRatioZoomMax);
  
  // Save the figure to a file
  //sprintf(namerX,"%sPtRatio",fBaseHistograms->GetSingleJetHistogramName(iJetCategory));
  //SaveFigure(namerX,compactCentralityString);
}

/*
 * Draw single jet histograms
 */
void DijetComparingDrawer::DrawSingleJetHistograms(){
  
  // Legend helper variable
  TLegend *legend;
  
  // Helper variables for centrality naming in figures
  TString centralityString;
  TString compactCentralityString;
  char namerX[100];
  char namerY[100];
  const char* singleJetNormalizationName[4] = {"dijet","dijet","jet","jet"};
  
  // TODO: Do the RAA comparison nicer in some small plotter script
  int nBinsForRaa = 13;  // For RAA comparison
  double binBordersForRaa[] = {100,112,125,141,158,177,199,223,251,281,316,354,398,500}; // For RAA comparison
  double raaBinContent[] = {0.4381,0.4633,0.4847,0.5023,0.5151,0.535,0.557,0.572,0.561,0.563,0.589,0.589,0.593};
  double raaBinError[] = {0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.03,0.04,0.04};
  TH1D *raaAtlas = new TH1D("raaA","RaaA",nBinsForRaa,binBordersForRaa); raaAtlas->Sumw2();
  for(int iBin = 1; iBin <= nBinsForRaa; iBin++){
    raaAtlas->SetBinContent(iBin,raaBinContent[iBin]);
    raaAtlas->SetBinError(iBin,raaBinError[iBin]);
  }
  raaAtlas->SetLineColor(kBlack);
  raaAtlas->Scale(3);

  // Loop over single jet categories
  for(int iJetCategory = 0; iJetCategory < DijetHistogramManager::knSingleJetCategories; iJetCategory++){
    if(!fDrawSingleJets[iJetCategory]) continue;  // Only draw selected jet categories
    
    // Loop over centrality
    for(int iCentrality = fFirstDrawnCentralityBin; iCentrality <= fLastDrawnCentralityBin; iCentrality++){

      centralityString = Form("Cent: %.0f-%.0f%%",fBaseHistograms->GetCentralityBinBorder(iCentrality),fBaseHistograms->GetCentralityBinBorder(iCentrality+1));
      compactCentralityString = Form("_C=%.0f-%.0f",fBaseHistograms->GetCentralityBinBorder(iCentrality),fBaseHistograms->GetCentralityBinBorder(iCentrality+1));
      
      // Select logarithmic drawing for pT
      fDrawer->SetLogY(fLogPt);
      
      if(fApplyScaling) FindScalingFactors(iJetCategory, iCentrality, fAsymmetryBin);
      
      // === Jet pT ===
      
      // Prepare the jet pT histograms and ratio to be drawn
      PrepareRatio("jetPt", 1, iJetCategory, iCentrality, fAsymmetryBin);
      
      // Draw the jet pT distributions to the upper panel of a split canvas plot
      sprintf(namerX,"%s p_{T}  (GeV)",fBaseHistograms->GetSingleJetAxisName(iJetCategory));
      if(fApplyScaling){
        sprintf(namerY,"#frac{1}{N_{%s}} #frac{dN}{dp_{T}}  (1/GeV)",singleJetNormalizationName[iJetCategory]);
      } else {
        sprintf(namerY,"#frac{dN}{dp_{T}}  (1/GeV)");
      }
      DrawToUpperPad(namerX, namerY, fLogPt);
      
      // Add a legend to the plot
      legend = new TLegend(0.27,0.05,0.55,0.3);
      SetupLegend(legend,centralityString);
      legend->Draw();
      
      // Draw the ratios to the lower portion of the split canvas
      DrawToLowerPad(namerX,fRatioLabel.Data(),fRatioZoomMin,fRatioZoomMax);
      /*raaAtlas->Draw("same");  // TODO: Put this to some other script
      legend = new TLegend(0.2,0.6,0.5,0.8);
      legend->SetFillStyle(0);legend->SetBorderSize(0);legend->SetTextSize(0.1);legend->SetTextFont(62);
      legend->AddEntry(raaAtlas,"Atlas RAA x 3");
      legend->Draw();*/
      
      // Save the figure to a file
      sprintf(namerX,"%sPtRatio",fBaseHistograms->GetSingleJetHistogramName(iJetCategory));
      SaveFigure(namerX,compactCentralityString);
      
      // === Jet phi ===
      
      // Prepare the jet phi histograms and ratio to be drawn
      PrepareRatio("jetPhi", 1, iJetCategory, iCentrality, fAsymmetryBin);
      
      // Draw the jet phi distributions to the upper panel of a split canvas plot
      sprintf(namerX,"%s #varphi",fBaseHistograms->GetSingleJetAxisName(iJetCategory));
      if(fApplyScaling){
        sprintf(namerY,"#frac{1}{N_{%s}} #frac{dN}{d#varphi}",singleJetNormalizationName[iJetCategory]);
      } else {
        sprintf(namerY,"#frac{dN}{d#varphi}");
      }
      DrawToUpperPad(namerX,namerY);
      
      // Add a legend to the plot
      legend = new TLegend(0.62,0.65,0.82,0.9);
      SetupLegend(legend,centralityString);
      legend->Draw();
      
      // Draw the ratios to the lower portion of the split canvas
      DrawToLowerPad(namerX,fRatioLabel.Data(),fRatioZoomMin,fRatioZoomMax);
      
      // Save the figure to a file
      sprintf(namerX,"%sPhiRatio",fBaseHistograms->GetSingleJetHistogramName(iJetCategory));
      SaveFigure(namerX,compactCentralityString);

      // === Jet eta ===
      
      // Prepare the jet eta histograms and ratio to be drawn
      PrepareRatio("jetEta", 1, iJetCategory, iCentrality, fAsymmetryBin);
      
      // Draw the jet eta distributions to the upper panel of a split canvas plot
      sprintf(namerX,"%s #eta",fBaseHistograms->GetSingleJetAxisName(iJetCategory));
      if(fApplyScaling){
        sprintf(namerY,"#frac{1}{N_{%s}} #frac{dN}{d#eta}",singleJetNormalizationName[iJetCategory]);
      } else {
        sprintf(namerY,"#frac{dN}{d#eta}");
      }
      DrawToUpperPad(namerX,namerY);
      
      // Add a legend to the plot
      legend = new TLegend(0.42,0.07,0.62,0.32);
      SetupLegend(legend,centralityString);
      legend->Draw();

      // Draw the ratios to the lower portion of the split canvas
      DrawToLowerPad(namerX,fRatioLabel.Data(),fRatioZoomMin,fRatioZoomMax);
      
      // Save the figure to a file
      sprintf(namerX,"%sEtaRatio",fBaseHistograms->GetSingleJetHistogramName(iJetCategory));
      SaveFigure(namerX,compactCentralityString);
      
    } // Centrality loop
  } // Single jet category loop
}

/*
 * Draw the track histograms
 */
void DijetComparingDrawer::DrawTrackHistograms(){
  
  // Legend helper variable
  TLegend *legend;
  double legendX1;
  double legendY1;
  double legendX2;
  double legendY2;

  // Helper variables for centrality naming in figures
  TString centralityString;
  TString compactCentralityString;
  TString trackPtString;
  TString compactTrackPtString;
  char namerX[100];

  // Loop over track types
  for(int iTrackType = 0; iTrackType < DijetHistogramManager::knTrackCategories; iTrackType++){
    if(!fDrawTracks[iTrackType]) continue;  // Only draw selected tracks

    // Loop over centrality
    for(int iCentrality = fFirstDrawnCentralityBin; iCentrality <= fLastDrawnCentralityBin; iCentrality++){

      centralityString = Form("Cent: %.0f-%.0f%%",fBaseHistograms->GetCentralityBinBorder(iCentrality),fBaseHistograms->GetCentralityBinBorder(iCentrality+1));
      compactCentralityString = Form("_C=%.0f-%.0f",fBaseHistograms->GetCentralityBinBorder(iCentrality),fBaseHistograms->GetCentralityBinBorder(iCentrality+1));

      // For tracks drawing only for same and mixed events. No additional corrections are applied.
      for(int iCorrelationType = 0; iCorrelationType <= DijetHistogramManager::kMixedEvent; iCorrelationType++){
        if(!fDrawCorrelationType[iCorrelationType]) continue; // Draw only types of correlations that are requested

        // Prepare the track pT histograms and ratio to be drawn
        PrepareRatio("trackPt", 4, iTrackType, iCorrelationType, iCentrality);
        
        // Draw the track pT distributions to the upper panel of a split canvas plot
        sprintf(namerX,"%s p_{T}  (GeV)",fBaseHistograms->GetTrackAxisName(iTrackType));
        DrawToUpperPad(namerX, "#frac{dN}{dp_{T}}  (1/GeV)", fLogPt); // TODO: Add correlation type to title
        
        // Add a legend to the plot
        legend = new TLegend(0.56,0.66,0.76,0.81);
        SetupLegend(legend,centralityString);
        legend->Draw();
        
        // Draw the ratios to the lower portion of the split canvas
        fDrawer->SetGridY(true);
        DrawToLowerPad(namerX,fRatioLabel.Data(),fRatioZoomMin,fRatioZoomMax);
        fDrawer->SetGridY(false);
        
        // Save the figure to a file
        sprintf(namerX,"%sPtRatio",fBaseHistograms->GetTrackHistogramName(iTrackType));
        SaveFigure(namerX,compactCentralityString,fBaseHistograms->GetCompactCorrelationTypeString(iCorrelationType));

        // Loop over track pT bins
        for(int iTrackPt = fFirstDrawnTrackPtBin; iTrackPt <= fBaseHistograms->GetNTrackPtBins(); iTrackPt++){

          // Draw the selected track pT bins and the special bin containing integrated distributions
          if(iTrackPt > fLastDrawnTrackPtBin && iTrackPt != fBaseHistograms->GetNTrackPtBins()) continue;

          // Set the correct track pT bins
          trackPtString = Form("Track pT: %.1f-%.1f GeV",fBaseHistograms->GetTrackPtBinBorder(iTrackPt),fBaseHistograms->GetTrackPtBinBorder(iTrackPt+1));
          compactTrackPtString = Form("_pT=%.1f-%.1f",fBaseHistograms->GetTrackPtBinBorder(iTrackPt),fBaseHistograms->GetTrackPtBinBorder(iTrackPt+1));
          compactTrackPtString.ReplaceAll(".","v");

          // No pT selection for integrated distributions
          if(iTrackPt == fBaseHistograms->GetNTrackPtBins()){
            trackPtString = "";
            compactTrackPtString = "";
          }

          // === Track phi ===
          
          // Prepare the track phi histograms to be drawn
          PrepareRatio("trackPhi", 1, iTrackType, iCorrelationType, iCentrality, iTrackPt);
          
          // Draw the track phi distributions to the upper panel of a split canvas plot
          sprintf(namerX,"%s #varphi",fBaseHistograms->GetTrackAxisName(iTrackType));
          DrawToUpperPad(namerX, "#frac{dN}{d#varphi}"); // TODO: Add correlation type to title

          // Add a legend to the plot
          legend = new TLegend(0.24,0.11,0.44,0.26);
          SetupLegend(legend,centralityString,trackPtString);
          legend->Draw();

          // Draw the ratios to the lower portion of the split canvas
          fDrawer->SetGridY(true);
          DrawToLowerPad(namerX,fRatioLabel.Data(),fRatioZoomMin,fRatioZoomMax);
          fDrawer->SetGridY(false);
          
          // Save the figure to a file
          sprintf(namerX,"%sPhiRatio",fBaseHistograms->GetTrackHistogramName(iTrackType));
          SaveFigure(namerX,compactCentralityString,fBaseHistograms->GetCompactCorrelationTypeString(iCorrelationType),compactTrackPtString);

          // === Track eta ===

          // Set nice position for the legend
          legendY1 = 0.20; legendY2 = legendY1+0.15;
          if(iTrackPt == fBaseHistograms->GetNTrackPtBins()){
            legendX1 = 0.4; legendX2 = legendX1+0.2;
          } else if (iTrackPt == fBaseHistograms->GetNTrackPtBins() - 1){
            legendX1 = 0.32; legendX2 = legendX1+0.2;
          } else {
            legendX1 = 0.34; legendX2 = legendX1+0.2;
          }

          // Prepare the track eta histograms to be drawn
          PrepareRatio("trackEta", 1, iTrackType, iCorrelationType, iCentrality, iTrackPt);
          
          // Draw the track eta distributions to the upper panel of a split canvas plot
          sprintf(namerX,"%s #eta",fBaseHistograms->GetTrackAxisName(iTrackType));
          DrawToUpperPad(namerX, "#frac{dN}{d#eta}"); // TODO: Add correlation type to title
          
          // Add a legend to the plot
          legend = new TLegend(legendX1,legendY1,legendX2,legendY2);
          SetupLegend(legend,centralityString,trackPtString);
          legend->Draw();

          // Draw the ratios to the lower portion of the split canvas
          fDrawer->SetGridY(true);
          DrawToLowerPad(namerX,fRatioLabel.Data(),fRatioZoomMin,fRatioZoomMax);
          fDrawer->SetGridY(false);
          
          // Save the figure to a file
          sprintf(namerX,"%sEtaRatio",fBaseHistograms->GetTrackHistogramName(iTrackType));
          SaveFigure(namerX,compactCentralityString,fBaseHistograms->GetCompactCorrelationTypeString(iCorrelationType),compactTrackPtString);

        } // Track pT loop
      } // Correlation type loop
    } // Centrality loop
  } // Track type loop
}

/*
 * Draw different deltaEta regions in mixed event corrected deltaPhi histograms to the same figure to ensure event mixing gives good background
 */
void DijetComparingDrawer::DrawEventMixingCheck(){
  
  // Only draw is selected to do so
  if(!fDrawEventMixingCheck) return;
  
  // Legend helper variables
  TLegend *legend;
  double legendX1, legendX2, legendY1, legendY2;
  
  // Helper variables for centrality naming in figures
  TString centralityString;
  TString compactCentralityString;
  TString trackPtString;
  TString compactTrackPtString;
  char namerX[150];
  char namerY[150];
  
  // Rebinning
  int nRebinDeltaEta = 10; // 500 bins in deltaEta
  int nRebinDeltaPhi = 2; // 200 bins in deltaPhi
  
  // Manual zooming options for deltaEta and deltaPhi
  double deltaPhiZoomAdder = 0.2;
  double deltaEtaZoomAdder[6] = {0.2,0.2,0.3,0.4,0.5,0.7};
  double deltaEtaZoomAdderSubleadingJet[6] = {0.2,0.3,0.5,0.8,1,1.5};
  double zoomMin, zoomMax;
  
  // For the event mixing check, there will be one added histogram
  fnAddedHistograms = 1;
  
  // Loop over jet-track correlation categories
  for(int iJetTrack = 0; iJetTrack < DijetHistogramManager::knJetTrackCorrelations; iJetTrack++){
    if(!fDrawJetTrackCorrelations[iJetTrack]) continue;  // Only draw the selected categories
    
    // Loop over centrality
    for(int iCentrality = fFirstDrawnCentralityBin; iCentrality <= fLastDrawnCentralityBin; iCentrality++){
      
      centralityString = Form("Cent: %.0f-%.0f%%",fBaseHistograms->GetCentralityBinBorder(iCentrality),fBaseHistograms->GetCentralityBinBorder(iCentrality+1));
      compactCentralityString = Form("_C=%.0f-%.0f",fBaseHistograms->GetCentralityBinBorder(iCentrality),fBaseHistograms->GetCentralityBinBorder(iCentrality+1));
      
      // Loop over track pT bins
      for(int iTrackPt = fFirstDrawnTrackPtBin; iTrackPt <= fLastDrawnTrackPtBin; iTrackPt++){
        
        // Set the correct track pT bins
        trackPtString = Form("Track pT: %.1f-%.1f GeV",fBaseHistograms->GetTrackPtBinBorder(iTrackPt),fBaseHistograms->GetTrackPtBinBorder(iTrackPt+1));
        compactTrackPtString = Form("_pT=%.1f-%.1f",fBaseHistograms->GetTrackPtBinBorder(iTrackPt),fBaseHistograms->GetTrackPtBinBorder(iTrackPt+1));
        compactTrackPtString.ReplaceAll(".","v");
        
        // Change the zoom value for subleading jets
        if(iJetTrack == DijetHistogramManager::kTrackSubleadingJet || iJetTrack == DijetHistogramManager::kPtWeightedTrackSubleadingJet){
          deltaEtaZoomAdder[iTrackPt] = deltaEtaZoomAdderSubleadingJet[iTrackPt];
        }
        
        /////////////////////////////////////////////
        //   Step one, deltaPhi in deltaEta bins   //
        /////////////////////////////////////////////
        
        // Set up the histograms and draw them to the upper pad of a split canvas
        fMainHistogram = (TH1D*)fBaseHistograms->GetHistogramJetTrackDeltaPhi(iJetTrack,fEventMixingDistribution,DijetHistogramManager::kMaxAsymmetryBins,iCentrality,iTrackPt,DijetHistogramManager::kSignalEtaRegion)->Clone();

        // Possibility to do rebinning
        if(nRebinDeltaPhi > 1){
          fMainHistogram->Rebin(nRebinDeltaPhi);
          fMainHistogram->Scale(1.0/nRebinDeltaPhi);
        }
        
        fMainHistogram->GetXaxis()->SetRangeUser(-TMath::Pi()/2.0,TMath::Pi()/2.0); // Only plot near side
        
        fComparisonHistogram[0] = (TH1D*)fBaseHistograms->GetHistogramJetTrackDeltaPhi(iJetTrack,fEventMixingDistribution,DijetHistogramManager::kMaxAsymmetryBins,iCentrality,iTrackPt,DijetHistogramManager::kBackgroundEtaRegion)->Clone();
        
        // Possibility to do rebinning
        if(nRebinDeltaPhi > 1){
          fComparisonHistogram[0]->Rebin(nRebinDeltaPhi);
          fComparisonHistogram[0]->Scale(1.0/nRebinDeltaPhi);
        }
        
        fComparisonHistogram[0]->GetXaxis()->SetRangeUser(-TMath::Pi()/2.0,TMath::Pi()/2.0); // Only plot near side
        
        // If specified, zoom to the tails of the histogram to see the most interesting region in detail
        if(fEventMixingZoom) ZoomToRegion(TMath::Pi()/2.0,10,4,true,true);

        sprintf(namerX,"%s #Delta#varphi",fBaseHistograms->GetJetTrackAxisName(iJetTrack));
        DrawToUpperPad(namerX,"#frac{1}{N_{jets}}  #frac{dN}{d#Delta#varphi}");

        // Setup a legend to the plot
        legendX1 = 0.22; legendX2 = 0.5; legendY1 = 0.71; legendY2 = 0.91;
        if(fBaseHistograms->GetSystem().Contains("PbPb")){
          legendY1 = 0.65; legendY2 = 0.92;
        }
        legend = new TLegend(legendX1,legendY1,legendX2,legendY2);
        legend->SetFillStyle(0);legend->SetBorderSize(0);legend->SetTextSize(0.05);legend->SetTextFont(62);
        if(fBaseHistograms->GetSystem().Contains("PbPb")) legend->AddEntry((TObject*) 0,centralityString.Data(),"");
        legend->AddEntry((TObject*) 0,trackPtString.Data(),"");
        legend->AddEntry(fMainHistogram,"#||{#Delta#eta} < 1.0","l");
        legend->AddEntry(fComparisonHistogram[0],"1.5 < #||{#Delta#eta} < 2.5","l");
        legend->Draw();

        // Prepare the ratio and draw it to the lower pad
        fRatioHistogram[0] = (TH1D*) fMainHistogram->Clone(Form("mixedEventDeltaPhiCheckRatio%d%d%d",iJetTrack,iCentrality,iTrackPt));
        
        if(fUseDifferenceInsteadOfRatio){
          fRatioHistogram[0]->Add(fComparisonHistogram[0],-1);
          sprintf(namerY,"Black - #color[2]{red}");
        } else {
          fRatioHistogram[0]->Divide(fComparisonHistogram[0]);
          sprintf(namerY,"#frac{#||{#Delta#eta} < 1.0}{1.5 < #||{#Delta#eta} < 2.5}");
        }
        
        if(fEventMixingZoom){
          zoomMin = 1 - deltaPhiZoomAdder;
          zoomMax = 1 + deltaPhiZoomAdder;
          if(zoomMin < 0) zoomMin = 0;
        } else {
          zoomMin = fRatioZoomMin;
          zoomMax = fRatioZoomMax;
        }
        DrawToLowerPad(namerX,namerY,zoomMin,zoomMax);
        
        // Save the figure to a file
        sprintf(namerX,"%sMixedEventPhiCheck",fBaseHistograms->GetJetTrackHistogramName(iJetTrack));
        if(fEventMixingZoom) sprintf(namerX,"%sMixedEventPhiCheckZoom",fBaseHistograms->GetJetTrackHistogramName(iJetTrack));
        SaveFigure(namerX,compactCentralityString,compactTrackPtString);
        
        /////////////////////////////////////////////
        //   Step two, deltaEta in deltaPhi bins   //
        /////////////////////////////////////////////
        
        // Set up the histograms and draw them to the upper pad of a split canvas
        fMainHistogram = (TH1D*)fBaseHistograms->GetHistogramJetTrackDeltaEta(iJetTrack,fEventMixingDistribution, DijetHistogramManager::kMaxAsymmetryBins, iCentrality,iTrackPt,DijetHistogramManager::kNearSide)->Clone();
        
        // Possibility to do rebinning
        if(nRebinDeltaEta > 1){
          fMainHistogram->Rebin(nRebinDeltaEta);
          fMainHistogram->Scale(1.0/nRebinDeltaEta);
        }
        
        // Zoom the interesting region
        fMainHistogram->GetXaxis()->SetRangeUser(-3,3);
        
        fComparisonHistogram[0] = (TH1D*)fBaseHistograms->GetHistogramJetTrackDeltaEta(iJetTrack,fEventMixingDistribution, DijetHistogramManager::kMaxAsymmetryBins,iCentrality,iTrackPt,DijetHistogramManager::kBetweenPeaks)->Clone();
        
        // Possibility to do rebinning
        if(nRebinDeltaEta > 1){
          fComparisonHistogram[0]->Rebin(nRebinDeltaEta);
          fComparisonHistogram[0]->Scale(1.0/nRebinDeltaEta);
        }
        
        // Zoom the interesting region
        fComparisonHistogram[0]->GetXaxis()->SetRangeUser(-3,3);
        
        // If specified, zoom to the tails of the distribution to see the interesting region better
        if(fEventMixingZoom) ZoomToRegion(3,10,3,true,false);
        
        sprintf(namerX,"%s #Delta#eta",fBaseHistograms->GetJetTrackAxisName(iJetTrack));
        DrawToUpperPad(namerX,"#frac{1}{N_{jets}}  #frac{dN}{d#Delta#eta}");
        
        // Setup a legend to the plot
        legend = new TLegend(legendX1,legendY1,legendX2,legendY2);
        legend->SetFillStyle(0);legend->SetBorderSize(0);legend->SetTextSize(0.05);legend->SetTextFont(62);
        if(fBaseHistograms->GetSystem().Contains("PbPb")) legend->AddEntry((TObject*) 0,centralityString.Data(),"");
        legend->AddEntry((TObject*) 0,trackPtString.Data(),"");
        sprintf(namerX,"%.1f < #Delta#varphi < %.1f",fBaseHistograms->GetDeltaPhiBorderLow(DijetHistogramManager::kNearSide),fBaseHistograms->GetDeltaPhiBorderHigh(DijetHistogramManager::kNearSide));
        legend->AddEntry(fMainHistogram,namerX,"l");
        
        // If the upper deltaPhi limit is at 3Pi/2, format legend as deltaPhi > valueLow instead of valueLow < deltaPhi < valueHigh
        if(fBaseHistograms->GetDeltaPhiBorderHigh(DijetHistogramManager::kBetweenPeaks) > 3*TMath::Pi()/2-0.1){
          sprintf(namerX,"#Delta#varphi > %.1f",fBaseHistograms->GetDeltaPhiBorderLow(DijetHistogramManager::kBetweenPeaks));
        } else {
          sprintf(namerX,"%.1f < #Delta#varphi < %.1f", fBaseHistograms->GetDeltaPhiBorderLow(DijetHistogramManager::kBetweenPeaks), fBaseHistograms->GetDeltaPhiBorderHigh(DijetHistogramManager::kBetweenPeaks));
        }
        
        legend->AddEntry(fComparisonHistogram[0],namerX,"l");
        legend->Draw();
        
        // Prepare the ratio and draw it to the lower pad
        sprintf(namerX,"%s #Delta#eta",fBaseHistograms->GetJetTrackAxisName(iJetTrack));
        
        // If the upper deltaPhi limit is at 3Pi/2, format axis title as deltaPhi > valueLow instead of valueLow < deltaPhi < valueHigh
        
        if(fBaseHistograms->GetDeltaPhiBorderHigh(DijetHistogramManager::kBetweenPeaks) > 3*TMath::Pi()/2-0.1){
          sprintf(namerY,"#frac{%.1f < #Delta#varphi < %.1f}{#Delta#varphi > %.1f}",fBaseHistograms->GetDeltaPhiBorderLow(DijetHistogramManager::kNearSide), fBaseHistograms->GetDeltaPhiBorderHigh(DijetHistogramManager::kNearSide), fBaseHistograms->GetDeltaPhiBorderLow(DijetHistogramManager::kBetweenPeaks));
        } else {
          sprintf(namerY,"#frac{%.1f < #Delta#varphi < %.1f}{%.1f < #Delta#varphi < %.1f}",fBaseHistograms->GetDeltaPhiBorderLow(DijetHistogramManager::kNearSide), fBaseHistograms->GetDeltaPhiBorderHigh(DijetHistogramManager::kNearSide), fBaseHistograms->GetDeltaPhiBorderLow(DijetHistogramManager::kBetweenPeaks), fBaseHistograms->GetDeltaPhiBorderHigh(DijetHistogramManager::kBetweenPeaks));
        }
        
        fRatioHistogram[0] = (TH1D*) fMainHistogram->Clone(Form("mixedEventDeltaEtaCheckRatio%d%d%d",iJetTrack,iCentrality,iTrackPt));
        
        if(fUseDifferenceInsteadOfRatio){
          fRatioHistogram[0]->Add(fComparisonHistogram[0],-1);
          sprintf(namerY,"Black - #color[2]{red}");
        } else {
          fRatioHistogram[0]->Divide(fComparisonHistogram[0]);
        }
        
        
        if(fEventMixingZoom){
          zoomMin = 1 - deltaEtaZoomAdder[iTrackPt];
          zoomMax = 1 + deltaEtaZoomAdder[iTrackPt];
          if(zoomMin < 0) zoomMin = 0;
        } else {
          zoomMin = fRatioZoomMin;
          zoomMax = fRatioZoomMax;
        }
        DrawToLowerPad(namerX,namerY,zoomMin,zoomMax);
        
        // Save the figure to a file
        sprintf(namerX,"%sMixedEventEtaCheck",fBaseHistograms->GetJetTrackHistogramName(iJetTrack));
        if(fEventMixingZoom) sprintf(namerX,"%sMixedEventEtaCheckZoom",fBaseHistograms->GetJetTrackHistogramName(iJetTrack));
        SaveFigure(namerX,compactCentralityString,compactTrackPtString);
        
      } // Track pT loop
    } // Centrality loop
  } // Jet-track category loop
  
}

/*
 * Drawer for track jet correlation histograms
 */
void DijetComparingDrawer::DrawJetTrackCorrelationHistograms(){
  
  // Legend helper variables
  TLegend *legend;
  double legendX1;
  double legendY1;
  double legendX2;
  double legendY2;

  // Helper variables for centrality naming in figures
  TString centralityString;
  TString compactCentralityString;
  TString trackPtString;
  TString compactTrackPtString;
  char namerX[100];
  
  // Loop over jet-track correlation categories
  for(int iJetTrack = 0; iJetTrack < DijetHistogramManager::knJetTrackCorrelations; iJetTrack++){
    if(!fDrawJetTrackCorrelations[iJetTrack]) continue;  // Only draw the selected categories
    
    // Loop over centrality
    for(int iCentrality = fFirstDrawnCentralityBin; iCentrality <= fLastDrawnCentralityBin; iCentrality++){
      
      centralityString = Form("Cent: %.0f-%.0f%%",fBaseHistograms->GetCentralityBinBorder(iCentrality),fBaseHistograms->GetCentralityBinBorder(iCentrality+1));
      compactCentralityString = Form("_C=%.0f-%.0f",fBaseHistograms->GetCentralityBinBorder(iCentrality),fBaseHistograms->GetCentralityBinBorder(iCentrality+1));

      FindScalingFactors(iJetTrack/3, iCentrality, DijetHistogramManager::kMaxAsymmetryBins);
      
      // Draw only selected event correlation types
      for(int iCorrelationType = 0; iCorrelationType < DijetHistogramManager::knCorrelationTypes; iCorrelationType++){
        if(!fDrawCorrelationType[iCorrelationType]) continue; // Draw only types of correlations that are requested

        // Same and mixed event histograms are not scaled by the number of dijets in the histogram manager
        // For all the other histograms the scaling is already done
        fApplyScaling = (iCorrelationType == DijetHistogramManager::kSameEvent || iCorrelationType == DijetHistogramManager::kMixedEvent);
        
        // Loop over track pT bins
        for(int iTrackPt = fFirstDrawnTrackPtBin; iTrackPt <= fLastDrawnTrackPtBin; iTrackPt++){

          // Set the correct track pT bins
          trackPtString = Form("Track pT: %.1f-%.1f GeV",fBaseHistograms->GetTrackPtBinBorder(iTrackPt),fBaseHistograms->GetTrackPtBinBorder(iTrackPt+1));
          compactTrackPtString = Form("_pT=%.1f-%.1f",fBaseHistograms->GetTrackPtBinBorder(iTrackPt),fBaseHistograms->GetTrackPtBinBorder(iTrackPt+1));
          compactTrackPtString.ReplaceAll(".","v");

          // ===== Jet-track deltaPhi =====
          if(fDrawJetTrackDeltaPhi){

            PrepareRatio("jettrackdeltaphi", 1, iJetTrack, iCorrelationType, DijetHistogramManager::kMaxAsymmetryBins, iCentrality, iTrackPt, DijetHistogramManager::kWholeEta);
            
            // Draw the track phi distributions to the upper panel of a split canvas plot
            sprintf(namerX,"%s #Delta#varphi",fBaseHistograms->GetJetTrackAxisName(iJetTrack));
            DrawToUpperPad(namerX, "#frac{dN}{d#Delta#varphi}");
            
            // Move legend to different place for leading jet background figures
            legendX1 = 0.52; legendY1 = 0.75; legendX2 = 0.82; legendY2 = 0.9;
            if(iJetTrack < DijetHistogramManager::kTrackSubleadingJet){
              if(iCorrelationType == DijetHistogramManager::kBackground) { // Move legend to top left corner for leading jet-track background figures
                legendX1 = 0.17; legendY1 = 0.75; legendX2 = 0.37; legendY2 = 0.9;
              } else if (iCorrelationType == DijetHistogramManager::kCorrected || iCorrelationType == DijetHistogramManager::kBackgroundSubtracted){ // Move legend away from peaks
                if(iTrackPt == 2 || iTrackPt == 3){
                  legendX1 = 0.31; legendY1 = 0.75; legendX2 = 0.61; legendY2 = 0.9;
                } else if (iTrackPt < 2){
                  legendX1 = 0.17; legendY1 = 0.75; legendX2 = 0.37; legendY2 = 0.9;
                }
              }
            }

            legend = new TLegend(legendX1,legendY1,legendX2,legendY2);
            SetupLegend(legend,centralityString,trackPtString);
            legend->Draw();
            
            // Draw the ratios to the lower portion of the split canvas
            fDrawer->SetGridY(true);
            DrawToLowerPad(namerX,fRatioLabel.Data(),fRatioZoomMin,fRatioZoomMax);
            fDrawer->SetGridY(false);

            // Save the figure to a file
            sprintf(namerX,"%sDeltaPhiComparison",fBaseHistograms->GetJetTrackHistogramName(iJetTrack));
            SaveFigure(namerX, compactCentralityString, compactTrackPtString, fBaseHistograms->GetCompactCorrelationTypeString(iCorrelationType));
          } // Drawing jet-track deltaPhi

          // Note: There is no deltaEta-deltaPhi drawing here, as ratio for two dimensional distribution is not so
          // straightforward

          // ===== Jet-track deltaEta =====
          if(fDrawJetTrackDeltaEta){
            for(int iDeltaPhi = 0; iDeltaPhi < DijetHistogramManager::knDeltaPhiBins; iDeltaPhi++){

              // Do not draw the deltaEta histograms for background because they are flat by construction
              if(iCorrelationType == DijetHistogramManager::kBackground) continue;

              PrepareRatio("jettrackdeltaeta", 4, iJetTrack, iCorrelationType, DijetHistogramManager::kMaxAsymmetryBins, iCentrality, iTrackPt, iDeltaPhi, -3, 3);
              
              // Draw the track phi distributions to the upper panel of a split canvas plot
              sprintf(namerX,"%s #Delta#eta",fBaseHistograms->GetJetTrackAxisName(iJetTrack));
              DrawToUpperPad(namerX, "#frac{dN}{d#Delta#eta}");
              
              // Move legend to different place for mixed event distributions
              if(iCorrelationType == DijetHistogramManager::kBackgroundSubtracted && iDeltaPhi == DijetHistogramManager::kBetweenPeaks){
                legendX1 = 0.31; legendY1 = 0.75; legendX2 = 0.61; legendY2 = 0.9;
              } else if(iCorrelationType == DijetHistogramManager::kMixedEvent || iDeltaPhi > DijetHistogramManager::kNearSide) {
                legendX1 = 0.31; legendY1 = 0.25; legendX2 = 0.61; legendY2 = 0.4;
              } else {
                legendX1 = 0.52; legendY1 = 0.6; legendX2 = 0.82; legendY2 = 0.9;
              }

             // fCorrelationTypeString[iCorrelationType]+fDeltaPhiString[iDeltaPhi])
              legend = new TLegend(legendX1,legendY1,legendX2,legendY2);
              legend->SetHeader(fBaseHistograms->GetCorrelationTypeString(iCorrelationType) + " " + fBaseHistograms->GetDeltaPhiString(iDeltaPhi));
              SetupLegend(legend,centralityString,trackPtString);
              legend->Draw();

              // Draw the ratios to the lower portion of the split canvas
              fDrawer->SetGridY(true);
              DrawToLowerPad(namerX,fRatioLabel.Data(),fRatioZoomMin,fRatioZoomMax);
              fDrawer->SetGridY(false);
              
              // Save the figure to a file
              sprintf(namerX,"%sDeltaEtaComparison",fBaseHistograms->GetJetTrackHistogramName(iJetTrack));
              SaveFigure(namerX, compactCentralityString, compactTrackPtString, fBaseHistograms->GetCompactCorrelationTypeString(iCorrelationType), fBaseHistograms->GetCompactDeltaPhiString(iDeltaPhi));


            } // DeltaPhi loop
          } // Drawing jet-track deltaEta
        } // Track pT loop
      } // Correlation type loop
    } // Centrality loop
  } // Jet-track correlation category loop
}

/*
 * Drawer for jet shape histograms compared to inclusive analysis
 */
void DijetComparingDrawer::DrawJetShapeHistograms(){
  
  // Only draw the jet shape comparison if chosen to do so
  if(!fDrawJetShape[DijetHistogramManager::kJetShape]) return;
  
  // Legend helper variables
  TLegend *legend;
  double legendX1;
  double legendY1;
  double legendX2;
  double legendY2;
  
  // Helper variables for centrality naming in figures
  TString centralityString;
  TString compactCentralityString;
  TString trackPtString;
  TString compactTrackPtString;
  char namerX[100];
  char namerY[100];
  
  TFile *comparisonFile = TFile::Open("data/JS5TeV_HIN_16_020.root");
  //TFile *comparisonFile = TFile::Open("data/inclJetShapes_GenGen_PYTHIA6.root");
  const int nTrackPtBins = fBaseHistograms->GetNTrackPtBins();
  const int nCentralityBins = fBaseHistograms->GetNCentralityBins();
  TH1D *comparisonHistograms[nCentralityBins][nTrackPtBins];
  TH1D *sumHistogram[nCentralityBins];
  TH1D *dijetSumHistogram;
  TH1D *helperHistogram;
  
  // Find the histograms to compare with from the comparison file
  //comparisonHistograms[0] = (TH1D*) comparisonFile->Get("dr_pTweighted_0_0");
  for(int iCentrality = 0; iCentrality < nCentralityBins; iCentrality++){
    sprintf(namerX,"JS_pb_0_%d",iCentrality);
    comparisonHistograms[iCentrality][0] = (TH1D*) comparisonFile->Get(namerX);
    sprintf(namerX,"normalizationSum%d",iCentrality);
    sumHistogram[iCentrality] = (TH1D*) comparisonHistograms[iCentrality][0]->Clone(namerX);
    for(int iTrackPt = 1; iTrackPt < nTrackPtBins; iTrackPt++){
      sprintf(namerX,"JS_pb_%d_%d",iTrackPt,iCentrality);
      //sprintf(namerX,"dr_pTweighted_%d_0",iTrackPt);
      comparisonHistograms[iCentrality][iTrackPt] = (TH1D*) comparisonFile->Get(namerX);
      sumHistogram[iCentrality]->Add(comparisonHistograms[iCentrality][iTrackPt]);
    }
  }
  
  // There are more pT bins in the comparison file, so sum them up to match the pT bins in this analysis
  for(int iCentrality = 0; iCentrality < nCentralityBins; iCentrality++){
    for(int iTrackPt = nTrackPtBins; iTrackPt < 9; iTrackPt++){
      sprintf(namerX,"JS_pb_%d_%d",iTrackPt,iCentrality);
      //sprintf(namerX,"dr_pTweighted_%d_0",iTrackPt);
      helperHistogram = (TH1D*) comparisonFile->Get(namerX);
      comparisonHistograms[iCentrality][nTrackPtBins-1]->Add(helperHistogram);
      sumHistogram[iCentrality]->Add(helperHistogram);
    }
  }
  
  // Normalize the jet shape histograms from the comparison file
  //double jetShapeIntegral = sumHistogram->Integral(1,sumHistogram->FindBin(0.99),"width");
  //for(int iTrackPt = 0; iTrackPt < nTrackPtBins; iTrackPt++){
    //comparisonHistograms[iTrackPt]->Scale(1.0/jetShapeIntegral);
  //}
  
  // Scale the sum histogram
  //sumHistogram->Scale(1.0/jetShapeIntegral);
  
  // For the jet shape, there will be one added histogram
  fnAddedHistograms = 1;
  
  // Loop over jet-track correlation categories
  for(int iJetTrack = 0; iJetTrack < DijetHistogramManager::knJetTrackCorrelations; iJetTrack++){
    if(!fDrawJetTrackCorrelations[iJetTrack]) continue;  // Only draw the selected categories
    
    // Loop over centrality
    for(int iCentrality = fFirstDrawnCentralityBin; iCentrality <= fLastDrawnCentralityBin; iCentrality++){
      
      centralityString = Form("Cent: %.0f-%.0f%%",fBaseHistograms->GetCentralityBinBorder(iCentrality),fBaseHistograms->GetCentralityBinBorder(iCentrality+1));
      compactCentralityString = Form("_C=%.0f-%.0f",fBaseHistograms->GetCentralityBinBorder(iCentrality),fBaseHistograms->GetCentralityBinBorder(iCentrality+1));
      
      // Loop over track pT bins
      for(int iTrackPt = fFirstDrawnTrackPtBin; iTrackPt <= fLastDrawnTrackPtBin; iTrackPt++){
        
        // Set the correct track pT bins
        trackPtString = Form("Track pT: %.1f-%.1f GeV",fBaseHistograms->GetTrackPtBinBorder(iTrackPt),fBaseHistograms->GetTrackPtBinBorder(iTrackPt+1));
        compactTrackPtString = Form("_pT=%.1f-%.1f",fBaseHistograms->GetTrackPtBinBorder(iTrackPt),fBaseHistograms->GetTrackPtBinBorder(iTrackPt+1));
        compactTrackPtString.ReplaceAll(".","v");
        
        legendX1 = 0.48; legendY1 = 0.68; legendX2 = 0.82; legendY2 = 0.93;
        sprintf(namerX,"#DeltaR");
        sprintf(namerY,"%s",fBaseHistograms->GetJetShapeAxisName(DijetHistogramManager::kJetShape));
        
        // Prepare the histograms and draw then to the upper pad
        fComparisonHistogram[0] = comparisonHistograms[iCentrality][iTrackPt];
        fMainHistogram = (TH1D*) fComparisonHistogram[0]->Clone(Form("Klooni%d%d%d",iJetTrack,iCentrality,iTrackPt));
        helperHistogram = (TH1D*)fBaseHistograms->GetHistogramJetShape(DijetHistogramManager::kJetShape,iJetTrack, DijetHistogramManager::kMaxAsymmetryBins,iCentrality,iTrackPt)->Clone();
        
        // A couple of last bins are missing from the comparison histogram, so drop them also from main histogram
        for(int iBin = 1; iBin <= fMainHistogram->GetNbinsX(); iBin++){
          fMainHistogram->SetBinContent(iBin,helperHistogram->GetBinContent(iBin));
          fMainHistogram->SetBinError(iBin,helperHistogram->GetBinError(iBin));
        }
        
        // Create a sum histogram for the jet shape from dijet analysis
        if(iTrackPt == fFirstDrawnTrackPtBin){
          dijetSumHistogram = (TH1D*) fMainHistogram->Clone(Form("dijetSum%d%d%d",iJetTrack,iCentrality,iTrackPt));
        } else {
          dijetSumHistogram->Add(fMainHistogram);
        }
        
        DrawToUpperPad(namerX,namerY,fLogJetShape);
        
        // Setup a legend to the plot
        legend = new TLegend(legendX1,legendY1,legendX2,legendY2);
        legend->SetFillStyle(0);legend->SetBorderSize(0);legend->SetTextSize(0.05);legend->SetTextFont(62);
        if(fBaseHistograms->GetSystem().Contains("PbPb")) legend->AddEntry((TObject*) 0,centralityString.Data(),"");
        legend->AddEntry((TObject*) 0,trackPtString.Data(),"");
        legend->AddEntry(fMainHistogram,"This analysis pp","l");
        legend->AddEntry(fComparisonHistogram[0],"Inclusive pp","l");
        legend->Draw();
        
        // Prepare the ratio and draw it to the lower pad
        fRatioHistogram[0] = (TH1D*) fMainHistogram->Clone(Form("jetShapeRatio%d%d%d",iJetTrack,iCentrality,iTrackPt));
        fRatioHistogram[0]->Divide(fComparisonHistogram[0]);
        DrawToLowerPad(namerX,fRatioLabel.Data(),fRatioZoomMin,fRatioZoomMax);
        
        // Save the figure to a file
        sprintf(namerX,"%s%sRatio",fBaseHistograms->GetJetTrackHistogramName(iJetTrack),fBaseHistograms->GetJetShapeHistogramName(DijetHistogramManager::kJetShape));
        SaveFigure(namerX,compactCentralityString,compactTrackPtString);
        
      } // Track pT loop
      
      // Draw the histogram for pT summed jet shape
      
      // Set the correct track pT bins
      trackPtString = Form("Track pT: %.1f-%.1f GeV",fBaseHistograms->GetTrackPtBinBorder(fFirstDrawnTrackPtBin),fBaseHistograms->GetTrackPtBinBorder(fLastDrawnTrackPtBin+1));
      compactTrackPtString = Form("_pT=%.1f-%.1f",fBaseHistograms->GetTrackPtBinBorder(fFirstDrawnTrackPtBin),fBaseHistograms->GetTrackPtBinBorder(fLastDrawnTrackPtBin+1));
      compactTrackPtString.ReplaceAll(".","v");
      
      // Setup legend position and axis names
      legendX1 = 0.48; legendY1 = 0.68; legendX2 = 0.82; legendY2 = 0.93;
      sprintf(namerX,"#DeltaR");
      sprintf(namerY,"%s",fBaseHistograms->GetJetShapeAxisName(DijetHistogramManager::kJetShape));
      
      // Setup the main histogram and comparison histogram and draw them
      fMainHistogram = dijetSumHistogram;
      fComparisonHistogram[0] = sumHistogram[iCentrality];
      
      DrawToUpperPad(namerX,namerY,fLogJetShape);
      
      // Setup a legend to the plot
      legend = new TLegend(legendX1,legendY1,legendX2,legendY2);
      legend->SetFillStyle(0);legend->SetBorderSize(0);legend->SetTextSize(0.05);legend->SetTextFont(62);
      if(fBaseHistograms->GetSystem().Contains("PbPb")) legend->AddEntry((TObject*) 0,centralityString.Data(),"");
      legend->AddEntry((TObject*) 0,trackPtString.Data(),"");
      legend->AddEntry(fMainHistogram,"This analysis pp","l");
      legend->AddEntry(fComparisonHistogram[0],"Inclusive pp","l");
      legend->Draw();
      
      // Prepare the ratio and draw it to the lower pad
      fRatioHistogram[0] = (TH1D*) fMainHistogram->Clone(Form("jetShapeRatio%d%d",iJetTrack,iCentrality));
      fRatioHistogram[0]->Divide(fComparisonHistogram[0]);
      DrawToLowerPad(namerX,fRatioLabel.Data(),fRatioZoomMin,fRatioZoomMax);
      
      // Save the figure to a file
      sprintf(namerX,"%s%sRatio",fBaseHistograms->GetJetTrackHistogramName(iJetTrack),fBaseHistograms->GetJetShapeHistogramName(DijetHistogramManager::kJetShape));
      SaveFigure(namerX,compactCentralityString,compactTrackPtString);
      
    } // Centrality loop
  } // Jet-track correlation category loop
  
}

/*
 * Drawer for comparison between data and MC from this analysis
 */
void DijetComparingDrawer::DrawJetShapeMCComparison(){
  
  // Only draw the jet shape comparison if chosen to do so
  if(!fDrawJetShape[DijetHistogramManager::kJetShapeBinCount]) return;
  
  // Legend helper variables
  TLegend *legend;
  double legendX1;
  double legendY1;
  double legendX2;
  double legendY2;
  
  // Helper variables for centrality naming in figures
  TString centralityString;
  TString compactCentralityString;
  TString trackPtString;
  TString compactTrackPtString;
  TString asymmetryString = "";
  TString compactAsymmetryString = "";
  char namerX[100];
  
  // Check if we are doing comparison for pp
  bool isPp = fBaseHistograms->GetSystem().Contains("pp");
  
  // Set the asymmetry string based on the selected asymmetry bin
  if(fAsymmetryBin >= 0 && fAsymmetryBin < fBaseHistograms->GetNAsymmetryBins()){
    asymmetryString = Form("%.2f < %s < %.2f", fBaseHistograms->GetCard()->GetLowBinBorderAsymmetry(fAsymmetryBin), fBaseHistograms->GetCard()->GetAsymmetryBinType(), fBaseHistograms->GetCard()->GetHighBinBorderAsymmetry(fAsymmetryBin));
    compactAsymmetryString = Form("_A=%.2f-%.2f", fBaseHistograms->GetCard()->GetLowBinBorderAsymmetry(fAsymmetryBin), fBaseHistograms->GetCard()->GetHighBinBorderAsymmetry(fAsymmetryBin));
    compactAsymmetryString.ReplaceAll(".","v");
  }
  
  // Helper histograms for summing over pT
  TH1D *mainSum;
  TH1D *comparisonSum[knMaxRatios];
  TH1D *comparisonSumRatio[knMaxRatios];
  
  // Canvas for the big closure plot
  double canvasWidth = isPp ? 250 : 1000;
  TCanvas *closureCanvas = new TCanvas(Form("jetShapeClosureCanvas%s",compactAsymmetryString.Data()), Form("jetShapeClosureCanvas%s",compactAsymmetryString.Data()), canvasWidth, 1800);
  double nCentralityBins = fLastDrawnCentralityBin-fFirstDrawnCentralityBin+1;
  double nTrackPtBins = fLastDrawnTrackPtBin-fFirstDrawnTrackPtBin+1;
  closureCanvas->Divide(nCentralityBins,nTrackPtBins);
  TH1D *closureHistogram;
  
  // Loop over jet-track correlation categories
  for(int iJetTrack = 0; iJetTrack < DijetHistogramManager::knJetTrackCorrelations; iJetTrack++){
    if(!fDrawJetTrackCorrelations[iJetTrack]) continue;  // Only draw the selected categories
    
    // Loop over centrality
    for(int iCentrality = fFirstDrawnCentralityBin; iCentrality <= fLastDrawnCentralityBin; iCentrality++){
      
      centralityString = Form("Cent: %.0f-%.0f%%",fBaseHistograms->GetCentralityBinBorder(iCentrality),fBaseHistograms->GetCentralityBinBorder(iCentrality+1));
      compactCentralityString = Form("_C=%.0f-%.0f",fBaseHistograms->GetCentralityBinBorder(iCentrality),fBaseHistograms->GetCentralityBinBorder(iCentrality+1));
      if(isPp){
        centralityString = "pp";
        compactCentralityString = "_pp";
      }
      
      // Loop over track pT bins
      for(int iTrackPt = fFirstDrawnTrackPtBin; iTrackPt <= fLastDrawnTrackPtBin; iTrackPt++){
        
        // Set the correct track pT bins
        trackPtString = Form("Track pT: %.1f-%.1f GeV",fBaseHistograms->GetTrackPtBinBorder(iTrackPt),fBaseHistograms->GetTrackPtBinBorder(iTrackPt+1));
        compactTrackPtString = Form("_pT=%.1f-%.1f",fBaseHistograms->GetTrackPtBinBorder(iTrackPt),fBaseHistograms->GetTrackPtBinBorder(iTrackPt+1));
        compactTrackPtString.ReplaceAll(".","v");
        
        legendX1 = 0.45; legendY1 = 0.58; legendX2 = 0.77; legendY2 = 0.83;
        
        // Prepare the jet shape histograms to be drawn
        PrepareRatio("JetShape", 1, DijetHistogramManager::kJetShape, iJetTrack, fAsymmetryBin, iCentrality, iTrackPt);
        closureHistogram = (TH1D*) fRatioHistogram[0]->Clone(Form("closureHistogram%d%d%d",iJetTrack,iCentrality,iTrackPt));
        
        // Calculate the pT sum
        if(iTrackPt == fFirstDrawnTrackPtBin){
          mainSum = (TH1D*) fMainHistogram->Clone("mainClone");
          for(int iAdditional = 0; iAdditional < fnAddedHistograms; iAdditional++){
            comparisonSum[iAdditional] = (TH1D*) fComparisonHistogram[iAdditional]->Clone(Form("comparisonClone%d",iAdditional));
          }
        } else {
          mainSum->Add(fMainHistogram);
          for(int iAdditional = 0; iAdditional < fnAddedHistograms; iAdditional++){
            comparisonSum[iAdditional]->Add(fComparisonHistogram[iAdditional]);
          }
        }
        
        // Draw the track phi distributions to the upper panel of a split canvas plot
        sprintf(namerX,"#DeltaR");
        fMainHistogram->GetXaxis()->SetRangeUser(0,1);
        DrawToUpperPad(namerX, "P(#DeltaR)", fLogJetShape);
        
        // Add a legend to the plot
        legend = new TLegend(legendX1,legendY1,legendX2,legendY2);
        SetupLegend(legend,centralityString,trackPtString,asymmetryString);
        legend->Draw();
        
        // Draw the ratios to the lower portion of the split canvas
        fDrawer->SetGridY(true);
        fRatioHistogram[0]->GetXaxis()->SetRangeUser(0,1);
        DrawToLowerPad(namerX,fRatioLabel.Data(),fRatioZoomMin,fRatioZoomMax);
        fDrawer->SetGridY(false);
        
        // Save the figure to a file
        sprintf(namerX,"%sJetShapeComparison",fBaseHistograms->GetJetTrackHistogramName(iJetTrack));
        SaveFigure(namerX,compactCentralityString,compactTrackPtString);
        
        // Draw the ratio also to the closure canvas
        
        // Find the correct pad inside the canvas
        closureCanvas->cd(nCentralityBins-1-iCentrality+nCentralityBins*iTrackPt+1);
        gPad->SetTopMargin(0.1);
        gPad->SetBottomMargin(0.2);
        
        // Style the histogram to nicely fit the big canvas
        SetHistogramStyle(closureHistogram,1,"#DeltaR");
        closureHistogram->SetMarkerStyle(kFullCircle);
        closureHistogram->SetMarkerSize(0.5);
        closureHistogram->SetMarkerColor(kBlack);
        
        // Draw the ratio histogram to canvas
        closureHistogram->Draw("p");
        
        // Draw a legend to the canvas
        legend = new TLegend(0.2,0.9,0.8,0.98);
        legend->SetFillStyle(0);legend->SetBorderSize(0);legend->SetTextSize(0.08);legend->SetTextFont(62);
        legend->SetHeader(Form("%s - %s",centralityString.Data(),trackPtString.Data()));
        legend->Draw();
        
      } // Track pT loop
      
      // Set the correct track pT bins
      trackPtString = Form("Track pT: %.1f-%.1f GeV",fBaseHistograms->GetTrackPtBinBorder(fFirstDrawnTrackPtBin),fBaseHistograms->GetTrackPtBinBorder(fLastDrawnTrackPtBin+1));
      compactTrackPtString = Form("_pT=%.1f-%.1f",fBaseHistograms->GetTrackPtBinBorder(fFirstDrawnTrackPtBin),fBaseHistograms->GetTrackPtBinBorder(fLastDrawnTrackPtBin+1));
      compactTrackPtString.ReplaceAll(".","v");
      
      // Calculate the pT summed ratios and set the pointers to class histograms
      mainSum->GetXaxis()->SetRangeUser(0,1);
      fMainHistogram = mainSum;
      for(int iAdditional = 0; iAdditional < fnAddedHistograms; iAdditional++){
        comparisonSumRatio[iAdditional] = (TH1D*) mainSum->Clone(Form("sumClone%d",iAdditional));
        comparisonSumRatio[iAdditional]->Divide(comparisonSum[iAdditional]);
        fComparisonHistogram[iAdditional] = comparisonSum[iAdditional];
        fRatioHistogram[iAdditional] = comparisonSumRatio[iAdditional];
      }

      // Draw the pT summed jet shape to the upper panel of a split canvas plot
      sprintf(namerX,"#DeltaR");
      fMainHistogram->GetXaxis()->SetRangeUser(0,1);
      DrawToUpperPad(namerX, "P(#DeltaR)", fLogJetShape);
      
      // Add a legend to the plot
      legend = new TLegend(legendX1,legendY1,legendX2,legendY2);
      SetupLegend(legend,centralityString,trackPtString,asymmetryString);
      legend->Draw();
      
      // Draw the ratios to the lower portion of the split canvas
      fDrawer->SetGridY(true);
      DrawToLowerPad(namerX,fRatioLabel.Data(),fRatioZoomMin,fRatioZoomMax);
      fDrawer->SetGridY(false);
      
      // Save the figure to a file
      sprintf(namerX,"%sJetShapeComparison",fBaseHistograms->GetJetTrackHistogramName(iJetTrack));
      SaveFigure(namerX,compactCentralityString,compactTrackPtString);
      
      
    } // Centrality loop
  } // Jet-track correlation category loop
  
}


/*
 * Prepare histograms for ratio plots
 *
 *  Arguments:
 *   TString name = Name for the histograms to be filled in arrays
 *   int rebin = Rebinning the histograms before taking the ratio
 *   int bin1 = First bin index for the loaded histograms
 *   int bin2 = Second bin index for the loaded histograms
 *   int bin3 = Third bin index for the loaded histograms
 *   int bin4 = Fourth bin index for the loaded histograms
 *   int bin5 = Fifth bin index for the loaded histograms
 *   int bin6 = Sixth bin index for the loaded histograms
 *   double minRange = Minimum range shown for the histograms
 *   double maxRange = Maximum range shown for the histograms
 */
void DijetComparingDrawer::PrepareRatio(TString name, int rebin, int bin1, int bin2, int bin3, int bin4, int bin5, int bin6, double minRange, double maxRange){
  
  // Helper variable
  char namer[100];
  
  // Read the histograms, scale them to one and take the ratio
  fMainHistogram = (TH1D*)fBaseHistograms->GetOneDimensionalHistogram(name,bin1,bin2,bin3,bin4,bin5,bin6)->Clone();
  if(rebin > 1) {fMainHistogram->Rebin(rebin); fMainHistogram->Scale(1.0/rebin);}
  if(maxRange > minRange) fMainHistogram->GetXaxis()->SetRangeUser(minRange,maxRange);
  if(fApplyScaling) fMainHistogram->Scale(1.0/fScalingFactors[0]);
  for(int iAdditional = 0; iAdditional < fnAddedHistograms; iAdditional++){
    fComparisonHistogram[iAdditional] = (TH1D*)fAddedHistograms[iAdditional]->GetOneDimensionalHistogram(name,bin1,bin2,bin3,bin4,bin5,bin6)->Clone();
    if(rebin > 1) {fComparisonHistogram[iAdditional]->Rebin(rebin); fComparisonHistogram[iAdditional]->Scale(1.0/rebin);}
    if(maxRange > minRange) fComparisonHistogram[iAdditional]->GetXaxis()->SetRangeUser(minRange,maxRange);
    if(fApplyScaling) fComparisonHistogram[iAdditional]->Scale(1.0/fScalingFactors[1+iAdditional]);
    sprintf(namer,"%sRatio%d",fMainHistogram->GetName(),iAdditional);
    fRatioHistogram[iAdditional] = (TH1D*)fMainHistogram->Clone(namer);
    if(fUseDifferenceInsteadOfRatio){
      fRatioHistogram[iAdditional]->Add(fComparisonHistogram[iAdditional],-1);
    } else {
      fRatioHistogram[iAdditional]->Divide(fComparisonHistogram[iAdditional]);
    }
  }
}

/*
 * Find the scaling factors for drawn distributions from the jet pT distribution
 *
 *  int iJetCategory = Index for the catogery of jets we are looking at
 *  int iCentrality = Index for the centrality bin of the jet
 *  int iAsymmetry = Index for the asymmetry bin of the jet
 */
void DijetComparingDrawer::FindScalingFactors(int iJetCategory, int iCentrality, int iAsymmetry){
  
  // Helper variable for reading the normalization scales
  TH1D *scaleReader;
  scaleReader = (TH1D*)fBaseHistograms->GetOneDimensionalHistogram("jetPt",iJetCategory,iCentrality,iAsymmetry)->Clone();
  fScalingFactors[0] = scaleReader->Integral("width");
  //fScalingFactors[0] = scaleReader->Integral(scaleReader->FindBin(200),scaleReader->GetNbinsX(),"width"); // XXXXX
  for(int iAdditional = 0; iAdditional < fnAddedHistograms; iAdditional++){
    scaleReader = (TH1D*)fAddedHistograms[iAdditional]->GetOneDimensionalHistogram("jetPt",iJetCategory,iCentrality,iAsymmetry)->Clone();
    fScalingFactors[1+iAdditional] = scaleReader->Integral("width");
    //fScalingFactors[1+iAdditional] = scaleReader->Integral(scaleReader->FindBin(200),scaleReader->GetNbinsX(),"width"); // XXXXX
  }
  //cout << "For iCentrality = " << iCentrality << " Reco / Gen = " << fScalingFactors[0]/fScalingFactors[1] << endl;
  
}

/*
 * Draw to upper pad the histograms that are most recently prepared for drawing
 *
 *  Arguments:
 *   const char* xTitle = Title given to the x-axis
 *   const char* yTitle = Title given to the y-axis
 *   bool logAxis = True: logarithmic y-axis, false = linear y-axis
 */
void DijetComparingDrawer::DrawToUpperPad(const char* xTitle, const char* yTitle, bool logAxis){

  // Define some nice colors for histograms
  fMainHistogram->SetLineColor(kBlack);

  // Create a split canvas and draw the histograms to the upped part of the canvas
  fDrawer->SetDefaultAppearanceSplitCanvas();
  fDrawer->CreateSplitCanvas();
  fDrawer->SetLogY(logAxis);
  fDrawer->DrawHistogramToUpperPad(fMainHistogram,xTitle,yTitle," ");
  for(int iAdditional = 0; iAdditional < fnAddedHistograms; iAdditional++){
    fComparisonHistogram[iAdditional]->SetLineColor(fColors[iAdditional]);
    fComparisonHistogram[iAdditional]->Draw("same");
  }
  
  // Reset back to linear for ratio
  fDrawer->SetLogY(false);
}

/*
 * Draw to the lower pad the ratio histograms that are most recently prepared
 *
 *  Arguments:
 *   const char* xTitle = Title given to the x-axis
 *   const char* yTitle = Title given to the y-axis
 *   const double zoomMin = Minimum value of the y-axis in the ratio plot
 *   const double zoomMax = Maximum value of the y-axis in the ratio plot
 */
void DijetComparingDrawer::DrawToLowerPad(const char* xTitle, const char* yTitle, const double zoomMin, const double zoomMax){
  if(fnAddedHistograms > 0){
    fRatioHistogram[0]->SetLineColor(fColors[0]);
    fRatioHistogram[0]->GetYaxis()->SetRangeUser(zoomMin,zoomMax);
    fDrawer->DrawHistogramToLowerPad(fRatioHistogram[0],xTitle,yTitle, " ");
  }
  for(int iAdditional = 1; iAdditional < fnAddedHistograms; iAdditional++){
    fRatioHistogram[iAdditional]->SetLineColor(fColors[iAdditional]);
    fRatioHistogram[iAdditional]->Draw("same");
  }
}

/*
 * Zoom the y-axis scale to the specified region of the distribution
 *
 *  const double maxZoomValue = Maximum value in the distribution used to calculate the zoom
 *  const int nZoomBins = Number of bins around the maxZoomValue used to calculate the scale
 *  const double scaleFactor = How many times the variance is shown around the average value of the bin contents
 *  const bool bothSides = True: Use bins above -maxZoomValue together with bins below maxZoomValue, False = Use only bins below maxZoomValue
 *  const bool asymmetricZoom = Zoom more to the upper side than the lower. Good for zooming to tail below peak.
 */
void DijetComparingDrawer::ZoomToRegion(const double maxZoomValue, const int nZoomBins, const double scaleFactor, const bool bothSides, const bool asymmetricZoom){
  
  // Create array for average values in the main histogram and added histograms
  double averageValues[fnAddedHistograms+1];
  double differences[fnAddedHistograms+1];
  
  // Fill the array with average values from the main histogram and added histograms
  std::tie(averageValues[0],differences[0]) = GetHistogramAverageAndDifferenceInRegion(fMainHistogram,maxZoomValue,nZoomBins,bothSides);
  for(int iAdditional = 0; iAdditional < fnAddedHistograms; iAdditional++){
    std::tie(averageValues[iAdditional+1],differences[iAdditional+1]) = GetHistogramAverageAndDifferenceInRegion(fComparisonHistogram[iAdditional],maxZoomValue,nZoomBins,bothSides);
  }
  
  // Find the maximum and minimum value from the array
  double maxAverage = -1e12;
  double minAverage = 1e12;
  double maxDifference = -1;
  for(int iBin = 0; iBin < fnAddedHistograms+1; iBin++){
    if(averageValues[iBin] < minAverage) minAverage = averageValues[iBin];
    if(averageValues[iBin] > maxAverage) maxAverage = averageValues[iBin];
    if(differences[iBin] > maxDifference) maxDifference = differences[iBin];
  }
  
  // Calculate the maximum and minimum scale for the histogram
  double asymmetryUp = asymmetricZoom ? 1.5 : 1;
  double asymmetryDown = asymmetricZoom ? 0.5 : 1;
  double maxScale = maxAverage + maxDifference*scaleFactor*asymmetryUp;
  double minScale = minAverage - maxDifference*scaleFactor*asymmetryDown;
  if(minScale < 0 || maxScale/minScale > 4) minScale = 0;
  
  // Set the new y-axis ranges for the histogram according to obtained values
  fMainHistogram->GetYaxis()->SetRangeUser(minScale,maxScale);
  for(int iAdditional = 0; iAdditional < fnAddedHistograms; iAdditional++){
    fComparisonHistogram[iAdditional]->GetYaxis()->SetRangeUser(minScale,maxScale);
  }
  
}

/*
 * Get the average and maximum difference in bin values from a histogram in the specific area
 *
 *  const double maxZoomValue = Maximum value in the distribution used to calculate the zoom
 *  const int nZoomBins = Number of bins around the maxZoomValue used to calculate the scale
 *  const double scaleFactor = How much around the average scale in the region is shown as a fraction of the average scale
 *  const bool bothSides = True: Use bins above -maxZoomValue together with bins below maxZoomValue, False = Use only bins below maxZoomValue
 *
 *  return: Average and difference of the bin contents in the given region
 */
std::tuple<double,double> DijetComparingDrawer::GetHistogramAverageAndDifferenceInRegion(TH1D *histogram, const double maxZoomValue, const int nZoomBins, const bool bothSides){
  
  // Calculate the sum of bin contents from the positive side
  double binSum = 0;
  int nBinsSummedOver = 0;
  double maxBinValue = -1e12;
  double minBinValue = 1e12;
  double binContent = 0;
  int maxBin = histogram->FindBin(maxZoomValue-1e-4);
  for(int iBin = maxBin-nZoomBins+1; iBin <= maxBin; iBin++){
    binContent = histogram->GetBinContent(iBin);
    binSum += binContent;
    if(binContent < minBinValue) minBinValue = binContent;
    if(binContent > maxBinValue) maxBinValue = binContent;
    nBinsSummedOver++;
  }
  
  // If required, add the bin content also from the negative side
  if(bothSides){
    maxBin = histogram->FindBin(-maxZoomValue+1e-4);
    for(int iBin = maxBin; iBin < maxBin+nZoomBins; iBin++){
      binContent = histogram->GetBinContent(iBin);
      binSum += binContent;
      if(binContent < minBinValue) minBinValue = binContent;
      if(binContent > maxBinValue) maxBinValue = binContent;
      nBinsSummedOver++;
    }
  }
  
  // Calculate the average and max difference of the bin contents
  double binAverage = binSum/nBinsSummedOver;
  double binDifference = maxBinValue-minBinValue;
  
  // Return the average value and the difference in bin values of the considered region of the histogram
  return std::make_tuple(binAverage,binDifference);
  
}

/*
 * Common legend style setup for figures
 *
 *  TLegend *legend = Pointer to legend that needs setup
 *  TString centralityString = Collision centrality
 *  TString trackString = Track pT information
 *  TString asymmetryString = Asymmetry information
 */
void DijetComparingDrawer::SetupLegend(TLegend *legend, TString centralityString, TString trackString, TString asymmetryString){
  legend->SetFillStyle(0);legend->SetBorderSize(0);legend->SetTextSize(0.05);legend->SetTextFont(62);
  if(fBaseHistograms->GetSystem().Contains("PbPb")) legend->AddEntry((TObject*) 0,centralityString.Data(),"");
  if(trackString != "") legend->AddEntry((TObject*) 0,trackString.Data(),"");
  if(asymmetryString != "") legend->AddEntry((TObject*) 0,asymmetryString.Data(),"");
  legend->AddEntry(fMainHistogram,/*fBaseHistograms->GetSystem() + " " +*/ fLegendComment[0],"l");
  for(int iAdditional = 0; iAdditional < fnAddedHistograms; iAdditional++){
    legend->AddEntry(fComparisonHistogram[iAdditional],/*fAddedHistograms[iAdditional]->GetSystem() + " " +*/ fLegendComment[iAdditional+1],"l");
  }
}

/*
 * Save the figure in current canvas to a file
 *
 *  TString figureName = Name for the saved figures
 *  TString centralityString = Information about collision centrality
 *  TString trackPtString = Information about track pT
 *  TString correlationTypeString = Information about correlation type (same/mixed event)
 *  TString deltaPhiString = Information about deltaPhi binning
 */
void DijetComparingDrawer::SaveFigure(TString figureName, TString centralityString, TString trackPtString, TString correlationTypeString, TString deltaPhiString){
  
  // Only save the figures if flag is set
  if(!fSaveFigures) return;
  
  // Write the figure to a file
  TString figName = Form("figures/%s%s",figureName.Data(),fFigureComment);
  if(fBaseHistograms->GetSystem().Contains("PbPb")) figName.Append(centralityString);
  figName.Append(trackPtString);
  figName.Append(correlationTypeString);
  figName.Append(deltaPhiString);
  gPad->GetCanvas()->SaveAs(Form("%s.%s",figName.Data(),fFigureFormat));
  
}

/*
 * Set a nice drawing style for histograms drawn to big pad
 *
 *  TH1 * histogram = Histogram needing a nice style
 *  double rangeX = Maximum drawing range for x-axis.
 *  const char* xTitle = Title for the x-axis
 */
void DijetComparingDrawer::SetHistogramStyle(TH1 *histogram, double rangeX, const char* xTitle){
  histogram->GetXaxis()->SetRangeUser(0,rangeX);
  histogram->GetYaxis()->SetRangeUser(fRatioZoomMin,fRatioZoomMax);
  histogram->SetStats(kFALSE);
  histogram->GetXaxis()->SetTitle(xTitle);
  histogram->SetLabelSize(0.09,"xy");
  histogram->SetTitleSize(0.09,"x");
}

// Setter for drawing dijet histograms
void DijetComparingDrawer::SetDrawDijetHistograms(const bool drawOrNot){
  fDrawDijets = drawOrNot;
}

// Setter for drawing leading jet histograms
void DijetComparingDrawer::SetDrawLeadingJetHistograms(const bool drawOrNot){
  fDrawSingleJets[DijetHistogramManager::kLeadingJet] = drawOrNot;
}

// Setter for drawing subleading jet histograms
void DijetComparingDrawer::SetDrawSubleadingJetHistograms(const bool drawOrNot){
  fDrawSingleJets[DijetHistogramManager::kSubleadingJet] = drawOrNot;
}

// Setter for drawing all jet histograms
void DijetComparingDrawer::SetDrawAnyJetHistograms(const bool drawOrNot){
  fDrawSingleJets[DijetHistogramManager::kAnyJet] = drawOrNot;
}

// Setter for drawing all leading jet histograms
void DijetComparingDrawer::SetDrawAnyLeadingJetHistograms(const bool drawOrNot){
  fDrawSingleJets[DijetHistogramManager::kAnyLeadingJet] = drawOrNot;
}

// Setter for drawing jet histograms
void DijetComparingDrawer::SetDrawAllJets(const bool drawLeading, const bool drawSubleading, const bool drawAny, const bool drawAnyLeading){
  SetDrawLeadingJetHistograms(drawLeading);
  SetDrawSubleadingJetHistograms(drawSubleading);
  SetDrawAnyJetHistograms(drawAny);
  SetDrawAnyLeadingJetHistograms(drawAnyLeading);
}

// Setter for drawing tracks
void DijetComparingDrawer::SetDrawTracks(const bool drawOrNot){
  fDrawTracks[DijetHistogramManager::kTrack] = drawOrNot;
}

// Setter for drawing uncorrected tracks
void DijetComparingDrawer::SetDrawTracksUncorrected(const bool drawOrNot){
  fDrawTracks[DijetHistogramManager::kUncorrectedTrack] = drawOrNot;
}

// Setter for drawing track histograms
void DijetComparingDrawer::SetDrawAllTracks(const bool drawTracks, const bool drawUncorrected){
  SetDrawTracks(drawTracks);
  SetDrawTracksUncorrected(drawUncorrected);
}

// Setter for drawing inclusive tracks
void DijetComparingDrawer::SetDrawInclusiveTracks(const bool drawOrNot){
  fDrawTracks[DijetHistogramManager::kInclusiveTrack] = drawOrNot;
}

// Setter for drawing uncorrected inclusive tracks
void DijetComparingDrawer::SetDrawInclusiveTracksUncorrected(const bool drawOrNot){
  fDrawTracks[DijetHistogramManager::kUncorrectedInclusiveTrack] = drawOrNot;
}

// Setter for drawing inclusive track histograms
void DijetComparingDrawer::SetDrawAllInclusiveTracks(const bool drawTracks, const bool drawUncorrected){
  SetDrawInclusiveTracks(drawTracks);
  SetDrawInclusiveTracksUncorrected(drawUncorrected);
}

// Setter for drawing leading jet-track correlations
void DijetComparingDrawer::SetDrawTrackLeadingJetCorrelations(const bool drawOrNot){
  fDrawJetTrackCorrelations[DijetHistogramManager::kTrackLeadingJet] = drawOrNot;
}

// Setter for drawing uncorrected leading jet-track correlations
void DijetComparingDrawer::SetDrawTrackLeadingJetCorrelationsUncorrected(const bool drawOrNot){
  fDrawJetTrackCorrelations[DijetHistogramManager::kUncorrectedTrackLeadingJet] = drawOrNot;
}

// Setter for drawing pT weighted leading jet-track correlations
void DijetComparingDrawer::SetDrawTrackLeadingJetCorrelationsPtWeighted(const bool drawOrNot){
  fDrawJetTrackCorrelations[DijetHistogramManager::kPtWeightedTrackLeadingJet] = drawOrNot;
}

// Setter for drawing all correlations related to tracks and leading jets
void DijetComparingDrawer::SetDrawAllTrackLeadingJetCorrelations(const bool drawLeading, const bool drawUncorrected, const bool drawPtWeighted){
  SetDrawTrackLeadingJetCorrelations(drawLeading);
  SetDrawTrackLeadingJetCorrelationsUncorrected(drawUncorrected);
  SetDrawTrackLeadingJetCorrelationsPtWeighted(drawPtWeighted);
}

// Setter for drawing subleading jet-track correlations
void DijetComparingDrawer::SetDrawTrackSubleadingJetCorrelations(const bool drawOrNot){
  fDrawJetTrackCorrelations[DijetHistogramManager::kTrackSubleadingJet] = drawOrNot;
}

// Setter for drawing uncorrected subleading jet-track correlations
void DijetComparingDrawer::SetDrawTrackSubleadingJetCorrelationsUncorrected(const bool drawOrNot){
  fDrawJetTrackCorrelations[DijetHistogramManager::kUncorrectedTrackSubleadingJet] = drawOrNot;
}

// Setter for drawing pT weighted subleading jet-track correlations
void DijetComparingDrawer::SetDrawTrackSubleadingJetCorrelationsPtWeighted(const bool drawOrNot){
  fDrawJetTrackCorrelations[DijetHistogramManager::kPtWeightedTrackSubleadingJet] = drawOrNot;
}

// Setter for drawing all correlations related to tracks and subleading jets
void DijetComparingDrawer::SetDrawAllTrackSubleadingJetCorrelations(const bool drawSubleading, const bool drawUncorrected, const bool drawPtWeighted){
  SetDrawTrackSubleadingJetCorrelations(drawSubleading);
  SetDrawTrackSubleadingJetCorrelationsUncorrected(drawUncorrected);
  SetDrawTrackSubleadingJetCorrelationsPtWeighted(drawPtWeighted);
}

// Setter for drawing inclusive jet-track correlations
void DijetComparingDrawer::SetDrawTrackInclusiveJetCorrelations(const bool drawOrNot){
  fDrawJetTrackCorrelations[DijetHistogramManager::kTrackInclusiveJet] = drawOrNot;
}

// Setter for drawing pT weighted inclusive jet-track correlations
void DijetComparingDrawer::SetDrawTrackInclusiveJetCorrelationsPtWeighted(const bool drawOrNot){
  fDrawJetTrackCorrelations[DijetHistogramManager::kPtWeightedTrackInclusiveJet] = drawOrNot;
}

// Setter for drawing all correlations related to tracks and inclusive jets
void DijetComparingDrawer::SetDrawAllTrackInclusiveJetCorrelations(const bool drawInclusive, const bool drawPtWeighted){
  SetDrawTrackInclusiveJetCorrelations(drawInclusive);
  SetDrawTrackInclusiveJetCorrelationsPtWeighted(drawPtWeighted);
}

// Setter for drawing jet-track deltaPhi correlations
void DijetComparingDrawer::SetDrawJetTrackDeltaPhi(const bool drawOrNot){
  fDrawJetTrackDeltaPhi = drawOrNot;
}

// Setter for drawing jet-track deltaEta correlations
void DijetComparingDrawer::SetDrawJetTrackDeltaEta(const bool drawOrNot){
  fDrawJetTrackDeltaEta = drawOrNot;
}

// Setter for drawing jet-track deltaEta-deltaPhi correlations
void DijetComparingDrawer::SetDrawJetTrackDeltaEtaDeltaPhi(const bool drawOrNot){
  fDrawJetTrackDeltaEtaDeltaPhi = drawOrNot;
}

// Setter for drawing all the jet-track deltaEta/Phi correlations
void DijetComparingDrawer::SetDrawJetTrackDeltas(const bool deltaPhi, const bool deltaEta, const bool deltaEtaDeltaPhi){
  SetDrawJetTrackDeltaPhi(deltaPhi);
  SetDrawJetTrackDeltaEta(deltaEta);
  SetDrawJetTrackDeltaEtaDeltaPhi(deltaEtaDeltaPhi);
}


// Setter for drawing jet shapes
void DijetComparingDrawer::SetDrawJetShape(const bool drawOrNot){
  fDrawJetShape[DijetHistogramManager::kJetShape] = drawOrNot;
}

// Setter for drawing jet shape counts
void DijetComparingDrawer::SetDrawJetShapeCounts(const bool drawOrNot){
  fDrawJetShape[DijetHistogramManager::kJetShapeBinCount] = drawOrNot;
}

// Setter for drawing all different jet shape histograms
void DijetComparingDrawer::SetDrawAllJetShapes(const bool jetShape, const bool counts){
  SetDrawJetShape(jetShape);
  SetDrawJetShapeCounts(counts);
}

// Setter for drawing bin mapping between Rbins and deltaEta-deltaPhi bins
void DijetComparingDrawer::SetDrawJetShapeBinMap(const bool drawOrNot){
  fDrawCorrelationType[DijetHistogramManager::kJetShapeBinMap] = drawOrNot;
}

// Setter for drawing same event correlation distributions
void DijetComparingDrawer::SetDrawSameEvent(const bool drawOrNot){
  fDrawCorrelationType[DijetHistogramManager::kSameEvent] = drawOrNot;
}

// Setter for drawing mixed event correlation distributions
void DijetComparingDrawer::SetDrawMixedEvent(const bool drawOrNot){
  fDrawCorrelationType[DijetHistogramManager::kMixedEvent] = drawOrNot;
}

// Setter for drawing normalized mixed event correlation distributions
void DijetComparingDrawer::SetDrawNormalizedMixedEvent(const bool drawOrNot){
  fDrawCorrelationType[DijetHistogramManager::kMixedEventNormalized] = drawOrNot;
}

// Setter for drawing corrected correlation distributions
void DijetComparingDrawer::SetDrawCorrectedCorrelations(const bool drawOrNot){
  fDrawCorrelationType[DijetHistogramManager::kCorrected] = drawOrNot;
}

// Setter for drawing different correlation types
void DijetComparingDrawer::SetDrawCorrelationTypes(const bool sameEvent, const bool mixedEvent, const bool normalizedMixedEvent, const bool corrected){
  SetDrawSameEvent(sameEvent);
  SetDrawMixedEvent(mixedEvent);
  SetDrawNormalizedMixedEvent(normalizedMixedEvent);
  SetDrawCorrectedCorrelations(corrected);
}

// Setter for drawing background subtracted jet-track correlation histograms
void DijetComparingDrawer::SetDrawBackgroundSubtracted(const bool drawOrNot){
  fDrawCorrelationType[DijetHistogramManager::kBackgroundSubtracted] = drawOrNot;
}

// Setter for drawing the generated background distributions
void DijetComparingDrawer::SetDrawBackground(const bool drawOrNot){
  fDrawCorrelationType[DijetHistogramManager::kBackground] = drawOrNot;
}

// Setter for drawing the event mixing check
void DijetComparingDrawer::SetDrawEventMixingCheck(const bool drawOrNot, const bool zoom, const int distributionType){
  fDrawEventMixingCheck = drawOrNot;
  fEventMixingZoom = zoom;
  fEventMixingDistribution = distributionType;
}

// Setter for saving the figures to a file
void DijetComparingDrawer::SetSaveFigures(const bool saveOrNot, const char *format, const char *comment){
  fSaveFigures = saveOrNot;
  fFigureFormat = format;
  fFigureComment = comment;
}

// Set if we should scale the histograms with their integral before comparing them
void DijetComparingDrawer::SetApplyScaling(const bool applyScaling){
  fApplyScaling = applyScaling;
}

// Setter for logarithmic pT axis
void DijetComparingDrawer::SetLogPt(const bool isLog){
  fLogPt = isLog;
}

// Setter for logarithmic z axis for correlation plots
void DijetComparingDrawer::SetLogCorrelation(const bool isLog){
  fLogCorrelation = isLog;
}

// Setter for logarithmic jet shape drawing
void DijetComparingDrawer::SetLogJetShape(const bool isLog){
  fLogJetShape = isLog;
}

// Setter for logarithmix axes
void DijetComparingDrawer::SetLogAxes(const bool pt, const bool correlation, const bool jetShape){
  SetLogPt(pt);
  SetLogCorrelation(correlation);
  SetLogJetShape(jetShape);
}

// Setter for plotting difference instead of ratio to lower pad
void DijetComparingDrawer::SetUseDifferenceInRatioPlot(const bool useDifference){
  fUseDifferenceInsteadOfRatio = useDifference;
}

// Setter for minimum value of y-axis in ratio plots
void DijetComparingDrawer::SetRatioZoomMin(const double minValue){
  fRatioZoomMin = minValue;
}

// Setter for maximum value of y-axis in ratio plots
void DijetComparingDrawer::SetRatioZoomMax(const double maxValue){
  fRatioZoomMax = maxValue;
}

// Setter for y-axis values in ratio plots
void DijetComparingDrawer::SetRatioZoom(const double minValue, const double maxValue){
  SetRatioZoomMin(minValue);
  SetRatioZoomMax(maxValue);
}

// Setter for the y-axis label in ratio plots
void DijetComparingDrawer::SetRatioLabel(TString label){
  fRatioLabel = label;
}

// Setter for color palette
void DijetComparingDrawer::SetColorPalette(const int color){
  fColorPalette = color;
  gStyle->SetPalette(color);
}

// Setter for 2D drawing style
void DijetComparingDrawer::SetDrawingStyle2D(const char* style){
  fStyle2D = style;
}

// Setter for 3D drawing style
void DijetComparingDrawer::SetDrawingStyle3D(const char* style){
  fStyle3D = style;
}

// Setter for 2D drawing style
void DijetComparingDrawer::SetDrawingStyles(const int color, const char* style2D, const char* style3D){
  SetColorPalette(color);
  SetDrawingStyle2D(style2D);
  SetDrawingStyle3D(style3D);
}

// Setter for drawn centrality bins
void DijetComparingDrawer::SetCentralityBinRange(const int first, const int last){
  fFirstDrawnCentralityBin = first;
  fLastDrawnCentralityBin = last;
  
  // Sanity check for drawn centrality bins
  BinSanityCheck(fBaseHistograms->GetNCentralityBins(),fFirstDrawnCentralityBin,fLastDrawnCentralityBin);
}

// Setter for drawn track pT bins
void DijetComparingDrawer::SetTrackPtBinRange(const int first, const int last){
  fFirstDrawnTrackPtBin = first;
  fLastDrawnTrackPtBin = last;
  
  // Sanity check for drawn track pT bins
  BinSanityCheck(fBaseHistograms->GetNTrackPtBins(),fFirstDrawnTrackPtBin,fLastDrawnTrackPtBin);
}

// Setter for the selected asymmetry bin
void DijetComparingDrawer::SetAsymmetryBin(const int asymmetry){
  if(asymmetry >= 0 && asymmetry < DijetHistogramManager::kMaxAsymmetryBins){
    fAsymmetryBin = asymmetry;
  } else {
    fAsymmetryBin = DijetHistogramManager::kMaxAsymmetryBins;
  }
}

// Sanity check for set bins
void DijetComparingDrawer::BinSanityCheck(const int nBins, int first, int last){
  if(first < 0) first = 0;
  if(last < first) last = first;
  if(last > nBins-1) last = nBins-1;
}
