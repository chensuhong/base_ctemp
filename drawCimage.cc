#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <TTree.h>
#include "TFile.h"
#include "TMath.h"
//#include "TH1D.h"
//#include "TH2D.h"
#include "WFCTAEvent.h"
//#include "WFCTAMap.h"
//#include "WFCTAAdcShow.h"

using namespace std;

int main(int argc, char**argv)
{
	//read event files
	char Name1[300]="root://eos01.ihep.ac.cn/";
	char Name2[300];
	strcpy(Name2,Name1);
	strcat(Name2,argv[1]);
	TFile *file_event = TFile::Open(Name2);
	if(!file_event)
	{
		printf("%s dose not exist %s\n",argv[1]);
		return 1;
	}
	if(file_event->IsZombie()||file_event->GetEND()<50)
	{
		printf("%s is null file\n",argv[1]);
		file_event->Close();
		return 1;
	}
	
	TTree *Eventchain = (TTree *)file_event->Get("eventShow");
	if(Eventchain==nullptr)
	{
		printf("%s is null file\n",argv[1]);
		file_event->Close();
		return 1;
	}

	TFile *outfile= new TFile(argv[2],"recreate");

	WFCTAEvent* wfctaevent = new WFCTAEvent();
	Eventchain->SetBranchAddress("WFCTAEvent", &wfctaevent);
	long nevents = Eventchain->GetEntries();
	Eventchain->GetEntry(0);
	int itel = wfctaevent->iTel;
        int64_t rbT0 = wfctaevent->rabbitTime;
	float base[1024]= {0};	
   	short hit_channel[1024] = {0};
	int itime=0;	
	
	TTree *Base = new TTree("Base","BaseH");
	Base->Branch("itel",&itel,"itel/I");
	Base->Branch("time",&rbT0,"rbT0/L");
        //Base->Branch("itime",&itime,"itime/I");
//	Base->Branch("channel",hit_channel,"hit_channel[1024]/S");
	Base->Branch("basevalue",base,"base[1024]/F");
        
	for(int eventEntry=0;eventEntry<nevents;eventEntry++)
        {
		Eventchain->GetEntry(eventEntry);
         
		for(int i=0;i<wfctaevent->iSiPM.size();i++)
		{
			int isipm = wfctaevent->iSiPM.at(i);
			int ibase = wfctaevent->BaseH.at(i);
			base[isipm] += ibase;
		        hit_channel[isipm]++;
		}
	
		if(wfctaevent->rabbitTime-rbT0>30)
		{
			itime++;
			for(int ichannel = 0; ichannel<1024; ichannel++)
			{
				if(hit_channel[ichannel]==0) continue;
				base[ichannel] /= hit_channel[ichannel];
			}
			Base->Fill();
			for(int j = 0;j<1024;j++)
			{
				base[j] = {0};
			 	hit_channel[j] = {0};
			}
			rbT0 = wfctaevent->rabbitTime;
		}
        }

	delete wfctaevent;
	outfile->Write();
	outfile->Close();
	return 0;
}
