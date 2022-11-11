#include "astnode.h"

#define DEBUG

extern int spaces;
extern std::unique_ptr<LLVMContext> theContext;
extern std::unique_ptr<Module> theModule;
extern std::unique_ptr<IRBuilder<>> builder;
extern std::map<std::string, AllocaInst *> namedValues; // 存当前函数局部变量
extern std::unique_ptr<legacy::FunctionPassManager> theFPM;
extern int grammererror;
extern std::map<std::string, AllocaInst *> curNamedValues; // 存当前所有变量

extern BasicBlock *continueBasicBlock;
void printspaces() {
  for (int i = 0; i < spaces; ++i)
    std::cout << " ";
}
void printGrammerInfo(std::string nodeName, int line) {
  printspaces();
  std::cout << nodeName << " (" << line << ")" << std::endl;
}

void printSemanticError(int type, int line, std::string info = "") {
  grammererror = 1;
  std::cout << "Error type " << type << " at Line " << line << "."
            << std::endl;
  exit(0);
}

int parseNIdentifier(NIdentifier &nIdentifier) {
  printspaces();
  std::cout << "ID: " << nIdentifier.name << std::endl;
  return 0;
}

Value *LogErrorV(const char *Str) {
  // std::cout << Str << std::endl;
  return nullptr;
}

void InitializeModuleAndPassManager() {
  // Open a new module.
  theContext = std::make_unique<LLVMContext>();
  theModule = std::make_unique<Module>("test", *theContext);

  // theModule->setDataLayout(dL);

  // Create a new builder for the module.
  builder = std::make_unique<IRBuilder<>>(*theContext);

  // Create a new pass manager attached to it.
  theFPM = std::make_unique<legacy::FunctionPassManager>(theModule.get());

  // Promote allocas to registers.
  //theFPM->add(createPromoteMemoryToRegisterPass());
  // Do simple "peephole" optimizations and bit-twiddling optzns.
  //theFPM->add(createInstructionCombiningPass());
  // Reassociate expressions.
  //theFPM->add(createReassociatePass());
  // Eliminate Common SubExpressions.
  //theFPM->add(createGVNPass());
  // Simplify the control flow graph (deleting unreachable blocks, etc).
  //theFPM->add(createCFGSimplificationPass());

  theFPM->doInitialization();
}

Function *getFunction(std::string Name) {
  // First, see if the function has already been added to the current module.
  if (auto *F = theModule->getFunction(Name))
    return F;
  return nullptr;
}

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block
/// of the function.  This is used for mutable variables etc.
static AllocaInst *CreateEntryBlockAlloca(Function *TheFunction,
                                          StringRef VarName, Type *varType) {
  IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                   TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(varType, nullptr, VarName);
}

