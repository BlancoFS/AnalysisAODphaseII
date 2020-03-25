#include "MuonAnalyzer.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/MessageService/interface/MessageServicePresence.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TH1I.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TLorentzVector.h"
#include "TMath.h"



using namespace edm;
using namespace reco;
using namespace std;



ExampleMuonAnalyzer2::ExampleMuonAnalyzer2(const ParameterSet& pset)
{

  muonToken = consumes<reco::MuonCollection>(pset.getParameter<InputTag>("MuonCollection"));
  genToken = consumes<reco::GenParticleCollection>(pset.getParameter<InputTag>("genCollection"));
  vtxToken = consumes<reco::VertexCollection>(pset.getParameter<InputTag> ("vertices"));
  beamSpotToken = consumes<reco::BeamSpot>(pset.getParameter<InputTag> ("beamSpot")); 
  
}


ExampleMuonAnalyzer2::~ExampleMuonAnalyzer2() {}



void ExampleMuonAnalyzer2::beginJob() 
{
  cout << "\n [ExampleMuonAnalyzer::beginJob]\n" << endl;
  
  edm::Service<TFileService> fileService;

}


void ExampleMuonAnalyzer2::endJob() {}


void ExampleMuonAnalyzer2::analyze(const Event& event, const EventSetup& eventSetup)
{
  
  
  // BeamSpot
  edm::Handle<reco::BeamSpot> beamSpot;
  event.getByToken(beamSpotToken, beamSpot);
  reco::BeamSpot bs;

  if (beamSpot.isValid()) bs = *beamSpot;    


  // Vertex collection
  edm::Handle<reco::VertexCollection> vertices;
  event.getByToken(vtxToken, vertices);


  // Look for the PrimaryVertex and use the BeamSpot if you can't find it. Code snippet taken from
  // https://github.com/cms-sw/cmssw/blob/master/DQMOffline/Muon/src/EfficiencyAnalyzer.cc#L186-L218
  //----------------------------------------------------------------------------
  reco::Vertex::Point posVtx;
  reco::Vertex::Error errVtx;

  UInt_t theIndexOfThePrimaryVertex = 999.;

  if (vertices.isValid()) {
    for (unsigned int ind=0; ind<vertices->size(); ++ind) {
	    
	    double vertex_chi2 = (*vertices)[ind].normalizedChi2();
	    double vertex_ndof = (*vertices)[ind].ndof();
	    double vertex_Z = (*vertices)[ind].z();
	    
      if ((*vertices)[ind].isValid() && !((*vertices)[ind].isFake()) && vertex_ndof > 4 && vertex_chi2 < 999 && fabs(vertex_Z) < 24.0) {
	      theIndexOfThePrimaryVertex = ind;
	    break;
      }
    }
  }
  
  if (theIndexOfThePrimaryVertex < 300) {
    posVtx = ((*vertices)[theIndexOfThePrimaryVertex]).position();
    errVtx = ((*vertices)[theIndexOfThePrimaryVertex]).error();
  }
  else if (beamSpot.isValid()) {
    LogInfo("ExampleMuonAnalyzer") << "reco::PrimaryVertex not found, use BeamSpot position instead\n";
    
    posVtx      = bs.position();
    errVtx(0,0) = bs.BeamWidthX();
    errVtx(1,1) = bs.BeamWidthY();
    errVtx(2,2) = bs.sigmaZ();
  }
    
  const reco::Vertex thePrimaryVertex(posVtx,errVtx);
  
  
  
  //look over reconstructed mouns.
  Handle<reco::MuonCollection> muons;
  event.getByToken(muonToken, muons);
  
  
  //look over gen particles.
  Handle<reco::GenParticleCollection> genParticles;
  event.getByToken(genToken, genParticles);
  
  
  
  
  //Loop over gen paticles
  for (size_t i=0; i < genParticles->size(); i++) {
    
    
    if (abs((*genParticles)[i].pdgId()) != 13)    continue;
    if (!(*genParticles)[i].isPromptFinalState()) continue;
    if (!(*genParticles)[i].isLastCopy())         continue;
    
    
    Float_t charge = (*genParticles)[i].charge();
    Float_t eta = (*genParticles)[i].eta();
    Float_t phi = (*genParticles)[i].phi();
    Float_t pt = (*genParticles)[i].pt();
    Float_t vx = (*genParticles)[i].vx();
    Float_t vy = (*genParticles)[i].vy();
    Float_t vz = (*genParticles)[i].vz();
    
    Float_t vxy = sqrt(vx*vx + vy*vy);
    Float_t vr = sqrt(vx*vx + vy*vy + vz*vz);
    
    
    if (fabs(eta) > 2.4) continue;
    if (pt < pt_min) continue;
    
    
    Float_t dxy = vxy;
    Float_t dz  = vz;
    
    
    
    // Loop over reconstructed muons
    //--------------------------------------------------------------------------
    Float_t sta_min_deltaR   = 999;   
    Float_t trk_min_deltaR   = 999; 
    Float_t glb_min_deltaR   = 999; 
    Float_t tight_min_deltaR = 999;   
    Float_t soft_min_deltaR  = 999;   

    Float_t sta_pt_resolution   = -999;
    Float_t trk_pt_resolution   = -999;
    Float_t glb_pt_resolution   = -999;
    Float_t tight_pt_resolution = -999;
    Float_t soft_pt_resolution  = -999;
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
  } //...End gen particles loop.
    
    
  


}
DEFINE_FWK_MODULE(ExampleMuonAnalyzer2);
