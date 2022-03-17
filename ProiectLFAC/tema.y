%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tema.h"

#define MAX 100

struct expr var[MAX];
struct expr stru[MAX][MAX];
struct denumire ids[MAX][MAX];
struct functii fct[MAX];
struct arbore arb[MAX];
int nrvar=0;
int structuri=0;
int nrstru=0;
int nrfunctii=0;
int val;
int tempnr=0;
int arbnod=1;
char tempvar[100][100];

void initids();
int addfrunza(char* tipu,int valu);
int addnod(char * tipu,char* valu,int st,int dr);
int evalarb(int n);
int add_var( char* tip, char*id, int cate,int where);
int assign(char* id1, char* id2,int poz1,int poz2);
int assignint(char* id, int nr,int poz);
int assignfloat(char* id, float nr, int poz);
int assignstring(char *id, char *nr, int poz);
int assignbool(char *id, int nr, int poz);
int returnint(char* id, int poz);
float returnfloat(char *id, int poz);
char* returnstring(char *id, int poz);
int returnbool(char *id, int poz);
int printint(char *s,int val);
int printfloat(char *s,float val);
int printbool(char *s,int val);
int adds(char *s, char *s2, int val);
int setstruct(char *s,int where);
int setvarstr(char *s,char *s2);
int sassign(char *idu, char *camp, char *s);
int sassignint(char *idu, char *camp, int val);
int sassignfloat(char *idu, char *camp, float val);
int sassignbool(char *idu, char *camp, int val);
int sassignstr(char *idu, char *camp, char *val);
int sreturnint(char* idu, char *camp);
int createfct(char *tip, char *nume,int gata,int where);
void addparam(char *tip, char *id);
void addtemp(const char *s);
int checkparam();
void goliretemp();
void afiseroare(int cod);
void makeconst(char *s);
void copyfiles();

extern FILE* yyin;
extern char* yytext;
extern int yylineno;
%}
%union{
int intval;
int boolval;
float floatval;
char* strval;
}
%token ASSIGN LE NE GE EQ IF WHILE FOR ELSE THEN DO PRINT AND OR STRUCT VARSTART VAREND PROGSTART PROGEND FCTSTART FCTEND CONST
%token <intval>NR
%token <floatval>FLOAT
%token <strval>string
%token <strval>outstring
%token <strval> TIP
%token <strval> ID
%token <strval> IDS
%token <boolval> TRUE
%token <boolval> FALSE
%type <intval>e
%type <floatval>flo
%type <strval>str
%type <boolval>boolexp
%left OR
%left AND
%left LE NE GE EQ '<' '>' 
%left '+' '-'
%left '*' '/' '%'


%start progr
%%
progr: declaratiivar functionsdeclare bloc {printf("Program corect sintactic.\n");}
     ;

/*declaratii de variabile*/
declaratiivar: VARSTART declaratii VAREND
             ;

declaratii :  declaratie ';'
     | declaratii declaratie ';'
     ;
declaratie : TIP ID  {afiseroare(add_var($1,$2,-1,0));}
           | TIP ID '[' NR ']' {afiseroare(add_var($1,$2,$4,0));}
           | CONST TIP ID ASSIGN NR {afiseroare(add_var($2,$3,-1,0));afiseroare(assignint($3,$<intval>5,-1));makeconst($3);}
           | CONST TIP ID ASSIGN FLOAT {afiseroare(add_var($2,$3,-1,0));afiseroare(assignfloat($3,$<floatval>5,-1));makeconst($3);}
           | CONST TIP ID ASSIGN string {afiseroare(add_var($2,$3,-1,0));afiseroare(assignstring($3,$<strval>5,-1));makeconst($3);}
           | CONST TIP ID ASSIGN TRUE {afiseroare(add_var($2,$3,-1,0));afiseroare(assignbool($3,$<boolval>5,-1));makeconst($3);}
           | CONST TIP ID ASSIGN FALSE {afiseroare(add_var($2,$3,-1,0));afiseroare(assignbool($3,$<boolval>5,-1));makeconst($3);}
           ;

/*declaratii de functii si structuri*/
functionsdeclare: FCTSTART declaratiif FCTEND
                ;
declaratiif : declarefct ';'
            | declaratiif declarefct ';'
            ; 

declarefct:  TIP ID '(' lista_param ')' {if(createfct($1,$2,-1,0)==0) yyerror("Functie deja declarata");}
           | TIP ID '(' ')' {if(createfct($1,$2,1,0)==0) yyerror("Functie deja declarata");}
           | STRUCT IDS '(' lista_params ')' {if(setstruct($2,0)==0) yyerror("Deja declarat");}
           | IDS ID {if(setvarstr($1,$2)==0) yyerror("Error");}
           ;

lista_param : TIP ID {addparam($1,$2);}
            | lista_param ','  TIP ID {addparam($3,$4);}
            ;


lista_params : TIP ID {adds($1,$2,-1);}
            | lista_params ','  TIP ID {adds($3,$4,-1);}  
            ;

      
/* bloc */
bloc : PROGSTART list PROGEND  {copyfiles();}
     ;
     
/* lista instructiuni */
list :  statement ';' 
     | list statement ';'
     | list IF cond THEN '{' list '}'
     | list IF cond THEN '{' list '}' ELSE '{' list '}'
     | list WHILE cond DO '{' list '}'
     | declaratiimain 
     ;
cond : '(' e ')'
     ;

declaratiimain :  declar ';'
     | declaratiimain declar ';'
     ;
