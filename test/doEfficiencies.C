//---------------------------------------------------------------------------
//
//     Create histograms of the samples.
//
//     root -l -b -q 'doEfficiencies.C+("sample")'
//---------------------------------------------------------------------------

#include "TCanvas.h"
#include "TFile.h"
#include "TFrame.h"
#include "TGraphAsymmErrors.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TInterpreter.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include "TString.h"
#include "TSystem.h"


//Data members:
Bool_t         doSavePdf = false;
Bool_t         doSavePng = true;


Bool_t         draw_sta   = true;
Bool_t         draw_trk   = true;
Bool_t         draw_glb   = true;
Bool_t         draw_tight = false;
Bool_t         draw_soft  = false;
Bool_t         draw_disp  = true;
Bool_t	       draw_dispSta = true;


TString        directory = "displaced-muons";

TFile*         file = NULL;


// Main arguments
//------------------------------------------------------------------------------
TString        filename;




//functions of the class: -------------------------------------------------------

TGraphAsymmErrors* MakeEfficiency(TString  effType,
				  TString  muonType,
				  TString  variable,
				  TFile*   file,
				  Color_t  color,
				  Style_t  style);



void 		DrawEfficiency(TString effType,
			       TString variable,
			       TString xtitle);

void               Compare(TString  hname,
			   TString  xtitle,
			   Bool_t   setlogy = true);
                       
                       
TH1F* 		AddOverflow(TH1F* hist);
                       

void		SetLegend(TLegend* lt,
			  Size_t tsize);

//-----------------------------------------------------------------------------

//Constructor of the class.

void doEfficiencies(TString name)
{
	
  filename = name;
	
  printf("\n");
  printf(" filename = %s\n", filename.Data());
  printf("\n");

  gInterpreter->ExecuteMacro("PaperStyle.C");
	
  if (doSavePdf) gSystem->mkdir(directory, kTRUE);
  if (doSavePng) gSystem->mkdir(directory, kTRUE);
  
  TH1::SetDefaultSumw2();
  
  file = TFile::Open("rootfiles/" + filename + ".root");
  
  
  DrawEfficiency("efficiency", "vxy", "gen production distance in xy [cm]");
  DrawEfficiency("efficiency", "vz", "gen production distance in z [cm]");
  DrawEfficiency("efficiency", "eta", "gen production vs eta");
  DrawEfficiency("efficiency", "pt", "gen production vs pt");
  DrawEfficiency("efficiency", "vr", "gen production distance in r [cm]");
	
	
	
  Compare("dxy", "gen dxy [cm]");
  Compare("dz",  "gen dz [cm]");
  Compare("vxy", "gen production distance in xy [cm]");
  Compare("vz",  "gen production distance in z [cm]");
  Compare("vr",  "gen production distance in xyz [cm]");
  Compare("pt",  "gen p_{T} [GeV]", false);
  Compare("MuPFIso",        "muon PF isolation");
  Compare("MuPFChargeIso",  "muon charged PF isolation");
  Compare("MuPFNeutralIso", "muon neutral PF isolation");
  Compare("MuPFPhotonIso",  "muon photon PF isolation");
  Compare("MuPFPUIso",      "muon PU PF isolation");
  if (draw_sta)   Compare("StaMuons_dR",   "#DeltaR(gen, standalone)");
  if (draw_trk)   Compare("TrkMuons_dR",   "#DeltaR(gen, tracker)");
  if (draw_glb)   Compare("GlbMuons_dR",   "#DeltaR(gen, global)");
  if (draw_tight) Compare("TightMuons_dR", "#DeltaR(gen, tight)");
  if (draw_soft)  Compare("SoftMuons_dR",  "#DeltaR(gen, soft)");
  if (draw_disp)  Compare("DispGlbMuons_dR", "#DeltaR(gen, displacedGlobal)");
  if (draw_dispSta) Compare("DispStaMuons_dR", "DeltaR(gen, displacedStandAlone)");
  
}