int NInteger::parse() {
  printGrammerInfo(getNodeName(), line);
  spaces += 2;
  printspaces();
  std::cout << "INT"
            << ": " << value << std::endl;
  spaces -= 2;
  return 0;
}
int NFloat::parse() {
  printGrammerInfo(getNodeName(), line);
  spaces += 2;
  printspaces();
  std::cout << "FLOAT"
            << ": " << value << std::endl;
  spaces -= 2;
  return 0;
}
int NChar::parse() {
  printGrammerInfo(getNodeName(), line);
  spaces += 2;
  printspaces();
  std::cout << "CHAR"
            << ": " << value << std::endl;
  spaces -= 2;
  return 0;
}
int NIdentifier::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "ID"
            << ": " << name << std::endl;
  spaces -= 2;
  return 0;
}
int NDotOperator::parse() {
  printGrammerInfo(getNodeName(), line);
  spaces += 2;
  exp.parse();
  printspaces();
  std::cout << "DOT" << std::endl;
  parseNIdentifier(id);
  // id.parse();
  spaces -= 2;
  return 0;
}
int NListOperator::parse() {
  printGrammerInfo(getNodeName(), line);
  spaces += 2;
  lhs.parse();
  printspaces();
  std::cout << "LB" << std::endl;
  rhs.parse();
  printspaces();
  std::cout << "RB" << std::endl;
  spaces -= 2;
  return 0;
}
int NArgs::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  exp.parse();
  if (nArgs) {
    printspaces();
    std::cout << "COMMA" << std::endl;
    nArgs->parse();
  }
  spaces -= 2;
  return 0;
}
int NMethodCall::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  parseNIdentifier(id);
  // id.parse();
  printspaces();
  std::cout << "LP" << std::endl;
  if (nargs) {
    nargs->parse();
  }  
  printspaces();
  std::cout << "RP" << std::endl;
  spaces -= 2;   
  return 0;
}
int NParenOperator::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "LP" << std::endl;
  printspaces();
  exp.parse();
  printspaces();
  std::cout << "RP" << std::endl;
  spaces -= 2;
  return 0;
}
int NSingleOperator::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << name << std::endl;
  hs.parse();
  spaces -= 2;
  return 0;
}
int NBinaryOperator::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  lhs.parse();
  printspaces();
  if (name.substr(0, 5) == "RELOP")
    std::cout << "RELOP" << std::endl;
  else
    std::cout << name << std::endl;
  rhs.parse();
  spaces -= 2;
  return 0;
}
int NAssignment::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  lhs.parse();
  printspaces();
  std::cout << name << std::endl;
  rhs.parse();
  spaces -= 2;
  return 0;
}
int NSpecifier::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "TYPE: " << type << std::endl;
  spaces -= 2;
  return 0;
}
int NVarDec::parse() {
  printGrammerInfo(getNodeName(), line);

  if (v.size()) {
    spaces += 2;
    for (int i = 0; i < v.size(); ++i) {
      printGrammerInfo(getNodeName(), line);

      spaces += 2;
    }
    parseNIdentifier(Id);
    // Id.parse();
    spaces -= 2;
    for (int i = 0; i < v.size(); ++i) {
      printspaces();
      std::cout << "LB" << std::endl;
      printspaces();
      std::cout << "INT: " << v[i] << std::endl;
      printspaces();
      std::cout << "RB" << std::endl;
      spaces -= 2;
    }
  } else {
    spaces += 2;
    parseNIdentifier(Id);
    // Id.parse();
    spaces -= 2;
  }
  return 0;
}
int NParamDec::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  nSpecifier.parse();
  varDec.parse();
  spaces -= 2;
  return 0;
}
int NVarList::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  nParamDec.parse();
  if (nVarList) {
    printspaces();
    std::cout << "COMMA" << std::endl;
    nVarList->parse();
  }
  spaces -= 2;
  return 0;
}
int NFunDec::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  parseNIdentifier(Id);
  // Id.parse();
  printspaces();
  std::cout << "LP" << std::endl;
  if (arguments)
    arguments->parse();
  printspaces();
  std::cout << "RP" << std::endl;
  spaces -= 2;
  return 0;
}
int NDec::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  vardec.parse();
  if (exp) {
    printspaces();
    std::cout << "ASSIGNOP" << std::endl;
    exp->parse();
  }
  spaces -= 2;
  return 0;
}
int NDecList::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  dec.parse();
  if (nDecList) {
    printspaces();
    std::cout << "COMMA" << std::endl;
    nDecList->parse();
  }
  spaces -= 2;
  return 0;
}
int NDef::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  nSpecifier.parse();
  if (nDecList)
    nDecList->parse();
  printspaces();
  std::cout << "SEMI" << std::endl;
  spaces -= 2;
  return 0;
}
int NDefList::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  nDef.parse();
  if (nDefList) {
    nDefList->parse();
  }
  spaces -= 2;
  return 0;
}
int NStructSpecifier::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printGrammerInfo("StructSpecifier", line);

  spaces += 2;
  printspaces();
  std::cout << "STRUCT" << std::endl;
  if (deflist) {
    if (tag) {
      printGrammerInfo("OptTag", line);
      spaces += 2;
      parseNIdentifier(*tag);
      spaces -= 2;
      printspaces();
      std::cout << "LC" << std::endl;
      deflist->parse();
      printspaces();
      std::cout << "RC" << std::endl;
    } else {
      deflist->parse();
    }
  } else if (tag) {
    printGrammerInfo("Tag", line);

    spaces += 2;
    parseNIdentifier(*tag);
    spaces -= 2;
  }
  spaces -= 2;
  spaces -= 2;
  return 0;
}
int NStmtList::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  nStmt.parse();
  if (nStmtList)
    nStmtList->parse();
  spaces -= 2;
  return 0;
}

