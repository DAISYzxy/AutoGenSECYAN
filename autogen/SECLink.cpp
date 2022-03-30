# include "SECLink.h"



void getAttrHeight(string strTree, unordered_map<int, vector<string>> *relatedAttr, unordered_map<int, vector<int>> *relatedAttrHeight){
  int height = 0;
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