declar : TIP ID  {afiseroare(add_var($1,$2,-1,1));}
       | TIP ID '[' NR ']' {afiseroare(add_var($1,$2,$4,1));}
       | CONST TIP ID ASSIGN NR {afiseroare(add_var($2,$3,-1,1));afiseroare(assignint($3,$<intval>5,-1));makeconst($3);}
       | CONST TIP ID ASSIGN FLOAT {afiseroare(add_var($2,$3,-1,1));afiseroare(assignfloat($3,$<floatval>5,-1));makeconst($3);}
       | CONST TIP ID ASSIGN string {afiseroare(add_var($2,$3,-1,1));afiseroare(assignstring($3,$<strval>5,-1));makeconst($3);}
       | CONST TIP ID ASSIGN TRUE {afiseroare(add_var($2,$3,-1,1));afiseroare(assignbool($3,$<boolval>5,-1));makeconst($3);}
       | CONST TIP ID ASSIGN FALSE {afiseroare(add_var($2,$3,-1,1));afiseroare(assignbool($3,$<boolval>5,-1));makeconst($3);}
       | TIP ID '(' lista_param ')' {if(createfct($1,$2,-1,1)==0) yyerror("Functie deja declarata");}
       | TIP ID '(' ')' {if(createfct($1,$2,1,1)==0) yyerror("Functie deja declarata");}
       | STRUCT IDS '(' lista_params ')' {if(setstruct($2,1)==0) yyerror("Deja declarat");}
       | IDS ID {if(setvarstr($1,$2)==0) yyerror("Error");}
       ;
       
/* instructiune */
statement: ID ASSIGN ID  {afiseroare(assign($1,$3,-1,-1));}
         | ID ASSIGN ID '[' NR ']'  {afiseroare(assign($1,$3,-1,$5));}
         | ID '[' NR ']' ASSIGN ID '[' NR ']'  {afiseroare(assign($1,$6,$3,$8));}
         | ID '[' NR ']' ASSIGN ID  {afiseroare(assign($1,$6,$3,-1));} 
         | ID ASSIGN e    {afiseroare(assignint($1,evalarb($<intval>3),-1));arbnod=1;}
         | ID ASSIGN flo    {afiseroare(assignfloat($1,$<floatval>3,-1));}
         | ID ASSIGN str    {afiseroare(assignstring($1,$<strval>3,-1));}
         | ID ASSIGN boolexp   {afiseroare(assignbool($1,$<boolval>3,-1));}
         | ID '[' NR ']' ASSIGN e    {afiseroare(assignint($1,$<intval>6,evalarb($3)));arbnod=1;}
         | ID '[' NR ']' ASSIGN flo    {afiseroare(assignfloat($1,$<floatval>6,$3));}
         | ID '[' NR ']' ASSIGN str    {afiseroare(assignstring($1,$<strval>6,$3));}
         | ID '[' NR ']' ASSIGN boolexp   {afiseroare(assignbool($1,$<boolval>6,$3));}
         | ID'.'ID ASSIGN ID  {afiseroare(sassign($1,$3,$5));}
         | ID'.'ID ASSIGN e  {afiseroare(sassignint($1,$3,evalarb($<intval>5)));arbnod=1;}
         | ID'.'ID ASSIGN flo  {afiseroare(sassignfloat($1,$3,$<floatval>5));}
         | ID'.'ID ASSIGN str  {afiseroare(sassignbool($1,$3,$<strval>5));}
         | ID'.'ID ASSIGN boolexp {afiseroare(sassignstr($1,$3,$<boolval>5));}
         | PRINT '(' outstring  ',' e       ')' { if(printint($3,evalarb($5))==0) yyerror("Eroare la apelarea functiei Print"); arbnod=1; }
         | PRINT '(' outstring  ',' flo     ')' { if(printfloat($3,$5)==0) yyerror("Eroare la apelarea functiei Print");  }
         | PRINT '(' outstring  ',' boolexp ')' { if(printbool($3,$5)==0) yyerror("Eroare la apelarea functiei Print");  }
         | nume st lista_apel dr {if(checkparam()==0) yyerror("Eroare la apelarea functiei");}
         ;
        
lista_apel : e {addtemp("int");}
           | flo {addtemp("float");}
           | str {addtemp("string");}
           | boolexp {addtemp("bool");}
           | lista_apel ',' e {addtemp("int");}
           | lista_apel ',' flo {addtemp("float");}
           | lista_apel ',' str {addtemp("string");}
           | lista_apel ',' boolexp {addtemp("bool");}
           | nume st lista_apel dr
           | lista_apel ',' nume st lista_apel dr
           ;
nume : ID {addtemp($1);}
     ;
st : '(' {addtemp("(");}
   ;
dr : ')' {addtemp(")");}
   ;

e : e '+' e   {$$=addnod("plus","+",$1,$3); }
  | e '*' e   {$$=addnod("inmul","*",$1,$3); }
  | e '-' e   {$$=addnod("minus","-",$1,$3); }
  | e '/' e   {$$=addnod("impar","/",$1,$3); }
  | e '%' e   {$$=addnod("modul","%",$1,$3);  }
  | e '<' e   {$$=addnod("maimic","<",$1,$3); }
  | e '>' e   {$$=addnod("maimare",">",$1,$3); }
  | e LE e   {$$=addnod("LE","<=",$1,$3); }
  | e GE e   {$$=addnod("GE",">=",$1,$3); }
  | e EQ e   {$$=addnod("egal","==",$1,$3); }
  | e NE e   {$$=addnod("notegal","!=",$1,$3);}
  | e AND e   {$$=addnod("si","&&",$1,$3); }
  | e OR e   {$$=addnod("ori","||",$1,$3);  }
  | '(' e ')' {$$=addnod("paran","()",$2,0);}
  | NR      {$$=addfrunza("NUMBER",$1); }
  | ID      {if(returnint($1,-1)==-19909) yyerror("Expresia contine tipuri diferite.");
            else if(returnint($1,-1)==-29909) yyerror("Varabila nedeclarata.");  else $$=addfrunza("IDENTIFIER",returnint($1,-1));}
  | ID '[' NR ']' {if(returnint($1,$3)==-19909) yyerror("Expresia contine tipuri diferite.");
            else if(returnint($1,$3)==-29909) yyerror("Varabila nedeclarata.");  else $$=addfrunza("ARRAY_ELEM",returnint($1,$3));}
  | ID'.'ID {if(sreturnint($1,$3)==-139909) yyerror("Structura nedeclarate");
                 else if(sreturnint($1,$3)==-149909) yyerror("Campul nu partine struccturii"); else if(sreturnint($1,$3)==-69909) yyerror("Expresia contine tipuri diferite."); else $$=addfrunza("STRUCT_IDENTIFIER",sreturnint($1,$3));}
  ;
