#include "headers/interpreter.h"

#define MaxReservedWords 5
#define MaxTokensTypes 27

int8 TokenWords[MaxTokensTypes][32] = {
		"var", "print", "input", "if", "repeat",
		"OPENPAREN", "CLOSEPAREN", "OPENBRACKED", "CLOSEBRACKED", "DOT", "PLUS", "MINUS",
		"STAR", "SLASH",

		"EQUAL",

		"EQUAL_EQUAL", "NOT_EQUAL", "GREATER", "LESS",
		"GREATER_EQUAL", "LESS_EQUAL",

		"COMMA", "SEMICOLON",

		"IDENTIFIER", "NUMBER", "STRING",

		"EOF"};

enum TokenType
{

	VAR = 0,
	PRINT,
	INPUT,
	IF,
	REPEAT,
	OPENPAREN,
	CLOSEPAREN,
	OPENBRACKED,
	CLOSEBRACKED,
	DOT,
	PLUS,
	MINUS,
	STAR,
	SLASH,

	EQUAL,

	EQUAL_EQUAL,
	NOT_EQUAL,
	GREATER,
	LESS,
	GREATER_EQUAL,
	LESS_EQUAL,

	COMMA,
	SEMICOLON,

	IDENTIFIER,
	NUMBER,
	STRING,

	EOF
};

struct Token
{
	int8 value[32];
	TokenType type;
};

void shift(int8 *buffer)
{
	buffer++;
}
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////ENVIRONMENT///////////////////////////////////////

#define MAX_IDENTIFIERS 512
#define MAX_NUMBERS 512
#define MAX_STRINGS 512
#define MAX_BINARYEXPRS 512
#define MAX_VARASSIGNMENT 512
#define MAX_FUNCTIONSTATEMENT 512
#define MAX_UNARYFUNCTIONSTATEMENT 512
#define MAX_FLOWFUNCTIONSTATEMENT 512

uint32 variables[MAX_IDENTIFIERS];
int8 symbols[MAX_IDENTIFIERS][32];
uint32 varIndex = 0;
class Environment
{
private:
	Environment *parent;
	uint32 variablesEnv[MAX_IDENTIFIERS];
	int8 symbolsEnv[MAX_IDENTIFIERS][32];

public:
	Environment() {}

	void init()
	{
		varIndex = 0;
		memset((uint32)variables, 0, sizeof(variables));
		for (uint32 i = 0; i < MAX_IDENTIFIERS; i++)
			memset((uint32)symbols[i], 0, 32);
	}

	uint32 has(int8 *varname)
	{
		for (uint32 i = 0; i < varIndex; i++)
		{
			if (strcmp(varname, symbols[i]) == 0)
	return i + 1;
		}
		return 0;
	}

	void showVars(uint32 max = varIndex)
	{
		for (uint32 i = 0; i < max; i++)
		{
			printf("/n");
			printf(symbols[i]);
			printf(": value: %d", lookupVar(symbols[i]));
		}
	}

	uint32 declareVar(int8 *varname, uint32 value)
	{
		uint32 s = this->has(varname);
		// if(s!=0){printf("VARIABLE YA DECLARADA: '");printf(varname);printf("'/n");return 0;}
		variables[varIndex] = value;
		strcpy(symbols[varIndex], varname);
		varIndex++;
		return value;
	}
	uint32 lookupVar(int8 *varname)
	{
		uint32 s = this->has(varname);
		// if(s==0){printf("VARIABLE NO DECLARADA: '");printf(varname);printf("'/n");return 0;}
		uint32 value = variables[s - 1];
		return value;
	}

