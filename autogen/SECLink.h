# include <stdlib.h>
# include <string>
# include <iostream>

# include <typeinfo>
# include <string>

# include <map>
# include <queue>
# include <vector>
# include <unordered_map>

// #include "../SECYAN/src/example/TPCH.h"
#include "core/relation.h"
#include <vector>
#include <string>
#include <iostream>
#include <chrono>

#pragma once
#include <cstdint>

using namespace SECYAN;

enum RelationName
{
    CUSTOMER,
    ORDERS,
    LINEITEM,
    PART,
    SUPPLIER,
    PARTSUPP,
    RTOTAL
};

enum QueryName
{
    Q3,
    Q10,
    Q18,
    Q8,
    Q9,
    Qzxy,
    QTOTAL
};

enum DataSize
{
    _1MB,
    _3MB,
    _10MB,
    _33MB,
    _100MB,
    DTOTAL
};



//inline std::string newGetFilePath(RelationName rn, DataSize ds);
//inline Relation::RelationInfo GetTemplateRI(RelationName rn, DataSize ds, e_role owner, vector<string> attrNames, vector<Relation::DataType> attrTypes);
using run_query = void(DataSize ds, bool printResult, vector<string> groupByList, string treeString, unordered_map<int, vector<string>> relatedAttr);
run_query run_TemplateQ3;


