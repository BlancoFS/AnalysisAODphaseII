//------------------------------------------------------------------------------
//
// Produce and compare displaced muons resolution
//
// root -l -b -q 'doResolution.C+("Sample")'
//
//------------------------------------------------------------------------------
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TFrame.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TInterpreter.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include "TString.h"
#include "TSystem.h"


// Data members
//------------------------------------------------------------------------------
const Int_t    nbins_pt = 6;

const Double_t pt_bins[nbins_pt+1] = {10, 60, 90, 130, 170, 250, 500};

Color_t        ptcolor[nbins_pt] = {kRed-10, kRed-9, kRed-7, kRed-4, kRed, kRed+1};

Bool_t         doSavePdf = false;
Bool_t         doSavePng = true;

Bool_t         draw_sta   = false;
Bool_t         draw_trk   = true;
Bool_t         draw_glb   = true;
Bool_t         draw_tight = false;
Bool_t         draw_soft  = false;
Bool_t         draw_disp  = true;
Bool_t         draw_dispSta = true;

TString        directory = "displaced-muons";

TFile*         file = NULL;

TLegend*       resolution_legend = NULL;

TMultiGraph*   mg_mean  = NULL;
TMultiGraph*   mg_width = NULL;


// Main arguments
//------------------------------------------------------------------------------
TString        filename;


// Member functions
//------------------------------------------------------------------------------
void          DrawMultiGraph(TMultiGraph* mg,
			     TString      title);

void          DrawResolution(TString      muonType,
			     TString      xtitle,
			     TString      filename,
			     Color_t      color
                             Style_t      gr_style);


void 	      DrawCompare(TString filename);


void          SetLegend     (TLegend*     tl,
			     Size_t       tsize);

TGraphErrors* SetGraph      (Int_t        npoints,
			     Color_t      color,
			     Style_t      style);

void          DrawLatex     (Font_t       tfont,
			     Float_t      x,
			     Float_t      y,
			     Float_t      tsize,
			     Short_t      align,
			     const char*  text,
			     Bool_t       setndc = true);


//------------------------------------------------------------------------------
//
// doResolution
//
//------------------------------------------------------------------------------
void doResolution(TString name)
{
  filename = name;

  printf("\n");
  printf(" filename = %s\n", filename.Data());
  printf("\n");

  gInterpreter->ExecuteMacro("PaperStyle.C");

  if (doSavePdf) gSystem->mkdir(directory, kTRUE);
  if (doSavePng) gSystem->mkdir(directory, kTRUE);

  TH1::SetDefaultSumw2();


  // Input files
  //----------------------------------------------------------------------------
  file = TFile::Open("rootfiles/" + filename + ".root");


  // Do the work
  //----------------------------------------------------------------------------
  mg_mean  = new TMultiGraph();
  mg_width = new TMultiGraph();

  resolution_legend = new TLegend(0.74, 0.5, 0.95, 0.91);

  SetLegend(resolution_legend, 0.025);

  if (draw_sta)   DrawResolution("Sta",   "standalone muons", filename, kBlack, kFullCircle);
  if (draw_trk)   DrawResolution("Trk",   "tracker muons",    filename, kRed+1, kFullCircle);
  if (draw_glb)   DrawResolution("Glb",   "global muons",     filename, kBlue,  kFullCirlce);
  if (draw_tight) DrawResolution("Tight", "tight muons",      filename, kGreen+2, kFullCircle);
  if (draw_soft)  DrawResolution("Soft",  "soft muons",       filename, kOrange+7, kFullCircle);
  if (draw_disp)  DrawResolution("DispGlb", "displacedGlobal", filename, kBlue, kOpenCircle);
  if (draw_dispSta) DrawResolution("DispSta", "displacedStandAlone", filename, kBlack, kOpenCircle);
	
  DrawCompare(filename);


  DrawMultiGraph(mg_mean,  "mean");
  DrawMultiGraph(mg_width, "width");
}


