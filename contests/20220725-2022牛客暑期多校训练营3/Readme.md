# 7.25 牛客训练第二场


赛时过了A、C两道题，~~（稳定两道题）~~，J题其实好早想出正解了，不知道是题面太抽象了还是我太菜了qwq

下面记录一下赛时过程以及赛后改题



**A**
**zhn-verse**

首先观察这道题，通过分类讨论可以得知，对于树上一组点的lca，如果去掉这组点其中的一个点，**最多**只有一个点去掉之后，会对这个lca产生影响。第二个结论就是，只有**dfs序中最浅的与最深的两个点会对lca产生影响**（感谢xf小鸭子的idea）,之后我们讨论最浅的点与最深的点对于lca的影响，记录下来lca的变化结果，进行比较即可。

我写的比较麻烦，代码如下

```c++
#include <bits/stdc++.h>
using namespace std;
const int N=1e5+7;
int n,k,val1[N],val2[N];
bool vis[N];
int fa1[N][26],fa2[N][26],id1,id2;
vector<int>A[N],B[N];
int siz1[N],dfn1[N],dep1[N];
int siz2[N],dfn2[N],dep2[N];
int K[N];
int lca_fir[5],lca_sec[5];
inline int read(){
    int x=0,f=1;
    char ch=getchar();
    while(ch<'0'||ch>'9'){if(ch=='-')f=-1;ch=getchar();}
    while(ch>='0'&&ch<='9'){x=x*10+ch-'0';ch=getchar();}
    return x*f;
}
void dfs1(int s,int fa){
    dfn1[s]=++id1;
    dep1[s]=dep1[fa]+1;
    fa1[s][0]=fa;
    siz1[s]=1;
    for(int i=1;i<=20;i++)
        fa1[s][i]=fa1[fa1[s][i-1]][i-1];

    for(auto u:A[s]){
        if(u==fa) continue;
        dfs1(u,s);
        siz1[s]+=siz1[u];
    }
    return ;
}
void dfs2(int s,int fa){
    dfn2[s]=++id2;
    dep2[s]=dep2[fa]+1;
    fa2[s][0]=fa;
    siz2[s]=1;
    for(int i=1;i<=20;i++)
        fa2[s][i]=fa2[fa2[s][i-1]][i-1];
    for(auto u:B[s]){
        if(u==fa) continue;
        dfs2(u,s);
        siz2[s]+=siz2[u];
    }
    return ;
}
int lca1(int x,int y)
{
	if(dep1[x]<dep1[y]) swap(x,y);
	for(int i=19;i>=0;i--)
		if(dep1[x]-(1<<i)>=dep1[y])
			x=fa1[x][i];
	if(x==y) return x;
	for(int i=19;i>=0;i--)
		if(fa1[x][i]!=fa1[y][i])
			x=fa1[x][i],y=fa1[y][i];
	return fa1[x][0];
}
int lca2(int x,int y)
{
	if(dep2[x]<dep2[y]) swap(x,y);
	for(int i=19;i>=0;i--)
		if(dep2[x]-(1<<i)>=dep2[y])
			x=fa2[x][i];
	if(x==y) return x;
	for(int i=19;i>=0;i--)
		if(fa2[x][i]!=fa2[y][i])
			x=fa2[x][i],y=fa2[y][i];
	return fa2[x][0];
}

int main(){
    n=read();
    k=read();
    for(int i=1;i<=k;i++){
        int x;
        x=read();
        vis[x]=1;
        K[i]=x;
    }
    for(int i=1;i<=n;i++) val1[i]=read();
    for(int i=1;i<n;i++){
        int x,y;
        x=read();
        A[x].push_back(i+1);
        A[i+1].push_back(x);
    }
    for(int i=1;i<=n;i++) val2[i]=read();
    for(int i=1;i<n;i++){
        int x,y;
        x=read();
        B[x].push_back(i+1);
        B[i+1].push_back(x);
    }
    id1=0;
    id2=0;
    dfs1(1,0);
    dfs2(1,0);
    //µÚÒ»¿ÃÊ÷×îÇ³µÄºÍ×îÉîµÄ¶ù×Ó
    int minn_fir=2147483640,maxx_fir=0;
    int minn_fir_dier=2147438640,maxx_fir_dier=0;
    int minn_node_fir,maxx_node_fir;
    int minn_node_fir_dier,maxx_node_fir_dier;

    for(int i=1;i<=k;i++){
        if(dfn1[K[i]]<=minn_fir){
            minn_fir=dfn1[K[i]];
            minn_node_fir=K[i];
        }

        if(dfn1[K[i]]>=maxx_fir){
            maxx_fir=dfn1[K[i]];
            maxx_node_fir=K[i];
        }
    }

    for(int i=1;i<=k;i++){
        if(dfn1[K[i]]<=minn_fir_dier&&dfn1[K[i]]!=minn_fir){
            minn_fir_dier=dfn1[K[i]];
            minn_node_fir_dier=K[i];
        }
        if(dfn1[K[i]]>=maxx_fir_dier&&dfn1[K[i]]!=maxx_fir){
            maxx_fir_dier=dfn1[K[i]];
            maxx_node_fir_dier=K[i];
        }
    }

    lca_fir[1]=lca1(minn_node_fir,maxx_node_fir);
    lca_fir[2]=lca1(minn_node_fir,maxx_node_fir_dier);
    lca_fir[3]=lca1(minn_node_fir_dier,maxx_node_fir);

    /* µÚ¶þ¸öÊ÷*/
    int minn_sec=2147483640,maxx_sec=0;
    int minn_sec_dier=2147438640,maxx_sec_dier=0;
    int minn_node_sec,maxx_node_sec;
    int minn_node_sec_dier,maxx_node_sec_dier;

    for(int i=1;i<=k;i++){
        if(dfn2[K[i]]<=minn_sec){
            minn_sec=dfn2[K[i]];
            minn_node_sec=K[i];
        }

        if(dfn2[K[i]]>=maxx_sec){
            maxx_sec=dfn2[K[i]];
            maxx_node_sec=K[i];
        }
    }

    for(int i=1;i<=k;i++){
        if(dfn2[K[i]]<=minn_sec_dier&&dfn2[K[i]]!=minn_sec){
            minn_sec_dier=dfn2[K[i]];
            minn_node_sec_dier=K[i];
        }
        if(dfn2[K[i]]>=maxx_sec_dier&&dfn2[K[i]]!=maxx_sec){
            maxx_sec_dier=dfn2[K[i]];
            maxx_node_sec_dier=K[i];
        }
    }

     lca_sec[1]=lca2(minn_node_sec,maxx_node_sec);
     lca_sec[2]=lca2(minn_node_sec,maxx_node_sec_dier);
     lca_sec[3]=lca2(minn_node_sec_dier,maxx_node_sec);

     bool flag1=0;
     bool flag2=0;

//    for(int i=1;i<=3;i++){
//        for(int j=i+1;j<=3;j++){
//            if(lca_fir[i]!=lca_fir[j]){
//                flag1=1;
//            }
//            if(lca_sec[i]!=lca_sec[j]){
//                flag2=1;
//            }
//        }
//    }
//
//    if((!flag1)&&(!flag2)){
//        if(val1[lca_fir[1]]>val2[lca_sec[1]){
//            printf("%d",k);
//        }
//        else printf("0");
//    }
//    else if((!flag1)&&flag2){
//
//    }
//    else if(flag1&&(!flag2)){
//
//    }
//    else{
//
//    }
	
	
	int cnt=0;
	for(int i=1;i<=k;i++)
	{
		if((K[i]==maxx_node_fir)||(K[i]==minn_node_fir))
		{
			int la;
			if(K[i]==maxx_node_fir)
				la=lca_fir[2];
			else
				la=lca_fir[3];
				
			int lb;
			if(K[i]==maxx_node_sec)
				lb=lca_sec[2];
			else if(K[i]==minn_node_sec)
				lb=lca_sec[3];
			else
				lb=lca_sec[1];
			if(val1[la]>val2[lb])
				cnt++;
		}
		else
		{
			int la=lca_fir[1];
			
			int lb;
			if(K[i]==maxx_node_sec)
				lb=lca_sec[2];
			else if(K[i]==minn_node_sec)
				lb=lca_sec[3];
			else
				lb=lca_sec[1];	
			if(val1[la]>val2[lb])
				cnt++;		
		}
	} 
	printf("%d",cnt);
    /*output */
    return 0;
}
```

