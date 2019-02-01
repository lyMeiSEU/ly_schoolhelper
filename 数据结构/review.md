# chapter01 BasicConcepts
* 排序方法
	* Insertion Sort 
	* Bubble Sort 
    * Selection Sort 
    * Counting Sort
    * Shell Sort
    * Heap Sort
    * Merge Sort
    * Quick Sort
## Insert Sort
* Start with a sequence of size 1
* Repeatedly insert remaining elements
``` cpp
// insert
for(int i = 1; i < a.length; i++) 
{
	// insert a[i] into a[0:i-1] 
	int t = a[i]; 
	int j; 
	for (j = i - 1; j >= 0 && t < a[j]; j--) 
	{
		a[j + 1] = a[j];
 	}
	a[j + 1] = t; 
}
```
##  System Life Cycle
Good programmers regard large-scare computer programs a system.
1. Requirements
2. Analysis
3. Design
4. Refinement and coding
   
## Recursive
* direct recursion
* indirect recursion

## Complexity
* Asymptotic Notation(渐进符号) $(O)$ 上限（最坏复杂度）
* Asymptotic Notation $(\Omega)$ 下限（最忧复杂度）
* Asymptotic Notation $(\Theta)$  如果$O$和$\Omega$可以用同一个多项式表示，存在



* space complexity
* time complexity
>时间复杂度：\
一般情况下，算法中基本操作重复执行的次数是问题规模n的某个函数f(n)，进而分析f(n)随n的变化情况并确定T(n)的数量级。这里用"O"来表示数量级，给出算法的时间复杂度。$T(n)=O(f(n))$它表示随着问题规模的n的增大，算法的执行时间的增长率和f(n)的增长率相同，这称作算法的渐进时间复杂度，简称时间复杂度。而我们一般讨论的是最坏时间复杂度，这样做的原因是：最坏情况下的时间复杂度是算法在任何输入实例上运行时间的上界，分析最坏的情况以估算算法指向时间的一个上界。\
时间复杂度的分析方法\
1、时间复杂度就是函数中基本操作所执行的次数\
2、一般默认的是最坏时间复杂度，即分析最坏情况下所能执行的次数\
3、忽略掉常数项\
4、关注运行时间的增长趋势，关注函数式中增长最快的表达式，忽略系数\
5、计算时间复杂度是估算随着n的增长函数执行次数的增长趋势\
6、递归算法的时间复杂度为：递归总次数 * 每次递归中基本操作所执行的次数\
常用的时间复杂度有以下七种，算法时间复杂度依次增加：$O(1)$常数型constant、$O(log_2n)$对数型logarithmic、$O(n)$线性型linear、$O(nlog_2n)$二维型log-linear、$O(n^2)$平方型quadratic、$O(n^3)$立方型、O(2^n)指数型、polynomial: $O(n^k)$ (k is a constant)、exponential: $O(c^n)$ (c is a constant > 1)\
: O(1) logarithmic: O(log n) : O(n) log-linear: O(n log n) quadratic: O(n2) 
空间复杂度：
算法的空间复杂度并不是计算实际占用的空间，而是计算整个算法的辅助空间单元的个数，与问题的规模没有关系。算法的空间复杂度S(n)定义为该算法所耗费空间的数量级。
$S(n)=O(f(n))$若算法执行时所需要的辅助空间相对于输入数据量n而言是一个常数，则称这个算法的辅助空间为$O(1)$; 
递归算法的空间复杂度：递归深度N*每次递归所要的辅助空间， 如果每次递归所需的辅助空间是常数，则递归的空间复杂度是$O(N)$.
