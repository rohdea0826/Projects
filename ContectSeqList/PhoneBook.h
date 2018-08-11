#define _CRT_SECURE_NO_WARNINGS 1
#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

#define MAX_NAME	10
#define MAX_SEX		5
#define MAX_TELE	15
#define MAX_ADDR	20

#define MAX 1000

typedef struct PersonInfo
{
	char name[MAX_NAME];
	char sex[MAX_SEX];
	int age;
	char tele[MAX_TELE];
	char addr[MAX_ADDR];
}personInfo,*pInfo;

typedef struct Data
{
	personInfo data[MAX];
	//pInfo data;
	int sz;
}dat,*pdat;

enum
{
	EXIT, ADD, DELET, CHANGE, SEARCH, SORT, SHOW, DELET_ALL
};

int menu();
//Ôö
void add(pdat p);
//É¾
void delet(pdat p);
//¸Ä
void change(pdat p);
//²é
void search(const pdat p);
//ÅÅ
void sort(pdat p);
//ÏÔ
void show(const pdat p);

void delet_all(pdat p);

void start();

