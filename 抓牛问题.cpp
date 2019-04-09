#include<stdio.h>
#include<string.h>
#include<iostream>
#include<algorithm>
using namespace std;

int N,K;
int dfs(int now,int type,int depth){
    if(now==K){
        return depth;
    }
    int res=0;
    depth++;
    if(type==0){
       res=min(dfs(N--,0,depth),min(dfs(N--,1,depth),dfs(N--,2,depth))); 
    }
    if(type==1){
       res=min(res,min(dfs(N++,0,depth),min(dfs(N++,1,depth),dfs(N++,2,depth))));
    }
    if(type==2){
       res=min(res,min(dfs(N*2,0,depth),min(dfs(N*2,1,depth),dfs(N*2,2,depth))));
    }
    return res;
}
int main(){
    cin>>N>>K;
    if(K<=N){
        cout<<N-K;
        return 0;
    }
    cout<<min(dfs(N,0,0),min(dfs(N,1,0),dfs(N,2,0)))<<endl;
    return 0;
}
