#include "JParticleTools.h"
#include <set>
#include <map>
#include <iostream>
#include <vector>
#include <algorithm> // For std::find
#include <cmath>     // For std::abs


using namespace std;


void JParticleTools::GetParticles() { 
    constexpr float MinPt = 0.f;
    
    Nch = 0;
    NchFT0M = 0;
    
    // Loop over all particles in the event
    for (std::size_t i = 0; i < event.size(); ++i) {
        if (!event[i].isFinal() || !event[i].isCharged() || !event[i].isHadron()) {
            continue;
        }
        
        float eta = event[i].eta();
        if ((eta>3.5 && eta<4.9) || (eta>-3.3 && eta<-2.1)) {
            NchFT0M++;
        }
        
        if (std::abs(eta) > TrackEtaRange || event[i].pT() < MinPt) {
            continue;
        }

        float px = event[i].px();
        float py = event[i].py();
        float pz = event[i].pz();
        float energy = event[i].e();
        int pid = event[i].id();
        int stat = event[i].status();
        
        int label = 0;

		new ((*fInputList)[Nch++]) AliJBaseTrack(px, py, pz, energy, pid, label, stat);
	}
}

