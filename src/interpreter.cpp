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
////////////////////////////////////////LEXER///////////////////////////////////////////


void Lexer(int8* buffer);

Token* tokens;
uint32 tokenIndex=1;


void initInterpreterByArgv(int8* argv){}
void initInterpreterByFile(DIR file){

  int8 buffer[512];
  memset((uint32)buffer,0,512);
 file.read((uint8*)buffer); 
 tokenIndex=1;

Lexer(buffer);
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


int8 isNumeric(int8 ch){
return (ch>=48&&ch<=57);
}

int8 isAlpha(int8 ch){
return ((ch>=65&&ch<=90)||(ch>=97&&ch<=122));
}

void printTokens(){
  for(uint32 i=1;i<tokenIndex;i++){
    printf(TokenWords[tokens[i].type]);
    printf(": ");
    printf(tokens[i].value);
    printf("/n");
  }
}


void Lexer(int8* buffer){
 uint32 i=0;
 int8* referenceBuffer=buffer;

printf(buffer);
printf("/n");


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
    asignToken(charToStr(buffer[0]),EQUAL);
    buffer++;
  }
  else{

if(isNumeric(buffer[0])){
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

printTokens();
}
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////PARSER//////////////////////////////////////////






