%option nodefault
%{

#include "grammar.tab.h"

%}

whitespace          [ \t]+
newline             "\n"
carriage_return     "\r"
lambda              "λ"

%%
<<EOF>>                           {yyterminate();}
[(]                               {return LPAREN;}
[)]                               {return RPAREN;}
{lambda}                          {return LAMBDA;}
[a-zA-Z]+                         {char *new_var = strdup(yytext);
                                   yylval.var = new_var;
                                   return VAR;}
[0-9]+                            {yylval.num = atof(yytext); return NUMBER;}
[+]                               {return PLUS;}
[-]                               {return MINUS;}
[*]                               {return MULT;}
[/]                               {return DIVIDE;}
[,]                               {return COMMA;}
{whitespace}                      {}
{newline}                         {}
{carriage_return}                 {}
.                                 {std::cout<<yytext<<" unmatched string"<<std::endl;}
%%

int yywrap() {return 1;}
