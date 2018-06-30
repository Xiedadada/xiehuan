//Author:xiehuan
//这是关于操作系统中关于动态分配内存的代码，包括最优适应算法，最先适应算法，和回收算法。
#include<stdio.h>
#include<stdlib.h>
#define SIZE 640            // 内存初始大小
#define MINSIZE 5           // 碎片最小值
enum STATE { Free, Busy };

struct SubAreaNode 
{
	int addr;               // 起始地址
	int size;               // 分区大小
	int taskId;             // 作业号
	STATE state;            // 分区状态
	SubAreaNode* pre;       // 分区前向指针
	SubAreaNode* nxt;       // 分区后向指针
}SubHead;

// 初始化空闲分区链
void IntSubArea()
{
	// 分配初始分区内存
	SubAreaNode* fir = (SubAreaNode*)malloc(sizeof(SubAreaNode));
	// 给首个分区赋值
	fir->addr = 0;
	fir->size = SIZE;
	fir->state = Free;
	fir->taskId = -1;
	fir->pre = &SubHead;
	fir->nxt = NULL;
	// 初始化分区头部信息
	SubHead.pre = NULL;
	SubHead.nxt = fir;
}

// 首次适应算法
int FirstFit(int taskId, int size)
{
	SubAreaNode* p = SubHead.nxt;
	while (p != NULL)
	{
		if (p->state == Free && p->size >= size) 
		{
			// 找到要分配的空闲分区
			if (p->size - size <= MINSIZE) 
			{
				// 整块分配
				p->state = Busy;
				p->taskId = taskId;
			}
			else 
			{
				// 分配大小为size的区间
				SubAreaNode* node = (SubAreaNode*)malloc(sizeof(SubAreaNode));
				node->addr = p->addr + size;
				node->size = p->size - size;
				node->state = Free;
				node->taskId = -1;
				// 修改分区链节点指针
				node->pre = p;
				node->nxt = p->nxt;
				if (p->nxt != NULL)
				{
					p->nxt->pre = node;
				}
				p->nxt = node;
				// 分配空闲区间
				p->size = size;
				p->state = Busy;
				p->taskId = taskId;
			}
			printf("内存分配成功！\n");
			return 1;
		}
		p = p->nxt;
	}
	printf("找不到合适的内存分区，分配失败...\n");
	return 0;
}

// 最佳适应算法
int BestFit(int taskId, int size)
{
	SubAreaNode* tar = NULL;
	int tarSize = SIZE + 1;
	SubAreaNode* p = SubHead.nxt;
	while (p != NULL)
	{
		// 寻找最佳空闲区间
		if (p->state == Free && p->size >= size && p->size < tarSize) 
		{
			tar = p;
			tarSize = p->size;
		}
		p = p->nxt;
	}
	if (tar != NULL) 
	{
		// 找到要分配的空闲分区
		if (tar->size - size <= MINSIZE) 
		{
			// 整块分配
			tar->state = Busy;
			tar->taskId = taskId;
		}
		else 
		{
			// 分配大小为size的区间
			SubAreaNode* node = (SubAreaNode* )malloc(sizeof(SubAreaNode));
			node->addr = tar->addr + size;
			node->size = tar->size - size;
			node->state = Free;
			node->taskId = -1;
			// 修改分区链节点指针
			node->pre = tar;
			node->nxt = tar->nxt;
			if (tar->nxt != NULL) 
			{
				tar->nxt->pre = node;
			}
			tar->nxt = node;
			// 分配空闲区间
			tar->size = size;
			tar->state = Busy;
			tar->taskId = taskId;
		}
		printf("内存分配成功！\n");
		return 1;
	}
	else 
	{
		// 找不到合适的空闲分区
		printf("找不到合适的内存分区，分配失败...\n");
		return 0;
	}
}

// 回收内存
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
				// 情况1：合并上下两个分区
				// 先合并上区间
				pp = p;
				p = p->pre;
				p->size += pp->size;
				p->nxt = pp->nxt;
				pp->nxt->pre = p;
				free(pp);
				// 后合并下区间
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
				// 情况2：只合并下面的分区
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
				// 情况3：只合并上面的分区
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
				// 情况4：上下分区均不用合并
				p->state = Free;
				p->taskId = -1;
			}
		}
		p = p->nxt;
	}
	if (flag == 1) 
	{
		// 回收成功
		printf("内存分区回收成功...\n");
		return 1;
	}
	else 
	{
		// 找不到目标作业，回收失败
		printf("找不到目标作业，内存分区回收失败...\n");
		return 0;
	}
}

// 显示空闲分区链情况
void ShowSubArea()
{
	printf("*********************************************\n");
	printf("**         当前的内存分配情况如下：        **\n");
	printf("*********************************************\n");
	printf("** 起始地址 | 空间大小 | 工作状态 | 作业号 **\n");
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
	// 初始化空闲分区链
	IntSubArea();
	// 选择分配算法
	while (1)
	{
		printf("请选择要模拟的分配算法：0 表示首次适应算法，1 表示最佳适应算法\n");
		scanf_s("%d", &option);
		if (option == 0) 
		{
			printf("你选择了首次适应算法，下面进行算法的模拟\n");
			break;
		}
		else if (option == 1)
		{
			printf("你选择了最佳适应算法，下面进行算法的模拟\n");
			break;
		}
		else 
		{
			printf("错误：请输入 0/1\n\n");
		}
	}
	// 模拟动态分区分配算法
    while (1)
	{
		printf("\n");
		printf("*********************************************\n");
		printf("**  1: 分配内存    2: 回收内存    0: 退出  **\n");
		printf("*********************************************\n");
		scanf_s("%d", &ope);
		if (ope == 0) break;
		if (ope == 1) 
		{
			// 模拟分配内存
			printf("请输入作业号： ");
			scanf_s("%d", &taskId);
			printf("请输入需要分配的内存大小(KB)： ");
			scanf_s("%d", &size);
			if (size <= 0) 
			{
				printf("错误：分配内存大小必须为正值\n");
				continue;
			}
			// 调用分配算法
			if (option == 0) 
			{
				FirstFit(taskId, size);
			}
			else 
			{
				BestFit(taskId, size);
			}
			// 显示空闲分区链情况
			ShowSubArea();
		}
		else if (ope == 2) 
		{
			// 模拟回收内存
			printf("请输入要回收的作业号： ");
			scanf_s("%d", &taskId);
			FreeSubArea(taskId);
			// 显示空闲分区链情况
			ShowSubArea();
		}
		else 
		{
			printf("错误：请输入 0/1/2\n");
		}
	}
	printf("分配算法模拟结束\n");
	return 0;
}