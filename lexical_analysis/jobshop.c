#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define crossover_rate 0.95
#define mutate_rate 0.95
#define GROUP_SIZE 3000
#define LIMIT 300000

double T=10000;
double T_min=1e-8;
double k=0.997; //�˻��������// 
int length=0,n,m;//�ܹ����� ,���������� 

struct project
{
	int no;//��¼��i�������Ѿ�����˼������� 
	int number;//��¼��i�������ܹ����� 
	int machine[100];//��¼��i��������j��������ռ�õĻ��� 
	int time[100];//��¼��i��������j���������õ�ʱ�� 
};
typedef struct project pro;//�洢��Ʒ��Ϣ��

struct machine
{
	int NO;
	int time[100][2];
	int operation[100];
	int process[100];
};
typedef struct machine mach;

void input(pro *);//���빤����Ϣ��������� 20�� 
int caltime(pro *,int *);//����DNAʱ�䣬��������� 30�� 
void mutate(int* );//��DNA���б��죨һ��DNA�������λ�ã� ��������� 
void crossover(int** ,int** );//��DNA���н��滥��������DNA�������һ�����У� ��������� 
void population(int** ,pro* );//���������Ⱥ ��������� 
void select(int** ,pro* ,int);//��Ȼѡ�� ������� 
void output(pro*,int*);//���ؽ�� 
int max(int ,int );//���������������ֵ ��������� 
int findmax(int* ,int);//Ѱ��һ��һά�����е����ֵ ��������� 
void copy(int *,int *);//����һά���� ��������� 

void printDNA(int* DNA)
{
	int i;
	for(i=0;i<length;i++)
		printf("%d ",DNA[i]);
	printf("\n");
}

void input(pro *proj)//���빤����Ϣ��
{
	int i1,total=0;
	char ch='\0';
	
	for(i1=0;i1<n;i1++)
	{
		while(ch!='\n')
		{
			scanf("%d %d",&proj[i1].machine[total],&proj[i1].time[total]);
			total++;
			ch=getchar();
		}
		
		(proj+i1)->number=total;
		
		ch='\0';
		total=0;
	}	
}

int caltime(pro* proj,int* DNA)//����DNAʱ��
{
	int operationtime[10000];//�������Ź�����ʼʱ�� 
	int machinetime[10000];//��ʼ���Ż�����ʼʱ�� 
	int totaltime=0,w=0,maxtime=0,non,a1,b1;//�м���� 
	
	for(w=0;w<n;w++)
	{
		operationtime[w]=0;
		machinetime[w]=0;
		(proj+w)->no=0;
	}
	w=0;

	while(w<length)
	{
		non=proj[DNA[w]].no;
		a1=operationtime[DNA[w]];
		b1=machinetime[proj[DNA[w]].machine[non]];
		maxtime=max(a1,b1);
		maxtime+=proj[DNA[w]].time[proj[DNA[w]].no];
		operationtime[DNA[w]]=maxtime;
		machinetime[proj[DNA[w]].machine[non]]=maxtime;
		((proj+DNA[w])->no)++;
		w++;
	}
	totaltime=max(findmax(operationtime,n),findmax(machinetime,m));//������ʱ�� 
	
	return totaltime;
} //����ʱ��// 

int max(int a,int b)//Ѱ�����ֵ 
{
	int max;
	
	if(a>b)
		max=a;
	else
		max=b;

	return max;
} //�����ֵ// 

int findmax(int* array,int length1)//Ѱ��һά�������ֵ
{
	int i=0,max=array[0];
	
	while(i<length1)
	{
		if(max<array[i])
			max=array[i];
		i++;
	}
	
	return max;
}
 
void mutate(int* DNA)//��DNA���б��죨һ��DNA�������λ�ã�
{
	int position1,position2,temp;
	
	position1=rand()%length;
	position2=rand()%length;
	/*���ȷ��Ҫ�������������ֵ�λ��*/
	 
	temp=*(DNA+position1);
	*(DNA+position1)=*(DNA+position2);
	*(DNA+position2)=temp;
	/*����λ��*/ 
}

