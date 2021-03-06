#ifndef H_tools_rdf3xload_StringLookup
#define H_tools_rdf3xload_StringLookup
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
#include <string>
#include "TempFile.hpp"
//---------------------------------------------------------------------------
/// Lookup cache for early string aggregation
class StringLookup {
   private:
   /// The hash table size
   static const unsigned lookupSize = 1009433;

   /// Strings seen so far
   std::string* strings;
   /// Ids for the strings
   uint64_t* ids;
   /// The next IDs
   uint64_t nextPredicate,nextNonPredicate;

   public:
   /// Constructor
   StringLookup();
   /// Destructor
   ~StringLookup();

   /// Lookup a predicate
   unsigned lookupPredicate(TempFile& stringsFile,const std::string& predicate);
   /// Lookup a value
   unsigned lookupValue(TempFile& stringsFile,const std::string& value);
};
//---------------------------------------------------------------------------
#endif
