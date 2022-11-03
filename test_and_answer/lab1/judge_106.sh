#!/bin/bash
cd /home/$(whoami)/miniC/lab1/task106
make clean >/dev/null 2>&1  						#make clean（清除上一次的评测程序）
touch result.txt    									#新建文本，准备存放程序输出结果
touch error.txt											#新建文本，准备存放错误信息
make > error.txt 2>&1						#用gcc编译生成可执行文件
./scanner <case$1.in > result.txt  	#执行最终生成的程序
#cat result.txt            								#显示程序运行结果
diff -b result.txt /home/$(whoami)/miniC/answers/lab1/task106/answer$1.txt
