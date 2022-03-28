
# include <stdlib.h>
# include <string>
# include <string.h>

// include the sql parser
# include "SQLParser.h"
#include <unordered_map>

// contains printing utilities
# include "util/sqlhelper.h"
# include "sql/statements.h"
# include <typeinfo>
#include <map>

//
// Created by 朱心怡 on 2/3/2022.
//

# include <iostream>
# include <queue>
# include "free_connex_tree.h"

using namespace std;
using namespace hsql;


void normalExpr(Expr* exp, vector<string> *result);
void opExpr(Expr* exp, vector<string> *result);



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





static const map<const OperatorType, const string> operatorToToken = {
    {kOpNone, "None"},     {kOpBetween, "BETWEEN"},
    {kOpCase, "CASE"},     {kOpCaseListElement, "CASE LIST ELEMENT"},
    {kOpPlus, "+"},        {kOpMinus, "-"},
    {kOpAsterisk, "*"},    {kOpSlash, "/"},
    {kOpPercentage, "%"},  {kOpCaret, "^"},
    {kOpEquals, "="},      {kOpNotEquals, "!="},
    {kOpLess, "<"},        {kOpLessEq, "<="},
    {kOpGreater, ">"},     {kOpGreaterEq, ">="},
    {kOpLike, "LIKE"},     {kOpNotLike, "NOT LIKE"},
    {kOpILike, "ILIKE"},   {kOpAnd, "AND"},
    {kOpOr, "OR"},         {kOpIn, "IN"},
    {kOpConcat, "CONCAT"}, {kOpNot, "NOT"},
    {kOpUnaryMinus, "-"},  {kOpIsNull, "IS NULL"},
    {kOpExists, "EXISTS"}};



void normalExpr(Expr* exp, vector<string> *result){
  if (exp->type == kExprColumnRef){
    string str = exp->name;
    (*result).push_back(str);
    //cout << "put: " << exp->name << endl;
  }
  if (exp->type == kExprLiteralString){
    string str = exp->name;
    str = "'" + str + "'";
    (*result).push_back(str);
    //cout << "put: " << exp->name << endl;
  }
  if (exp->type == kExprLiteralInt){
    string ival = to_string(exp->ival);
    (*result).push_back(ival);
    //cout << "put: " << new_ival << endl;
  }
  if (exp->type == kExprOperator){
    opExpr(exp, result);
  }
  if (exp->type == kExprFunctionRef){
    string str = exp->name;
    (*result).push_back(str);
    vector<Expr*>* expLst = exp->exprList;
    for (int i = 0; i < (*expLst).size(); i++)
    {
      Expr* tmpExp = (*expLst)[i];
      normalExpr(tmpExp, result);
    }
  }
}




void opExpr(Expr* exp, vector<string> *result) {

  if (exp->type == kExprOperator) {
    const auto iterFound = operatorToToken.find(exp->opType);
    const string str = (*iterFound).second;
    (*result).push_back(str);
    //cout << "put: " << opStr << endl;

    Expr* exp1 = exp->expr;
    normalExpr(exp1, result);

    Expr* exp2 = exp->expr2;
    normalExpr(exp2, result);
  }

  else return;
}




