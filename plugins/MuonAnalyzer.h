#ifndef MuonAnalyzer_H
#define MuonAnalyzer_H

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"


namespace edm {
  class ParameterSet;
  class Event;
  class EventSetup;
}

class TH1I;
class TH1F;
class TH2F;


const int    nbins_pt =   6;
const double pt_min   =  10;
const double pt_max   = 500;
const double pt_bins[nbins_pt+1] = {pt_min, 60, 90, 130, 170, 250, pt_max};

const int    nbins_vxy =  15;
const double vxy_max   = 300;
const double vxy_bins[nbins_vxy+1] = {0.0, 0.5, 1.0, 1.5, 2.5, 5.0, 7.5, 10.0, 15.0, 20.0, 25.0, 35.0, 50.0, 75.0, 100.0, vxy_max};

const int    nbins_vz =   15;
const double vz_max   = 1000;
const double vz_bins[nbins_vz+1] = {0.0, 0.5, 1.0, 1.5, 2.5, 5.0, 7.5, 10.0, 15.0, 20.0, 25.0, 35.0, 50.0, 75.0, 100.0, vz_max};

const int    nbins_vr =   15;
const double vr_max   = 1000;
const double vr_bins[nbins_vr+1] = {0.0, 0.5, 1.0, 1.5, 2.5, 5.0, 7.5, 10.0, 15.0, 20.0, 25.0, 35.0, 50.0, 75.0, 100.0, vz_max};

const double deltaR_max = 0.3;



class ExampleMuonAnalyzer2: public edm::EDAnalyzer {
 public:
  // Constructor
  ExampleMuonAnalyzer2(const edm::ParameterSet& pset);

  // Destructor
  virtual ~ExampleMuonAnalyzer2();

