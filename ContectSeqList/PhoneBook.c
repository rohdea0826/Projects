#define _CRT_SECURE_NO_WARNINGS 1
#include "PhoneBook.h"


void savefile(pdat p)
{
	int i;
	FILE *fp = fopen("phonebook.dat", "wb");
	if (fp == NULL)
	{
		perror("open file for write");
	}
	for (i = 0; i < p->sz; i++)
	{
		fwrite(p->data + i, sizeof(personInfo), 1, fp);
	}
	fclose(fp);
	fp = NULL;
}

void loadfile(pdat p)
{
	FILE *fp = fopen("phonebook.dat", "rb");
	if (fp == NULL)
	{
		perror("open file for read");
		savefile(p);
	}
	personInfo tmp = { 0 };
	while (fread(&tmp, sizeof(personInfo), 1, fp))
	{
		p->data[p->sz] = tmp;
		p->sz++;
	}
	fclose(fp);
	fp = NULL;
}


int menu()
{
	int input = 0;
	printf("\n***************************\n");
	printf("*** 1.add      2.delet  ***\n");
	printf("*** 3.change   4.serach ***\n");
	printf("*** 5.sort     6.show   ***\n");
	printf("***     7.delet_all     ***\n");
	printf("***     0.exit&save     ***\n");
	printf("***************************\n");
	printf("请输入选择：");
	scanf("%d", &input);
	return input;
}

static void ad(pdat p, int i)
{
	assert(p != NULL);
	printf("请输入姓名：");
	scanf("%s", p->data[i].name);
	printf("请输入性别：");
	scanf("%s", p->data[i].sex);
	printf("请输入年龄：");
	scanf("%d", &(p->data[i].age));
	printf("请输入电话：");
	scanf("%s", p->data[i].tele);
	printf("请输入地址：");
	scanf("%s", p->data[i].addr);
}

static int find(const pdat p)
{
	assert(p != NULL);
	char name[MAX_NAME] = { 0 };
	scanf("%s", name);
	int i = 0;
	for (i = 0; i < p->sz; i++)
	{
		if (strcmp(name, p->data[i].name) == 0)
			return i;
	}
	printf("没有找到！\n");
	return -1;
}

static void print(const pdat p, int i)
{
	assert(p != NULL);
	
	printf("%-10s\t%-5s\t%-d\t%-15s\t%-20s\n",
		p->data[i].name,
		p->data[i].sex,
		p->data[i].age,
		p->data[i].tele,
		p->data[i].addr);
}

void add(pdat p)
{
	assert(p != NULL);
	if ((p->sz) == MAX)
	{
		printf("容量已满，无法增加");
		return;
	}
	ad(p, p->sz);
	p->sz++;
	printf("增加成功！\n");
}


void delet(pdat p)
{
	assert(p != NULL);
	printf("请输入要删除的姓名：");
	int tmp = find(p);
	int i = 0;
	if (tmp != -1)
	{
		for (i = tmp; i < p->sz - 1; i++)
		{
			p->data[i] = p->data[i+1];
		}
		p->sz--;
		printf("删除成功！\n");
	}
}

void change(pdat p)
{
	assert(p != NULL);
	printf("请输入要修改的姓名：");
	int tmp = find(p);
	if (tmp != -1)
	{
		ad(p, tmp);
		printf("修改成功！\n");
	}
		
}

void search(const pdat p)
{
	assert(p != NULL);
	printf("请输入要查找的姓名：");
	int tmp = find(p);
	if (tmp != -1)
	{
		printf("%-10s\t%-5s\t%-s\t%-15s\t%-20s\n", "姓名", "性别", "年龄", "电话", "地址");
		print(p, tmp);
	}
		
}

void sort(pdat p)
{
	assert(p != NULL);
	int i, j, flag;
	for (i = 0; i < p->sz - 1; i++)
	{
		flag = 0;
		for (j = 0; j < p->sz - 1 - i; j++)
		{
			if (strcmp(p->data[j].name, p->data[j + 1].name) > 0)
			{
				personInfo tmp = p->data[j];
				p->data[j] = p->data[j + 1];
				p->data[j + 1] = tmp;
				flag = 1;
			}
		}
		if (flag == 0)
		{
			printf("排序成功！\n");
			show(p);
			return;
		}
	}
	printf("排序成功！\n");
	show(p);
}

void show(const pdat p)
{
	assert(p != NULL);
	int i;
	printf("%-10s\t%-5s\t%-s\t%-15s\t%-20s\n", "姓名", "性别", "年龄", "电话", "地址");
	for (i = 0; i < p->sz; i++)
	{
		print(p,i);
	}
}

void delet_all(pdat p)
{
	p->sz = 0;
}

void start()
{
	dat phonebook;
	phonebook.sz = 0;
	memset(phonebook.data, '0', sizeof(phonebook.data));
	//phonebook.data = NULL;
	loadfile(&phonebook);
	do
	{
		switch (menu())
		{
		case EXIT:
			savefile(&phonebook);
			exit(EXIT_SUCCESS);
		case ADD:
			add(&phonebook);
			break;
		case DELET:
			delet(&phonebook);
			break;
		case CHANGE:
			change(&phonebook);
			break;
		case SEARCH:
			search(&phonebook);
			break;
		case SORT:
			sort(&phonebook);
			break;
		case SHOW:
			show(&phonebook);
			break;
		case DELET_ALL:
			delet_all(&phonebook);
			break;
		default:
			printf("输入错误");
			break;
		}
	} while (1);
}