	uint32 assignVar(int8 *varname, uint32 value)
	{
		uint32 s = this->has(varname);
		// if(s==0){printf("VARIABLE NO DECLARADA: '");printf(varname);printf("'/n");return 0;}
		variables[s - 1] = value;

		return value;
	}
	Environment resolve(int8 *varname)
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////PARSER//////////////////////////////////////////

#define NODES 11
int8 NodeWords[NODES][32] = {
		"NodeProgram",
		"NodeBlock",
		"NodeNumericLiteral",
		"NodeStringLiteral",
		"NodeIdentifier",
		"NodeBinaryExpr",
		"NodeVarAssignment",
		"NodeFunctionStatement",
		"NodeUnaryFunctionStatement",
		"NodeFlowFunctionStatement",
		"NodeEndExpression"};

enum NodeType
{
	NodeProgram,
	NodeBlock,
	NodeNumericLiteral,
	NodeStringLiteral,
	NodeIdentifier,
	NodeBinaryExpr,
	NodeVarAssignment,
	NodeFunctionStatement,
	NodeUnaryFunctionStatement,
	NodeFlowFunctionStatement,
	NodeEndExpression
};

class Statement
{
public:
	NodeType type;
};

class Program : public Statement
{
public:
	Program(NodeType nd, Statement **st)
	{
		type = nd;
		body = st;
	}
	Statement **body;
};

class Expr : public Statement
{
};

class BinaryExpr : public Expr
{
public:
	BinaryExpr() {}
	Expr *left;
	Expr *right;
	TokenType op;
};

class VarAssignment : public Expr
{
public:
	VarAssignment() {}
	int8 symbol[32];
	Expr *right;
	int8 op;
};

class EndExpression : public Expr
{
public:
	EndExpression() {}
};

class Identifier : public Expr
{
public:
	Identifier() {}
	int8 symbol[32];
};

class NumericLiteral : public Expr
{
public:
	NumericLiteral() {}
	int32 number;
};

class StringLiteral : public Expr
{
public:
	StringLiteral() {}
	int8 string[32];
};

class UnaryFunctionStatement : public Expr
{
public:
	UnaryFunctionStatement() {}
	int8 symbol[32];
	Expr *arg;
	TokenType subtype;
};

class FunctionStatement : public Expr
{
public:
	FunctionStatement() {}
	int8 symbol[32];
	uint32 nargc;
	Expr *argvs[32];
	TokenType subtype;
};
class FlowFunctionStatement : public Expr
{
public:
	FlowFunctionStatement() {}
	int8 symbol[32];
	Expr *comparison;
	TokenType subtype;
};

EndExpression end_pool[MAX_IDENTIFIERS];
uint32 end_count = 0;

Identifier identifier_pool[MAX_IDENTIFIERS];
uint32 indentifier_count = 0;

NumericLiteral number_pool[MAX_NUMBERS];
uint32 number_count = 0;

StringLiteral string_pool[MAX_STRINGS];
uint32 string_count = 0;

BinaryExpr binaryexpr_pool[MAX_BINARYEXPRS];
uint32 binaryexpr_count = 0;

VarAssignment varassignment_pool[MAX_VARASSIGNMENT];
uint32 varassignment_count = 0;

FunctionStatement functionstatement_pool[MAX_FUNCTIONSTATEMENT];
uint32 functionstatement_count = 0;

UnaryFunctionStatement unaryfunctionstatement_pool[MAX_UNARYFUNCTIONSTATEMENT];
uint32 unaryfunctionstatement_count = 0;

FlowFunctionStatement flowfunctionstatement_pool[MAX_FLOWFUNCTIONSTATEMENT];
uint32 flowfunctionstatement_count = 0;

static NumericLiteral tempNumericLiteral;
static Identifier tempIndentifier;
static StringLiteral tempStringLiteral;
static BinaryExpr tempBinaryExpr;

void initPools()
{
	end_count = 0;
	indentifier_count = 0;
	number_count = 0;
	string_count = 0;
	binaryexpr_count = 0;
	varassignment_count = 0;
	functionstatement_count = 0;
	unaryfunctionstatement_count = 0;
	flowfunctionstatement_count = 0;

	memset((uint32)end_pool, 0, MAX_IDENTIFIERS * sizeof(EndExpression));
	memset((uint32)identifier_pool, 0, MAX_IDENTIFIERS * sizeof(Identifier));
	memset((uint32)number_pool, 0, MAX_NUMBERS * sizeof(NumericLiteral));
	memset((uint32)string_pool, 0, MAX_STRINGS * sizeof(StringLiteral));
	memset((uint32)binaryexpr_pool, 0, MAX_BINARYEXPRS * sizeof(BinaryExpr));
	memset((uint32)varassignment_pool, 0, MAX_VARASSIGNMENT * sizeof(VarAssignment));
	memset((uint32)functionstatement_pool, 0, MAX_FUNCTIONSTATEMENT * sizeof(FunctionStatement));
	memset((uint32)unaryfunctionstatement_pool, 0, MAX_UNARYFUNCTIONSTATEMENT * sizeof(UnaryFunctionStatement));
	memset((uint32)flowfunctionstatement_pool, 0, MAX_FLOWFUNCTIONSTATEMENT * sizeof(FlowFunctionStatement));
}
Statement *make_all(Statement *ref);

EndExpression *make_end()
{
	if (end_count >= MAX_IDENTIFIERS)
		return nullptr;
	EndExpression *e = &end_pool[end_count++];
	e->type = NodeEndExpression;
	return e;
}

Identifier *make_identifiers(int8 *sym, uint8 op = 0)
{
	if (op == 1)
	{
		memcpy((uint32)sym, (uint32)tempIndentifier.symbol, 32);
		tempIndentifier.type == NodeIdentifier;
		return &tempIndentifier;
	}
	if (indentifier_count >= MAX_IDENTIFIERS)
		return nullptr;
	Identifier *id = &identifier_pool[indentifier_count++];
	memcpy((uint32)sym, (uint32)id->symbol, 32);
	id->type = NodeIdentifier;
	return id;
}

NumericLiteral *make_number(int32 num, uint8 op = 0)
{
	if (op == 1)
	{
		tempNumericLiteral.number = num;
		tempNumericLiteral.type == NodeNumericLiteral;
		return &tempNumericLiteral;
	}
	NumericLiteral *n;
	if (number_count >= MAX_NUMBERS)
		return nullptr;
	n = &number_pool[number_count++];
	n->number = num;
	n->type = NodeNumericLiteral;
	return n;
}

StringLiteral *make_string(int8 str[32], uint8 op = 0)
{
	if (op == 1)
	{
		strcpy(tempStringLiteral.string, str);
		tempStringLiteral.type == NodeStringLiteral;
		return &tempStringLiteral;
	}
	if (string_count >= MAX_STRINGS)
		return nullptr;
	StringLiteral *s = &string_pool[string_count++];
	strcpy(s->string, str);
	s->type = NodeStringLiteral;
	return s;
}

BinaryExpr *make_binary_expr(Expr *l, Expr *r, TokenType o, uint8 op = 0)
{
	if (op == 1)
	{
		tempBinaryExpr.left = l;
		tempBinaryExpr.right = r;
		tempBinaryExpr.op = o;
		tempBinaryExpr.type == NodeBinaryExpr;
		return &tempBinaryExpr;
	}
	if (binaryexpr_count >= MAX_BINARYEXPRS)
		return nullptr;
	BinaryExpr *b = &binaryexpr_pool[binaryexpr_count++];
	b->left = l;
	b->right = r;
	b->op = o;
	b->type = NodeBinaryExpr;
	return b;
}

VarAssignment *make_var_assignment(int8 *sym, Expr *r, int8 o)
{
	if (varassignment_count >= MAX_VARASSIGNMENT)
		return nullptr;
	VarAssignment *v = &varassignment_pool[varassignment_count++];
	memcpy((uint32)sym, (uint32)v->symbol, 32);
	v->right = r;
	v->type = NodeVarAssignment;
	v->op = o;
	return v;
}

UnaryFunctionStatement *make_unary_function_statement(int8 *sym, Expr *ar, TokenType o)
{
	if (unaryfunctionstatement_count >= MAX_UNARYFUNCTIONSTATEMENT)
		return nullptr;
	UnaryFunctionStatement *f = &unaryfunctionstatement_pool[unaryfunctionstatement_count++];
	strcpy(f->symbol, sym);
	f->arg = (Expr *)make_all(ar);
	f->subtype = o;
	f->type = NodeUnaryFunctionStatement;
	return f;
}

FunctionStatement *make_function_statement(int8 *sym, uint32 nar, Expr **ar, TokenType o)
{
	if (functionstatement_count >= MAX_FUNCTIONSTATEMENT)
		return nullptr;
	FunctionStatement *f = &functionstatement_pool[functionstatement_count++];
	strcpy(f->symbol, sym);
	for (uint32 i = 0; i < nar; i++)
		f->argvs[i] = (Expr *)make_all(ar[i]);

	f->nargc = nar;
	f->subtype = o;
	f->type = NodeFunctionStatement;
	return f;
}

FlowFunctionStatement *make_flow_function_statement(int8 *sym, Expr *ar, TokenType o)
{
	if (flowfunctionstatement_count >= MAX_FLOWFUNCTIONSTATEMENT)
		return nullptr;
	FlowFunctionStatement *f = &flowfunctionstatement_pool[flowfunctionstatement_count++];
	strcpy(f->symbol, sym);
	f->comparison = ar;
	f->subtype = o;
	f->type = NodeFlowFunctionStatement;

	return f;
}

Statement *make_all(Statement *ref)
{
	if (ref->type == NodeEndExpression)
	{
		return make_end();
	}
	if (ref->type == NodeNumericLiteral)
	{
		NumericLiteral *change = (NumericLiteral *)ref;
		return make_number(change->number);
	}
	else if (ref->type == NodeIdentifier)
	{
		Identifier *change = (Identifier *)ref;
		return make_identifiers(change->symbol);
	}
	else if (ref->type == NodeStringLiteral)
	{
		StringLiteral *change = (StringLiteral *)ref;
		return make_string(change->string);
	}
	else if (ref->type == NodeBinaryExpr)
	{
		BinaryExpr *change = (BinaryExpr *)ref;
		return make_binary_expr(change->left, change->right, change->op);
	}
	else if (ref->type == NodeVarAssignment)
	{
		VarAssignment *change = (VarAssignment *)ref;
		return make_var_assignment(change->symbol, change->right, change->op);
	}
	else if (ref->type == NodeUnaryFunctionStatement)
	{
		UnaryFunctionStatement *change = (UnaryFunctionStatement *)ref;
		return make_unary_function_statement(change->symbol, change->arg, change->subtype);
	}
	else if (ref->type == NodeFunctionStatement)
	{
		FunctionStatement *change = (FunctionStatement *)ref;
		return make_function_statement(change->symbol, change->nargc, change->argvs, change->subtype);
	}
	else if (ref->type = NodeFlowFunctionStatement)
	{
		FlowFunctionStatement *change = (FlowFunctionStatement *)ref;
		return make_flow_function_statement(change->symbol, change->comparison, change->subtype);
	}

	return make_number(0);
}

uint32 stackPrintNode = 0;
void printSpacesStack()
{
	for (uint32 i = 0; i < stackPrintNode; i++)
	{
		printf(" ");
	}
}

void printNodes(Statement *st)
{
	if (st->type == NodeNumericLiteral)
	{
		NumericLiteral *lit = (NumericLiteral *)st;
		printSpacesStack();
		printf("NUMBER: %d/n", lit->number);
	}
	else if (st->type == NodeIdentifier)
	{
		Identifier *id = (Identifier *)st;
		printSpacesStack();
		printf("IDENTIFIER: ");
		printf(id->symbol);
		printf("/n");
	}

	else if (st->type == NodeBinaryExpr)
	{
		BinaryExpr *be = (BinaryExpr *)st;
		printSpacesStack();
		printf("EXPR: /n");

		printSpacesStack();
		printf("LEFT:/n");
		stackPrintNode++;
		printSpacesStack();
		printNodes((Statement *)be->left);
		stackPrintNode--;
		printSpacesStack();
		printf("RIGHT:/n");
		stackPrintNode++;
		printSpacesStack();
		printNodes((Statement *)be->right);
		stackPrintNode--;
		printSpacesStack();
		printf("OPERATOR: ");
		printf(TokenWords[be->op]);
		printf("/n");
	}
	else if (st->type == NodeVarAssignment)
	{
		VarAssignment *va = (VarAssignment *)st;
		printSpacesStack();
		if (va->op == 0)
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
		printNodes((Statement *)va->right);
		stackPrintNode--;
		stackPrintNode--;
	}

	else if (st->type == NodeFunctionStatement)
	{
		FunctionStatement *fn = (FunctionStatement *)st;
		printSpacesStack();
		printf("FUNCTION: ");
		printf(fn->symbol);
		printf("/n");
		printSpacesStack();
		stackPrintNode++;
		for (uint32 i = 0; i < fn->nargc; i++)
		{
			Statement *res = (Statement *)fn->argvs[i];
			printNodes(res);
			// printf(NodeWords[res->type]);
		}
		stackPrintNode--;
	}
	else if (st->type == NodeUnaryFunctionStatement)
	{
		UnaryFunctionStatement *uf = (UnaryFunctionStatement *)st;
		printSpacesStack();
		printf("UNARY FUNCTION: ");
		printf(uf->symbol);
		printf("/n");
		printSpacesStack();
		stackPrintNode++;
		printNodes((Statement *)uf->arg);
		stackPrintNode--;
	}
	else if (st->type == NodeStringLiteral)
	{
		StringLiteral *str = (StringLiteral *)st;
		printSpacesStack();
		printf("STRING: ");
		printf(str->string);
		printf("/n");
	}
	else if (st->type == NodeFlowFunctionStatement)
	{
		FlowFunctionStatement *ff = (FlowFunctionStatement *)st;
		printSpacesStack();
		printf("FLOWFUNCTION: ");
		printf(ff->symbol);
		printf("/n");
		printSpacesStack();
		printf(" COMPARISON: /n");
		stackPrintNode++;
		printNodes((Statement *)ff->comparison);
		stackPrintNode--;
		printSpacesStack();
		printf(" BODY: /n");
	}
}

Expr *evaluateNodes(Statement *st, Environment *env)
{

	if (st->type == NodeStringLiteral)
	{
		StringLiteral *str = (StringLiteral *)st;
		return str;
	}

	else if (st->type == NodeNumericLiteral)
	{
		NumericLiteral *lit = (NumericLiteral *)st;
		return lit;
	}
	else if (st->type == NodeIdentifier)
	{
		Identifier *id = (Identifier *)st;
		return id;
	}

	else if (st->type == NodeBinaryExpr)
	{
		BinaryExpr *be = (BinaryExpr *)st;
		Expr *l = evaluateNodes((Statement *)be->left, env);
		Expr *r = evaluateNodes((Statement *)be->right, env);
		NumericLiteral *result;
		NumericLiteral *lcast, *rcast;

		if (l->type == NodeNumericLiteral)
		{
			lcast = (NumericLiteral *)l;
		}

		else if (l->type == NodeIdentifier)
		{
			uint32 n = env->lookupVar(((Identifier *)l)->symbol);
			lcast = make_number(n);
		}

		if (r->type == NodeNumericLiteral)
		{
			rcast = (NumericLiteral *)r;
		}

		else if (r->type == NodeIdentifier)
		{
			uint32 n = env->lookupVar(((Identifier *)r)->symbol);
			rcast = make_number(n);
		}

		switch (be->op)
		{
		case PLUS:
			return make_number(lcast->number + rcast->number);
		case MINUS:
			return make_number(lcast->number - rcast->number);
		case STAR:
			return make_number(lcast->number * rcast->number);
		case SLASH:
			if (rcast->number != 0)
	return make_number(lcast->number / rcast->number);
			return make_number(0);
		case EQUAL_EQUAL:
			return make_number((lcast->number) != (rcast->number));
		case NOT_EQUAL:
			return make_number((lcast->number) == (rcast->number));
		case GREATER:
			return make_number((lcast->number) <= (rcast->number));
		case GREATER_EQUAL:
			return make_number((lcast->number) < (rcast->number));
		case LESS:
			return make_number((lcast->number) >= (rcast->number));
		case LESS_EQUAL:
			return make_number((lcast->number) > (rcast->number));
		}
	}
	else if (st->type == NodeVarAssignment)
	{
		VarAssignment *va = (VarAssignment *)st;
		NumericLiteral *r;
		Statement *n = evaluateNodes((Statement *)va->right, env);
		// printf(NodeWords[n->type]);
		// printf("/n");
		if (n->type == NodeIdentifier)
		{
			uint32 s = env->lookupVar(((Identifier *)n)->symbol);
			r = make_number(s);
		}
		else if (n->type == NodeNumericLiteral)
		{
			uint32 num = ((NumericLiteral *)n)->number;
			r = make_number(num);
		}
		else if (n->type == NodeBinaryExpr)
		{
			Statement *result = evaluateNodes(n, env);
			if (result->type == NodeNumericLiteral)
	r = make_number(((NumericLiteral *)result)->number);
		}
		uint32 res = 1;
		if (va->op == 0)
			res = env->declareVar(va->symbol, r->number);
		else if (va->op == 1)
			res = env->assignVar(va->symbol, r->number);
		if (res == 0)
		{
			printf("/nERROR on var assignment/n");
		}
		return va;
	}

	else if (st->type == NodeFunctionStatement)
	{
		FunctionStatement *fs = (FunctionStatement *)st;
		if (fs->subtype == PRINT)
		{
			for (uint32 i = 0; i < fs->nargc; i++)
			{
	Statement *t = (Statement *)evaluateNodes(fs->argvs[i], env);
	if (t->type == NodeStringLiteral)
	{
		printf(((StringLiteral *)t)->string);
	}
	else if (t->type == NodeNumericLiteral)
	{
		printf("%d", ((NumericLiteral *)t)->number);
	}
	else if (t->type == NodeIdentifier)
	{
		uint32 n = env->lookupVar(((Identifier *)t)->symbol);
		printf("%d", n);
	}
	refresh();
			}
		}
	}
	else if (st->type == NodeUnaryFunctionStatement)
	{
		UnaryFunctionStatement *uf = (UnaryFunctionStatement *)st;
		if (uf->subtype == PRINT)
		{
			Statement *t = (Statement *)evaluateNodes(uf->arg, env);
			if (t->type == NodeStringLiteral)
			{
	printf(((StringLiteral *)t)->string);
			}
			else if (t->type == NodeNumericLiteral)
			{
	printf("%d", ((NumericLiteral *)t)->number);
			}
			else if (t->type == NodeIdentifier)
			{
	uint32 n = env->lookupVar(((Identifier *)t)->symbol);
	printf("%d", n);
			}
			else if (t->type == NodeBinaryExpr)
			{
	Statement *res = evaluateNodes(t, env);
	if (res->type == NodeNumericLiteral)
	{
		printf("%d", ((NumericLiteral *)res)->number);
	}
			}
			refresh();
		}
		else if (uf->subtype == INPUT)
		{
#define ENTER 128
			uint8 ch;
			eatKeyBuffered();
			uint32 Intindex = 0;
			int8 num[32];
			while (1)
			{
	ch = getLastAsciiKey();
	if (!isKeyPressed())
		continue;
	eatKeyBuffered();
	if (isNumeric(ch))
	{
		printf("%d", ch - 48);
		num[Intindex++] = ch;
		refresh();
	}
	if (ch == ENTER)
		break;
			}
			Statement *s = evaluateNodes(uf->arg, env);
			if (s->type == NodeIdentifier)
			{
	Identifier *i = (Identifier *)s;
	refresh();
	env->assignVar(i->symbol, toInt(num));
			}
			else
			{
	printf("/nPorque no es una variables?? el %d /n", s->type);
			}
		}
	}
	else if (st->type == NodeFlowFunctionStatement)
	{
		FlowFunctionStatement *ff = (FlowFunctionStatement *)st;
		if (ff->subtype == IF)
		{
			if (((NumericLiteral *)evaluateNodes(ff->comparison, env))->number)
			{
	return make_number(1);
			}
			else
			{
	return make_number(0);
			}
		}
		else if (ff->subtype == REPEAT)
		{
			uint32 n = ((NumericLiteral *)evaluateNodes(ff->comparison, env))->number;
			return make_number(n);
		}
	}
	refresh();
}

uint8 SHOWVARS = 0;

class Parser
{
private:
	Token *tokes;
	uint32 aToken = 0;
	uint32 globalNum = 0;

