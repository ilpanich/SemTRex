//
// This file is part of T-Rex, a Complex Event Processing Middleware.
// See http://home.dei.polimi.it/margara
//
// Authors: Alessandro Margara, Alberto Negrello
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "StacksRule.h"

using namespace std;
using namespace boost;

StacksRule::StacksRule(RulePkt *pkt) {
	// Initializes the rule identifier
	ruleId = pkt->getRuleId();
	eventGenerator = new CompositeEventGenerator(pkt->getCompositeEventTemplate());
	if (pkt->getCompositeEventTemplate()->getAttributesNum()==0) {
		compositeEventId = pkt->getCompositeEventTemplate()->getEventType();
	} else {
		compositeEventId = -1;
	}
	stacksNum = 0;
	aggrsNum = 0;
	negsNum = 0;
	kbNum = 0;

	// Initialize stacks map with predicate and fills it with references
	for (int i=0; i<pkt->getPredicatesNum(); i++) {
		stacksSize[i]=0;
		Stack * tmpStack= new Stack(pkt->getPredicate(i).refersTo, pkt->getPredicate(i).win, pkt->getPredicate(i).kind);
		stacks.insert(make_pair(stacksNum,tmpStack));
		vector<PubPkt *> emptySet;
		receivedPkts.insert(make_pair(stacksNum, emptySet));
		stacksNum++;
		int refersTo = pkt->getPredicate(i).refersTo;
		if (refersTo!=-1) {
			stacks[refersTo]->addLookBackTo(stacksNum-1);
			referenceState.insert(make_pair(i, refersTo));
		}
	}

	// Initialize the query registry with the kb predicates related queries
	for (int i=0; i< pkt->getKBPredicatesNum(); i++) {
		QueryItem * item = new QueryItem(pkt->getKBPredicate(i).db, pkt->getKBPredicate(i).query, pkt->getKBPredicate(i).param, pkt->getKBPredicate(i).kind);
		queryRegistry.insert(make_pair(kbNum, item));
		kbNum++;
	}

	// Initialize negations and fills stacks with references
	for (int i=0; i<pkt->getNegationsNum(); i++) {
		negsSize[i]=0;
		addNegation(pkt->getNegation(i).eventType, pkt->getNegation(i).constraints, pkt->getNegation(i).constraintsNum, pkt->getNegation(i).lowerId, pkt->getNegation(i).lowerTime, pkt->getNegation(i).upperId);
	}

	// Initialize aggregates belonging to the rule
	for (int i=0; i<pkt->getAggregatesNum(); i++) {
		aggsSize[i]=0;
		addAggregate(pkt->getAggregate(i).eventType, pkt->getAggregate(i).constraints, pkt->getAggregate(i).constraintsNum, pkt->getAggregate(i).lowerId, pkt->getAggregate(i).lowerTime, pkt->getAggregate(i).upperId, pkt->getAggregate(i).name, pkt->getAggregate(i).fun);
		// if KB predicates negation is allowed, here we must handle it.
	}

	// Initialize parameters belonging to the rule
	for (int i=0; i<pkt->getParametersNum(); i++) {
		// Verify the following code: it must correctly handle also parameters between a predicate and KB predicate and
		// between two KB predicates
		addParameter(pkt->getParameter(i).evIndex1, pkt->getParameter(i).name1, pkt->getParameter(i).evIndex2, pkt->getParameter(i).name2, pkt->getParameter(i).type,pkt);
	}

	// Initialize the set of consuming indexes
	set<int> cons = pkt->getConsuming();
	for (set<int>::iterator it=cons.begin(); it!=cons.end(); ++it) {
		int consumedIndex = *it;
		consumingIndexes.insert(consumedIndex);
	}

}

