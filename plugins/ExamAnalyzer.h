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
const double vxy_bins[nbins_vxy+1] = {0.0, 0.0072, 0.0160, 0.0260, 0.0384, 0.0526, 0.0702, 0.0916, 0.1194, 0.1576, 0.2168, 0.3292, 1.0, 2.0, 3.0, vxy_max};

const int    nbins_vz =   15;
const double vz_max   = 1000;
const double vz_bins[nbins_vz+1] = {0.0, 0.4292, 0.8582, 1.3158, 1.7888, 2.3100, 2.8726, 3.4866, 4.1736, 4.9496, 5.9360, 7.3894, 10.0, 15.0, 20.0, vz_max};

const int    nbins_vr =   15;
const double vr_max   = 1000;
const double vr_bins[nbins_vr+1] = {0.0, 0.4706, 0.8794, 1.3364, 1.8032, 2.3190, 2.8786, 3.4920, 4.1806, 4.9548, 5.9422, 7.3908, 10.0, 15.0, 20.0, vr_max};

const double deltaR_max = 0.3;



class ExampleMuonAnalyzer: public edm::EDAnalyzer {
 public:
  // Constructor
  ExampleMuonAnalyzer(const edm::ParameterSet& pset);

  // Destructor
  virtual ~ExampleMuonAnalyzer();

  // Operations
  void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);

  virtual void beginJob();
  virtual void endJob();
  
  
 protected:

 private:
  edm::EDGetTokenT<reco::Muon>                        muonToken;
  edm::EDGetTokenT<reco::GenParticle>                 genToken;
  edm::EDGetTokenT<reco::Vertex>                      vtxToken;
  edm::EDGetTokenT<reco::BeamSpot>                    beamSpotToken;
  
  
};
#endif


