int NCompSt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "LC" << std::endl;
  if (ndeflist)
    ndeflist->parse();
  if (nstmtlist)
    nstmtlist->parse();
  printspaces();
  std::cout << "RC" << std::endl;
  spaces -= 2;
  return 0;
}
int NExpStmt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  this->exp.parse();
  printspaces();
  std::cout << "SEMI" << std::endl;
  spaces -= 2;
  return 0;
}
int NCompStStmt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  compst.parse();
  spaces -= 2;
  return 0;
}
int NRetutnStmt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "RETURN" << std::endl;
  this->exp.parse();
  printspaces();
  std::cout << "SEMI" << std::endl;
  spaces -= 2;
  return 0;
}
int NIfStmt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "IF" << std::endl;
  printspaces();
  std::cout << "LP" << std::endl;
  this->exp.parse();
  printspaces();
  std::cout << "RP" << std::endl;
  this->stmt.parse();
  spaces -= 2;
  return 0;
}
int NIfElseStmt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "IF" << std::endl;
  printspaces();
  std::cout << "LP" << std::endl;
  this->exp.parse();
  printspaces();
  std::cout << "RP" << std::endl;
  this->stmt.parse();
  printspaces();
  std::cout << "ELSE" << std::endl;
  this->stmt_else.parse();
  spaces -= 2;
  return 0;
}
int NWhileStmt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "WHILE" << std::endl;
  printspaces();
  std::cout << "LP" << std::endl;
  this->exp.parse();
  printspaces();
  std::cout << "RP" << std::endl;
  this->stmt.parse();
  spaces -= 2;
  return 0;
}
int NBreakStmt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "BREAK" << std::endl;
  printspaces();
  std::cout << "SEMI" << std::endl;
  spaces -= 2;
  return 0;
}
int NExtDecList::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  nVarDec.parse();
  if (nExtDecList) {
    printspaces();
    std::cout << "COMMA" << std::endl;
    nExtDecList->parse();
  }
  spaces -= 2;
  return 0;
}
int NExtDef::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  specifier.parse();
  if (fundec) {
    fundec->parse();
    if (compst) {
      compst->parse();
    }
  } else {
    if (nextdeclist) {
      nextdeclist->parse();
    }
    printspaces();
    std::cout << "SEMI" << std::endl;
  }

  spaces -= 2;
  return 0;
}
int NExtDefList::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  nExtDef.parse();
  if (nExtDefList)
    nExtDefList->parse();
  spaces -= 2;
  return 0;
}
int NProgram::parse() {
  printGrammerInfo("Program", line);
  spaces += 2;
  if (nextdeflist)
    nextdeflist->parse();
  spaces -= 2;
  return 0;
}

