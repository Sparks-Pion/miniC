cd /home/cse/miniC/lab2
make clean > /dev/null 2>&1 					#make clean（清除上一次的评测程序）
touch tokens.txt										#新建文本，准备存放程序输出结果
touch error.txt											#新建文本，准备存放错误信息										#读取键盘输入到typ中
make > error.txt 2>&1
#flex task2.l > error.txt 2>&1							#用flex编译源文件，保存错误信息
if [ ! -f "token.cpp" ];then								#检查是否成功生成
echo "所需c文件:token.cpp 不存在，编译未成功，请检查源程序的格式。"
echo "下面是错误信息："
cat error.txt
else
#gcc -o task2 lex.yy.c									#生成最终的可执行程序文件
./minic task202case/$1.in > tokens.txt		#执行最终生成的程序
#cat tokens.txt
diff -b tokens.txt /home/ubuntu/miniC/answers/lab2/task202case/answer$1.txt		
fi 