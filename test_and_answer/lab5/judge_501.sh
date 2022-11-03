cd /home/cse/miniC/lab5/task501
make clean > /dev/null 2>&1 					#make clean（清除上一次的评测程序）
touch tokens.txt										#新建文本，准备存放程序输出结果
touch error.txt	

clang++ -c countPass.cpp `llvm-config --cxxflags`
clang++ -shared -o countPass.so countPass.o `llvm-config --ldflags`
opt -load ./countPass.so -CountPass test.txt -o error.txt >tmp1
diff -b tmp1 /home/ubuntu/miniC/answers/lab5/task501/answer0.txt
