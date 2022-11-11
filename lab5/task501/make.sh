clang++ -c countPass.cpp `llvm-config --cxxflags`
clang++ -shared -o countPass.so countPass.o `llvm-config --ldflags`
opt -load ./countPass.so -CountPass test.ll