	Token at()
	{
		return this->tokes[aToken];
	}
	Token eat()
	{
		uint32 s = this->aToken;
		this->aToken++;
		return this->tokes[s];
	}
	Token expect(TokenType t)
	{
		if (this->at().type != t)
		{
			printf("ERROR on ");
			printf(TokenWords[this->at().type]);
			printf("/n");
		}
		return this->eat();
	}

	Statement *parse_stmt()
	{
		switch (this->at().type)
		{
		case VAR:
			return this->parse_var_declaration();
		case PRINT:
			return this->parse_print_expression();
		case INPUT:
			return this->parse_input_expression();
		case IF:
		case REPEAT:
			return this->parse_flow_expression();
		default:
			return this->parse_expr();
		}
	}

	Expr *parse_flow_expression()
	{
		Token a = this->eat();
		Expr **args;
		args = this->parse_args();

		// this->parse_block_function();
		this->expect(OPENBRACKED);
		return make_flow_function_statement(TokenWords[a.type], args[0], a.type);
	}

	Statement **parse_block_function()
	{
		this->expect(OPENBRACKED);
		Statement **b;
		uint32 i = 0;
		while (this->at().type != CLOSEBRACKED)
		{
			b[i++] = this->parse_stmt();
		}
		this->eat();
		this->globalNum = i;
		return b;
	}