//------------------------------------------------------------------------------
// Draw MultiGraph
//------------------------------------------------------------------------------
void DrawMultiGraph(TMultiGraph* mg,
		    TString      title)
{
  TCanvas* canvas = new TCanvas(title, title, 650, 600);

  canvas->SetLeftMargin (0.14);
  canvas->SetRightMargin(0.28);
  canvas->SetGridx();
  canvas->SetGridy();

  mg->Draw("apz");

  mg->GetXaxis()->SetTitle("gen p_{T} [GeV]");
  mg->GetYaxis()->SetTitle("");

  DrawLatex(42, 0.43, 0.95, 0.04, 21, Form("#Deltaq/p_{T} / (q/p_{T}) fit %s", title.Data()));

  mg->GetXaxis()->SetTitleOffset(1.6);
  mg->GetYaxis()->SetTitleOffset(2.1);

  resolution_legend->Draw("same");

  if (doSavePdf) canvas->SaveAs(directory + "/resolution_" + title + ".pdf");
  if (doSavePng) canvas->SaveAs(directory + "/resolution_" + title + ".png");
}


//------------------------------------------------------------------------------
//
// Draw resolution
//
//------------------------------------------------------------------------------
void DrawResolution(TString muonType,
		    TString xtitle,
		    TString filename,
		    Color_t color
        Style_t gr_style)
{
  
  TCanvas* canvas = new TCanvas(xtitle + " resolution (" + filename + ")",
				xtitle + " resolution (" + filename + ")");
  
  // One histogram per pt bin
  TH1F* h_resolution[nbins_pt];


  TGraphErrors* gr_mean  = SetGraph(nbins_pt, color, gr_style);
  TGraphErrors* gr_width = SetGraph(nbins_pt, color, gr_style);

  // Legend
  TLegend* legend = new TLegend(0.61, 0.6, 0.82, 0.89);

  SetLegend(legend, 0.03);

  // Vertical maximum
  Float_t ymax = 0;


  // Loop over pt bins
  //----------------------------------------------------------------------------
  for (Int_t i=0; i<nbins_pt; i++) {

    TH2F* h2 = (TH2F*)file->Get(Form("muonAnalysis/%sMuons_pt_resolution_pt%d", muonType.Data(), i));

    TString hname = Form("%s_%s_vxy", h2->GetName(), filename.Data());

    h_resolution[i] = (TH1F*)h2->ProjectionY(hname);

    TF1* gfit = new TF1("gfit", "gaus", -0.035, 0.035);

    gfit->SetParameters(1,0,1);

    h_resolution[i]->Fit("gfit", "nqr");
    
    Float_t mean_value  = gfit->GetParameter(1);
    Float_t width_value = gfit->GetParameter(2);

    Float_t mean_error  = gfit->GetParError(1);
    Float_t width_error = gfit->GetParError(2);

    gr_mean ->SetPoint(i, 0.5*(pt_bins[i]+pt_bins[i+1]), mean_value);
    gr_width->SetPoint(i, 0.5*(pt_bins[i]+pt_bins[i+1]), width_value);

    gr_mean ->SetPointError(i, 0.5*(pt_bins[i+1]-pt_bins[i]), mean_error);
    gr_width->SetPointError(i, 0.5*(pt_bins[i+1]-pt_bins[i]), width_error);

    if (h_resolution[i]->GetMaximum() > ymax) ymax = h_resolution[i]->GetMaximum();

    h_resolution[i]->SetLineColor(ptcolor[i]);
    h_resolution[i]->SetLineWidth(2);

    TString option = (i == 0) ? "hist" : "hist,same";

    h_resolution[i]->Draw(option);

    legend->AddEntry(h_resolution[i], Form(" %.0f < p_{T} < %.0f GeV", pt_bins[i], pt_bins[i+1]), "l");
  }


  // Save the mean and the width
  mg_mean ->Add(gr_mean);
  mg_width->Add(gr_width);

  // Legend
  if (muonType.EqualTo("Sta")   && draw_sta)   resolution_legend->AddEntry(gr_mean, "(" + filename + ") sta",   "lp");
  if (muonType.EqualTo("Trk")   && draw_trk)   resolution_legend->AddEntry(gr_mean, "(" + filename + ") trk",   "lp");
  if (muonType.EqualTo("Glb")   && draw_glb)   resolution_legend->AddEntry(gr_mean, "(" + filename + ") glb",   "lp");
  if (muonType.EqualTo("Tight") && draw_tight) resolution_legend->AddEntry(gr_mean, "(" + filename + ") tight", "lp");
  if (muonType.EqualTo("Soft")  && draw_soft)  resolution_legend->AddEntry(gr_mean, "(" + filename + ") soft",  "lp");
  if (muonType.EqualTo("DispGlb") && draw_disp) resolution_legend->AddEntry(gr_mean, "(" + filename +") dispGlb", "lp");
  if (muonType.EqualTo("DispSta") && draw_dispSta) resolution_legend->AddEntry(gr_mean, "(" + filename +") dispSta", "lp");

  // Cosmetics
  h_resolution[0]->SetMaximum(1.1 * ymax);
  h_resolution[0]->SetTitle("");
  h_resolution[0]->SetXTitle(xtitle + " #Deltaq/p_{T} / (q/p_{T})");
  h_resolution[0]->SetYTitle("entries / bin");
  h_resolution[0]->GetXaxis()->SetTitleOffset(1.5);
  h_resolution[0]->GetYaxis()->SetTitleOffset(2.0);

  DrawLatex(42, 0.940, 0.945, 0.04, 31, filename);

  legend->Draw();

  canvas->GetFrame()->DrawClone();

  // Save
  if (doSavePdf) canvas->SaveAs(directory + "/resolution_" + muonType + "_" + filename + ".pdf");
  if (doSavePng) canvas->SaveAs(directory + "/resolution_" + muonType + "_" + filename + ".png");
}



