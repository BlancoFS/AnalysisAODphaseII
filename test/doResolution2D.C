//------------------------------------------------------------------------------
//
// Produce 2D efficiencies
//
// root -l -b -q 'doEfficiencies2D.C+("Hit/chi2", "sample")'
//
//------------------------------------------------------------------------------
#include "TCanvas.h"
#include "TFile.h"
#include "TFrame.h"
#include "TH2F.h"
#include "TInterpreter.h"
#include "TLatex.h"
#include "TPaletteAxis.h"
#include "TString.h"
#include "TSystem.h"

#include <fstream>








// Data members
//------------------------------------------------------------------------------
Float_t        bigLabelSize = 0.04;

TString        directory = "displaced-muons";


// Member functions
//------------------------------------------------------------------------------
void AxisFonts    (TAxis*      axis,
		   TString     title);

void TH2FAxisFonts(TH2F*       h,
		   TString     coordinate,
		   TString     title);

void DrawTLatex   (Font_t      tfont,
		   Double_t    x,
		   Double_t    y,
		   Double_t    tsize,
		   Short_t     align,
		   const char* text,
		   Bool_t      setndc = true);


void doResolution2D(TString type,
                    TString filename)
{
  
  gInterpreter->ExecuteMacro("PaperStyle.C");

  gSystem->mkdir(directory + "/tcl", kTRUE);
  
  TH1::SetDefaultSumw2();
  TH2::SetDefaultSumw2();
  
  TFile* inputfile = TFile::Open("rootfiles/" + filename + ".root");


  TH2F* hist = (TH2F*)inputfile->Get("muonAnalysis/" + "DispSta_pt_resolution_" + type);
  
  TString hname = "DispSta_pt_resolution_" + type + filename;
  
  hist->SetDirectory(0);
  hist->SetTitle("");
  
  TCanvas* canvas = new TCanvas(hname, hname);

  canvas->SetLeftMargin (0.9 * canvas->GetLeftMargin());
  canvas->SetRightMargin(3.5 * canvas->GetRightMargin());
  canvas->SetTopMargin  (1.2 * canvas->GetTopMargin());

  TH2FAxisFonts(hist, "x", "pt_resolution");
  TH2FAxisFonts(hist, "y", type);
  
  hist->Draw("colz");
  
  Double_t hmin = hist->GetMinimum();
  Double_t hmax = hist->GetMaximum();
  
  
  for (Int_t i=1; i<=hist->GetNbinsX(); i++) {
      for (Int_t j=1; j<=hist->GetNbinsY(); j++) {

	      Double_t value = h->GetBinContent(i,j);
      
	      Double_t ypos = yaxis->GetBinCenter(j);
	      Double_t xpos = xaxis->GetBinCenter(i);
      
	      TLatex* latex = new TLatex(xpos, ypos, Form("%.2f", value));
      
	      latex->SetTextAlign(   22);
	      latex->SetTextFont (   42);
	      latex->SetTextSize (0.027);

	      if (value < hmin + 0.3*(hmax - hmin)) latex->SetTextColor(kWhite);
	
	      latex->Draw();
      }
  }
  
  canvas->Update();

  TPaletteAxis* palette = (TPaletteAxis*)h->GetListOfFunctions()->FindObject("palette");

  palette->SetLabelFont(42);
  
  canvas->Update();
  
  canvas->Modified();

  canvas->GetFrame()->DrawClone();

  canvas->SaveAs(directory + "/" + hname + ".png");
    
}


//------------------------------------------------------------------------------
// AxisFonts
//------------------------------------------------------------------------------
void AxisFonts(TAxis*  axis,
	       TString title)
{
  axis->SetLabelFont  (   42);
  axis->SetLabelOffset(0.015);
  axis->SetNdivisions (  505);
  axis->SetTitleFont  (   42);
  axis->SetTitleOffset(  1.5);
  axis->SetLabelSize  (bigLabelSize);
  axis->SetTitleSize  (bigLabelSize);

  axis->SetTitle(title);
}
  


//------------------------------------------------------------------------------
// TH2FAxisFonts
//------------------------------------------------------------------------------
void TH2FAxisFonts(TH2F*   h,
		   TString coordinate,
		   TString title)
{
  TAxis* axis = NULL;

  if (coordinate.Contains("x")) axis = h->GetXaxis();
  if (coordinate.Contains("y")) axis = h->GetYaxis();

  AxisFonts(axis, title);
}



//------------------------------------------------------------------------------
// DrawTLatex
//------------------------------------------------------------------------------
void DrawTLatex(Font_t      tfont,
		Double_t    x,
		Double_t    y,
		Double_t    tsize,
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




