flo: flo '+' flo   {$$=$1+$3;}
  | flo '*' flo   {$$=$1*$3;}
  | flo '-' flo   {$$=$1-$3; }
  | flo '/' flo   {$$=$1/$3; }
  | flo '<' flo   {$$=$1 < $3 ? 1 : 0; }
  | flo '>' flo   {$$=$1 > $3 ? 1 : 0;  }
  | flo LE flo   {$$=$1 <= $3 ? 1 : 0; }
  | flo GE flo   {$$=$1 >= $3 ? 1 : 0; }
  | flo EQ flo   {$$=$1 == $3 ? 1 : 0; }
  | flo NE flo   {$$=$1 != $3 ? 1 : 0; }
  | flo AND flo   {$$=$1 && $3 ? 1 : 0; }
  | flo OR flo   {$$=$1 || $3 ? 1 : 0; }
  | '(' flo ')' {$$=$2;}
  | FLOAT      {$$=$1; }
  | ID      {if(returnfloat($1,-1)==-19909) yyerror("Expresia contine tipuri diferite");
                 else if(returnfloat($1,-1)==-29909) yyerror("Varabila nedeclarata");  else $$=returnfloat($1,-1);}
  | ID '[' NR ']' {if(returnfloat($1,$3)==-19909) yyerror("Expresia contine tipuri diferite");
                 else if(returnfloat($1,$3)==-29909) yyerror("Varabila nedeclarata");  else $$=returnfloat($1,$3);}
  | ID'.'ID {if(sreturnfloat($1,$3)==-139909) yyerror("Structura nedeclarate");
                 else if(sreturnfloat($1,$3)==-149909) yyerror("Campul nu partine struccturii"); else if(sreturnfloat($1,$3)==-69909) yyerror("Expresia contine tipuri diferite."); else $$=sreturnfloat($1,$3);}
  ;
str : str '+' str  {
                    char* s=strdup($1);
                    strcat(s,$3);
                    $$=s;
                   }
    | '(' str ')' {$$=strdup($2);}
    | string      {$$=strdup($1);}
    | ID {if(strcmp(returnstring($1,-1),"ERR1")==0) yyerror("Expresia contine tipuri diferite");
                     else if(strcmp(returnstring($1,-1),"ERR2")==0) yyerror("Variabila nedeclarata"); else $$=returnstring($1,-1); }
    | ID '[' NR ']' {if(strcmp(returnstring($1,$3),"ERR1")==0) yyerror("Expresia contine tipuri diferite");
                     else if(strcmp(returnstring($1,$3),"ERR2")==0) yyerror("Variabila nedeclarata"); else $$=returnstring($1,$3); }
    | ID'.'ID {if(sreturnstr($1,$3)==-139909) yyerror("Structura nedeclarate");
                 else if(sreturnstr($1,$3)==-149909) yyerror("Campul nu partine struccturii"); else if(sreturnstr($1,$3)==-69909) yyerror("Expresia contine tipuri diferite."); else $$=sreturnstr($1,$3);}
    ;
boolexp : boolexp  EQ boolexp    {$$=$1 == $3 ? 1 : 0; }
      | boolexp  NE boolexp    {$$=$1 != $3 ? 1 : 0; }
      | '(' boolexp  ')' {$$=$2;}
      | TRUE      {$$=$1; }
      | FALSE      {$$=$1;  }
      | ID      {if(returnbool($1,-1)==-1) yyerror("Expresia contine tipuri diferite");
                 else if(returnbool($1,-1)==-2) yyerror("Varabila nedeclarata");  else $$=returnbool($1,-1);}
      | ID '[' NR ']' {if(returnbool($1,$3)==-1) yyerror("Expresia contine tipuri diferite");
                 else if(returnbool($1,$3)==-2) yyerror("Varabila nedeclarata");  else $$=returnbool($1,$3);}
      | ID'.'ID {if(sreturnbool($1,$3)==-139909) yyerror("Structura nedeclarate");
                 else if(sreturnbool($1,$3)==-149909) yyerror("Campul nu partine structurii"); else if(sreturnbool($1,$3)==-69909) yyerror("Expresia contine tipuri diferite."); else $$=sreturnbool($1,$3);}
      ;
