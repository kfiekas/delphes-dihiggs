//-------------------------------------------------------------------
// Clean up merged files from lxbtch
//
// execute with:
// root -l -q cleanUpMergedFiles(_infile_, _outfile_)
//
// Jay Lawhorn 11/4/13
//-------------------------------------------------------------------
#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TClonesArray.h>
#include <TVector2.h>
#include <TMath.h>
#include <TChain.h>
#include <TH1.h>
#include <TCanvas.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "Math/LorentzVector.h"

#endif

typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > LorentzVector;

Double_t calcFakeRate(TFile* infile);
Float_t deltaR( const Float_t eta1, const Float_t eta2, const Float_t phi1, const Float_t phi2 );

void newJetStudies() {

  enum { hadron=1, electron, muon };
  enum { all=0, dijet, jetmu, jetele, muele };

  // set up output variables and file
  UInt_t nEvents;
  Float_t eventWeight;

  Float_t met, metPhi;

  UInt_t eventType;
  UInt_t tauCat1=0, tauCat2=0;
  UInt_t bTag1=0, bTag2=0;

  Double_t mt2;

  LorentzVector *sRecoTau1=0, *sRecoTau2=0;
  LorentzVector *sGenJetTau1=0, *sGenJetTau2=0;
  LorentzVector *sGenTau1=0, *sGenTau2=0;

  LorentzVector *sRecoB1=0, *sRecoB2=0;
  LorentzVector *sGenJetB1=0, *sGenJetB2=0;
  LorentzVector *sGenB1=0, *sGenB2=0;

  TFile* infile = new TFile("root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/future/sethzenz/Mar20/VBF_HToTauTau_M-125_14TeV-powheg-pythia6/001.root"); //assert(infile);
  cout << calcFakeRate(infile) << endl;

  /*
  TFile* infile = new TFile("/afs/cern.ch/work/k/klawhorn/hhttbb_ntuples/b_select.root"); assert(infile);
  cout << "b  : " << calcFakeRate(infile) << endl;
  infile = new TFile("/afs/cern.ch/work/k/klawhorn/hhttbb_ntuples/bb_select.root"); assert(infile);
  cout << "bb : " << calcFakeRate(infile) << endl;
  infile = new TFile("/afs/cern.ch/work/k/klawhorn/hhttbb_ntuples/bbb_select.root"); assert(infile);
  cout << "bbb: " << calcFakeRate(infile) << endl;
  infile = new TFile("/afs/cern.ch/work/k/klawhorn/hhttbb_ntuples/bj_select.root"); assert(infile);
  cout << "bj : " << calcFakeRate(infile) << endl;
  infile = new TFile("/afs/cern.ch/work/k/klawhorn/hhttbb_ntuples/bjj_select.root"); assert(infile);
  cout << "bjj: " << calcFakeRate(infile) << endl;
  infile = new TFile("/afs/cern.ch/work/k/klawhorn/hhttbb_ntuples/ll_select.root"); assert(infile);
  cout << "ll : " << calcFakeRate(infile) << endl;
  infile = new TFile("/afs/cern.ch/work/k/klawhorn/hhttbb_ntuples/tt_select.root"); assert(infile);
  cout << "tt : " << calcFakeRate(infile) << endl;
  infile = new TFile("/afs/cern.ch/work/k/klawhorn/hhttbb_ntuples/hhttbb_select.root"); assert(infile);
  cout << "hh : " << calcFakeRate(infile) << endl;
  */
}

