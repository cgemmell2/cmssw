#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMap.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/MessageLogger/interface/MessageDrop.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "HLTrigger/HLTfilters/interface/HLTL1TSeed.h"
#include "HLTrigger/HLTfilters/interface/HLTL1TSeed.h"

using namespace std;


// constructors
HLTL1TSeed::HLTL1TSeed(const edm::ParameterSet& parSet) : 
  HLTStreamFilter(parSet),
  //useObjectMaps_(parSet.getParameter<bool>("L1UseL1TriggerObjectMaps")),
  //logicalExpression_(parSet.getParameter<string>("L1SeedsLogicalExpression")),
  muonCollectionsTag_(parSet.getParameter<edm::InputTag>("muonCollectionsTag")), // FIX WHEN UNPACKERS ADDED
  muonTag_(muonCollectionsTag_),
  muonToken_(consumes<l1t::MuonBxCollection>(muonTag_)),
  egammaCollectionsTag_(parSet.getParameter<edm::InputTag>("egammaCollectionsTag")), // FIX WHEN UNPACKERS ADDED
  egammaTag_(egammaCollectionsTag_),
  egammaToken_(consumes<l1t::EGammaBxCollection>(egammaTag_)),
  jetCollectionsTag_(parSet.getParameter<edm::InputTag>("jetCollectionsTag")), // FIX WHEN UNPACKERS ADDED
  jetTag_(jetCollectionsTag_),
  jetToken_(consumes<l1t::JetBxCollection>(jetTag_)),
  tauCollectionsTag_(parSet.getParameter<edm::InputTag>("tauCollectionsTag")), // FIX WHEN UNPACKERS ADDED
  tauTag_(tauCollectionsTag_),
  tauToken_(consumes<l1t::TauBxCollection>(tauTag_)),
  etsumCollectionsTag_(parSet.getParameter<edm::InputTag>("etsumCollectionsTag")), // FIX WHEN UNPACKERS ADDED
  etsumTag_(etsumCollectionsTag_),
  etsumToken_(consumes<l1t::EtSumBxCollection>(etsumTag_)),
  m_isDebugEnabled(edm::isDebugEnabled())
{


  // InputTag for L1 Global Trigger object maps
  //m_l1GtObjectMapTag(parSet.getParameter<edm::InputTag> (
  //        "L1GtObjectMapTag")),
  //m_l1GtObjectMapToken(consumes<L1GlobalTriggerObjectMapRecord>(m_l1GtObjectMapTag)),
  

  
  //if (m_l1SeedsLogicalExpression != "L1GlobalDecision") {
  // check also the logical expression - add/remove spaces if needed
  //m_l1AlgoLogicParser = L1GtLogicParser(m_l1SeedsLogicalExpression);
  // list of required algorithms for seeding
        // dummy values for tokenNumber and tokenResult
        //m_l1AlgoSeeds.reserve((m_l1AlgoLogicParser.operandTokenVector()).size());
        //m_l1AlgoSeeds = m_l1AlgoLogicParser.expressionSeedsOperandList();
        //size_t l1AlgoSeedsSize = m_l1AlgoSeeds.size();

        //
        //m_l1AlgoSeedsRpn.reserve(l1AlgoSeedsSize);
        //m_l1AlgoSeedsObjType.reserve(l1AlgoSeedsSize);
  //} else {
  //    m_l1GlobalDecision = true;
  //}


  cout << "DEBUG:  muonCollectionsTag:  " << muonCollectionsTag_ << "\n";
  cout << "DEBUG:  muonTag:  " << muonTag_ << "\n";
  cout << "DEBUG:  egammaCollectionsTag:  " << egammaCollectionsTag_ << "\n";
  cout << "DEBUG:  egammaTag:  " << egammaTag_ << "\n";
  cout << "DEBUG:  jetCollectionsTag:  " << jetCollectionsTag_ << "\n";
  cout << "DEBUG:  jetTag:  " << jetTag_ << "\n";
  cout << "DEBUG:  tauCollectionsTag:  " << tauCollectionsTag_ << "\n";
  cout << "DEBUG:  tauTag:  " << tauTag_ << "\n";
  cout << "DEBUG:  etsumCollectionsTag:  " << etsumCollectionsTag_ << "\n";
  cout << "DEBUG:  etsumTag:  " << etsumTag_ << "\n";

  //LogDebug("HLTL1TSeed") 
  //<< "\n";
    //<< "L1 Seeding using L1 trigger object maps:       "
    //<< useL1TriggerObjectMaps_ << "\n"
    //<< "  if false: seeding with all L1T objects\n"
    //<< "L1 Seeds Logical Expression:                   " << "\n      "
    //<< logicalExpression_ << "\n";
}

