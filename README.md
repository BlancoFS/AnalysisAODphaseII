# AnalysisAODphaseII


This analyzer computes the efficiencies in the muon reconstruction of AOD samples. The next part explain the instructions to execute the code. 

First, log in gridui:

```ssh -Y ***@gridui.ifca.es -o ServerAliveInterval=240```

CMS eviroment on gridui:

```
bash -l

source /cvmfs/cms.cern.ch/cmsset_default.sh

cd CMSSW_9_1_1_patch1/src

cmsenv
```

Get the code on gridui:

```git clone https://github.com/BlancoFS/AnalysisAODphaseII```

Compile:

```scram b -j 8```

### Run the analyzer:

```cmsRun MuonAnalyzer_cfg.py  inputDataset='ExampleEvent'           maxEvents=-1```

The result will be in rootfiles directory.

### Draw the histograms:

```
root -l -b -q 'doEfficiencies.C+("ExampleEvent")

root -l -b -q 'doVertexType.C+("ExampleEvent")
```


### Combine samples:

```
pushd rootfiles

hadd combineEvent ExampleEvent1 ExampleEvent2 ...

popd
```

### Analyzer:

Look for the collections on a root file

```
edmDumpEventContent  ExampleEvent.root
```



