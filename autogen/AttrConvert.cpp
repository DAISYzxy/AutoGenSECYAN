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
  string tableDict[6] = {"customer", "orders", "lineitem", "part", "supplier", "partsupp"};
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
  char prefixDict[5] = {'c', 'o', 'l', 'p', 's'};
  for (int i = 0; i < groupBy.size(); i++)
  {
    char prefix = groupBy[i][0];
    char prefix2 = groupBy[i][1];
    if (prefix == 'p' && prefix2 == 's'){
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
  char prefixDict[5] = {'c', 'o', 'l', 'p', 's'};
  char prefix = exp[0];
  char prefix2 = exp[1];
  if (prefix == 'p' && prefix2 == 's'){
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



void constructLinkGraph(unordered_map<int, vector<int>> *linkGraph, int exp1Idx, int exp2Idx){
  if ((*linkGraph).find(exp1Idx) == (*linkGraph).end()){
    vector<int> neighbourLink;
    neighbourLink.push_back(exp2Idx);
    (*linkGraph).insert(make_pair(exp1Idx, neighbourLink));
  }
  else{
    vector<int> neighbourLink = (*linkGraph)[exp1Idx];
    for (int i = 0; i < neighbourLink.size(); i++){
      if (neighbourLink[i] == exp2Idx) return;
    }
    (*linkGraph)[exp1Idx].push_back(exp2Idx);
  }
  return;
}



bool attrIsIn(string attrName, vector<string> *outputAttr){
  bool flag = false;
  for (int i = 0; i < (*outputAttr).size(); i++){
    if ((*outputAttr)[i] == attrName){
      flag = true;
      break;
    }
  }
  return flag;
}




bool attrIsIn(string attrName, vector<string> *outputAttr, vector<string> *linkIsOut){
  bool flag = false;
  bool isLinkageAndIsOut = false;
  string attrTail = getTail(attrName);
  for (int j = 0; j < (*linkIsOut).size(); j++){
    if ((*linkIsOut)[j] == attrTail){
      isLinkageAndIsOut = true;
      break;
    }
  }
  
  for (int i = 0; i < (*outputAttr).size(); i++){
    if ((*outputAttr)[i] == attrName || isLinkageAndIsOut){
      flag = true;
      break;
    }
  }
  return flag;
}





void linkageIsOutputed(vector<string> *linkIsOut, unordered_map<int, vector<string>> *relatedAttr, vector<string> *outputAttr){
  for(auto it = (*relatedAttr).begin(); it != (*relatedAttr).end(); it++){
    int tableIdx = it->first;
    vector<string> tmpRelated = it->second;
    for (int i = 0; i < tmpRelated.size(); i++){
      string attr = tmpRelated[i];
      bool attrInOutput = attrIsIn(attr, outputAttr);
      if (attrInOutput){
        string attrTail = getTail(attr);
        for (int i = 0; i < 4; i++){
          if (attrTail == linkKey[i]){
            //(*linkIsOut).push_back(attr);
            bool flag = true;
            for (int i = 0; i < (*linkIsOut).size(); i++){
              if ((*linkIsOut)[i] == attrTail){
                flag = false;
                break;
              }
            }
            if (flag) (*linkIsOut).push_back(attrTail);
          }
        }
      }
    }    
  }
}




void getAttrHeight(string strTree, unordered_map<int, vector<string>> *relatedAttr, unordered_map<int, vector<int>> *relatedAttrHeight){
  int height = 0;
  cout << endl << "Get Attr Height: " << endl;
  for (int i = 0; i < strTree.length(); i++){
    if (strTree[i] == 'C') height += 1;
    else{
      int tableIdx = int(strTree[i]) - int('0');
      for (int j = 0; j < (*relatedAttr)[tableIdx].size(); j++){
        if ((*relatedAttrHeight).find(tableIdx) == (*relatedAttrHeight).end()){
          vector<int> relatedHeight;
          relatedHeight.push_back(height);
          (*relatedAttrHeight).insert(make_pair(tableIdx, relatedHeight));
        }
        else{
          (*relatedAttrHeight)[tableIdx].push_back(height);
        }
      }
      for (int i = 0; i < (*relatedAttrHeight)[tableIdx].size(); i++)
      {
        cout << (*relatedAttrHeight)[tableIdx][i] << ",";
      }
      cout << endl;
    }
  }
}




void hasOutputAttr(bool *hasOuts, vector<int> tableHas){
  for (int i = 0; i < tableHas.size(); i++) hasOuts[tableHas[i]] = true;
  return;
}





void constructAttrHeightMap(string treeString, unordered_map<int, vector<string>> *relatedAttr, unordered_map<int, vector<int>> *relatedAttrHeight, vector<string> *outputAttr, vector<string> *linkIsOut){
  vector<string> linkOutputed;
  for (int i = 0; i < treeString.length(); i++){
    if (treeString[i] != 'C'){
      int tableIdx = int(treeString[i]) - int('0');
      vector<string> thisTableAttr = (*relatedAttr)[tableIdx];
      for (int i = 0; i < thisTableAttr.size(); i++){
        string attr = thisTableAttr[i];
        bool isOutput = attrIsIn(attr, outputAttr, linkIsOut);
        if (isOutput){
          string attrTail = getTail(attr);
          for (int t = 0; t < 4; t++){
            if (linkKey[t] == attrTail){
              // attr is linkage -> find whether has been tagged output before
              bool hasTaggedOutput = false;
              for (int j = 0; j < linkOutputed.size(); j++){
                if (linkOutputed[j] == attrTail){
                  hasTaggedOutput = true;
                  break;
                }
              }
              if (hasTaggedOutput) (*relatedAttrHeight)[tableIdx][i] = 9999;
              else linkOutputed.push_back(attrTail);
              break;
            }
          }
        }
        else{
          (*relatedAttrHeight)[tableIdx][i] = 9999;
        }
      }
    }
  }
}



bool isFreeConnex(string treeString, unordered_map<int, vector<int>> *relatedAttrHeight){
  int lastLayerMax = 0;
  int thisLayerMin = 9999;
  int thisLayerMax = 0;

  for (int i = 0; i < treeString.length(); i++){
    if (treeString[i] != 'C'){
      int tableIdx = int(treeString[i]) - int('0');
      for (int j = 0; j < (*relatedAttrHeight)[tableIdx].size(); j++){
        int attrHeight = (*relatedAttrHeight)[tableIdx][j];
        if (thisLayerMin > attrHeight) thisLayerMin = attrHeight;
        if (thisLayerMax < attrHeight) thisLayerMax = attrHeight;
      }
      if (lastLayerMax > thisLayerMin) return false;
    }
    else{
      lastLayerMax = thisLayerMax;
      thisLayerMax = 0;
      thisLayerMin = 9999;
    }
  }
  return true;
}