void crossover(int** DNA1ptr,int** DNA2ptr)//��DNA���н��滥��������DNA�������һ�����У�
{
	int opera,i=0;
	int count1=0;
	int newDNA1[10000];
	int newDNA2[10000];
	
	srand(time(NULL));
	
	opera=rand()%n;//���ѡ���ض������� 
	for(i=0;i<length;i++)
	{
		newDNA1[i]=-1;
		newDNA2[i]=-1;
	}
	 
	for(i=0;i<length;i++)
	{
		if((*DNA1ptr)[i]==opera)
			newDNA1[i]=opera;
		if((*DNA2ptr)[i]==opera)
			newDNA2[i]=opera;
	}
	/*���ѡ����ض�������λ�ò���*/
	
	for(i=0;i<length;i++)
	{
		if((*DNA2ptr)[i]!=opera)
		{
			if(newDNA1[count1]==-1)
			{
				newDNA1[count1]=(*DNA2ptr)[i];
				count1++;
			}
			else
			{
				while(newDNA1[count1]!=-1)
					count1++;
				newDNA1[count1]=(*DNA2ptr)[i];
			}
		}
	}
	count1=0;
	for(i=0;i<length;i++)
	{
		if((*DNA1ptr)[i]!=opera)
		{
			if(newDNA2[count1]==-1)
			{
				newDNA2[count1]=(*DNA1ptr)[i];
				count1++;
			}
			else
			{
				while(newDNA2[count1]!=-1)
				count1++;
				newDNA2[count1]=(*DNA1ptr)[i];
			}
		}
	}
	count1=0;
	/*����λ�ð�˳�򲹵���һ����DNA��*/ 
	for(i=0;i<length;i++)
	{
		*(*DNA1ptr+i)=newDNA1[i];
		*(*DNA2ptr+i)=newDNA2[i];
	}

}

void population(int** oper,pro* proj)//���������Ⱥ 
{
	int i=0;
	int count=0;
	int j=0;
	
	for(i=0;i<n;i++)//��DNA���Ƶ���һ��DNA�� 
	{
		for(j=0;j<proj[i].number;j++)
		{
			*(*oper+count)=i;
			count++;
		}
	}
	
	for(i=0;i<GROUP_SIZE-1;i++)//����DNA���ж�α��� 
	{
		copy(*(oper+i),*(oper+i+1));
		for(count=0;count<100;count++)
			mutate(*(oper+i+1));
	}
}

void select (int** oper,pro* proj,int i)//�����˻�ɸѡ�ķ�ʽ������Ȼѡ�� 
{
    int det_X1,det_X2;
    int i1=0,i2=0;
    
    int new_oper[2][length];
	for(i1=0;i1<length;i1++)
		new_oper[0][i1]=oper[2*(i+1)-2][i1];
	for(i2=0;i2<length;i2++)
		new_oper[1][i2]=oper[2*(i+1)-1][i2];
	int child1,child2,parent1,parent2;
	
	srand((time(NULL)));
	
    if((double)rand()/(RAND_MAX)<crossover_rate)
        crossover((oper+2*(i+1)-2),(oper+2*(i+1)-1));
	if((double)rand()/(RAND_MAX)<mutate_rate)
		mutate(*(oper+2*(i+1)-2));
	if((double)rand()/(RAND_MAX)<mutate_rate)
		mutate(*(oper+2*(i+1)-1));
		
	child1=caltime(proj,*(oper+2*(i+1)-2));
	parent1=caltime(proj,new_oper[0]);
	child2=caltime(proj,*(oper+2*(i+1)-1));
	parent2=caltime(proj,new_oper[1]);
	
		    
	det_X1=child1-parent1;
	det_X2=child2-parent2;
	
	if(det_X1>=0)
	   	copy(new_oper[0],*(oper+2*(i+1)-2));
	else if(exp(det_X1/T)<rand()/(RAND_MAX))
		copy(new_oper[0],*(oper+2*(i+1)-2));
		
	if(det_X2>=0)
	    copy(new_oper[1],*(oper+2*(i+1)-1));
	else if(exp(det_X2/T)<rand()/(RAND_MAX))
		copy(new_oper[1],*(oper+2*(i+1)-1));	
}

