#include<iostream>
using namespace std;

// p 进程数，r 资源种类
#define p 4
#define r 3


/*-----------------------------------------------*/  
/*输入函数*/  
/*-----------------------------------------------*/
//a-max,b-allocation,c-need,d-available
void input(int a[p][r],int b[p][r],int c[p][r],int d[r])
{
	int i,j;
	cout<<"* input max data:"<<endl;
	for(i=0;i<p;i++)
		for(j=0;j<r;j++)cin>>a[i][j];
	cout<<"* input allocation data:"<<endl;
	for(i=0;i<p;i++)
		for(j=0;j<r;j++)cin>>b[i][j];
	cout<<"* input need data:"<<endl;
	for(i=0;i<p;i++)
		for(j=0;j<r;j++)cin>>c[i][j];
    cout<<"* input available data:"<<endl;
	for(j=0;j<r;j++)cin>>d[j];
}

/*-----------------------------------------------*/  
/*比較函数*/  
/*-----------------------------------------------*/
//比較结果为m中的元素全大于n中的元素返回1，否则返回0
int com(int m[r],int n[r])
{
	int i,flag=0;
	for(i=0;i<r;i++)
		if(m[i]<n[i])
		{
			flag=1;
		    break;
		}
	if(flag==1)	return(0);
	else return(1);
}


/*-----------------------------------------------*/  
/*安全性检验函数*/  
/*-----------------------------------------------*/
//b、c、d意义同上
int stest(int b[p][r],int c[p][r],int d[r])
{
	int i,j,k,l,flag=0,flag1=0;
	int t[r],finish[p],dd[r];
	for(i=0;i<p;i++)finish[i]=0;//finish为1即表示available满足某一进程并让事实上现

	for(i=0;i<r;i++)dd[i]=d[i];
	cout<<"分配序列："<<endl;
	for(k=0;k<p;k++)            //全搜索。直至实现或不可能实现
	{
		for(i=0;i<p;i++)
		{
			if(finish[i]==1)continue;
			else
			{
				for(j=0;j<r;j++)t[j]=c[i][j];
				if(com(dd,t))
				{
					finish[i]=1;
					cout<<i+1<<'\t';
				    flag=1;
					for(l=0;l<r;l++)dd[l]=dd[l]+b[i][l];
					break;
				}
			}
			if(flag==1)break;
		}	
	}
	cout<<'\n';
	for(l=0;l<p;l++)
	{
		//cout<<finish[l]<<endl;
		if(finish[l]==0)flag1=1;
	}
	    //cout<<flag1<<endl;
	if(flag1==0)return(1);    //flag1为记录finish是否有0存在的标记，当flag1=0时，安全
	else return(0);
}


/*-----------------------------------------------*/  
/*申请进程后的安全性检验函数*/  
/*-----------------------------------------------*/
//req-request,n-第n个进程申请资源
void rtest(int b[p][r],int c[p][r],int d[r],int req[r],int n)
{
	int i,j;
	int t[r];
	n=n-1;
	for(i=0;i<r;i++)t[i]=c[n][i];
	if(com(d,req)&&com(t,req))//对available，request进行比較
	{
		for(j=0;j<r;j++)
		{
			b[n][j]=b[n][j]+req[j];
			c[n][j]=c[n][j]-req[j];
			d[j]=d[j]-req[j];
		}
	    if(stest(b,c,d))cout<<"同意"<<n+1<<"个进程申请资源！"<<endl;
	    else 
		{
		cout<<"不同意"<<n+1<<"个进程申请资源！"<<endl;

		cout<<"恢复曾经状态！"<<endl;
	    for(j=0;j<r;j++)
		{
			b[n][j]=b[n][j]-req[j];
			c[n][j]=c[n][j]+req[j];
			d[j]=d[j]+req[j];
		}
		}
	}

	else cout<<"申请资源量出错;"<<endl;
}


/*-----------------------------------------------*/  
/*主函数*/  
/*-----------------------------------------------*/
int main()
{
	int j,n;                   //n-第n个资源申请
	int max[p][r],allocation[p][r],need[p][r];
	int available[r],request[r];
	input(max,allocation,need,available);

	if(stest(allocation,need,available)==1)cout<<"初始状态安全。"<<endl;
	else cout<<"初始状态不安全。"<<endl;

	cout<<" input request data:"<<endl;
	for(j=0;j<r;j++)cin>>request[j];

	cout<<"第n个进程申请资源——n的值"<<endl;
	cin>>n;

	rtest(allocation,need,available,request,n);
	return 0;
}