TTree *readTree = (TTree*)outFile->Get("tree");

	particleData readParticle;
	readTree->SetBranchAddress("Particles Data", &readParticle);
		
	TH1F *massHisto = new TH1F("Mass Histo", "Mass Distribution", 200, 0, 15);

	std::vector<particleData> piMesonVec, protonVec;
	
	readTree->GetEntry(0);

	int nEntries = 1;

	for (int i = 0; i < 1; i++)
	{
		do
		{
	 		if (readParticle.isPiMeson == 1) 
				piMesonVec.push_back(readParticle);
			else
				protonVec.push_back(readParticle);

			readTree->GetEntry(nEntries);	
			nEntries++;
		} while (readParticle.nEvent == i);

		for (auto v: piMesonVec)
			for (auto w: protonVec)
				massHisto->Fill(sqrt( (v.E + w.E)* (v.E + w.E) -  (v.p_x + w.p_x)*(v.p_x + w.p_x) -  (v.p_y + w.p_y)*(v.p_y + w.p_y) - (v.p_z + w.p_z)*(v.p_z + w.p_z) ));
		
		piMesonVec.clear();
		protonVec.clear();	

	}
		
	massHisto->Write();	
	outFile->Close();
	readTree->Delete();