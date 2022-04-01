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

using run_query = void(DataSize, bool);
run_query run_Q3, run_Q10, run_Q18, run_Q8, run_Q9, run_Qzxy;


