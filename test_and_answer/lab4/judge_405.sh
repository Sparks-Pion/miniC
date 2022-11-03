cd /home/$(whoami)/miniC/lab4/task405
make clean > /dev/null 2>&1 					#make clean（清除上一次的评测程序）
touch tokens.txt										#新建文本，准备存放程序输出结果
touch error.txt											#新建文本，准备存放错误信息										#读取键盘输入到typ中
make > error.txt 2>&1
#flex task2.l > error.txt 2>&1							#用flex编译源文件，保存错误信息
if [ ! -f "minic" ];then								#检查是否成功生成
echo "所需文件:minic 不存在，编译未成功，请检查源程序的格式。"
echo "下面是错误信息："
cat error.txt
else
#gcc -o task2 lex.yy.c									#生成最终的可执行程序文件
./minic taskcase/$1.in > tokens.txt	        	    #执行最终生成的程序
if [ $1 == '0' ] ; then
echo "ab" | lli tokens.txt > tmp
elif [ $1 == '1' ] ; then
echo 6 | lli tokens.txt > tmp
elif [ $1 == '2' ] ; then
echo "aa" | lli tokens.txt > tmp
elif [ $1 == '3' ] ; then
echo 9 | lli tokens.txt > tmp
else
cat tokens.txt > tmp										#显示程序运行结果
fi
diff -b tmp /home/$(whoami)/miniC/answers/lab4/task405/answer$1.txt
fi