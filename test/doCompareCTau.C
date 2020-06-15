//---------------------------------------------------------------------------
//
//     Create histograms of the samples.
//
//     root -l -b -q 'doCompareCTau.C+("sample1", "sample2", "sample3", "sample4")'
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



TString        directory = "displaced-muons";

TFile*         file1 = NULL;
TFile*         file2 = NULL;
TFile*         file3 = NULL;
TFile*         file4 = NULL;


// Main arguments
//------------------------------------------------------------------------------

TString        file1name;
TString        file2name;
TString        file3name;
TString        file4name;


//-----------------------------------------------------


void     Compare(TString  hname,
			           TString  xtitle,
			           Bool_t   setlogy = true);
                       
                       
TH1F*  	AddOverflow(TH1F* hist);
                       

void		SetLegend(TLegend* lt,
			            Size_t tsize);


//-----------------------------------------------------------------------------

//Constructor of the class.

void doCompareCTau(TString name1, 
                    TString name2,
                    TString name3,
                    TString name4)
{

  file1name = name1;
  file2name = name2;
  file3name = name3;
  file4name = name4;
	
  printf("\n");
  printf(" file1name = %s\n", file1name.Data());
  printf(" file2name = %s\n", file2name.Data());
  printf(" file3name = %s\n", file3name.Data());
  printf(" file4name = %s\n", file4name.Data());
  printf("\n");

  gInterpreter->ExecuteMacro("PaperStyle.C");
	
  if (doSavePdf) gSystem->mkdir(directory, kTRUE);
  if (doSavePng) gSystem->mkdir(directory, kTRUE);
  
  TH1::SetDefaultSumw2();
  
  file1 = TFile::Open("rootfiles/DisplacedSUSY_" + file1name + ".root");
  file2 = TFile::Open("rootfiles/DisplacedSUSY_" + file2name + ".root");
  file3 = TFile::Open("rootfiles/DisplacedSUSY_" + file3name + ".root");
  file4 = TFile::Open("rootfiles/DisplacedSUSY_" + file4name + ".root");
  
  
  Compare("vxy", "gen production distance in xy [cm]");
  Compare("vz",  "gen production distance in z [cm]");
  Compare("vr",  "gen production distance in xyz [cm]");
  
  
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
  TH1F* hist2 = NULL;
  TH1F* hist3 = NULL;
  TH1F* hist4 = NULL;
  
  
  hist1 = (TH1F*)(file1->Get("muonAnalysis/" + hname))->Clone("hist1_" + hname);
  hist2 = (TH1F*)(file2->Get("muonAnalysis/" + hname))->Clone("hist2_" + hname);
  hist3 = (TH1F*)(file3->Get("muonAnalysis/" + hname))->Clone("hist3_" + hname);
  hist4 = (TH1F*)(file4->Get("muonAnalysis/" + hname))->Clone("hist4_" + hname);
  
  
  hist1->Scale(1. / hist1->Integral(-1, -1));
  hist2->Scale(1. / hist2->Integral(-1, -1));
  hist3->Scale(1. / hist3->Integral(-1, -1));
  hist4->Scale(1. / hist4->Integral(-1, -1));


  hist1->SetLineColor(kBlack);
  hist2->SetLineColor(kBlack+2);
  hist3->SetLineColor(kBlack+4);
  hist4->SetLineColor(kBlack+6);


  hist1->SetLineWidth(2);
  hist2->SetLineWidth(2);
  hist3->SetLineWidth(2);
  hist4->SetLineWidth(2);



  // Draw
  //----------------------------------------------------------------------------
  TCanvas* canvas = new TCanvas("compare " + hname,
				"compare " + hname);

  canvas->SetLogy(setlogy);

  TH1F* hist1_overflow = AddOverflow(hist1);
  TH1F* hist2_overflow = AddOverflow(hist2);
  TH1F* hist3_overflow = AddOverflow(hist3);
  TH1F* hist4_overflow = AddOverflow(hist4);


  if (hist1_overflow->GetMaximum() > hist2_overflow->GetMaximum() && hist1_overflow->GetMaximum() > hist3_overflow->GetMaximum() && hist1_overflow->GetMaximum() > hist4_overflow->GetMaximum())
    {
      hist1_overflow->Draw("hist");
      hist2_overflow->Draw("hist,same");
      hist3_overflow->Draw("hist,same");
      hist4_overflow->Draw("hist,same");
      hist1_overflow->GetXaxis()->SetTitle(xtitle);
    }
  else if(hist2_overflow->GetMaximum() > hist1_overflow->GetMaximum() && hist2_overflow->GetMaximum() > hist3_overflow->GetMaximum() && hist2_overflow->GetMaximum() > hist4_overflow->GetMaximum())
    {
      hist2_overflow->Draw("hist");
      hist1_overflow->Draw("hist,same");
      hist3_overflow->Draw("hist,same");
      hist4_overflow->Draw("hist,same");
      hist2_overflow->GetXaxis()->SetTitle(xtitle);
    }
   else if(hist3_overflow->GetMaximum() > hist1_overflow->GetMaximum() && hist3_overflow->GetMaximum() > hist2_overflow->GetMaximum() && hist3_overflow->GetMaximum() > hist4_overflow->GetMaximum())
    {
      hist3_overflow->Draw("hist");
      hist1_overflow->Draw("hist,same");
      hist2_overflow->Draw("hist,same");
      hist4_overflow->Draw("hist,same");
      hist3_overflow->GetXaxis()->SetTitle(xtitle);
    }
   else if(hist4_overflow->GetMaximum() > hist1_overflow->GetMaximum() && hist4_overflow->GetMaximum() > hist2_overflow->GetMaximum() && hist4_overflow->GetMaximum() > hist3_overflow->GetMaximum())
    {
      hist4_overflow->Draw("hist");
      hist1_overflow->Draw("hist,same");
      hist2_overflow->Draw("hist,same");
      hist3_overflow->Draw("hist,same");
      hist4_overflow->GetXaxis()->SetTitle(xtitle);
    }

  // Legend
  //----------------------------------------------------------------------------
  TLegend* legend = new TLegend(0.64, 0.78, 0.80, 0.89);

  SetLegend(legend, 0.03);

  legend->AddEntry(hist1_overflow, " " + file1name, "l");
  legend->AddEntry(hist2_overflow, " " + file2name, "l");
  legend->AddEntry(hist3_overflow, " " + file3name, "l");
  legend->AddEntry(hist4_overflow, " " + file4name, "l");

  legend->Draw();

  // Save
  //----------------------------------------------------------------------------
  canvas->GetFrame()->DrawClone();

  if (doSavePdf) canvas->SaveAs(directory + "/" + file1name + "__vs__" + file2name + "__compare-" + hname + ".pdf");
  if (doSavePng) canvas->SaveAs(directory + "/" + file1name + "__vs__" + file2name + "__compare-" + hname + ".png");
}



//------------------------------------------------------------------------------
//
// Add overflow
//
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
