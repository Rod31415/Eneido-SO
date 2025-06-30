#include "headers/interpreter.h"

#define MaxReservedWords 2
#define MaxTokensTypes 21

int8 TokenWords[MaxTokensTypes][32]={
  "VAR","PRINT",
  "OPENPAREN", "CLOSEPAREN", "DOT", "PLUS", "MINUS",
  "STAR", "SLASH",

  "EQUAL",

  "EQUAL_EQUAL", "NOT_EQUAL", "GREATER", "LESS",
  "GREATER_EQUAL", "LESS_EQUAL",

  "COMMA",

  "IDENTIFIER", "NUMBER", "STRING",

  "EOF"
};

enum TokenType{
   
  VAR=0,PRINT,
  OPENPAREN, CLOSEPAREN, DOT, PLUS, MINUS,
  STAR, SLASH,

  EQUAL,

  EQUAL_EQUAL, NOT_EQUAL, GREATER, LESS,
  GREATER_EQUAL, LESS_EQUAL,

  COMMA,

  IDENTIFIER, NUMBER, STRING,

  EOF
};


struct Token {
  int8 value[32];
  TokenType type;
};

void shift(int8* buffer){
buffer++;
}
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////ENVIRONMENT///////////////////////////////////////


#define MAX_IDENTIFIERS   512
#define MAX_NUMBERS       512
#define MAX_STRINGS       512
#define MAX_BINARYEXPRS   512
#define MAX_VARASSIGNMENT 512
#define MAX_FUNCTIONSTATEMENT 512


class Environment{
private:
  Environment *parent;
  uint32 varIndex=0;
  uint32 variables[MAX_IDENTIFIERS];
  int8 symbols[MAX_IDENTIFIERS][32];
  public:
  Environment(){
  }

  void init(){
    this->varIndex=0;
   memset((uint32)this->variables,0,MAX_IDENTIFIERS*sizeof(uint32));
  memset((uint32)this->symbols,0,MAX_IDENTIFIERS*32);
  }

  int32 has(int8* varname){
    for(uint32 i=0;i<this->varIndex;i++){
      if(strcmp(varname,this->symbols[i])==0)return i+1;
    }
    return 0;
  }

  uint32 declareVar(int8* varname, uint32 value){
    uint32 s=this->has(varname);
    if(s!=0){printf("VARIABLE YA DECLARADA: '");printf(varname);printf("'/n");}
    this->variables[this->varIndex]=value;
    strcpy(this->symbols[this->varIndex],varname);
    this->varIndex++;
        return value;
  }
  uint32 lookupVar(int8* varname){
    uint32 s=this->has(varname);
    if(s==0){printf("VARIABLE NO DECLARADA: '");printf(varname);printf("'/n");return 0;}
    uint32 value=this->variables[s-1];
    return value;
  }

  uint32 assignVar(int8* varname,uint32 value){
    int32 s=this->has(varname);
    if(s==0){printf("VARIABLE NO DECLARADA: '");printf(varname);printf("'/n");return 0;}

    this->variables[s-1]=value;
  
    return value;
  }
  Environment resolve(int8* varname){

  }


};





////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////PARSER//////////////////////////////////////////

enum NodeType{
  NodeProgram,
  NodeNumericLiteral,
  NodeStringLiteral,
  NodeIdentifier,
  NodeBinaryExpr,
  NodeVarAssignment,
  NodeFunctionStatement
  
};

class Statement{
  public:
  NodeType type;
};

class Program: public Statement{
  public:
    Program(NodeType nd,Statement** st){type=nd;body=st;}
    Statement** body;
};

class Expr: public Statement{
};

class BinaryExpr: public Expr {
  public:
  BinaryExpr(){}
  Expr* left;
  Expr* right;
  TokenType op;
};

class VarAssignment: public Expr {
  public:
  VarAssignment(){}
  int8 symbol[32];
  Expr* right;
  int8 op;
};

class Identifier: public Expr {
  public:
    Identifier(){}
    int8 symbol[32];
};

class NumericLiteral: public Expr {
  public:
    NumericLiteral(){}
    int32 number;
};

