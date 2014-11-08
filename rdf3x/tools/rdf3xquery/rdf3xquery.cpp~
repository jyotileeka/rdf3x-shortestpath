#include "cts/codegen/CodeGen.hpp"
#include "cts/infra/QueryGraph.hpp"
#include "cts/parser/SPARQLLexer.hpp"


#include "rts/segment/DictionarySegment.hpp"
#include "cts/parser/SPARQLParser.hpp"
#include "cts/plangen/PlanGen.hpp"
#include "cts/semana/SemanticAnalysis.hpp"
#include "infra/osdep/Timestamp.hpp"
#include "rts/database/Database.hpp"
#include "rts/runtime/Runtime.hpp"
#include "rts/operator/Operator.hpp"
#include "rts/operator/PlanPrinter.hpp"
#include "cts/semana/SemanticAnalysis.hpp"
#ifdef CONFIG_LINEEDITOR
#include "lineeditor/LineInput.hpp"
#endif
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <queue>
#include <sstream>
//JYoti --begin
#include "rts/operator/AggregatedIndexScan.hpp"
#include "rts/operator/ResultsPrinter.hpp"
#include "rts/operator/Selection.hpp"
#include "rts/operator/IndexScan.hpp"
#include <time.h>
#include <sys/time.h>
//#include <>
//JYoti --end
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
using namespace std;
//---------------------------------------------------------------------------
bool smallAddressSpace()
   // Is the address space too small?
{
   return sizeof(void*)<8;
}
//---------------------------------------------------------------------------
static string readInput(istream& in)
   // Read a stream into a string
{
   string result;
   while (true) {
      string s;
      getline(in,s);
      result+=s;
      if (!in.good())
         break;
      result+='\n';
   }
   return result;
}
//---------------------------------------------------------------------------
static bool readLine(string& query)
   // Read a single line
{
#ifdef CONFIG_LINEEDITOR
   // Use the lineeditor interface
   static lineeditor::LineInput editHistory(L">");
   return editHistory.readUtf8(query);
#else
   // Default fallback
   cerr << ">"; cerr.flush();
   return getline(cin,query);
#endif
}
//---------------------------------------------------------------------------
static void showHelp()
   // Show internal commands
{
   cout << "Recognized commands:" << endl
        << "help          shows this help" << endl
        << "select ...    runs a SPARQL query" << endl
        << "explain ...   shows the execution plan for a SPARQL query" << endl
        << "exit          exits the query interface" << endl;
}
//---------------------------------------------------------------------------
static void runQuery(Database& db,const string& query,bool explain)
   // Evaluate a query
{
   QueryGraph queryGraph;
   {
      // Parse the query
      SPARQLLexer lexer(query);
      SPARQLParser parser(lexer);
      try {
         parser.parse();
      } catch (const SPARQLParser::ParserException& e) {
         cerr << "parse error: " << e.message << endl;
         return;
      }

      // And perform the semantic anaylsis
      try {
         SemanticAnalysis semana(db);
         semana.transform(parser,queryGraph);
      } catch (const SemanticAnalysis::SemanticException& e) {
         cerr << "semantic error: " << e.message << endl;
         return;
      }
      if (queryGraph.knownEmpty()) {
         if (explain)
            cerr << "static analysis determined that the query result will be empty" << endl; else
            cout << "<empty result>" << endl;
         return;
      }
   }

   // Run the optimizer
   PlanGen plangen;
   Plan* plan=plangen.translate(db,queryGraph);
   if (!plan) {
      cerr << "internal error plan generation failed" << endl;
      return;
   }

   // Build a physical plan
   Runtime runtime(db);
   Operator* operatorTree=CodeGen().translate(runtime,queryGraph,plan,false);

   // Explain if requested
   if (explain) {
      DebugPlanPrinter out(runtime,false);
      operatorTree->print(out);
   } else {
      // Else execute it
      if (operatorTree->first()) {
         while (operatorTree->next()) ;
      }
   }

   delete operatorTree;
}
//---------------------------------------------------------------------------
map<unsigned,pair<unsigned,unsigned > > dijkstra(unsigned int s, Database &db, unsigned int dist_id)
{
    //unsigned edges=0;
    map<unsigned, pair<unsigned,std::vector< pair<unsigned,unsigned> > > >g;
    map<unsigned int, unsigned int> d1; //distance
    map<unsigned int, unsigned int> c1; //state
    map<unsigned int, pair<unsigned,unsigned> >prev1;
    vector <pair<unsigned,unsigned> >adjacency_list_variable;
    d1[s]=0;
    c1[s]=1;
    priority_queue<pair<unsigned,pair<unsigned, unsigned > >,vector<pair<unsigned,pair<unsigned,unsigned> > >,greater<pair<unsigned,pair<unsigned, unsigned> > > > q; // declare priority_queue
    q.push(make_pair(d1[s],make_pair(~0u,s))); //push starting vertex
    while(!q.empty())
    {
        unsigned u=(q.top().second).second;
        adjacency_list_variable.clear();
        
        Register subject,predicate,object;
        subject.reset(); 
        predicate.reset(); 
        object.reset();
      
        subject.value=u;
      
        IndexScan * scan=IndexScan::create(db, Database::Order_Subject_Predicate_Object, &subject, true, &predicate, false, &object, false, 0);
        if(scan->first())do
        {            
            adjacency_list_variable.push_back(make_pair(predicate.value,object.value));
            adjacency_list_variable.push_back(make_pair(~0u,predicate.value));         
        }while (scan->next());
        else
        {
            adjacency_list_variable.clear();
        }
        delete scan;
        g[u]=make_pair((q.top().second).first,adjacency_list_variable);
        
        q.pop(); //pop vertex from queue
        if(c1[u]==2)
            continue;
        c1[u]=2; 
        for(int i=0;i<(g[u].second).size();i++)
        {
            unsigned v=(g[u].second)[i].second;
            unsigned predicateBeforeObject=(g[u].second)[i].first;
            if(c1[v]==0) //new vertex found
            {
                d1[v]=d1[u]+1;
                c1[v]=1;
                q.push(make_pair(d1[v],make_pair(predicateBeforeObject,v))); //add vertex to queue
                prev1[v]=make_pair(u,predicateBeforeObject);
            }
            else if(c1[v]==1 && d1[v]>d1[u]+1) //shorter path to gray vertex found
            {
                d1[v]=d1[u]+1;
                //q.push(make_pair(d1[v],make_pair(predicateBeforeObject,v))); //push this vertex to queue
                prev1[v]=make_pair(u,predicateBeforeObject);
            }
            else if(c1[v]==2)
            { 
                continue;
            }
            if(v==dist_id)
            {
                return prev1;
            }
        }
    }
    g.clear();
    d1.clear();
    c1.clear();
    adjacency_list_variable.clear();
    q=priority_queue<pair<unsigned,pair<unsigned, unsigned > >,vector<pair<unsigned,pair<unsigned,unsigned> > >,greater<pair<unsigned,pair<unsigned, unsigned> > > >();    
    return prev1;
}
//-------------------------------------------------------
void dijkstra_implemenation(Database& db, string srcDest[2]) //srcDest[2] is an array of source destination pairs
{
    unsigned int id=0;    
    ofstream outputFile1;
    /*outputFile1.open("OutputFile.txt", ios::app|ios::out);           
    outputFile1<<"\n The vertexes in the path from source to destination in reverse order are:";     
    outputFile1.close();*/

    timespec tS;
    tS.tv_sec = 0;
    tS.tv_nsec = 0;
    struct timeval startTime, endTime;
    long mtime, seconds, useconds;    
    gettimeofday(&startTime, NULL);  
    
    SemanticAnalysis semana(db);
    //Type type;
    semana.lookupNode(srcDest[0] ,Type::URI , 0, id); //the correct implementation for obtaining an id
    map<unsigned int, pair<unsigned,unsigned> > prev;  
    unsigned int vertex;
    
    DictionarySegment& dic=db.getDictionary(); 
    const char* start,*stop; 
    Type::ID type; 
    unsigned subType;
    
    unsigned int dist_id=0;
    semana.lookupNode(srcDest[1],Type::URI , 0, dist_id);
    if(dist_id==0)
    {
        semana.lookupNode(srcDest[1], Type::Literal, 0, dist_id);
    }
    
    vertex=dist_id;
    prev=dijkstra(id, db, dist_id);
    unsigned resourcePath=0;  bool pathExists=false;
    do
    {
        if(prev[vertex].first!=prev[vertex].second)
        {                            
            if(prev[vertex].second!=~0u)
            {
                resourcePath++;
                dic.lookupById(prev[vertex].second, start, stop, type, subType);
                char *var=(char*)(start);
                while(var<(stop))
                {
                    /*outputFile1.open("OutputFile.txt", ios::app|ios::out);           
                    outputFile1<<*var;
                    outputFile1.close();*/
                    var++;
                }
                /*outputFile1.open("OutputFile.txt", ios::app|ios::out);               
                outputFile1<<"\n";
                outputFile1.close();*/
                pathExists=true;
            }        
            if(prev[vertex].first!=~0u){
                resourcePath++;
                dic.lookupById(prev[vertex].first, start, stop, type, subType);
                char *var=(char*)(start);
                while(var<(stop))
                {
                    /*outputFile1.open("OutputFile.txt", ios::app|ios::out);             
                    outputFile1<<*var;
                    outputFile1.close();*/
                    var++;
                }
                pathExists=true;
                /*outputFile1.open("OutputFile.txt", ios::app|ios::out);           
                outputFile1<<"\n";            
                outputFile1.close();*/
            }
        }
    
        vertex=prev[vertex].first;
    }while(prev.count(vertex)); 
    /*if (pathExists)
        resourcePath=resourcePath+1; //It will give the path to the source to the destination
    */
    gettimeofday(&endTime, NULL);
    seconds  = endTime.tv_sec  - startTime.tv_sec;
    useconds = endTime.tv_usec - startTime.tv_usec;
    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    
    outputFile1.open("OutputFile.txt", ios::app|ios::out);           
    outputFile1<<"\n"<<resourcePath<<"\t"<<mtime <<" milliseconds";    
    outputFile1.close();
}
//-------------------------------------------------------
void dijkstra_implemenationOutputPrintedOnConsole(Database& db, string srcDest[2]) //srcDest[2] is an array of source destination pairs
{
    unsigned int id=0;    
    ofstream outputFilePrintOnConsole;
    /*outputFilePrintOnConsole.open("OutputFile.txt", ios::app|ios::out);           
    outputFilePrintOnConsole<<"\n The vertexes in the path from source to destination in reverse order are:";     
    outputFilePrintOnConsole.close();*/

    timespec tS;
    tS.tv_sec = 0;
    tS.tv_nsec = 0;
    struct timeval startTime, endTime;
    long mtime, seconds, useconds;    
    gettimeofday(&startTime, NULL);  
    
    SemanticAnalysis semana(db);
    //Type type;
    semana.lookupNode(srcDest[0] ,Type::URI , 0, id); //the correct implementation for obtaining an id
    map<unsigned int, pair<unsigned,unsigned> > prev;  
    unsigned int vertex;
    
    DictionarySegment& dic=db.getDictionary(); 
    const char* start,*stop; 
    Type::ID type; 
    unsigned subType;
    
    unsigned int dist_id=0;
    semana.lookupNode(srcDest[1],Type::URI , 0, dist_id);
    if(dist_id==0)
    {
        semana.lookupNode(srcDest[1], Type::Literal, 0, dist_id);
    }
    
    vertex=dist_id;
    prev=dijkstra(id, db, dist_id);
    unsigned resourcePath=0;  bool pathExists=false;
    do
    {
        if(prev[vertex].first!=prev[vertex].second)
        {                
            if(prev[vertex].second!=~0u)
            {
                resourcePath++;
                dic.lookupById(prev[vertex].second, start, stop, type, subType);
                char *var=(char*)(start);
                while(var<(stop))
                {
                    /*outputFile1.open("OutputFile.txt", ios::app|ios::out);           
                    outputFile1<<*var;
                    outputFile1.close();*/
                    cout<<*var;
                    var++;
                }
                /*outputFile1.open("OutputFile.txt", ios::app|ios::out);               
                outputFile1<<"\n";
                outputFile1.close();*/
                pathExists=true;
            }        
            if(prev[vertex].first!=~0u)
            {
                resourcePath++;
                dic.lookupById(prev[vertex].first, start, stop, type, subType);
                char *var=(char*)(start);
                while(var<(stop))
                {
                    /*outputFile1.open("OutputFile.txt", ios::app|ios::out);             
                    outputFile1<<*var;
                    outputFile1.close();*/
                    cout<<*var;
                    var++;
                }
                /*outputFile1.open("OutputFile.txt", ios::app|ios::out);           
                outputFile1<<"\n";            
                outputFile1.close();*/
                pathExists=true;
            }
        }
    
        vertex=prev[vertex].first;
    }while(prev.count(vertex)); 
    /*if(pathExists)
        resourcePath=resourcePath+1; //It will give the path to the source to the destination*/

    gettimeofday(&endTime, NULL);
    seconds  = endTime.tv_sec  - startTime.tv_sec;
    useconds = endTime.tv_usec - startTime.tv_usec;
    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    
    outputFilePrintOnConsole.open("OutputFilePrintOnConsole.txt", ios::app|ios::out);           
    outputFilePrintOnConsole<<"\n"<<resourcePath<<"\t"<<mtime <<" milliseconds";    
    outputFilePrintOnConsole.close();
}

