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



inline Relation::RelationInfo GetTemplateRI(RelationName rn, QueryName qn, DataSize ds, e_role owner, vector<string> attrNames, vector<Relation::DataType> attrTypes)
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






void run_TemplateQ3(DataSize ds, bool printResult, vector<string> groupByList, string treeString)
{

	vector<string> o_groupBy = {"o_orderkey", "o_orderdate", "o_shippriority"};
	auto cust_ri = GetRI(CUSTOMER, Q3, ds, SERVER);
	Relation::AnnotInfo cust_ai = {true, true};
	Relation customer(cust_ri, cust_ai);
	auto filePath = GetFilePath(CUSTOMER, ds);
	customer.LoadData(filePath.c_str(), "q3_annot");

	auto orders_ri = GetRI(ORDERS, Q3, ds, CLIENT);
	Relation::AnnotInfo orders_ai = {true, true};
	Relation orders(orders_ri, orders_ai);
	filePath = GetFilePath(ORDERS, ds);
	orders.LoadData(filePath.c_str(), "q3_annot");
	//orders.Print();

	auto lineitem_ri = GetRI(LINEITEM, Q3, ds, SERVER);
	Relation::AnnotInfo lineitem_ai = {false, true};
	Relation lineitem(lineitem_ri, lineitem_ai);
	filePath = GetFilePath(LINEITEM, ds);
	lineitem.LoadData(filePath.c_str(), "q3_annot");
	lineitem.Aggregate();

	orders.SemiJoin(customer, "o_custkey", "c_custkey");
	//orders.PrintTableWithoutRevealing("orders semijoin customer");
	// PSI: 95.4267ms
	// Two OEPs: 53.8459ms
	// AnnotMul: 4.4265ms

	orders.SemiJoin(lineitem, "o_orderkey", "l_orderkey");
	//orders.PrintTableWithoutRevealing("orders semijoin lineitem");
	// PSI: 162.877ms
	// Two OEPs: 25.8395ms
	// AnnotMul: 51.1201ms

	orders.Aggregate(o_groupBy);
	orders.RevealAnnotToOwner();
	if (printResult)
		orders.Print();
}