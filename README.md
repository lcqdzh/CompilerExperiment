# CompilerExperiment
编译原理实验  
本实验为编译原理课程实验 包含词法分析和语法分析  
## 实验一 词法分析
读取文件方式获取字符串，进行词法分析后，在屏幕上打印token，并以文件形式保存。 

## 实验二 LR语法分析
分析下面语句
```
int a;
int b;
int c;
a = 2;
b = 1;
if （a>b）
c = a+b;
else
c = a-b;
```

文法 
```
P’-> P
P → D S
P → S
D →L id ; D
D →L id ; 
L → int
L → float
S → id = E;	     
S → if （C）  S1   
S → if （C）  S1   else   S2
S →  else   S2
S → while （C）  S1  
S → S  S
C → E1 > E2
C → E1 < E2                  
C → E1 = = E2                 
E → E1 + T           
E → E1 – T 	
E → T 				
T → F 				
T → T1 * F 			
T → T1 / F 			
F → ( E )			  
F → id 
F → int10
```
## 使用说明 
程序使用codeblocks17.12编写，可用其打开项目。两次实验的项目文件分别为shiyan1，shiyan2。
如果打不开，可以打开项目文件夹中的main.cpp，用c++编译器编译运行即可。

可执行文件在项目目录bin/Debug文件夹中。可直接运行。
使用前请将要分析的内容存放在fenxi.txt中，与可执行文件存放在同一路径下。
词法分析结果存放在tokens.txt中。
文法分析结果存放在yf.txt中。

默认选择读取文件更加方便。

输入示例：
可在两个实验可执行文件同一目录下把下面内容存放在fenxi.txt文件中，使用时选取1读文件，之后确认y即可。
```
int a;
int b;
int c;
a=2;
b=1;
if (a>b)
c=a+b;
else
c=a-b;
```
也可以选取2屏幕输入，之后将上述内容逐行输入，并在结束时另起一行输入#end结束。
输入是括号分号等请用英文输入状态下输入。


种别码对应单词情况： 
```
                    表1 种别码
单词	种别码	单词	种别码	单词	种别码
num	1	=	9	!=	17
id	2	(	10	if	18
+	3	)	11	then	19
-	4	;	12	else	20
*	5	‘	13	wile	21
/	6	==	14	do	22
>	7	>=	15	error	23
<	8	<=	16	float	24
				int	25
```