  // Operations
  void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);

  virtual void beginJob();
  virtual void endJob();
  
  
 protected:

 private:
  edm::EDGetTokenT<reco::MuonCollection>                        muonToken;        //Muon collection
  edm::EDGetTokenT<reco::GenParticleCollection>                 genToken;         //Gen Particles 
  edm::EDGetTokenT<reco::VertexCollection>                      vtxToken;         //Vertex collection
  edm::EDGetTokenT<reco::BeamSpot>                              beamSpotToken;    //Beam spot collection
  edm::EDGetTokenT<reco::TrackCollection>                       dispToken;        //displaced global muons
  edm::EDGetTokenT<reco::TrackCollection>     	      	      	dispStaToken;     //dispalced standAlone muons
  
  TH1F* vertexType;
  
  TH1F* h_dxy;
  TH1F* h_dz;
  TH1F* h_vxy;
  TH1F* h_vz;
  TH1F* h_vr;
  TH1F* h_pt;

  TH2F* hGenMuons_vxy_vz;
  TH2F* hStaMuons_vxy_vz;
  TH2F* hTrkMuons_vxy_vz;
  TH2F* hGlbMuons_vxy_vz;
  TH2F* hTightMuons_vxy_vz;
  TH2F* hSoftMuons_vxy_vz;
  TH2F* hStaMuons_noGen_vxy_vz;
  TH2F* hTrkMuons_noGen_vxy_vz;
  TH2F* hGlbMuons_noGen_vxy_vz;
  TH2F* hTightMuons_noGen_vxy_vz;
  TH2F* hSoftMuons_noGen_vxy_vz;

  TH1F* hGenMuons_vxy;
  TH1F* hStaMuons_vxy;
  TH1F* hTrkMuons_vxy;
  TH1F* hGlbMuons_vxy;
  TH1F* hTightMuons_vxy;
  TH1F* hSoftMuons_vxy;
  TH1F* hStaMuons_noGen_vxy;
  TH1F* hTrkMuons_noGen_vxy; 
  TH1F* hGlbMuons_noGen_vxy; 
  TH1F* hTightMuons_noGen_vxy; 
  TH1F* hSoftMuons_noGen_vxy; 
  TH1F* hDispGlbMuons_vxy;
  TH1F* hDispGlbMuons_noGen_vxy;
  TH1F* hDispStaMuons_vxy;
  TH1F* hDispStaMuons_noGen_vxy;

  TH1F* hGenMuons_vz;
  TH1F* hStaMuons_vz;
  TH1F* hTrkMuons_vz;
  TH1F* hGlbMuons_vz;
  TH1F* hTightMuons_vz;
  TH1F* hSoftMuons_vz;
  TH1F* hStaMuons_noGen_vz;
  TH1F* hTrkMuons_noGen_vz; 
  TH1F* hGlbMuons_noGen_vz; 
  TH1F* hTightMuons_noGen_vz; 
  TH1F* hSoftMuons_noGen_vz; 
  TH1F* hDispGlbMuons_vz;
  TH1F* hDispGlbMuons_noGen_vz;
  TH1F* hDispStaMuons_vz;
  TH1F* hDispStaMuons_noGen_vz;

  TH1F* hGenMuons_vr;
  TH1F* hStaMuons_vr;
  TH1F* hTrkMuons_vr;
  TH1F* hGlbMuons_vr;
  TH1F* hTightMuons_vr;
  TH1F* hSoftMuons_vr;
  TH1F* hStaMuons_noGen_vr;
  TH1F* hTrkMuons_noGen_vr; 
  TH1F* hGlbMuons_noGen_vr; 
  TH1F* hTightMuons_noGen_vr; 
  TH1F* hSoftMuons_noGen_vr; 
  TH1F* hDispGlbMuons_vr;
  TH1F* hDispGlbMuons_noGen_vr;
  TH1F* hDispStaMuons_vr;
  TH1F* hDispStaMuons_noGen_vr;
  
  TH1F* hGenMuons_eta;
  TH1F* hStaMuons_eta;
  TH1F* hTrkMuons_eta;
  TH1F* hGlbMuons_eta;
  TH1F* hTightMuons_eta;
  TH1F* hSoftMuons_eta;
  TH1F* hStaMuons_noGen_eta;
  TH1F* hTrkMuons_noGen_eta;
  TH1F* hGlbMuons_noGen_eta;
  TH1F* hTightMuons_noGen_eta;
  TH1F* hSoftMuons_noGen_eta;
  TH1F* hDispGlbMuons_eta;
  TH1F* hDispGlbMuons_noGen_eta;
  TH1F* hDispStaMuons_eta;
  TH1F* hDispStaMuons_noGen_eta;

  TH1F* hGenMuons_phi;
  TH1F* hStaMuons_phi;
  TH1F* hTrkMuons_phi;
  TH1F* hGlbMuons_phi;
  TH1F* hTightMuons_phi;                                                                                                           
  TH1F* hSoftMuons_phi;
  TH1F* hStaMuons_noGen_phi;    
  TH1F* hTrkMuons_noGen_phi;
  TH1F* hGlbMuons_noGen_phi;                                                                                                        
  TH1F* hTightMuons_noGen_phi;
  TH1F* hSoftMuons_noGen_phi;
  TH1F* hDispGlbMuons_phi;
  TH1F* hDispGlbMuons_noGen_phi;
  TH1F* hDispStaMuons_phi;
  TH1F* hDispStaMuons_noGen_phi;

  TH1F* hGenMuons_pt;
  TH1F* hStaMuons_pt;
  TH1F* hTrkMuons_pt;
  TH1F* hGlbMuons_pt;
  TH1F* hTightMuons_pt;                                                                                                             
  TH1F* hSoftMuons_pt;
  TH1F* hStaMuons_noGen_pt;                                                                                            
  TH1F* hTrkMuons_noGen_pt;                                                                                                         
  TH1F* hGlbMuons_noGen_pt;                                                                                                         
  TH1F* hTightMuons_noGen_pt;                                                                                                          
  TH1F* hSoftMuons_noGen_pt;
  TH1F* hDispGlbMuons_pt;
  TH1F* hDispGlbMuons_noGen_pt;
  TH1F* hDispStaMuons_pt;
  TH1F* hDispStaMuons_noGen_pt;

  TH1F* hStaMuons_dR;
  TH1F* hTrkMuons_dR;
  TH1F* hGlbMuons_dR;
  TH1F* hTightMuons_dR;                                                                                                            
  TH1F* hSoftMuons_dR;
  TH1F* hDispGlbMuons_dR;
  TH1F* hDispStaMuons_dR;

  TH2F* hStaMuons_pt_resolution  [nbins_pt];
  TH2F* hTrkMuons_pt_resolution  [nbins_pt];
  TH2F* hGlbMuons_pt_resolution  [nbins_pt];
  TH2F* hTightMuons_pt_resolution[nbins_pt];
  TH2F* hSoftMuons_pt_resolution [nbins_pt];

  TH1F* hMuPFChargeIso; 
  TH1F* hMuPFNeutralIso;
  TH1F* hMuPFPhotonIso; 
  TH1F* hMuPFPUIso;   
  TH1F* hMuPFIso;
  
  
};
#endif