// codegen()
Value *Node::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *Node::codegen"<<std::endl;
#endif

  assert(false); // Never use this function.
  // This is a list.
  return ConstantInt::get(*theContext, APInt(32, 0, true));
}
Value *NExpression::codegen() {
        
#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NExpression::codegen"<<std::endl;
#endif

  return ConstantInt::get(*theContext, APInt(32, 0, true));
}
Value *NInteger::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NInteger::codegen"<<std::endl;
#endif

  return ConstantInt::get(*theContext, APInt(32, value, true));
}    
Value *NFloat::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NFloat::codegen"<<std::endl;
#endif

  // begin
  return ConstantFP::get(*theContext,APFloat(value));
  // end
}
Value *NChar::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NChar::codegen"<<std::endl;
#endif

  // begin
  return ConstantInt::get(*theContext,APInt(8,value,false));
  // end
}
Value *NIdentifier::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NIdentifier::codegen"<<std::endl;
#endif

  // begin
  AllocaInst *var = namedValues[name];
  if(var) return builder->CreateLoad(var->getAllocatedType(), var);
  printSemanticError(1, line, "Undeclared variable " + name);
  return nullptr;
  // end
}
Value *NArgs::codegen() { 
            
#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NArgs::codegen"<<std::endl;
#endif

  return exp.codegen(); 
}
Value *NMethodCall::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NMethodCall::codegen"<<std::endl;
#endif

  // begin
  Function *func = theModule->getFunction(id.name);
  if(!func) 
    printSemanticError(2, line, "Undeclared function " + id.name);

  std::vector<Value *> argV;
  NArgs *arg = nargs;
  for(auto &argI : func->args()) {
    if(!arg) 
      printSemanticError(8, line, "Too few arguments in function " + id.name);
    Value *argVal = arg->codegen();
    if(argVal->getType() != argI.getType()) 
      printSemanticError(8, line, "Type mismatched for argument " + argI.getName().str() + " in function " + id.name);
    argV.push_back(argVal);
    arg = arg->nArgs;
  }
  if(arg) 
    printSemanticError(8, line, "Too many arguments in function " + id.name);

  return builder->CreateCall(func, argV);
  // end
}
Value *NParenOperator::codegen() {  

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NParenOperator::codegen"<<std::endl;
#endif

  return exp.codegen(); 
}
Value *NSingleOperator::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NSingleOperator::codegen"<<std::endl;
#endif

  // begin

  return nullptr;
  // end
}
Value *NBinaryOperator::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NBinaryOperator::codegen"<<std::endl;
#endif

  // begin
  Value * left = lhs.codegen();
  Value * right = rhs.codegen();
  if(name == "AND")
    return builder->CreateAnd(left,right);
  if(name == "OR")
    return builder->CreateOr(left,right);
  if(name == "PLUS")
    return builder->CreateAdd(left,right);
  if(name == "MINUS")
    return builder->CreateSub(left,right);
  if(name == "STAR")
    return builder->CreateMul(left,right);
  if(name == "DIV")
    return builder->CreateSDiv(left,right);
  if(name == "MOD")
    return builder->CreateSRem(left,right);
  if(name == "RELOP<=")
    return builder->CreateICmpSLE(left,right);
  if(name == "RELOP>=")
    return builder->CreateICmpSGE(left,right);
  if(name == "RELOP<")
    return builder->CreateICmpSLT(left,right);
  if(name == "RELOP>")
    return builder->CreateICmpSGT(left,right);
  if(name == "RELOP==")
    return builder->CreateICmpEQ(left,right);
  if(name == "RELOP!=")
    return builder->CreateICmpNE(left,right);
  
  return nullptr;
  // end
}
Value *NAssignment::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NAssignment::codegen"<<std::endl;
#endif

  // Assignment requires the LHS to be an identifier.
  // begin
  Value * rightVal = rhs.codegen();
  if(lhs.name == "")
    printSemanticError(6, line, "The left-hand side of an assignment must be a variable");
  AllocaInst *leftVar = namedValues[lhs.name];
  if(!leftVar)
    printSemanticError(1, line, "Undeclared variable " + lhs.name);
  if(leftVar->getAllocatedType() != rightVal->getType())
    printSemanticError(5, line, "Type mismatch in assignment");
  if(name == "ASSIGNOP")
    return builder->CreateStore(rightVal,leftVar);
  if(name == "PLUSASS")
    return builder->CreateStore(builder->CreateAdd(leftVar,rightVal),leftVar);
  if(name == "MINUSASS")
    return builder->CreateStore(builder->CreateSub(leftVar,rightVal),leftVar);
  if(name == "STARASS")
    return builder->CreateStore(builder->CreateMul(leftVar,rightVal),leftVar);
  if(name == "DIVASS")
    return builder->CreateStore(builder->CreateSDiv(leftVar,rightVal),leftVar);

  return nullptr;
  // end
}
Value *NSpecifier::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NSpecifier::codegen"<<std::endl;
#endif

  // begin

  return nullptr;
  // end
}
Type *NSpecifier::getType() {
  if (type == "int")
    return Type::getInt32Ty(*theContext);
  if (type == "float")
    return Type::getFloatTy(*theContext);
  if (type == "char")
    return Type::getInt8Ty(*theContext);
  assert(false);
  return Type::getInt32Ty(*theContext);
}
Value *NVarDec::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NVarDec::codegen"<<std::endl;
#endif

  // begin

  return nullptr;
  // end
}
//函数形参
Value *NParamDec::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NParamDec::codegen"<<std::endl;
#endif

  // begin

  return nullptr;
  // end
}