StacksRule::~StacksRule() {
	// Deletes stored messages used
	for (map<int, vector<PubPkt *> >::iterator it=receivedPkts.begin(); it!=receivedPkts.end(); ++it) {
		vector<PubPkt *> temp = it->second;
		for (vector<PubPkt *>::iterator it2=temp.begin(); it2!=temp.end(); ++it2) {
			PubPkt *pkt = *it2;
			if (pkt->decRefCount()) delete pkt;
		}
	}
	for (map<int, vector<PubPkt *> >::iterator it=receivedAggs.begin(); it!=receivedAggs.end(); ++it) {
		vector<PubPkt *> temp = it->second;
		for (vector<PubPkt *>::iterator it2=temp.begin(); it2!=temp.end(); ++it2) {
			PubPkt *pkt = *it2;
			if (pkt->decRefCount()) delete pkt;
		}
	}
	for (map<int, vector<PubPkt *> >::iterator it=receivedNegs.begin(); it!=receivedNegs.end(); ++it) {
		vector<PubPkt *> temp = it->second;
		for (vector<PubPkt *>::iterator it2=temp.begin(); it2!=temp.end(); ++it2) {
			PubPkt *pkt = *it2;
			if (pkt->decRefCount()) delete pkt;
		}
	}

	//frees heap memory
	for (map<int, Stack *>::iterator it=stacks.begin(); it!=stacks.end(); ++it) {
		delete it->second;
	}
	for (set<Parameter *>::iterator it=endStackParameters.begin(); it!=endStackParameters.end(); ++it) {
		Parameter *par= *it;
		delete par;
	}
	for (map<int, set<Parameter *> >::iterator it=branchStackParameters.begin(); it!=branchStackParameters.end(); ++it) {
		set<Parameter *> temp =  it->second;
		for(set<Parameter*>::iterator it2=temp.begin(); it2!=temp.end();++it2){
			Parameter *par= *it2;
			delete par;
		}
	}
	for (map<int, set<Parameter *> >::iterator it=negationParameters.begin(); it!=negationParameters.end(); ++it) {
		set<Parameter *> temp =  it->second;
		for(set<Parameter*>::iterator it2=temp.begin(); it2!=temp.end();++it2){
			Parameter *par= *it2;
			delete par;
		}
	}
	for (map<int, set< Parameter *> >::iterator it=aggregateParameters.begin(); it!=aggregateParameters.end(); ++it) {
		set<Parameter *> temp =  it->second;
		for(set<Parameter*>::iterator it2=temp.begin(); it2!=temp.end();++it2){
			Parameter *par= *it2;
			delete par;
		}
	}
	for (map<int, Aggregate *>::iterator it=aggregates.begin(); it!=aggregates.end(); ++it) {
		delete it->second;
	}
	for (map<int, Negation *>::iterator it=negations.begin(); it!=negations.end(); ++it) {
		delete it->second;
	}


	for (map<int, QueryItem *>::iterator it=queryRegistry.begin(); it!=queryRegistry.end(); ++it) {
		delete it->second;
	}


	delete eventGenerator;
	// Check if the new code requires other cleaning actions to be performed on destroy
}

void StacksRule::addToStack(PubPkt *pkt, int index) {
	parametricAddToStack(pkt, stacksSize[index], receivedPkts[index]);
	// **A** Evaluate if on-the-fly selection (packet pre-filtering) could be insert here
}

void StacksRule::addToAggregateStack(PubPkt *pkt, int index) {
	parametricAddToStack(pkt, aggsSize[index], receivedAggs[index]);
}

void StacksRule::addToNegationStack(PubPkt *pkt, int index) {
	parametricAddToStack(pkt, negsSize[index], receivedNegs[index]);
}

void StacksRule::startComputation(PubPkt *pkt, set<PubPkt *> &results, ResultsCache *cache) {

	queryCache = cache;

	// Adds the terminator to the last stack
	pkt->incRefCount();
	receivedPkts[0].push_back(pkt);
	stacksSize[0] = 1;
	// Removes expired events from stacks
	clearStacks();
	// Gets partial results (patterns)
	list<PartialEvent *> * partialResults = getPartialResults(pkt);
	// Checks parameters and removes invalid results from collected ones
	removePartialEventsNotMatchingParameters(partialResults, endStackParameters);

	// After having computed the CEP results, KB filtering must be performed (if any)
	// See (and adapt) the above function code.
	// Also check if checking KB related stuff is appropriated in an inline function

	// Creates complex events and adds them to the results
	createComplexEvents(partialResults, results);
	// Deletes consumed events
	removeConsumedEvent(partialResults);
	// Deletes partial results
	deletePartialEvents(partialResults);
	// Removes the terminator from the last stack
	receivedPkts[0].clear();
	if (pkt->decRefCount()) delete pkt;
	stacksSize[0] = 0;

	for(map<int,QueryItem*>::iterator it=queryRegistry.begin(); it!=queryRegistry.end(); it++) {
		it->second->resetExtParRepl();
	}
}

