#!/bin/bash

#./processHistograms.sh "data/dijet_ppMC_GenGen_mergedSkims_Pythia6_pfJets_noJetLimit_2019-01-15.root" "data/dijet_ppMC_GenGen_mergedSkims_Pythia6_pfJets_noJetLimit_smoothedMixing_adjustedBackground_processed_2019-01-15.root" false false
#./processHistograms.sh "data/dijet_ppMC_RecoGen_mergedSkims_Pythia6_pfJets_noJetLimit_2019-01-15.root" "data/dijet_ppMC_RecoGen_mergedSkims_Pythia6_pfJets_noJetLimit_smoothedMixing_adjustedBackground_processed_2019-01-15.root" false false
#./processHistograms.sh "data/dijet_ppMC_GenReco_mergedSkims_Pythia6_pfJets_noJetLimit_2019-01-15.root" "data/dijet_ppMC_GenReco_mergedSkims_Pythia6_pfJets_noJetLimit_smoothedMixing_adjustedBackground_noCorrections_processed_2019-01-15.root" false false
./processHistograms.sh "data/dijet_ppMC_RecoReco_mergedSkims_Pythia6_pfJets_noJetLimit_2019-01-15.root" "data/dijet_ppMC_RecoReco_mergedSkims_Pythia6_pfJets_noJetLimit_smoothedMixing_adjustedBackground_noCorrections_processed_2019-01-15.root" false false
