# include <stdlib.h>
# include <string>
# include <string.h>
# include <iostream>
# include <queue>
# include "FreeConnexTree.h"

using namespace std;

void MTree::init(MNode *root) { this->root = root; }


void MTree::putChild(MNode *node, MNode *parent) {
  parent->children.push_back(node);
  node->Parent = parent;
}


void MTree::putChildren(vector<MNode *> nodes, MNode *parent) {
  for (int i = 0; i < nodes.size(); ++i) {
    putChild(nodes[i], parent);
  }
}


void MTree::tranversal() {
  this->tranversal(this->root);
}


void MTree::tranversal(MNode *root) {
  vector<MNode *> nodes = root->children;
  cout << root->element << ",";
  for (int i = 0; i < nodes.size(); ++i) {
    if (nodes[i]->children.size() > 0)
      tranversal(nodes[i]);
    else {
      cout << nodes[i]->element << ",";
      //cout << nodes[i]->tag << ",";
    }
  }
}


int MTree::getMaxDepth(MNode *root,vector<MNode*> nodes) {
  auto iResult = 0;

  return iResult;
}



unordered_map<string, string> loadTemplateTree(){
  unordered_map<string, string> templateTreeMap;
  ifstream myfile("../autogen/templateTree.txt");
  if (!myfile.is_open())
  {
    cout << "can not open this file" << endl;
    return templateTreeMap;
  }
  string str = "";
  while (myfile.good() && !myfile.eof()) {
    myfile >> str;
    for (int i = 0; i < str.length(); i++)
    {
      if (str[i] == ':'){
        string key = str.substr(0, i);
        string value = str.substr(i + 1, str.length());
        templateTreeMap.insert(make_pair(key, value));
      }
    }
  }
  myfile.close();
  return templateTreeMap;
}

void tree2String(MNode *root, string *outTreeStr){
  (*outTreeStr) += to_string(root->element);
  vector<MNode *> nodes = root->children;
  if (nodes.size() > 0) (*outTreeStr) += "C";
  for (int i = 0; i < nodes.size(); ++i) {
    if (nodes[i]->children.size() > 0)
      tree2String(nodes[i], outTreeStr);
    else {
      (*outTreeStr) += to_string(nodes[i]->element);
    }
  }
}

bool isSameTree(string QName, string templateTree, string tree, vector<string> outputList){
  if (templateTree.length() != tree.length()) return false;
  for (int i = 0; i < tree.length(); i++){
    if (templateTree[i] == 'C' || tree[i] == 'C'){
      if (templateTree[i] != tree[i]) return false;
    }  
  }
  if (QName == "Q3" || QName == "Q10"){
    int root = int(tree[0]) - int('0');
    char rootIdx;
    if (root < 6){
      rootIdx = idxKey[root - 1];
    }
    for (int i = 0; i < outputList.size(); i++){
      if (root == 6){
        if(outputList[i][0] != 'p' || outputList[i][0] != 's') return false;
      }
      else{
        if (outputList[i][0] != rootIdx) return false;
      }
    }
  }
  return true;
}


bool allNeighbourInTree(vector<int> neighbour, bool *inTree){
  for (int i = 0; i < neighbour.size(); i++){
    int nodeIdx = neighbour[i];
    if (!inTree[nodeIdx]) return false;
  }
  return true;
}

void constructTree(MNode *node, MTree *tree, unordered_map<int, vector<int>> linkGraph, bool *inTree, bool *hasOutAttr){
  vector<int> neighbour = linkGraph[node->element];
  bool allInTree = allNeighbourInTree(neighbour, inTree);
  if (allInTree) return;
  else{
    for (int i = 0; i < neighbour.size(); i++){
      if (!inTree[neighbour[i]]){
        MNode *node2 = new MNode;
        node2->element = neighbour[i];
        node2->tag = hasOutAttr[node2->element];
        tree->putChild(node2, node);
        inTree[node2->element] = true;
      }
    }
    vector<MNode*> children = node->children;
    for (int i = 0; i < children.size(); i++){
      constructTree(children[i], tree, linkGraph, inTree, hasOutAttr);
    }
  }
}