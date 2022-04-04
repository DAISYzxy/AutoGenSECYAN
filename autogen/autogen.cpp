
# include <stdlib.h>
# include <string>

# include <iostream>
# include <queue>
#include <fstream>

# include <typeinfo>
# include <map>
# include <unordered_map>

//
// Created by 朱心怡 on 2/3/2022.
//


// include free connex tree class
# include "FreeConnexTree.h"

// include the sql parser
# include "SqlParserLoader.h"

// include attributes converter
# include "AttrConvert.h"

// include SECYAN Link attr construction
# include "SECLink.h"


using namespace std;
using namespace hsql;

// Free-connex Tree Construction and Test Part
int main(){


  std::string query = "select o_orderkey, o_orderdate, o_shippriority, sum(l_extendedprice * (1 - l_discount)) as revenue from customer, orders, lineitem where c_mktsegment = 'automobile' and c_custkey = o_custkey and l_orderkey = o_orderkey and o_orderdate < date '1995-03-13' and l_shipdate > date '1995-03-13' group by o_orderkey, o_orderdate, o_shippriority;";

  // parse a given query
  hsql::SQLParserResult result;
  hsql::SQLParser::parse(query, &result);

  /*
  if (result.isValid()) {
    printf("Parsed successfully!\n");
    printf("Number of statements: %lu\n", result.size());

    for (auto i = 0u; i < result.size(); ++i) {
      // Print a statement summary.
      hsql::printStatementInfo(result.getStatement(i));
    }
    return 0;
  }
  */
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

  vector<int> tableLstIdx;
  vector<int> *pTableLstIdx = &tableLstIdx;

  cout << endl << "Table Index List: " << endl;
  getIdx(pTableLstIdx, tables);
  for (int i = 0; i < tableLstIdx.size(); i++){
    cout << tableLstIdx[i] << ", ";
  }
  cout << endl;


  unordered_map<int, vector<string>> relatedAttr;
  unordered_map<int, vector<string>> *pRelatedAttr = &relatedAttr;

  // Obtain the output attributes in the query
  vector<string> columnExp;
  vector<Expr*> operationExp;
  for (Expr* expr : *result_vct->selectList){
    if (!expr) break;
    if (expr->type == kExprColumnRef){
      string str = expr->name;
      columnExp.push_back(str);
      int expIdx = getAttrIdx(str);
      constructRelatedAttr(pRelatedAttr, expIdx, str);
    }
    else operationExp.push_back(expr);
  }
  

  cout << endl << "Output Column Attributes:" <<endl;
  for (int i = 0; i < columnExp.size(); i++){
    cout << columnExp[i] << endl;
  }

  vector<string> outputList;
  vector<string> *pOutputList = &outputList;
  for (int i = 0; i < operationExp.size(); i++){
    Expr* tmpExpr = operationExp[i];
    // cout << "tmpExpr is " << tmpExpr->type << "." << endl;
    normalExpr(tmpExpr, pOutputList);
  }

  cout << endl << "Output Operations:" <<endl;
  for (int i = 0; i < (*pOutputList).size(); i++){
    cout << (*pOutputList)[i] << endl;
  }


  // Obtain the Search Condition (where clause) -> iteratively divide the operation expression
  Expr* whereClause = result_vct->whereClause;
  vector<string> resultList;
  vector<string> *whereRst = &resultList;
  normalExpr(whereClause, whereRst);





  unordered_map<int, vector<int>> linkGraph;

  cout << endl << "Output Where Clause:" <<endl;
  for (int i = 0; i < (*whereRst).size(); i++){
    //cout << (*whereRst)[i] << endl;
    if ((*whereRst)[i] == "=")
    {
      if ((*whereRst)[i+1][0] != '\'' && (*whereRst)[i+2][0] != '\''){
        string exp1 = (*whereRst)[i+1];
        string exp2 = (*whereRst)[i+2];
        int exp1Idx = getAttrIdx(exp1);
        int exp2Idx = getAttrIdx(exp2);
        cout << "(" << exp1Idx << ", " << exp2Idx << ")" << endl;
        constructLinkGraph(&linkGraph, exp1Idx, exp2Idx);
        constructLinkGraph(&linkGraph, exp2Idx, exp1Idx);
        constructRelatedAttr(pRelatedAttr, exp1Idx, exp1);
        constructRelatedAttr(pRelatedAttr, exp2Idx, exp2);
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


  vector<int> tableHas;
  vector<int> *pTableHas = &tableHas;

  cout << endl << "Table Has: ";
  getAttrIdx(pTableHas, columnExp);
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



  unordered_map<string, string> templateTreeMap = loadTemplateTree();
  cout << endl << "Template Tree: " << endl;
  for(auto it = templateTreeMap.begin(); it != templateTreeMap.end(); it++){
      cout << it->first << ": " << it->second << endl;
  }



  string templateTree;
  string outTreeStr;
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
    constructTree(node, tree, linkGraph, inTree, hasOutAttr);
    tree->tranversal();
    cout << endl;
    bool same2template = false;
    string *pOutTreeStr = &outTreeStr;
    tree2String(node, pOutTreeStr);
    unordered_map<int, vector<int>> relatedAttrHeight;
    unordered_map<int, vector<int>> *pRelatedAttrHeight = &relatedAttrHeight;
    getAttrHeight(outTreeStr, pRelatedAttr, pRelatedAttrHeight);
    vector<string> linkIsOut;
    vector<string> *pLinkIsOut = &linkIsOut;
    vector<string> *pColumnOut = &columnExp;
    linkageIsOutputed(pLinkIsOut, pRelatedAttr, pColumnOut);

    cout << endl << "Construct Attribute Height Map: " << endl;
    constructAttrHeightMap(outTreeStr, pRelatedAttr, pRelatedAttrHeight, pColumnOut, pLinkIsOut);

    for(auto it = relatedAttrHeight.begin(); it != relatedAttrHeight.end(); it++){
        cout << it->first << ": ";
        for (int i = 0; i < relatedAttrHeight[it->first].size(); i++){
          cout << relatedAttrHeight[it->first][i] << ",";
        }
        cout << endl;
    }

    bool isFreeConnexTree = isFreeConnex(outTreeStr, pRelatedAttrHeight);
    if (isFreeConnexTree){
      cout << endl << "Free Connex!!!" << endl;
      for(auto it = templateTreeMap.begin(); it != templateTreeMap.end(); it++){
        same2template = isSameTree(it->second, outTreeStr);
        if (same2template){
          cout << "Same to template " << it->first << endl;
          templateTree = it->first;
          break;
        }
      }
      if (same2template) break;
    }
    else{
      cout << endl << "Not a free connex" << endl;
      return 0;
    }
  }

  int irole, iqn, ids;
  string address = "127.0.0.1";
  uint16_t port = 7766;
  e_role role = SERVER;

  cout << "Who are you? [0. Server, 1. Client]: ";
  cin >> irole;
  if (irole != 0 && irole != 1)
  {
      cerr << "Role error!" << endl;
      exit(1);
  }
  role = (e_role)irole;

  cout << "Establishing connection... ";
  gParty.Init(address, port, role);
  cout << "Finished!" << endl;
  if (templateTree == "Q3"){
    DataSize ds;
    cout << "Which TPCH data size to use? [0. 1MB, 1. 3MB, 2. 10MB, 3. 33MB, 4. 100MB]: ";
    cin >> ids;
    if (ids < 0 || ids >= 5)
    {
        cerr << "Data size selection error!" << endl;
        exit(1);
    }
    ds = (DataSize)ids;
    cout << "Start running query..." << endl;
    gParty.Tick("Running time");
    run_TemplateQ3(ds, true, groupByColumn, outTreeStr, relatedAttr);
    gParty.Tick("Running time");
    auto cost = gParty.GetCommCostAndResetStats();
    cout << "Communication cost: " << cost / 1024 / 1024.0 << " MB" << endl;
    cout << "Finished!" << endl;
    return 0;
  }
  
  return 0;
}

