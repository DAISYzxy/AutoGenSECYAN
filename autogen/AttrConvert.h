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


static const string linkKey[4] = {"custkey", "orderkey", "partkey", "suppkey"};



// get sql query attr index
void getIdx(vector<int> *tableLstIdx, vector<string> retrieveStr);
void getAttrIdx(vector<int> *tableHas, vector<string> groupBy);
int getAttrIdx(string exp);
string getTail(string attrName);

// LinkGraph functions
void hasOutputAttr(bool *hasOuts, vector<int> tableHas);
bool attrIsIn(string attrName, vector<string> *outputAttr);
bool attrIsIn(string attrName, vector<string> *outputAttr, vector<string> *linkIsOut);
void constructLinkGraph(unordered_map<int, vector<int>> *linkGraph, int exp1Idx, int exp2Idx);
void constructRelatedAttr(unordered_map<int, vector<string>> *relatedAttr, int expIdx, string attrName);
void linkageIsOutputed(vector<string> *linkIsOut, unordered_map<int, vector<string>> *relatedAttr, vector<string> *outputAttr);
void getAttrHeight(string strTree, unordered_map<int, vector<string>> *relatedAttr, unordered_map<int, vector<int>> *relatedAttrHeight);
void constructAttrHeightMap(string treeString, unordered_map<int, vector<string>> *relatedAttr, unordered_map<int, vector<int>> *relatedAttrHeight, vector<string> *outputAttr, vector<string> *linkIsOut);
bool isFreeConnex(string treeString, unordered_map<int, vector<int>> *relatedAttrHeight);


