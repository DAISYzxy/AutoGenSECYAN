
# include <stdlib.h>
# include <string>
# include <string.h>

// include the sql parser
# include "SQLParser.h"

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


// Iteratively Tackle the Operation Expression
void opExpr(Expr* exp, vector<char*> *result) {

  if (exp->type == kExprOperator) {
    const auto iterFound = operatorToToken.find(exp->opType);
    const string str = (*iterFound).second;
    char* opStr = (char*)str.data();
    (*result).push_back(opStr);

    Expr* exp1 = exp->expr;
    opExpr(exp1, result);
    if (exp1->type == kExprColumnRef) (*result).push_back(exp1->name);

    Expr* exp2 = exp->expr2;
    opExpr(exp2, result);
    if (exp2->type == kExprColumnRef) (*result).push_back(exp2->name);
  }

  else return;
}


/*
// Iteratively tackle the where clause: ... AND ... AND ......
void iterWhereClause(Expr* whereClause) {

  vector<char*> whereRes;
  // AND's OperatorType is kOpAnd
  if (whereClause->type == kExprOperator) {
    const auto iterFound = operatorToToken.find(whereClause->opType);
    char* opStr = (*iterFound).second;
    whereRes.push_back(opStr);
  }
}
*/


// SQL-Parser Test
int main() {

  std::string query = "SELECT TOP 10 L_ORDERKEY, SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT)) AS REVENUE,O_ORDERDATE, O_SHIPPRIORITY FROM CUSTOMER, ORDERS, LINEITEM WHERE C_MKTSEGMENT = 'BUILDING' AND C_CUSTKEY = O_CUSTKEY AND L_ORDERKEY = O_ORDERKEY AND O_ORDERDATE < '1995-03-15' AND L_SHIPDATE > '1995-03-15' GROUP BY L_ORDERKEY, O_ORDERDATE, O_SHIPPRIORITY";

  // parse a given query
  hsql::SQLParserResult result;
  hsql::SQLParser::parse(query, &result);

  // check whether the parsing was successful

  if (result.isValid()) {
    printf("Parsed successfully!\n");
    printf("Number of statements: %lu\n", result.size());

    for (auto i = 0u; i < result.size(); ++i) {
      // Print a statement summary.
      // printStatementInfo(result.getStatement(i));

      ///*
      auto result_vct = (const SelectStatement *)result.getStatement(i);

      // Obtain the tables in the query
      auto tableList = result_vct->fromTable;
      vector<char*> tables;
      for (TableRef* tbl : *tableList->list) tables.push_back(tbl->name);
      cout << endl << "Selected Tables:" << endl;
      for (int i = 0; i < tables.size(); i++){
        cout << tables[i] << endl;
      }


      // Obtain the output attributes in the query
      vector<char*> columnExp;
      vector<Expr*> operationExp;
      for (Expr* expr : *result_vct->selectList){
        if (!expr) break;
        if (expr->type == kExprColumnRef) columnExp.push_back(expr->name);
        if (expr->type == kExprOperator) operationExp.push_back(expr);
      }

      cout << endl << "Output Column Attributes:" <<endl;
      for (int i = 0; i < columnExp.size(); i++){
        cout << columnExp[i] << endl;
      }


      // Obtain the Search Condition (where clause) -> iteratively divide the operation expression
      // Expr* whereClause = result_vct->whereClause;
      

      // Obtain the groupby condition
      vector<char*> groupByColumn;
      for (Expr* expr : *result_vct->groupBy->columns) groupByColumn.push_back(expr->name);
      cout << endl << "GroupBy Column Attributes:" <<endl;
      for (int i = 0; i < groupByColumn.size(); i++){
        cout << groupByColumn[i] << endl;
      }
      //*/

    }
    return 0;
  } else {
    fprintf(stderr, "Given string is not a valid SQL query.\n");
    fprintf(stderr, "%s (L%d:%d)\n",
            result.errorMsg(),
            result.errorLine(),
            result.errorColumn());
    return -1;
  }
}




/*
// Free-connex Tree Construction and Test Part
int main(){
  MNode node;
  MTree tree;
  node.Parent = nullptr;
  node.element = 1;
  node.tag = true;
  tree.init(&node);
  MNode node2, node3, node4, node5, node6, node7, node8, node9, node10;
  node2.element = 2, node3.element = 3, node4.element = 4, node5.element = 5, node6.element = 6;
  node2.tag = true, node3.tag = true, node4.tag = false, node5.tag = true;
  tree.putChild(&node2, &node), tree.putChild(&node3, &node), tree.putChild(&node4, &node);
  tree.putChild(&node6, &node3);
  tree.tranversal();
  cout << endl;
  if (tree.isFreeConnex())
    cout << "true!" << endl;
  else
    cout << "false!" << endl;
}


*/