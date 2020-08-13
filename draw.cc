#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <TTree.h>
#include "TFile.h"
#include "TMath.h"
#include <TGraph.h>
#include <TNtupleD.h>
#include <TCanvas.h>
#include <TAxis.h>

using namespace std;
main(int argc,char *argv[]){
   int month=atoi(argv[1]);
   int day=atoi(argv[2]);
   int iTel=atoi(argv[3]);
   //int ich=atoi(argv[4]);

   TFile *file = new TFile(Form("/scratchfs/ybj/chensuh/2020/EventShow/2020/%02d%02d.WFCTA%02d.root",month,day,iTel));
   TFile *fcloud = new TFile(Form("/scratchfs/ybj/chensuh/2020/cloudmapdata/temp_30s/outFile/data/separate/2020_%02d_%02d_%02d.root",month,day,iTel));

   TTree *tree = (TTree*)file->Get("Base");
   TTree *tcloud = (TTree*)fcloud->Get("t");

   int itel;
   Long64_t rbT0;
   float base[1024]= {0};
   float base_deriv[1024] = {0};
   int timestep = 30*30;
   int timestep1 = 30*2;
   float BaseValue,Base1stDer,Base2ndDer;
   double value1,value2,value3;
   double sumbase1 = 0;
   double sumbase2 = 0;
   double sumbase3 = 0;
   double time_stamp;
   float slow_control_stu,temperature_stu,daq_stu,cloud_stu,phase,delta_angle;

   tree->SetBranchAddress("itel",&itel);
   tree->SetBranchAddress("time",&rbT0);
   tree->SetBranchAddress("basevalue",base);

   tcloud->SetBranchAddress("time_stamp",&time_stamp);
   tcloud->SetBranchAddress("slow_control_stu",&slow_control_stu);
   tcloud->SetBranchAddress("temperature_stu",&temperature_stu);
   tcloud->SetBranchAddress("daq_stu",&daq_stu);
   tcloud->SetBranchAddress("cloud_stu",&cloud_stu);
   tcloud->SetBranchAddress("phase",&phase);
   tcloud->SetBranchAddress("delta_angle",&delta_angle);
	
   TGraph *gr = new TGraph();
   TGraph *gr1 = new TGraph();
   TGraph *gr2 = new TGraph();
   TGraph *gr3 = new TGraph();	
   TGraph *gr4 = new TGraph();
   TGraph *gr5 = new TGraph();

   int npots=tree->GetEntries();
   int npotcloud=tcloud->GetEntries();
   double stu1,stu2,stu3;
   double topen[3] = {0};
   double tclose[3] = {0};
   int ipoints = 0;
   int opentime = 0;
   int closetime = 0;
   const int gwcloudtemp = -70;
   int ict = 0;
   /* cut open door stu */
   for(int i=1;i<npotcloud-1;i++)
   {
      tcloud->GetEntry(i-1);
      stu1=slow_control_stu;
      tcloud->GetEntry(i);
      stu2=slow_control_stu;
      tcloud->GetEntry(i+1);
      stu3=slow_control_stu;
      if(stu2-stu1>0)
      {
         topen[opentime]=time_stamp+1800;//after opendoor 30min
         opentime++;
      }      
      if(stu2-stu3>0)
      {
         tclose[closetime]=time_stamp-1800;//after opendoor 30min
         closetime++;
      }
   }

   for(int i =1;i<npots-1;i++)
   {
      tree->GetEntry(i-1);
      double sumbase =0 ;
      for(int ch1=0;ch1<1024;ch1++)
         sumbase+=base[ch1];
      value1=sumbase/1024;        

      tree->GetEntry(i);
      sumbase =0 ;
      for(int ch2=0;ch2<1024;ch2++)
         sumbase+=base[ch2];
      value2=sumbase/1024;
      BaseValue=value2;

      tree->GetEntry(i+1);
      sumbase = 0;
      for(int ch3=0;ch3<1024;ch3++)
         sumbase+=base[ch3];
      value3=sumbase/1024;				

      Base1stDer = (value3-value1)/timestep1;	        
      Base2ndDer = (value3-2*value2+value1)/timestep;
      
      for(int ic =1;ic<npotcloud-1;ic++)
      {
         tcloud->GetEntry(ic);
       //  if(cloud_stu==100) continue;  
         for(int iot=0;iot<opentime;iot++)
         {
            if(time_stamp>=topen[iot]&&time_stamp<=tclose[iot])
            {
               if(cloud_stu==100) continue;
               if(fabs(rbT0-time_stamp)<30)
               {
                  gr->SetPoint(ipoints,rbT0,BaseValue);
                  gr1->SetPoint(ipoints,rbT0,Base1stDer);
                  gr2->SetPoint(ipoints,rbT0,Base2ndDer);
                  gr3->SetPoint(ipoints,rbT0,cloud_stu);
                  gr4->SetPoint(ipoints,rbT0,gwcloudtemp);
                  gr5->SetPoint(ipoints,cloud_stu,Base2ndDer);
                  ipoints++;
               }
            }
         }

      }
   }

  /* for(int i =0;i<npotcloud;i++)
        {
                tcloud->GetEntry(i);
                if(cloud_stu==100) continue;
                gr3->SetPoint(ict,time_stamp,cloud_stu);
                gr4->SetPoint(ict,time_stamp,gwcloudtemp);
                ict++;
        }
   
*/
   TCanvas *c = new TCanvas("c");   
   gr->SetMarkerStyle(21);
   gr->SetMarkerColor(1);
   gr->SetMarkerSize(0.25);
   gr->SetTitle(Form("2020.%02d.%02d.WFCTA%02d.allch.BaseValue",month,day,iTel));
   gr->GetXaxis()->SetTitle("rbtime/s");
   gr->GetYaxis()->SetTitle("BaseValue");
   gr->GetXaxis()->CenterTitle();
   gr->GetYaxis()->CenterTitle();
   gr->Draw("ap");

   TCanvas *c1 = new TCanvas("c1");   
   gr1->SetMarkerStyle(21);
   gr1->SetMarkerColor(1);
   gr1->SetMarkerSize(0.25);
   gr1->SetTitle(Form("2020.%02d.%02d.WFCTA%02d.allch.Base1stDer",month,day,iTel));
   gr1->GetXaxis()->SetTitle("rbtime/s");
   gr1->GetYaxis()->SetTitle("Base1stDer");
   gr1->GetXaxis()->CenterTitle();
   gr1->GetYaxis()->CenterTitle();
   gr1->Draw("ap");     
   
   TCanvas *c2 = new TCanvas("c2");
   gr2->SetMarkerStyle(21);
   gr2->SetMarkerColor(1);
   gr2->SetMarkerSize(0.25);
   gr2->SetTitle(Form("2020.%02d.%02d.WFCTA%02d.allch.Base2ndDer",month,day,iTel));
   gr2->GetXaxis()->SetTitle("rbtime/s");
   gr2->GetYaxis()->SetTitle("Base2ndDer");
   gr2->GetXaxis()->CenterTitle();
   gr2->GetYaxis()->CenterTitle();
   gr2->Draw("ap");

   TCanvas *c3 = new TCanvas("c3");
   gr3->SetMarkerStyle(21);
   gr3->SetMarkerColor(1);
   gr3->SetMarkerSize(0.25);
   gr3->SetTitle(Form("2020.%02d.%02d.WFCTA%02d.allch.CloudMapTemp",month,day,iTel));
   gr3->GetXaxis()->SetTitle("rbtime/s");
   gr3->GetYaxis()->SetTitle("CloudMapTemp");
   gr3->GetXaxis()->CenterTitle();
   gr3->GetYaxis()->CenterTitle();
   gr3->Draw("alp");
   gr4->SetLineColor(2);
   gr4->SetMarkerColor(2);
   gr4->Draw("same");
   gr4->Draw("l");

   TCanvas *c4 = new TCanvas("c4");
   gr5->SetMarkerStyle(21);
   gr5->SetMarkerColor(1);
   gr5->SetMarkerSize(0.25);
   gr5->SetTitle(Form("2020.%02d.%02d.WFCTA%02d.allch.Base2ndDer:CloudMapTemp",month,day,iTel));
   gr5->GetXaxis()->SetTitle("CloudMapTemp");
   gr5->GetYaxis()->SetTitle("Base2ndDer");
   gr5->GetXaxis()->CenterTitle();
   gr5->GetYaxis()->CenterTitle();
   gr5->Draw("ap");

   c->SaveAs(Form("/scratchfs/ybj/chensuh/2020/EventShow/outfile2/2020.%02d%02d.WFCTA%02d.allch.basevalue.png",month,day,iTel));
   c1->SaveAs(Form("/scratchfs/ybj/chensuh/2020/EventShow/outfile2/2020.%02d%02d.WFCTA%02d.allch.base1stder.png",month,day,iTel));
   c2->SaveAs(Form("/scratchfs/ybj/chensuh/2020/EventShow/outfile2/2020.%02d%02d.WFCTA%02d.allch.base2ndder.png",month,day,iTel));
   c3->SaveAs(Form("/scratchfs/ybj/chensuh/2020/EventShow/outfile2/2020.%02d%02d.WFCTA%02d.allch.cloudtemp.png",month,day,iTel));
   c4->SaveAs(Form("/scratchfs/ybj/chensuh/2020/EventShow/outfile2/2020.%02d%02d.WFCTA%02d.allch.base2ndcloudtemp.png",month,day,iTel));
   //c1->SaveAs(Form("/scratchfs/ybj/chensuh/2020/EventShow/outfile2/2020.%02d%02d.WFCTA%02d.allch.base1stder.png",month,day,iTel));

}
