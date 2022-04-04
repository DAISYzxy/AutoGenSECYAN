# include "SECLink.h"

using namespace std;
using namespace SECYAN;


size_t NumRows[RTOTAL][DTOTAL] = {
	{150, 450, 1500, 4950, 15000},
	{1500, 4500, 15000, 49500, 150000},
	{6005, 17973, 60175, 198344, 595215},
	{200, 600, 2000, 6600, 20000},
	{10, 30, 100, 330, 1000},
	{800, 2400, 8000, 26400, 80000}};





std::string filename[] = {
	"customer.tbl",
	"orders.tbl",
	"lineitem.tbl",
	"part.tbl",
	"supplier.tbl",
	"partsupp.tbl"};



std::string newDatapath[] = {
	"../../SECYAN/data/1MB/",
	"../../SECYAN/data/3MB/",
	"../../SECYAN/data/10MB/",
	"../../SECYAN/data/33MB/",
	"../../SECYAN/data/100MB/"};



inline Relation::RelationInfo GetTemplateRI(RelationName rn, DataSize ds, e_role owner, vector<string> attrNames, vector<Relation::DataType> attrTypes)
{
	Relation::RelationInfo ri = {
		owner,
		false,
		attrNames,
		attrTypes,
		NumRows[rn][ds],
		false};
	return ri;
}



inline std::string newGetFilePath(RelationName rn, DataSize ds)
{
	return newDatapath[ds] + filename[rn];
}



static const map<const int, const RelationName> idx2RelationName = {
  {1, CUSTOMER}, {2, ORDERS}, {3, LINEITEM}, {4, PART}, {5, SUPPLIER}, {6, PARTSUPP}};


static const map<const int, const e_role> idx2Role = {
  {1, SERVER}, {2, CLIENT}, {3, SERVER}, {4, CLIENT}, {5, CLIENT}, {6, CLIENT}};


static const map<const string, const Relation::DataType> attrName2Datatype = {
  {"o_orderkey", Relation::INT}, {"o_custkey", Relation::INT}, {"o_orderdate", Relation::DATE}, {"o_shippriority", Relation::STRING},
  {"o_totalprice", Relation::DECIMAL}, {"o_year", Relation::INT}, {"o_orderstatus", Relation::INT}, {"o_priority", Relation::STRING},
  {"o_clerk", Relation::STRING}, {"o_comment", Relation::STRING},
  {"c_custkey", Relation::INT}, {"c_name", Relation::STRING}, {"c_nationkey", Relation::INT}, {"c_address", Relation::STRING},
  {"c_phone", Relation::STRING}, {"c_acctbal", Relation::DECIMAL}, {"c_mktsegment", Relation::STRING}, {"c_comment", Relation::STRING},
  {"l_orderkey", Relation::INT}, {"l_suppkey", Relation::INT}, {"l_partkey", Relation::INT}, {"l_returnflag", Relation::STRING},
  {"l_linenumber", Relation::INT}, {"l_quantity", Relation::DECIMAL}, {"l_extendedprice", Relation::DECIMAL}, {"l_discount", Relation::DECIMAL},
  {"l_tax", Relation::DECIMAL}, {"l_linestatus", Relation::STRING}, {"l_shipdate", Relation::DATE}, {"l_commitdate", Relation::DATE},
  {"l_receiptdate", Relation::DATE}, {"l_shippinstruct", Relation::STRING}, {"l_shipmode", Relation::STRING}, {"l_comment", Relation::STRING},
  {"p_partkey", Relation::INT}, {"p_name", Relation::STRING}, {"p_mfgr", Relation::STRING}, {"p_brand", Relation::STRING},
  {"p_type", Relation::STRING}, {"p_size", Relation::INT}, {"p_container", Relation::STRING}, {"p_retailprice", Relation::DECIMAL},
  {"p_comment", Relation::STRING},
  {"s_suppkey", Relation::INT}, {"s_name", Relation::STRING}, {"s_address", Relation::STRING}, {"s_nationkey", Relation::INT},
  {"s_phone", Relation::STRING}, {"s_acctbal", Relation::DECIMAL}, {"s_comment", Relation::STRING},
  {"ps_suppkey", Relation::INT}, {"ps_partkey", Relation::INT}, {"ps_availqty", Relation::INT}, {"ps_supplycost", Relation::DECIMAL},
  {"ps_comment", Relation::STRING}
};



static const map<const int, const bool> idx2AnnotInfo = {
  {1, true}, {2, true}, {3, false}, {4, true}, {5, true}, {6, false}};






