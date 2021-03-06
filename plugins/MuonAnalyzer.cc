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
  dispToken = consumes<reco::TrackCollection>(pset.getParameter<InputTag>("dispGlbTracks"));
  dispStaToken = consumes<reco::TrackCollection>(pset.getParameter<InputTag>("dispStaTracks"));
  
}


ExampleMuonAnalyzer2::~ExampleMuonAnalyzer2() {}



void ExampleMuonAnalyzer2::beginJob() 
{

  cout << "\n [ExampleMuonAnalyzer::beginJob]\n" << endl;

  TH1::SetDefaultSumw2();
  TH2::SetDefaultSumw2();

  edm::Service<TFileService> fileService;
	
  //Vertex histogram:
  vertexType = fileService->make<TH1F>("typeOfVertex", "", 2, 0 , 1);
  //-----------------------------------------------------------     

  h_dxy = fileService->make<TH1F>("dxy", "",  100, -0.1,     0.1);
  h_dz  = fileService->make<TH1F>("dz",  "",  100, -0.1,     0.1);
  h_vxy = fileService->make<TH1F>("vxy", "", 1000,    0, vxy_max);
  h_vz  = fileService->make<TH1F>("vz",  "", 1000,    0,  vz_max);
  h_vr  = fileService->make<TH1F>("vr",  "", 1000,    0,  vr_max);
  h_pt  = fileService->make<TH1F>("pt",  "", 1000,    0,  pt_max);
	
  hNumberOfLepton = fileService->make<TH1F>("NumberOfLepton",  "",  11, 0.0, 10.0);
  hNumber2D       = fileService->make<TH2F>("NumberofLepton2D", "", 6, 0.0, 5.0, 6, 0.0, 5.0);
  hInvMass        = fileService->make<TH1F>("InvariantMass",   "",  100, 0.0, 200.0);
  hInvMassTrk     = fileService->make<TH1F>("InvariantMassTrk",   "",  100, 0.0, 200.0);
  hInvMassGlb     = fileService->make<TH1F>("InvariantMassGlb",   "",  100, 0.0, 200.0);
  hInvMassSta     = fileService->make<TH1F>("InvariantMassSta",   "",  100, 0.0, 200.0);

  hInvMassHiggs        = fileService->make<TH1F>("InvariantMassHiggs",   "",  200, 0.0, 500.0);
  hInvMassTrkHiggs     = fileService->make<TH1F>("InvariantMassHiggsTrk",   "",  200, 0.0, 500.0);
  hInvMassGlbHiggs     = fileService->make<TH1F>("InvariantMassHiggsGlb",   "",  200, 0.0, 500.0);
  hInvMassStaHiggs     = fileService->make<TH1F>("InvariantMassHiggsSta",   "",  200, 0.0, 500.0);

  hGenMuons_vxy_vz         = fileService->make<TH2F>("GenMuons_vxy_vz",         "", nbins_vxy, vxy_bins, nbins_vz, vz_bins);
  hStaMuons_vxy_vz         = fileService->make<TH2F>("StaMuons_vxy_vz",         "", nbins_vxy, vxy_bins, nbins_vz, vz_bins);
  hTrkMuons_vxy_vz         = fileService->make<TH2F>("TrkMuons_vxy_vz",         "", nbins_vxy, vxy_bins, nbins_vz, vz_bins);
  hGlbMuons_vxy_vz         = fileService->make<TH2F>("GlbMuons_vxy_vz",         "", nbins_vxy, vxy_bins, nbins_vz, vz_bins);
  hTightMuons_vxy_vz       = fileService->make<TH2F>("TightMuons_vxy_vz",       "", nbins_vxy, vxy_bins, nbins_vz, vz_bins);
  hSoftMuons_vxy_vz        = fileService->make<TH2F>("SoftMuons_vxy_vz",        "", nbins_vxy, vxy_bins, nbins_vz, vz_bins);
  hStaMuons_noGen_vxy_vz   = fileService->make<TH2F>("StaMuons_noGen_vxy_vz",   "", nbins_vxy, vxy_bins, nbins_vz, vz_bins);
  hTrkMuons_noGen_vxy_vz   = fileService->make<TH2F>("TrkMuons_noGen_vxy_vz",   "", nbins_vxy, vxy_bins, nbins_vz, vz_bins);
  hGlbMuons_noGen_vxy_vz   = fileService->make<TH2F>("GlbMuons_noGen_vxy_vz",   "", nbins_vxy, vxy_bins, nbins_vz, vz_bins);
  hTightMuons_noGen_vxy_vz = fileService->make<TH2F>("TightMuons_noGen_vxy_vz", "", nbins_vxy, vxy_bins, nbins_vz, vz_bins);
  hSoftMuons_noGen_vxy_vz  = fileService->make<TH2F>("SoftMuons_noGen_vxy_vz",  "", nbins_vxy, vxy_bins, nbins_vz, vz_bins);
	
  hGenMuons_vxy         = fileService->make<TH1F>("GenMuons_vxy",         "", nbins_vxy, vxy_bins);
  hStaMuons_vxy         = fileService->make<TH1F>("StaMuons_vxy",         "", nbins_vxy, vxy_bins);
  hTrkMuons_vxy         = fileService->make<TH1F>("TrkMuons_vxy",         "", nbins_vxy, vxy_bins);
  hGlbMuons_vxy         = fileService->make<TH1F>("GlbMuons_vxy",         "", nbins_vxy, vxy_bins);
  hTightMuons_vxy       = fileService->make<TH1F>("TightMuons_vxy",       "", nbins_vxy, vxy_bins);
  hSoftMuons_vxy        = fileService->make<TH1F>("SoftMuons_vxy",        "", nbins_vxy, vxy_bins);
  hStaMuons_noGen_vxy   = fileService->make<TH1F>("StaMuons_noGen_vxy",   "", nbins_vxy, vxy_bins);
  hTrkMuons_noGen_vxy   = fileService->make<TH1F>("TrkMuons_noGen_vxy",   "", nbins_vxy, vxy_bins);
  hGlbMuons_noGen_vxy   = fileService->make<TH1F>("GlbMuons_noGen_vxy",   "", nbins_vxy, vxy_bins);
  hTightMuons_noGen_vxy = fileService->make<TH1F>("TightMuons_noGen_vxy", "", nbins_vxy, vxy_bins);
  hSoftMuons_noGen_vxy  = fileService->make<TH1F>("SoftMuons_noGen_vxy",  "", nbins_vxy, vxy_bins);
  hDispGlbMuons_vxy     = fileService->make<TH1F>("DispGlbMuons_vxy",    "", nbins_vxy, vxy_bins);
  hDispGlbMuons_noGen_vxy     = fileService->make<TH1F>("DispGlbMuons_noGen_vxy",    "", nbins_vxy, vxy_bins);
  hDispStaMuons_vxy     = fileService->make<TH1F>("DispStaMuons_vxy",     "", nbins_vxy, vxy_bins);
  hDispStaMuons_noGen_vxy     = fileService->make<TH1F>("DispStaMuons_noGen_vxy",     "", nbins_vxy, vxy_bins);

  hGenMuons_vz         = fileService->make<TH1F>("GenMuons_vz",         "", nbins_vz, vz_bins);
  hStaMuons_vz         = fileService->make<TH1F>("StaMuons_vz",         "", nbins_vz, vz_bins);
  hTrkMuons_vz         = fileService->make<TH1F>("TrkMuons_vz",         "", nbins_vz, vz_bins);
  hGlbMuons_vz         = fileService->make<TH1F>("GlbMuons_vz",         "", nbins_vz, vz_bins);
  hTightMuons_vz       = fileService->make<TH1F>("TightMuons_vz",       "", nbins_vz, vz_bins);
  hSoftMuons_vz        = fileService->make<TH1F>("SoftMuons_vz",        "", nbins_vz, vz_bins);
  hStaMuons_noGen_vz   = fileService->make<TH1F>("StaMuons_noGen_vz",   "", nbins_vz, vz_bins);
  hTrkMuons_noGen_vz   = fileService->make<TH1F>("TrkMuons_noGen_vz",   "", nbins_vz, vz_bins);
  hGlbMuons_noGen_vz   = fileService->make<TH1F>("GlbMuons_noGen_vz",   "", nbins_vz, vz_bins);
  hTightMuons_noGen_vz = fileService->make<TH1F>("TightMuons_noGen_vz", "", nbins_vz, vz_bins);
  hSoftMuons_noGen_vz  = fileService->make<TH1F>("SoftMuons_noGen_vz",  "", nbins_vz, vz_bins);
  hDispGlbMuons_vz     = fileService->make<TH1F>("DispGlbMuons_vz",    "", nbins_vz, vz_bins);
  hDispGlbMuons_noGen_vz     = fileService->make<TH1F>("DispGlbMuons_noGen_vz",    "", nbins_vz, vz_bins);
  hDispStaMuons_vz     = fileService->make<TH1F>("DispStaMuons_vz",     "", nbins_vz, vz_bins);
  hDispStaMuons_noGen_vz     = fileService->make<TH1F>("DispStaMuons_noGen_vz",     "", nbins_vz, vz_bins);

  hGenMuons_vr         = fileService->make<TH1F>("GenMuons_vr",         "", nbins_vr, vr_bins);
  hStaMuons_vr         = fileService->make<TH1F>("StaMuons_vr",         "", nbins_vr, vr_bins);
  hTrkMuons_vr         = fileService->make<TH1F>("TrkMuons_vr",         "", nbins_vr, vr_bins);
  hGlbMuons_vr         = fileService->make<TH1F>("GlbMuons_vr",         "", nbins_vr, vr_bins);
  hTightMuons_vr       = fileService->make<TH1F>("TightMuons_vr",       "", nbins_vr, vr_bins);
  hSoftMuons_vr        = fileService->make<TH1F>("SoftMuons_vr",        "", nbins_vr, vr_bins);
  hStaMuons_noGen_vr   = fileService->make<TH1F>("StaMuons_noGen_vr",   "", nbins_vr, vr_bins);
  hTrkMuons_noGen_vr   = fileService->make<TH1F>("TrkMuons_noGen_vr",   "", nbins_vr, vr_bins);
  hGlbMuons_noGen_vr   = fileService->make<TH1F>("GlbMuons_noGen_vr",   "", nbins_vr, vr_bins);
  hTightMuons_noGen_vr = fileService->make<TH1F>("TightMuons_noGen_vr", "", nbins_vr, vr_bins);
  hSoftMuons_noGen_vr  = fileService->make<TH1F>("SoftMuons_noGen_vr",  "", nbins_vr, vr_bins);
  hDispGlbMuons_vr     = fileService->make<TH1F>("DispGlbMuons_vr",    "", nbins_vr, vr_bins);
  hDispGlbMuons_noGen_vr     = fileService->make<TH1F>("DispGlbMuons_noGen_vr",    "", nbins_vr, vr_bins);
  hDispStaMuons_vr     = fileService->make<TH1F>("DispStaMuons_vr",     "", nbins_vr, vr_bins);
  hDispStaMuons_noGen_vr     = fileService->make<TH1F>("DispStaMuons_noGen_vr",     "", nbins_vr, vr_bins);

  hGenMuons_pt         = fileService->make<TH1F>("GenMuons_pt",         "", nbins_pt, pt_bins);
  hStaMuons_pt         = fileService->make<TH1F>("StaMuons_pt",         "", nbins_pt, pt_bins);
  hTrkMuons_pt         = fileService->make<TH1F>("TrkMuons_pt",         "", nbins_pt, pt_bins);
  hGlbMuons_pt         = fileService->make<TH1F>("GlbMuons_pt",         "", nbins_pt, pt_bins);
  hTightMuons_pt       = fileService->make<TH1F>("TightMuons_pt",       "", nbins_pt, pt_bins);
  hSoftMuons_pt        = fileService->make<TH1F>("SoftMuons_pt",        "", nbins_pt, pt_bins);
  hStaMuons_noGen_pt   = fileService->make<TH1F>("StaMuons_noGen_pt",   "", nbins_pt, pt_bins);
  hTrkMuons_noGen_pt   = fileService->make<TH1F>("TrkMuons_noGen_pt",   "", nbins_pt, pt_bins);
  hGlbMuons_noGen_pt   = fileService->make<TH1F>("GlbMuons_noGen_pt",   "", nbins_pt, pt_bins);
  hTightMuons_noGen_pt = fileService->make<TH1F>("TightMuons_noGen_pt", "", nbins_pt, pt_bins);
  hSoftMuons_noGen_pt  = fileService->make<TH1F>("SoftMuons_noGen_pt",  "", nbins_pt, pt_bins);
  hDispGlbMuons_pt     = fileService->make<TH1F>("DispGlbMuons_pt",    "", nbins_pt, pt_bins);
  hDispGlbMuons_noGen_pt     = fileService->make<TH1F>("DispGlbMuons_noGen_pt",    "", nbins_pt, pt_bins);
  hDispStaMuons_pt     = fileService->make<TH1F>("DispStaMuons_pt",     "", nbins_pt, pt_bins);
  hDispStaMuons_noGen_pt     = fileService->make<TH1F>("DispStaMuons_noGen_pt",     "", nbins_pt, pt_bins);

  hGenMuons_eta         = fileService->make<TH1F>("GenMuons_eta",         "", 100, -2.5, 2.5);
  hStaMuons_eta         = fileService->make<TH1F>("StaMuons_eta",         "", 100, -2.5, 2.5);
  hTrkMuons_eta         = fileService->make<TH1F>("TrkMuons_eta",         "", 100, -2.5, 2.5);
  hGlbMuons_eta         = fileService->make<TH1F>("GlbMuons_eta",         "", 100, -2.5, 2.5);
  hTightMuons_eta       = fileService->make<TH1F>("TightMuons_eta",       "", 100, -2.5, 2.5);
  hSoftMuons_eta        = fileService->make<TH1F>("SoftMuons_eta",        "", 100, -2.5, 2.5);
  hStaMuons_noGen_eta   = fileService->make<TH1F>("StaMuons_noGen_eta",   "", 100, -2.5, 2.5);
  hTrkMuons_noGen_eta   = fileService->make<TH1F>("TrkMuons_noGen_eta",   "", 100, -2.5, 2.5);
  hGlbMuons_noGen_eta   = fileService->make<TH1F>("GlbMuons_noGen_eta",   "", 100, -2.5, 2.5);
  hTightMuons_noGen_eta = fileService->make<TH1F>("TightMuons_noGen_eta", "", 100, -2.5, 2.5);
  hSoftMuons_noGen_eta  = fileService->make<TH1F>("SoftMuons_noGen_eta",  "", 100, -2.5, 2.5);
  hDispGlbMuons_eta     = fileService->make<TH1F>("DispGlbMuons_eta",    "", 100, -2.5, 2.5);
  hDispGlbMuons_noGen_eta     = fileService->make<TH1F>("DispGlbMuons_noGen_eta",    "", 100, -2.5, 2.5);
  hDispStaMuons_eta     = fileService->make<TH1F>("DispStaMuons_eta",     "", 100, -2.5, 2.5);
  hDispStaMuons_noGen_eta     = fileService->make<TH1F>("DispStaMuons_noGen_eta",     "", 100, -2.5, 2.5);

  hGenMuons_phi         = fileService->make<TH1F>("GenMuons_phi",         "", 100, -3.2, 3.2);
  hStaMuons_phi         = fileService->make<TH1F>("StaMuons_phi",         "", 100, -3.2, 3.2);
  hTrkMuons_phi         = fileService->make<TH1F>("TrkMuons_phi",         "", 100, -3.2, 3.2);
  hGlbMuons_phi         = fileService->make<TH1F>("GlbMuons_phi",         "", 100, -3.2, 3.2);
  hTightMuons_phi       = fileService->make<TH1F>("TightMuons_phi",       "", 100, -3.2, 3.2);
  hSoftMuons_phi        = fileService->make<TH1F>("SoftMuons_phi",        "", 100, -3.2, 3.2);
  hStaMuons_noGen_phi   = fileService->make<TH1F>("StaMuons_noGen_phi",   "", 100, -3.2, 3.2);
  hTrkMuons_noGen_phi   = fileService->make<TH1F>("TrkMuons_noGen_phi",   "", 100, -3.2, 3.2);
  hGlbMuons_noGen_phi   = fileService->make<TH1F>("GlbMuons_noGen_phi",   "", 100, -3.2, 3.2);
  hTightMuons_noGen_phi = fileService->make<TH1F>("TightMuons_noGen_phi", "", 100, -3.2, 3.2);
  hSoftMuons_noGen_phi  = fileService->make<TH1F>("SoftMuons_noGen_phi",  "", 100, -3.2, 3.2);
  hDispGlbMuons_phi     = fileService->make<TH1F>("DispGlbMuons_phi",    "", 100, -3.2, 3.2);
  hDispGlbMuons_noGen_phi     = fileService->make<TH1F>("DispGlbMuons_noGen_phi",    "", 100, -3.2, 3.2);
  hDispStaMuons_phi     = fileService->make<TH1F>("DispStaMuons_phi",    "", 100, -3.2, 3.2);
  hDispStaMuons_noGen_phi     = fileService->make<TH1F>("DispStaMuons_noGen_phi",    "", 100, -3.2, 3.2);

  hStaMuons_dR      = fileService->make<TH1F>("StaMuons_dR",   "", 100, 0, 4);
  hTrkMuons_dR      = fileService->make<TH1F>("TrkMuons_dR",   "", 100, 0, 4);
  hGlbMuons_dR      = fileService->make<TH1F>("GlbMuons_dR",   "", 100, 0, 4);
  hTightMuons_dR    = fileService->make<TH1F>("TightMuons_dR", "", 100, 0, 4);
  hSoftMuons_dR     = fileService->make<TH1F>("SoftMuons_dR",  "", 100, 0, 4);
  hDispGlbMuons_dR  = fileService->make<TH1F>("DispGlbMuons_dR",    "", 100, 0, 4);
  hDispStaMuons_dR  = fileService->make<TH1F>("DispStaMuons_dR",    "", 100, 0, 4);

  for (Int_t i=0; i<nbins_pt; i++) {
    hStaMuons_pt_resolution  [i] = fileService->make<TH2F>(Form("StaMuons_pt_resolution_pt%d",   i), "", nbins_vxy, vxy_bins, 120, -0.15, 0.15);
    hTrkMuons_pt_resolution  [i] = fileService->make<TH2F>(Form("TrkMuons_pt_resolution_pt%d",   i), "", nbins_vxy, vxy_bins, 120, -0.15, 0.15);
    hGlbMuons_pt_resolution  [i] = fileService->make<TH2F>(Form("GlbMuons_pt_resolution_pt%d",   i), "", nbins_vxy, vxy_bins, 120, -0.15, 0.15);
    hTightMuons_pt_resolution[i] = fileService->make<TH2F>(Form("TightMuons_pt_resolution_pt%d", i), "", nbins_vxy, vxy_bins, 120, -0.15, 0.15);
    hSoftMuons_pt_resolution [i] = fileService->make<TH2F>(Form("SoftMuons_pt_resolution_pt%d",  i), "", nbins_vxy, vxy_bins, 120, -0.15, 0.15);
    hDispGlbMuons_pt_resolution[i] = fileService->make<TH2F>(Form("DispGlbMuons_pt_resolution_pt%d", i), "", nbins_vxy, vxy_bins, 120, -0.15, 0.15);
    hDispStaMuons_pt_resolution[i] = fileService->make<TH2F>(Form("DispStaMuons_pt_resolution_pt%d", i), "", nbins_vxy, vxy_bins, 120, -0.15, 0.15);
  }

  hMuPFChargeIso  = fileService->make<TH1F>("MuPFChargeIso",  "#DeltaR=0.4 PFChargeIso",  200, 0, 1);
  hMuPFNeutralIso = fileService->make<TH1F>("MuPFNeutralIso", "#DeltaR=0.4 PFNeutralIso", 200, 0, 1);
  hMuPFPhotonIso  = fileService->make<TH1F>("MuPFPhotonIso",  "#DeltaR=0.4 PFPhotonIso",  200, 0, 1);
  hMuPFPUIso      = fileService->make<TH1F>("MuPFPUIso",      "#DeltaR=0.4 PFPUIso",      200, 0, 1);
  hMuPFIso        = fileService->make<TH1F>("MuPFIso",        "#DeltaR=0.4 SumPtIso",     200, 0, 1); 

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
    vertexType->AddBinContent(1);
  }
  else if (beamSpot.isValid()) {
    LogInfo("ExampleMuonAnalyzer") << "reco::PrimaryVertex not found, use BeamSpot position instead\n";
    
    posVtx      = bs.position();
    errVtx(0,0) = bs.BeamWidthX();
    errVtx(1,1) = bs.BeamWidthY();
    errVtx(2,2) = bs.sigmaZ();
    vertexType->AddBinContent(2);
  }
    
  const reco::Vertex thePrimaryVertex(posVtx,errVtx);
  
  
  //look over reconstructed mouns.
  Handle<reco::MuonCollection> muons;
  event.getByToken(muonToken, muons);
  
  
  //look over gen particles.
  Handle<reco::GenParticleCollection> genParticles;
  event.getByToken(genToken, genParticles);
  
	
  //Handle the Displaced global muons track collection.
  Handle<reco::TrackCollection> displacedGlobalMuons;
  event.getByToken(dispToken, displacedGlobalMuons);
	
	
  //Handle the displaced StandAlone Muons track collection.                                                                                                                                                  
  Handle<reco::TrackCollection>	displacedStandAloneMuons;
  event.getByToken(dispStaToken, displacedStandAloneMuons);

	
  //Local variable that save the number of match leptons                                                                                                                                                    
  Float_t numberOfLeptons = 0.0;
  Float_t numberOfGenMuon = 0.0;
  Int_t ind_muon1 = NULL;
  Int_t ind_muon2 = NULL;
  Int_t ind_muon3 = NULL;
  Int_t ind_muon4 = NULL;
	
  
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
	  
    if ((*genParticles)[i].isMuon()) numberOfGenMuon = numberOfGenMuon + 1;
    
    
    Float_t dxy = fabs(vxy);
    Float_t dz  = fabs(vz);
    
    
    
    // Loop over reconstructed muons
    //--------------------------------------------------------------------------
    Float_t sta_min_deltaR     = 999;   
    Float_t trk_min_deltaR     = 999; 
    Float_t glb_min_deltaR     = 999; 
    Float_t tight_min_deltaR   = 999;   
    Float_t soft_min_deltaR    = 999;
    Float_t dispGlb_min_deltaR = 999;
    Float_t dispSta_min_deltaR = 999;

    Float_t sta_pt_resolution     = -999;
    Float_t trk_pt_resolution     = -999;
    Float_t glb_pt_resolution     = -999;
    Float_t tight_pt_resolution   = -999;
    Float_t soft_pt_resolution    = -999;
    Float_t dispGlb_pt_resolution = -999;
    Float_t dispSta_pt_resolution = -999;
	
    Int_t i = 0;
    if ((*genParticles)[i].isMuon()) i++;
    
    Int_t k = 0;
    
    for (reco::MuonCollection::const_iterator muon=muons->begin(); muon!=muons->end(); ++muon) {
	    
	    Float_t chargeIso  = muon->pfIsolationR04().sumChargedHadronPt;
	    Float_t neutralIso = muon->pfIsolationR04().sumNeutralHadronEt;
	    Float_t photonIso  = muon->pfIsolationR04().sumPhotonEt;
	    Float_t puIso      = muon->pfIsolationR04().sumPUPt;
	    Float_t iso        = chargeIso + max(0., neutralIso + photonIso - 0.5*puIso);

	    hMuPFChargeIso ->Fill(chargeIso  / muon->pt());
	    hMuPFNeutralIso->Fill(neutralIso / muon->pt());
	    hMuPFPhotonIso ->Fill(photonIso  / muon->pt());
	    hMuPFPUIso     ->Fill(puIso      / muon->pt());
	    hMuPFIso       ->Fill(iso        / muon->pt());
	    
	    
	    
      // isStandAloneMuon
      //------------------------------------------------------------------------
      if (muon->isStandAloneMuon()) {
     
	Float_t muEta    = muon->standAloneMuon()->eta();
	Float_t muPhi    = muon->standAloneMuon()->phi();
	Float_t muPt     = muon->standAloneMuon()->pt();
	Float_t muCharge = muon->standAloneMuon()->charge();

	if (fabs(muEta) > 2.4) continue;
	if (muPt < pt_min)     continue;

	Float_t dPhi = muPhi - phi;
	Float_t dEta = muEta - eta;

	Float_t dR = sqrt(dPhi*dPhi + dEta*dEta); 

	if (dR < sta_min_deltaR) {
	  sta_min_deltaR    = dR;
	  sta_pt_resolution = ((muCharge/muPt) - (charge/pt)) / (charge/pt);
	}
      }


      // isTrackerMuon
      //------------------------------------------------------------------------
      if (muon->isTrackerMuon()) {

	float muEta    = muon->innerTrack()->eta();
	float muPhi    = muon->innerTrack()->phi();
	float muPt     = muon->innerTrack()->pt();
	float muCharge = muon->innerTrack()->charge();

	if (fabs(muEta) > 2.4) continue;
	if (muPt < pt_min)     continue;
       
	float dPhi = muPhi - phi;
	float dEta = muEta - eta;

	float dR = sqrt(dPhi*dPhi + dEta*dEta); 
	
	if (dR < trk_min_deltaR) {
	  trk_min_deltaR    = dR;
	  trk_pt_resolution = ((muCharge/muPt) - (charge/pt)) / (charge/pt);
	  
	  if (i == 1){
		  ind_muon1 = k;
	  } else if (i == 2){
		  ind_muon2 = k;
	  } else if (i == 3){
		  ind_muon3 = k;
	  } else if (i == 4){
		  ind_muon4 = k;
	  }
	}
      }


      // isGlobalMuon
      //------------------------------------------------------------------------
      if (muon->isGlobalMuon() && muon->isStandAloneMuon()) {

	float muEta    = muon->globalTrack()->eta();
	float muPhi    = muon->globalTrack()->phi();
	float muPt     = muon->globalTrack()->pt();
	float muCharge = muon->globalTrack()->charge();

	if (fabs(muEta) > 2.4) continue;
	if (muPt < pt_min)     continue;
       
	float dPhi = muPhi - phi;
	float dEta = muEta - eta;

	float dR = sqrt(dPhi*dPhi + dEta*dEta); 
	
	if (dR < glb_min_deltaR) {
	  glb_min_deltaR    = dR;
	  glb_pt_resolution = ((muCharge/muPt) - (charge/pt)) / (charge/pt);
	}
      }


      // isTightMuon
      //------------------------------------------------------------------------
      if (muon::isTightMuon(*muon, thePrimaryVertex)) {
	
	Float_t muEta    = muon->eta();
	Float_t muPhi    = muon->phi();
	Float_t muPt     = muon->pt();
	Float_t muCharge = muon->charge();

	if (fabs(muEta) > 2.4) continue;
	if (muPt < pt_min)     continue;
 
	Float_t dPhi = muPhi - phi;
	Float_t dEta = muEta - eta;

	Float_t dR = sqrt(dPhi*dPhi + dEta*dEta); 

	if (dR < tight_min_deltaR) {
	  tight_min_deltaR    = dR;
	  tight_pt_resolution = ((muCharge/muPt) - (charge/pt)) / (charge/pt);
	}
      }


      // isSoftMuon
      //------------------------------------------------------------------------
      if (muon::isSoftMuon(*muon, thePrimaryVertex)) {
	
	Float_t muEta    = muon->eta();
	Float_t muPhi    = muon->phi();
	Float_t muPt     = muon->pt();
	Float_t muCharge = muon->charge();

	if (fabs(muEta) > 2.4) continue;
	if (muPt < pt_min)     continue;
 
	Float_t dPhi = muPhi - phi;
	Float_t dEta = muEta - eta;

	Float_t dR = sqrt(dPhi*dPhi + dEta*dEta); 

	if (dR < soft_min_deltaR) {
	  soft_min_deltaR    = dR;
	  soft_pt_resolution = ((muCharge/muPt) - (charge/pt)) / (charge/pt);
	}
      }
    }  // for..muons
	  
    //Loop over the displacedGlobalMuons collection. 
    //Is displacedGlobalMuon:	  
    for (reco::TrackCollection::const_iterator track = displacedGlobalMuons->begin(); track < displacedGlobalMuons->end(); ++track){

      if (!track->innerOk()) continue;                                                                                                                                                
      if (!track->outerOk()) continue;                                                                                                                                                

      double trEta = track->eta();
      double trPhi = track->phi();
      double trPt  = track->pt();
      double trCharge = track->charge();
	    
      if (fabs(trEta) > 2.4) continue;
      if (trPt < pt_min)     continue;

      double dPhi = trPhi - phi;
      double dEta = trEta - eta;

      double dR = sqrt(dPhi*dPhi + dEta*dEta);

      if (dR < dispGlb_min_deltaR) {
        dispGlb_min_deltaR = dR;
	dispGlb_pt_resolution = ((trCharge/trPt) - (charge/pt)) / (charge/pt);
      }
	    
      k++;
	    
    }//for...displacedGlobalMuons collection	
	  
    //Loop over the displacedStandAloneMuons collection.
    //Is displacedStandAlone:
    for (reco::TrackCollection::const_iterator itrack = displacedStandAloneMuons->begin(); itrack < displacedStandAloneMuons->end(); ++itrack){

      if (!itrack->innerOk()) continue;                                                                                                                                                
      if (!itrack->outerOk()) continue;                                                                                                                                                

      double itrEta = itrack->eta();
      double itrPhi = itrack->phi();
      double itrPt  = itrack->pt();
      double itrCharge = itrack->charge();
	    
      if (fabs(itrEta) > 2.4) continue;
      if (itrPt < pt_min)     continue;

      double idPhi = itrPhi - phi;
      double idEta = itrEta - eta;

      double dR = sqrt(idPhi*idPhi + idEta*idEta);

      if (dR < dispSta_min_deltaR) {
        dispSta_min_deltaR = dR;
	dispSta_pt_resolution = ((itrCharge/itrPt) - (charge/pt)) / (charge/pt);
      }
    }


    // Fill gen histograms
    //--------------------------------------------------------------------------
    h_dxy->Fill(dxy);
    h_dz ->Fill(dz);
    h_vxy->Fill(vxy);
    h_vz ->Fill(fabs(vz));
    h_vr ->Fill(vr);
    h_pt ->Fill(pt);

    hGenMuons_vxy->Fill(vxy);
    hGenMuons_vz ->Fill(fabs(vz));
    hGenMuons_vr ->Fill(vr);
    hGenMuons_eta->Fill(eta);
    hGenMuons_phi->Fill(phi);
    hGenMuons_pt ->Fill(pt);

    hGenMuons_vxy_vz->Fill(vxy,fabs(vz));


    // Fill sta histograms
    //--------------------------------------------------------------------------
    if (sta_min_deltaR < 999)
      {
	hStaMuons_dR->Fill(sta_min_deltaR);
	    
	if (sta_min_deltaR < deltaR_max)
	  {
	    hStaMuons_vxy->Fill(vxy);
	    hStaMuons_vz ->Fill(fabs(vz));
	    hStaMuons_vr ->Fill(vr);
	    hStaMuons_eta->Fill(eta);
	    hStaMuons_phi->Fill(phi);
	    hStaMuons_pt ->Fill(pt);

	    hStaMuons_vxy_vz->Fill(vxy,fabs(vz));

	    for (Int_t i=0; i<nbins_pt; i++)
	      if (pt >= pt_bins[i] && pt < pt_bins[i+1]) hStaMuons_pt_resolution[i]->Fill(vxy, sta_pt_resolution);
	  }
	else
	  {
	    hStaMuons_noGen_vxy->Fill(vxy);
	    hStaMuons_noGen_vz ->Fill(fabs(vz));
	    hStaMuons_noGen_vr ->Fill(vr);
	    hStaMuons_noGen_eta->Fill(eta);
	    hStaMuons_noGen_phi->Fill(phi);
	    hStaMuons_noGen_pt ->Fill(pt);

	    hStaMuons_noGen_vxy_vz->Fill(vxy,fabs(vz));
	  }
      }


    // Fill trk histograms
    //--------------------------------------------------------------------------
    if (trk_min_deltaR < 999)
      {
	hTrkMuons_dR->Fill(trk_min_deltaR);
	
	numberOfLeptons = numberOfLeptons + 1;
	    
	if (trk_min_deltaR < deltaR_max)
	  {
	    hTrkMuons_vxy->Fill(vxy);
	    hTrkMuons_vz ->Fill(fabs(vz));
	    hTrkMuons_vr ->Fill(vr);
	    hTrkMuons_eta->Fill(eta);
	    hTrkMuons_phi->Fill(phi);
	    hTrkMuons_pt ->Fill(pt);

	    hTrkMuons_vxy_vz->Fill(vxy,fabs(vz));

	    for (Int_t i=0; i<nbins_pt; i++)
	      if (pt >= pt_bins[i] && pt < pt_bins[i+1]) hTrkMuons_pt_resolution[i]->Fill(vxy, trk_pt_resolution);
	  }
	else
	  {
	    hTrkMuons_noGen_vxy->Fill(vxy);
	    hTrkMuons_noGen_vz ->Fill(fabs(vz));
	    hTrkMuons_noGen_vr ->Fill(vr);
	    hTrkMuons_noGen_eta->Fill(eta);
	    hTrkMuons_noGen_phi->Fill(phi);
	    hTrkMuons_noGen_pt ->Fill(pt);

	    hTrkMuons_noGen_vxy_vz->Fill(vxy,fabs(vz));
	  }
      }


    // Fill glb histograms
    //--------------------------------------------------------------------------
    if (glb_min_deltaR < 999)
      {
	hGlbMuons_dR->Fill(glb_min_deltaR);

	if (glb_min_deltaR < deltaR_max)
	  {
	    hGlbMuons_vxy->Fill(vxy);
	    hGlbMuons_vz ->Fill(fabs(vz));
	    hGlbMuons_vr ->Fill(vr);
	    hGlbMuons_eta->Fill(eta);	
	    hGlbMuons_phi->Fill(phi);
	    hGlbMuons_pt ->Fill(pt);
	    
	    hGlbMuons_vxy_vz->Fill(vxy,fabs(vz));

	    for (Int_t i=0; i<nbins_pt; i++)
	      if (pt >= pt_bins[i] && pt < pt_bins[i+1]) hGlbMuons_pt_resolution[i]->Fill(vxy, glb_pt_resolution);
	  }
	else
	  {
	    hGlbMuons_noGen_vxy->Fill(vxy);
	    hGlbMuons_noGen_vz ->Fill(fabs(vz));
	    hGlbMuons_noGen_vr ->Fill(vr);
	    hGlbMuons_noGen_eta->Fill(eta);	
	    hGlbMuons_noGen_phi->Fill(phi);
	    hGlbMuons_noGen_pt ->Fill(pt);

	    hGlbMuons_noGen_vxy_vz->Fill(vxy,fabs(vz));
	  }
      }


    // Fill tight histograms
    //--------------------------------------------------------------------------
    if (tight_min_deltaR < 999)
      {
	hTightMuons_dR->Fill(tight_min_deltaR);
	
	if (tight_min_deltaR < deltaR_max)
	  {
	    hTightMuons_vxy->Fill(vxy);
	    hTightMuons_vz ->Fill(fabs(vz));
	    hTightMuons_vr ->Fill(vr);
	    hTightMuons_eta->Fill(eta);	
	    hTightMuons_phi->Fill(phi);
	    hTightMuons_pt ->Fill(pt);

	    hTightMuons_vxy_vz->Fill(vxy,fabs(vz));

	    for (Int_t i=0; i<nbins_pt; i++)
	      if (pt >= pt_bins[i] && pt < pt_bins[i+1]) hTightMuons_pt_resolution[i]->Fill(vxy, tight_pt_resolution);
	  }
	else
	  {
	    hTightMuons_noGen_vxy->Fill(vxy);
	    hTightMuons_noGen_vz ->Fill(fabs(vz));
	    hTightMuons_noGen_vr ->Fill(vr);
	    hTightMuons_noGen_eta->Fill(eta);
	    hTightMuons_noGen_phi->Fill(phi);
	    hTightMuons_noGen_pt ->Fill(pt);

	    hTightMuons_noGen_vxy_vz->Fill(vxy,fabs(vz));
	  }
      }

    // Fill soft histograms
    //--------------------------------------------------------------------------
    if (soft_min_deltaR < 999)
      {
	hSoftMuons_dR->Fill(soft_min_deltaR);
	
	if (soft_min_deltaR < deltaR_max)
	  {
	    hSoftMuons_vxy->Fill(vxy);
	    hSoftMuons_vz ->Fill(fabs(vz));
	    hSoftMuons_vr ->Fill(vr);
	    hSoftMuons_eta->Fill(eta);	
	    hSoftMuons_phi->Fill(phi);
	    hSoftMuons_pt ->Fill(pt);

	    hSoftMuons_vxy_vz->Fill(vxy,fabs(vz));

	    for (Int_t i=0; i<nbins_pt; i++)
	      if (pt >= pt_bins[i] && pt < pt_bins[i+1]) hSoftMuons_pt_resolution[i]->Fill(vxy, soft_pt_resolution);
	  }
	else
	  {
	    hSoftMuons_noGen_vxy->Fill(vxy);
	    hSoftMuons_noGen_vz ->Fill(fabs(vz));
	    hSoftMuons_noGen_vr ->Fill(vr);
	    hSoftMuons_noGen_eta->Fill(eta);
	    hSoftMuons_noGen_phi->Fill(phi);
	    hSoftMuons_noGen_pt ->Fill(pt);

	    hSoftMuons_noGen_vxy_vz->Fill(vxy,fabs(vz));
	  }
      }
      //Fill displacedGlobal histograms
      if (dispGlb_min_deltaR < 999) 
      {
	      
	      hDispGlbMuons_dR->Fill(dispGlb_min_deltaR);
	      
	      if (dispGlb_min_deltaR < deltaR_max)
	      {
		      hDispGlbMuons_vxy->Fill(vxy);
		      hDispGlbMuons_vz->Fill(fabs(vz));
		      hDispGlbMuons_vr->Fill(vr);
		      hDispGlbMuons_eta->Fill(eta);
		      hDispGlbMuons_phi->Fill(phi);
		      hDispGlbMuons_pt->Fill(pt);
		      for (Int_t i=0; i<nbins_pt; i++)
	      		if (pt >= pt_bins[i] && pt < pt_bins[i+1]) hDispGlbMuons_pt_resolution[i]->Fill(vxy, dispGlb_pt_resolution);
	      }
	      else
	      {
		      hDispGlbMuons_noGen_vxy->Fill(vxy);
		      hDispGlbMuons_noGen_vz->Fill(fabs(vz));
		      hDispGlbMuons_noGen_vr->Fill(vr);
		      hDispGlbMuons_noGen_eta->Fill(eta);
		      hDispGlbMuons_noGen_phi->Fill(phi);
		      hDispGlbMuons_noGen_pt->Fill(pt);  
	      }
      }
      //Fill displacedStandAlone histograms.
      if (dispSta_min_deltaR < 999) 
      {
	      
	      hDispStaMuons_dR->Fill(dispSta_min_deltaR);
	      
	      if (dispSta_min_deltaR < deltaR_max)
	      {
		      hDispStaMuons_vxy->Fill(vxy);
		      hDispStaMuons_vz->Fill(fabs(vz));
		      hDispStaMuons_vr->Fill(vr);
		      hDispStaMuons_eta->Fill(eta);
		      hDispStaMuons_phi->Fill(phi);
		      hDispStaMuons_pt->Fill(pt);
		      for (Int_t i=0; i<nbins_pt; i++)
	      		if (pt >= pt_bins[i] && pt < pt_bins[i+1]) hDispStaMuons_pt_resolution[i]->Fill(vxy, dispSta_pt_resolution);
	      }
	      else
	      {
		      hDispStaMuons_noGen_vxy->Fill(vxy);
		      hDispStaMuons_noGen_vz->Fill(fabs(vz));
		      hDispStaMuons_noGen_vr->Fill(vr);
		      hDispStaMuons_noGen_eta->Fill(eta);
		      hDispStaMuons_noGen_phi->Fill(phi);
		      hDispStaMuons_noGen_pt->Fill(pt);  
	      }
      }
  } //...End gen particles loop.
	
  


  hNumberOfLepton->Fill(numberOfLeptons);
  hNumber2D->Fill(numberOfGenMuon, numberOfLeptons);


  //----------------------------------------------------------------------                                                                                                                                  
  //Do mumu invariant mass                                                                                                                                                                                  
  //---------------------------------------------------------------------- 
	
	
  //Case 2 leptons matched:                                                                                                                                                                                 
  if (numberOfLeptons == 2){

    Int_t i = 0;
    for (reco::MuonCollection::const_iterator mu1=muons->begin(); mu1!=muons->end(); ++mu1) {

      Int_t j = 0;
      for (reco::MuonCollection::const_iterator mu2=muons->begin(); mu2!=muons->end(); ++mu2) {
        if (i == ind_muon1 && j == ind_muon2){
          if (mu1 < mu2) {
  
            if (mu1->charge() * mu2->charge() < 0){
  
              hInvMass->Fill((mu1->p4() + mu2->p4()).mass());
	    
	    if (mu1->isStandAloneMuon() && mu2->isStandAloneMuon()){
  
                hInvMassSta->Fill((mu1->p4() + mu2->p4()).mass());
  
            }
  
              if (mu1->isGlobalMuon() && mu2->isGlobalMuon()){
  
                hInvMassGlb->Fill((mu1->p4() + mu2->p4()).mass());
  
              }
  
              if (mu1->isTrackerMuon() && mu2->isTrackerMuon()){
  
                hInvMassTrk->Fill((mu1->p4() + mu2->p4()).mass());
  
              }
  
	    }
  
          }
		
	}
	j++;
      }
      i++;
    }

  }