// destructor
HLTL1TSeed::~HLTL1TSeed() {
    // empty now
}

// member functions

void
HLTL1TSeed::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  makeHLTFilterDescription(desc);

  // # default: true
  // #    seeding done via L1 trigger object maps, with objects that fired
  // #    only objects from the central BxInEvent (L1A) are used
  // # if false:
  // #    seeding is done ignoring if a L1 object fired or not,
  // #    adding all L1EXtra objects corresponding to the object types
  // #    used in all conditions from the algorithms in logical expression
  // #    for a given number of BxInEvent
  //desc.add<bool>("L1UseL1TriggerObjectMaps",true);

  // # logical expression for the required L1 algorithms;
  // # the algorithms are specified by name
  // # allowed operators: "AND", "OR", "NOT", "(", ")"
  // #
  // # by convention, "L1GlobalDecision" logical expression means global decision
  //desc.add<string>("L1SeedsLogicalExpression","");

  desc.add<edm::InputTag>("muonCollectionsTag",edm::InputTag("simGmtStage2Digis"));
  desc.add<edm::InputTag>("egammaCollectionsTag",edm::InputTag("simCaloStage2Digis"));
  desc.add<edm::InputTag>("jetCollectionsTag",edm::InputTag("simCaloStage2Digis"));
  desc.add<edm::InputTag>("tauCollectionsTag",edm::InputTag("simCaloStage2Digis"));
  desc.add<edm::InputTag>("etsumCollectionsTag",edm::InputTag("simCaloStage2Digis"));

  descriptions.add("hltL1TSeed", desc);
  //descriptions.add("hltL1TSeed", desc);
}

bool HLTL1TSeed::hltFilter(edm::Event& iEvent, const edm::EventSetup& evSetup, trigger::TriggerFilterObjectWithRefs & filterproduct) {

  cout << "MY SIZE of FILTERPRODUCT:  " << filterproduct.getCollectionTagsAsStrings().size() << "\n";

  // the filter object
  if (saveTags()) {
    
    // muons
    filterproduct.addCollectionTag(muonTag_);

    // egamma 
    filterproduct.addCollectionTag(egammaTag_);

    // jet 
    filterproduct.addCollectionTag(jetTag_);

    // tau 
    filterproduct.addCollectionTag(tauTag_);

    // etsum 
    filterproduct.addCollectionTag(etsumTag_);
  }
  
  //seedsL1TriggerObjectMaps(iEvent, filterproduct, l1GtTmAlgo.product(), gtReadoutRecordPtr, physicsDaqPartition))
  seedsAll(iEvent, filterproduct);

  cout << "NEW SIZE of FILTERPRODUCT:  " << filterproduct.getCollectionTagsAsStrings().size() << "\n";
  cout << "m_isDebugEnabled = " << m_isDebugEnabled << endl;

  if (m_isDebugEnabled) {
        dumpTriggerFilterObjectWithRefs(filterproduct);
  }
  return true;

}

