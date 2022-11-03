cd /home/cse/miniC/lab4/task402
touch tokens.txt										#新建文本，准备存放程序输出结果
touch error.txt											#新建文本，准备存放错误信息										#读取键盘输入到typ中

if [ $1 == '0' ] ; then
make TARGET=task1 > error.txt 2>&1
touch tmp
./task1 > tmp
lli tmp > tokens.txt
#cat tokens.txt
diff -b tokens.txt /home/ubuntu/miniC/answers/lab4/task402/answer$1.txt	
fi

if [ $1 == '1' ] ; then
make TARGET=task2 > error.txt 2>&1
touch tmp
./task2 > tmp
echo 'a' | lli tmp > tokens.txt
#cat tokens.txt
diff -b tokens.txt /home/ubuntu/miniC/answers/lab4/task402/answer$1.txt
fi

if [ $1 == '2' ] ; then
make TARGET=task2 > error.txt 2>&1
touch tmp
./task2 > tmp
echo 'T' | lli tmp > tokens.txt
#cat tokens.txt
diff -b tokens.txt /home/ubuntu/miniC/answers/lab4/task402/answer$1.txt
fi