//===========================================================
// JParticleTools.h
// DongJo Kim (dong.jo.kim@cern.ch)
//===========================================================

#ifndef JPARTICLETOOLS_H
#define JPARTICLETOOLS_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#include <Pythia8/Pythia.h>

#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "TRandom3.h"

#include "AliJBaseTrack.h"
#include "set"
#include "map"

using namespace std;
using namespace Pythia8; 

class JParticleTools {

	public:

		JParticleTools (Event &inevent): 
			event(inevent){

				unif = new TRandom3();
				fInputList = new TClonesArray("AliJBaseTrack", 1500 );
				TrackEtaRange = 1.0;
			}

		void InitializeEvent(){
			fInputList->Clear();
			UniqueID=0;
		}

		void GetParticles();
		TClonesArray * GetInputList() const{return fInputList;}
		TRandom3 *unif;
		int GetMidMultiplicity()  const {return Nch;}
		int GetFT0MMultiplicity() const {return NchFT0M;}
		void SetEtaRange(double eta){TrackEtaRange = eta;}
		int UniqueID;
		TLorentzVector lvParticle;
		TClonesArray *fInputList;  // tracklist

		Event &event;
		int Nch;
		int NchFT0M;

		double TrackEtaRange ;

};

#endif