TGraphAsymmErrors* MakeEfficiency(TString effType,
				  TString muonType,
				  TString variable,
				  TFile*  file,
				  Color_t color,
				  Style_t style)
{
  TString num_name = "muonAnalysis/" + muonType + "Muons_";

  if (effType.Contains("fakes")) num_name = num_name + "noGen_";

  TString den_name = "muonAnalysis/GenMuons_";

  TH1F* hnum = (TH1F*)(file->Get(num_name + variable))->Clone("hnum");
  TH1F* hden = (TH1F*)(file->Get(den_name + variable))->Clone("hden");

	
  TGraphAsymmErrors* tgae = new TGraphAsymmErrors(hnum, hden);

  tgae->SetLineColor  (color);
  tgae->SetLineWidth  (    1);
  tgae->SetMarkerColor(color);
  tgae->SetMarkerStyle(style);

  return tgae;
}



//DrawEfficiencies function, represents the num and den of the efficiency's histogram.
void DrawEfficiency(TString effType,
		    TString variable,
		    TString xtitle)
{
  

  TGraphAsymmErrors* sta_efficiency   = MakeEfficiency(effType, "Sta",   variable, file, kBlack,    kFullCircle);
  TGraphAsymmErrors* trk_efficiency   = MakeEfficiency(effType, "Trk",   variable, file, kRed+1,    kFullCircle);
  TGraphAsymmErrors* glb_efficiency   = MakeEfficiency(effType, "Glb",   variable, file, kBlue,     kFullCircle);
  TGraphAsymmErrors* tight_efficiency = MakeEfficiency(effType, "Tight", variable, file, kGreen+2,  kFullCircle);
  TGraphAsymmErrors* soft_efficiency  = MakeEfficiency(effType, "Soft",  variable, file, kOrange+7, kFullCircle);
  TGraphAsymmErrors* dispGlb_efficiency = MakeEfficiency(effType, "DispGlb", variable, file, kBlue, kOpenCircle);
  TGraphAsymmErrors* dispSta_efficiency = MakeEfficiency(effType, "DispSta", variable, file, kBlack, kOpenCircle);
  
  
  // Canvas settings
  TCanvas* canvas = new TCanvas(effType + " vs. " + xtitle,
				effType + " vs. " + xtitle, 650, 600);

  canvas->SetLeftMargin (0.14);
  canvas->SetRightMargin(0.28);
  canvas->SetGridx();
  canvas->SetGridy();

  if (variable.EqualTo("vxy")) canvas->SetLogx();
  if (variable.EqualTo("vz"))  canvas->SetLogx();
  if (variable.EqualTo("vr"))  canvas->SetLogx();
  
  TMultiGraph* mg = new TMultiGraph();

  if (draw_sta) mg->Add(sta_efficiency);
  if (draw_trk) mg->Add(trk_efficiency);
  if (draw_glb) mg->Add(glb_efficiency);
  if (draw_tight) mg->Add(tight_efficiency);
  if (draw_soft) mg->Add(soft_efficiency);
  if (draw_disp) mg->Add(dispGlb_efficiency);
  if (draw_dispSta) mg->Add(dispSta_efficiency);
  
  
  mg->Draw("apz");

  mg->SetMinimum(-0.05);
  mg->SetMaximum( 1.05);

  // Labels
  mg->SetTitle("");
  mg->GetXaxis()->SetTitle(xtitle);
  mg->GetYaxis()->SetTitle(effType);
  mg->GetXaxis()->SetTitleOffset(1.5);
  mg->GetYaxis()->SetTitleOffset(1.6);

  // Legend
  TLegend* legend = new TLegend(0.74, 0.5, 0.90, 0.91);

  SetLegend(legend, 0.025);
  
  
  if (draw_sta) legend->AddEntry(sta_efficiency, "StandAloneMuons", "lp");
  if (draw_trk) legend->AddEntry(trk_efficiency, "trakerMuons", "lp");
  if (draw_glb) legend->AddEntry(glb_efficiency, "GlobalMuons", "lp");
  if (draw_tight) legend->AddEntry(tight_efficiency, " tight", "lp");
  if (draw_soft) legend->AddEntry(soft_efficiency, " soft", "lp");
  if (draw_disp) legend->AddEntry(dispGlb_efficiency, "DisplacedGlobalMuons", "lp");
  if (draw_dispSta) legend->AddEntry(dispSta_efficiency, "DisplacedStandAlone", "lp");
  
  
  legend->Draw();

  // Save
  canvas->Modified();
  canvas->Update();

  if (doSavePdf) canvas->SaveAs(directory + "/" + filename + variable + "-" + effType + ".pdf");
  if (doSavePng) canvas->SaveAs(directory + "/" + filename + variable + "-" + effType + ".png");
}




