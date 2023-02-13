#include "node.h"

pair<map<string,variant_t>,variant_t> eval(node* root){
  map<string, variant_t> M;
  return root->eval(M);
}

int evalInt(node *body, map<string, variant_t> M){
  variant_t w = body->eval(M).second;
  return get<int>(w);
}

pair<map<string,variant_t>,variant_t> node::eval(map<string, variant_t> M){
  throw invalid_argument("This node should not be accesible");
}

pair<map<string,variant_t>,variant_t> number_node::eval(map<string, variant_t> M){
  return {M,number};
}

pair<map<string,variant_t>,variant_t> var_node::eval(map<string, variant_t> M){
  return {M,M.at(var)};
}

pair<map<string,variant_t>,variant_t> op_node::eval(map<string, variant_t> M){
  try{
    int left_num = evalInt(left, M);
    int right_num = evalInt(right, M);
    switch (op) {
      case operators::PLUS_OP:
          return {M,(left_num + right_num)};
      case operators::MINUS_OP:
          return {M,(left_num - right_num)};
      case operators::MULT_OP:
          return {M,(left_num * right_num)};
      case operators::DIVIDE_OP:
          return {M,(left_num / right_num)};
      default://This case should be unreachable.
          return {M,left_num};
    }
  }
  catch (bad_variant_access const& ex){
    throw invalid_argument("Expected "+ left->print() +" and "+right->print() +" to be a num");
  }; 
}

pair<map<string,variant_t>,variant_t> lambda::eval(map<string, variant_t> M){
  return {M,this};
}

pair<map<string,variant_t>,variant_t> application::eval(map<string, variant_t> M){
  pair<map<string,variant_t>,variant_t> r = right->eval(M);
  pair<map<string,variant_t>,variant_t> l = left->eval(r.first);
  pair<string,node*> new_lambda= get<node*>(l.second)->getLambda();
  l.first[new_lambda.first] = r.second;
  return {new_lambda.second->eval(l.first)}; 
}