#include "node.h"
#include <fstream>
#include <sstream>

int fun_counter = 0;

string new_fun_name(){
    string var = "l" +to_string(fun_counter);
    fun_counter++;
    return var;
}

string new_lambda(string s){
    return "(any_t)(new closure("+s+",env))";
}

string proccesClosureFile(){
    ifstream closure_file;
    closure_file.open("closure.cpp");
    string content( (std::istreambuf_iterator<char>(closure_file) ), (std::istreambuf_iterator<char>()    ) );

    return content;
}

void printToFile(pair<string,code_Gen_state> program){
    ofstream file;
    file.open("output.cpp");
    file<< program.second << "\n" << program.first;
    file.close();
    system("g++ -std=c++2a -o compiledProgram output.cpp");
    system("./compiledProgram");
}

void codeGeneration(node* root){
    string prelude = proccesClosureFile() +"\n";
    pair<string,code_Gen_state> program= root->codeGen(prelude);
    string main = R"(
int main(){
    map<string, any_t> env = {};
    try{
        cout<< get<int>()" + program.first + R"END()<<endl;
    }
    catch(bad_variant_access const& ex){
        cout << "Type error" <<endl;
        return 1;
    }
    return 0;
}
)END";
    printToFile({main, program.second});
}

code_Gen_state emitFunction(string f,code_Gen_state state){
    return state + f + "\n}\n";
}

pair<string,code_Gen_state> node::codeGen(code_Gen_state state){
    return {"",state};
}

pair<string,code_Gen_state> number_node::codeGen(code_Gen_state state){
    return {"(any_t)("+to_string(number)+")",state};
}

pair<string,code_Gen_state> var_node::codeGen(code_Gen_state state){
    return {"env.at(\""+var+"\")",state};
}

pair<string,code_Gen_state> op_node::codeGen(code_Gen_state state){
    pair<string,code_Gen_state> l;
    pair<string,code_Gen_state> r;
    l = left->codeGen(state);
    string left_var = l.first;
    r = right->codeGen(l.second);
    string right_var = r.first;

    string sign;

    switch (op) {
      case operators::PLUS_OP: 
          sign = "+";
          break;
      case operators::MINUS_OP:
          sign = "-";
          break;
      case operators::MULT_OP:
          sign = "*";
          break;
      case operators::DIVIDE_OP:
          sign = "/";
          break;
    }
    return{"(any_t)(get<int>("+left_var + ") " + sign  + " get<int>("+right_var+") )",r.second};
}

pair<string,code_Gen_state> lambda::codeGen(code_Gen_state state){
    pair<string,code_Gen_state> body_Gen = body->codeGen(state);
    string function_name = new_fun_name();
    string lambda_name = new_lambda(function_name);
    string fuction_parameters = "(any_t " + this->var_name+"," + " map<string, any_t> env)";
    string insert_to_env = "env.insert({\""+this->var_name+"\"," + this->var_name +"});";
    state = emitFunction("any_t " + function_name + fuction_parameters + "\n{\n\t"+ insert_to_env +"\n\treturn "+ body_Gen.first +";",body_Gen.second);
    return {lambda_name,state};
}

pair<string,code_Gen_state> application::codeGen(code_Gen_state state){
    pair<string,code_Gen_state> l;
    pair<string,code_Gen_state> r;

    l = left->codeGen(state);
    r = right->codeGen(l.second);

    return {"get<closure*>("+l.first+")->applyClosure("+r.first+")",r.second};
}