//------------------------------------------------------------------------------
//
// Compare
//
//------------------------------------------------------------------------------
void Compare(TString hname,
	     TString xtitle,
	     Bool_t  setlogy)
{
  TH1F* hist1 = NULL;

  hist1 = (TH1F*)(file->Get("muonAnalysis/" + hname))->Clone("hist1_" + hname);

  hist1->Scale(1. / hist1->Integral(-1, -1));
  hist2->Scale(1. / hist2->Integral(-1, -1));

  hist1->SetLineColor(kBlack);
  hist2->SetLineColor(kRed+1);

  hist1->SetLineWidth(2);
  hist2->SetLineWidth(2);


  // Draw
  //----------------------------------------------------------------------------
  TCanvas* canvas = new TCanvas("compare " + hname,
				"compare " + hname);

  canvas->SetLogy(setlogy);

  TH1F* hist1_overflow = AddOverflow(hist1);
  
  hist1_overflow->Draw("hist");
  hist1_overflow->GetXaxis()->SetTitle(xtitle);


  // Legend
  //----------------------------------------------------------------------------
  TLegend* legend = new TLegend(0.64, 0.78, 0.80, 0.89);

  SetLegend(legend, 0.03);

  legend->AddEntry(hist1_overflow, " " + filename, "l");

  legend->Draw();


  // Save
  //----------------------------------------------------------------------------
  canvas->GetFrame()->DrawClone();
  if (doSavePng) canvas->SaveAs(directory + "/" + filename + "__compare-" + hname + ".png");
  if (doSavePdf) canvas->SaveAs(directory + "/" + filename + "__compare-" + hname + ".pdf");
}







//------------------------------------------------------------------------------
//Add overflow
//------------------------------------------------------------------------------

TH1F* AddOverflow(TH1F* hist)
{
  TString  name = hist->GetName();
  Int_t    nx   = hist->GetNbinsX()+1;
  Double_t bw   = hist->GetBinWidth(nx);
  Double_t x1   = hist->GetBinLowEdge(1);
  Double_t x2   = hist->GetBinLowEdge(nx) + bw;
  
  // Book a new histogram having an extra bin for overflows
  TH1F* htmp = new TH1F(name + "_overflow", "", nx, x1, x2);

  // Fill the new histogram including the extra bin for overflows
  for (Int_t i=1; i<=nx; i++) {
    htmp->Fill(htmp->GetBinCenter(i), hist->GetBinContent(i));
  }

  // Fill the underflow
  htmp->Fill(x1-1, hist->GetBinContent(0));

  // Restore the number of entries
  htmp->SetEntries(hist->GetEffectiveEntries());

  // Cosmetics
  htmp->SetLineColor(hist->GetLineColor());
  htmp->SetLineWidth(hist->GetLineWidth());
  htmp->GetXaxis()->SetTitleOffset(1.5);

  return htmp;
}




//------------------------------------------------------------------------------
// Set legend
//------------------------------------------------------------------------------
void SetLegend(TLegend* tl,
	       Size_t   tsize)
{
  tl->SetBorderSize(    0);
  tl->SetFillColor (    0);
  tl->SetTextAlign (   12);
  tl->SetTextFont  (   42);
  tl->SetTextSize  (tsize);
}
  
  
  
  
  
  
  
  






