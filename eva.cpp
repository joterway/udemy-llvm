#ifndef EvaLLVM_h
#define EvaLLVM_h

#include <string>
#include <iostream>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"

class EvaLLVM {
  public:
    EvaLLVM() {
      moduleInit();

    }

    void exec(const std::string& program) {
      // 1. parse program
      // auto ast = parser->parser(program);

      // 2. compile to LLVM IR
      compile();

      /*
      builder->CreateGlobalString("hello world!\n");
      */

      /*
      auto entry = llvm::BasicBlock::Create(*ctx, fnName, fn);
      builder->SetInsertPoint(entry);
      */

      // 2b. Print generated code
      mod->print(llvm::outs(), nullptr);
      std::cout << "\n";

      // 3. save module IR to file
      saveModuleToFile("out.ll");
    }

  private:
    void compile(/* ast */) {
      // 1. Create main function
      fn = createFunction(
        "main",
        llvm::FunctionType::get(/* return */ builder->getInt32Ty(),
                                /* vararg */ false));

      // 2. Compile main body
      auto result = gen(/* ast */);

      auto i32Result =
        builder->CreatePtrToInt(result, builder->getInt32Ty());
      builder->CreateRet(i32Result);
    }

    llvm::Value* gen(/* exp */) {
      // return builder-getInt32(42);
      return builder->CreateGlobalString("Hello world\n");
    }

    llvm::Function* createFunction(const std::string& fnName,
                                 llvm::FunctionType* fnType) {
      auto fn = mod->getFunction(fnName);
      if (fn == nullptr) {
        fn = createFunctionProto(fnName, fnType);
      }

      createFunctionBlock(fn);
      return fn;
    }

    llvm::Function* createFunctionProto(const std::string& fnName,
                                        llvm::FunctionType* fnType) {
      auto fn = llvm::Function::Create(
        fnType, llvm::Function::ExternalLinkage, fnName, *mod);
      verifyFunction(*fn);

      return fn;
    }

    void createFunctionBlock(llvm::Function* fn) {
      auto entry = createBB("entry", fn);
      builder->SetInsertPoint(entry);
    }

    /**
      * Creates a basic block. If the fn is passed, the block is automatically
      * appended to the parent function. Otherwise, the block should later be
      * appended manually via fn->getBasicBlockList().push_back(block);
      */
    llvm::BasicBlock* createBB(
      std::string name, llvm::Function* fn = nullptr) {
      return llvm::BasicBlock::Create(*ctx, name, fn);
    }

    /**
      * Saves IR to file
      */
    void saveModuleToFile(const std::string name) {
      std::error_code errorCode;
      llvm::raw_fd_ostream outLL(name, errorCode);
      mod->print(outLL, nullptr);
    }

    void moduleInit() {
      ctx = std::make_unique<llvm::LLVMContext>();
      mod = std::make_unique<llvm::Module>("EvaLLVM", *ctx);
      builder = std::make_unique<llvm::IRBuilder<>>(*ctx);

      // Setup extern functions
      mod->getOrInsertFunction(
        "printf", llvm::FunctionType::get(builder->getInt32Ty(), 
                                          llvm::PointerType::get(builder->getInt8Ty(), 0),
                                          true));
    }

    llvm::Function* fn;

    std::unique_ptr<llvm::LLVMContext> ctx;

    std::unique_ptr<llvm::Module> mod;

    std::unique_ptr<llvm::IRBuilder<>> builder;
};

int main(int argc, char const* argv[]) {
  std::string program = R"(

    42

  )";

  EvaLLVM eva;
  eva.exec("");

  return 42;
}


#endif
