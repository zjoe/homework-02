#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXN 1000000
#define ARRAY 40
#define BLOCK 50
#define SURROUND 150
FILE *fin,*fout;
const int dx[5]={0,-1,0,1,0};
const int dy[5]={0,0,1,0,-1};
void ArgCheck(int argc, char * argv[],int count[]);
void Error(char *s);
void Input(int *n,int *m,int a[][ARRAY]);
void Work(int n,int m,int a[][ARRAY],int count[]);
int Rectangle(int n,int m,int LimN,int LimM,int a[][ARRAY]);
int MonoList(int x[],int len,int Lim);
int InterConnect(int n,int m,int a[][ARRAY]);
int Partition(int n,int m,int a[][ARRAY],int fixed[][ARRAY],int part[][ARRAY],int value[BLOCK]);
int DFS(int i,int j,int num,int n,int m,int fixed[][ARRAY],int part[][ARRAY]);
void SPFA(int n,int m,int bx,int by,int a[][ARRAY],int fixed[][ARRAY],int dis[][ARRAY],int prex[][ARRAY],int prey[][ARRAY]);
void GetContour(int n,int m,int fixed[][ARRAY],int part[][ARRAY],int conx[][SURROUND],int cony[][SURROUND],int ncon[BLOCK]);
void TestOut(int a[][ARRAY],int l1,int l2);
int main(int argc, char * argv[])
{
	int n,m,a[ARRAY][ARRAY],count[3];
	FILE *fout=fopen("output.txt","w");
	ArgCheck(argc,argv,count);
	Input(&n,&m,a);
	Work(n,m,a,count);
	return 0;
}
void ArgCheck(int argc, char * argv[],int count[])
{
	if(argc>5)
		Error("too many arguments");
	int i;

	count[0]=count[1]=count[2]=0;
	for(i=1;i<argc-1;i++)
	{
		if(argv[i][0]!='\\' || (argv[i][1]!='a' && argv[i][1]!='h' && argv[i][1]!='v') || argv[i][2]!='\0')
			Error("invalid argument");
		else if(argv[i][1]=='a')
			count[0]++;
		else if(argv[i][1]=='h')
			count[1]++;
		else if(argv[i][1]=='v')
			count[2]++;
	}

	if(count[0]>1 || count[1]>1 || count[2]>1)
		Error("repeated arguments");

	fin=fopen(argv[argc-1],"r");
	if(fin==NULL)
		Error("no such file");
}
void Error(char *s)
{
	printf("%s\n",s);
	exit(1);
}
void Input(int *n,int *m,int a[][ARRAY])
{
	int i,j;
	char s[500],*ptr,*endptr;

	fscanf(fin,"%s",s);
	ptr=s;
	*n=GetInt(ptr,&endptr);
	if(endptr[0]!=',' || endptr[1]!='\0')
		Error("invalid characters appears");

	fscanf(fin,"%s",s);
	ptr=s;
	*m=GetInt(ptr,&endptr);
	if(endptr[0]!=',' || endptr[1]!='\0')
		Error("invalid characters appears");

	for(i=1;i<=*n;i++)
	{
		fscanf(fin,"%s",s);
		ptr=s;
		for(j=1;j<=*m;j++)
		{
			endptr=NULL;
			a[i][j]=GetInt(ptr,&endptr);
			if(endptr[0]!=',' && endptr[0]!='\0')
				Error("invalid characters appears");
			ptr=endptr+1;
		}
		if(endptr[0]!='\0')
			Error("invalid end of line");
	}

	printf("n=%d m=%d\n",*n,*m);
	for(i=1;i<=*n;i++)
	{
		for(j=1;j<=*m;j++)
			printf("%d ",a[i][j]);
		printf("\n");
	}
}
int GetInt(char *s,char **end)
{
	int x;
	x=strtol(s,end,10);
	if(*end==s)
		Error("not integer");
	if(*end[0]!=',' && *end[0]!='\0')
		Error("invalid character appears");
	return x;
}
void Work(int n,int m,int a[][ARRAY],int count[])
{
	int ans,i,j;
	if(count[0]==0 && count[1]==0 && count[2]==0)
	{
		printf("no arg\n");
		ans=Rectangle(n,m,n,m,a);
		printf("%d\n",ans);
	}
	else if(count[0]==0 && count[1]==1 && count[2]==0)
	{
		printf("\\h\n");
		for(i=1;i<=n;i++)
			memmove(a[i]+m+1,a[i]+1,sizeof(int)*m);
		TestOut(a,n,2*m);
		ans=Rectangle(n,2*m,n,m,a);
		printf("%d\n",ans);
	}
	else if(count[0]==0 && count[1]==0 && count[2]==1)
	{
		printf("\\v\n");
		for(i=1;i<=n;i++)
			memmove(a[i+n],a[i],sizeof(int)*m);
		
		TestOut(a,2*n,m);

		ans=Rectangle(2*n,m,n,m,a);
		printf("%d\n",ans);
	}
	else if(count[0]==0 && count[1]==1 && count[2]==1)
	{
		printf("\\v\\h\n");
		for(i=1;i<=n;i++)
		{
			memmove(a[i]+m+1,a[i]+1,sizeof(int)*m);
			memmove(a[i+n],a[i],sizeof(int)*m);
			memmove(a[i+n]+m+1,a[i]+1,sizeof(int)*m);
		}

		TestOut(a,2*n,2*m);

		ans=Rectangle(2*n,2*m,n,m,a);
		printf("%d\n",ans);

	}
	else if(count[0]==1 && count[1]==0 && count[2]==0)
	{
		printf("\\a\n");
		ans=InterConnect(n,m,a);
		printf("%d\n",ans);
	}
}
int Rectangle(int n,int m,int LimN,int LimM,int a[][ARRAY])
{
	int sum[2*ARRAY][2*ARRAY],sum1[2*ARRAY],minN,ans,temp,i,j,k;
	for(i=0;i<=n;i++)
		memset(sum[i],0,sizeof(int)*(m+1));

	for(i=1;i<=n;i++)
		for(j=1;j<=m;j++)
			sum[i][j]=sum[i-1][j]+sum[i][j-1]-sum[i-1][j-1]+a[i][j];
	ans=-MAXN;
	for(i=1;i<=n;i++)
	{
		if(i+LimN-1<n)
			minN=i+LimN-1;
		else
			minN=n;
		
		for(j=i;j<=minN;j++)
		{
			for(k=0;k<=m;k++)
				sum1[k]=sum[j][k]-sum[i-1][k];

			temp=MonoList(sum1,m,LimM);
			if(temp>ans)
				ans=temp;
		}

	}
	return ans;
}
int MonoList(int x[],int len,int Lim)
{
	int head,tail,ans,temp,i;
	int list[2*ARRAY];

	list[1]=0;
	head=1;
	tail=1;
	if(x[list[tail]]>=x[1])
		list[tail]=1;
	else
		list[++tail]=1;

	ans=x[1];
	for(i=2;i<=len;i++)
	{
		while(i-list[head]>Lim)head++;
		temp=x[i]-x[list[head]];
		if(temp>ans)ans=temp;
		while(tail>=head && x[list[tail]]>=x[i])
			tail--;
		list[++tail]=i;
	}
	return ans;
}
void TestOut(int a[][ARRAY],int l1,int l2)
{
	int i,j;
	for(i=1;i<=l1;i++)
	{
		for(j=1;j<=l2;j++)
			printf("%6d ",a[i][j]);
		printf("\n");
	}
	printf("\n");
}
int InterConnect(int n,int m,int a[][ARRAY])
{
	int i,j,k,num,max,bx,by,ex,ey,k1,k2,nowx,nowy,temp,ans;
	int dis[ARRAY][ARRAY],fixed[ARRAY][ARRAY],part[ARRAY][ARRAY];
	int value[BLOCK],prex[ARRAY][ARRAY],prey[ARRAY][ARRAY];
	int conx[BLOCK][SURROUND],cony[BLOCK][SURROUND],ncon[BLOCK];

	for(i=1;i<=n;i++)for(j=1;j<=m;j++)
		if(a[i][j]>=0)
			fixed[i][j]=1;
		else 
			fixed[i][j]=0;

	while(1)
	{
		num=Partition(n,m,a,fixed,part,value);
		
		TestOut(part,n,m);
		TestOut(fixed,n,m);
		GetContour(n,m,fixed,part,conx,cony,ncon);

		printf("num=%d\n",num);
		max=-MAXN;
		for(i=1;i<=num-1;i++)
			for(k1=1;k1<=ncon[i];k1++)
			{
				SPFA(n,m,conx[i][k1],cony[i][k1],a,fixed,dis,prex,prey);
				for(j=i+1;j<=num;j++)
					for(k2=1;k2<=ncon[j];k2++)
						if(dis[conx[j][k2]][cony[j][k2]]-a[conx[j][k2]][cony[j][k2]]>max)
						{
							max=dis[conx[j][k2]][cony[j][k2]]-a[conx[j][k2]][cony[j][k2]];
							bx=conx[i][k1];
							by=cony[i][k1];
							ex=conx[j][k2];
							ey=cony[j][k2];
						}
			}

		if(max==-MAXN)break;

		printf("%d %d %d %d %d\n",bx,by,ex,ey,max);
		SPFA(n,m,bx,by,a,fixed,dis,prex,prey);
		nowx=ex;
		nowy=ey;
		while(nowx!=0 && nowy!=0)
		{
			fixed[nowx][nowy]=1;
			temp=nowx;
			nowx=prex[nowx][nowy];
			nowy=prey[temp][nowy];
		}
	}
	ans=0;
	for(i=1;i<=n;i++)for(j=1;j<=m;j++)if(fixed[i][j])ans+=a[i][j];
	return ans;
}
void SPFA(int n,int m,int bx,int by,int a[][ARRAY],int fixed[][ARRAY],int dis[][ARRAY],int prex[][ARRAY],int prey[][ARRAY])
{
	int i,j,h,t,x,y,tx,ty,sx[10000],sy[10000],in[100][100];
	for(i=1;i<=n;i++)
		for(j=1;j<=m;j++)
		{
			dis[i][j]=-MAXN;
			in[i][j]=0;
			prex[i][j]=0;
			prey[i][j]=0;
		}
	sx[1]=bx;
	sy[1]=by;
	dis[bx][by]=0;
	h=t=1;
	in[bx][by]=1;

	while(h<=t)
	{
		x=sx[h];
		y=sy[h];
		for(i=1;i<=4;i++)
		{
			tx=x+dx[i];
			ty=y+dy[i];
			if(tx>=1 && tx<=n && ty>=1 && ty<=m && 
				((fixed[x][y]==0 && (tx!=bx || ty!=by)) || (x==bx && y==by && fixed[tx][ty]==0)) &&
				dis[x][y]+a[tx][ty]>dis[tx][ty])
			{
				dis[tx][ty]=dis[x][y]+a[tx][ty];
				prex[tx][ty]=x;
				prey[tx][ty]=y;
				if(in[tx][ty]==0)
				{
					in[tx][ty]=1;
					t++;
					sx[t]=tx;
					sy[t]=ty;
				}
			}
		}
		in[x][y]=0;
		h++;
	}
}
void GetContour(int n,int m,int fixed[][ARRAY],int part[][ARRAY],int conx[100][150],int cony[100][150],int ncon[100])
{
	int i,j,k,p,num;
	
	for(i=0;i<BLOCK;i++)
	{
		memset(conx[i],0,sizeof(int)*SURROUND);
		memset(cony[i],0,sizeof(int)*SURROUND);
	}
	memset(ncon,0,sizeof(int)*BLOCK);

	for(i=1;i<=n;i++)
		for(j=1;j<=m;j++)
			if(fixed[i][j]==1)
			{
				p=part[i][j];
				for(k=1;k<=4;k++)
				{
					if(i+dx[k]>=1 && i+dx[k]<=n && j+dy[k]>=1 && j+dy[k]<=m && fixed[i+dx[k]][j+dy[k]]==0)
					{
						ncon[p]++;
						conx[p][ncon[p]]=i;
						cony[p][ncon[p]]=j;
						break;
					}

				}
			}
}
int Partition(int n,int m,int a[][ARRAY],int fixed[][ARRAY],int part[][ARRAY],int value[30])
{
	int i,j,num;
	for(i=1;i<=n;i++)
		memset(part[i],0,sizeof(int)*(m+1));

	num=0;
	for(i=1;i<=n;i++)
		for(j=1;j<=m;j++)
			if(fixed[i][j]==1 && part[i][j]==0)
			{
				num++;
				value[num]=DFS(i,j,num,n,m,fixed,part);
			}
	return num;
}
int DFS(int i,int j,int num,int n,int m,int fixed[][ARRAY],int part[][ARRAY])
{
	if(part[i][j]!=0 || fixed[i][j]==0)return 0;
	int sum=0;
	part[i][j]=num;
	if(i-1>=1)sum+=DFS(i-1,j,num,n,m,fixed,part);
	if(i+1<=n)sum+=DFS(i+1,j,num,n,m,fixed,part);
	if(j-1>=1)sum+=DFS(i,j-1,num,n,m,fixed,part);
	if(j+1<=m)sum+=DFS(i,j+1,num,n,m,fixed,part);
	return sum;
}