#include "grammar.tab.h"
#include "node.h"
#include <fstream>

node* root;


int main(int argc, char *argv[]){
    input_handler(argc,argv);
    if (!root){return 1;}
    renamer(root);
    try{
        auto result = eval(root);
        print_variant(result.second);
    }
    catch (bad_variant_access const& ex){
        cout << "Type error" << endl;
    }
    catch (invalid_argument const&ex){
        cout << ex.what() << endl;
    }
    return 0;
}