	Expr *parse_input_expression()
	{
		this->eat();
		Expr **args;
		args = parse_args();

		return make_unary_function_statement("INPUT", args[0], INPUT);
	}

	Expr *parse_print_expression()
	{
		this->eat();
		Expr **args;
		args = parse_args();

		return make_unary_function_statement("PRINT", args[0], PRINT);
	}

	Expr **parse_args()
	{

		if (this->at().type != OPENPAREN)
		{
			printf("ERROR");
		}
		this->eat();
		Expr **args = this->parse_args_list();
		if (this->at().type != CLOSEPAREN)
		{
			printf("ERROR");
		}
		this->eat();

		return args;
	}

	Expr **parse_args_list()
	{
		Expr **args;
		args[0] = this->parse_expr();
		uint32 i = 1;
		while (this->at().type == COMMA)
		{
			this->eat();
			args[i] = this->parse_var_assignment();
			i++;
		}
		this->globalNum = i;
		return args;
	}

	Expr *parse_var_declaration()
	{
		this->eat();
		if (this->at().type != IDENTIFIER)
		{
			printf("Syntaxis Error/n");
		}
		int8 name[32];
		strcpy(name, this->at().value);
		make_identifiers(this->eat().value);

		if (this->eat().type != EQUAL)
		{
			printf("Syntaxis Error/n");
		}

		Expr *right = this->parse_expr();
		return make_var_assignment(name, right, 0);
	}