void StacksRule::processPkt(PubPkt *pkt, MatchingHandler *mh, set<PubPkt *> &results, int index) {
	map<int, set<int> >::iterator aggIt=mh->matchingAggregates.find(index);
	if (aggIt!=mh->matchingAggregates.end()) {
		for (set<int>::iterator it=aggIt->second.begin(); it!=aggIt->second.end(); ++it) {
			int aggIndex = *it;
			addToAggregateStack(pkt, aggIndex);
		}
	}
	map<int, set<int> >::iterator negIt=mh->matchingNegations.find(index);
	if (negIt!=mh->matchingNegations.end()) {
		for (set<int>::iterator it=negIt->second.begin(); it!=negIt->second.end(); ++it) {
			int negIndex = *it;
			addToNegationStack(pkt, negIndex);
		}
	}
	// **A** Evaluate if on-the-fly selection (packet pre-filtering) could be insert here
	map<int, set<int> >::iterator stateIt=mh->matchingStates.find(index);
	if (stateIt!=mh->matchingStates.end()) {
		bool lastStack = false;
		for (set<int>::iterator it=stateIt->second.begin(); it!=stateIt->second.end(); ++it) {
			int stateIndex = *it;
			if (stateIndex!=0) addToStack(pkt, stateIndex);
			else lastStack = true;
		}
		if (lastStack) startComputation(pkt, results, NULL);
	}
}

void StacksRule::parametricAddToStack(PubPkt *pkt, int &parStacksSize, vector<PubPkt *> &parReceived) {
	// **A** Evaluate if on-the-fly selection (packet pre-filtering) could be insert here
	TimeMs timeStamp = pkt->getTimeStamp();
	int i = getFirstValidElement(parReceived, parStacksSize, timeStamp);
	if (i==-1) {
		parStacksSize++;
		parReceived.push_back(pkt);
		pkt->incRefCount();
	} else {
		parStacksSize++;
		vector<PubPkt *>::iterator vecIt = parReceived.begin();
		parReceived.insert(vecIt+i, pkt);
		pkt->incRefCount();
	}
}

void StacksRule::addParameter(int index1, char *name1, int index2, char *name2, StateType type,RulePkt *pkt) {
	Parameter * tmp = new Parameter;
	tmp->evIndex1 = index1;
	tmp->evIndex2 = index2;
	tmp->type = type;
	strcpy(tmp->name1, name1);
	strcpy(tmp->name2, name2);
	switch(type){
	case STATE:
		if (pkt->isInTheSameSequence(index1, index2)  /*&& index2>0*/) {
			branchStackParameters[index2].insert(tmp);
		}
		else endStackParameters.insert(tmp);

		break;
	case KB:
		// KB related parameters are endStackParameters if they must be checked after the whole CEP processing
		if (!pkt->getKBPredicate(index2).param.empty())
			endStackParameters.insert(tmp);
		else
			// TODO - KB related parameters are branchStackParameters if they could be checked during the CEP processing
			// The following rule must be uncommented (and the second one commented) after the first test
			//branchStackParameters[index1].insert(tmp);
			endStackParameters.insert(tmp);
		break;
	case NEG:
		negationParameters[index2].insert(tmp);
		break;
	case AGG:
		aggregateParameters[index2].insert(tmp);
		break;
	}
}

void StacksRule::addNegation(int eventType, Constraint constraints[], int constrLen, int lowIndex, TimeMs &lowTime, int highIndex) {
	negations[negsNum] = new Negation;
	negations[negsNum]->eventType = eventType;
	negations[negsNum]->constraintsNum = constrLen;
	negations[negsNum]->constraints = constraints;
	negations[negsNum]->lowerId = lowIndex;
	negations[negsNum]->lowerTime = lowTime;
	negations[negsNum]->upperId = highIndex;
	vector<PubPkt *> emptyVec;
	receivedNegs.insert(make_pair(negsNum, emptyVec));
	if (lowIndex<0) stacks[highIndex]->addLinkedNegation(negsNum);
	else stacks[lowIndex]->addLinkedNegation(negsNum);
	negsNum++;
}

