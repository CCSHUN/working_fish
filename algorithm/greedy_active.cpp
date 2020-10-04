#include<cstdio>  
#include<iostream>   
#include<algorithm>   
using namespace std;      
int N;  
struct Act  
{  
    int start;  
    int end;  
}act[100010];  
  
bool cmp(Act a,Act b)    
{    
    return a.end<b.end;    
}   
  
int greedy_activity_selector()    
{    
    int num=1,i=1;     
    for(int j=2;j<=N;j++)    
    {    
        if(act[j].start>=act[i].end)    
        {    
            i=j;    
            num++;    
        }    
    }    
    return num;  
}  
  
int main()    
{    
    int t;  
    scanf("%d",&t);  
    while(t--)  
    {  
        scanf("%d",&N);  
        for(int i=1;i<=N;i++)  
        {  
            scanf("%lld %lld",&act[i].start,&act[i].end);  
        }  
        act[0].start=-1;  
        act[0].end=-1;  
        sort(act+1,act+N+1,cmp);   
        int res=greedy_activity_selector();  
        cout<<res<<endl;    
    }  
}