#include "Pythia8/Pythia.h"
#include <TTree.h> 
#include <TFile.h> 
#include <TH1F.h> 
#include <vector> 
#include <cmath> 
#include <fstream> 
#include <iostream> 

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

int main(int argc, char* argv[])
{
	if (argc < 4)
       	{
		std::cerr << "Usage: " << argv[0] << " INIT_SEED N_EVENTS OUTFILE_NAME" << std::endl;
		return 1;
	}

	
	std::string seed( argv[1] );
	std::string nCollisions( argv[2] );
	std::string outfileName( argv[3] );

	std::string pythiaSeedString("Random:seed = ");
	pythiaSeedString.append( seed );

	std::string rootFileName ("output/");
	rootFileName.append( outfileName );
	rootFileName.append("_");
	rootFileName.append( seed );
	rootFileName.append(".root");

	particleData particle;

	TFile *outFile = new TFile(rootFileName.c_str(), "RECREATE");

	TTree *tree = new TTree("tree", "Tree kek");
	tree->Branch("ParticlesData", &particle, "E/D:p_x/D:p_y/D:p_z/D:isPiMeson/I:nEvent/I");

	Pythia pythia;
	pythia.readString("Beams:eCM = 8000.");
	pythia.readString("HardQCD:all = on");
	pythia.readString("PhaseSpace:pTHatMin = 20.");
	pythia.readString("Random:setSeed = on");
	pythia.readString( pythiaSeedString );

	pythia.init();

	for (int iEvent = 0; iEvent < std::stoi( nCollisions ); ++iEvent)
	{
		if (!pythia.next()) continue;

		for (int i = 0; i < pythia.event.size(); ++i)
		{
	 		if ((pythia.event[i].isFinal()) && (pythia.event[i].id() == -211)) 
			{
				particle.E   = pythia.event[i].e();
				particle.p_x = pythia.event[i].px();
				particle.p_y = pythia.event[i].py();
				particle.p_z = pythia.event[i].pz();
				particle.isPiMeson = 1;
				particle.nEvent = iEvent;
				tree->Fill();
			}		

	 		if ((pythia.event[i].isFinal()) && (pythia.event[i].id() == 2212)) 
			{
				particle.E   = pythia.event[i].e();
				particle.p_x = pythia.event[i].px();
				particle.p_y = pythia.event[i].py();
				particle.p_z = pythia.event[i].pz();
				particle.isPiMeson = 0;
				particle.nEvent = iEvent;
				tree->Fill();
			}		

		}
	}

	tree->Write();
	tree->Print();

	outFile->Close();
}


