void StacksRule::addAggregate(int eventType, Constraint *constraints, int constrLen, int lowIndex, TimeMs &lowTime, int highIndex, char *parName, AggregateFun &fun) {
	aggregates[aggrsNum] = new Aggregate;
	aggregates[aggrsNum]->eventType = eventType;
	aggregates[aggrsNum]->constraintsNum = constrLen;
	aggregates[aggrsNum]->constraints = constraints;
	aggregates[aggrsNum]->lowerId = lowIndex;
	aggregates[aggrsNum]->lowerTime = lowTime;
	aggregates[aggrsNum]->upperId = highIndex;
	aggregates[aggrsNum]->fun = fun;
	strcpy(aggregates[aggrsNum]->name, parName);
	vector<PubPkt *> emptyVec;
	receivedAggs.insert(make_pair(aggrsNum, emptyVec));
	aggrsNum++;
}

void StacksRule::getWinEvents(list<PartialEvent *> *results, int index, TimeMs tsUp, CompKind mode, PartialEvent *partialEvent) {
	bool useParameters = false;
	if (stacksSize[index]==0) return;
	// Extracts the minimum and maximum element to process. Returns immediately if they cannot be found.
	TimeMs minTimeStamp = tsUp-stacks[index]->getWindow();
	int index1 = getFirstValidElement(receivedPkts[index], stacksSize[index], minTimeStamp);
	if (index1<0) return;
	// TODO: ws if (receivedPkts[index][index1]->getTimeStamp()>tsUp) return;
	if (receivedPkts[index][index1]->getTimeStamp()>=tsUp) return;
	int index2 = getLastValidElement(receivedPkts[index], stacksSize[index], tsUp, index1);
	if (index2<0) index2 = index1;
	map<int, set<Parameter *> >::iterator it = branchStackParameters.find(index);
	if (it!=branchStackParameters.end()) useParameters = true;
	// Computes the indexes for processing
	int count = 0;
	int endCount = index2-index1;
	// In the case of a LAST_WITHIN semantics, reverses processing order
	if (mode==LAST_WITHIN) {
		count = index2-index1;
		endCount = 0;
	}
	// Starts processing
	while (true) {
		bool usable = true;
		PubPkt * tmpPkt = receivedPkts[index][index1+count];
		if (useParameters) usable = checkParameters(tmpPkt, partialEvent, it->second);
		if (usable) {
			PartialEvent *newPartialEvent = new PartialEvent;
			memcpy(newPartialEvent->indexes, partialEvent->indexes, sizeof(PubPkt *)*stacksNum);
			newPartialEvent->indexes[index] = tmpPkt;
			// Check negations
			bool invalidatedByNegations = false;
			set<int> *negation = stacks[index]->getLinkedNegations();
			if (! negation->empty()) {
				for (set<int>::iterator it=negation->begin(); it!=negation->end(); ++it) {
					int neg = *it;
					if (checkNegation(neg, newPartialEvent)) {
						invalidatedByNegations = true;
						break;
					}
				}
			}
			// If it is not invalidated by events, add the new partial event to results, otherwise delete it
			if (! invalidatedByNegations) {
				results->push_back(newPartialEvent);
				if (mode==LAST_WITHIN || mode==FIRST_WITHIN) break;
			} else {
				delete newPartialEvent;
			}
		}
		// Updates index (increasing or decreasing, depending from the semantics) and check termination condition
		if (mode==LAST_WITHIN) {
			count--;
			if (count<endCount) return;
		} else {
			count++;
			if (count>endCount) return;
		}
	}
}

