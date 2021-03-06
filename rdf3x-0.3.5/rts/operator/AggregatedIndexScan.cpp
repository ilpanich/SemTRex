#include "rts/operator/AggregatedIndexScan.hpp"
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
class AggregatedIndexScan::Scan : public AggregatedIndexScan {
   public:
   /// Constructor
   Scan(Database& db,Database::DataOrder order,Register* value1,bool bound1,Register* value2,bool bound2) : AggregatedIndexScan(db,order,value1,bound1,value2,bound2) {}

   /// First tuple
   unsigned first();
   /// Next tuple
   unsigned next();

   /// Return the resultset
   Resultset getResultset();
};
//---------------------------------------------------------------------------
/// Implementation
class AggregatedIndexScan::ScanFilter2 : public AggregatedIndexScan {
   public:
   /// The filter value
   unsigned filter;

   public:
   /// Constructor
   ScanFilter2(Database& db,Database::DataOrder order,Register* value1,bool bound1,Register* value2,bool bound2) : AggregatedIndexScan(db,order,value1,bound1,value2,bound2) {}

   /// First tuple
   unsigned first();
   /// Next tuple
   unsigned next();

   /// Return the resultset
   Resultset getResultset();
};
//---------------------------------------------------------------------------
/// Implementation
class AggregatedIndexScan::ScanPrefix1 : public AggregatedIndexScan {
   private:
   /// The stop condition
   unsigned stop1;

   public:
   /// Constructor
   ScanPrefix1(Database& db,Database::DataOrder order,Register* value1,bool bound1,Register* value2,bool bound2) : AggregatedIndexScan(db,order,value1,bound1,value2,bound2) {}

   /// First tuple
   unsigned first();
   /// Next tuple
   unsigned next();

   /// Return the resultset
   Resultset getResultset();
};
//---------------------------------------------------------------------------
/// Implementation
class AggregatedIndexScan::ScanPrefix12 : public AggregatedIndexScan {
   private:
   /// The stop condition
   unsigned stop1,stop2;

   public:
   /// Constructor
   ScanPrefix12(Database& db,Database::DataOrder order,Register* value1,bool bound1,Register* value2,bool bound2) : AggregatedIndexScan(db,order,value1,bound1,value2,bound2) {}

   /// First tuple
   unsigned first();
   /// Next tuple
   unsigned next();

