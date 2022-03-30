# include "AttrConvert.h"


void constructRelatedAttr(unordered_map<int, vector<string>> *relatedAttr, int expIdx, string attrName){
  if ((*relatedAttr).find(expIdx) == (*relatedAttr).end()){
    vector<string> related;
    related.push_back(attrName);
    (*relatedAttr).insert(make_pair(expIdx, related));
  }
  else{
    bool flag = true;
    for (int i = 0; i < (*relatedAttr)[expIdx].size(); i++){
      if ((*relatedAttr)[expIdx][i] == attrName)
      {
        flag = false;
        break;
      }
    }
    if (flag) (*relatedAttr)[expIdx].push_back(attrName);
  }
  for (int i = 0; i < (*relatedAttr)[expIdx].size(); i++)
  {
    cout << (*relatedAttr)[expIdx][i] << ",";
  }
  cout << endl;
  return;
}



void getIdx(vector<int> *tableLstIdx, vector<string> retrieveStr){
  string tableDict[6] = {"CUSTOMER", "ORDERS", "LINEITEM", "PART", "SUPPLIER", "PARTSUPP"};
  for (int j = 0; j < retrieveStr.size(); j++){
    for (int i = 0; i < 6; i++){
      if (retrieveStr[j] == tableDict[i]){
        (*tableLstIdx).push_back(i+1);
        break;
      }
    }
  }
  return;
}


void getAttrIdx(vector<int> *tableHas, vector<string> groupBy){
  char prefixDict[5] = {'C', 'O', 'L', 'P', 'S'};
  for (int i = 0; i < groupBy.size(); i++)
  {
    char prefix = groupBy[i][0];
    char prefix2 = groupBy[i][1];
    if (prefix == 'P' && prefix2 == 'S'){
      (*tableHas).push_back(6);
    }

    else{
      for (int j = 0; j < 5; j++)
      {
        if (prefix == prefixDict[j]){
          (*tableHas).push_back(j+1);
          break;
        }
      }
    }
  }
  return;
}


int getAttrIdx(string exp){
  char prefixDict[5] = {'C', 'O', 'L', 'P', 'S'};
  char prefix = exp[0];
  char prefix2 = exp[1];
  if (prefix == 'P' && prefix2 == 'S'){
    return 6;
  }

  else{
    for (int j = 0; j < 5; j++)
    {
      if (prefix == prefixDict[j]){
        return j+1;
      }
    }
  }
  return 0;
}

string getTail(string attrName){
  for (int i = 0; i < attrName.length(); i++){
    if (attrName[i] == '_') return attrName.substr(i + 1, attrName.length());
  }
  return attrName;
}


void hasOutputAttr(bool *hasOuts, vector<int> tableHas){

  for (int i = 0; i < tableHas.size(); i++) hasOuts[tableHas[i]] = true;
  return;
}


void constructLinkGraph(unordered_map<int, vector<int>> *linkGraph, int exp1Idx, int exp2Idx){
  if ((*linkGraph).find(exp1Idx) == (*linkGraph).end()){
    vector<int> neighbourLink;
    neighbourLink.push_back(exp2Idx);
    (*linkGraph).insert(make_pair(exp1Idx, neighbourLink));
  }
  else{
    (*linkGraph)[exp1Idx].push_back(exp2Idx);
  }
  return;
}


void linkageIsOutputed(unordered_map<string bool> *linkIsOut, unordered_map<int, vector<string>> *relatedAttr){
  for(auto it = (*relatedAttr).begin(); it != (*relatedAttr).end(); it++){
    int tableIdx = it->first;
    vector<string> tmpRelated = it->second;
    for (int i = 0; i < tmpRelated.size(); i++){
      string attr = tmpRelated[i];
      string attrTail = getTail(attr);
      
    }
  }
}