%%
int yyerror(char * s){
printf("eroare: %s la linia:%d\n",s,yylineno);
exit(0);
}
int add_var( char* tip, char* id, int cate,int where)
{
  int i,j;
  for(i=0;i<nrvar;i++)
    {
        if(strcmp(var[i].idul,id)==0)
            return -119909;
    }
    var[nrvar].vecdim=cate;
    var[nrvar].unde=where;
    if(strcmp(tip,"string")==0)
    {
        strcpy(var[nrvar].idul,id);
        var[nrvar].type=2;
        if(cate>=100)
        return -129909;
        nrvar++;
        return 1;
    }
    if(strcmp(tip,"int")==0)
    {
        strcpy(var[nrvar].idul,id);
        var[nrvar].type=1;
        if(cate>=100)
        return -129909;
          else for(j=0;j<cate;j++) var[nrvar].intvec[j]=0;
        nrvar++;
        return 1;
    }
    if(strcmp(tip,"float")==0)
    {
        strcpy(var[nrvar].idul,id);
        var[nrvar].type=3;
        if(cate>=100)
        return -129909;
          else for(j=0;j<cate;j++) var[nrvar].floatvec[j]=0;
        nrvar++;
        return 1;
    }
    if(strcmp(tip,"bool")==0)
    {
        strcpy(var[nrvar].idul,id);
        var[nrvar].type=4;
        if(cate>=100)
        return -129909;
          else for(j=0;j<cate;j++) var[nrvar].boolvec[j]=0;
        nrvar++;
        return 1;
    }


}
int assign(char* id1, char* id2,int poz1,int poz2)
{
    int valint2,valbool2;
    char valstring2[100];
    float valfloat2;
    int i,j;
    for(i=0;i<nrvar;i++)
    {
        if(strcmp(var[i].idul,id1)==0)
         break;
    }
    for(j=0;j<nrvar;j++)
    {
        if(strcmp(var[j].idul,id2)==0)
         break;
    }
    if(i>=nrvar||j>=nrvar)
     return -59909;
    if(var[i].type!=var[j].type)
     return -69909;
    if((var[i].vecdim==-1&&poz1>=0)||(var[j].vecdim==-1&&poz2>=0))
      return -79909;
    if((var[i].vecdim>=0&&poz1==-1)||(var[j].vecdim>=0&&poz2==-1))
      return -89909;
    if((var[i].vecdim>=0&&poz1>=var[i].vecdim)||(var[j].vecdim>=0&&poz2>=var[j].vecdim))
      return -99909;
    if(var[i].isconst==1)
      return -159909;
     if(var[i].type==1)
     {
        if(poz2>=0) valint2=var[j].intvec[poz2]; else valint2=var[j].intvalue;
        if(poz1>=0) var[i].intvec[poz1]=valint2; else var[i].intvalue=valint2;
        return 1;
     }
     if(var[i].type==3)
     {
        if(poz2>=0) valfloat2=var[j].floatvec[poz2]; else valfloat2=var[j].floatvalue;
        if(poz1>=0) var[i].floatvec[poz1]=valfloat2; else var[i].floatvalue=valfloat2;
        return 1;
     }   
    if(var[i].type==4)
     {
        if(poz2>=0) valbool2=var[j].boolvec[poz2]; else valbool2=var[j].boolvalue;
        if(poz1>=0) var[i].boolvec[poz1]=valbool2; else var[i].boolvalue=valbool2;
        return 1;
     }   
    if(var[i].type==2)
     {
        if(poz2>=0) strcpy(valstring2,var[j].strvec[poz2]); else strcpy(valstring2,var[j].strvalue);
        if(poz1>=0) strcpy(var[i].strvec[poz1],valstring2); else strcpy(var[i].strvalue,valstring2);
        return 1;
     } 
    return -109909;
}
int sassign(char *idu, char *camp, char *s)
{
    int i,poz,fel,loc;
    for(i=0;i<nrstru;i++)
    {
        if(strcmp(idu,stru[i][0].idul)==0)
            {
                poz=i;
                fel=stru[i][0].type;
                break;
            }
    }
    if(i>=nrstru)
        return -139909;
    for(i=1;i<=10;i++)
        {
            if(strcmp(ids[poz][i].denum,camp)==0)
            {
                loc=i;
                break;
                
            }
        }
    if(i>10)
        return -149909;
    for(i=0;i<nrvar;i++)
        {
            if(strcmp(s,var[i].idul)==0)
                {
                    if(1==var[i].type&&fel==1)
                    {
                        stru[poz][loc].intvalue=var[i].intvalue;
                        return 1;
                    }
                    if(2==var[i].type&&fel==2)
                    {
                        strcpy(stru[poz][loc].strvalue,var[i].strvalue);
                        return 1;
                    }
                    if(3==var[i].type&&fel==3)
                    {
                        stru[poz][loc].floatvalue=var[i].floatvalue;
                        return 1;
                    }
                    if(4==var[i].type&&fel==4)
                    {
                        stru[poz][loc].boolvalue=var[i].boolvalue;
                        return 1;
                    }
                }
        }
    return -69909;
    
}
int sassignint(char *idu, char *camp, int val)
{
    int i,poz,fel,loc;
    for(i=0;i<nrstru;i++)
    {
        if(strcmp(idu,stru[i][0].idul)==0)
            {
                poz=i;
                fel=stru[i][0].type;
                break;
            }
    }
    if(i>=nrstru)
        return -139909;
    for(i=1;i<=10;i++)
        {
            if(strcmp(ids[poz][i].denum,camp)==0)
            {
                loc=i;
                break;
                
            }
        }
    if(i>10)
        return -149909;
    if(ids[poz][loc].tip==1)
        {
            stru[poz][loc].intvalue=val;
            return 1;
        }
    return -69909;
}
int sreturnint(char* idu, char *camp)
{
    int i,poz,fel,loc;
    for(i=0;i<nrstru;i++)
    {
        if(strcmp(idu,stru[i][0].idul)==0)
            {
                poz=i;
                fel=stru[i][0].type;
                break;
            }
    }
    if(i>=nrstru)
        return -139909;
    for(i=1;i<=10;i++)
        {
            if(strcmp(ids[poz][i].denum,camp)==0)
            {
                loc=i;
                break;
                
            }
        }
    if(i>10)
        return -149909;
    if(ids[poz][loc].tip==1)
        {
            return stru[poz][loc].intvalue;
        }
    return -69909;
}
int sreturnfloat(char* idu, char *camp)
{
    int i,poz,fel,loc;
    for(i=0;i<nrstru;i++)
    {
        if(strcmp(idu,stru[i][0].idul)==0)
            {
                poz=i;
                fel=stru[i][0].type;
                break;
            }
    }
    if(i>=nrstru)
        return -139909;
    for(i=1;i<=10;i++)
        {
            if(strcmp(ids[poz][i].denum,camp)==0)
            {
                loc=i;
                break;
                
            }
        }
    if(i>10)
        return -149909;
    if(ids[poz][loc].tip==3)
        {
            return stru[poz][loc].floatvalue;
        }
    return -69909;
}
int sreturnbool(char* idu, char *camp)
{
    int i,poz,fel,loc;
    for(i=0;i<nrstru;i++)
    {
        if(strcmp(idu,stru[i][0].idul)==0)
            {
                poz=i;
                fel=stru[i][0].type;
                break;
            }
    }
    if(i>=nrstru)
        return -139909;
    for(i=1;i<=10;i++)
        {
            if(strcmp(ids[poz][i].denum,camp)==0)
            {
                loc=i;
                break;
                
            }
        }
    if(i>10)
        return -149909;
    if(ids[poz][loc].tip==4)
        {
            return stru[poz][loc].boolvalue;
        }
    return -69909;
}
int sreturnstr(char* idu, char *camp)
{
    int i,poz,fel,loc;
    for(i=0;i<nrstru;i++)
    {
        if(strcmp(idu,stru[i][0].idul)==0)
            {
                poz=i;
                fel=stru[i][0].type;
                break;
            }
    }
    if(i>=nrstru)
        return -139909;
    for(i=1;i<=10;i++)
        {
            if(strcmp(ids[poz][i].denum,camp)==0)
            {
                loc=i;
                break;
                
            }
        }
    if(i>10)
        return -149909;
    if(ids[poz][loc].tip==2)
        {
            return stru[poz][loc].strvalue;
        }
    return -69909;
}
int sassignfloat(char *idu, char *camp, float val)
{
    int i,poz,fel,loc;
    for(i=0;i<nrstru;i++)
    {
        if(strcmp(idu,stru[i][0].idul)==0)
            {
                poz=i;
                fel=stru[i][0].type;
                break;
            }
    }
    if(i>=nrstru)
        return -139909;
    for(i=1;i<=10;i++)
        {
            if(strcmp(ids[poz][i].denum,camp)==0)
            {
                loc=i;
                break;
                
            }
        }
    if(i>10)
        return -149909;
printf("%d",ids[poz][loc].tip);
    if(ids[poz][loc].tip==3)
        {
            stru[poz][loc].floatvalue=val;
            return 1;
        }
     return -69909;
    printf("poate");
}
int sassignbool(char *idu, char *camp, int val)
{
    int i,poz,fel,loc;
    for(i=0;i<nrstru;i++)
    {
        if(strcmp(idu,stru[i][0].idul)==0)
            {
                poz=i;
                fel=stru[i][0].type;
                break;
            }
    }
    if(i>=nrstru)
        return -139909;
    for(i=1;i<=10;i++)
        {
            if(strcmp(ids[poz][i].denum,camp)==0)
            {
                loc=i;
                break;
                
            }
        }
    if(i>10)
        return -149909;
    if(ids[poz][loc].tip==4)
        {
            stru[poz][loc].boolvalue=val;
            return 1;
        }
    return -69909;
}
int sassignstr(char *idu, char *camp, char* val)
{
    int i,poz,fel,loc;
    for(i=0;i<nrstru;i++)
    {
        if(strcmp(idu,stru[i][0].idul)==0)
            {
                poz=i;
                fel=stru[i][0].type;
                break;
            }
    }
    if(i>=nrstru)
        return -139909;
    for(i=1;i<=10;i++)
        {
            if(strcmp(ids[poz][i].denum,camp)==0)
            {
                loc=i;
                break;
                
            }
        }
    if(i>10)
        return -149909;
    if(ids[poz][loc].tip==2)
        {
            strcpy(stru[poz][loc].strvalue,val);
            return 1;
        }
    return -69909;
}
int assignint(char* id, int nr,int poz)
{
    int i;
    for(i=0;i<nrvar;i++)
    {
        if(strcmp(var[i].idul,id)==0)
            {
              if(var[i].type!=1)
                return -19909;
              else if(var[i].vecdim==-1 && poz>=0)
                return -29909;
              else
              {
                 if(var[i].isconst==1)
                   return -159909;
                 if(poz==-1)
                 {var[i].intvalue=nr;
                 return 1;
                 }
                 else {
                   if(poz>=var[i].vecdim)
                     return -39909;
                  else {
                     var[i].intvec[poz]=nr; return 1;                  
                  } 
                 }
              }
            }
    }
    return -49909;
}

