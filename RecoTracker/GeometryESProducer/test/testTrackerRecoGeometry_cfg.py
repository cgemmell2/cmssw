import FWCore.ParameterSet.Config as cms

process = cms.Process("TrackerRecoGeometryTest")

process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32(1) )

process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger = cms.Service("MessageLogger",
                                    debugModules = cms.untracked.vstring('TrackerRecoGeometryAnalyzer'),
                                    destinations = cms.untracked.vstring('TrackerRecoGeometryAnalyzer_infos'),
                                    TrackerRecoGeometryAnalyzer_infos = cms.untracked.PSet(
                                      threshold = cms.untracked.string('INFO') 
                                      ),
                                    categories = cms.untracked.vstring('TrackerRecoGeometryAnalyzer')
                                    )

process.source = cms.Source("EmptySource")

process.TrackerRecoGeometryAnalyzer = cms.EDAnalyzer("TrackerRecoGeometryAnalyzer")
process.p1 = cms.Path(process.TrackerRecoGeometryAnalyzer)