bool StacksRule::checkNegation(int negIndex, PartialEvent *partialResult) {
	Negation *neg = negations[negIndex];
	// No negations: return false
	if (negsSize[negIndex]==0) return false;
	// Extracts timestamps and indexes
	TimeMs maxTS = partialResult->indexes[neg->upperId]->getTimeStamp();
	TimeMs minTS;
	if (neg->lowerId<0)	{
		minTS = maxTS-neg->lowerTime;
	} else {
		minTS = partialResult->indexes[neg->lowerId]->getTimeStamp();
	}
	int index1 = getFirstValidElement(receivedNegs[negIndex], negsSize[negIndex], minTS);
	// TODO: Aggiungere la seguente riga per avere uguaglianza semantica con TRex nel test Rain.
	// if (receivedNegs[negIndex][0]->getTimeStamp()<=maxTS && receivedNegs[negIndex][0]->getTimeStamp()>=minTS) return true;
	if (index1<0) return false;
	if (receivedNegs[negIndex][index1]->getTimeStamp()>maxTS) return false;
	int index2 = getLastValidElement(receivedNegs[negIndex], negsSize[negIndex], maxTS, index1);
	if (index2<0) index2 = index1;
	// No parameters to check: return true
	map<int, set<Parameter *> >::iterator it = negationParameters.find(negIndex);
	if (it==negationParameters.end()) return true;
	// Iterates over all negations and over all parameters.
	// If a negation can be found that satisfies all parameters, then return true, otherwise return false
	for (int count=0; count<=index2-index1; count++) {
		PubPkt * tmpPkt = receivedNegs[negIndex].at(index1+count);
		if (checkParameters(tmpPkt, partialResult, it->second)) return true;
	}
	return false;
}

list<PartialEvent *> * StacksRule::getPartialResults(PubPkt *pkt) {
	list<PartialEvent *> *prevEvents = new list<PartialEvent *>;
	list<PartialEvent *> *currentEvents = new list<PartialEvent *>;
	PartialEvent *last = new PartialEvent;
	last->indexes[0] = pkt;
	prevEvents->push_back(last);
	// Checks negations on the first state
	set<int> *negation = stacks[0]->getLinkedNegations();
	if (! negation->empty()) {
		for (set<int>::iterator it=negation->begin(); it!=negation->end(); ++it) {
			int neg = *it;
			if (checkNegation(neg, last)) {
				delete last;
				delete prevEvents;
				return currentEvents;
			}
		}
	}
	// Iterates over all states
	for (int state=1; state<stacksNum; state++) {
		Stack *stack = stacks[state];
		// Iterates over all previously generated events
		for (list<PartialEvent *>::iterator listIt=prevEvents->begin(); listIt!=prevEvents->end(); ++listIt) {
			PartialEvent *event = *listIt;
			// Extract events for next iteration
			int refState = referenceState[state];
			TimeMs maxTimeStamp = event->indexes[refState]->getTimeStamp();
			CompKind kind = stack->getKind();
			getWinEvents(currentEvents, state, maxTimeStamp, kind, event);
		}
		// Swaps current and previous results and deletes previous ones
		for (list<PartialEvent *>::iterator it=prevEvents->begin(); it!=prevEvents->end(); ++it) {
			PartialEvent *pe = *it;
			delete pe;
		}
		prevEvents->clear();
		list<PartialEvent *> *temp = prevEvents;
		prevEvents = currentEvents;
		currentEvents = temp;
		if (prevEvents->empty()) {
			break;
		}
	}
	delete currentEvents;
	return prevEvents;
}

