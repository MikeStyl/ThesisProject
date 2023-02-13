#ifndef NODE_H_
#define NODE_H_

#include <iostream>
#include <string>
#include <variant>
#include <map>
#include <vector>

using namespace std;

class node;
extern node* root;

using variant_t = variant<int, node*>;
using code_Gen_state = string;

enum class operators {
    PLUS_OP,
    MINUS_OP,
    MULT_OP,
    DIVIDE_OP,
};

string print_operators(operators op);

void print_variant(variant_t w);

int evalInt(node *body, map<string, variant_t> M);

void input_handler(int argc, char *argv[]);

code_Gen_state emitFunction(string f,code_Gen_state state);

string new_variable_name();

void codeGeneration(node* root);

void renamer(node* root);

pair<map<string,variant_t>,variant_t> eval(node* root);

class node { 
    public:
        virtual void rename(map<string, string>);
        virtual string print();
        virtual pair<map<string,variant_t>,variant_t> eval(map<string, variant_t>);
        virtual pair<string,node*> getLambda();
        virtual pair<string,code_Gen_state> codeGen(code_Gen_state);
}; 


class number_node : public node {
    public:
        int number;
        number_node(int num);
        void rename(map<string, string>);
        string print();
        pair<map<string,variant_t>,variant_t> eval(map<string, variant_t>);
        pair<string,node*> getLambda();
        pair<string,code_Gen_state> codeGen(code_Gen_state);
};


class var_node : public node {
    public:
        string var;
        var_node(string temp_var);
        void rename(map<string, string>);
        string print();
        pair<map<string,variant_t>,variant_t> eval(map<string, variant_t>);
        pair<string,node*> getLambda();
        pair<string,code_Gen_state> codeGen(code_Gen_state);
};


class op_node : public node{
    public:
        node *left;
        node *right;
        operators op;
        op_node(node *l, node *r,operators oper);
        void rename(map<string, string>);
        string print();
        pair<map<string,variant_t>,variant_t> eval(map<string, variant_t>);
        pair<string,node*> getLambda();
        pair<string,code_Gen_state> codeGen(code_Gen_state);
};


class lambda : public node{
    public:
        string var_name;
        node *body;
        lambda(string var, node* temp_body);
        void rename(map<string, string>);
        string print();
        pair<map<string,variant_t>,variant_t> eval(map<string, variant_t>);
        pair<string,node*> getLambda();
        pair<string,code_Gen_state> codeGen(code_Gen_state);
};


class application : public node{
    public:
        node *left;
        node *right;
        application(node* l, node* r);
        void rename(map<string, string>);
        string print();
        pair<map<string,variant_t>,variant_t> eval(map<string, variant_t>);
        pair<string,node*> getLambda();
        pair<string,code_Gen_state> codeGen(code_Gen_state);
};
#endif // NODE_H_