	Expr *parse_expr()
	{
		return parse_var_assignment();
	}

	Expr *parse_var_assignment()
	{
		Expr *left = this->parse_comparison_expr();
		if (this->at().type == EQUAL)
		{
			this->eat();
			Expr *right = this->parse_var_assignment();
			Identifier *var = (Identifier *)left;
			return make_var_assignment(var->symbol, right, 1);
		}
		return left;
	}

	Expr *parse_comparison_expr()
	{
		Expr *left = this->parse_additive_expr();

		while (this->at().type == EQUAL_EQUAL || this->at().type == NOT_EQUAL || this->at().type == GREATER_EQUAL || this->at().type == LESS_EQUAL || this->at().type == GREATER || this->at().type == LESS)
		{
			TokenType op = this->eat().type;
			Expr *right = this->parse_additive_expr();
			left = make_binary_expr(left, right, op);
		}
		return left;
	}

	Expr *parse_additive_expr()
	{
		Expr *left = this->parse_multiplicative_expr();

		while (this->at().type == PLUS || this->at().type == MINUS)
		{
			TokenType op = this->eat().type;
			Expr *right = this->parse_multiplicative_expr();
			left = make_binary_expr(left, right, op);
		}
		return left;
	}

	Expr *parse_multiplicative_expr()
	{
		Expr *left = this->parse_primary_expr();

		while (this->at().type == STAR || this->at().type == SLASH)
		{
			TokenType op = this->eat().type;
			Expr *right = this->parse_primary_expr();
			left = make_binary_expr(left, right, op);
		}
		return left;
	}