class StringLiteral: public Expr {
  public:
    StringLiteral(){}
    int8 string[32];
};

class FunctionStatement: public Expr {
  public:
    FunctionStatement(){}
    int8 symbol[32];
    uint32 nargc;
    Expr** argvs;
    TokenType subtype;
};




Identifier identifier_pool[MAX_IDENTIFIERS];
uint32 indentifier_count=0;

NumericLiteral number_pool[MAX_NUMBERS];
uint32 number_count=0;

StringLiteral string_pool[MAX_STRINGS];
uint32 string_count=0;


BinaryExpr binaryexpr_pool[MAX_BINARYEXPRS];
uint32 binaryexpr_count=0;

VarAssignment varassignment_pool[MAX_VARASSIGNMENT];
uint32 varassignment_count=0;

FunctionStatement functionstatement_pool[MAX_FUNCTIONSTATEMENT];
uint32 functionstatement_count=0;


Identifier* make_identifiers(int8* sym){
  if(indentifier_count>=MAX_IDENTIFIERS)return nullptr;
  Identifier* id =&identifier_pool[indentifier_count++];
memset((uint32)id->symbol,0,32);
  strcpy(id->symbol,sym);
  id->type=NodeIdentifier;
  return id;
}

NumericLiteral* make_number(int32 num){
  if(number_count>=MAX_NUMBERS)return nullptr;
  NumericLiteral* n = &number_pool[number_count++];
  n->number=num;
  n->type=NodeNumericLiteral;
  return n;
}

StringLiteral* make_string(int8 str[32]){
  if(string_count>=MAX_STRINGS)return nullptr;
  StringLiteral* s = &string_pool[string_count++];
  strcpy(s->string,str);
  s->type=NodeStringLiteral;
  return s;
}

BinaryExpr* make_binary_expr(Expr* l, Expr* r,TokenType o){
  if(binaryexpr_count>=MAX_BINARYEXPRS)return nullptr;
  BinaryExpr* b = &binaryexpr_pool[binaryexpr_count++];
  b->left=l;
  b->right=r;
  b->op=o;
  b->type=NodeBinaryExpr;
  return b;
}

VarAssignment* make_var_assignment(int8* sym, Expr* r,int8 o){
  if(varassignment_count>=MAX_VARASSIGNMENT)return nullptr;
  VarAssignment* v = &varassignment_pool[varassignment_count++];
  strcpy(v->symbol,sym);
  v->right=r;
  v->type=NodeVarAssignment;
  v->op=o;
  return v;
}

FunctionStatement* make_function_statement(int8* sym, uint32 nar,Expr** ar,TokenType o){
  if(functionstatement_count>=MAX_FUNCTIONSTATEMENT)return nullptr;
  FunctionStatement* f = &functionstatement_pool[functionstatement_count++];
  strcpy(f->symbol,sym);
  f->nargc=nar;
  f->argvs=ar;
  f->subtype=o;
  f->type=NodeFunctionStatement;
  return f;
}

uint32 stackPrintNode=0;
void printSpacesStack(){
  for(uint32 i=0;i<stackPrintNode;i++){
    printf(" ");
  }
}

void printNodes(Statement* st){
  if(st->type==NodeNumericLiteral){
    NumericLiteral *lit=(NumericLiteral*)st;
    printSpacesStack();
    printf("NUMBER: %d/n",lit->number);}
  else if(st->type==NodeIdentifier){
    Identifier *id=(Identifier*)st;
    printSpacesStack();
    printf("IDENTIFIER: ");printf(id->symbol);printf("/n");}
  else if(st->type==NodeBinaryExpr){
    BinaryExpr *be=(BinaryExpr*)st;
    printSpacesStack();
    printf("EXPR: /n");
   
    printSpacesStack();
      printf("LEFT:/n");
      stackPrintNode++;
    printSpacesStack();
      printNodes((Statement*)be->left);
      stackPrintNode--;
    printSpacesStack();
      printf("RIGHT:/n");
      stackPrintNode++;
    printSpacesStack();
      printNodes((Statement*)be->right);
      stackPrintNode--;
       printSpacesStack();
    int8 ch=0;
    switch (be->op) {
      case 4:ch='+';break;
      case 5:ch='-';break;
      case 6:ch='*';break;
      case 7:ch='/';break;
    }
    printf("OPERATOR: '%c'/n",ch);

    }
  else if(st->type==NodeVarAssignment){
    VarAssignment *va=(VarAssignment*)st;
    printSpacesStack();
    if(va->op==0)
    printf("DECLARE: /n");
    else
    printf("ASSIGN: /n");

    stackPrintNode++;
    printSpacesStack();
    printf("SYMBOL: ");
    printf(va->symbol);
    printf("/n");
    printSpacesStack();
    printf("RIGHT: /n");
    stackPrintNode++;
      printNodes((Statement*)va->right);
    stackPrintNode--;
    stackPrintNode--;


  }
}