Double_t calcFakeRate(TFile* infile) {

  enum { hadron=1, electron, muon };
  enum { all=0, dijet, jetmu, jetele, muele };

  // set up output variables and file
  Int_t nEvents;
  Float_t eventWeight;

  Float_t met, metPhi;

  Int_t eventType;
  UInt_t tauCat1=0, tauCat2=0;
  UInt_t bTag1=0, bTag2=0;

  Double_t mt2;

  LorentzVector *sRecoTau1=0, *sRecoTau2=0;
  LorentzVector *sGenJetTau1=0, *sGenJetTau2=0;
  LorentzVector *sGenTau1=0, *sGenTau2=0;

  LorentzVector *sRecoB1=0, *sRecoB2=0;
  LorentzVector *sGenJetB1=0, *sGenJetB2=0;
  LorentzVector *sGenB1=0, *sGenB2=0;

  TTree* inTree = (TTree*) infile->Get("Events"); assert(inTree);

  inTree->SetBranchAddress("eventWeight",    &eventWeight);   // event weight from cross-section and Event->Weight
  inTree->SetBranchAddress("eventType",      &eventType);     // event type (0=signal, 1=tt, 2=zh, 3=other)
  inTree->SetBranchAddress("tauCat1",        &tauCat1);       // leading tau final state - jet, muon, electron
  inTree->SetBranchAddress("tauCat2",        &tauCat2);       // second tau final state - jet, muon, electron
  inTree->SetBranchAddress("bTag1",          &bTag1);         // leading b-jet tag from delphes
  inTree->SetBranchAddress("bTag2",          &bTag2);         // second b-jet tag from delphes
  inTree->SetBranchAddress("met",            &met);          // missing transverse energy
  inTree->SetBranchAddress("metPhi",         &metPhi);        // missing transverse energy phi
  inTree->SetBranchAddress("mt2",            &mt2);           // "stransverse mass"
  inTree->SetBranchAddress("sGenTau1",       &sGenTau1);      // 4-vector for generator leading tau
  inTree->SetBranchAddress("sGenTau2",       &sGenTau2);      // 4-vector for generator second tau
  inTree->SetBranchAddress("sGenB1",         &sGenB1);        // 4-vector for generator leading b-jet
  inTree->SetBranchAddress("sGenB2",         &sGenB2);        // 4-vector for generator second b-jet
  inTree->SetBranchAddress("sGenJetTau1",    &sGenJetTau1);   // 4-vector for generator leading tau
  inTree->SetBranchAddress("sGenJetTau2",    &sGenJetTau2);   // 4-vector for generator second tau
  inTree->SetBranchAddress("sGenJetB1",      &sGenJetB1);     // 4-vector for generator leading b-jet
  inTree->SetBranchAddress("sGenJetB2",      &sGenJetB2);     // 4-vector for generator second b-jet
  inTree->SetBranchAddress("sRecoTau1",      &sRecoTau1);     // 4-vector for reconstructed leading tau
  inTree->SetBranchAddress("sRecoTau2",      &sRecoTau2);     // 4-vector for reconstructed second tau
  inTree->SetBranchAddress("sRecoB1",        &sRecoB1);       // 4-vector for reconstructed leading b-jet
  inTree->SetBranchAddress("sRecoB2",        &sRecoB2);       // 4-vector for reconstructed second b-jet

  Float_t fakes=0;
  Float_t allHadrons=0;

  for(UInt_t iEntry=0; iEntry<inTree->GetEntries(); iEntry++) { // entry loop
    inTree->GetEntry(iEntry);

    if (sRecoTau1->Pt()!=999) { 
      if (tauCat1==hadron) {
	allHadrons+=1;
	if (sGenTau1->Pt()==999) fakes+=1;
      }

      if (sRecoTau2->Pt()!=999) { 
	if (tauCat2==hadron) {
	  allHadrons+=1;
	  if (sGenTau1->Pt()==999) fakes+=1;
	}
      }
    }
  }

  return 100*fakes/allHadrons;
    
}

Float_t deltaR( const Float_t eta1, const Float_t eta2, const Float_t phi1, const Float_t phi2 ) {

  const Float_t pi = 3.14159265358979;

  Float_t etaDiff = (eta1-eta2);
  Float_t phiDiff = fabs(phi1-phi2);
  while ( phiDiff>pi ) phiDiff = fabs(phiDiff-2.0*pi);

  Float_t deltaRSquared = etaDiff*etaDiff + phiDiff*phiDiff;

  return TMath::Sqrt(deltaRSquared);

}
