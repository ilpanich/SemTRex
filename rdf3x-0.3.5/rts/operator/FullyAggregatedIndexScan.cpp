#include "rts/operator/FullyAggregatedIndexScan.hpp"
#include "rts/runtime/Runtime.hpp"
#include <iostream>
#include <cassert>
//---------------------------------------------------------------------------
// RDF-3X
// (c) 2008 Thomas Neumann. Web site: http://www.mpi-inf.mpg.de/~neumann/rdf3x
//
// This work is licensed under the Creative Commons
// Attribution-Noncommercial-Share Alike 3.0 Unported License. To view a copy
// of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/
// or send a letter to Creative Commons, 171 Second Street, Suite 300,
// San Francisco, California, 94105, USA.
//---------------------------------------------------------------------------
/// Implementation
class FullyAggregatedIndexScan::Scan : public FullyAggregatedIndexScan {
   public:
   /// Constructor
   Scan(Database& db,Database::DataOrder order,Register* value1,bool bound1) : FullyAggregatedIndexScan(db,order,value1,bound1) {}

   /// First tuple
   unsigned first();
   /// Next tuple
   unsigned next();

   /// Return the resultset
   Resultset getResultset();
};
//---------------------------------------------------------------------------
/// Implementation
class FullyAggregatedIndexScan::ScanPrefix1 : public FullyAggregatedIndexScan {
   private:
   /// The stop condition
   unsigned stop1;

   public:
   /// Constructor
   ScanPrefix1(Database& db,Database::DataOrder order,Register* value1,bool bound1) : FullyAggregatedIndexScan(db,order,value1,bound1) {}

   /// First tuple
   unsigned first();
   /// Next tuple
   unsigned next();