// seeding is done ignoring if a L1 object fired or not
// if the event is selected at L1, fill all the L1 objects of types corresponding to the
// L1 conditions from the seeding logical expression for bunch crosses F, 0, 1
// directly from L1Extra and use them as seeds at HLT
// method and filter return true if at least an object is filled
bool HLTL1TSeed::seedsAll(edm::Event & iEvent, trigger::TriggerFilterObjectWithRefs & filterproduct) const {

    //
    bool objectsInFilter = false;

    // Muon L1T
    
    edm::Handle<l1t::MuonBxCollection> muons;
    iEvent.getByToken(muonToken_, muons);
    if (!muons.isValid()){ 
      edm::LogWarning("HLTL1TSeed")
	<< "\nWarning: L1MuonBxCollection with input tag "
	<< muonTag_
	<< "\nrequested in configuration, but not found in the event."
	<< "\nNo muons added to filterproduct."
	<< endl;	
    } else {
      cout << "DEBUG: L1T adding muons to filterproduct...\n";

      l1t::MuonBxCollection::const_iterator iter;
      for (iter = muons->begin(0); iter != muons->end(0); ++iter){
	//objectsInFilter = true;
	l1t::MuonRef myref(muons, muons->key(iter));
	filterproduct.addObject(trigger::TriggerL1Mu, myref);
      }
    }

    //l1t::MuonBxCollection::const_iterator iter;

    // EGamma L1T
    
    edm::Handle<l1t::EGammaBxCollection> egammas;
    iEvent.getByToken(egammaToken_, egammas);
    if (!egammas.isValid()){ 
      edm::LogWarning("HLTL1TSeed")
	<< "\nWarning: L1EGammaBxCollection with input tag "
	<< egammaTag_
	<< "\nrequested in configuration, but not found in the event."
	<< "\nNo egammas added to filterproduct."
	<< endl;	
    } else {
      cout << "DEBUG: L1T adding egammas to filterproduct...\n";

      l1t::EGammaBxCollection::const_iterator iter;
      for (iter = egammas->begin(0); iter != egammas->end(0); ++iter){
	//objectsInFilter = true;
	l1t::EGammaRef myref(egammas, egammas->key(iter));
	filterproduct.addObject(trigger::TriggerL1NoIsoEG, myref); // Temp just use NoIsoEG
      }
    }

    //l1t::EGammaBxCollection::const_iterator iter;
    
    // Jet L1T
    
    edm::Handle<l1t::JetBxCollection> jets;
    iEvent.getByToken(jetToken_, jets);
    if (!jets.isValid()){ 
      edm::LogWarning("HLTL1TSeed")
	<< "\nWarning: L1JetBxCollection with input tag "
	<< jetTag_
	<< "\nrequested in configuration, but not found in the event."
	<< "\nNo jets added to filterproduct."
	<< endl;	
    } else {
      cout << "DEBUG: L1T adding jets to filterproduct...\n";

      l1t::JetBxCollection::const_iterator iter;
      for (iter = jets->begin(0); iter != jets->end(0); ++iter){
	//objectsInFilter = true;
	l1t::JetRef myref(jets, jets->key(iter));
	filterproduct.addObject(trigger::TriggerL1CenJet, myref); // Temp just use CenJet
      }
    }

    //l1t::JetBxCollection::const_iterator iter;
    
    // Tau L1T
    
    edm::Handle<l1t::TauBxCollection> taus;
    iEvent.getByToken(tauToken_, taus);
    if (!taus.isValid()){ 
      edm::LogWarning("HLTL1TSeed")
	<< "\nWarning: L1TauBxCollection with input tag "
	<< tauTag_
	<< "\nrequested in configuration, but not found in the event."
	<< "\nNo taus added to filterproduct."
	<< endl;	
    } else {
      cout << "DEBUG: L1T adding taus to filterproduct...\n";

      l1t::TauBxCollection::const_iterator iter;
      for (iter = taus->begin(0); iter != taus->end(0); ++iter){
	//objectsInFilter = true;
	l1t::TauRef myref(taus, taus->key(iter));
	filterproduct.addObject(trigger::TriggerL1TauJet, myref); // Temp just use TauJet
      }
    }

    //l1t::TauBxCollection::const_iterator iter;
    
    // EtSum L1T
    
    edm::Handle<l1t::EtSumBxCollection> etsums;
    iEvent.getByToken(etsumToken_, etsums);
    if (!etsums.isValid()){ 
      edm::LogWarning("HLTL1TSeed")
	<< "\nWarning: L1EtSumBxCollection with input tag "
	<< etsumTag_
	<< "\nrequested in configuration, but not found in the event."
	<< "\nNo etsums added to filterproduct."
	<< endl;	
    } else {
      cout << "DEBUG: L1T adding etsums to filterproduct...\n";

      l1t::EtSumBxCollection::const_iterator iter;
      for (iter = etsums->begin(0); iter != etsums->end(0); ++iter){
	//objectsInFilter = true;
	l1t::EtSumRef myref(etsums, etsums->key(iter));
	filterproduct.addObject(trigger::TriggerL1ETT, myref); // Temp just use ETT
      }
    }

    //l1t::EtSumBxCollection::const_iterator iter;
    
    /*
      int iObj = -1;

      iObj++;    
      int bxNr = objIter->bx();
      if ((bxNr >= minBxInEvent) && (bxNr <= maxBxInEvent))	    
      objectsInFilter = true;
      filterproduct.addObject(
      trigger::TriggerL1Mu,
      l1extra::L1MuonParticleRef(
      l1Muon, iObj));
    */

    return objectsInFilter;
}

