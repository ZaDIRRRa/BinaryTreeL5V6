//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                    ����� ���� ���� � ��������� ��� ���������� ���������                                                      */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                                                                                                                                //
																																								//
#define _CRT_SECURE_NO_WARNINGS																																	//
#include "Header.h"				     																															//
																																								//
#include <stdio.h>				     																															//
#include <stdbool.h>																																			//
#include <string.h>				    																															// 
#include <stdlib.h>			     																																//
#include <io.h>							            																											//
#include <time.h>																	     																		//
																																								//
extern FILE* f_ptr;																																				//
                                                                                                                                                                //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                        ����� ��������� ������� ��� ����� � ����������                                                        */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char* GetStr(int a)                                                                                      //������� ����� ������ ��������
{
	char buf[LENGTH] = "";
	int len = 0;
	char* string = (char*)malloc(sizeof(char));


	if (!string)
	{

		printf("There's not enough memory!");
		return NULL;

	}

	if (a == 0)
		scanf_s("%*c");
	*string = '\0';


	int n = 0;

	do
	{

		n = scanf_s("%127[^\n]", buf, LENGTH);

		if (n > 0)
		{

			len += strlen(buf);
			char* tmp_ptr = (char*)realloc(string, len + 1);

			if (!tmp_ptr)
			{
				printf("It's not enough memory!");
				free(string);
				return NULL;
			}

			string = tmp_ptr;

			strcat(string, buf);
		}
		else if (n == 0)
			scanf_s("%*c");
		else
		{
			free(string);
			return NULL;
		}
	} while (n > 0);

	return string;
}

