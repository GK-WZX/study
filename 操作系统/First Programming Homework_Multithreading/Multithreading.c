#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<windows.h>

//���νṹ
typedef struct PARAMETER
{
	int* List;
	int num;
	int (*Func)(int* List, int num);
}PARAMETER;

//ȫ�ֱ�����Ϊͳ��ֵ
int g_max = 0, g_min = 0;
float g_average = 0;

//�û����������б���һ��Ϊ�б����������ָ������ڶ���Ϊ�б����������ݣ��ո�ָ�
int InputNumberList(int** List, int* num)
{
	int ret = 0;
	int i = 0;

	//�������
	if (List == NULL || num == NULL)
	{
		ret = -1;
		printf("func InputNumberList() err: ret = %d(List == NULL || num == NULL)\n", ret);
		return ret;
	}

	printf("�����������б�����ݸ�����");
	scanf("%d", num);

	//��ջ������ռ�洢�����б��е�����
	*List = (int*)malloc(sizeof(int) * (*num));
	if (*List == NULL)
	{
		ret = -2;
		printf("func InputNumberList() err: ret = %d(*List == NULL)\n", ret);
		return ret;
	}

	//���û����������б������
	printf("�����������б�����ݣ�\n");
	for (i = 0; i < *num; i++)
	{
		scanf("%d", &((*List)[i]));
	}

	return ret;
}

//��ӡ�����б�
void PrintNumList(int* List, int num)
{
	int i = 0;

	//��ӡ�����б�
	printf("������������б�Ϊ��");
	for (i = 0; i < num; i++)
	{
		printf("%d ", List[i]);
	}
	printf("\n");
	printf("ƽ��ֵ��%f\n", g_average);
	printf("���ֵ��%d\n", g_max);
	printf("��Сֵ��%d\n", g_min);
}

//�̺߳���
DWORD ThreadFunc(LPVOID List)
{
	//���ݴ������Ĳ�������ִ���Ǹ�����
	PARAMETER* p = (PARAMETER*)List;
	p->Func(p->List, p->num);

	return 0;
}

//��ƽ��ֵ
int GetAverage(int *List, int num)
{
	int ret = 0;
	int i = 0;
	int sum = 0;

	for (i = 0; i < num; i++)
	{
		sum = sum + List[i];
	}
	g_average = (float)sum / num;

	return ret;
}

//�����ֵ
int GetMax(int* List, int num)
{
	int ret = 0;
	int i = 0;

	g_max = List[0];
	for (i = 1; i < num ; i++)
	{
		if (List[i] > g_max)
		{
			g_max = List[i];
		}
	}

	return ret;
}

//����Сֵ
int GetMin(int* List, int num)
{
	int ret = 0;
	int i = 0;

	g_min = List[0];
	for (i = 1; i < num; i++)
	{
		if (List[i] < g_min)
		{
			g_min = List[i];
		}
	}

	return ret;
}

int main(void)
{
	int		ret = 0;			//����ֵ
	int		num = 0;			//�����б��������
	int*	List = NULL;		//ָ�������б��ָ��

	//�߳̾��
	HANDLE	htd0 = NULL;
	HANDLE	htd1 = NULL;
	HANDLE	htd2 = NULL;

	//�߳�ID
	DWORD	thrd_id0 = 0;
	DWORD	thrd_id1 = 0;
	DWORD	thrd_id2 = 0;

	//�̺߳��������ṹ
	PARAMETER	parameter0 = { 0 };
	PARAMETER	parameter1 = { 0 };
	PARAMETER	parameter2 = { 0 };


	//�û����������б�
	ret = InputNumberList(&List, &num);


	//���̲߳����ṹ��ֵ
	parameter0.List = List;
	parameter0.num = num;
	parameter0.Func = GetAverage;
	parameter1.List = List;
	parameter1.num = num;
	parameter1.Func = GetMax;
	parameter2.List = List;
	parameter2.num = num;
	parameter2.Func = GetMin;

	//�����߳�
	htd0 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc, (void*)&parameter0, 0, &thrd_id0);
	if (htd0 == NULL)
	{
		ret = -1;
		printf("func CreateThread() err��%d (htd0 == NULL)\n", ret);
		goto END;
	}
	htd1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc, (void*)&parameter1, 0, &thrd_id1);
	if (htd1 == NULL)
	{
		ret = -1;
		printf("func CreateThread() err��%d (htd1 == NULL)\n", ret);
		goto END;
	}
	htd2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc, (void*)&parameter2, 0, &thrd_id2);
	if (htd2 == NULL)
	{
		ret = -1;
		printf("func CreateThread() err��%d (htd2 == NULL)\n", ret);
		goto END;
	}

	//�ȴ������߳�ִ�����
	WaitForSingleObject(htd0, INFINITE);
	WaitForSingleObject(htd1, INFINITE);
	WaitForSingleObject(htd2, INFINITE);

	//��ӡ�����б����ƽ��ֵ�����ֵ����Сֵ
	PrintNumList(List, num);

END:
	//�ͷ�ջ���ռ䣬����Ұָ��
	if (List != NULL)
	{
		free(List);
		List = NULL;
	}

	return ret;
}