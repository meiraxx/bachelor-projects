%{
// $Id: xpl_parser.y,v 1.21 2017/05/22 05:36:14 ist182014 Exp $
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE       compiler->scanner()->lineno()
#define yylex()    compiler->scanner()->scan()
#define yyerror(s) compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!
%}

%union {
  int                   i;	/* integer value */
  std::string          *s;	/* symbol name or string literal */
  double                d;  /* real value */
  cdk::basic_node      *node;	/* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;
  xpl::vardecl_node    *vardecl;
  xpl::fundecl_node    *fundecl;
  xpl::fundef_node     *fundef;
  basic_type           *basictype;
  xpl::body_node       *body;
  xpl::block_node      *block;
};

%token <i> tINTEGER
%token <s> tIDENTIFIER tSTRING
%token <d> tREAL
%token tWHILE tIF tREAD tPRINT tSWEEP tPRINTLN tSTOP tNEXT tRETURN tNULL tTYPEINT tTYPEREAL tTYPESTRING tPROCEDURE tPUBLIC tUSE

%nonassoc tFUNCX
%nonassoc ';'
%nonassoc ')'           // then
%nonassoc tIFX
%nonassoc tELSE tELSIF
%nonassoc '{' '}'

%right '='
%left '|'
%left '&'
%nonassoc '~'
%left tEQ tNE
%left tGE tLE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc '?'
%nonassoc '(' '[' ']'                   // primary precedences

%type <node> declaration instruction variableDeclaration function novosimbolo
%type <sequence> file declarations instructions variableList argumentFunctionCallList
%type <expression> expr literal functionCall argumentFunctionCall
%type <lvalue> lval
%type <vardecl> variable
%type <fundecl> functionDeclStart functionDeclaration
%type <fundef> functionDefStart functionDefinition
%type <basictype> type
%type <body> body
%type <block> block
%type <s> string

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

file: declarations                     { compiler->ast($$ = $1); }
    ;

declarations: declaration             { $$ = new cdk::sequence_node(LINE, $1); }
            | declarations declaration { $$ = new cdk::sequence_node(LINE, $2, $1); }
            ;

declaration: variableDeclaration ';'   { $$ = $1; }
           | function                  { $$ = $1; }
           ;

variableDeclaration
    : tUSE type tIDENTIFIER                 { $$ = new xpl::vardecl_node(LINE, $2, $3, true, false, nullptr); }
    | tPUBLIC variable                      { $2->setPublicFlag(); $$ = $2; }
    | variable                              { $$ = $1; }
    ;

function: functionDefinition                   { $$ = $1; }
        | functionDeclaration                  { $$ = $1; }
        ;

functionDeclStart
    : tPROCEDURE tIDENTIFIER '(' variableList ')' { $$ = new xpl::fundecl_node(LINE, new basic_type(0, basic_type::TYPE_VOID), $2, false, false, $4); }
    | type tIDENTIFIER '(' variableList ')'       { $$ = new xpl::fundecl_node(LINE, $1, $2, false, false, $4); }
    ;

functionDeclaration
    : functionDeclStart                         { $$ = $1; }
    | tUSE functionDeclStart                    { $2->setUseFlag(); $$ = $2; }
    | tPUBLIC functionDeclStart                 { $2->setPublicFlag(); $$ = $2; }
    ;

functionDefStart
    : type tIDENTIFIER '(' variableList ')' '=' literal body       { $$ = new xpl::fundef_node(LINE, $1, $2, false, $4, $7, $8); }
    | tPROCEDURE tIDENTIFIER '(' variableList ')' body             { $$ = new xpl::fundef_node(LINE, new basic_type(0, basic_type::TYPE_VOID), $2, false, $4, nullptr, $6); }
    // segundo o enunciado, se o tipo da expressao da funcao for int ou pointer no caso em que nao definimos um default value, damos um null ou um 0. Caso contrario, indeterminado (???)
    | type tIDENTIFIER '(' variableList ')' body                   { $$ = new xpl::fundef_node(LINE, $1, $2, false, $4, nullptr, $6); }
    ;

functionDefinition 
    : functionDefStart                     { $$ = $1; }
    | tPUBLIC functionDefStart             { $2->setPublicFlag(); $$ = $2; }
    ;

functionCall: tIDENTIFIER '(' argumentFunctionCallList ')'  { $$ = new xpl::funcall_node(LINE, $1, $3); }
            ;
            
body: block                                { $$ = new xpl::body_node(LINE, $1); }
    ;

block: '{' declarations instructions '}'   { $$ = new xpl::block_node(LINE, $2, $3); }
     | '{' declarations '}'                { $$ = new xpl::block_node(LINE, $2, new cdk::sequence_node(LINE)); }
     | '{' instructions '}'                { $$ = new xpl::block_node(LINE, new cdk::sequence_node(LINE), $2); }
     | '{' '}'                             { $$ = new xpl::block_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE)); }
     ;

instructions: instruction              { $$ = new cdk::sequence_node(LINE, $1); }
            | instructions instruction { $$ = new cdk::sequence_node(LINE, $2, $1); }
            ;

instruction
     : expr ';'                                { $$ = new xpl::evaluation_node(LINE, $1); }
     | expr tPRINT                             { $$ = new xpl::print_node(LINE, $1, false); }
     | expr tPRINTLN                           { $$ = new xpl::print_node(LINE, $1, true); }
     | tSTOP                                   { $$ = new xpl::stop_node(LINE); }
     | tNEXT                                   { $$ = new xpl::next_node(LINE); }
     | tRETURN                                 { $$ = new xpl::return_node(LINE); }
     | tIF '(' expr ')' instruction novosimbolo { $$ = new xpl::if_else_node(LINE, $3, $5, $6); }
     | tWHILE '(' expr ')' instruction         { $$ = new xpl::while_node(LINE, $3, $5); }
     | tSWEEP '+' '(' lval ':' expr ':' expr ':' expr ')' instruction { $$ = new xpl::sweep_node(LINE, true, $4, $6 , $8 , $10, $12);}
     | tSWEEP '-' '(' lval ':' expr ':' expr ':' expr ')' instruction { $$ = new xpl::sweep_node(LINE, false, $4, $6 , $8 , $10, $12);}
     | tSWEEP '+' '(' lval ':' expr ':' expr ')' instruction          { $$ = new xpl::sweep_node(LINE, true, $4, $6 , $8 , new cdk::integer_node(LINE, 1), $10);}
     | tSWEEP '-' '(' lval ':' expr ':' expr ')' instruction          { $$ = new xpl::sweep_node(LINE, false, $4, $6 , $8 , new cdk::integer_node(LINE, 1), $10);}
     | block                                   { $$ = $1; }
     ;

novosimbolo: tELSIF '(' expr ')' instruction novosimbolo 	{ $$ = new xpl::if_else_node(LINE, $3, $5, $6); }
	   | tELSE instruction                 { $$ = $2; }
     |	/* empty */		%prec tIFX         { $$ = new cdk::nil_node(LINE); }
     ;

expr: literal                      { $$ = $1;}
    | tREAD                        { $$ = new xpl::read_node(LINE); }
    | '+' expr                     { $$ = new xpl::identity_node(LINE, $2); }
    | '-' expr                     { $$ = new cdk::neg_node(LINE, $2); }
    | '~' expr                     { $$ = new cdk::not_node(LINE, $2); }
    | '[' expr ']'                 { $$ = new xpl::memalloc_node(LINE, $2); }
    | expr '+' expr                { $$ = new cdk::add_node(LINE, $1, $3); }
    | expr '-' expr                { $$ = new cdk::sub_node(LINE, $1, $3); }
    | expr '*' expr                { $$ = new cdk::mul_node(LINE, $1, $3); }
    | expr '/' expr                { $$ = new cdk::div_node(LINE, $1, $3); }
    | expr '%' expr                { $$ = new cdk::mod_node(LINE, $1, $3); }
    | expr '<' expr                { $$ = new cdk::lt_node(LINE, $1, $3); }
    | expr '>' expr                { $$ = new cdk::gt_node(LINE, $1, $3); }
    | expr tGE expr                { $$ = new cdk::ge_node(LINE, $1, $3); }
    | expr tLE expr                { $$ = new cdk::le_node(LINE, $1, $3); }
    | expr tNE expr                { $$ = new cdk::ne_node(LINE, $1, $3); }
    | expr tEQ expr                { $$ = new cdk::eq_node(LINE, $1, $3); }
    | '(' expr ')'                 { $$ = $2; }
    | lval                         { $$ = new cdk::rvalue_node(LINE, $1); }
    | lval '=' expr                { $$ = new cdk::assignment_node(LINE, $1, $3); }
    | expr '?'                     { $$ = new xpl::memaddr_node(LINE, $1); }
    | expr '&' expr                { $$ = new cdk::and_node(LINE, $1, $3); }
    | expr '|' expr                { $$ = new cdk::or_node(LINE, $1, $3); }
    | functionCall                 { $$ = $1; }
    ;

argumentFunctionCallList
    : argumentFunctionCall                                    { $$ = new cdk::sequence_node(LINE, $1); }
    | argumentFunctionCallList ',' argumentFunctionCall       { $$ = new cdk::sequence_node(LINE, $3, $1); }
    |                                                         { $$ = new cdk::sequence_node(LINE); }
    ;

argumentFunctionCall:  expr                         { $$ = $1; }
                    ;

variableList: variable                              { $$ = new cdk::sequence_node(LINE, $1); }
       | variableList ',' variable                  { $$ = new cdk::sequence_node(LINE, $3, $1); }
       |                                            { $$ = new cdk::sequence_node(LINE); }
       ;

variable: type tIDENTIFIER '=' expr          { $$ = new xpl::vardecl_node(LINE, $1, $2, false, false, $4); }
        | type tIDENTIFIER                   { $$ = new xpl::vardecl_node(LINE, $1, $2, false, false, nullptr); }
        ;

type: tTYPEINT                 { $$ = new basic_type(4, basic_type::TYPE_INT); }
    | tTYPESTRING              { $$ = new basic_type(4, basic_type::TYPE_STRING); }
    | tTYPEREAL                { $$ = new basic_type(8, basic_type::TYPE_DOUBLE); }
    | '[' type ']'             { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype = $2;}
    ;

lval: tIDENTIFIER               { $$ = new cdk::identifier_node(LINE, $1); }
    | expr '[' expr ']'         { $$ = new xpl::indexer_node(LINE, $1, $3); }
    ;

literal: tINTEGER               { $$ = new cdk::integer_node(LINE, $1); }
       | tREAL                  { $$ = new cdk::double_node(LINE, $1); }
       | string                 { $$ = new cdk::string_node(LINE, $1); }
       | tNULL                  { $$ = nullptr; }       // equivalente 

string: tSTRING                 { $$ = $1; }
      | string tSTRING          { $$ = new std::string((*$1).append(*$2)); delete $1; delete $2; }
      ;

%%