//Case 3 leptons matched:                                                                                                                                                                                 
  if (numberOfLeptons == 3){

    Bool_t firstMass = false;

    Float_t M1 = -999.0;
    Float_t M2 = -999.0;

    Float_t M1Trk = -999.0;
    Float_t M2Trk = -999.0;

    Float_t M1Sta = -999.0;
    Float_t M2Sta = -999.0;

    Float_t M1Glb = -999.0;
    Float_t M2Glb = -999.0;

    Int_t i = 0;
    for(reco::MuonCollection::const_iterator mu1=muons->begin(); mu1!=muons->end(); ++mu1) {

      Int_t j = 0;	
      for (reco::MuonCollection::const_iterator mu2=muons->begin(); mu2!=muons->end(); ++mu2) {
        if (j == ind_muon2 || j == ind_muon3){
          if (mu1 < mu2 && i == ind_muon1) {
  
            if (mu1->charge() * mu2->charge() < 0){
  
              if (firstMass == false){
                M1 = (mu1->p4() + mu2->p4()).mass();  //First combiantion mu+ with mu-                                                                                                                        
  
                if (mu1->isTrackerMuon() && mu2->isTrackerMuon()){
                  M1Trk = M1;
                }
                if (mu1->isStandAloneMuon() && mu2->isStandAloneMuon()){
                  M1Sta = M1;
                }
                if (mu1->isGlobalMuon() && mu2->isGlobalMuon()){
                  M1Glb = M1;
                }
	   } else {                               //Second combination                                                                                                                                     
                M2 = (mu1->p4() + mu2->p4()).mass();
  
                if (mu1->isTrackerMuon() && mu2->isTrackerMuon()){
                  M2Trk = M2;
                }
                if (mu1->isStandAloneMuon() && mu2->isStandAloneMuon()){
                  M2Sta = M2;
                }
                if (mu1->isGlobalMuon() && mu2->isGlobalMuon()){
	  	M2Glb = M2;
                }
  
              }

              firstMass = true;

            }
	  }

        }
        j++;
      }
      i++;
    }

   //Reconstructed muons                                                                                                                                                                                   
    if (M1 > 0 && M2 > 0){

      if ( fabs(50.0 - M1) < fabs(50.0 - M2)){
        hInvMass->Fill(M1);
      }else {
        hInvMass->Fill(M2);
      }

    }else if(M1 > 0){
      hInvMass->Fill(M1);
    }else if(M2 > 0){
      hInvMass->Fill(M2);
    }

    //Tracker Muons                                                                                                                                                                                         
    if (M1Trk > 0 && M2Trk > 0){

      if ( fabs(50.0 - M1Trk) < fabs(50.0 - M2Trk)){
        hInvMassTrk->Fill(M1Trk);
      }else {
        hInvMassTrk->Fill(M2Trk);
      }

    }else if(M1Trk > 0){
      hInvMassTrk->Fill(M1);
    }else if(M2Trk > 0){
      hInvMassTrk->Fill(M2);
    }
   //StandAlone Muons                                                                                                                                                                                      
    if (M1Sta > 0 && M2Sta > 0){

      if ( fabs(50.0 - M1Sta) < fabs(50.0 - M2Sta)){
        hInvMassSta->Fill(M1Sta);
      }else {
        hInvMassSta->Fill(M2Sta);
      }

    }else if(M1Sta > 0){
      hInvMassSta->Fill(M1Sta);
    }else if(M2Sta > 0){
      hInvMassSta->Fill(M2Sta);
    }

    //Global Muons                                                                                                                                                                                          
    if (M1Glb > 0 && M2Glb > 0){

      if ( fabs(50.0 - M1Glb) < fabs(50.0 - M2Glb)){
        hInvMassGlb->Fill(M1Glb);
      }else {
        hInvMassGlb->Fill(M2Glb);
      }

    }else if(M1Glb > 0){
      hInvMassGlb->Fill(M1Glb);
    }else if(M2Glb > 0){
      hInvMassGlb->Fill(M2Glb);
    }

  }

  //Case of 4 leptons matched                                                                                                                                                                               
  if (numberOfLeptons == 4){

    Int_t index = 1;

    Float_t M1 = -999.0;
    Float_t M2 = -999.0;
    Float_t M3 = -999.0;
    Float_t M4 = -999.0;

    Float_t M1Trk = -999.0;
    Float_t M2Trk = -999.0;
    Float_t M3Trk = -999.0;
    Float_t M4Trk = -999.0;

    Float_t M1Sta = -999.0;
    Float_t M2Sta = -999.0;
    Float_t M3Sta = -999.0;
    Float_t M4Sta = -999.0;

    Float_t M1Glb = -999.0;
    Float_t M2Glb = -999.0;
    Float_t M3Glb = -999.0;
    Float_t M4Glb = -999.0;

    Int_t i = 0;
    for (reco::MuonCollection::const_iterator mu1=muons->begin(); mu1!=muons->end(); ++mu1) {

      Int_t j = 0;
      for(reco::MuonCollection::const_iterator mu2=muons->begin(); mu2!=muons->end(); ++mu2) {

	 if (i == ind_muon1 || i == ind_muon2 || i == ind_muon3 || i == ind_muon4){
	
	  if (j == ind_muon1 || j == ind_muon2 || j == ind_muon3 || j == ind_muon4){
            if (mu1 < mu2) {
    
              if (mu1->charge() * mu2->charge() < 0){
    
    
                if (index == 1){
                  M1 = (mu1->p4() + mu2->p4()).mass();  //First combiantion mu+ with mu-                                                                                                                       \
                                                                                                                                                                                                                
    
                  if (mu1->isTrackerMuon() && mu2->isTrackerMuon()){
		    M1Trk = M1;
                  }
                  if (mu1->isStandAloneMuon() && mu2->isStandAloneMuon()){
                    M1Sta = M1;
                  }
                  if (mu1->isGlobalMuon() && mu2->isGlobalMuon()){
                    M1Glb = M1;
                  }
    
                  index++;
    
                } else if(index == 2){                               //Second combination                                                                                                                      \
                                                                                                                                                                                                                
                  M2 = (mu1->p4() + mu2->p4()).mass();
    
                  if (mu1->isTrackerMuon() && mu2->isTrackerMuon()){
                    M2Trk = M2;
                  }
                  if (mu1->isStandAloneMuon() && mu2->isStandAloneMuon()){
                    M2Sta = M2;
                  }
                  if (mu1->isGlobalMuon() && mu2->isGlobalMuon()){
                    M2Glb = M2;
                    }
    
                  index++;
	        }else if(index == 3){                          //Third combination, second pair                                                                                                                 
  
                  M3 = (mu1->p4() + mu2->p4()).mass();
    
                  if (mu1->isTrackerMuon() && mu2->isTrackerMuon()){
                    M3Trk = M3;
                  }
                  if (mu1->isStandAloneMuon() && mu2->isStandAloneMuon()){
                    M3Sta = M3;
                  }
                  if (mu1->isGlobalMuon() && mu2->isGlobalMuon()){
                    M3Glb = M3;
                  }
    
                  index++;
    
	       }else {                                     //Fourth combination, second pair                                                                                                                   
    
                  M4 = (mu1->p4() + mu2->p4()).mass();
  
                  if (mu1->isTrackerMuon() && mu2->isTrackerMuon()){
                    M4Trk = M4;
                  }
                  if (mu1->isStandAloneMuon() && mu2->isStandAloneMuon()){
                    M4Sta = M4;
                  }
                  if (mu1->isGlobalMuon() && mu2->isGlobalMuon()){
                    M4Glb = M4;
                  }
    
                }
    
              }
	    }
	  }
		
        }
        j++;
      }
      i++;
    }

  //First pair combination.                                                                                                                                                                               
    //Reconstructed muons                                                                                                                                                                                   
    if (M1 > 0 && M2 > 0){

      if ( fabs(50.0 - M1) < fabs(50.0 - M2)){
        hInvMass->Fill(M1);
      }else {
        hInvMass->Fill(M2);
      }

    }else if(M1 > 0){
      hInvMass->Fill(M1);
    }else if(M2 > 0){
      hInvMass->Fill(M2);
    }

    //Tracker Muons                                                                                                                                                                                         
    if (M1Trk > 0 && M2Trk > 0){

      if ( fabs(50.0 - M1Trk) < fabs(50.0 - M2Trk)){
        hInvMassTrk->Fill(M1Trk);
      }else {
        hInvMassTrk->Fill(M2Trk);
      }

    }else if(M1Trk > 0){
      hInvMassTrk->Fill(M1);
    }else if(M2Trk > 0){
      hInvMassTrk->Fill(M2);
    }

    //StandAlone Muons                                                                                                                                                                                      
    if (M1Sta > 0 && M2Sta > 0){

      if ( fabs(50.0 - M1Sta) < fabs(50.0 - M2Sta)){
        hInvMassSta->Fill(M1Sta);
      }else {
        hInvMassSta->Fill(M2Sta);
      }

    }else if(M1Sta > 0){
      hInvMassSta->Fill(M1Sta);
    }else if(M2Sta > 0){
      hInvMassSta->Fill(M2Sta);
    }
  //Global Muons                                                                                                                                                                                          
    if (M1Glb > 0 && M2Glb > 0){

      if ( fabs(50.0 - M1Glb) < fabs(50.0 - M2Glb)){
        hInvMassGlb->Fill(M1Glb);
      }else {
        hInvMassGlb->Fill(M2Glb);
      }

    }else if(M1Glb > 0){
      hInvMassGlb->Fill(M1Glb);
    }else if(M2Glb > 0){
      hInvMassGlb->Fill(M2Glb);
    }


    //Second pair combination                                                                                                                                                                               
    //Reconstructed muons                                                                                                                                                                                   
    if (M3 > 0 && M4 > 0){

      if (fabs(50.0 - M3) < fabs(50.0 - M4)){
        hInvMass->Fill(M3);
      }else {
        hInvMass->Fill(M4);
      }

    }else if(M3 > 0){
      hInvMass->Fill(M3);
    }else if(M4 > 0){
      hInvMass->Fill(M4);
    }
  //Tracker Muons                                                                                                                                                                                         
    if (M3Trk > 0 && M4Trk > 0){

      if ( fabs(50.0 - M3Trk) < fabs(50.0 - M4Trk)){
        hInvMassTrk->Fill(M3Trk);
      }else {
        hInvMassTrk->Fill(M4Trk);
      }

    }else if(M3Trk > 0){
      hInvMassTrk->Fill(M3);
    }else if(M4Trk > 0){
      hInvMassTrk->Fill(M4);
    }

    //StandAlone Muons                                                                                                                                                                                      
    if (M3Sta > 0 && M4Sta > 0){

      if ( fabs(50.0 - M3Sta) < fabs(50.0 - M4Sta)){
        hInvMassSta->Fill(M3Sta);
      }else {
        hInvMassSta->Fill(M4Sta);
      }

    }else if(M3Sta > 0){
      hInvMassSta->Fill(M3Sta);
    }else if(M4Sta > 0){
      hInvMassSta->Fill(M4Sta);
    }

  //Global Muons                                                                                                                                                                                          
    if (M3Glb > 0 && M4Glb > 0){

      if (fabs(50.0 - M3Glb) < fabs(50.0 - M4Glb)){
        hInvMassGlb->Fill(M3Glb);
      }else {
        hInvMassGlb->Fill(M4Glb);
      }

    }else if(M3Glb > 0){
      hInvMassGlb->Fill(M3Glb);
    }else if(M4Glb > 0){
      hInvMassGlb->Fill(M4Glb);
    }

  Int_t a = 0;
  for (reco::MuonCollection::const_iterator mu1=muons->begin(); mu1!=muons->end(); ++mu1) {
      Int_t b = 0;
      for (reco::MuonCollection::const_iterator mu2=muons->begin(); mu2!=muons->end(); ++mu2) {
        Int_t c = 0;
        for (reco::MuonCollection::const_iterator mu3=muons->begin(); mu3!=muons->end(); ++mu3) {
          Int_t d = 0;
          for (reco::MuonCollection::const_iterator mu4=muons->begin(); mu4!=muons->end(); ++mu4) {

            if (a == ind_muon1 && b == ind_muon2 && c == ind_muon3 && d == ind_muon4){

              Float_t mass = (mu1->p4() + mu2->p4() + mu3->p4() + mu4->p4()).mass();
              hInvMassHiggs->Fill(mass);

              if (mu1->isTrackerMuon() && mu2->isTrackerMuon() && mu3->isTrackerMuon() && mu4->isTrackerMuon()){
                hInvMassTrkHiggs->Fill(mass);
              }


              if (mu1->isStandAloneMuon() && mu2->isStandAloneMuon() && mu3->isStandAloneMuon() && mu4->isStandAloneMuon()){
                hInvMassStaHiggs->Fill(mass);
              }

              if (mu1->isGlobalMuon() && mu2->isGlobalMuon() && mu3->isGlobalMuon() && mu4->isGlobalMuon()){
                hInvMassGlbHiggs->Fill(mass);
              }


            }

          }
          d++;
        }
        c++;
      }
      b++;
    }
    a++;
  }

}
DEFINE_FWK_MODULE(ExampleMuonAnalyzer2);
