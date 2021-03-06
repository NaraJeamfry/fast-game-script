%option noyywrap
%option nounput
%option noinput
%{

#include "fgs_comp.h"
#include "fgs_comp_y.h"
#include <stdio.h>

extern int yylineno;

%}

%x IN_COMMENT

newline \n
%%

:=			{
				return ASSIGN;
			}

[*]			{
				return MUL;
			}

[+]			{
				return ADD;
			}

[-]			{
				return SUB;
			}

[/]			{
				return DIV;
			}

=			{
				return EQ;
			}

[<][>]			{
				return NE;
			}
			
[>]			{
				return GT;
			}

[>]=			{
				return GE;
			}

[<]			{
				return LT;
			}

[<]=		{
				return LE;
			}
			
[(]			{
				return OPEN_PARENTH;
			}

[)]			{
				return CLOSE_PARENTH;
			}
			
[{]			{
				return OPEN_CLAUD;
			}
			
[}]			{
				return CLOSE_CLAUD;
			}
			
[;]			{
				return SEPARATOR;
			}
			
for			{
				return FOR;
			}
			
in			{
				return IN;
			}
			
\.\.		{
				return FOR_RANGE;
			}
			
while		{
				return WHILE;
			}
			
if			{
				return IF;
			}
			
else		{
				return ELSE;
			}

true		{
				yylval.literal.type = BOOLT;
				yylval.literal.value.bool_value = 1;
				return LITERAL_BOOL;
			}
			
false		{
				yylval.literal.type = BOOLT;
				yylval.literal.value.bool_value = 0;
				return LITERAL_BOOL;
			}
			
return		{
				return RETURN;
			}
			
[&&]		{
				return AND;
			}
			
and			{
				return AND;
			}
			
[||]		{
				return OR;
			}

or			{
				return OR;
			}

[!]			{
				return NOT;
			}		
			
not			{
				return NOT;
			}
			
int			{
				return INTEGER;
			}
			
float		{
				return FLOAT;
			}
		
string		{
				return STRING;
			}
			
[0-9]+\.[0-9]+	{
				yylval.literal.type = FLOATT;
				yylval.literal.value.float_value = atof(yytext);
				return LITERAL_FLOAT;
			}

[0-9]+ 			{
				yylval.literal.type = INTT;
				yylval.literal.value.int_value = atoi(yytext);
				return LITERAL_INT;
			}


[a-zA-Z][a-zA-Z_0-9]*	{
				yylval.variable.identifier = malloc(strlen(yytext)+1);
				strcpy(yylval.variable.identifier, yytext);
				return ID;
			}
		
["][^"]*["]		{
				yylval.literal.type = STRINGT;
				yylval.literal.value.string_value = malloc(sizeof(char) * yyleng-2);
				strncpy(yylval.literal.value.string_value, yytext+1, yyleng-2);
				return LITERAL_STRING;
			}

[ \t]			{
				/* pass */
			}

[/][/].*[\n]	{
				yylineno++;
			}

<IN_COMMENT>{
"*/"	BEGIN(INITIAL);
[^*\n]+
"*"
\n	yylineno++;
}

<IN_COMMENT><<EOF>> {
		yyerror("Unfinished comment");
		yyterminate();
		}
			
<INITIAL>{
"/*"		BEGIN(IN_COMMENT);
}
			
[\n]*		{
				yylineno++;
				/* return NEWLINE; */
			}

"\r\n"		{
				yylineno++;
				/* return NEWLINE; */
			}

.			{
				yylval.error = malloc(sizeof(char));
				strcpy(yylval.error, yytext);
				return ERROR;
			}

%%