//-------------------------------------------------------------
//Draw compare histograms:
//-------------------------------------------------------------


void DrawCompare(TString filename)
{
	
	TH1F* h_resolution1[nbins_pt];
	TH1F* h_resolution2[nbins_pt];
	TH1F* h_resolution3[nbins_pt];
	TH1F* h_resolution4[nbins_pt];
	TH1F* h_resolution5[nbins_pt];

	
	for (Int_t i=0; i<nbins_pt; i++) {
		
		TCanvas* canvas = new TCanvas(Form(" resolution%d (" + filename + ")",i),
                                  Form(" resolution%d (" + filename + ")",i));
		
		Float_t ymax = 0;
		
		TH2F* hm1 = (TH2F*)file->Get(Form("muonAnalysis/StaMuons_pt_resolution_pt%d", i));
		TString hname1 = Form("%s_%s_vxy", hm1->GetName(), filename.Data());
		h_resolution1[i] = (TH1F*)hm1->ProjectionY(hname1); 
		
		TH2F* hm2 = (TH2F*)file->Get(Form("muonAnalysis/TrkMuons_pt_resolution_pt%d", i));
		TString hname2 = Form("%s_%s_vxy", hm2->GetName(), filename.Data());
		h_resolution2[i] = (TH1F*)hm2->ProjectionY(hname2);
		
		TH2F* hm3 = (TH2F*)file->Get(Form("muonAnalysis/GlbMuons_pt_resolution_pt%d", i));
		TString hname3 = Form("%s_%s_vxy", hm3->GetName(), filename.Data());
		h_resolution3[i] = (TH1F*)hm3->ProjectionY(hname3);
		
		TH2F* hm4 = (TH2F*)file->Get(Form("muonAnalysis/DispGlbMuons_pt_resolution_pt%d", i));
		TString hname4 = Form("%s_%s_vxy", hm4->GetName(), filename.Data());
		h_resolution4[i] = (TH1F*)hm4->ProjectionY(hname4);
		
		TH2F* hm5 = (TH2F*)file->Get(Form("muonAnalysis/DispStaMuons_pt_resolution_pt%d", i));
		TString hname5 = Form("%s_%s_vxy", hm5->GetName(), filename.Data());
		h_resolution5[i] = (TH1F*)hm5->ProjectionY(hname5);
		
		
		
		Float_t max[5] = {h_resolution1[i]->GetMaximum(), h_resolution2[i]->GetMaximum(), h_resolution3[i]->GetMaximum(), h_resolution4[i]->GetMaximum(), h_resolution5[i]->GetMaximum()};
		
		for (Int_t k = 0; k< 5; k++){
			
			if (max[k] > ymax){
				ymax = max[k];
			}
		}
		
		h_resolution1[i]->SetLineColor(kBlack);
		h_resolution2[i]->SetLineColor(kRed);
		h_resolution3[i]->SetLineColor(kBlue);
		h_resolution4[i]->SetLineColor(kGreen);
		h_resolution5[i]->SetLineColor(kOrange);
		
		h_resolution1[i]->SetLineWidth(2);
		h_resolution2[i]->SetLineWidth(2);
		h_resolution3[i]->SetLineWidth(2);
		h_resolution4[i]->SetLineWidth(2);
		h_resolution5[i]->SetLineWidth(2);
		
		TMultiGraph* mg = new TMultiGraph();
		
		TGraphAsymmErrors* hist1 = new TGraphAsymmErrors(h_resolution1[i]);
		TGraphAsymmErrors* hist2 = new TGraphAsymmErrors(h_resolution2[i]);
		TGraphAsymmErrors* hist3 = new TGraphAsymmErrors(h_resolution3[i]);
		TGraphAsymmErrors* hist4 = new TGraphAsymmErrors(h_resolution4[i]);
		TGraphAsymmErrors* hist5 = new TGraphAsymmErrors(h_resolution5[i]);
		
		if (draw_sta) mg->Add(hist1);
		if (draw_trk) mg->Add(hist2);
		if (draw_glb) mg->Add(hist3);
		if (draw_disp) mg->Add(hist4);
		if (draw_dispSta) mg->Add(hist5);
		
		mg->Draw("apz");
		
		mg->SetMinimum(0.0);
		mg->SetMaximum(ymax);
		
		mg->SetTitle("");
  		mg->GetXaxis()->SetTitle(Form(" %.0f < p_{T} < %.0f GeV: #Deltaq/p_{T} / (q/p_{T})", pt_bins[i], pt_bins[i+1])));
  		mg->GetYaxis()->SetTitle("entries / bin");
 		mg->GetXaxis()->SetTitleOffset(1.5);
 		mg->GetYaxis()->SetTitleOffset(2.0);
		
		TLegend* legend = new TLegend(0.74, 0.5, 0.90, 0.91);

  		SetLegend(legend, 0.025);
		
		if (draw_sta) legend->AddEntry(hist1, "Sta", "lp");
		if (draw_trk) legend->AddEntry(hist2, "Trk", "lp");
		if (draw_glb) legend->AddEntry(hist3, "Glb", "lp");
		if (draw_disp) legend->AddEntry(hist4, "DispGlb", "lp");
		if (draw_dispSta) legend->AddEntry(hist5, "DispSta", "lp");
		
		legend->Draw();

  		// Save
 		canvas->Modified();
  		canvas->Update();
		
		if (doSavePng) canvas->SaveAs(Form(directory + "/resolution_%d" + "_" + filename + ".png",i));
		
		
	}



}