// detailed print of filter content
void HLTL1TSeed::dumpTriggerFilterObjectWithRefs(trigger::TriggerFilterObjectWithRefs & filterproduct) const
{

  LogDebug("HLTL1TSeed") << "\nHLTL1TSeed::hltFilter "
			 << "\n  Dump TriggerFilterObjectWithRefs\n" << endl;
  
  vector<l1t::MuonRef> seedsL1Mu;
  filterproduct.getObjects(trigger::TriggerL1Mu, seedsL1Mu);
  const size_t sizeSeedsL1Mu = seedsL1Mu.size();

  LogTrace("HLTL1TSeed") << "  L1Mu seeds:      " << sizeSeedsL1Mu << "\n"
			 << endl;

  for (size_t i = 0; i != sizeSeedsL1Mu; i++) {

    
    l1t::MuonRef obj = l1t::MuonRef( seedsL1Mu[i]);
    
        LogTrace("HLTL1TSeed") << "L1Mu     " << "\t" << "q*PT = "
                << obj->charge() * obj->pt() << "\t" << "eta =  " << obj->eta()
                << "\t" << "phi =  " << obj->phi();  //<< "\t" << "BX = " << obj->bx();
  }

  vector<l1t::EGammaRef> seedsL1EG;
  // !!! FIXME: trigger::TriggerL1EG does not exist.  Using trigger::TriggerNoIsoL1EG
  filterproduct.getObjects(trigger::TriggerL1NoIsoEG, seedsL1EG);
  const size_t sizeSeedsL1EG = seedsL1EG.size();

  LogTrace("HLTL1TSeed") << "  L1EG seeds:      " << sizeSeedsL1EG << "\n"
			 << endl;

  for (size_t i = 0; i != sizeSeedsL1EG; i++) {

    
    l1t::EGammaRef obj = l1t::EGammaRef( seedsL1EG[i]);
    
        LogTrace("HLTL1TSeed") << "L1EG     " << "\t" << "q*PT = "
                << obj->charge() * obj->pt() << "\t" << "eta =  " << obj->eta()
                << "\t" << "phi =  " << obj->phi();  //<< "\t" << "BX = " << obj->bx();
  }

  vector<l1t::JetRef> seedsL1Jet;
  // !!! FIXME: trigger::TriggerL1Jet does not exist.  Using trigger::TriggerCenJet
  filterproduct.getObjects(trigger::TriggerL1CenJet, seedsL1Jet);
  const size_t sizeSeedsL1Jet = seedsL1Jet.size();

  LogTrace("HLTL1TSeed") << "  L1Jet seeds:      " << sizeSeedsL1Jet << "\n"
			 << endl;

  for (size_t i = 0; i != sizeSeedsL1Jet; i++) {

    
    l1t::JetRef obj = l1t::JetRef( seedsL1Jet[i]);
    
        LogTrace("HLTL1TSeed") << "L1Jet     " << "\t" << "q*PT = "
                << obj->charge() * obj->pt() << "\t" << "eta =  " << obj->eta()
                << "\t" << "phi =  " << obj->phi();  //<< "\t" << "BX = " << obj->bx();
  }

  vector<l1t::TauRef> seedsL1Tau;
  // !!! FIXME: trigger::TriggerL1Tau does not exist.  Using trigger::TriggerTauJet
  filterproduct.getObjects(trigger::TriggerL1TauJet, seedsL1Tau);
  const size_t sizeSeedsL1Tau = seedsL1Tau.size();

  LogTrace("HLTL1TSeed") << "  L1Tau seeds:      " << sizeSeedsL1Tau << "\n"
			 << endl;

  for (size_t i = 0; i != sizeSeedsL1Tau; i++) {

    
    l1t::TauRef obj = l1t::TauRef( seedsL1Tau[i]);
    
        LogTrace("HLTL1TSeed") << "L1Tau     " << "\t" << "q*PT = "
                << obj->charge() * obj->pt() << "\t" << "eta =  " << obj->eta()
                << "\t" << "phi =  " << obj->phi();  //<< "\t" << "BX = " << obj->bx();
  }

  vector<l1t::EtSumRef> seedsL1EtSum;
  // !!! FIXME: trigger::TriggerL1EtSum does not exist.  Using trigger::TriggerETT
  filterproduct.getObjects(trigger::TriggerL1ETT, seedsL1EtSum);
  const size_t sizeSeedsL1EtSum = seedsL1EtSum.size();

  LogTrace("HLTL1TSeed") << "  L1EtSum seeds:      " << sizeSeedsL1EtSum << "\n"
			 << endl;

  for (size_t i = 0; i != sizeSeedsL1EtSum; i++) {

    
    l1t::EtSumRef obj = l1t::EtSumRef( seedsL1EtSum[i]);
    
        LogTrace("HLTL1TSeed") << "L1EtSum     " << "\t" << "q*PT = "
                << obj->charge() * obj->pt() << "\t" << "eta =  " << obj->eta()
                << "\t" << "phi =  " << obj->phi();  //<< "\t" << "BX = " << obj->bx();
  }

  LogTrace("HLTL1TSeed") << " \n\n" << endl;

}

// register as framework plugin
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(HLTL1TSeed);