Expr* evaluateNodes(Statement *st,Environment *env){
  if(st->type==NodeNumericLiteral){
    NumericLiteral *lit=(NumericLiteral*)st;
    return lit;
  }
  if(st->type==NodeIdentifier){
    Identifier *id=(Identifier*)st;
    NumericLiteral *num=make_number(env->lookupVar(id->symbol));
    return num;
  }

  if(st->type==NodeBinaryExpr){
    BinaryExpr *be=(BinaryExpr*)st;
    Expr* l=evaluateNodes((Statement*)be->left,env);
    Expr* r=evaluateNodes((Statement*)be->right,env);
    if(l->type!=NodeNumericLiteral&&r->type!=NodeNumericLiteral){
      printf("ERROR on BinaryExpr/n");
      return 0;
    }

    NumericLiteral* result;
    NumericLiteral *lcast,*rcast;
    lcast=(NumericLiteral*)l;
    rcast=(NumericLiteral*)r;
    
    
    switch (be->op) {
      case PLUS:
        return make_number(lcast->number+rcast->number);
      case MINUS:
        return make_number(lcast->number-rcast->number);
      case STAR:
        return make_number(lcast->number*rcast->number);
      case SLASH:
        if(rcast->number!=0)
        return make_number(lcast->number/rcast->number);
        return make_number(0);
      case EQUAL_EQUAL:
        return make_number(lcast->number==rcast->number);
      case NOT_EQUAL:
        return make_number(lcast->number!=rcast->number);
      case GREATER:
        return make_number(lcast->number>rcast->number);
      case GREATER_EQUAL:
        return make_number(lcast->number>=rcast->number);
      case LESS:
        return make_number(lcast->number<rcast->number);
      case LESS_EQUAL:
        return make_number(lcast->number<=rcast->number);
          
    }
  }
  if(st->type==NodeVarAssignment){
    VarAssignment *va=(VarAssignment*)st;
    NumericLiteral* r=(NumericLiteral*)evaluateNodes((Statement*)va->right,env);
    if(va->op==0)
    env->declareVar(va->symbol,r->number);
    if(va->op==1)
    env->assignVar(va->symbol,r->number);

    return va;
  }
  if(st->type==NodeFunctionStatement){
    FunctionStatement* fs=(FunctionStatement*)st;
    if(fs->subtype==PRINT){
      for(uint32 i=0;i<fs->nargc;i++){
        NodeType t=fs->argvs[i]->type;
        if(t==NodeNumericLiteral||t==NodeIdentifier||t==NodeBinaryExpr){
          printf("%d",((NumericLiteral*)evaluateNodes((Statement*)fs->argvs[i],env))->number);
        }
      }
    }
  }

}

class Parser{
  private:
    Token* tokes;
    uint32 aToken=0;
    
    Token at(){
      return this->tokes[aToken];
    }
    Token eat(){
      uint32 s=this->aToken;
      this->aToken++;
      return this->tokes[s];
    }



    Statement* parse_stmt(){
      switch (this->at().type) {

        case VAR:
          return parse_var_declaration();
        case PRINT:
          return parse_print_expression();
        default:    
          return this->parse_expr();
      }
    }

    Expr* parse_print_expression(){
      uint32 n=0;
      this->eat();
      Expr* right=this->parse_comparison_expr();
      Expr** args;
      args[0]=right;
      make_function_statement("PRINT",1,args,PRINT);
    }

