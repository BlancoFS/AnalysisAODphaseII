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



ExampleMuonAnalyzer::ExampleMuonAnalyzer(const ParameterSet& pset)
{

  muonToken = consumes<pat::MuonCollection>(pset.getParameter<InputTag>("MuonCollection"));
  genToken = consumes<reco::GenParticle>(pset.getParameter<InputTag>("GenParticles"));
  packedGen = consumes<pat::packedGenParticle>(pset.getParameter<InputTag>("packed"));
  
  
}


ExampleMuonAnalyzer::~ExampleMuonAnalyzer() {}



void ExampleMuonAnalyzer::beginJob() 
{
  cout << "\n [ExampleMuonAnalyzer::beginJob]\n" << endl;
  
  edm::Service<TFileService> fileService;

}


void ExampleMuonAnalyzer::endJob() {}


void ExampleMuonAnalyzer::analyze(const Event& event, const EventSetup& eventSetup)
{
  
  Handle<pat::MuonCollection> muons;
  event.getByToken(muonToken, muons);
  
  Hanlde<reco::GenParticle> genP;
  event.getByToken(genToken, genP);
  
  
  for (size_t i=0; i < genP->size(); i++) {
    
    
    if (abs((*genP)[i].pdgId()) != 13)    continue;
    if (!(*genP)[i].isPromptFinalState()) continue;
    if (!(*genP)[i].isLastCopy())         continue;
  }
    
    
  


}
DEFINE_FWK_MODULE(ExampleMuonAnalyzer);


  






