bool StacksRule::checkParameter(PubPkt *pkt, PartialEvent *partialEvent, Parameter *parameter) {
	// **B** KB related parameters must be checked here
	int indexOfReferenceEvent = parameter->evIndex1;
	PubPkt *receivedPkt = partialEvent->indexes[indexOfReferenceEvent];
	if (parameter->type != KB) {
		ValType type1, type2;
		int index1, index2;
		if (! receivedPkt->getAttributeIndexAndType(parameter->name2, index2, type2)) return false;
		if (! pkt->getAttributeIndexAndType(parameter->name1, index1, type1)) return false;
		if (type1!=type2) return false;
		switch(type1) {
		case INT:
			return receivedPkt->getIntAttributeVal(index2)==pkt->getIntAttributeVal(index1);
		case FLOAT:
			return receivedPkt->getFloatAttributeVal(index2)==pkt->getFloatAttributeVal(index1);
		case BOOL:
			return receivedPkt->getBoolAttributeVal(index2)==pkt->getBoolAttributeVal(index1);
		case STRING:
			char result1[STRING_VAL_LEN];
			char result2[STRING_VAL_LEN];
			receivedPkt->getStringAttributeVal(index2, result2);
			pkt->getStringAttributeVal(index1, result1);
			return strcmp(result1, result2)==0;
		default:
			return false;
		}
	} else {
		// TODO - Complete the insertion here of the code to verify KB predicates validity.
		// The following line executes the query. Must be moved to the appropriate TESLA rule execution section
		// THE FOLLOWING LINES MUST BE FIXED! Insert KB and QUERY in stacks or use a different structure to store KB predicates
		ValType type1;
		int index1, parSize;
		vector<string> pars1;
		vector<string> pars2;

		char * p;
		for (p = strtok( parameter->name1, "," );  p;  p = strtok( NULL, "," )) {
			pars1.push_back(p);
		}
		for (p = strtok( parameter->name2, "," );  p;  p = strtok( NULL, "," )) {
			pars2.push_back(p);
		}

		if(pars1.size() != pars2.size())
			return false;
		else
			parSize = pars1.size();

		if(parSize == 1) {
			if (! pkt->getAttributeIndexAndType(parameter->name1, index1, type1)) return false;
			QueryItem * item = queryRegistry.at(parameter->evIndex2);

			if (item->needsReplace()) {
				vector<ExtParameter> ep = item->getExtParams();
				if (ep.size() > 0){
					for (vector<ExtParameter>::iterator iter = ep.begin(); iter != ep.end(); iter++) {
						ExtParameter epar = *iter;
						PubPkt* referred = partialEvent->indexes[epar.evIndex1];
						int attIdx;
						ValType attV;
						referred->getAttributeIndexAndType(epar.name1, attIdx, attV);
						switch(attV) {
						case BOOL:
							if (!item->replaceExtParam(epar.name2, "" + lexical_cast<string>(referred->getAttribute(attIdx).boolVal)))
								return false;
							break;
						case INT:
							if(!item->replaceExtParam(epar.name2, "" + lexical_cast<string>(referred->getAttribute(attIdx).intVal)))
								return false;
							break;
						case FLOAT:
							if(!item->replaceExtParam(epar.name2, lexical_cast<string>(referred->getAttribute(attIdx).floatVal)))
								return false;
							break;
						case STRING:
							if(!item->replaceExtParam(epar.name2, string(referred->getAttribute(attIdx).stringVal)))
								return false;
							break;
						}
					}
				}
			}
			if (item->needsReplace()) {
				//cerr << "Errors replacing parameters" << endl;
				return false;
			}
			else {

				if (item->runQuery(queryCache)) {
					Resultset rs = item->getResult();
					for(Resultset::iterator it=rs.first(); it!=rs.last(); it++) {
						Result res = *it;
						if(item->getField(parameter->name2) != -1) {
							Field f = res.getResult()[item->getField(parameter->name2)];
							if (type1 == INT && f.getType() != INTV) { rs.clearRes(); return false; }
							if (type1 == FLOAT && f.getType() != FLOATV) { rs.clearRes(); return false; };
							if (type1 == BOOL && f.getType() != BOOLV) { rs.clearRes(); return false; }
							if (type1 == STRING && f.getType() != STRINGV) { rs.clearRes(); return false; }
							switch(type1) {
							case INT:
								if (pkt->getIntAttributeVal(index1)==f.getIValue()) {
									rs.clearRes();
									return true;
								}
								break;
							case FLOAT:
								if (pkt->getFloatAttributeVal(index1)==f.getFValue()) {
									rs.clearRes();
									return true;
								}
								break;
							case BOOL:
								if (pkt->getBoolAttributeVal(index1)==f.getBValue()) {
									rs.clearRes();
									return true;
								}
								break;
							case STRING:
								char result1[STRING_VAL_LEN];
								pkt->getStringAttributeVal(index1, result1);
								if (strcmp(result1, f.getSValue())==0) {
									rs.clearRes();
									return true;
								}
								break;
							}
						} else {
							rs.clearRes();
							return false;
						}
					}
					rs.clearRes();
					return false;
				}
				return false;
			}
		} else {
			// More than a single parameter
			int valid;
			int idx;
			QueryItem * item = queryRegistry.at(parameter->evIndex2);

			if (item->needsReplace()) {
				vector<ExtParameter> ep = item->getExtParams();
				if (ep.size() > 0){
					for (vector<ExtParameter>::iterator iter = ep.begin(); iter != ep.end(); iter++) {
						ExtParameter epar = *iter;
						PubPkt* referred = partialEvent->indexes[epar.evIndex1];
						int attIdx;
						ValType attV;
						referred->getAttributeIndexAndType(epar.name1, attIdx, attV);
						switch(attV) {
						case BOOL:
							if (!item->replaceExtParam(epar.name2, "" + lexical_cast<string>(referred->getAttribute(attIdx).boolVal)))
								return false;
							break;
						case INT:
							if(!item->replaceExtParam(epar.name2, "" + lexical_cast<string>(referred->getAttribute(attIdx).intVal)))
								return false;
							break;
						case FLOAT:
							if(!item->replaceExtParam(epar.name2, lexical_cast<string>(referred->getAttribute(attIdx).floatVal)))
								return false;
							break;
						case STRING:
							if(!item->replaceExtParam(epar.name2, string(referred->getAttribute(attIdx).stringVal)))
								return false;
							break;
						}
					}
				}
			}
			if (item->needsReplace()) {
				//cerr << "Errors replacing parameters" << endl;
				return false;
			} else {

				if (item->runQuery(queryCache)) {
					Resultset rs = item->getResult();
					for(Resultset::iterator it=rs.first(); it!=rs.last(); it++) {
						Result res = *it;
						valid = 0;
						for(idx = 0; idx < parSize; idx++) {
							char * par1Name = new char[pars1[idx].length() + 1];
							strcpy(par1Name, pars1[idx].c_str());
							char * par2Name = new char[pars2[idx].length() + 1];
							strcpy(par2Name, pars2[idx].c_str());
							if (! pkt->getAttributeIndexAndType(par1Name, index1, type1)) return false;
							if(item->getField(par2Name) != -1) {
								Field f = res.getResult()[item->getField(par2Name)];
								if (type1 == INT && f.getType() != INTV) { rs.clearRes(); return false; }
								if (type1 == FLOAT && f.getType() != FLOATV) { rs.clearRes(); return false; }
								if (type1 == BOOL && f.getType() != BOOLV) { rs.clearRes(); return false; }
								if (type1 == STRING && f.getType() != STRINGV) { rs.clearRes(); return false; }
								switch(type1) {
								case INT:
									if (pkt->getIntAttributeVal(index1)==f.getIValue())
										valid++;
									break;
								case FLOAT:
									if (pkt->getFloatAttributeVal(index1)==f.getFValue())
										valid++;
									break;
								case BOOL:
									if (pkt->getBoolAttributeVal(index1)==f.getBValue())
										valid++;
									break;
								case STRING:
									char result1[STRING_VAL_LEN];
									pkt->getStringAttributeVal(index1, result1);
									if (strcmp(result1, f.getSValue())==0)
										valid++;
									break;
								}
							} else {
								rs.clearRes();
								return false;
							}
							if (valid != idx +1)
								break;
						}
						if (valid == parSize) {
							rs.clearRes();
							return true;
						}
					}
					rs.clearRes();
					return false;
				} else
					return false;
			}
		}
	}
}