int assignbool(char* id, int nr,int poz)
{
    int i;
    for(i=0;i<nrvar;i++)
    {
        if(strcmp(var[i].idul,id)==0)
            {
              if(var[i].type!=4)
                return -19909;
              else if(var[i].vecdim==-1 && poz>=0)
                return -29909;
              else
              {
                 if(var[i].isconst==1)
                   return -159909;
                 if(poz==-1)
                 {var[i].boolvalue=nr;
                 return 1;
                 }
                 else {
                   if(poz>=var[i].vecdim)
                     return -39909;
                  else {
                     var[i].boolvec[poz]=nr; return 1;                  
                  } 
                 }
              }
            }
    }
    return -49909;
}
int assignfloat(char* id, float nr,int poz)
{
    int i;
    for(i=0;i<nrvar;i++)
    {
        if(strcmp(var[i].idul,id)==0)
            {
              if(var[i].type!=3)
                return -19909;
              else if(var[i].vecdim==-1 && poz>=0)
                return -29909;
              else
              {
                 if(var[i].isconst==1)
                   return -159909;
                 if(poz==-1)
                 {var[i].floatvalue=nr;
                 return 1;
                 }
                 else {
                   if(poz>=var[i].vecdim)
                     return -39909;
                  else {
                     var[i].floatvec[poz]=nr; return 1;                  
                  } 
                 }
              }
            }
    }
    return -49909;
}
int assignstring(char* id, char *nr,int poz)
{
    int i;
    for(i=0;i<nrvar;i++)
    {
        if(strcmp(var[i].idul,id)==0)
            {
                if(var[i].type!=2)
                  return -19909;
                else if(var[i].vecdim==-1 && poz>=0)
                 return -29909;
                else
              {
                 if(var[i].isconst==1)
                   return -159909;
                 if(poz==-1)
                 {strcpy(var[i].strvalue,nr);
                 return 1;
                 }
                 else {
                   if(poz>=var[i].vecdim)
                     return -39909;
                  else {
                     strcpy(var[i].strvec[poz],nr); return 1;                  
                  } 
                 }
              }
            }
    }
    return -49909;
}
int returnint(char* id,int poz)
{

    int i;
    for(i=0;i<nrvar;i++)
    {
        if(strcmp(var[i].idul,id)==0)
            {
                if(var[i].type==1)
                 {if(poz==-1)
                    return var[i].intvalue;
                    else return var[i].intvec[poz];
                 }
                 else return -19909;
            }
    }
    return -29909;
}
int returnbool(char* id,int poz)
{
    int i;
    for(i=0;i<nrvar;i++)
    {
        if(strcmp(var[i].idul,id)==0)
            {
                if(var[i].type==4)
                 {if(poz==-1)
                    return var[i].boolvalue;
                    else return var[i].boolvec[poz];
                 }
                return -1;
            }
    }
    return -2;
}
float returnfloat(char* id,int poz)
{
    int i;
    for(i=0;i<nrvar;i++)
    {
        if(strcmp(var[i].idul,id)==0)
            {
                if(var[i].type==3)
                  {if(poz==-1)
                    return var[i].floatvalue;
                    else return var[i].floatvec[poz];
                  }
                  else return -19909;
            }
    }
    return -29909;
}

