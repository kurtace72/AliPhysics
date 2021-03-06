void PHOSPbPbQA(const char* dataset="collection.xml")
{
  gSystem->Load("libTree");
  gSystem->Load("libGeom");
  gSystem->Load("libVMC");
  gSystem->Load("libPhysics");
  
  //load analysis framework
  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISalice"); //AliAnalysisTaskSE

  gSystem->Load("libPWGGAPHOSTasks");
  
  gSystem->AddIncludePath("-I$ALICE_ROOT/include -I$ALICE_ROOT/PHOS");

  cout << "PHOSPbPbQA: processing collection " << dataset << endl;

  TString data = dataset;
  TChain* chain = 0;
  
  if(data.Contains(".xml")) {
    
    TGrid::Connect("alien://");
    
    chain = new TChain("esdTree");
    TGridCollection * collection = gGrid->OpenCollection(dataset);
    
    TGridResult* result = collection->GetGridResult("", 0, 0);
    TList* rawFileList = result->GetFileInfoList();
    
    for (Int_t counter=0 ; counter < rawFileList->GetEntries() ; counter++) {
      TFileInfo * fi =  static_cast<TFileInfo*>(rawFileList->At(counter)) ; 
      const char * rawFile = fi->GetCurrentUrl()->GetUrl() ;  
      printf("Processing %s\n", rawFile) ;
      chain->Add(rawFile);
      printf("Chain: %d entries.\n",chain->GetEntries()); 
    }
  }
  
  if(data.Contains(".txt")) {
    gROOT->LoadMacro("$ALICE_ROOT/PWG0/CreateESDChain.C");
    chain = CreateESDChain(dataset, 300);
  }
  
  if(data.Contains(".root")) {
    chain = new TChain("esdTree");
    chain->Add(dataset);
  }

  AliLog::SetGlobalLogLevel(AliLog::kError);
  
  Int_t nentr = chain->GetEntries();
  printf("Number of events in the collection is %d\n",nentr);

  // Make the analysis manager
  AliAnalysisManager *mgr  = new AliAnalysisManager("Manager", "Manager");

    // Input handler
  AliESDInputHandler *esdHandler = new AliESDInputHandler();
  mgr->SetInputEventHandler(esdHandler);
  
  // Output handler
  AliESDHandler* esdoutHandler   = new AliESDHandler();
  
  // Debug level
  mgr->SetDebugLevel(0);
  
  //Add centrality task!
  gROOT->LoadMacro("$ALICE_ROOT/OADB/macros/AddTaskCentrality.C");
  AliCentralitySelectionTask *taskCentrality = AddTaskCentrality() ;
  // taskCentrality->SetMCInput();

  // Add my task
  AliAnalysisTaskPHOSPbPbQA *task1 = new AliAnalysisTaskPHOSPbPbQA("PbPbQA");
  mgr->AddTask(task1);
  
  // Create containers for input/output
  AliAnalysisDataContainer *cinput   = mgr->GetCommonInputContainer(); 
  AliAnalysisDataContainer *coutput = mgr->CreateContainer("histESD",TList::Class(),
							    AliAnalysisManager::kOutputContainer,
							    "PHOSPbPbQA.root"); 
  
  // Connect input/output for task1
  mgr->ConnectInput(task1 , 0, cinput);
  mgr->ConnectOutput(task1, 1, coutput);
  
  if (mgr->InitAnalysis()) {
    mgr->PrintStatus();
    mgr->StartAnalysis("local", chain);
  }
  
  cout <<" Analysis ended sucessfully "<< endl ;
  
}
