# include "SECLink.h"

using namespace SECYAN;
using namespace std;


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
	"../SECYAN/data/1MB/",
	"../SECYAN/data/3MB/",
	"../SECYAN/data/10MB/",
	"../SECYAN/data/33MB/",
	"../SECYAN/data/100MB/"};



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



inline std::string GetFilePath(RelationName rn, DataSize ds)
{
	return newDatapath[ds] + filename[rn];
}



map<int, RelationName> idx2RelationName = {
  {1, CUSTOMER}, {2, ORDERS}, {3, LINEITEM}, {4, PART}, {5, SUPPLIER}, {6, PARTSUPP}};


map<int, e_role> idx2Role = {
  {1, SERVER}, {2, CLIENT}, {3, SERVER}, {4, CLIENT}, {5, CLIENT}, {6, CLIENT}};


map<string, Relation::DataType> attrName2Datatype = {
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



map<int, bool> idx2AnnotInfo = {
  {1, true}, {2, true}, {3, false}, {4, true}, {5, true}, {6, false}};






void run_TemplateQ3(DataSize ds, bool printResult, vector<string> groupByList, string treeString, unordered_map<int, vector<string>> relatedAttr)
{
  // treeString: 2C13 -> 2, (1, 3)
  // 2 -> create ORDERS table
  //std::cout << std::endl << "Table Index:" << std::endl;
  vector<int> tableIdx;
  for (int i = 0; i < treeString.length(); i++){
    if (treeString[i] != 'C'){
      int newTableIdx = int(treeString[i]) - int('0');;
      tableIdx.push_back(newTableIdx);
      //std::cout << newTableIdx << std::endl;
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
  bool firstInAI1 = idx2AnnotInfo[tableIdx1];
  Relation::AnnotInfo table1_ai = {firstInAI1, true};
  Relation table1(table1_ri, table1_ai);
  auto filePath1 = GetFilePath(rn1, ds);
  if (tableIdx1 == 2) table1.LoadData(filePath1.c_str(), "q3_annot");
  if (tableIdx1 == 3){
    if (tableIdx[1] == 6 || tableIdx[2] == 6){
      table1.LoadData(filePath1.c_str(), "q9_annot2");
    }
    else{
      table1.LoadData(filePath1.c_str(), "q3_annot");
    }
  }
  if (!firstInAI1) table1.Aggregate();
  
  /*
  std::cout << "Table1 Name:" << tableIdx1 << std::endl;
  std::cout << "Table1 second boolean:" << firstInAI1 << std::endl;
  std::cout << "Table1 related Attr:" << std::endl;
  for (int i = 0; i < tableAttr1.size(); i++){
    std::cout << tableAttr1[i] << std::endl;
  }*/



  int tableIdx2 = tableIdx[1];
  vector<string> tableAttr2 = relatedAttr[tableIdx2];
  vector<Relation::DataType> attrTypes2;
  for (int i = 0; i < tableAttr2.size(); i++){
    attrTypes2.push_back(attrName2Datatype[tableAttr2[i]]);
  }
  RelationName rn2 = idx2RelationName[tableIdx2];
  e_role owner2 = idx2Role[tableIdx2];
  auto table2_ri = GetTemplateRI(rn2, ds, owner2, tableAttr2, attrTypes2);
  bool firstInAI2 = idx2AnnotInfo[tableIdx2];
  Relation::AnnotInfo table2_ai = {firstInAI2, true};
  Relation table2(table2_ri, table2_ai);
  auto filePath2 = GetFilePath(rn2, ds);
  if (tableIdx2 <= 3) table2.LoadData(filePath2.c_str(), "q3_annot");
  if (tableIdx2 == 4) table2.LoadData(filePath2.c_str(), "q8_annot1");
  if (tableIdx2 == 5) table2.LoadData(filePath2.c_str(), "q9_annot");
  if (tableIdx2 == 6) table2.LoadData(filePath2.c_str(), "q9_annot2");
  if (!firstInAI2) table2.Aggregate();




  int tableIdx3 = tableIdx[2];
  vector<string> tableAttr3 = relatedAttr[tableIdx3];
  vector<Relation::DataType> attrTypes3;
  for (int i = 0; i < tableAttr3.size(); i++){
    attrTypes3.push_back(attrName2Datatype[tableAttr3[i]]);
  }
  RelationName rn3 = idx2RelationName[tableIdx3];
  e_role owner3 = idx2Role[tableIdx3];
  auto table3_ri = GetTemplateRI(rn3, ds, owner3, tableAttr3, attrTypes3);
  bool firstInAI3 = idx2AnnotInfo[tableIdx3];
  Relation::AnnotInfo table3_ai = {firstInAI3, true};
  Relation table3(table3_ri, table3_ai);
  auto filePath3 = GetFilePath(rn3, ds);
  if (tableIdx3 <= 3) table3.LoadData(filePath3.c_str(), "q3_annot");
  if (tableIdx3 == 4) table3.LoadData(filePath3.c_str(), "q8_annot1");
  if (tableIdx3 == 5) table3.LoadData(filePath3.c_str(), "q9_annot");
  if (tableIdx3 == 6) table3.LoadData(filePath3.c_str(), "q9_annot2");
  if (!firstInAI3) table3.Aggregate();

  
  if (tableIdx1 == 2){
    if (tableIdx2 == 1){
      table1.SemiJoin(table2, "o_custkey", "c_custkey");
      table1.SemiJoin(table3, "o_orderkey", "l_orderkey");
    }
    if (tableIdx2 == 3){
      table1.SemiJoin(table3, "o_custkey", "c_custkey");
      table1.SemiJoin(table2, "o_orderkey", "l_orderkey");
    }
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



void run_TemplateQ10(DataSize ds, bool printResult, vector<string> groupByList, string treeString, unordered_map<int, vector<string>> relatedAttr)
{
  // treeString: 1C2C3 -> 1, 2, 3
  // 1 -> create customer table
  vector<int> tableIdx;
  for (int i = 0; i < treeString.length(); i++){
    if (treeString[i] != 'C'){
      int newTableIdx = int(treeString[i]) - int('0');;
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
  bool firstInAI1 = idx2AnnotInfo[tableIdx1];
  Relation::AnnotInfo table1_ai = {firstInAI1, true};
  Relation table1(table1_ri, table1_ai);
  auto filePath1 = GetFilePath(rn1, ds);
  if (tableIdx1 < 4) table1.LoadData(filePath1.c_str(), "q10_annot");
  if (tableIdx1 == 4) table1.LoadData(filePath1.c_str(), "q9_annot");
  if (tableIdx1 == 5) table1.LoadData(filePath1.c_str(), "q8_annot1");
  if (tableIdx1 == 6) table1.LoadData(filePath1.c_str(), "q9_annot2");
  if (!firstInAI1) table1.Aggregate();

  
  /*
  std::cout << "Table1 Name:" << tableIdx1 << std::endl;
  std::cout << "Table1 second boolean:" << firstInAI1 << std::endl;
  std::cout << "Table1 related Attr:" << std::endl;
  for (int i = 0; i < tableAttr1.size(); i++){
    std::cout << tableAttr1[i] << std::endl;
  }*/



  int tableIdx2 = tableIdx[1];
  int tableIdx3 = tableIdx[2];
  vector<string> tableAttr2 = relatedAttr[tableIdx2];
  vector<Relation::DataType> attrTypes2;
  for (int i = 0; i < tableAttr2.size(); i++){
    attrTypes2.push_back(attrName2Datatype[tableAttr2[i]]);
  }
  RelationName rn2 = idx2RelationName[tableIdx2];
  e_role owner2 = idx2Role[tableIdx2];
  auto table2_ri = GetTemplateRI(rn2, ds, owner2, tableAttr2, attrTypes2);
  bool firstInAI2 = idx2AnnotInfo[tableIdx2];
  Relation::AnnotInfo table2_ai = {firstInAI2, true};
  Relation table2(table2_ri, table2_ai);
  auto filePath2 = GetFilePath(rn2, ds);
  if (tableIdx2 < 3) table2.LoadData(filePath2.c_str(), "q10_annot");
  if (tableIdx2 == 3){
    if (tableIdx1 == 6 || tableIdx3 == 6) table2.LoadData(filePath2.c_str(), "q9_annot2");
    else table2.LoadData(filePath2.c_str(), "q10_annot");
  }
  if (tableIdx2 == 4) table2.LoadData(filePath2.c_str(), "q9_annot");
  if (tableIdx2 == 5) table2.LoadData(filePath2.c_str(), "q8_annot1");
  if (tableIdx2 == 6) table2.LoadData(filePath2.c_str(), "q9_annot2");
  if (!firstInAI2) table2.Aggregate();




  vector<string> tableAttr3 = relatedAttr[tableIdx3];
  vector<Relation::DataType> attrTypes3;
  for (int i = 0; i < tableAttr3.size(); i++){
    attrTypes3.push_back(attrName2Datatype[tableAttr3[i]]);
  }
  RelationName rn3 = idx2RelationName[tableIdx3];
  e_role owner3 = idx2Role[tableIdx3];
  auto table3_ri = GetTemplateRI(rn3, ds, owner3, tableAttr3, attrTypes3);
  bool firstInAI3 = idx2AnnotInfo[tableIdx3];
  Relation::AnnotInfo table3_ai = {firstInAI3, true};
  Relation table3(table3_ri, table3_ai);
  auto filePath3 = GetFilePath(rn3, ds);
  if (tableIdx3 < 4) table3.LoadData(filePath3.c_str(), "q10_annot");
  if (tableIdx3 == 4) table3.LoadData(filePath3.c_str(), "q8_annot1");
  if (tableIdx3 == 5) table3.LoadData(filePath3.c_str(), "q9_annot");
  if (tableIdx3 == 6) table3.LoadData(filePath3.c_str(), "q9_annot2");
  if (!firstInAI3) table3.Aggregate();

  
  if (tableIdx2 == 2){
    if (tableIdx3 == 3){
      table2.SemiJoin(table3, "o_orderkey", "l_orderkey");
      table2.Aggregate("o_custkey");
      table1.SemiJoin(table2, "c_custkey", "o_custkey");
    }
    if (tableIdx3 == 1){
      table2.SemiJoin(table3, "o_custkey", "c_custkey");
      table2.Aggregate("o_orderkey");
      table1.SemiJoin(table2, "l_orderkey", "o_orderkey");
    }
  }

  if (tableIdx2 == 3){
    if (tableIdx3 == 2){
      table2.SemiJoin(table3, "l_orderkey", "o_orderkey");
      if (tableIdx1 == 4){
        table2.Aggregate("l_partkey");
        table1.SemiJoin(table2, "p_partkey", "l_partkey");
      }
      if (tableIdx1 == 5){
        table2.Aggregate("l_suppkey");
        table1.SemiJoin(table2, "s_suppkey", "l_suppkey");
      }
      if (tableIdx1 == 6){
        vector<string> ps_joinAttrs = {"ps_partkey", "ps_suppkey"};
        vector<string> line_joinAttrs = {"l_partkey", "l_suppkey"};
        table2.Aggregate(line_joinAttrs);
        table1.SemiJoin(table2, ps_joinAttrs, line_joinAttrs);
      }
    }
    if (tableIdx3 == 4){
      table2.SemiJoin(table3, "l_partkey", "p_partkey");
      if (tableIdx1 == 2){
        table2.Aggregate("l_orderkey");
        table1.SemiJoin(table2, "o_orderkey", "l_orderkey");
      }
      if (tableIdx1 == 5){
        table2.Aggregate("l_suppkey");
        table1.SemiJoin(table2, "s_suppkey", "l_suppkey");
      }
      if (tableIdx1 == 6){
        vector<string> ps_joinAttrs = {"ps_partkey", "ps_suppkey"};
        vector<string> line_joinAttrs = {"l_partkey", "l_suppkey"};
        table2.Aggregate(line_joinAttrs);
        table1.SemiJoin(table2, ps_joinAttrs, line_joinAttrs);
      }
    }
    if (tableIdx3 == 5){
      table2.SemiJoin(table3, "l_suppkey", "s_suppkey");
      if (tableIdx1 == 2){
        table2.Aggregate("l_orderkey");
        table1.SemiJoin(table2, "o_orderkey", "l_orderkey");
      }
      if (tableIdx1 == 4){
        table2.Aggregate("l_partkey");
        table1.SemiJoin(table2, "p_partkey", "l_partkey");
      }
      if (tableIdx1 == 6){
        vector<string> ps_joinAttrs = {"ps_partkey", "ps_suppkey"};
        vector<string> line_joinAttrs = {"l_partkey", "l_suppkey"};
        table2.Aggregate(line_joinAttrs);
        table1.SemiJoin(table2, ps_joinAttrs, line_joinAttrs);
      }
    }
    if (tableIdx3 == 6){
      vector<string> ps_joinAttrs = {"ps_partkey", "ps_suppkey"};
      vector<string> line_joinAttrs = {"l_partkey", "l_suppkey"};
      table2.SemiJoin(table3, line_joinAttrs, ps_joinAttrs);
    if (tableIdx1 == 2){
        table2.Aggregate("l_orderkey");
        table1.SemiJoin(table2, "o_orderkey", "l_orderkey");
      }
      if (tableIdx1 == 4){
        table2.Aggregate("l_partkey");
        table1.SemiJoin(table2, "p_partkey", "l_partkey");
      }
      if (tableIdx1 == 5){
        table2.Aggregate("l_suppkey");
        table1.SemiJoin(table2, "s_suppkey", "l_suppkey");
      }
    }
  }


	table1.RevealAnnotToOwner();
	if (printResult)
		table1.Print();
}





void run_TemplateQ0(DataSize ds, bool printResult, vector<string> groupByList, string treeString, unordered_map<int, vector<string>> relatedAttr)
{
  vector<int> tableIdx;
  for (int i = 0; i < treeString.length(); i++){
    if (treeString[i] != 'C'){
      int newTableIdx = int(treeString[i]) - int('0');;
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
  bool firstInAI1 = idx2AnnotInfo[tableIdx1];
  Relation::AnnotInfo table1_ai = {firstInAI1, true};
  Relation orders(table1_ri, table1_ai);
  auto filePath1 = GetFilePath(rn1, ds);
	orders.LoadData(filePath1.c_str(), "q9_annot");
  //orders.Print();



  int tableIdx2 = tableIdx[1];
  vector<string> tableAttr2 = relatedAttr[tableIdx2];
  vector<Relation::DataType> attrTypes2;
  for (int i = 0; i < tableAttr2.size(); i++){
    attrTypes2.push_back(attrName2Datatype[tableAttr2[i]]);
  }
  RelationName rn2 = idx2RelationName[tableIdx2];
  e_role owner2 = idx2Role[tableIdx2];
  auto table2_ri = GetTemplateRI(rn2, ds, owner2, tableAttr2, attrTypes2);
  bool firstInAI2 = idx2AnnotInfo[tableIdx2];
  Relation::AnnotInfo table2_ai = {firstInAI2, true};
  Relation customer(table2_ri, table2_ai);
  auto filePath2 = GetFilePath(rn2, ds);
	customer.LoadData(filePath2.c_str(), "q18_annot");
  //customer.Print();




  int tableIdx3 = tableIdx[2];
  vector<string> tableAttr3 = relatedAttr[tableIdx3];
  vector<Relation::DataType> attrTypes3;
  for (int i = 0; i < tableAttr3.size(); i++){
    attrTypes3.push_back(attrName2Datatype[tableAttr3[i]]);
  }
  RelationName rn3 = idx2RelationName[tableIdx3];
  e_role owner3 = idx2Role[tableIdx3];
  auto table3_ri = GetTemplateRI(rn3, ds, owner3, tableAttr3, attrTypes3);
  bool firstInAI3 = idx2AnnotInfo[tableIdx3];
  Relation::AnnotInfo table3_ai = {firstInAI3, true};
  Relation lineitem(table3_ri, table3_ai);
  auto filePath3 = GetFilePath(rn3, ds);
	lineitem.LoadData(filePath3.c_str(), "q9_annot2");
  //lineitem.Print();





  int tableIdx6 = tableIdx[5];
  vector<string> tableAttr6 = relatedAttr[tableIdx6];
  vector<Relation::DataType> attrTypes6;
  for (int i = 0; i < tableAttr6.size(); i++){
    attrTypes6.push_back(attrName2Datatype[tableAttr6[i]]);
  }
  RelationName rn6 = idx2RelationName[tableIdx6];
  e_role owner6 = idx2Role[tableIdx6];
  auto table6_ri = GetTemplateRI(rn6, ds, owner6, tableAttr6, attrTypes6);
  bool firstInAI6 = idx2AnnotInfo[tableIdx6];
  Relation::AnnotInfo table6_ai = {firstInAI6, true};
  Relation part(table6_ri, table6_ai);
  auto filePath6 = GetFilePath(rn6, ds);
	part.LoadData(filePath6.c_str(), "q9_annot");
  //part.Print();





  int tableIdx4 = tableIdx[3];
  vector<string> tableAttr4 = relatedAttr[tableIdx4];
  vector<Relation::DataType> attrTypes4;
  for (int i = 0; i < tableAttr4.size(); i++){
    attrTypes4.push_back(attrName2Datatype[tableAttr4[i]]);
  }
  RelationName rn4 = idx2RelationName[tableIdx4];
  e_role owner4 = idx2Role[tableIdx4];
  auto table4_ri = GetTemplateRI(rn4, ds, owner4, tableAttr4, attrTypes4);
  bool firstInAI4 = idx2AnnotInfo[tableIdx4];
  Relation::AnnotInfo table4_ai = {firstInAI4, true};
  Relation supplier(table4_ri, table4_ai);
  auto filePath4 = GetFilePath(rn4, ds);
	supplier.LoadData(filePath4.c_str(), "q8_annot1");
  //supplier.Print();






  int tableIdx5 = tableIdx[4];
  vector<string> tableAttr5 = relatedAttr[tableIdx5];
  for (int i = 0; i < tableAttr5.size(); i++){
    std::cout << tableAttr5[i] << std::endl;
  }
  vector<Relation::DataType> attrTypes5;
  for (int i = 0; i < tableAttr5.size(); i++){
    attrTypes5.push_back(attrName2Datatype[tableAttr5[i]]);
  }
  RelationName rn5 = idx2RelationName[tableIdx5];
  e_role owner5 = idx2Role[tableIdx5];
  auto table5_ri = GetTemplateRI(rn5, ds, owner5, tableAttr5, attrTypes5);
  bool firstInAI5 = idx2AnnotInfo[tableIdx5];
  Relation::AnnotInfo table5_ai = {firstInAI5, true};
  Relation partsupp(table5_ri, table5_ai);
  auto filePath5 = GetFilePath(rn5, ds);
	partsupp.LoadData(filePath5.c_str(), "q9_annot2");
  partsupp.Aggregate();
  //partsupp.Print();



	//lineitem.PrintTableWithoutRevealing("lineitem Init");
	vector<string> ps_joinAttrs = {"ps_partkey", "ps_suppkey"};
	vector<string> line_joinAttrs = {"l_partkey", "l_suppkey"};
	lineitem.SemiJoin(partsupp, line_joinAttrs, ps_joinAttrs);
	lineitem.SemiJoin(part, "l_partkey", "p_partkey");
	lineitem.SemiJoin(supplier, "l_suppkey", "s_suppkey");
	//lineitem.PrintTableWithoutRevealing("lineitem_semijoin");


	auto lineitem_copy = lineitem;

	lineitem_copy.Aggregate("l_orderkey");

	orders.SemiJoin(lineitem_copy, "o_orderkey", "l_orderkey");
	orders.Aggregate();
	//orders.PrintTableWithoutRevealing("orders_1");

	orders.SemiJoin(customer, "o_custkey", "c_custkey");
	//orders.PrintTableWithoutRevealing("orders_semicust");

	Relation orders_custkey_bool_annot = orders;
	orders_custkey_bool_annot.Project("o_custkey");
	orders_custkey_bool_annot.AnnotOrAgg();


	Relation orders_orderkey_bool_annot = orders;
	orders_orderkey_bool_annot.Project("o_orderkey");
	orders_orderkey_bool_annot.AnnotOrAgg();

	customer.SemiJoin(orders_custkey_bool_annot, "c_custkey", "o_custkey");
	lineitem.SemiJoin(orders_orderkey_bool_annot, "l_orderkey", "o_orderkey");

	orders.RevealTuples();
	//orders.PrintTableWithoutRevealing("orders_2");
	customer.AnnotOrAgg();
	customer.RemoveZeroAnnotatedTuples();
	customer.RevealTuples();
	orders.Join(customer, "o_custkey", "c_custkey");
	//orders.PrintTableWithoutRevealing("orders_3");


	//lineitem.Project("l_orderkey");
	//lineitem.AnnotOrAgg();
	vector<string> l_groupBy = {"l_orderkey"};
  for (int i = 0; i < groupByList.size(); i++){
    if (groupByList[i] != "l_orderkey" && groupByList[i][0] == 'l') l_groupBy.push_back(groupByList[i]);
  }
	lineitem.Project(l_groupBy);
	lineitem.AnnotOrAgg();

	lineitem.RemoveZeroAnnotatedTuples();
	lineitem.RevealTuples();
	//lineitem.PrintTableWithoutRevealing("lineitem_group");
	orders.Join(lineitem, "o_orderkey", "l_orderkey");
	//orders.PrintTableWithoutRevealing("orders_4");


	orders.RevealAnnotToOwner();
	if (printResult)
		orders.Sort();
		orders.Print(500);
}