bool StacksRule::checkParameters(PubPkt *pkt, PartialEvent *partialEvent, set<Parameter *> &parameters) {
	// **B** KB related parameters must be skipped here
	for (set<Parameter *>::iterator it=parameters.begin(); it!=parameters.end(); ++it) {
		Parameter *par = *it;
		if (! checkParameter(pkt, partialEvent, par)) return false;
	}
	return true;
}

void StacksRule::removePartialEventsNotMatchingParameters(list<PartialEvent *> *partialEvents, set<Parameter *> &parameters) {
	// The function must handle also KB parameters
	for (list<PartialEvent *>::iterator it=partialEvents->begin(); it!=partialEvents->end(); ) {
		bool valid = true;
		for (set<Parameter *>::iterator it2=parameters.begin(); it2!=parameters.end(); ++it2) {
			Parameter *par = *it2;
			int indexOfReferenceEvent = -1;
			if (par->type == KB)
				indexOfReferenceEvent = par->evIndex1;
			else
				indexOfReferenceEvent = par->evIndex2;
			PartialEvent *partialEvent = *it;
			PubPkt *receivedPkt = partialEvent->indexes[indexOfReferenceEvent];
			if (! checkParameter(receivedPkt, partialEvent, par)) {
				valid = false;
				break;
			}
		}
		if (! valid) it = partialEvents->erase(it);
		else ++it;
	}
}

