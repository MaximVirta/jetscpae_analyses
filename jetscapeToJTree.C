/*
#include <string>
#include <fstream>
#include <sstream>
#include "TLorentzVector.h"
#include <iostream>
#include "TTree.h"
#include "TClonesArray.h"
#include "TFile.h"
//#include <stdio>
#include "TMath.h"
*/
#include "src/JBaseEventHeader.h"
#include "src/JBaseTrack.h"

//using namespace std;
R__LOAD_LIBRARY(/scratch/project_2003154/containers/jetscape_analyses/src/JBaseEventHeader_h.so);
R__LOAD_LIBRARY(/scratch/project_2003154/containers/jetscape_analyses/src/JBaseEventHeader_cxx.so);
// R__LOAD_LIBRARY(/scratch/project_2003154/containers/jetscape_analyses/src/JBaseTrack_h.so);
R__LOAD_LIBRARY(/scratch/project_2003154/containers/jetscape_analyses/src/JBaseTrack_cxx.so);
typedef struct {
	Double_t phi,eta,pt,mass,E,correction;
	Int_t pid;
} JTRACKS;
typedef struct {
	Int_t runnumber,event,ntrack,icent;
	// Double_t eCM,psi_2,psi_3,v_2,v_3;
} JEVENT;

JTRACKS jtracks;
JEVENT jevent;
TTree *vTree;
TTree *jTree;

TClonesArray *event;
TClonesArray *tracks;

void ClearEvent();
void createTree(Bool_t useJTree=kTRUE);
void AddEvent(Int_t seed, Int_t iEvent, Int_t Ntrk, Int_t ic,Bool_t useJTree=kTRUE);
void AddTrack(Int_t i, Int_t pid, Double_t E, Double_t px, Double_t py, Double_t pz,Bool_t useJTree=kTRUE);

void jetscapeToJTree(const TString outputfile = "test.root", const TString inputfile = "output/output_0_final_state_hadrons.dat", const string Ind = 0) {
	printf("Starting the macro\n");
	Bool_t useJTree = kTRUE;
	fstream fin_jetscape;
	TFile *fout = new TFile(outputfile.Data(), "recreate");
	fin_jetscape.open(inputfile.Data(),ios::in);
	if (!fin_jetscape.is_open()) cout << "File not open" << endl;
	Int_t startInd = stoi(Ind);
	createTree();
	string tp;
	int j=0;
	int i =0;
	getline(fin_jetscape, tp);
	int nevent = 0;
	while(getline(fin_jetscape, tp)) {
		stringstream ss(tp);
		string sub;
		int evtInfo[] = {0,0};
		float trackinfo[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
		if (tp.find("Event")!=string::npos) {
			j=0;
			if (useJTree && nevent>0) {
				//cout << "Nr tracks is :" << tracks->GetEntries() << endl;
				jTree->Fill(); // Fill events
			}
			if (useJTree) ClearEvent(); // Clear events
			nevent++;
			while(ss>>sub) {
				// printf("%s\n", sub.data());
				if (j==2) evtInfo[0]=stoi(sub)+startInd*10000;
				if (j==8) evtInfo[1]=stoi(sub);
				j++;
			}

			AddEvent(0,evtInfo[0],evtInfo[1], 0);
		} else if (tp.find("#")!=0){
			j=0;
			while(ss>>sub) {
				trackinfo[j] = stof(sub);
				j++;
			}
			AddTrack((int)trackinfo[0], trackinfo[1], trackinfo[3], trackinfo[4], trackinfo[5], trackinfo[6]);
		}
	}
	jTree->Fill(); // Fill the final event
	fin_jetscape.close();
	if (useJTree) {
		jTree->Write();
	} else {
		vTree->Write();
	}

	fout->Close();
}

void createTree(Bool_t useJTree=kTRUE){
	if (useJTree) {
		jTree = new TTree("jTree", "Jetscape tree");

		event = new TClonesArray("JBaseEventHeader",1000);
		tracks = new TClonesArray("JBaseTrack",10000);
	    jTree->Branch("JTrackList",&tracks);
	    jTree->Branch("JEventHeaderList",&event);
	}
	vTree = new TTree("vTree", "Jetscape tree");
	
	vTree->Branch("phi",&jtracks.phi,"phi/D");
	vTree->Branch("eta",&jtracks.eta,"eta/D");
	vTree->Branch("pt",&jtracks.pt,"pt/D");
	vTree->Branch("mass",&jtracks.mass,"mass/D");
	vTree->Branch("E",&jtracks.E,"E/D");
	vTree->Branch("correction",&jtracks.correction,"correction/D");
	vTree->Branch("pid",&jtracks.pid,"PID/I");
	vTree->Branch("runnumber",&jevent.event,"runnumber/I");
	vTree->Branch("event",&jevent.event,"event/I");
	vTree->Branch("ntrack",&jevent.ntrack,"ntrack/I");
	vTree->Branch("icent",&jevent.icent,"icent/I");
}

void ClearEvent() {event->Clear(); tracks->Clear();}

void AddEvent(Int_t seed, Int_t iEvent, Int_t Ntrk, Int_t ic, Bool_t useJTree=kTRUE){
	if (useJTree) {
		JBaseEventHeader *hdr = new( (*event)[event->GetEntriesFast()] ) JBaseEventHeader;
		hdr->SetEventID(iEvent);
		hdr->SetCentrality(Ntrk);
	} else {
		jevent.runnumber = seed; // just for MC
		jevent.event = iEvent;
		jevent.ntrack = Ntrk;
		jevent.icent = ic;
	}
}

void AddTrack(Int_t i, Int_t pid, Double_t E, Double_t px, Double_t py, Double_t pz, Bool_t useJTree=kTRUE) {

	if (useJTree) {
		new ( (*tracks)[i] )TLorentzVector(px, py, pz, E); // No fill needed every track
	} else {
		float phi = TMath::ATan2(py, px);
		float pt = TMath::Sqrt(px*px+py*py);
		float eta = TMath::ASinH(pz/pt);
		float mass = TMath::Sqrt(E*E-pt*pt-pz*pz);

		jtracks.phi = phi;
		jtracks.eta = eta;
		jtracks.pt  = pt;
		jtracks.E   = E;
		jtracks.mass = mass;
		jtracks.correction = 1.0;
		jtracks.pid = pid;
		vTree->Fill(); // Needed to fill vTree 
	}
}

