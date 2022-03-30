//
// Created by 朱心怡 on 8/3/2022.
//

# include <iostream>
#include <fstream>

# include <typeinfo>
# include <string>

# include <map>
# include <queue>
# include <vector>
# include <unordered_map>

using namespace std;

typedef struct MNode {
  int element;
  bool tag = false;
  vector<MNode*> children;
  MNode *Parent;
} MNode;

class MTree {
 private:
  MNode *root;
 public:
  void init(MNode *root);
  void putChild(MNode* node,MNode* parent);
  void putChildren(vector<MNode*> nodes, MNode *parent);
  void tranversal(MNode *root);
  void tranversal();
  int getMaxDepth(MNode *root,vector<MNode*> nodes);
  bool isFreeConnex();
  bool isFreeConnex(int LastLayerFlag, queue<MNode*> *q1, queue<MNode*> *q2);
};

// construct tree with linkGraph
bool allNeighbourInTree(vector<int> neighbour, bool *inTree);
void constructTree(MNode *node, MTree *tree, unordered_map<int, vector<int>> linkGraph, bool *inTree, bool *hasOutAttr);

// tree2string functions
unordered_map<string, string> loadTemplateTree();
void tree2String(MNode *root, string *outTreeStr);
bool isSameTree(string templateTree, string tree);