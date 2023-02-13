#include "node.h"
#include "grammar.tab.h"
#include <fstream>

using namespace std;

extern "C" FILE* yyin;

string print_operators(operators op){
switch (op) {
    case operators::PLUS_OP:
       return("+");
    case operators::MINUS_OP:
       return("-");
    case operators::MULT_OP:
       return("*");
    case operators::DIVIDE_OP:
       return("/");
       break;
    default:
       return("No operator found in the enum space"); 
  }
}

void input_handler(int argc, char *argv[]){
  if (argc>1){
      FILE *fp;
      fp = fopen(argv[1],"r");
      yyin = fp;
      yyparse();
      fclose(fp); 
  }
  else{
      cout<<"Press Ctrl+D to end input"<<endl;
      yyparse();
  }
}

void print_variant(variant_t w){
  visit([](auto&& arg){
    using T = decay_t<decltype(arg)>;
    if constexpr(is_same_v<T,int>){
      cout<<arg<<endl;
      return;
    }
    else if constexpr (is_same_v<T, node*>){
      cout<<arg->print()<<endl;
      return;
    }
  },w);
  return;
}

string node::print(){
  return "";
}

pair<string,node*> node::getLambda(){
  throw invalid_argument("this is not a lambda node");;
}   


number_node::number_node(int num) {
  number = num;
}

string number_node::print(){
  return to_string(number);
}

pair<string,node*> number_node::getLambda(){
    throw invalid_argument("this is not a lambda node\n");
}  

var_node::var_node(string var_) {
  var = var_;
}

string var_node::print(){
  return var;
}

pair<string,node*> var_node::getLambda(){
  throw invalid_argument("this is not a lambda node\n");
} 

op_node::op_node(node *l, node *r,operators oper){
  left = l;
  right = r;
  op = oper;
}

string op_node::print(){
  return left->print()+print_operators(op)+ right->print();
}

pair<string,node*> op_node::getLambda(){
  throw invalid_argument("this is not a lambda node\n");
} 


lambda::lambda(string var, node* temp_body){
  var_name = var;
  body = temp_body;
}

string lambda::print(){
  return "λ"+var_name+","+body->print();
}

pair<string,node*> lambda::getLambda(){
    return {var_name,body};;
}  


application::application(node* l, node* r){
  left = l;
  right = r;
}

string application::print(){
  string newleft = left->print();
  string newRight = right->print();
  return "("+newleft+") ("+newRight+")";
}

pair<string,node*> application::getLambda(){
    throw invalid_argument("this is not a lambda node\n");;
}         

