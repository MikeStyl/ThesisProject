#include "grammar.tab.h"
#include "node.h"
#include <fstream>

node* root;


int main(int argc, char *argv[]){
    input_handler(argc,argv);
    if (!root){return 1;}
    renamer(root);

    try{
        codeGeneration(root);
    }
    catch (bad_variant_access const& ex){
        cout << ex.what() << " This is not a node variant\n";
    }
    catch (invalid_argument const&ex){
        cout << ex.what();
    }
    return 0;
}