#include <iostream>

#include <string>
#include <vector>

#include <algorithm>
#include <assert.h>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

#include <fstream>

using namespace llvm;

std::unique_ptr<LLVMContext> theContext;
std::unique_ptr<Module> theModule;
std::unique_ptr<IRBuilder<>> builder;
std::map<std::string, AllocaInst *> namedValues;
std::unique_ptr<legacy::FunctionPassManager> theFPM;

std::map<std::string, AllocaInst *> curNamedValues;

BasicBlock *continueBasicBlock = nullptr;

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

int main(int argc, char *argv[]) {
  // Init
  InitializeModuleAndPassManager();

  //默认输出函数putchar
  std::vector<Type *> putArgs;
  putArgs.push_back(Type::getInt32Ty(*theContext));

  FunctionType *putType =
      FunctionType::get(builder->getInt32Ty(), putArgs, false);
  Function *putFunc = Function::Create(putType, Function::ExternalLinkage,
                                       "putchar", theModule.get());
  //默认输入函数getchar
  std::vector<Type *> getArgs;

  FunctionType *getType =
      FunctionType::get(builder->getInt32Ty(), getArgs, false);
  Function *getFunc = Function::Create(getType, Function::ExternalLinkage,
                                       "getchar", theModule.get());
  //根据输入的单字符，判断，如果是'a'，则输出'Y'，否则输出'N'。
  //设置返回类型
  //begin
  Type *retType = Type::getInt32Ty(*theContext);
  FunctionType *mainType = FunctionType::get(retType, false);
  Function *mainFunc = Function::Create(mainType, Function::ExternalLinkage, 
                                        "main", theModule.get());

  BasicBlock *bb = BasicBlock::Create(*theContext, "entry", mainFunc);
  BasicBlock *thenBB = BasicBlock::Create(*theContext, "then", mainFunc);
  BasicBlock *elseBB = BasicBlock::Create(*theContext, "else", mainFunc);
  BasicBlock *returnBB = BasicBlock::Create(*theContext, "return", mainFunc);
  // entry:
  builder->SetInsertPoint(bb);
  // %a_get = call i32 @getchar()
  Value *a_get = builder->CreateCall(getFunc,std::vector<Value *>{}, "a_get");
  // %a_eq = icmp eq i32 %a_get, 97('a')
  Value *a_eq = builder->CreateICmpEQ(a_get, builder->getInt32('a'), "a_eq");
  // br %a_eq, label %then, label %else
  builder->CreateCondBr(a_eq, thenBB, elseBB);
  // then:
  builder->SetInsertPoint(thenBB);
  // call i32 @putchar(i32 89('Y'))
  builder->CreateCall(putFunc, {builder->getInt32('Y')});
  // br label %return
  builder->CreateBr(returnBB);
  // else:
  builder->SetInsertPoint(elseBB);
  // call i32 @putchar(i32 78('N'))
  builder->CreateCall(putFunc, {builder->getInt32('N')});
  // br label %return
  builder->CreateBr(returnBB);
  // return :
  builder->SetInsertPoint(returnBB);
  // end
  //设置返回值
  builder->CreateRet(builder->getInt32(0));
  verifyFunction(*mainFunc);
  // Run the optimizer on the function.
  // theFPM->run(*f);
  //输出
  theModule->print(outs(), nullptr);

  return 0;
}