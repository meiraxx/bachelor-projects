%option c++ yyclass="xpl_scanner" outfile="xpl_scanner.cpp"
%option stack noyywrap yylineno 8bit
%{ 
/* $Id: xpl_scanner.l,v 1.8 2017/05/04 23:47:34 ist182014 Exp $ */
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "xpl_scanner.h"
#include "xpl_parser.tab.h"

// don't change this
#define yyerror LexerError
%}

%x X_STRING X_OPCOMMENT

%%

"/*"					yy_push_state(X_OPCOMMENT);
<X_OPCOMMENT>"/*"		yy_push_state(X_OPCOMMENT);
<X_OPCOMMENT>\n 		;
<X_OPCOMMENT>.			;
<X_OPCOMMENT>"*/"		yy_pop_state();

"//".*$                 ; /* ignore comments */

">="                   return tGE; 
"<="                   return tLE; 
"=="                   return tEQ; 
"!="                   return tNE; 

"while"                return tWHILE; 
"if"                   return tIF; 
"else"                 return tELSE;


	/* novos */
"@"                    return tREAD;
"!"					   return tPRINT;
"!!"				   return tPRINTLN;
"sweep"				   return tSWEEP;
"stop"				   return tSTOP;
"next"				   return tNEXT;
"return"			   return tRETURN;
"null"				   return tNULL;
"int"				   return tTYPEINT;
"real"				   return tTYPEREAL;
"string"			   return tTYPESTRING;
"procedure"			   return tPROCEDURE;
"public"			   return tPUBLIC;
"use"				   return tUSE;
"elsif"				   return tELSIF;

[A-Za-z_][A-Za-z0-9_]*  yylval.s = new std::string(yytext); return tIDENTIFIER;

\"                     					yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\"           					yy_pop_state(); return tSTRING;
<X_STRING>\\\"         					*yylval.s += yytext + 1;
<X_STRING>\\0[1-9a-fA-F]			   	*yylval.s += strtol(yytext + 1, nullptr, 16);
<X_STRING>\\[1-9a-fA-F][0-9a-fA-F]? 	*yylval.s += strtol(yytext + 1, nullptr, 16);
<X_STRING>\\\\      					*yylval.s += yytext + 1;
<X_STRING>\\t       					*yylval.s += "\t";
<X_STRING>\\r       					*yylval.s += "\r";
<X_STRING>\\n       					*yylval.s += "\n";
<X_STRING>.         					*yylval.s += yytext;
<X_STRING>\n        					yyerror("newline in string");

0[xX][0-9a-fA-F]+	   yylval.i = strtol(yytext, nullptr, 16); return tINTEGER;

[1-9][0-9]*        yylval.i = strtol(yytext, nullptr, 10); return tINTEGER;
0				   yylval.i = 0; return tINTEGER;

[0-9]*"."[0-9]+[Ee][+-]?[0-9]+      yylval.d = strtod(yytext, nullptr); return tREAL;
[0-9]+"."[0-9]*[Ee][+-]?[0-9]+      yylval.d = strtod(yytext, nullptr); return tREAL;
[0-9]+[Ee][+-]?[0-9]+				yylval.d = strtod(yytext, nullptr); return tREAL;
[0-9]*"."[0-9]+					    yylval.d = strtod(yytext, nullptr); return tREAL;
[0-9]+"."[0-9]*                     yylval.d = strtod(yytext, nullptr); return tREAL;

[-()\[\]<>=+*/%{},;:&|?~]       return *yytext;

[ \r\t\n]+               ; /* ignore whitespace */

.                      yyerror("Unknown character");

%%
// Very, very dirty hack: flex is a mess generating C++ scanners.
int xpl_scanner::yywrap() { return 1; }
