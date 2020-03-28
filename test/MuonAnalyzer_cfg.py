import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as opts

process = cms.Process("RecoMuon")

options = opts.VarParsing('analysis')

options.register ('inputDataset',
                  '',
                  opts.VarParsing.multiplicity.singleton,
                  opts.VarParsing.varType.string,
                  'Input dataset')

options.register ('location',
                  'gridui',
                  opts.VarParsing.multiplicity.singleton,
                  opts.VarParsing.varType.string,
                  'Can be lxplus or gridui')

samplesPath = '/gpfs/projects/cms/fernance/'

# Defaults
options.maxEvents = -1

options.parseArguments()

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(options.maxEvents))

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('Demo')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.cerr.INFO = cms.untracked.PSet(limit = cms.untracked.int32(-1))


if 'BSMH_CTau-400' in options.inputDataset :
    print '\n Will read /H2ToLLPXToLeptons_MH_400_MX_50_ctau_400mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-400_HXX_RunIISummer16DR80Premix_step2_230220-1650\n'
    for i in range(1,1000):
      options.inputFiles = \
      'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_400mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-400_HXX_RunIISummer16DR80Premix_step2_230220-1650/200224_233912/0000/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \
    
    for i in range(1000,2000):
        options.inputFiles = \
        'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_400mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-400_HXX_RunIISummer16DR80Premix_step2_230220-1650/200224_233912/0001/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \

    for i in range(2000,3000):
        if i == 2004:
          continue
        if i == 2271:
          continue
        options.inputFiles = \
        'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_400mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-400_HXX_RunIISummer16DR80Premix_step2_230220-1650/200224_233912/0002/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \

    for i in range(3000,4000):
        options.inputFiles = \
        'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_400mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-400_HXX_RunIISummer16DR80Premix_step2_230220-1650/200224_233912/0003/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \

    for i in range(4000,5000):
        options.inputFiles = \
        'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_400mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-400_HXX_RunIISummer16DR80Premix_step2_230220-1650/200224_233912/0004/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \

    for i in range(5000,6000):
        options.inputFiles = \
        'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_400mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-400_HXX_RunIISummer16DR80Premix_step2_230220-1650/200224_233912/0005/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \

    for i in range(6000,7000):
        if i == 6728:
          continue
        options.inputFiles = \
        'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_400mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-400_HXX_RunIISummer16DR80Premix_step2_230220-1650/200224_233912/0006/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \

    for i in range(7000,8000):
        options.inputFiles = \
        'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_400mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-400_HXX_RunIISummer16DR80Premix_step2_230220-1650/200224_233912/0007/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \

    for i in range(8000,9000):
        if i == 8045:
          continue
        if i == 8758:
          continue
        options.inputFiles = \
        'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_400mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-400_HXX_RunIISummer16DR80Premix_step2_230220-1650/200224_233912/0008/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \

    for i in range(9000,9972):
        if i == 9600:
          continue
        options.inputFiles = \
        'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_400mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-400_HXX_RunIISummer16DR80Premix_step2_230220-1650/200224_233912/0009/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \

    
if 'BSMH_CTau-40' in options.inputDataset :
    print '\n Will read /H2ToLLPXToLeptons_MH_400_MX_50_ctau_40mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-40_HXX_RunIISummer16DR80Premix_step2_230220-1650\n' 
    
    for i in range(1,1000):
      options.inputFiles = \
      'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_40mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-40_HXX_RunIISummer16DR80Premix_step2_230220-1650/200225_082618/0000/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \
    
    for i in range(1000,2000):
      options.inputFiles = \
      'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_40mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-40_HXX_RunIISummer16DR80Premix_step2_230220-1650/200225_082618/0001/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \
    
    for i in range(2000,3000):
      options.inputFiles = \
      'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_40mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-40_HXX_RunIISummer16DR80Premix_step2_230220-1650/200225_082618/0002/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \
    
    for i in range(3000,4000):
      options.inputFiles = \
      'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_40mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-40_HXX_RunIISummer16DR80Premix_step2_230220-1650/200225_082618/0003/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \
    
    for i in range(4000,5000):
      options.inputFiles = \
      'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_40mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-40_HXX_RunIISummer16DR80Premix_step2_230220-1650/200225_082618/0004/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \
    
    for i in range(5000,6000):
      options.inputFiles = \
      'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_40mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-40_HXX_RunIISummer16DR80Premix_step2_230220-1650/200225_082618/0005/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \
    
    for i in range(6000,7000):
      options.inputFiles = \
      'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_40mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-40_HXX_RunIISummer16DR80Premix_step2_230220-1650/200225_082618/0006/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \
    
    for i in range(7000,8000):
      options.inputFiles = \
      'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_40mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-40_HXX_RunIISummer16DR80Premix_step2_230220-1650/200225_082618/0007/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \
    
    for i in range(8000,9000):
      options.inputFiles = \
      'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_40mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-40_HXX_RunIISummer16DR80Premix_step2_230220-1650/200225_082618/0008/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \
    
    for i in range(9000,10000):
      options.inputFiles = \
      'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_40mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-40_HXX_RunIISummer16DR80Premix_step2_230220-1650/200225_082618/0009/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \
    
        
      