    Expr* parse_var_declaration(){
      this->eat();
      if(this->at().type!=IDENTIFIER){
        printf("Syntaxis Error/n");
      }
      int8 name[32];
      strcpy(name,this->at().value);
      make_identifiers(this->eat().value);

      if(this->eat().type!=EQUAL){
       printf("Syntaxis Error/n");
      }
      
      Expr* right=this->parse_expr();
      return make_var_assignment(name,right,0);
    }

    
    Expr* parse_expr(){
      return parse_var_assignment();      
    }

  
    Expr* parse_var_assignment(){
      Expr* left=this->parse_comparison_expr();
      if(this->at().type==EQUAL){
        this->eat();
        Expr* right=this->parse_var_assignment();
        Identifier* var =(Identifier* )left;
        return make_var_assignment(var->symbol, right,1);
      }
      return left;

    }

       
    Expr* parse_comparison_expr(){
      Expr* left=this->parse_additive_expr();

      while(this->at().type==EQUAL_EQUAL||this->at().type==NOT_EQUAL||this->at().type==GREATER_EQUAL||this->at().type==LESS_EQUAL||this->at().type==GREATER||this->at().type==LESS){
        TokenType op =this->eat().type;
        Expr* right=this->parse_additive_expr();
        left = make_binary_expr(left,right,op);
      }
      return left;
    }

    Expr* parse_additive_expr(){
      Expr* left=this->parse_multiplicative_expr();

      while(this->at().type==PLUS||this->at().type==MINUS){
        TokenType op =this->eat().type;
        Expr* right=this->parse_multiplicative_expr();
        left = make_binary_expr(left,right,op);
      }
      return left;
    }
    
    Expr* parse_multiplicative_expr(){
      Expr* left=this->parse_primary_expr();

      while(this->at().type==STAR||this->at().type==SLASH){
        TokenType op =this->eat().type;
        Expr* right=this->parse_primary_expr();
        left = make_binary_expr(left,right,op);
      }
      return left;
    }

    Expr* parse_primary_expr(){
      TokenType tk=this->at().type;

      
      switch (tk) {
        case IDENTIFIER:
          return make_identifiers(this->eat().value);
            break;
        case NUMBER:
          return make_number(toInt(this->eat().value));
            break;
        case STRING:
            return make_string(this->eat().value);
            break;
        case OPENPAREN:{
            this->eat();
            Expr* value=this->parse_expr();
            this->eat();
            return value;}
            break;
        default:
          printf("ERROR/n");
          return (Expr*)0;
          break;
      }

    }

      public:
    Program produceAST(Token* t){
      Statement* st[512];
      Program prog(NodeProgram,st);
      uint32 prog_count=0;
      this->tokes=t;
      
 indentifier_count=0;
 number_count=0;
 string_count=0;
 binaryexpr_count=0;
 varassignment_count=0;
 functionstatement_count=0;

      Environment* env;
      env->init();
    
    while(this->at().type!=EOF){
  prog.body[prog_count++]=parse_stmt();
    }

   //printNodes(prog.body[prog_count-1]);
   for(uint32 i=0;i<prog_count;i++){
    Expr* res=evaluateNodes(prog.body[i],env);
   if(res->type==NodeNumericLiteral){
     NumericLiteral* n=(NumericLiteral*)res;
    printf("Statement %d return: %d/n",i+1,n->number);}
    }

    
  
          
      return prog;
    }
    


};

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////LEXER///////////////////////////////////////////


void Lexer(int8* buffer);

Token tokens[512];
uint32 tokenIndex=0;


void initInterpreterByArgv(int8* argv){
  tokenIndex=0;
Lexer(argv);

Parser parser;

parser.produceAST(tokens);

}

void initInterpreterByFile(DIR file){
  
  int8 buffer[512];
  memset((uint32)buffer,0,512);
 file.read((uint8*)buffer); 
 tokenIndex=0;

Lexer(buffer+4);

Parser parser;

parser.produceAST(tokens);
}
 

