#!/bin/bash
cd /home/$(whoami)/miniC/lab1/task104
make clean >/dev/null 2>&1  						#make clean（清除上一次的评测程序）
touch result.txt    									#新建文本，准备存放程序输出结果
touch error.txt											#新建文本，准备存放错误信息
flex lab104.l	> error.txt 2>&1						#用flex编译源文件，保存错误信息
if [ ! -f "lex.yy.c" ];then								#检查是否成功生成
echo "所需c文件:lex.yy.c 不存在!编译未成功，请检查源程序的格式。"
echo "下面是错误信息："
cat error.txt
else
make > error.txt 2>&1						#用gcc编译生成可执行文件
./scanner <case$1.in > result.txt  	#执行最终生成的程序
diff -b result.txt /home/$(whoami)/miniC/answers/lab1/task104/answer$1.txt
fi  