//---------------------------------------------------------------------------
map<unsigned int, unsigned int> OldDijkstra(unsigned int s, Database &db, unsigned int dist_id)
{
    map<unsigned int, std::vector<unsigned int> >g;
    DictionarySegment& dic=db.getDictionary();     
    map<unsigned int, unsigned int> d1; //distance
    map<unsigned int, unsigned int> c1; //state
    map<unsigned int, unsigned int>prev1;
    vector <unsigned int>adjacency_list_variable;
    d1[s]=0;
    c1[s]=1;
    priority_queue<pair<unsigned int,unsigned int>,vector<pair<unsigned int,int> >,greater<pair<unsigned int, unsigned int> > > q; // declare priority_queue
    q.push(make_pair(d1[s],s)); //push starting vertex
    while(!q.empty())
    {
        unsigned int u=q.top().second;
        adjacency_list_variable.clear();
        
        Register subject,predicate,object;
        subject.reset(); 
        predicate.reset(); 
        object.reset();
      
        subject.value=u;
      
        IndexScan * scan=IndexScan::create(db, Database::Order_Subject_Predicate_Object, &subject, true, &predicate, false, &object, false, 0);
        if(scan->first())do
        {
         adjacency_list_variable.push_back(object.value);
         if (object.value==dist_id)
         {
             break;
         }
         
        }while (scan->next());
        else
        {
            adjacency_list_variable.clear();
            //try working with the code with continue
        }
        delete scan;
        g[u]=adjacency_list_variable;

        
        q.pop(); //pop vertex from queue
        if(c1[u]==2)
            continue;
        c1[u]=2; 
        for(int i=0;i<g[u].size();i++)
        {
            unsigned int v=g[u][i];
            if(c1[v]==0) //new vertex */found
            {
                d1[v]=d1[u]+1;
                c1[v]=1;
                q.push(make_pair(d1[v],v)); //add vertex to queue
                prev1[v]=u;
            }
            else if(c1[v]==1 && d1[v]>d1[u]+1) //shorter path to gray vertex found
            {
                d1[v]=d1[u]+1;
                //q.push(make_pair(d1[v],v)); //push this vertex to queue
                prev1[v]=u;
            }
            if(v==dist_id)
            {
                return prev1;
            }
        }
    }
    return prev1;
}

