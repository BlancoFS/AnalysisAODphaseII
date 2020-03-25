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
    options.inputFiles = \
    'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_400mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-400_HXX_RunIISummer16DR80Premix_step2_230220-1650/200224_233912/0000/EXO-RunIISummer16DR80Premix-03591_1.root', \
    'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_400mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-400_HXX_RunIISummer16DR80Premix_step2_230220-1650/200224_233912/0000/EXO-RunIISummer16DR80Premix-03591_2.root', \
    'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_400mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-400_HXX_RunIISummer16DR80Premix_step2_230220-1650/200224_233912/0000/EXO-RunIISummer16DR80Premix-03591_3.root', \
    'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_400mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-400_HXX_RunIISummer16DR80Premix_step2_230220-1650/200224_233912/0000/EXO-RunIISummer16DR80Premix-03591_4.root', \
    'file:' + samplesPath + 'H2ToLLPXToLeptons_MH_400_MX_50_ctau_400mm_TuneCP2_13TeV_pythia8_80X_13082019-1313/400-50-400_HXX_RunIISummer16DR80Premix_step2_230220-1650/200224_233912/0000/EXO-RunIISummer16DR80Premix-03591_5.root', \
    
    
    
    
    
    
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
                                      beamSpot = cms.InputTag("offlineBeamSpot")
                                      )

process.p = cms.Path(process.muonAnalysis)