if 'BSMH_CTau-4' in options.inputDataset :
    print '\n Will read /H2ToLLPXToLeptons_MH_400_MX_50_ctau_4mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-4_HXX_RunIISummer16DR80Premix_step2_230220-1650\n' 
    
    for i in range(1,1000):
      options.inputFiles = \
      'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_4mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-4_HXX_RunIISummer16DR80Premix_step2_230220-1650/200225_115305/0000/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \
    
    for i in range(1000,2000):
      options.inputFiles = \
      'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_4mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-4_HXX_RunIISummer16DR80Premix_step2_230220-1650/200225_115305/0001/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \
    
    for i in range(2000,3000):
      options.inputFiles = \
      'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_4mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-4_HXX_RunIISummer16DR80Premix_step2_230220-1650/200225_115305/0002/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \
    
    for i in range(3000,4000):
      options.inputFiles = \
      'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_4mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-4_HXX_RunIISummer16DR80Premix_step2_230220-1650/200225_115305/0003/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \
    
    for i in range(4000,5000):
      options.inputFiles = \
      'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_4mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-4_HXX_RunIISummer16DR80Premix_step2_230220-1650/200225_115305/0004/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \
    
    for i in range(5000,6000):
      options.inputFiles = \
      'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_4mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-4_HXX_RunIISummer16DR80Premix_step2_230220-1650/200225_115305/0005/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \
    
    for i in range(6000,7000):
      options.inputFiles = \
      'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_4mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-4_HXX_RunIISummer16DR80Premix_step2_230220-1650/200225_115305/0006/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \
    
    for i in range(7000,8000):
      options.inputFiles = \
      'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_4mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-4_HXX_RunIISummer16DR80Premix_step2_230220-1650/200225_115305/0007/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \
    
    for i in range(8000,9000):
      options.inputFiles = \
      'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_4mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-4_HXX_RunIISummer16DR80Premix_step2_230220-1650/200225_115305/0008/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \
    
    for i in range(9000,10000):
      options.inputFiles = \
      'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_4mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-4_HXX_RunIISummer16DR80Premix_step2_230220-1650/200225_115305/0009/EXO-RunIISummer16DR80Premix-03591_'+str(i)+'.root', \
    
        
        
      
      
      
      
      
      
        
        
    
    
# Output
outputFileName = 'rootfiles/' + options.inputDataset + '.root'

print ' The output will be written in', outputFileName, '\n'

process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(options.inputFiles))

process.TFileService=cms.Service('TFileService',
                                 fileName=cms.string(outputFileName)
                                 )

process.muonAnalysis = cms.EDAnalyzer("ExampleMuonAnalyzer2",
                                      MuonCollection = cms.InputTag("muons"),
                                      genCollection = cms.InputTag("genParticles"),
                                      vertices = cms.InputTag("offlinePrimaryVertices"),
                                      beamSpot = cms.InputTag("offlineBeamSpot"),
                                      dispGlbTracks = cms.InputTag("displacedGlobalMuons"),
                                      dispStaTracks = cms.InputTag("displacedStandAloneMuons")
                                      )

process.p = cms.Path(process.muonAnalysis)