void copy(int *DNA1,int *DNA2)//������1���Ƶ�����2�� 
{ 
	int i;
	
 	for(i=0;i<length;i++)
		*(DNA2+i)=*(DNA1+i);
}

void output(pro* proj,int* DNA)
{
	static int operationtime[1000]={0};
	static int machinetime[1000]={0};
	int w=0,maxtime=0,non,a1,b1,i,j;
	static	mach machdata[1000];
	int zz;
	for(zz=0;zz<1000;zz++){
		machdata[zz].NO=0;
		memset(machdata[zz].operation,0,sizeof(machdata[zz].operation));
		memset(machdata[zz].process,0,sizeof(machdata[zz].process));
		memset(machdata[zz].time,0,sizeof(machdata[zz].time));
	}
	
	for(w=0;w<n;w++)
		(proj+w)->no=0;
	w=0;

	while(w<length)
	{
		non=proj[DNA[w]].no;
		
		machdata[proj[DNA[w]].machine[non]].operation[machdata[proj[DNA[w]].machine[non]].NO]=DNA[w];
		machdata[proj[DNA[w]].machine[non]].process[machdata[proj[DNA[w]].machine[non]].NO]=non;
		
		a1=operationtime[DNA[w]];
		b1=machinetime[proj[DNA[w]].machine[non]];
		maxtime=max(a1,b1);
		machdata[proj[DNA[w]].machine[non]].time[machdata[proj[DNA[w]].machine[non]].NO][0]=maxtime;
		maxtime+=proj[DNA[w]].time[proj[DNA[w]].no];
		machdata[proj[DNA[w]].machine[non]].time[machdata[proj[DNA[w]].machine[non]].NO][1]=maxtime;
		operationtime[DNA[w]]=maxtime;
		machinetime[proj[DNA[w]].machine[non]]=maxtime;
		((proj+DNA[w])->no)++;
		
		
		machdata[proj[DNA[w]].machine[non]].NO++;
		w++;
	}
	
	for(i=0;i<m;i++)
	{
		printf("M%d",i);
		for(j=0;j<machdata[i].NO;j++)
		{
			printf("(%d,%d-%d,%d)",machdata[i].time[j][0],machdata[i].operation[j],machdata[i].process[j],machdata[i].time[j][1]);
		}
		printf("\n");
	}
		
} 
int main (void)
{
	 while(clock()<LIMIT) 
	{   
		clock_t begin, end;
		begin=clock();
		double cost;
		int i,min,t=10000;
		int** oper;
		pro proj[1000];
		int zz;
		for(zz=0;zz<1000;zz++){
			proj[zz].no=proj[zz].number=0;
			memset(proj[zz].machine,0,sizeof(proj[zz].machine));
			memset(proj[zz].time,0,sizeof(proj[zz].time));
		}
		int array[10000];
		
		srand(time(NULL));	

		oper=(int**)malloc(sizeof(int*)*GROUP_SIZE);
		for(i=0;i<GROUP_SIZE;i++)
			oper[i]=(int*)malloc(sizeof(int)*10000);
		
		scanf("%d %d",&n,&m);
	
		input(proj);	
		
		for(i=0;i<n;i++)
			length+=proj[i].number;	
	
		population(oper,proj);	
	
		while(T>T_min)
		{
			for (i=0;i<GROUP_SIZE/2;i++)
			 	select(oper,proj,i);
		  	T=T*k;
		}
	
		min=caltime(proj,oper[0]);
	
		for(i=1;i<GROUP_SIZE;i++)
		{
			t=caltime(proj,oper[i]);
			if(min>t)
			{
				min=t;
				copy(oper[i],array);
			}
				
		}
		end=clock();
		cost = (double)(end - begin)/CLOCKS_PER_SEC;
		printf("Time Used:%.3lfs\n",cost);
		printf("%d\n",min);
		output(proj,array);
		printDNA(array);	
		for(i=0;i<GROUP_SIZE;i++)
			free(oper[i]);
		free(oper);
		oper=NULL;
		system("pause");
	} 
	return 0; 
}
