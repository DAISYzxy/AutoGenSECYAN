# include <stdlib.h>
# include <string>
# include <iostream>

# include <typeinfo>
# include <string>

# include <map>
# include <queue>
# include <vector>
# include <unordered_map>

using namespace std;


static const map<const string, const string> linkKey = {
  {"12", "CUSTKEY"}, {"23", "ORDERKEY"}, {"34", "PARTKEY"}, {"35", "SUPPKEY"}, {"36", "SUPPKEY_PARTKEY"}
};


// get sql query attr index
void getIdx(vector<int> *tableLstIdx, vector<string> retrieveStr);
void getAttrIdx(vector<int> *tableHas, vector<string> groupBy);
int getAttrIdx(string exp);
string getTail(string attrName);

// LinkGraph functions
void hasOutputAttr(bool *hasOuts, vector<int> tableHas);
void constructLinkGraph(unordered_map<int, vector<int>> *linkGraph, int exp1Idx, int exp2Idx);
void constructRelatedAttr(unordered_map<int, vector<string>> *relatedAttr, int expIdx, string attrName);
void linkageIsOutputed(unordered_map<string bool> *linkIsOut, unordered_map<int, vector<string>> *relatedAttr);