//------------------------------------------------------------------------------
// SetLegend
//------------------------------------------------------------------------------
void SetLegend(TLegend* tl, Size_t tsize)
{
  tl->SetBorderSize(    0);
  tl->SetFillColor (    0);
  tl->SetTextAlign (   12);
  tl->SetTextFont  (   42);
  tl->SetTextSize  (tsize);
}


//------------------------------------------------------------------------------
// SetGraph
//------------------------------------------------------------------------------
TGraphErrors* SetGraph(Int_t   npoints,
		       Color_t color,
		       Style_t style)
{
  TGraphErrors* gr = new TGraphErrors(npoints);

  gr->SetLineColor  (color);
  gr->SetLineWidth  (1    );
  gr->SetMarkerColor(color);
  gr->SetMarkerStyle(style);

  return gr;
}


//------------------------------------------------------------------------------
// DrawLatex
//------------------------------------------------------------------------------
void DrawLatex(Font_t      tfont,
	       Float_t     x,
	       Float_t     y,
	       Float_t     tsize,
	       Short_t     align,
	       const char* text,
	       Bool_t      setndc)
{
  TLatex* tl = new TLatex(x, y, text);

  tl->SetNDC      (setndc);
  tl->SetTextAlign( align);
  tl->SetTextFont ( tfont);
  tl->SetTextSize ( tsize);

  tl->Draw("same");
}
