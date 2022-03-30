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


bool MTree::isFreeConnex(){
  queue<MNode*> q1, q2;
  if (!this->root->tag){
    return false;
  }
  else{
    q1.push(this->root);
    int lastLayerFlag = 1;
    return(this->isFreeConnex(lastLayerFlag, &q1, &q2));
  }
}


bool MTree::isFreeConnex(int lastLayerFlag, queue<MNode*> *q1, queue<MNode*> *q2) {
  // cout << "q1's size: "<< (*q1).size() << endl;
  if ((*q1).size() == 0)
    return true;
  MNode *first = (*q1).front();
  int thisLayerFlag = first->tag;
  if (lastLayerFlag == 0 && thisLayerFlag == 1){
    return false;
  }
  for (int i = 0; i < first->children.size(); i++){
    q2->push(first->children[i]);
  }
  (*q1).pop();
  while ((*q1).size() > 0){
    MNode* tmp = (*q1).front();
    if (int(tmp->tag) != thisLayerFlag)
      return false;
    for (int i = 0; i < tmp->children.size(); i++){
      q2->push(tmp->children[i]);
    }
    (*q1).pop();
  }
  return isFreeConnex(thisLayerFlag, q2, q1);
}


unordered_map<string, string> loadTemplateTree(){
  unordered_map<string, string> templateTreeMap;
  ifstream myfile("templateTree.txt");
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

bool isSameTree(string templateTree, string tree){
  if (templateTree.length() != tree.length()) return false;
  for (int i = 0; i < tree.length(); i++){
    if (templateTree[i] == 'C' || tree[i] == 'C'){
      if (templateTree[i] != tree[i]) return false;
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