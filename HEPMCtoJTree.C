#include "TMath.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "src/JBaseEventHeader.h"
#include "src/JBaseTrack.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/Data/GenEventData.h"
#include "HepMC3/GenParticle.h"
#include <pdgcode.h>

using namespace std;

TTree *jTree;
TTree *hepTree;

TClonesArray *event;
TClonesArray *tracks;

HepMC3::GenEventData *evtData = new HepMC3::GenEventData();
HepMC3::GenEvent evt;


void ClearEvent();
void createTree(Bool_t useJTree=kTRUE);
void AddEvent(Int_t seed, Int_t iEvent, Int_t Ntrk, Int_t ic,Bool_t useJTree=kTRUE);
void AddTrack(Int_t i, Double_t px, Double_t py, Double_t pz, Double_t E, Int_t pid, Char_t crg);
void HEPMCtoJTree(const TString outputfile, const TString inputfile);
void readEvent(int iEvent);


int main(int argc, char const *argv[])
{
	if (argc!=3) {
		printf("Usage: %s <inputfile> <outputfile>\n", argv[0]);
		return 0;
	}
	HEPMCtoJTree(argv[1], argv[2]);
	return 1;
}


void HEPMCtoJTree(const TString inputfile, const TString outputfile) {
	printf("Starting the macro\n");

	TFile *fin = new TFile(inputfile.Data(), "read");
	TFile *fout = new TFile(outputfile.Data(), "recreate");

	createTree();

	hepTree = (TTree*)fin->Get("hepmc3_tree");

	hepTree->SetBranchAddress("hepmc3_event", &evtData);

	int nevents = hepTree->GetEntries();

	int tt = (int)(0.1*nevents);

	for (int ievt = 0; ievt < nevents; ++ievt) {
		if (ievt>0 && ievt % tt == 0) printf("%.0f %% completed\n", 100*(float)ievt/(float)nevents);
		readEvent(ievt);
		ClearEvent();
	}
	printf("All done! Nr. events read: %d\n", nevents);

	jTree->Write();
	fin->Close();
	fout->Close();
	return;

}

void readEvent(int iEvent) {
	hepTree->GetEntry(iEvent);
	evt.read_data(*evtData);
	uint j = 0;
	Bool_t acceptEventA = kFALSE;
	Bool_t acceptEventC = kFALSE;
	for (std::shared_ptr<HepMC3::GenParticle> sp: evt.particles()) {
		HepMC3::GenParticle *p = sp.get();
		HepMC3::FourVector momentum = p->momentum();
		// Include only final state particles
		if (p->status()!=1 || TMath::Abs(p->pid())<50) continue;
		// Include only hadrons
               	smash::PdgCode pdg = smash::PdgCode::from_decimal(p->pid());
		if (!pdg.is_hadron()) continue; // Read only final state hadrons.
		//if (iEvent==1) printf("id: %d, stat: %d, pid: %d, e: %.3f, px: %.3f, py: %.3f, pz: %.3f\n", j, p->status(), p->pid(), momentum.e(), momentum.px(), momentum.py(), momentum.pz());
		AddTrack(j, momentum.px(), momentum.py(), momentum.pz(), momentum.e(), p->pid(), -1);
		if (momentum.eta() > 2.8 && momentum.eta() < 5.1 && !acceptEventA) acceptEventA=kTRUE;
		if (momentum.eta() > -3.7 && momentum.eta() <-1.7 && !acceptEventC) acceptEventC=kTRUE;
		j++;
	}
	// if (iEvent<100) printf("%d\n", j);
	if (acceptEventA && acceptEventC) {
		jTree->Fill();
	} else {
		printf("Event number %d excluded from analysis.\n", iEvent);
	}
}

void createTree(Bool_t useJTree){
	jTree = new TTree("jTree", "Jetscape tree");

	event = new TClonesArray("JBaseEventHeader",1000);
	tracks = new TClonesArray("JBaseTrack",10000);
	jTree->Branch("JTrackList",&tracks);
	jTree->Branch("JEventHeaderList",&event);
}

void ClearEvent() {
	// Clear JTree events
	event->Clear(); 
	tracks->Clear();

	// Clear HepMCTree events
	evtData->particles.clear();
	evtData->vertices.clear();
	evtData->links1.clear();
	evtData->links2.clear();
	evtData->attribute_id.clear();
	evtData->attribute_name.clear();
	evtData->attribute_string.clear();
}

void AddEvent(Int_t iEvent, Int_t Ntrk){
	JBaseEventHeader *hdr = new( (*event)[event->GetEntriesFast()] ) JBaseEventHeader;
	hdr->SetEventID(iEvent);
	hdr->SetCentrality(Ntrk);
}

void AddTrack(Int_t i, Double_t px, Double_t py, Double_t pz, Double_t E, Int_t pid, Char_t crg) {
	new ( (*tracks)[i] )JBaseTrack(px, py, pz, E, i, pid, crg); // No fill needed every track
}