void OldDijkstra_implemenation(Database& db, string srcDest[2])
{
    unsigned int id=0;    
    timespec tS;
    tS.tv_sec = 0;
    tS.tv_nsec = 0;
    struct timeval startTime, endTime;
    long mtime, seconds, useconds;    
    gettimeofday(&startTime, NULL);  
   
    SemanticAnalysis semana(db);
    //Type type;
    semana.lookupNode(srcDest[0] ,Type::URI , 0, id); //the correct implementation for obtaining an id
    map<unsigned int,unsigned int> prev;    
    DictionarySegment& dic=db.getDictionary(); 
    const char* start,*stop; 
    Type::ID type; 
    unsigned subType;
    
    unsigned int dist_id=0;
    semana.lookupNode(srcDest[1] ,Type::URI , 0, dist_id);
    if(dist_id==0)
    {
        semana.lookupNode(srcDest[1] , Type::Literal, 0, dist_id);
    }
    unsigned int vertex;
    prev=OldDijkstra(id, db, dist_id);
    vertex=dist_id;
    unsigned triplePathDistance=0; bool pathExists=false;
    do
    {
        if(prev[vertex]!=0)
        { 
            triplePathDistance++;
            dic.lookupById(prev[vertex], start, stop, type, subType);
            char *var=(char*)(start);
            while(var<=(stop))
            {
                //cout<<*var;
                var++;
            }
            //cout<<"\n";
            pathExists=true;
        }        
        vertex=prev[vertex];
    }while(prev.count(vertex)); 
   /*if(pathExists)
       triplePathDistance=triplePathDistance+1;*/
   gettimeofday(&endTime, NULL);
   seconds  = endTime.tv_sec  - startTime.tv_sec;
   useconds = endTime.tv_usec - startTime.tv_usec;
   mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

   ofstream outputFileOldDijkstra;
   outputFileOldDijkstra.open("OutputFileOldDijkstra.txt", ios::app|ios::out);       
   outputFileOldDijkstra<<"\n"<<triplePathDistance<<"\t"<<mtime<<" milliseconds";
   outputFileOldDijkstra.close();       
   
}
//--------------------------------------------------------------------------
int main(int argc,char* argv[])
{   
   int runAsColdOrWarmCache=0;
   cout<<"\n Enter:";
   cout<<"\n 1. For Cold Cache Timings";
   cout<<"\n 2. For Warm Cache Timings";
   cin>>runAsColdOrWarmCache;
   // Warn first
   if (smallAddressSpace())
      cerr << "Warning: Running RDF-3X on a 32 bit system is not supported and will fail for large data sets. Please use a 64 bit system instead!" << endl;

   // Greeting
   cerr << "RDF-3X query interface" << endl
        << "(c) 2008 Thomas Neumann. Web site: http://www.mpi-inf.mpg.de/~neumann/rdf3x" << endl;

   // Check the arguments
   if ((argc!=2)&&(argc!=3)) {
      cerr << "usage: " << argv[0] << " <database> [queryfile]" << endl;
      return 1;
   }
   
   std::ifstream file(argv[2]);
   std::string str; 

   while (std::getline(file, str))
   {
       Database db;
       // Open the database   
       if (!db.open(argv[1],true)) {
           cerr << "unable to open database " << argv[1] << endl;
       }                  
   
       if(runAsColdOrWarmCache==1)
       {
           if(system("sudo sh -c \"sync; echo 3 > /proc/sys/vm/drop_caches\"")!=0)
               cout<<"\n sync failed";
       }
       string srcDest[2];       
       string word;
       stringstream stream(str);
       int i=0;
       while( getline(stream, word, '>') )
       {           
           stringstream streamTemp(word); string wordTemp; bool alternate=false;
           while(getline(streamTemp,wordTemp,'<'))
           {
               if(alternate)
                   srcDest[i]= wordTemp;    
               else
                   alternate=true;
           }
           i++;
       }
       cout<<"\n Source-destination pair:"<<srcDest[0]<<"\t"<<srcDest[1]<<"\n";
       dijkstra_implemenation(db,srcDest);
       db.close();
       ofstream outputFile1;
       outputFile1.open("OutputFile.txt", ios::app|ios::out);       
       outputFile1<<"\t"<<srcDest[0]<<"\t"<<srcDest[1]<<"\n";
       outputFile1.close();       
   } 

   std::ifstream file1(argv[2]);
   //Modified Dijkstra algorithm with output printed on the console 
   while (std::getline(file1, str))
   {
       Database db;
       // Open the database   
       if (!db.open(argv[1],true)) {
           cerr << "unable to open database " << argv[1] << endl;
       }                  

       if(runAsColdOrWarmCache==1)
       {
           if(system("sudo sh -c \"sync; echo 3 > /proc/sys/vm/drop_caches\"")!=0)
               cout<<"\n sync failed";
       }
       string srcDest[2];       
       string word;
       stringstream stream(str);
       int i=0;
       while( getline(stream, word, '>') )
       {           
           stringstream streamTemp(word); string wordTemp; bool alternate=false;
           while(getline(streamTemp,wordTemp,'<'))
           {
               if(alternate)
                   srcDest[i]= wordTemp;    
               else
                   alternate=true;
           }
           i++;
       }
       cout<<"\n Source-destination pair:"<<srcDest[0]<<"\t"<<srcDest[1]<<"\n";
       dijkstra_implemenationOutputPrintedOnConsole(db,srcDest);
       db.close();
       ofstream outputFilePrintOnConsole;
       outputFilePrintOnConsole.open("OutputFilePrintOnConsole.txt", ios::app|ios::out);       
       outputFilePrintOnConsole<<"\t"<<srcDest[0]<<"\t"<<srcDest[1]<<"\n";
       outputFilePrintOnConsole.close();       
   }   

   std::ifstream file2(argv[2]);
   //Old Dijkstra algorithm 
   while (std::getline(file2, str))
   {
       Database db;
       // Open the database   
       if (!db.open(argv[1],true)) {
           cerr << "unable to open database " << argv[1] << endl;
       }                  
       
       if(runAsColdOrWarmCache==1)
       {
           if(system("sudo sh -c \"sync; echo 3 > /proc/sys/vm/drop_caches\"")!=0)
               cout<<"\n sync failed";
       }
       string srcDest[2];       
       string word;
       stringstream stream(str);
       int i=0;
       while( getline(stream, word, '>') )
       {           
           stringstream streamTemp(word); string wordTemp; bool alternate=false;
           while(getline(streamTemp,wordTemp,'<'))
           {
               if(alternate)
                   srcDest[i]= wordTemp;    
               else
                   alternate=true;
           }
           i++;
       }
       cout<<"\n Source-destination pair:"<<srcDest[0]<<"\t"<<srcDest[1]<<"\n";
       OldDijkstra_implemenation(db,srcDest);
       db.close();
       ofstream outputFileOldDijkstra;
       outputFileOldDijkstra.open("OutputFileOldDijkstra.txt", ios::app|ios::out);       
       outputFileOldDijkstra<<"\t"<<srcDest[0]<<"\t"<<srcDest[1]<<"\n";
       outputFileOldDijkstra.close();       
   }   

}
//---------------------------------------------------------------------------
