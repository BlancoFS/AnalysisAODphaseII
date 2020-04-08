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
#include "THStack.h"


// Data members(Atributes)                                                                                                                                                                                  
//------------------------------------------------------------------------------                                                                                                                            
Bool_t         doSavePdf = true;
Bool_t         doSavePng = true;


TString        directory = "displaced-muons";

//This is the file name, the main argument.                                                                                                                                                                 
TString        filename;

TFile*          file = NULL;


//functions of the class: -------------------------------------------------------                                                                                                                           
void            DrawV(TString filePath);

void            SetLegend(TLegend* tl,
                          Size_t tsize);
//-----------------------------------------------------------------------------      



void doVertexType(TString name)
{

    filename = name;

    printf("\n");
    printf(" filename = %s\n", filename.Data());
    printf("\n");

    gInterpreter->ExecuteMacro("PaperStyle.C");

    if (doSavePdf) gSystem->mkdir(directory, kTRUE);
    if (doSavePng) gSystem->mkdir(directory, kTRUE);

    file = TFile::Open("rootfiles/" + filename + ".root");


    DrawV("muonAnalysis/typeOfVertex");

}

void DrawV(TString filePath)
{


  TH1* mg =(TH1*)(file->Get(filePath))->Clone("mg");


  TCanvas* canvas = new TCanvas("Vertex type", "Vertex type", 650, 600);
  canvas->SetLeftMargin (0.14);
  canvas->SetRightMargin(0.28);


  mg->SetOption("BAR");

  mg->Draw();

  mg->SetTitle("");
  mg->GetYaxis()->SetTitle("Events");
  mg->GetYaxis()->SetTitleOffset(2.0);

  mg->SetLineWidth(0);
  mg->SetBarWidth(0.45);
  mg->SetBarOffset(0.1);
  mg->SetFillColor(4);
  mg->GetXaxis()->SetBinLabel(1, "Primary vertex");
  mg->GetXaxis()->SetBinLabel(2, "Beam Spot");
  mg->SetStats(0);

  TLegend* legend = new TLegend(0.74, 0.5, 0.90, 0.91);

  SetLegend(legend, 0.025);

  legend->AddEntry(mg, filename, "lp");

  int bin1 = (int) mg->GetBinContent(1);
  int bin2 = (int) mg->GetBinContent(2);

  TString b1 = to_string(bin1);
  TString b2 = to_string(bin2);


  legend->AddEntry(mg, "Entries(P.V.): " + b1, "lp");
  legend->AddEntry(mg, "Entries(B.S.): " + b2, "lp");


  legend->Draw();

  canvas->Modified();
  canvas->Update();

  if (doSavePng) canvas->SaveAs(directory + "/" + filename
                                      + "__" + "vertexType" + ".png");

}


void SetLegend(TLegend* tl, Size_t tsize)
{
  tl->SetBorderSize(0);
  tl->SetFillColor(0);
  tl->SetTextAlign(12);
  tl->SetTextFont(42);
  tl->SetTextSize(tsize);

}