   /// Return the resultset
   Resultset getResultset();
};
//---------------------------------------------------------------------------
FullyAggregatedIndexScan::Hint::Hint(FullyAggregatedIndexScan& scan)
   : scan(scan)
   // Constructor
{
}
//---------------------------------------------------------------------------
FullyAggregatedIndexScan::Hint::~Hint()
   // Destructor
{
}
//---------------------------------------------------------------------------
void FullyAggregatedIndexScan::Hint::next(unsigned& value1)
   // Scanning hint
{
   // First value
   if (scan.bound1) {
      unsigned v=scan.value1->value;
      if ((~v)&&(v>value1)) {
         value1=v;
      }
   }
   for (std::vector<Register*>::const_iterator iter=scan.merge1.begin(),limit=scan.merge1.end();iter!=limit;++iter) {
      unsigned v=(*iter)->value;
      if ((~v)&&(v>value1)) {
         value1=v;
      }
   }
   if (scan.value1->domain) {
      unsigned v=scan.value1->domain->nextCandidate(value1);
      if (v>value1) {
         value1=v;
      }
   }
}
//---------------------------------------------------------------------------
FullyAggregatedIndexScan::FullyAggregatedIndexScan(Database& db,Database::DataOrder order,Register* value1,bool bound1)
   : value1(value1),bound1(bound1),facts(db.getFullyAggregatedFacts(order)),order(order),scan(&hint),hint(*this)
   // Constructor
{
}
//---------------------------------------------------------------------------
FullyAggregatedIndexScan::~FullyAggregatedIndexScan()
   // Destructor
{
}
//---------------------------------------------------------------------------
void FullyAggregatedIndexScan::print(DictionarySegment& dict,unsigned level)
   // Print the operator tree. Debugging only.
{
   indent(level); std::cout << "<FullyAggregatedIndexScan ";
   switch (order) {
      case Database::Order_Subject_Predicate_Object: std::cout << "Subject"; break;
      case Database::Order_Subject_Object_Predicate: std::cout << "Subject"; break;
      case Database::Order_Object_Predicate_Subject: std::cout << "Object"; break;
      case Database::Order_Object_Subject_Predicate: std::cout << "Object"; break;
      case Database::Order_Predicate_Subject_Object: std::cout << "Predicate"; break;
      case Database::Order_Predicate_Object_Subject: std::cout << "Predicate"; break;
   }
   std::cout << std::endl;
   indent(level+1);
   printRegister(dict,value1); if (bound1) std::cout << "*";
   std::cout << std::endl;
   indent(level); std::cout << ">" << std::endl;
}
//---------------------------------------------------------------------------
static void handleHints(Register* reg1,Register* reg2,Register* result,std::vector<Register*>& merges)
   // Add hints
{
   bool has1=false,has2=false;
   for (std::vector<Register*>::const_iterator iter=merges.begin(),limit=merges.end();iter!=limit;++iter) {
      if ((*iter)==reg1) has1=true;
      if ((*iter)==reg2) has2=true;
   }
   if (reg1==result) has1=true;
   if (reg2==result) has2=true;

   if (has1&&(!has2)) merges.push_back(reg2);
   if (has2&&(!has1)) merges.push_back(reg1);
}
//---------------------------------------------------------------------------
void FullyAggregatedIndexScan::addMergeHint(Register* reg1,Register* reg2)
   // Add a merge join hint
{
   handleHints(reg1,reg2,value1,merge1);
}
//---------------------------------------------------------------------------
void FullyAggregatedIndexScan::getAsyncInputCandidates(Scheduler& /*scheduler*/)
   // Register parts of the tree that can be executed asynchronous
{
}
//---------------------------------------------------------------------------
FullyAggregatedIndexScan* FullyAggregatedIndexScan::create(Database& db,Database::DataOrder order,Register* subject,bool subjectBound,Register* predicate,bool predicateBound,Register* object,bool objectBound)
   // Constructor
{
   // Setup the slot bindings
   Register* value1=0;
   bool bound1=false;
   switch (order) {
      case Database::Order_Subject_Predicate_Object:
         value1=subject;
         bound1=subjectBound;
         assert(!predicate);
         assert(!object);
         break;
      case Database::Order_Subject_Object_Predicate:
         value1=subject;
         bound1=subjectBound;
         assert(!object);
         assert(!predicate);
         break;
      case Database::Order_Object_Predicate_Subject:
         value1=object;
         bound1=objectBound;
         assert(!predicate);
         assert(!subject);
         break;
      case Database::Order_Object_Subject_Predicate:
         value1=object;
         bound1=objectBound;
         assert(!subject);
         assert(!predicate);
         break;
      case Database::Order_Predicate_Subject_Object:
         value1=predicate;
         bound1=predicateBound;
         assert(!subject);
         assert(!object);
         break;
      case Database::Order_Predicate_Object_Subject:
         value1=predicate;
         bound1=predicateBound;
         assert(!object);
         assert(!subject);
         break;
   }

   // Construct the proper operator
   FullyAggregatedIndexScan* result;
   if (!bound1) {
      result=new Scan(db,order,value1,bound1);
   } else {
      result=new ScanPrefix1(db,order,value1,bound1);
   }

   return result;
}
//---------------------------------------------------------------------------
unsigned FullyAggregatedIndexScan::Scan::first()
   // Produce the first tuple
{
   if (!scan.first(facts))
      return false;
   value1->value=scan.getValue1();
   return scan.getCount();
}
//---------------------------------------------------------------------------
unsigned FullyAggregatedIndexScan::Scan::next()
   // Produce the next tuple
{
   if (!scan.next())
      return false;
   value1->value=scan.getValue1();
   return scan.getCount();
}
//---------------------------------------------------------------------------
unsigned FullyAggregatedIndexScan::ScanPrefix1::first()
   // Produce the first tuple
{
   stop1=value1->value;
   if (!scan.first(facts,stop1))
      return false;
   if (scan.getValue1()>stop1)
      return false;
   return scan.getCount();
}
//---------------------------------------------------------------------------
unsigned FullyAggregatedIndexScan::ScanPrefix1::next()
   // Produce the next tuple
{
   if (!scan.next())
      return false;
   if (scan.getValue1()>stop1)
      return false;
   return scan.getCount();
}
//---------------------------------------------------------------------------
Resultset FullyAggregatedIndexScan::getResultset()
{
	Resultset res;
	return res;
}
//---------------------------------------------------------------------------
Resultset FullyAggregatedIndexScan::Scan::getResultset()
{
	Resultset res;
	return res;
}

//---------------------------------------------------------------------------
Resultset FullyAggregatedIndexScan::ScanPrefix1::getResultset()
{
	Resultset res;
	return res;
}