int AddNewBranch(struct ElementOfTree** trunk, char* key, char* str)                                     //������� ���������� ������ �������� �����
{
	//struct ElementOfTree** Branch = *trunk;

	if (*trunk == NULL)
	{
		struct ElementOfTree* Leaf = (struct ElementOfTree*)malloc(sizeof(struct ElementOfTree));

		if (!Leaf)
			return 0;

		Leaf->Parent = NULL;
		Leaf->Left = NULL;
		Leaf->Right = NULL;
		Leaf->key = key;
		Leaf->str = str;

		*trunk = Leaf;
	}
	else
	{
		struct ElementOfTree* parent = *trunk;

		if (!parent)
			return 0;

		struct ElementOfTree* tmp = *trunk;
		struct ElementOfTree** pmt = NULL;

		if (!tmp)
			return 0;

		while (tmp != NULL)
		{
			parent = tmp;

			if (FromTo(&tmp->key) == FromTo(&key))
			{
				printf("\nSorry, but an element with this key has already been added\n");
				return 0;
			}

			else if (FromTo(&tmp->key) > FromTo(&key))
				pmt = &tmp->Left, tmp = tmp->Left;

			else
				pmt = &tmp->Right, tmp = tmp->Right;

		}

		*pmt = (struct ElementOfTree*)malloc(sizeof(struct ElementOfTree));
		(*pmt)->Parent = parent;
		(*pmt)->Right = NULL;
		(*pmt)->Left = NULL;
		(*pmt)->key = key;
		(*pmt)->str = str;
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                                           ******                                                                             */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                          ����� ��������� ������� ��� ������ � ������                                                         */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Load(char* path, struct ElementOfTree** trunk)                                                     //������� ��� �������� ����� � ������ ��������� ����������� ����� �� ������ �� ����� 
{
	if (_access(path, 0))
	{
		printf("Creating the file with the name \"%s\"...\n", path);

		*trunk = NULL;
	}

	else
		printf("Loading the file with the name \"%s\"...\n", path);

	printf("\n");

	fopen_s(&f_ptr, path, "rb");

	if (!f_ptr)
		return;

	size_t sizeKey = 0;
	size_t sizeStr = 0;

	int a = fscanf_s(f_ptr, "%zu ", &sizeKey);
	if (a == -1)
		return;

	*trunk = (struct ElementOfTree*)malloc(sizeof(struct ElementOfTree));

	if (!(*trunk))
		return;

	

	(*trunk)->key = (char*)calloc(sizeKey + 1, sizeof(char));

	if (!(*trunk)->key)
		return;

	fgets((*trunk)->key, sizeKey + 1, f_ptr);

	(*trunk)->Parent = NULL;

	fscanf_s(f_ptr, "%zu ", &sizeStr);

	(*trunk)->str = (char*)calloc(sizeStr + 1, sizeof(char));

	if (!(*trunk)->str)
		return;

	(*trunk)->Left = NULL;

	(*trunk)->Right = NULL;

	fgets((*trunk)->str, sizeStr + 1, f_ptr);

	LoadingAdditional(*trunk, (*trunk)->Parent);

	fclose(f_ptr);
}                                                   //������� ������������ �������� ��

void LoadingAdditional(struct ElementOfTree* Branch)                                                   //�������� ����������� ����� �� ������ �� �����
{
	char* key = (char*)malloc(sizeof(char));

	if (!key)
		return;

	char* str = (char*)malloc(sizeof(char));

	if (!str)
		return;

	size_t sizeKey = 0;
	size_t sizeStr = 0;


	while (fscanf_s(f_ptr, "%zu \n", &sizeKey) > 0)                                      
	{
		char* key = (char*)malloc(sizeof(char));

		if (!key)
			return;

		char* str = (char*)malloc(sizeof(char));

		if (!str)
			return;

		fgets(key, sizeKey + 1, f_ptr);
		fscanf_s(f_ptr, "%zu \n", &sizeStr);
		fgets(str, sizeStr + 1, f_ptr);

		AddNewBranch(&Branch, key, str);
	}
}

void WritingData(struct ElementOfTree* Branch)                                                          //�������� ��� ������ � ���� ������ �� ��������� ����� 
{
	if (Branch == NULL)
		return;

	else
		fprintf(f_ptr, "%zu %s %zu %s\n", strlen(Branch->key), Branch->key, strlen(Branch->str), Branch->str);

	WritingData(Branch->Left);

	WritingData(Branch->Right);
}

void Save(char* path, struct ElementOfTree* Branch)                                                      //������� ������������ �������� �� ������ ������ �� ��������� ����� ����
{
	fopen_s(&f_ptr, path, "w");

	if (!f_ptr)
		return;

	WritingData(Branch);

	fclose(f_ptr);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                                           ******                                                                             */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                           ����� ��������� ������� ��� ������������                                                           */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PrintMenu(void)                                                                                 //������� ��� ������ ���� �� �����
{
	char* menu[OPTIONS] =
	{
		"0 - Exit",
		"1 - Add",
		"2 - Timing",
		"3 - Erase by key",
		"4 - Search by Key",
		"5 - Print by Key",
		"6 - Print all",
		"7 - Show options"
	};

	for (int i = 0; i < OPTIONS; ++i)
		printf("%s\n", menu[i]);

	printf("\n");
}

void Printing(struct ElementOfTree* trunk)                                                            //������� ��� ������ ��������� ����� �� �����
{
	if (trunk == NULL)
		return;

	Printing(trunk->Left);
	Printing(trunk->Right);
	if (trunk->key)
		printf("\n\nKey:%s\nString:%s", trunk->key, trunk->str);
}

void PrintOne(const struct ElementOfTree* Branch, const int key)                                     //������� ��� ������ ����������� �������� ����� �� �����
{
	if (Branch == NULL)
		return;
	if (Branch->key)
		printf("\n\nThe element is found\b\bKey:%s\nString:%s", Branch->key, Branch->str);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                                           ******                                                                             */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                            ����� ��������� ��������������� �������                                                           */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int FromTo(char** key)                                                                              //��������������� ������� �� �������� �������� �� ������� ASCII � �������� ����������
{
	char buf[LENGTH] = "";
	strcat(&buf, *key);
	
	int len = strlen(buf);
	int Res = 0;

	for (int i = 0; i < len; i++)
	{
		Res += buf[i];                                  
	}

	return Res;
}

struct ElementOfTree* SearchNode(const struct ElementOfTree* Branch, char* key)                        //��������������� ������� ��� ������ ����������� �������� �����
{
	if (Branch == NULL)
		return NULL;
	if (Branch->key  == key)
		return Branch;

	if (FromTo(&Branch->key) > FromTo(&key))
		return SearchNode(Branch->Left, key);

	else if (FromTo(&Branch->key) < FromTo(&key))
		return SearchNode(Branch->Right, key);

	else
		return Branch;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                                           ******                                                                             */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                          ����� ��������� ������� ��� �������� �����                                                          */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DeleteNode(struct ElementOfTree** ptr, struct ElementOfTree* tmp)                                 //������� ��� �������� �������� �����
{
	struct ElementOfTree* m = tmp->Parent;

	if (!tmp->Left && !tmp->Right)
	{
		if (m == NULL)
		{
			*ptr = NULL;

			free(*ptr);

			return;
		}

		else if (tmp == m->Left)
		{
			tmp->Parent->Left = NULL;

			free(tmp);

			return;
		}

		else
		{
			tmp->Parent->Right = NULL;

			free(tmp);

			return;
		}
	}

	else if (!tmp->Left && tmp->Right) // ������ ��������� 
	{
		if (!m)
		{
			*ptr = tmp->Right;

			free((*ptr)->Parent);

			(*ptr)->Parent = NULL;

			return;
		}

		if (tmp == m->Right)// � ������ �������
		{
			struct ElementOfTree* max = MaxElement(tmp);

			struct ElementOfTree* min = MinElement(max);

			m->Right = max;

			if (tmp->Right != max)
			{
				max->Parent->Right = NULL;

				max->Parent = m;

				min->Left = tmp->Right;

				tmp->Right->Parent = min;
			}

			else
				max->Parent = m;

			free(tmp);

			tmp = NULL;

			return;
		}

		else// � ����� �������
		{

			struct ElementOfTree* max = MaxElement(tmp);

			struct ElementOfTree* min = MinElement(max);

			m->Left = max;

			if (tmp->Right != max)
			{
				max->Parent->Right = NULL;

				max->Parent = m;

				min->Left = tmp->Right;

				tmp->Right->Parent = min;
			}

			else
				max->Parent = m;

			free(tmp);

			tmp = NULL;

			return;
		}
	}

	else if (tmp->Left && !tmp->Right) // ����� ���������
	{
		if (!m)
		{
			*ptr = tmp->Left;

			free((*ptr)->Parent);

			(*ptr)->Parent = NULL;

			return;
		}

		if (tmp == m->Right)// � ������ �������
		{
			struct ElementOfTree* max = MaxElement(tmp);

			struct ElementOfTree* min = MinElement(max);

			m->Right = max;

			if (tmp->Left != max)
			{
				max->Parent->Right = NULL;

				max->Parent = m;

				min->Left = tmp->Left;

				tmp->Left->Parent = min;
			}

			else
				max->Parent = m;

			free(tmp);

			tmp = NULL;

			return;
		}

		else// � ����� �������
		{
			struct ElementOfTree* max = MaxElement(tmp);

			struct ElementOfTree* min = MinElement(max);

			m->Left = max;

			if (tmp->Left != max)
			{
				max->Parent->Right = NULL;

				max->Parent = m;

				min->Left = tmp->Left;

				tmp->Left->Parent = min;
			}

			else
				max->Parent = m;

			free(tmp);

			tmp = NULL;

			return;
		}
	}

	else // � ����� � ������ ����������
	{
		if (!m)
		{
			*ptr = tmp->Right;

			(*ptr)->Left = (*ptr)->Parent->Left;

			(*ptr)->Left->Parent = *ptr;

			free((*ptr)->Parent);

			(*ptr)->Parent = NULL;

			return;
		}

		struct ElementOfTree* max = MaxElement(tmp);

		struct ElementOfTree* min = MinElement(max);

		if (tmp == m->Right)
			m->Right = max;

		else
			m->Left = max;

		if (tmp->Right != max)
		{
			max->Parent->Right = NULL;

			max->Parent = m;

			tmp->Left->Parent = tmp->Right;

			tmp->Right->Left = tmp->Left;

			min->Left = tmp->Right;

			tmp->Right->Parent = min;
		}

		else
		{
			max->Parent = m;

			min->Left = tmp->Left;

			tmp->Left->Parent = min;
		}

		free(tmp);

		tmp = NULL;

		return;
	}
}

struct ElementOfTree* MaxElement(struct ElementOfTree* ptr)                                            //��������������� ������� �� ���������� ������������� �������� ����� ����������� �� ���� 
{
	struct ElementOfTree* max = ptr;

	if (max->Right)

		while (max->Right)
			max = max->Right;

	else
	{
		if (max->Left->Right)
		{
			max = max->Left;

			while (max->Right)
				max = max->Right;
		}

		else
			max = max->Left;
	}

	return max;
}

struct ElementOfTree* MinElement(struct ElementOfTree* ptr)                                            //��������������� ������� �� ���������� ������������ �������� ����� ����������� �� ���� 
{
	struct ElementOfTree* min = ptr;

	while (min->Left)
		min = min->Left;

	return min;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                                           ******                                                                             */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                           ����� ��������� ������� ��� ������������                                                           */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                                           ******                                                                             */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                           ����� ��������� ������� ��� ������������                                                           */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Timing()
{

	int n = 16,
		cnt1 = 1000,
		cnt2 = cnt1;
	char* k;

	while (n--)
	{
		struct ElementOfTree* root = NULL;

		char* str = "ADjonoi4pipifkcmk";
		int a = 5000;
		char* key = "5000";
		AddNewBranch(&root, key, str);

		int count_branch = cnt2;
		clock_t first, last;

		first = clock();

		while (cnt2--)
		{
			k = "5000";
			if ((AddNewBranch(&root, k, str) != 1))
				break;
			k = "4000";
			if ((AddNewBranch(&root, k, str) != 1))
				break;
			cnt2--;
		}

		//struct ElementOfTree* tmp = MaxElement(root);
		//tmp = MinElement(root);
		//�ounting(root, key);
		last = clock();

		printf("Test number %u\n", 16 - n);
		printf("Count of Branches: %u\n", 1000 * (16 - n));
		printf("Time = %d\n", last - first);
		printf("\n");
		if ((16 - n) < 10)
			DelTree(&root);

		cnt1 += 1000;
		cnt2 = cnt1;
	}
	return 1;

}

void DelTree(struct ElementOfTree** root)
{
	if (*root)
	{
		DelTree(&(*root)->Left);

		DelTree(&(*root)->Right);

		free(*root);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                                           ******                                                                             */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                                           ******                                                                             */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////