void StacksRule::createComplexEvents(list<PartialEvent *> *partialEvents, set<PubPkt *> &results) {
	for (list<PartialEvent *>::iterator it=partialEvents->begin(); it!=partialEvents->end(); ++it) {
		PartialEvent *pe = *it;
		PubPkt *genPkt = NULL;
		if (compositeEventId>=0) {
			genPkt = new PubPkt(compositeEventId, NULL, 0);
			genPkt->setTime(receivedPkts[0][0]->getTimeStamp());
		} else {
			genPkt = eventGenerator->generateCompositeEvent(pe, aggregates, aggsSize, receivedPkts, receivedAggs, aggregateParameters);
		}
		results.insert(genPkt);
	}
}

void StacksRule::removeConsumedEvent(list<PartialEvent *> *partialEvents) {
	if (consumingIndexes.empty()) return;
	for (int i=1; i<stacksNum; i++) {
		if (consumingIndexes.find(i)==consumingIndexes.end()) continue;
		set<PubPkt *> pktsToRemove;
		for (list<PartialEvent *>::iterator it=partialEvents->begin(); it!=partialEvents->end(); ++it) {
			PartialEvent *pe = *it;
			PubPkt *pkt = pe->indexes[i];
			if (pktsToRemove.find(pkt)==pktsToRemove.end()) {
				pktsToRemove.insert(pkt);
			}
		}
		map<int, vector<PubPkt *> >::iterator mapIt = receivedPkts.find(i);
		for (vector<PubPkt *>::iterator it=mapIt->second.begin(); it!=mapIt->second.end(); ) {
			PubPkt *pkt = *it;
			if (pktsToRemove.find(pkt)!=pktsToRemove.end()) {
				it = mapIt->second.erase(it);
				if (pkt->decRefCount()) delete pkt;
				stacksSize[i]--;
			} else {
				++it;
			}
		}
	}
}

void StacksRule::deletePartialEvents(list<PartialEvent *> *partialEvents) {
	for (list<PartialEvent *>::iterator it=partialEvents->begin(); it!=partialEvents->end(); ++it) {
		PartialEvent *pe = *it;
		delete pe;
	}
	delete partialEvents;
}

void StacksRule::clearStacks() {
	for (int stack=1; stack<stacksNum; stack++) {
		int refersToStack = stacks[stack]->getRefersTo();
		if (stacksSize[refersToStack]==0) continue;
		TimeMs minTS = receivedPkts[refersToStack][0]->getTimeStamp() - stacks[stack]->getWindow();
		removeOldPacketsFromStack(minTS, stacksSize[stack], receivedPkts[stack]);
	}
	for (int negIndex=0; negIndex<negsNum; negIndex++) {
		Negation *neg = negations[negIndex];
		int refersToStack = neg->upperId;
		if (stacksSize[refersToStack]==0) continue;
		TimeMs win;
		if (neg->lowerId<0) {
			win = neg->lowerTime;
		} else {
			int secondIndex = neg->lowerId;
			win = stacks[secondIndex]->getWindow();
		}
		TimeMs minTS = receivedPkts[refersToStack][0]->getTimeStamp()-win;
		removeOldPacketsFromStack(minTS, negsSize[negIndex], receivedNegs[negIndex]);
	}
	for (int aggIndex=0; aggIndex<aggrsNum; aggIndex++) {
		Aggregate *agg = aggregates[aggIndex];
		int refersToStack = agg->upperId;
		if (stacksSize[refersToStack]==0) continue;
		TimeMs win = agg->lowerTime;
		if (win<0) {
			int secondIndex = agg->lowerId;
			win = stacks[secondIndex]->getWindow();
		}
		TimeMs minTS = receivedPkts[refersToStack][0]->getTimeStamp()-win;
		removeOldPacketsFromStack(minTS, aggsSize[aggIndex], receivedAggs[aggIndex]);
	}
}

void StacksRule::removeOldPacketsFromStack(TimeMs &minTS, int &parStacksSize, vector<PubPkt *> &parReceived) {
	if (parStacksSize==0) return;
	int newSize = deleteInvalidElements(parReceived, parStacksSize, minTS);
	if (newSize==parStacksSize) return;
	vector<PubPkt *>::iterator it=parReceived.begin();
	for (int count=0; count<parStacksSize-newSize; count++) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
		it=parReceived.erase(it);
	}
	parStacksSize = newSize;
}

