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
  
  Handle<reco::MuonCollection> muons;
  event.getByToken(muonToken, muons);
  
  Handle<reco::GenParticleCollection> genParticles;
  event.getByToken(genToken, genParticles);
  
  
  for (size_t i=0; i < genParticles->size(); i++) {
    
    
    if (abs((*genParticles)[i].pdgId()) != 13)    continue;
    if (!(*genParticles)[i].isPromptFinalState()) continue;
    if (!(*genParticles)[i].isLastCopy())         continue;
  }
    
    
  


}
DEFINE_FWK_MODULE(ExampleMuonAnalyzer2);
