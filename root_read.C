#include <TTree.h>
#include <TFile.h>
#include <vector>
#include <cmath>
#include <fstream>
#include <TH1D.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <iostream>

struct particleData 
{
	double E;
	double p_x;
	double p_y;
	double p_z;
	int    isPiMeson;
	int    nEvent;
};

int root_read(char* argv)
{
	std::cout << argv << std::endl;
	
	int iEvent = 0;

	TFile *inFile = new TFile(argv, "READ");
	TFile *outFile = new TFile("finalHist.root", "UPDATE");
	TH1D *massHisto = (TH1D*)outFile->Get("MassHisto");

	if (massHisto == nullptr)
		massHisto = new TH1D("MassHisto", "Mass Distribution", 1000, 1, 2);
	
	TH1D *newMassHisto = new TH1D("newMassHisto", "Mass Distribution", 1000, 1, 2);

	TTreeReader *treeReader = new TTreeReader("tree", inFile);
	
	TTreeReaderValue<double> readE  (*treeReader, "ParticlesData.E");
	TTreeReaderValue<double> readpx (*treeReader, "ParticlesData.p_x");
	TTreeReaderValue<double> readpy (*treeReader, "ParticlesData.p_y");
	TTreeReaderValue<double> readpz (*treeReader, "ParticlesData.p_z");
	
	TTreeReaderValue<int> readIsPi   (*treeReader, "ParticlesData.isPiMeson");
	TTreeReaderValue<int> readnEvent (*treeReader, "ParticlesData.nEvent");

	particleData readData;

	std::vector<particleData> piMesonVec, protonVec;

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
		else if (*readIsPi == 0)
		{
			readData.E   = *readE;
			readData.p_x = *readpx;
			readData.p_y = *readpy;
			readData.p_z = *readpz;

			protonVec.push_back(readData);
		}

		if (*readnEvent != iEvent)
		{
			for (int im = 0; im < piMesonVec.size(); im++)
				for (int ip = 0; ip < protonVec.size(); ip++)
					massHisto->Fill( sqrt( (piMesonVec[im].E   + protonVec[ip].E  )*(piMesonVec[im].E   + protonVec[ip].E  )
							     - (piMesonVec[im].p_x + protonVec[ip].p_x)*(piMesonVec[im].p_x + protonVec[ip].p_x)
							     - (piMesonVec[im].p_y + protonVec[ip].p_y)*(piMesonVec[im].p_y + protonVec[ip].p_y)
							     - (piMesonVec[im].p_z + protonVec[ip].p_z)*(piMesonVec[im].p_z + protonVec[ip].p_z) ) );
	
			piMesonVec.clear();
			protonVec.clear();	
			iEvent++;
		}

	} 

			
//	massHisto->Add(newMassHisto);	
	massHisto->Write("", TObject::kOverwrite);	

	inFile->Close();
	outFile->Close();
	
	return 0;
}






