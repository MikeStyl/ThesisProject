#include "node.h"

int counter = 0;

void renamer(node* root){
  map<string, string> renamer_map;
  root->rename(renamer_map);
}

void node::rename(map<string, string> used_variables){
}

void number_node::rename(map<string, string> used_variables){
}

void var_node::rename(map<string, string> used_variables){ 
  for (auto const& [key, val]: used_variables){
    if (key == var){
      var = val;
    }
  }
}

void op_node::rename(map<string, string> used_variables){
  left->rename(used_variables);
  right->rename(used_variables);        
}

void lambda::rename(map<string, string> used_variables){
  string new_var_name = "x_" + to_string(counter);
  counter++;
  used_variables[var_name] = new_var_name;
  body->rename(used_variables);
  var_name = new_var_name;
}

void application::rename(map<string, string> used_variables){
  left->rename(used_variables);
  right->rename(used_variables); 
}