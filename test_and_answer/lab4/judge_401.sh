cd /home/cse/miniC/lab4/task401
touch tokens.txt										#新建文本，准备存放程序输出结果
touch error.txt											#新建文本，准备存放错误信息										#读取键盘输入到typ中

if [ $1 == '0' ] ; then
lli task1.ll > tokens.txt
#cat tokens.txt
diff -b tokens.txt /home/ubuntu/miniC/answers/lab4/task401/answer$1.txt	
fi

if [ $1 == '1' ] ; then
echo 'a' | lli task2.ll > tokens.txt
#cat tokens.txt
diff -b tokens.txt /home/ubuntu/miniC/answers/lab4/task401/answer$1.txt	
fi

if [ $1 == '2' ] ; then
echo 'T' | lli task2.ll > tokens.txt
#cat tokens.txt
diff -b tokens.txt /home/ubuntu/miniC/answers/lab4/task401/answer$1.txt	
fi