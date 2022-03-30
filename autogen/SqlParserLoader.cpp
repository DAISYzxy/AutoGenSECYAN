
# include "SqlParserLoader.h"

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
    ival = "'" + ival + "'";
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
