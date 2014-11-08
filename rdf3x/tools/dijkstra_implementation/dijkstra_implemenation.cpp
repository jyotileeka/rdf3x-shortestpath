#include <queue>
#include <vector>
#include <iostream>
#include "tools/rdf3xquery.h"
using namespace std;

int main(int argc,char* argv[])
{
    Database db;
    string query; //would be included later somehow
    bool explain =false; //the explaination which would be false
    db=argv[1]; //the value from from argument 1 gets passed into db
    
    query="select ?p where { ?s ?p ?o }";
    runQuery(Database& db,const string& query,bool explain);
    
    
}
