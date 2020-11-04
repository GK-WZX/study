#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int Collatz(int num);

int main(void)
{
	int ret = 0;
	
	pid_t pid;		//进程id
	int num = 0;

	printf("请输入开始数据：");
	scanf("%d",&num);	//读入开始数据

	/* 创建子进程 */
	pid = fork();

	/* 进程创建出错 */
	if(pid < 0)
	{ 
		ret = -1;
		printf("func fork() err:%d\n",ret);
		return ret;
	}
	/* 子进程执行程序 */
	else if(pid == 0)
	{
		printf("子进程开始执行：\n");
		ret = Collatz(num);		//根据起始数据输出Collatz猜想数列
        	if(ret != 0)
        	{
                	printf("func Collatz() err:%d\n",ret);
                	return ret;
        	}
		printf("子进程执行完毕,退出进程\n");
		exit(0);			//退出子进程
	}
	/* 父进程执行程序 */
	else
	{
		wait(NULL);
		printf("父进程执行\n");
	}

	return ret;
}

int Collatz(int num)
{
	int ret = 0;
	int tmp = num;

	/* 判断输入是不是正整数 */
	if(num <= 0)
	{
		ret = -1;
		printf("func Collatz() err:%d\n",ret);
		return ret;
	}

	/* Collatz猜想 */
	while(tmp != 1)
	{
		printf("%d ",tmp);

		if(tmp%2 == 0)
		{
			tmp = tmp/2;
		}
		else
		{
			tmp = 3 * tmp + 1;
		}

	}
	printf("%d\n",tmp);

	return ret;
}
