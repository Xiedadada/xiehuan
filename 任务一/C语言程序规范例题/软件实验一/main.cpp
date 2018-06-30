//Author:xiehuan
//���ǹ��ڲ���ϵͳ�й��ڶ�̬�����ڴ�Ĵ��룬����������Ӧ�㷨��������Ӧ�㷨���ͻ����㷨��
#include<stdio.h>
#include<stdlib.h>
#define SIZE 640            // �ڴ��ʼ��С
#define MINSIZE 5           // ��Ƭ��Сֵ
enum STATE { Free, Busy };

struct SubAreaNode 
{
	int addr;               // ��ʼ��ַ
	int size;               // ������С
	int taskId;             // ��ҵ��
	STATE state;            // ����״̬
	SubAreaNode* pre;       // ����ǰ��ָ��
	SubAreaNode* nxt;       // ��������ָ��
}SubHead;

// ��ʼ�����з�����
void IntSubArea()
{
	// �����ʼ�����ڴ�
	SubAreaNode* fir = (SubAreaNode*)malloc(sizeof(SubAreaNode));
	// ���׸�������ֵ
	fir->addr = 0;
	fir->size = SIZE;
	fir->state = Free;
	fir->taskId = -1;
	fir->pre = &SubHead;
	fir->nxt = NULL;
	// ��ʼ������ͷ����Ϣ
	SubHead.pre = NULL;
	SubHead.nxt = fir;
}

// �״���Ӧ�㷨
int FirstFit(int taskId, int size)
{
	SubAreaNode* p = SubHead.nxt;
	while (p != NULL)
	{
		if (p->state == Free && p->size >= size) 
		{
			// �ҵ�Ҫ����Ŀ��з���
			if (p->size - size <= MINSIZE) 
			{
				// �������
				p->state = Busy;
				p->taskId = taskId;
			}
			else 
			{
				// �����СΪsize������
				SubAreaNode* node = (SubAreaNode*)malloc(sizeof(SubAreaNode));
				node->addr = p->addr + size;
				node->size = p->size - size;
				node->state = Free;
				node->taskId = -1;
				// �޸ķ������ڵ�ָ��
				node->pre = p;
				node->nxt = p->nxt;
				if (p->nxt != NULL)
				{
					p->nxt->pre = node;
				}
				p->nxt = node;
				// �����������
				p->size = size;
				p->state = Busy;
				p->taskId = taskId;
			}
			printf("�ڴ����ɹ���\n");
			return 1;
		}
		p = p->nxt;
	}
	printf("�Ҳ������ʵ��ڴ����������ʧ��...\n");
	return 0;
}

// �����Ӧ�㷨
int BestFit(int taskId, int size)
{
	SubAreaNode* tar = NULL;
	int tarSize = SIZE + 1;
	SubAreaNode* p = SubHead.nxt;
	while (p != NULL)
	{
		// Ѱ����ѿ�������
		if (p->state == Free && p->size >= size && p->size < tarSize) 
		{
			tar = p;
			tarSize = p->size;
		}
		p = p->nxt;
	}
	if (tar != NULL) 
	{
		// �ҵ�Ҫ����Ŀ��з���
		if (tar->size - size <= MINSIZE) 
		{
			// �������
			tar->state = Busy;
			tar->taskId = taskId;
		}
		else 
		{
			// �����СΪsize������
			SubAreaNode* node = (SubAreaNode* )malloc(sizeof(SubAreaNode));
			node->addr = tar->addr + size;
			node->size = tar->size - size;
			node->state = Free;
			node->taskId = -1;
			// �޸ķ������ڵ�ָ��
			node->pre = tar;
			node->nxt = tar->nxt;
			if (tar->nxt != NULL) 
			{
				tar->nxt->pre = node;
			}
			tar->nxt = node;
			// �����������
			tar->size = size;
			tar->state = Busy;
			tar->taskId = taskId;
		}
		printf("�ڴ����ɹ���\n");
		return 1;
	}
	else 
	{
		// �Ҳ������ʵĿ��з���
		printf("�Ҳ������ʵ��ڴ����������ʧ��...\n");
		return 0;
	}
}

