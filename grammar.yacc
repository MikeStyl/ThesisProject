%code top {
    int yylex (void);
    void yyerror (char const *);
}
%code requires {
    #include "node.h"
}
%code {
    extern node* root;
}

%union {
    int num;
    char * var;
    node * expr;
}
%start ROOT

%token <num> NUMBER
%token <var> VAR
%token COMMA
%right LAMBDA
%left PLUS MINUS 
%left MULT DIVIDE
%left LPAREN RPAREN
%type <expr> EXPR
%type <expr> ROOT
%type <expr> EXPR_LIST


%%
ROOT: EXPR_LIST { $$ = $1; root = $1; }

EXPR_LIST: 
    EXPR_LIST EXPR{
        $$ = new application($1,$2);}
    | EXPR;

EXPR:
    LPAREN EXPR_LIST RPAREN  {
        $$ = $2; }      
    |   NUMBER {
        $$ = new number_node($1);}
    |   VAR{
        $$ = new var_node($1);}
    |   EXPR_LIST PLUS EXPR_LIST{
        $$ = new op_node($1,$3,operators::PLUS_OP);   }
    |   EXPR_LIST MINUS EXPR_LIST{
        $$ = new op_node($1,$3,operators::MINUS_OP);  }
    |   EXPR_LIST MULT EXPR_LIST{
        $$ = new op_node($1,$3,operators::MULT_OP);   }
    |   EXPR_LIST DIVIDE EXPR_LIST{
        $$ = new op_node($1,$3,operators::DIVIDE_OP); } 
    |   LAMBDA VAR COMMA EXPR_LIST {
        $$ = new lambda($2,$4);}
           
%%

void yyerror(const char* s)
{
  fprintf (stderr, "%s\n", s);
}