void asignToken(int8* value,TokenType type){
  strcpy(tokens[tokenIndex].value,value);
  tokens[tokenIndex].type=type;
  tokenIndex++;
}

int8* charToStr(int8 ch){
int8* str;
str[0]=ch;
str[1]=0;
return str;
}



void printTokens(){
  for(uint32 i=0;i<tokenIndex;i++){
    printf(TokenWords[tokens[i].type]);
    printf(": ");
    printf(tokens[i].value);
    printf("/n");
  }
}


void Lexer(int8* buffer){
 uint32 i=0;
 int8* referenceBuffer=buffer;
memset((uint32)tokens,0,sizeof(Token)*512);
//printf(buffer);
//printf("/n");


while(buffer[0]!=0){

  if(buffer[0]==' '){
    buffer++;
    continue;
  }
  else if(buffer[0]==','){
    asignToken(charToStr(buffer[0]),COMMA);
    buffer++;
  }
  else if(buffer[0]=='('){
    asignToken(charToStr(buffer[0]),OPENPAREN);    
    buffer++;
  }
  else if(buffer[0]==')'){
    asignToken(charToStr(buffer[0]),CLOSEPAREN);    
    buffer++;
  }
  else if(buffer[0]=='+'){
    asignToken(charToStr(buffer[0]),PLUS);
    buffer++;
  }
  else if(buffer[0]=='-'){
    asignToken(charToStr(buffer[0]),MINUS);
    buffer++;
  }
  else if(buffer[0]=='*'){
    asignToken(charToStr(buffer[0]),STAR);
    buffer++;
  }
  else if(buffer[0]=='/'){
    asignToken(charToStr(buffer[0]),SLASH);
    buffer++;
  }
  else if(buffer[0]=='='){
    if(buffer[1]=='='){
    asignToken(charToStr(buffer[0]),EQUAL_EQUAL);
      buffer++;
    }
    else
    asignToken(charToStr(buffer[0]),EQUAL);
    buffer++;
  }


  else{
  


    if(buffer[0]=='!'&&buffer[1]=='='){
    asignToken(charToStr(buffer[0]),NOT_EQUAL);
        buffer+=2;
    }
    else if(buffer[0]=='>'){
      if(buffer[1]=='='){
      asignToken(charToStr(buffer[0]),GREATER_EQUAL);
      buffer++;
      }
      else 
    asignToken(charToStr(buffer[0]),GREATER);
    buffer++;
    }
    
    else if(buffer[0]=='<'){
      if(buffer[1]=='='){
      asignToken(charToStr(buffer[0]),LESS_EQUAL);
      buffer++;
      }
      else
    asignToken(charToStr(buffer[0]),LESS);
    buffer++;
    }


    else if(buffer[0]==34){

      int8 str[32];
    memset((uint32)str,0,32);
    uint32 i=0;
    buffer++;
    while(buffer[0]!=34){
      str[i]=buffer[0];
      buffer++;
      i++;
    }
    asignToken(str,NUMBER);
    buffer--;

    }

    else if(isNumeric(buffer[0])){
    int8 num[32];
    memset((uint32)num,0,32);
    num[0]=buffer[0];
    uint32 i=1;
    buffer++;
    while(isNumeric(buffer[0])){
      num[i]=buffer[0];
      buffer++;
      i++;
    }
    asignToken(num,NUMBER);
    buffer--;
  }

  else if(isAlpha(buffer[0])){
    int8 ide[32];
    memset((uint32)ide,0,32);
    ide[0]=buffer[0];
    uint32 i=1;
    buffer++;
    while(isAlpha(buffer[0])){
      ide[i]=buffer[0];
      buffer++;
      i++;
    }
    uint32 res=0;
    for(uint32 reserved=VAR;reserved<MaxReservedWords;reserved++){
    if(strcmp(ide,TokenWords[reserved])==0){
      asignToken(ide,(TokenType)reserved);
      res=1;
    }
    }
    if(!res)
    asignToken(ide,IDENTIFIER);
    buffer--;
  }

   // printf("%d ",buffer-referenceBuffer);
    buffer++;
  }
}
asignToken("EOF",EOF);
//printTokens();


}



