
# include <stdlib.h>
# include <string>

# include <iostream>
# include <queue>
#include <fstream>

# include <typeinfo>
# include <map>
# include <vector>
# include <unordered_map>

# include "SQLParser.h"
# include "util/sqlhelper.h"
# include "sql/statements.h"

using namespace std;
using namespace hsql;


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

// parse Expr to string
void normalExpr(Expr* exp, vector<string> *result);
void opExpr(Expr* exp, vector<string> *result);