void run_TemplateQ3(DataSize ds, bool printResult, vector<string> groupByList, string treeString, unordered_map<int, vector<string>> relatedAttr)
{
  // treeString: 2C13 -> 2, 1, 3
  // 2 -> create ORDERS table
  vector<int> tableIdx;
  for (int i = 0; i < treeString.length(); i++){
    if (treeString[i] != 'C'){
      int newTableIdx = treeString[i];
      tableIdx.push_back(newTableIdx);
    }
  }


  int tableIdx1 = tableIdx[0];
  vector<string> tableAttr1 = relatedAttr[tableIdx1];
  vector<Relation::DataType> attrTypes1;
  for (int i = 0; i < tableAttr1.size(); i++){
    attrTypes1.push_back(attrName2Datatype[tableAttr1[i]]);
  }
  RelationName rn1 = idx2RelationName[tableIdx1];
  e_role owner1 = idx2Role[tableIdx1];
  auto table1_ri = GetTemplateRI(rn1, ds, owner1, tableAttr1, attrTypes1);
  bool secondInAI = idx2AnnotInfo[tableIdx1];
  Relation::AnnotInfo table1_ai = {true, secondInAI};
  Relation table1(table1_ri, table1_ai);
  auto filePath = GetFilePath(rn1, ds);
  if (tableIdx1 == 2) table1.LoadData(filePath.c_str(), "q3_annot");
  if (tableIdx1 == 3){
    if (tableIdx[1] == 6 || tableIdx[2] == 6){
      table1.LoadData(filePath.c_str(), "q9_annot2");
    }
    else{
      table1.LoadData(filePath.c_str(), "q3_annot");
      if (!secondInAI) table1.Aggregate();
    }
  }



  int tableIdx2 = tableIdx[1];
  vector<string> tableAttr2 = relatedAttr[tableIdx2];
  vector<Relation::DataType> attrTypes2;
  for (int i = 0; i < tableAttr2.size(); i++){
    attrTypes2.push_back(attrName2Datatype[tableAttr2[i]]);
  }
  RelationName rn2 = idx2RelationName[tableIdx2];
  e_role owner2 = idx2Role[tableIdx2];
  auto table2_ri = GetTemplateRI(rn2, ds, owner2, tableAttr2, attrTypes2);
  bool secondInAI = idx2AnnotInfo[tableIdx2];
  Relation::AnnotInfo table2_ai = {true, secondInAI};
  Relation table2(table2_ri, table2_ai);
  auto filePath = GetFilePath(rn2, ds);
  if (tableIdx2 <= 3) table2.LoadData(filePath.c_str(), "q3_annot");
  if (tableIdx2 == 4) table2.LoadData(filePath.c_str(), "q8_annot1");
  if (tableIdx2 == 5) table2.LoadData(filePath.c_str(), "q9_annot");
  if (tableIdx2 == 6) table2.LoadData(filePath.c_str(), "q9_annot2");
  if (!secondInAI) table2.Aggregate();




  int tableIdx3 = tableIdx[2];
  vector<string> tableAttr3 = relatedAttr[tableIdx3];
  vector<Relation::DataType> attrTypes3;
  for (int i = 0; i < tableAttr3.size(); i++){
    attrTypes3.push_back(attrName2Datatype[tableAttr3[i]]);
  }
  RelationName rn3 = idx2RelationName[tableIdx3];
  e_role owner3 = idx2Role[tableIdx3];
  auto table3_ri = GetTemplateRI(rn3, ds, owner3, tableAttr3, attrTypes3);
  bool secondInAI = idx2AnnotInfo[tableIdx3];
  Relation::AnnotInfo table3_ai = {true, secondInAI};
  Relation table3(table3_ri, table3_ai);
  auto filePath = GetFilePath(rn3, ds);
  if (tableIdx3 <= 3) table3.LoadData(filePath.c_str(), "q3_annot");
  if (tableIdx3 == 4) table3.LoadData(filePath.c_str(), "q8_annot1");
  if (tableIdx3 == 5) table3.LoadData(filePath.c_str(), "q9_annot");
  if (tableIdx3 == 6) table3.LoadData(filePath.c_str(), "q9_annot2");
  if (!secondInAI) table3.Aggregate();

  
  if (tableIdx1 == 2){
    table1.SemiJoin(table2, "o_custkey", "c_custkey");
    table1.SemiJoin(table3, "o_orderkey", "l_orderkey");
  }

  if (tableIdx1 == 3){
    if (tableIdx2 == 2) table1.SemiJoin(table2, "l_orderkey", "o_orderkey");
    if (tableIdx2 == 4) table1.SemiJoin(table2, "l_partkey", "p_partkey");
    if (tableIdx2 == 5) table1.SemiJoin(table2, "l_suppkey", "s_suppkey");
    if (tableIdx2 == 6){
      vector<string> ps_joinAttrs = {"ps_partkey", "ps_suppkey"};
      vector<string> line_joinAttrs = {"l_partkey", "l_suppkey"};
      table1.SemiJoin(table2, line_joinAttrs, ps_joinAttrs);
    }
    if (tableIdx3 == 2) table1.SemiJoin(table3, "l_orderkey", "o_orderkey");
    if (tableIdx3 == 4) table1.SemiJoin(table3, "l_partkey", "p_partkey");
    if (tableIdx3 == 5) table1.SemiJoin(table3, "l_suppkey", "s_suppkey");
    if (tableIdx3 == 6){
      vector<string> ps_joinAttrs = {"ps_partkey", "ps_suppkey"};
      vector<string> line_joinAttrs = {"l_partkey", "l_suppkey"};
      table1.SemiJoin(table3, line_joinAttrs, ps_joinAttrs);
    }
  }

	table1.Aggregate(groupByList);
	table1.RevealAnnotToOwner();
	if (printResult)
		table1.Print();
}