	Expr *parse_primary_expr()
	{
		TokenType tk = this->at().type;

		switch (tk)
		{
		case IDENTIFIER:
			return make_identifiers(this->eat().value);
			break;
		case NUMBER:
			return make_number(toInt(this->eat().value));
			break;
		case STRING:
			return make_string(this->eat().value);
			break;
		case OPENPAREN:
		{
			this->eat();
			Expr *value = this->parse_expr();
			this->eat();
			return value;
		}
		break;
		case CLOSEBRACKED:
			this->eat();
			return make_end();
			break;
		default:
			this->eat();
			printf("ERROR :");
			printf(TokenWords[tk]);
			printf("/n");
			return (Expr *)0;
			break;
		}
	}

public:
	Program produceAST(Token *t)
	{
		Statement *st[512];
		memset((uint32)st, 0, sizeof(st));
		Program prog(NodeProgram, st);
		uint32 prog_count = 0;
		this->tokes = t;
		initPools();
		this->globalNum = 0;
		Environment *env;
		env->init();
		// env->showVars(10);

		while (this->at().type != EOF)
		{
			prog.body[prog_count++] = this->parse_stmt();
		}

		uint32 lastStatement = 0;
		uint32 repeatCount = 0;
		for (uint32 i = 0; i < prog_count; i++)
		{

			// printNodes(prog.body[i]);
			if (prog.body[i]->type == NodeEndExpression && repeatCount > 0)
			{
	i = lastStatement;
	repeatCount--;
			}
			Expr *res = evaluateNodes(prog.body[i], env);

			if (prog.body[i]->type == NodeFlowFunctionStatement)
			{
	if (((FlowFunctionStatement *)prog.body[i])->subtype == IF && res->type == NodeNumericLiteral && ((NumericLiteral *)res)->number == 1)
	{
		while (prog.body[i]->type != NodeEndExpression)
		{
			i++;
		}
	}
	else if (((FlowFunctionStatement *)prog.body[i])->subtype == REPEAT)
	{
		lastStatement = i + 1;
		repeatCount = ((NumericLiteral *)res)->number;
	}
			}
			// if(res->type==NodeNumericLiteral){
			//   NumericLiteral* n=(NumericLiteral*)res;
			//   printf("Statement %d return: %d/n",i,n->number);
			//}
		}
		if (SHOWVARS)
			env->showVars();
		return prog;
	}
};

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////LEXER///////////////////////////////////////////

void Lexer(int8 *buffer);

Token tokens[512];
uint32 tokenIndex = 0;

void initInterpreterByArgv(int8 *argv)
{
	tokenIndex = 0;
	Lexer(argv);

	// Parser parser;

	// parser.produceAST(tokens);
}

uint8 SHOWTOKENS = 0;

void initInterpreterByFile(DIR file, uint8 opt1, uint8 opt2)
{
	SHOWVARS = opt1;
	SHOWTOKENS = opt2;

	int8 buffer[512];
	memset((uint32)buffer, 0, 512);
	file.read((uint8 *)buffer);
	tokenIndex = 0;

	Lexer(buffer + 4);

	Parser parser;

	parser.produceAST(tokens);
}

void asignToken(int8 *value, TokenType type)
{
	strcpy(tokens[tokenIndex].value, value);
	tokens[tokenIndex].type = type;
	tokenIndex++;
}

int8 *charToStr(int8 ch)
{
	int8 *str;
	str[0] = ch;
	str[1] = 0;
	return str;
}

void printTokens()
{
	for (uint32 i = 0; i < tokenIndex; i++)
	{
		printf(TokenWords[tokens[i].type]);
		printf(": ");
		printf(tokens[i].value);
		printf("/n");
	}
}

void Lexer(int8 *buffer)
{
	uint32 i = 0;
	int8 *referenceBuffer = buffer;
	memset((uint32)tokens, 0, sizeof(Token) * 512);
	// printf(buffer);
	// printf("/n");

	while (buffer[0] != 0)
	{

		if (buffer[0] == ' ' || buffer[0] == 13 || buffer[0] == 1)
		{
			buffer++;
			continue;
		}
		else if (buffer[0] == ';')
		{
			asignToken(charToStr(buffer[0]), SEMICOLON);
			buffer++;
		}
		else if (buffer[0] == ',')
		{
			asignToken(charToStr(buffer[0]), COMMA);
			buffer++;
		}
		else if (buffer[0] == '(')
		{
			asignToken(charToStr(buffer[0]), OPENPAREN);
			buffer++;
		}
		else if (buffer[0] == ')')
		{
			asignToken(charToStr(buffer[0]), CLOSEPAREN);
			buffer++;
		}
		else if (buffer[0] == '{')
		{
			asignToken(charToStr(buffer[0]), OPENBRACKED);
			buffer++;
		}
		else if (buffer[0] == '}')
		{
			asignToken(charToStr(buffer[0]), CLOSEBRACKED);
			buffer++;
		}
		else if (buffer[0] == '+')
		{
			asignToken(charToStr(buffer[0]), PLUS);
			buffer++;
		}
		else if (buffer[0] == '-')
		{
			asignToken(charToStr(buffer[0]), MINUS);
			buffer++;
		}
		else if (buffer[0] == '*')
		{
			asignToken(charToStr(buffer[0]), STAR);
			buffer++;
		}
		else if (buffer[0] == '/')
		{
			asignToken(charToStr(buffer[0]), SLASH);
			buffer++;
		}
		else if (buffer[0] == '=')
		{
			if (buffer[1] == '=')
			{
	asignToken(charToStr(buffer[0]), EQUAL_EQUAL);
	buffer++;
			}
			else
	asignToken(charToStr(buffer[0]), EQUAL);
			buffer++;
		}
		else if (buffer[0] == '>')
		{
			if (buffer[1] == '=')
			{
	asignToken(charToStr(buffer[0]), GREATER_EQUAL);
	buffer++;
			}
			else
			{
	asignToken(charToStr(buffer[0]), GREATER);
			}
			buffer++;
		}

		else if (buffer[0] == '<')
		{
			if (buffer[1] == '=')
			{
	asignToken(charToStr(buffer[0]), LESS_EQUAL);
	buffer++;
			}
			else
			{
	asignToken(charToStr(buffer[0]), LESS);
			}
			buffer++;
		}

		else
		{

			if (buffer[0] == '!' && buffer[1] == '=')
			{
	asignToken(charToStr(buffer[0]), NOT_EQUAL);
	buffer += 2;
			}

			else if (buffer[0] == 39)
			{

	int8 str[32];
	memset((uint32)str, 0, 32);
	uint32 i = 0;
	buffer++;
	while (1)
	{
		str[i] = buffer[0];
		buffer++;
		i++;
		if (buffer[0] == 39)
			break;
	}
	asignToken(str, STRING);
			}

			else if (isNumeric(buffer[0]))
			{
	int8 num[32];
	memset((uint32)num, 0, 32);
	num[0] = buffer[0];
	uint32 i = 1;
	buffer++;
	while (isNumeric(buffer[0]))
	{
		num[i] = buffer[0];
		buffer++;
		i++;
	}
	asignToken(num, NUMBER);
	buffer--;
			}

			else if (isAlpha(buffer[0]))
			{
	int8 ide[32];
	memset((uint32)ide, 0, 32);
	ide[0] = buffer[0];
	uint32 i = 1;
	buffer++;
	while (isAlpha(buffer[0]))
	{
		ide[i] = buffer[0];
		buffer++;
		i++;
	}
	uint32 res = 0;
	for (uint32 reserved = VAR; reserved < MaxReservedWords; reserved++)
	{
		if (strcmp(ide, TokenWords[reserved]) == 0)
		{
			asignToken(ide, (TokenType)reserved);
			res = 1;
		}
	}
	if (!res)
		asignToken(ide, IDENTIFIER);
	buffer--;
			}
			else
			{
	printf("LEXER ERROR '%d' on %d /n", buffer[0], buffer - referenceBuffer);
			}

			buffer++;
		}
	}
	asignToken("EOF", EOF);
	if (SHOWTOKENS)
		printTokens();
}