void getIdx(vector<int> *tableLstIdx, vector<string> retrieveStr){
  string tableDict[6] = {"ORDERS", "CUSTOMER", "LINEITEM", "PART", "SUPPLIER", "PARTSUPP"};
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
  char prefixDict[5] = {'O', 'C', 'L', 'P', 'S'};
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
  char prefixDict[5] = {'O', 'C', 'L', 'P', 'S'};
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



void hasOutputAttr(bool *hasOuts, vector<int> tableHas){
  for (int i = 0; i < tableHas.size(); i++) hasOuts[tableHas[i]] = true;
  return;
}



// Free-connex Tree Construction and Test Part
int main(){


  std::string query = "SELECT O_ORDERKEY, SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT)) AS REVENUE,O_ORDERDATE, O_SHIPPRIORITY FROM CUSTOMER, ORDERS, LINEITEM WHERE C_MKTSEGMENT = 'AUTOMOBILE' AND C_CUSTKEY = O_CUSTKEY AND L_ORDERKEY = O_ORDERKEY AND O_ORDERDATE < '1995-03-13' AND L_SHIPDATE > '1995-03-13' GROUP BY O_ORDERKEY, O_ORDERDATE, O_SHIPPRIORITY";

  // parse a given query
  hsql::SQLParserResult result;
  hsql::SQLParser::parse(query, &result);

  auto result_vct = (const SelectStatement *)result.getStatement(0);

  // Obtain the tables in the query
  auto tableList = result_vct->fromTable;
  vector<string> tables;
  for (TableRef* tbl : *tableList->list){
    string str = tbl->name;
    tables.push_back(str);
  }
  cout << endl << "Selected Tables:" << endl;
  for (int i = 0; i < tables.size(); i++){
    cout << tables[i] << endl;
  }


  // Obtain the output attributes in the query
  vector<string> columnExp;
  vector<Expr*> operationExp;
  for (Expr* expr : *result_vct->selectList){
    if (!expr) break;
    if (expr->type == kExprColumnRef){
      string str = expr->name;
      columnExp.push_back(str);
    }
    else operationExp.push_back(expr);
  }

  cout << endl << "Output Column Attributes:" <<endl;
  for (int i = 0; i < columnExp.size(); i++){
    cout << columnExp[i] << endl;
  }

  vector<string> outputList;
  vector<string> *outputOp = &outputList;
  for (int i = 0; i < operationExp.size(); i++){
    Expr* tmpExpr = operationExp[i];
    // cout << "tmpExpr is " << tmpExpr->type << "." << endl;
    normalExpr(tmpExpr, outputOp);
  }

  cout << endl << "Output Operations:" <<endl;
  for (int i = 0; i < (*outputOp).size(); i++){
    cout << (*outputOp)[i] << endl;
  }


  // Obtain the Search Condition (where clause) -> iteratively divide the operation expression
  Expr* whereClause = result_vct->whereClause;
  vector<string> resultList;
  vector<string> *whereRst = &resultList;
  normalExpr(whereClause, whereRst);





  unordered_map<int, vector<int>> linkGraph;

  cout << endl << "Output Where Clause:" <<endl;
  for (int i = 0; i < (*whereRst).size(); i++){
    cout << (*whereRst)[i] << endl;
    if ((*whereRst)[i] == "=")
    {
      if ((*whereRst)[i+1][0] != '\'' && (*whereRst)[i+2][0] != '\''){
        string exp1 = (*whereRst)[i+1];
        string exp2 = (*whereRst)[i+2];
        int exp1Idx = getAttrIdx(exp1);
        int exp2Idx = getAttrIdx(exp2);
        constructLinkGraph(&linkGraph, exp1Idx, exp2Idx);
        constructLinkGraph(&linkGraph, exp2Idx, exp1Idx);
      }
    }
  }


  // Obtain the groupby condition
  vector<string> groupByColumn;
  for (Expr* expr : *result_vct->groupBy->columns){
    string str = expr->name;
    groupByColumn.push_back(str);
  }
  cout << endl << "GroupBy Column Attributes:" <<endl;
  for (int i = 0; i < groupByColumn.size(); i++){
    cout << groupByColumn[i] << endl;
  }

  vector<int> tableLstIdx;
  vector<int> *pTableLstIdx = &tableLstIdx;

  cout << endl << "Table Index List: " << endl;
  getIdx(pTableLstIdx, tables);
  for (int i = 0; i < tableLstIdx.size(); i++){
    cout << tableLstIdx[i] << ", ";
  }
  cout << endl;


  vector<int> tableHas;
  vector<int> *pTableHas = &tableHas;

  cout << endl << "Table Has: ";
  getAttrIdx(pTableHas, groupByColumn);
  for (int i = 0; i < tableHas.size(); i++){
    cout << tableHas[i] << ", ";
  }
  cout << endl;


  cout << endl << "Link Graph: " << endl;
  for(auto it = linkGraph.begin(); it != linkGraph.end(); it++){
      cout << it->first << ": ";
      vector<int> tmpNeighbour = it->second;
      for (int i = 0; i < tmpNeighbour.size(); i++)
      {
        cout << tmpNeighbour[i] << ", ";
      }
      cout << endl;
  }
  cout << endl;




  int tableSize = tableLstIdx.size();
  bool *inTree = new bool[tableSize + 1];
  for (int i = 0; i < tableSize + 1; i++) inTree[i] = false;

  bool *hasOutAttr = new bool[tableSize + 1];
  for (int i = 0; i < tableSize + 1; i++) hasOutAttr[i] = false;
  hasOutputAttr(hasOutAttr, tableHas);

  cout << endl << "Has Output: " << endl;
  for (int i = 0; i < tableSize + 1; i++)
  {
    cout << hasOutAttr[i] << endl;
  }
  cout << endl;


  cout << endl;
  for (int i = 0; i < tableSize; i++)
  {
    for (int i = 0; i < tableSize + 1; i++) inTree[i] = false;
    MNode *node = new MNode;
    MTree *tree = new MTree;
    node->Parent = nullptr;
    node->element = tableLstIdx[i];
    node->tag = hasOutAttr[node->element];
    tree->init(node);
    inTree[node->element] = true;
    vector<int> neighbour = linkGraph[node->element];
    for (int i = 0; i < neighbour.size(); i++)
    {
      if (!inTree[neighbour[i]]){
        MNode *node2 = new MNode;
        node2->element = neighbour[i];
        node2->tag = hasOutAttr[node2->element];
        tree->putChild(node2, node);
        inTree[node2->element] = true;
      }
    }
    tree->tranversal();
    cout << endl;
    if (tree->isFreeConnex()){
      cout << "true!" << endl;
      break;
    }
    else
      cout << "false!" << endl;
  }

}

