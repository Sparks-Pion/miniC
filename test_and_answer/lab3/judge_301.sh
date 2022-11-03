cd /home/$(whoami)/miniC/lab3/task301
if [ -f "foo.output" ];then	
rm foo.output > /dev/null 2>&1 		#（清除上一次的评测程序）
fi
if [ -f "result.txt" ];then	
rm result.txt > /dev/null 2>&1 		#（清除上一次的评测程序）
fi
if [ -f "error.txt" ];then	
rm error.txt > /dev/null 2>&1 		#（清除上一次的评测程序）
fi
			
touch result.txt										#新建文本，准备存放程序输出结果
touch error.txt											#新建文本，准备存放错误信息										#读取键盘输入到typ中
bison -r solved -v foo.y > error.txt 2>&1
#flex task2.l > error.txt 2>&1							#用flex编译源文件，保存错误信息
if [ ! -f "foo.output" ];then								#检查是否成功生成
echo "所需文件:foo.output 不存在，编译未成功，请检查源程序的格式。"
echo "下面是错误信息："
cat error.txt
else
 grep -E "useless|unused|resolved" foo.output >result.txt							#生成最终的可执行程序文件
#cat result.txt											#显示程序运行结果
diff -b result.txt /home/$(whoami)/miniC/answers/lab3/task301/answer.txt	
fi