   /// Return the resultset
   Resultset getResultset();
};
//---------------------------------------------------------------------------
AggregatedIndexScan::Hint::Hint(AggregatedIndexScan& scan)
   : scan(scan)
   // Constructor
{
}
//---------------------------------------------------------------------------
AggregatedIndexScan::Hint::~Hint()
   // Destructor
{
}
//---------------------------------------------------------------------------
void AggregatedIndexScan::Hint::next(unsigned& value1,unsigned& value2)
   // Scanning hint
{
   // First value
   if (scan.bound1) {
      unsigned v=scan.value1->value;
      if ((~v)&&(v>value1)) {
         value1=v;
         value2=0;
      }
   }
   for (std::vector<Register*>::const_iterator iter=scan.merge1.begin(),limit=scan.merge1.end();iter!=limit;++iter) {
      unsigned v=(*iter)->value;
      if ((~v)&&(v>value1)) {
         value1=v;
         value2=0;
      }
   }
   if (scan.value1->domain) {
      unsigned v=scan.value1->domain->nextCandidate(value1);
      if (v>value1) {
         value1=v;
         value2=0;
      }
   }

   // Second value
   if (scan.bound2) {
      unsigned v=scan.value2->value;
      if ((~v)&&(v>value2)) {
         value2=v;
      }
   }
   for (std::vector<Register*>::const_iterator iter=scan.merge2.begin(),limit=scan.merge2.end();iter!=limit;++iter) {
      unsigned v=(*iter)->value;
      if ((~v)&&(v>value2)) {
         value2=v;
      }
   }
   if (scan.value2->domain) {
      unsigned v=scan.value2->domain->nextCandidate(value2);
      if (v>value2) {
         value2=v;
      }
   }
}
//---------------------------------------------------------------------------
AggregatedIndexScan::AggregatedIndexScan(Database& db,Database::DataOrder order,Register* value1,bool bound1,Register* value2,bool bound2)
   : value1(value1),value2(value2),bound1(bound1),bound2(bound2),facts(db.getAggregatedFacts(order)),order(order),scan(&hint),hint(*this)
   // Constructor
{
}
//---------------------------------------------------------------------------
AggregatedIndexScan::~AggregatedIndexScan()
   // Destructor
{
}
//---------------------------------------------------------------------------
void AggregatedIndexScan::print(DictionarySegment& dict,unsigned level)
   // Print the operator tree. Debugging only.
{
   indent(level); std::cout << "<AggregatedIndexScan ";
   switch (order) {
      case Database::Order_Subject_Predicate_Object: std::cout << "SubjectPredicate"; break;
      case Database::Order_Subject_Object_Predicate: std::cout << "SubjectObject"; break;
      case Database::Order_Object_Predicate_Subject: std::cout << "ObjectPredicate"; break;
      case Database::Order_Object_Subject_Predicate: std::cout << "ObjectSubject"; break;
      case Database::Order_Predicate_Subject_Object: std::cout << "PredicateSubject"; break;
      case Database::Order_Predicate_Object_Subject: std::cout << "PredicateObject"; break;
   }
   std::cout << std::endl;
   indent(level+1);
   printRegister(dict,value1); if (bound1) std::cout << "*";
   std::cout << " ";
   printRegister(dict,value2); if (bound2) std::cout << "*";
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
void AggregatedIndexScan::addMergeHint(Register* reg1,Register* reg2)
   // Add a merge join hint
{
   handleHints(reg1,reg2,value1,merge1);
   handleHints(reg1,reg2,value2,merge2);
}
//---------------------------------------------------------------------------
void AggregatedIndexScan::getAsyncInputCandidates(Scheduler& /*scheduler*/)
   // Register parts of the tree that can be executed asynchronous
{
}
//---------------------------------------------------------------------------
AggregatedIndexScan* AggregatedIndexScan::create(Database& db,Database::DataOrder order,Register* subject,bool subjectBound,Register* predicate,bool predicateBound,Register* object,bool objectBound)
   // Constructor
{
   // Setup the slot bindings
   Register* value1=0,*value2=0;
   bool bound1=false,bound2=false;
   switch (order) {
      case Database::Order_Subject_Predicate_Object:
         value1=subject; value2=predicate;
         bound1=subjectBound; bound2=predicateBound;
         assert(!object);
         break;
      case Database::Order_Subject_Object_Predicate:
         value1=subject; value2=object;
         bound1=subjectBound; bound2=objectBound;
         assert(!predicate);
         break;
      case Database::Order_Object_Predicate_Subject:
         value1=object; value2=predicate;
         bound1=objectBound; bound2=predicateBound;
         assert(!subject);
         break;
      case Database::Order_Object_Subject_Predicate:
         value1=object; value2=subject;
         bound1=objectBound; bound2=subjectBound;
         assert(!predicate);
         break;
      case Database::Order_Predicate_Subject_Object:
         value1=predicate; value2=subject;
         bound1=predicateBound; bound2=subjectBound;
         assert(!object);
         break;
      case Database::Order_Predicate_Object_Subject:
         value1=predicate; value2=object;
         bound1=predicateBound; bound2=objectBound;
         assert(!subject);
         break;
   }

   // Construct the proper operator
   AggregatedIndexScan* result;
   if (!bound1) {
      if (bound2)
         result=new ScanFilter2(db,order,value1,bound1,value2,bound2); else
         result=new Scan(db,order,value1,bound1,value2,bound2);
   } else {
      if (!bound2)
         result=new ScanPrefix1(db,order,value1,bound1,value2,bound2); else
         result=new ScanPrefix12(db,order,value1,bound1,value2,bound2);
   }

   return result;
}
//---------------------------------------------------------------------------
unsigned AggregatedIndexScan::Scan::first()
   // Produce the first tuple
{
   if (!scan.first(facts))
      return false;
   value1->value=scan.getValue1();
   value2->value=scan.getValue2();
   return scan.getCount();
}
//---------------------------------------------------------------------------
unsigned AggregatedIndexScan::Scan::next()
   // Produce the next tuple
{
   if (!scan.next())
      return false;
   value1->value=scan.getValue1();
   value2->value=scan.getValue2();
   return scan.getCount();
}
//---------------------------------------------------------------------------
unsigned AggregatedIndexScan::ScanFilter2::first()
   // Produce the first tuple
{
   filter=value2->value;

   if (!scan.first(facts))
      return false;
   if (scan.getValue2()!=filter)
      return next();
   value1->value=scan.getValue1();
   return scan.getCount();
}
//---------------------------------------------------------------------------
unsigned AggregatedIndexScan::ScanFilter2::next()
   // Produce the next tuple
{
   while (true) {
      if (!scan.next())
         return false;
      if (scan.getValue2()!=filter)
         continue;
      value1->value=scan.getValue1();
      return scan.getCount();
   }
}
//---------------------------------------------------------------------------
unsigned AggregatedIndexScan::ScanPrefix1::first()
   // Produce the first tuple
{
   stop1=value1->value;
   if (!scan.first(facts,stop1,0))
      return false;
   if (scan.getValue1()>stop1)
      return false;
   value2->value=scan.getValue2();
   return scan.getCount();
}
//---------------------------------------------------------------------------
unsigned AggregatedIndexScan::ScanPrefix1::next()
   // Produce the next tuple
{
   if (!scan.next())
      return false;
   if (scan.getValue1()>stop1)
      return false;
   value2->value=scan.getValue2();
   return scan.getCount();
}
//---------------------------------------------------------------------------
unsigned AggregatedIndexScan::ScanPrefix12::first()
   // Produce the first tuple
{
   stop1=value1->value; stop2=value2->value;
   if (!scan.first(facts,stop1,stop2))
      return false;
   if ((scan.getValue1()>stop1)||((scan.getValue1()==stop1)&&(scan.getValue2()>stop2)))
      return false;
   return scan.getCount();
}
//---------------------------------------------------------------------------
unsigned AggregatedIndexScan::ScanPrefix12::next()
   // Produce the next tuple
{
   if (!scan.next())
      return false;
   if ((scan.getValue1()>stop1)||((scan.getValue1()==stop1)&&(scan.getValue2()>stop2)))
      return false;
   return scan.getCount();
}
//---------------------------------------------------------------------------
/// Return the resultset
Resultset AggregatedIndexScan::getResultset()
{
	Resultset res;
	return res;
}
//---------------------------------------------------------------------------
/// Return the resultset
Resultset AggregatedIndexScan::Scan::getResultset()
{
	Resultset res;
	return res;
}
//---------------------------------------------------------------------------
/// Return the resultset
Resultset AggregatedIndexScan::ScanFilter2::getResultset()
{
	Resultset res;
	return res;
}
//---------------------------------------------------------------------------
/// Return the resultset
Resultset AggregatedIndexScan::ScanPrefix1::getResultset()
{
	Resultset res;
	return res;
}
//---------------------------------------------------------------------------
/// Return the resultset
Resultset AggregatedIndexScan::ScanPrefix12::getResultset()
{
	Resultset res;
	return res;
}
