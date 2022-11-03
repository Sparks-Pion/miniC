cd /home/$(whoami)/miniC/lab2
make clean > /dev/null 2>&1 					#make clean（清除上一次的评测程序）
touch tokens.txt										#新建文本，准备存放程序输出结果
touch error.txt											#新建文本，准备存放错误信息										#读取键盘输入到typ中
make > error.txt 2>&1                                   #根据Makefile 生成minic可执行程序
if [ ! -f "token.cpp" ];then								#检查是否成功生成
echo "所需c文件:token.cpp 不存在，编译未成功，请检查源程序的格式。"
echo "下面是错误信息："
cat error.txt
else
./minic task203case/$1.in > tokens.txt		#执行最终生成的程序
#cat tokens.txt
diff -b tokens.txt /home/$(whoami)/miniC/answers/lab2/task203case/answer$1.txt	
fi 