std::pair<std::string, Type *> NParamDec::getType() {
  assert(varDec.v.size() == 0);
  std::pair<std::string, Type *> tmp(varDec.Id.name, nSpecifier.getType());
  return tmp;
}
//函数形参组
Value *NVarList::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NVarList::codegen"<<std::endl;
#endif

  assert(false); // Never use this function.
  // This is a list.
  return ConstantInt::get(*theContext, APInt(32, 0, true));
}
Function *NFunDec::funcodegen(Type *retType) {
    
#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Function *NFunDec::funcodegen"<<std::endl;
#endif

  // check if it exists the same name of fun
  if (theModule->getFunction(Id.name)) {
    printSemanticError(4, line, "Redefined " + Id.name);
    return nullptr;
  }

  // change args to vector(LLVM type)
  std::vector<Type *> argsTypes;
  std::vector<std::string> argNames;
  for (NVarList *item = arguments; item; item = item->nVarList) {
    auto tmp = item->nParamDec.getType();
    argNames.push_back(tmp.first);
    argsTypes.push_back(tmp.second);
  }
  // create function
  // define $retType @$Id.name($argsTypes)
  FunctionType *ft = FunctionType::get(retType, argsTypes, false);
  Function *f =
      Function::Create(ft, Function::ExternalLinkage, Id.name, theModule.get());
  // set name of args
  unsigned idx = 0;
  for (auto &arg : f->args()) {
    arg.setName(argNames[idx++]);
  }
  return f; 
}
Value *NDef::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NDef::codegen"<<std::endl;
#endif

  // begin
  Type *varType = nSpecifier.getType();
  for (NDecList *item = nDecList; item; item = item->nDecList) {
    std::string &varName = item->dec.vardec.Id.name;
    // check if it exists the same name of var
    if(curNamedValues.find(varName)!=curNamedValues.end())
      printSemanticError(3, line, "Redefined " + varName);

    // create var
    AllocaInst *allocaVar = builder->CreateAlloca(varType, nullptr, varName);
    if(item->dec.exp != nullptr){
      Value *initVal = item->dec.exp->codegen();
      builder->CreateStore(initVal, allocaVar);
    }
    // add var to namedValues
    namedValues[varName] = allocaVar;
    curNamedValues[varName] = allocaVar;
  }    
  return nullptr;  
  // end
}
Value *NDefList::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NDefList::codegen"<<std::endl;
#endif

  // begin
  nDef.codegen();
  if(nDefList != nullptr) 
     nDefList->codegen();
    
#ifdef DEBUG
  std::cout<<"[out]file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NDefList::codegen"<<std::endl;
#endif

  return nullptr;
  // end
}
Value *NStmtList::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NStmtList::codegen"<<std::endl;
#endif

  auto *retVal = nStmt.codegen();
  if (nStmtList)
    retVal = nStmtList->codegen();

#ifdef DEBUG
  std::cout<<"[out]file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NStmtList::codegen"<<std::endl;
#endif

  return retVal;
}
Value *NCompSt::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NCompSt::codegen"<<std::endl;
#endif

  // 自行处理变量作用域的问题  
  Value *retVal = nullptr;
  if (ndeflist)
    retVal = ndeflist->codegen();
  if (nstmtlist)
    retVal = nstmtlist->codegen();

#ifdef DEBUG
  std::cout<<"end of NCompSt::codegen"<<std::endl;
#endif

  // 删除函数内部的变量 (namedValues)
  for (auto &item : namedValues) 
    curNamedValues.erase(item.first);

#ifdef DEBUG
  std::cout<<"[out]file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NCompSt::codegen"<<std::endl;