// �����ڴ�
int FreeSubArea(int taskId)
{
	int flag = 0;
	SubAreaNode* p = SubHead.nxt,* pp;
	while (p != NULL)
	{
		if (p->state == Busy && p->taskId == taskId) 
		{
			flag = 1;
			if ((p->pre != &SubHead && p->pre->state == Free)
				&& (p->nxt != NULL && p->nxt->state == Free)) 
			{
				// ���1���ϲ�������������
				// �Ⱥϲ�������
				pp = p;
				p = p->pre;
				p->size += pp->size;
				p->nxt = pp->nxt;
				pp->nxt->pre = p;
				free(pp);
				// ��ϲ�������
				pp = p->nxt;
				p->size += pp->size;
				p->nxt = pp->nxt;
				if (pp->nxt != NULL) 
				{
					pp->nxt->pre = p;
				}
				free(pp);
			}
			else if ((p->pre == &SubHead || p->pre->state == Busy)
				&& (p->nxt != NULL && p->nxt->state == Free)) 
			{
				// ���2��ֻ�ϲ�����ķ���
				pp = p->nxt;
				p->size += pp->size;
				p->state = Free;
				p->taskId = -1;
				p->nxt = pp->nxt;
				if (pp->nxt != NULL) 
				{
					pp->nxt->pre = p;
				}
				free(pp);
			}
			else if ((p->pre != &SubHead && p->pre->state == Free)
				&& (p->nxt == NULL || p->nxt->state == Busy))
			{
				// ���3��ֻ�ϲ�����ķ���
				pp = p;
				p = p->pre;
				p->size += pp->size;
				p->nxt = pp->nxt;
				if (pp->nxt != NULL) 
				{
					pp->nxt->pre = p;
				}
				free(pp);
			}
			else 
			{
				// ���4�����·��������úϲ�
				p->state = Free;
				p->taskId = -1;
			}
		}
		p = p->nxt;
	}
	if (flag == 1) 
	{
		// ���ճɹ�
		printf("�ڴ�������ճɹ�...\n");
		return 1;
	}
	else 
	{
		// �Ҳ���Ŀ����ҵ������ʧ��
		printf("�Ҳ���Ŀ����ҵ���ڴ��������ʧ��...\n");
		return 0;
	}
}

// ��ʾ���з��������
void ShowSubArea()
{
	printf("*********************************************\n");
	printf("**         ��ǰ���ڴ����������£�        **\n");
	printf("*********************************************\n");
	printf("** ��ʼ��ַ | �ռ��С | ����״̬ | ��ҵ�� **\n");
	SubAreaNode* p = SubHead.nxt;
	while (p != NULL)
	{
		printf("**-----------------------------------------**\n");
		printf("**");
		printf("%d  k  |", p->addr);
		printf("%d  k  |", p->size);
		printf("   %s   |", p->state == Free ? "Free" : "Busy");
		if (p->taskId > 0) 
		{
			printf("%d   ", p->taskId);
		}
		else
		{
			printf("        ");
		}
		printf("**\n");
		p = p->nxt;
	}
	printf("*********************************************\n");
}

int main()
{
	int option, ope, taskId, size;
	// ��ʼ�����з�����
	IntSubArea();
	// ѡ������㷨
	while (1)
	{
		printf("��ѡ��Ҫģ��ķ����㷨��0 ��ʾ�״���Ӧ�㷨��1 ��ʾ�����Ӧ�㷨\n");
		scanf_s("%d", &option);
		if (option == 0) 
		{
			printf("��ѡ�����״���Ӧ�㷨����������㷨��ģ��\n");
			break;
		}
		else if (option == 1)
		{
			printf("��ѡ���������Ӧ�㷨����������㷨��ģ��\n");
			break;
		}
		else 
		{
			printf("���������� 0/1\n\n");
		}
	}
	// ģ�⶯̬���������㷨
    while (1)
	{
		printf("\n");
		printf("*********************************************\n");
		printf("**  1: �����ڴ�    2: �����ڴ�    0: �˳�  **\n");
		printf("*********************************************\n");
		scanf_s("%d", &ope);
		if (ope == 0) break;
		if (ope == 1) 
		{
			// ģ������ڴ�
			printf("��������ҵ�ţ� ");
			scanf_s("%d", &taskId);
			printf("��������Ҫ������ڴ��С(KB)�� ");
			scanf_s("%d", &size);
			if (size <= 0) 
			{
				printf("���󣺷����ڴ��С����Ϊ��ֵ\n");
				continue;
			}
			// ���÷����㷨
			if (option == 0) 
			{
				FirstFit(taskId, size);
			}
			else 
			{
				BestFit(taskId, size);
			}
			// ��ʾ���з��������
			ShowSubArea();
		}
		else if (ope == 2) 
		{
			// ģ������ڴ�
			printf("������Ҫ���յ���ҵ�ţ� ");
			scanf_s("%d", &taskId);
			FreeSubArea(taskId);
			// ��ʾ���з��������
			ShowSubArea();
		}
		else 
		{
			printf("���������� 0/1/2\n");
		}
	}
	printf("�����㷨ģ�����\n");
	return 0;
}