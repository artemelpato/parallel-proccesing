#include <TTree.h>
#include <TFile.h>
#include <vector>
#include <cmath>
#include <fstream>
#include <TH1F.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

struct particleData 
{
	double E;
	double p_x;
	double p_y;
	double p_z;
	int    isPiMeson;
	int    nEvent;
};

int root_read()
{
	int nCollisions;
	ifstream nColFile("ncollisions.txt");
	nColFile >> nCollisions;

	TFile *inFile = new TFile("output.root", "UPDATE");
	TTreeReader *treeReader = new TTreeReader("tree", inFile);
	TH1F *massHisto = new TH1F("MassHisto", "Mass Distribution", 200, 1.154, 1.158);

	TTreeReaderValue<double> readE  (*treeReader, "ParticlesData.E");
	TTreeReaderValue<double> readpx (*treeReader, "ParticlesData.p_x");
	TTreeReaderValue<double> readpy (*treeReader, "ParticlesData.p_y");
	TTreeReaderValue<double> readpz (*treeReader, "ParticlesData.p_z");
	
	TTreeReaderValue<int> readIsPi   (*treeReader, "ParticlesData.isPiMeson");
	TTreeReaderValue<int> readnEvent (*treeReader, "ParticlesData.nEvent");

	particleData readData;

	std::vector<particleData> piMesonVec, protonVec;
	
	for (int i = 0; i < nCollisions; i++)
	{
		while (treeReader->Next())
		{
	 		if (*readIsPi == 1) 
			{
				readData.E   = *readE;
				readData.p_x = *readpx;
				readData.p_y = *readpy;
				readData.p_z = *readpz;

				piMesonVec.push_back(readData);
			}
			else
			{
				readData.E   = *readE;
				readData.p_x = *readpx;
				readData.p_y = *readpy;
				readData.p_z = *readpz;

				protonVec.push_back(readData);
			}

			if (*readnEvent != i)
				break;

		} 

		for (int im = 0; im < piMesonVec.size(); im++)
			for (int ip = 0; ip < protonVec.size(); ip++)
				massHisto->Fill( sqrt( (piMesonVec[im].E + protonVec[ip].E)*(piMesonVec[im].E + protonVec[ip].E)
						       	- (piMesonVec[im].p_x + protonVec[ip].p_x)*(piMesonVec[im].p_x + protonVec[ip].p_x)
						       	- (piMesonVec[im].p_y + protonVec[ip].p_y)*(piMesonVec[im].p_y + protonVec[ip].p_y)
						       	- (piMesonVec[im].p_z + protonVec[ip].p_z)*(piMesonVec[ip].p_z + protonVec[ip].p_z) ) );
		
		piMesonVec.clear();
		protonVec.clear();	

	}
		
	massHisto->Write();	
	inFile->Close();
	
	new TBrowser;
	
	return 0;
}







