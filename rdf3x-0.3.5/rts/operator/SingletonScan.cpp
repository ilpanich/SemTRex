#include "rts/operator/SingletonScan.hpp"
#include <iostream>
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
SingletonScan::SingletonScan()
   // Constructor
{
}
//---------------------------------------------------------------------------
SingletonScan::~SingletonScan()
   // Destructor
{
}
//---------------------------------------------------------------------------
unsigned SingletonScan::first()
   // Produce the first tuple
{
   return 1;
}
//---------------------------------------------------------------------------
unsigned SingletonScan::next()
   // Produce the next tuple
{
   return false;
}
//---------------------------------------------------------------------------
void SingletonScan::print(DictionarySegment& /*dict*/,unsigned level)
   // Print the operator tree. Debugging only.
{
   indent(level); std::cout << "<SingletonScan>" << std::endl;
}
//---------------------------------------------------------------------------
void SingletonScan::addMergeHint(Register* /*reg1*/,Register* /*reg2*/)
   // Add a merge join hint
{
}
//---------------------------------------------------------------------------
void SingletonScan::getAsyncInputCandidates(Scheduler& /*scheduler*/)
   // Register parts of the tree that can be executed asynchronous
{
}
//---------------------------------------------------------------------------
Resultset SingletonScan::getResultset()
{
	Resultset res;
	return res;
}