#endif

  return retVal;
}
Value *NExpStmt::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NExpStmt::codegen"<<std::endl;
#endif

 return exp.codegen(); 
}
Value *NCompStStmt::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NCompStStmt::codegen"<<std::endl;
#endif

  // begin

  return nullptr;
  // end
}
Value *NRetutnStmt::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NRetutnStmt::codegen"<<std::endl;
#endif

  Function *theFun = builder->GetInsertBlock()->getParent();
  BasicBlock *bb = BasicBlock::Create(*theContext, "ret", theFun);
  builder->CreateBr(bb);
  builder->SetInsertPoint(bb);
  auto *retVal = exp.codegen();
  // check the return type and fundec type
  // begin
  if(retVal->getType() != theFun->getReturnType())
    printSemanticError(7, line, "Type mismatched for return");

  // end
  builder->CreateRet(retVal);
  return retVal;
}
Value *NIfStmt::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NIfStmt::codegen"<<std::endl;
#endif

  Function *theFun = builder->GetInsertBlock()->getParent();
  // begin

  return nullptr;
  // end
}
Value *NIfElseStmt::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NIfElseStmt::codegen"<<std::endl;
#endif

  Function *theFun = builder->GetInsertBlock()->getParent();
  // begin

  return nullptr;
  // end
}
Value *NWhileStmt::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NWhileStmt::codegen"<<std::endl;
#endif

  Function *theFun = builder->GetInsertBlock()->getParent();
  BasicBlock *condb = BasicBlock::Create(*theContext, "cond", theFun);
  // begin

  return nullptr;
  // end
}
Value *NBreakStmt::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NBreakStmt::codegen"<<std::endl;
#endif

  // begin

  return nullptr;
  // end
}



//全局变量
Value *NExtDefVarDec::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NExtDefVarDec::codegen"<<std::endl;
#endif

  // begin

  return nullptr;
  // end
}


//函数定义
Value *NExtDefFunDec::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NExtDefFunDec::codegen"<<std::endl;
#endif

  Type *retType = specifier.getType();
  
  Function *f = fundec->funcodegen(retType);
  if (!f) {
    return nullptr;
  }
  assert(compst != nullptr); // Assert compst is not null.
  BasicBlock *bb = BasicBlock::Create(*theContext, "entry", f);
  // entry:
  builder->SetInsertPoint(bb);
  namedValues.clear(); 
  for (auto &arg : f->args()) {
    // Create an alloca for this variable.
    AllocaInst *alloca =
        CreateEntryBlockAlloca(f, arg.getName(), arg.getType());

    if (curNamedValues[std::string(arg.getName())]) {
      printSemanticError(3, line, "Redefined " + arg.getName().str());
      return LogErrorV("Unknown function referenced");
    }
    // Store the initial value into the alloca.
    builder->CreateStore(&arg, alloca);
    // Add arguments to variable symbol table.
    namedValues[std::string(arg.getName())] = alloca;
    curNamedValues[std::string(arg.getName())] = alloca;
  }
  if (Value *retVal = compst->codegen()) {
    // Finish off the function.

    // Validate the generated code, checking for consistency.
    verifyFunction(*f);

    // Run the optimizer on the function.
    // theFPM->run(*f);
    return f;
  }
  // Error reading body, remove function.
  f->eraseFromParent();

  return nullptr;
}
Value *NExtDefList::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NExtDefList::codegen"<<std::endl;
#endif

  auto *lastCode = nExtDef.codegen();
  // lastCode->print(errs());
  // assert(nExtDefList == nullptr);
  if (nExtDefList)
    lastCode = nExtDefList->codegen();
  return lastCode;
}
Value *NProgram::codegen() {

#ifdef DEBUG
  std::cout<<"file: "<<__FILE__<<":"<<__LINE__<<" fuc: "<<"Value *NProgram::codegen"<<std::endl;
#endif


  //默认输出函数putchar
  std::vector<Type *> putArgs;
  putArgs.push_back(Type::getInt32Ty(*theContext));

  FunctionType *putType =
      FunctionType::get(builder->getInt32Ty(), putArgs, false);
  Function *putFunc = Function::Create(putType, Function::ExternalLinkage,
                                       "putchar", theModule.get());

  //默认输入函数getchar
  std::vector<Type *> getArgs;
  // getArgs.push_back(Type::getInt32Ty(*theContext));

  FunctionType *getType =
      FunctionType::get(builder->getInt32Ty(), getArgs, false);
  Function *getFunc = Function::Create(getType, Function::ExternalLinkage,
                                       "getchar", theModule.get());

  Value *lastCode = nextdeflist->codegen();
  if (grammererror)
    return nullptr;
  return lastCode;
}

