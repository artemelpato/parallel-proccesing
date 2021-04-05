#include "Pythia8/Pythia.h"
#include <TTree.h> 
#include <TFile.h> 
#include <TH1F.h> 
#include <vector> 
#include <cmath> 
#include <fstream> 

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
	int  nCollisions;
	ifstream nColFile("ncollisions.txt");
	nColFile >> nCollisions;
	
	particleData particle;

	TFile *outFile = new TFile("output.root", "RECREATE");

	TTree *tree = new TTree("tree", "Tree kek");
	tree->Branch("ParticlesData", &particle, "E/D:p_x/D:p_y/D:p_z/D:isPiMeson/I:nEvent/I");

	Pythia pythia;
	pythia.readString("Beams:eCM = 8000.");
	pythia.readString("HardQCD:all = on");
	pythia.readString("PhaseSpace:pTHatMin = 20.");

	pythia.init();

	for (int iEvent = 0; iEvent < nCollisions; ++iEvent)
	{
		if (!pythia.next()) continue;

		for (int i = 0; i < pythia.event.size(); ++i)
		{
	 		if (pythia.event[i].isFinal() == true && (pythia.event[i].id() == -211)) 
			{
				particle.E   = pythia.event[i].e();
				particle.p_x = pythia.event[i].px();
				particle.p_y = pythia.event[i].py();
				particle.p_z = pythia.event[i].pz();
				particle.isPiMeson = 1;
				particle.nEvent = iEvent;
			}		

	 		if (pythia.event[i].isFinal() == true && (pythia.event[i].id() == 2212)) 
			{
				particle.E   = pythia.event[i].e();
				particle.p_x = pythia.event[i].px();
				particle.p_y = pythia.event[i].py();
				particle.p_z = pythia.event[i].pz();
				particle.isPiMeson = 0;
				particle.nEvent = iEvent;
			}		

			tree->Fill();
		}
	}

	tree->Write();
	tree->Print();

	outFile->Close();
}



















