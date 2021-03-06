#ifndef H_rts_operator_Operator
#define H_rts_operator_Operator

#include "rts/operator/Resultset.hpp"

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
class Register;
class DictionarySegment;
class Scheduler;
//---------------------------------------------------------------------------
/// Base class for all operators of the runtime system
class Operator
{
   protected:
   /// Helper for indenting debug output
   static void indent(unsigned level);
   /// Helper for debug output
   static void printRegister(DictionarySegment& dict,const Register* reg);

   public:
   /// Constructor
   Operator();
   /// Destructor
   virtual ~Operator();

   /// Produce the first tuple
   virtual unsigned first() = 0;
   /// Produce the next tuple
   virtual unsigned next() = 0;

   /// Print the operator tree. Debugging only.
   virtual void print(DictionarySegment& dict,unsigned indent=0) = 0;
   /// Add a merge join hint
   virtual void addMergeHint(Register* reg1,Register* reg2) = 0;
   /// Register parts of the tree that can be executed asynchronous
   virtual void getAsyncInputCandidates(Scheduler& scheduler) = 0;

   /// Return the resultset
   virtual Resultset getResultset();

};
//---------------------------------------------------------------------------
#endif
