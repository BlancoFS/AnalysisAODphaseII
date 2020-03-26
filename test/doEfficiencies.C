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
                       
                       
                       

void		SetLegend(TLegend* lt,
			            Size_t tsize);

//-----------------------------------------------------------------------------

//Constructor of the class.

void doEfficienciesND(TString name)
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
	DrawEfficiency("efficiency", "vxy", "gen production distance in xy [cm]");
	DrawEfficiency("efficiency", "vxy", "gen production distance in xy [cm]");
  DrawEfficiency("efficiency", "vxy", "gen production distance in xy [cm]");
	
	DrawEfficiency("efficiency", "vz", "gen production distance in z [cm]");
	DrawEfficiency("efficiency", "vz", "gen production distance in z [cm]");
	DrawEfficiency("efficiency", "vz", "gen production distance in z [cm]");
  DrawEfficiency("efficiency", "vz", "gen production distance in z [cm]");
  
  
  
  
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
  
  
  legend->Draw();

  // Save
  canvas->Modified();
  canvas->Update();

  if (doSavePdf) canvas->SaveAs(directory + "/" + filename + variable + "-" + effType + ".pdf");
  if (doSavePng) canvas->SaveAs(directory + "/" + filename + variable + "-" + effType + ".png");
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
  
  
  
  
  
  
  
  






