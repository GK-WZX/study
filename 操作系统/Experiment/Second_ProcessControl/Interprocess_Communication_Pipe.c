#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//缓冲区长度
#define BUFFER_LENGTH 256

int ToggleCase(char *input,int in_length,char *output,int* out_length);

int main(void)
{
	int ret = 0;
	int pipefd1[2] = {0};
	int pipefd2[2] = {0};
	pid_t pid = 0;
	char buf[BUFFER_LENGTH] = {0};
	char tmp_buf[BUFFER_LENGTH] = {0};
	int tmp_buf_length = 0;

	/* 创建两个匿名管道 */
	if(pipe(pipefd1) < 0 )
	{
		ret = -1;
		printf("func pipe() err:%d(pipe(pipefd1) < 0)\n",ret);
		return ret;
	}
	if(pipe(pipefd2) < 0 )
        {
                ret = -1;
                printf("func pipe() err:%d(pipe(pipefd2) < 0)\n",ret);
                return ret;
        }

	/* 创建子进程 */
	pid = fork();
	//创建子进程失败
	if(pid  < 0)
	{
		ret = -2;
		printf("func fork() err:%d(pid = fork() < 0)\n",ret);
		return ret;
	}
	//子进程执行程序
	else if(pid == 0)
	{
		close(pipefd1[0]);	//关闭管道1的读文件功能
		close(pipefd2[1]);	//关闭管道2的写文件功能

		//读通道2的文件
		if(read(pipefd2[0],buf,BUFFER_LENGTH) < 0)
		{
			ret = -1;
			printf("func read() err:%d(read(pipefd[1],buf,BUFFER_LENGTH) < 0)\n",ret);
			return ret;
		}
		printf("这是子进程：%d\n",getpid());
		printf("子进程接收到字符串：%s\n",buf);

		//将字符串大小写转换
		ret = ToggleCase(buf,strlen(buf),tmp_buf,&tmp_buf_length);
		if(ret != 0)
       		{
                	ret = -1;
                	printf("func ToggleCase() err:%d\n",ret);
                	return ret;
        	}

		//将转换后的字符串传给父进程
		printf("子进程传给父进程：%s\n",tmp_buf);
                if(write(pipefd1[1],tmp_buf,strlen(tmp_buf)) < 0)
                {
                        ret = -1;
                        printf("func write() err:%d(write(pipefd1[1],tmp_buf,strlen(tmp_buf))\n",ret);
                        return ret;
                }

		//退出子进程
		exit(0);

		
	}
	//父进程执行程序
	else
	{
		printf("这是父进程:%d\n",getpid());
		close(pipefd1[1]);	//关闭管道1的写文件功能
		close(pipefd2[0]);	//关闭管道2的读文件功能

		//命令行读入字符串
		printf("请输入input字符串：");
        	fgets(buf,BUFFER_LENGTH,stdin);
	        printf("您输入的字符串为：%s\n",buf);

		//写入通道2
		if(write(pipefd2[1],buf,strlen(buf)) < 0)
		{
			ret = -1;
			printf("func write() err:%d(write(pipefd2[1],buf,strlen(buf)) < 0)\n",ret);
			return ret;
		}

		//等待通道1有进程写入信息
		if(read(pipefd1[0],tmp_buf,BUFFER_LENGTH) < 0)
                {
                        ret = -1;
                        printf("func read() err:%d(read(pipefd1[1],tmp_buf,BUFFER_LENGTH) < 0)\n",ret);
                        return ret;
                }
                printf("父进程接收到字符串：%s\n",tmp_buf);
		//等待子进程退出
		wait(NULL);
		printf("子进程退出\n");

	}

	/*
	//命令行将字符串传给input[]
	printf("请输入input字符串：");
	fgets(input,BUFFER_LENGTH,stdin);
	printf("您输入的字符串为：%s\n",input);

	//大小写转化
	ret = ToggleCase(input,strlen(input),output,&out_length);
	if(ret != 0)
	{
		ret = -1;
		printf("func ToggleCase() err:%d\n",ret);
		return ret;
	}

	//输出结果
	printf("大小写转化后的结果：%s\n",output);
	*/

	return ret;
}

int ToggleCase(char *input,int in_length, char* output, int* out_length)
{
	int ret = 0;
	int  i = 0;

	//参数检查
	if(output == NULL || in_length < 0)
	{
		ret = -1;
		printf("func ToggleCase() err:%d(output == NULL || in_length < 0)\n",ret);
		return ret;
	}

	//大小写转化
	for(i = 0; i < in_length; i++)
	{
		if(input[i] <= 'z' && input[i] >= 'a')
		{
			output[i] = input[i] - (int)('a'-'A');
		}
		else if(input[i] <= 'Z' && input[i] >= 'A')
		{
			output[i] = input[i] + (int)('a' - 'A');
		}
		else
		{
			output[i] = input[i];
		}
	}
	output[i] = '\0';

	*out_length = in_length;

	return ret;

}
