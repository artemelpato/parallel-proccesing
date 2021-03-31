#include "Pythia8/Pythia.h"
#include <TTree.h> 
#include <TFile.h> 
#include <TH1F.h> 
#include <vector> 
#include <cmath> 

using namespace Pythia8;

struct particleData 
{
	double E;
	double p_x;
	double p_y;
	double p_z;
	int    isPiMeson;
	int    nEvent;
};

int main()
{
	particleData particle;

	int nParticles = 0;

	TFile *outFile = new TFile("output.root", "RECREATE");

	TTree *tree = new TTree("tree", "Tree kek");
	tree->Branch("Particles Data", &particle, "E:p_x:p_y:p_z:isPiMeson:nEvent");

	Pythia pythia;
	pythia.readString("Beams:eCM = 8000.");
	pythia.readString("HardQCD:all = on");
	pythia.readString("PhaseSpace:pTHatMin = 20.");

	pythia.init();

	for (int iEvent = 0; iEvent < 1; ++iEvent)
	{
		if (!pythia.next()) continue;

		for (int i = 0; i < pythia.event.size(); ++i)
		{
	 		if (pythia.event[i].isFinal() && (pythia.event[i].id() == -211)) 
			{
				particle.E   = pythia.event[i].e();
				particle.p_x = pythia.event[i].px();
				particle.p_y = pythia.event[i].py();
				particle.p_z = pythia.event[i].pz();
				particle.isPiMeson = 1;
				particle.nEvent = i;
			}		

	 		if (pythia.event[i].isFinal() && (pythia.event[i].id() == 2212)) 
			{
				particle.E   = pythia.event[i].e();
				particle.p_x = pythia.event[i].px();
				particle.p_y = pythia.event[i].py();
				particle.p_z = pythia.event[i].pz();
				particle.isPiMeson = 0;
				particle.nEvent = i;
			}		

			nParticles++;
			tree->Fill();
		}
	}

	tree->Write();
	tree->Print();
	tree->Delete();

	outFile->Close();
}



