char* returnstring(char* id,int poz)
{
    int i;
    for(i=0;i<nrvar;i++)
    {
        if(strcmp(var[i].idul,id)==0)
            {
                if(var[i].type==2)
                 {if(poz==-1)
                    return var[i].strvalue;
                    else return var[i].strvec[poz];
                 }
                 return "ERR1";
                
            }
    }
    return "ERR2";
}
void makeconst(char *s)
{
    int i;
    for(i=0;i<nrvar;i++)
    {
        if(strcmp(var[i].idul,s)==0)
         {var[i].isconst=1;
          break;
          }
    }
}

int printint(char *s,int val)
{
  char sir[300];
  strcpy(sir,s+1);
  sir[strlen(sir)-1]=0;
  char *p=strstr(sir,"%d");
  if(p==NULL)
    return 0;
  *p=0;
  printf("%s",sir);printf("%d",val);printf("%s\n",p+2); 
  return 1;   
}
int printfloat(char *s,float val)
{
  char sir[300];
  strcpy(sir,s+1);
  sir[strlen(sir)-1]=0;
  char *p=strstr(sir,"%f");
  if(p==NULL)
    return 0;
  *p=0;
  printf("%s",sir);printf("%.3f",val);printf("%s\n",p+2); 
  return 1;
}
int printbool(char *s,int val)
{
  char sir[300];
  strcpy(sir,s+1);
  sir[strlen(sir)-1]=0;
  char *p=strstr(sir,"%b");
  if(p==NULL)
    return 0;
  *p=0;
  printf("%s",sir);if(val==1) printf("true"); else printf("false"); printf("%s\n",p+2); 
  return 1;
}
int adds(char *s, char *s2, int val)
{
    strcpy(ids[structuri][ids[structuri][0].tip].denum,s2);
    if(val!=-1)
    {

    }
    else
    {
        if(strcmp(s,"float")==0)
            ids[structuri][ids[structuri][0].tip].tip=3;
        if(strcmp(s,"char")==0)
            ids[structuri][ids[structuri][0].tip].tip=2;
        if(strcmp(s,"int")==0)
            ids[structuri][ids[structuri][0].tip].tip=1;
        if(strcmp(s,"bool")==0)
            ids[structuri][ids[structuri][0].tip].tip=4;
        
    }
    ids[structuri][0].tip++;
}
int setstruct(char *s,int where)
{
    strcpy(ids[structuri][0].denum,s);
    ids[structuri][0].unde=where;
    for(int i=0;i<structuri;i++)
    {
        if(strcmp(ids[i][0].denum,s)==0)
            {
                return 0;
            }
    }
    
    structuri++;
    return 1;
}
void initids()
{
    for(int i=0;i<100;i++)
    {
        ids[i][0].tip=1;
    }
}
int setvarstr(char *s,char *s2)
{
        int i=0,fel;
        for(i=0;i<structuri;i++)
            if(strcmp(s,ids[i][0].denum)==0)
                {
                    fel=structuri;
                    break;
                }
        if(i>=structuri)
            return 0;
        stru[nrstru][0].type=fel;
        strcpy(stru[nrstru][0].idul,s2);
        nrstru++;
        return 1;
        
}
int createfct(char *tip, char *nume,int gata,int where)
{
    int i,j,ok;
    if(gata==-1)
    {
     strcpy(fct[nrfunctii].tipreturn,tip);
     strcpy(fct[nrfunctii].numefct,nume);
     fct[nrfunctii].unde=where;
     for(i=0;i<nrfunctii;i++)
         if(strcmp(fct[i].numefct,fct[nrfunctii].numefct)==0)
           {
             if(fct[i].nrpar==fct[nrfunctii].nrpar)
              {
                ok=0;
                    for(j=0;j<fct[i].nrpar;j++)
                         if(strcmp(fct[i].tipul[j],fct[nrfunctii].tipul[j])!=0)
                         {
                           ok=1;break;
                         }
                if(!ok) return 0;
              }
           }
       nrfunctii++;
    }
    else
    {
     strcpy(fct[nrfunctii].tipreturn,tip);
     strcpy(fct[nrfunctii].numefct,nume);
     fct[nrfunctii].nrpar=0;
     for(i=0;i<nrfunctii;i++)
     if(strcmp(fct[i].numefct,fct[nrfunctii].numefct)==0)
       return 0;
     nrfunctii++;
    }
    return 1;
}
void addparam(char *tip, char *id)
{

    strcpy(fct[nrfunctii].tipul[fct[nrfunctii].nrpar],tip);
    strcpy(fct[nrfunctii].numepar[fct[nrfunctii].nrpar],id);
    fct[nrfunctii].nrpar++;
}
void addtemp(const char *s)
{
 strcpy(tempvar[tempnr],s);
 tempnr++;   
}
int checkparam()
{
     int ok,i,j,jnou,k,caut,m,gasit,gasit1;
     ok=1;
     while(ok)
      { ok=0; 

       for(i=0;i<tempnr;i++)
       if(strcmp(tempvar[i],")")==0)
       {
          j=i;
          while( strcmp(tempvar[j],"(")!=0 )
            j--;
          j--;
          gasit=0;
          for(m=0;m<nrfunctii;m++)
           if(strcmp(fct[m].numefct,tempvar[j])==0)
           {
             caut=m;
             if(i-j-2!=fct[caut].nrpar)
               continue;
             else
             {
             gasit1=1;
             for(k=0;k<fct[caut].nrpar;k++)
                  if(strcmp(fct[caut].tipul[k],tempvar[j+2+k])!=0)
                   {
                     gasit1=0;
                     break;
                   }
             if(!gasit1)
               continue;
             }
            gasit=1;
            break;
            }
          if(!gasit)
            {goliretemp();return 0;}
          strcpy(tempvar[j],fct[caut].tipreturn);
          jnou=j;
          for(k=i+1;k<tempnr;k++)
           {
           strcpy(tempvar[jnou+1],tempvar[k]);
           jnou++;
           }
          tempnr=tempnr-i+j;
          ok=1;
          break;
       }
      }
      goliretemp();
      return 1;
}
void goliretemp()
{
  int i;
  for(i=0;i<100;i++)
    memset(tempvar[i],0,sizeof(tempvar[i]));
  tempnr=0;
}
void afiseroare(int cod)
{
    if(cod==-19909)
     yyerror("Expresia contine tipuri diferite");
    else if(cod==-29909)
     yyerror("Variabila din stanga asignarii nu este de tip vector");
    else if(cod==-39909)
     yyerror("Index in afara limitei");
    else if(cod==-49909)
     yyerror("Variabila din stanga asignarii nu a fost declarata");
    else if(cod==-59909)
     yyerror("Variabila nedeclarata");
    else if(cod==-69909)
     yyerror("Asignare intre tipuri diferite imposibila");
    else if(cod==-79909)
     yyerror("Variabila nu este de tip vector");
    else if(cod==-89909)
     yyerror("Variabila este de tip vector");
    else if(cod==-99909)
     yyerror("Index in afara limitei");
    else if(cod==-109909)
     yyerror("Eroare la asignare");
    else if(cod==-119909)
     yyerror("Variabila deja declarata");
    else if(cod==-129909)
     yyerror("Dimensiunea vectorului este peste cea admisa");
    else if(cod==-139909)
     yyerror("Structura nu a fost declarata");
    else if(cod==-149909)
     yyerror("Campul nu apartine structurii");
    else if(cod==-159909)
     yyerror("Nu se poate atribui o valoare unei variabie constante");
}
int addfrunza(char* tipu,int valu)
{
    strcpy(arb[arbnod].tip,tipu);
    arb[arbnod].val=valu;
    arbnod++;
    return arbnod-1;
}
int addnod(char * tipu,char* valu,int st,int dr)
{
    strcpy(arb[arbnod].tip,valu);
    //strcpy(arb[arbnod].val,valu);
    arb[arbnod].nod_stang=st;
    arb[arbnod].nod_drept=dr;
    arbnod++;
    return arbnod-1;
}
int evalarb(int n)
{
    if(arb[n].nod_stang==0&&arb[n].nod_drept==0)
        return arb[n].val;
    if(strcmp(arb[n].tip,"+")==0)
        return evalarb(arb[n].nod_stang)+evalarb(arb[n].nod_drept);
    if(strcmp(arb[n].tip,"-")==0)
        return evalarb(arb[n].nod_stang)-evalarb(arb[n].nod_drept);
    if(strcmp(arb[n].tip,"*")==0)
        return evalarb(arb[n].nod_stang)*evalarb(arb[n].nod_drept);
    if(strcmp(arb[n].tip,"/")==0)
        return evalarb(arb[n].nod_stang)/evalarb(arb[n].nod_drept);
    if(strcmp(arb[n].tip,"%")==0)
        return evalarb(arb[n].nod_stang)%evalarb(arb[n].nod_drept);
    if(strcmp(arb[n].tip,"<")==0)
        return evalarb(arb[n].nod_stang) < evalarb(arb[n].nod_drept);
    if(strcmp(arb[n].tip,">")==0)
        return evalarb(arb[n].nod_stang) > evalarb(arb[n].nod_drept);
    if(strcmp(arb[n].tip,">=")==0)
        return evalarb(arb[n].nod_stang) >= evalarb(arb[n].nod_drept);
    if(strcmp(arb[n].tip,"<=")==0)
        return evalarb(arb[n].nod_stang) <= evalarb(arb[n].nod_drept);
    if(strcmp(arb[n].tip,"==")==0)
        return evalarb(arb[n].nod_stang)==evalarb(arb[n].nod_drept);
    if(strcmp(arb[n].tip,"!=")==0)
        return evalarb(arb[n].nod_stang)!=evalarb(arb[n].nod_drept);
    if(strcmp(arb[n].tip,"&&")==0)
        return evalarb(arb[n].nod_stang)&&evalarb(arb[n].nod_drept);
    if(strcmp(arb[n].tip,"||")==0)
        return evalarb(arb[n].nod_stang)||evalarb(arb[n].nod_drept);
    if(strcmp(arb[n].tip,"()")==0)
        return evalarb(arb[n].nod_stang);
    return 0;

}
void copyfiles()
{
    int i,j;
    char msg[20000];
    char aux[300];
    char aux2[300];
    FILE *vartab;
    FILE *fcttab;
    vartab=fopen("symbol_table.txt","w");
    for(int i=0;i<nrvar;i++)
    {
    sprintf(aux,"name: %s\n",var[i].idul);strcat(msg,aux);
    if(var[i].type==1)
      {sprintf(aux,"typename: int\n");
       sprintf(aux2,"value: %d\n",var[i].intvalue);
      }
    else if(var[i].type==2)
      {sprintf(aux,"typename: string\n");
      sprintf(aux2,"value: %s\n",var[i].strvalue);
      }
    else if(var[i].type==3)
      {sprintf(aux,"typename: float\n");
       sprintf(aux2,"value: %f\n",var[i].floatvalue);
      }
    else if(var[i].type==4)
      {sprintf(aux,"typename: bool\n");
       if(var[i].boolvalue==1) sprintf(aux2,"value: true\n"); else sprintf(aux2,"value: false\n");
      }
    strcat(msg,aux);
    if(var[i].vecdim>=0)
     {
        sprintf(aux,"is_vector: yes\nsize_of_vector: %d\n",var[i].vecdim);
        strcat(msg,aux);     
     }
    else{
        if(var[i].isconst==1)
          {
            strcat(msg,"is_vector: no\nis_const: yes\n");   
          }
        else
          {
            strcat(msg,"is_vector: no\nis_const: no\n"); 
          }
        strcat(msg,aux2);
     }
     if(var[i].unde==1)
      strcat(msg,"stackframe: main\n");
     else if(var[i].unde==0)
      strcat(msg,"stackframe: global\n");
     strcat(msg,"\n");
    }
  for(i=0;i<structuri;i++)
    {
    sprintf(aux,"name: %s\ntypename: struct\n",ids[i][0].denum);
    strcat(msg,aux);
    if(ids[i][0].unde==0)
     strcat(msg,"stackframe: global\n");
     else strcat(msg,"stackframe: main\n");
    for(j=1;j<10;j++)
        {  
        if(ids[i][j].tip==3)
            {sprintf(aux,"member_%d_type: float | member_%d_name: %s\n",j,j,ids[i][j].denum);strcat(msg,aux);}
        if(ids[i][j].tip==2)
            {sprintf(aux,"member_%d_type: string | member_%d_name: %s\n",j,j,ids[i][j].denum);strcat(msg,aux);}
        if(ids[i][j].tip==1)
            {sprintf(aux,"member_%d_type: int | member_%d_name: %s\n",j,j,ids[i][j].denum);strcat(msg,aux);}
        if(ids[i][j].tip==4)
            {sprintf(aux,"member_%d_type: bool | member_%d_name: %s\n",j,j,ids[i][j].denum);strcat(msg,aux);}
        if(ids[i][j].tip==0)
          break;
        }
      strcat(msg,"\n");
    }
  fprintf(vartab,"%s",msg);
  fclose(vartab);

  fcttab=fopen("symbol_table_functions.txt","w");

  memset(msg,0,sizeof(msg));
  memset(aux,0,sizeof(aux));
  memset(aux2,0,sizeof(aux2));

  for(i=0;i<nrfunctii;i++)
  {
    sprintf(aux,"name: %s\nreturntype: %s\nnr_parameters: %d\n",fct[i].numefct,fct[i].tipreturn,fct[i].nrpar);
    strcat(msg,aux);
    if(fct[i].unde==0)
    strcat(msg,"stackframe: global\n");
    else if(fct[i].unde==1) strcat(msg,"stackframe: main\n");
    for(j=0;j<fct[i].nrpar;j++)
      {
        sprintf(aux,"param_numb_%d_type: %s | param_numb_%d_name: %s\n",j+1,fct[i].tipul[j],j+1,fct[i].numepar[j]);
        strcat(msg,aux);
      }
    strcat(msg,"\n");
  }
  fprintf(fcttab,"%s",msg);
  fclose(fcttab);

}
int main(int argc, char** argv){
initids();
yyin=fopen(argv[1],"r");
yyparse();
} 

