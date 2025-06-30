#include "headers/interpreter.h"

#define MaxReservedWords 1
#define MaxTokensTypes 12

int8 TokenWords[MaxTokensTypes][32]={
  "VAR",
  "OPENPAREN", "CLOSEPAREN", "DOT", "PLUS", "MINUS",
  "STAR", "SLASH",

  "EQUAL",

  "IDENTIFIER", "NUMBER",

  "EOF"
};

enum TokenType{
   
  VAR=0,
  OPENPAREN, CLOSEPAREN, DOT, PLUS, MINUS,
  STAR, SLASH,

  EQUAL,

  EQUAL_EQUAL, NOT_EQUAL, GREATER, LESS,
  GREATER_EQUAL, LESS_EQUAL,

  IDENTIFIER, NUMBER,

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
////////////////////////////////////////PARSER//////////////////////////////////////////
enum NodeType{
  NodeProgram,
  NodeNumericLiteral,
  NodeIdentifier,
  NodeBinaryExpr
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

class Identifier: public Expr {
  public:
    Identifier(){}
    int8 symbol[32];
};

class NumericLiteral: public Expr {
  public:
    NumericLiteral(){}
    uint32 number;
};



#define MAX_IDENTIFIERS 512
#define MAX_NUMBERS     512
#define MAX_BINARYEXPRS 512

Identifier identifier_pool[MAX_IDENTIFIERS];
uint32 indentifier_count=0;

NumericLiteral number_pool[MAX_NUMBERS];
uint32 number_count=0;

BinaryExpr binaryexpr_pool[MAX_BINARYEXPRS];
uint32 binaryexpr_count=0;


Identifier* make_identifiers(int8* sym){
  if(indentifier_count>=MAX_IDENTIFIERS)return nullptr;
  Identifier* id =&identifier_pool[indentifier_count++];
memset((uint32)id->symbol,0,32);
  strcpy(id->symbol,sym);
  id->type=NodeIdentifier;
  return id;
}

NumericLiteral* make_number(uint32 num){
  if(number_count>=MAX_NUMBERS)return nullptr;
  NumericLiteral* n = &number_pool[number_count++];
  n->number=num;
  n->type=NodeNumericLiteral;
  return n;
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
}


uint32 evaluateNodes(Statement *st){
  if(st->type==NodeNumericLiteral){
    NumericLiteral *lit=(NumericLiteral*)st;
    return lit->number;
  }
  if(st->type==NodeBinaryExpr){
    BinaryExpr *be=(BinaryExpr*)st;
    uint32 l=evaluateNodes((Statement*)be->left);
    uint32 r=evaluateNodes((Statement*)be->right);
    uint32 result;
    switch (be->op) {
      case PLUS:
        return l+r;
      case MINUS:
        return l-r;
      case STAR:
        return l*r;
      case SLASH:
        if(r!=0)
        return l/r;
        return 0;
      case EQUAL_EQUAL:
        return l==r;
      case NOT_EQUAL:
        return l!=r;
      case GREATER:
        return l>r;
      case GREATER_EQUAL:
        return l>=r;
      case LESS:
        return l<r;
      case LESS_EQUAL:
        return l<=r;
          
    }
  }
}

class Parser{
  private:
    Token* tokes;
    uint32 aToken=0;
    
    Token at(){
      return tokes[aToken];
    }
    Token eat(){
      uint32 s=aToken;
      aToken++;
      return tokes[s];
    }

    Statement* parse_stmt(){
      return this->parse_expr();
    }

    Expr* parse_expr(){

      return this->parse_comparison_expr();
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
        case OPENPAREN:{
            this->eat();
            Expr* value=this->parse_expr();
            this->eat();
            return value;}
            break;
        case VAR:
              return make_number(toInt(this->eat().value));
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
    
    while(this->at().type!=EOF){
  prog.body[prog_count++]=parse_stmt();
   //printNodes(prog.body[prog_count-1]);
    }
    printf("/nResult: %d",evaluateNodes(prog.body[prog_count-1]));
      
      return prog;
    }
    


};

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////LEXER///////////////////////////////////////////


void Lexer(int8* buffer);

Token tokens[512];
uint32 tokenIndex=0;


void initInterpreterByArgv(int8* argv){}
void initInterpreterByFile(DIR file){

  int8 buffer[512];
  memset((uint32)buffer,0,512);
 file.read((uint8*)buffer); 
 tokenIndex=0;

Lexer(buffer+4);

Parser p;

p.produceAST(tokens);
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

//printf(buffer);
//printf("/n");


while(buffer[0]!=0){

  if(buffer[0]==' '){
    buffer++;
    continue;
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
    if(buffer[0]=='>'){
      if(buffer[1]=='='){
      asignToken(charToStr(buffer[0]),GREATER_EQUAL);
      buffer++;
      }
      else 
    asignToken(charToStr(buffer[0]),GREATER);
    buffer++;
    }
    
    if(buffer[0]=='<'){
      if(buffer[1]=='='){
      asignToken(charToStr(buffer[0]),LESS_EQUAL);
      buffer++;
      }
      else
    asignToken(charToStr(buffer[0]),LESS);
    buffer++;
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
printTokens();


}



