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
