// C++ includes
#include <iostream>   // Input/output stream. Needed for cout.
#include <fstream>    // File stream for intup/output to/from files
#include <stdlib.h>   // Standard utility libraries
#include <assert.h>   // Standard c++ debugging tool. Terminates the program if expression given evaluates to 0.
#include <vector>     // C++ vector class

// Includes from Root
//#include <TROOT.h>    // Not sure if needed...
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>

// Own includes
#include "src/DijetAnalyzer.h"
#include "src/ConfigurationCard.h"
#include "src/DijetHistograms.h"

using namespace std;

/*
 * File list reader
 *
 *  Arguments:
 *    std::vector<TString> &fileNameVector = Vector filled with filenames found in the file
 *    TString fileNameFile = Text file containing one analysis file name in each line
 *    int debug = Level of debug messages shown
 */
void ReadFileList(std::vector<TString> &fileNameVector, TString fileNameFile, int debug)
{
  // Set up the file names file for reading
  ifstream file_stream(fileNameFile);
  std::string line;
  fileNameVector.clear();
  if( debug > 0 ) std::cout << "Open file " << fileNameFile.Data() << " to extract files to run over" << std::endl;
  
  // Open the file names file for reading
  if( file_stream.is_open() ) {
    if( debug > 0) std::cout << "Opened " << fileNameFile.Data() << " for reading" << std::endl;
    int lineNumber = 0;
    
    // Loop over the lines in the file
    while( !file_stream.eof() ) {
      getline(file_stream, line);
      if( debug > 0) std::cout << lineNumber << ": " << line << std::endl;
      TString tstring_line(line);
      
      // Put all non-empty lines to file names vector
      if( tstring_line.CompareTo("", TString::kExact) != 0 ) fileNameVector.push_back(tstring_line);
      lineNumber++;
    }
    
  // If cannot read the file, give error and end program
  } else {
    std::cout << "Error, could not open " << fileNameFile.Data() << " for reading" << std::endl;
    assert(0);
  }
}

/*
 *  Main program
 *
 *  Command line arguments:
 *  argv[1] = List of files to be analyzed, given in text file
 *  argv[2] = Debugging level. 0 = none, 1 = some, 2 = all.
 */
int main(int argc, char **argv) {
  
  //==== Read arguments =====
  //TROOT root("nanoDST","nanoDST analysis");  // I do not really know what this does.
  if ( argc<4 ) {
    cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
    cout<<"+ Usage of the macro: " << endl;
    cout<<"+  "<<argv[0]<<" [fileNameFile] [configurationCard] [outputFileName] <debugLevel>"<<endl;
    cout<<"+  fileNameFile: Text file containing the list of files used in the analysis." <<endl;
    cout<<"+  configurationCard: Card file with binning and cut information for the analysis." <<endl;
    cout<<"+  outputFileName: .root file to which the histograms are written." <<endl;
    cout<<"+  debugLevel: Amount of debug messages shown. 0 = none, 1 = some, 2 = all." <<endl;
    cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
    cout << endl << endl;
    assert(0);
  }

  // First argument is the file list and second the debug level
  TString fileNameFile = argv[1];
  const char *cardName = argv[2];
  TString outputFileName = argv[3];
  int debugLevel = 0;
  if(argc >= 5) debugLevel = atoi(argv[4]);
  
  // Read the card
  ConfigurationCard *dijetCard = new ConfigurationCard(cardName);
  if(debugLevel > 0){
    dijetCard->PrintOut();
    cout << endl;
  }
  
  // Read the file names used for the analysis to a vector
  std::vector<TString> fileNameVector;
  fileNameVector.clear();
  ReadFileList(fileNameVector,fileNameFile,debugLevel);
  
  // Variable for histograms in the analysis
  DijetHistograms *histograms;
  
  // Run the analysis over the list of files
  DijetAnalyzer *jetAnalysis = new DijetAnalyzer(fileNameVector, dijetCard);
  jetAnalysis->RunAnalysis(debugLevel);
  histograms = jetAnalysis->GetHistograms();
  
  // Write the histograms and card to file
  TFile *outputFile = new TFile(outputFileName, "RECREATE");
  histograms->Write();
  dijetCard->WriteCard(outputFile);
  outputFile->Close();
  
}
