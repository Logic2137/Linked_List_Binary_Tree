#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <string.h>
#include <algorithm>
using namespace std;

const char NoInfo = '#';// 如果输入是#，则输入结束
int orderList[100];
char dataSet[6][2] = { {'c', 'f'}, {'a', 'c'}, {'e', 'f'}, {'u', 'w'}, {'t', 'q'}, {'#', '#'}};       //题目中给出来的初始化树的数据
int list[2];


typedef struct ListNode 
{
	char val;
	ListNode* next;
}ListNode;

typedef ListNode* ElementType;    //输入的数据的类型

typedef struct TreeNode* BinTree;
struct TreeNode//二叉树节点
{
	ElementType Data;
	BinTree Left;
	BinTree Right;
	int layer;   //该结点所在的层数
	int order;   //该结点在该层中的顺序
};

typedef struct QueueNode* PtrToNode;//队列中的节点
struct QueueNode
{
	BinTree Data;//指着那块内存
	PtrToNode Next;
};
typedef PtrToNode Position;
typedef struct QNode* PtrToQNode;//队列的头尾数据和队列的长度
struct QNode
{
	Position Front, Rear;
	int Size;
};
typedef PtrToQNode Queue;

Queue Qt;
///
BinTree CreateBinTree();      //初始化链表树
Queue CreateQueue();
void AddQ(Queue Q, BinTree BT, int layer);
BinTree DeleteQ(Queue Q);
void SequenceTraversal(BinTree BT);
char inputSearchLetter();    //输入要查找的字符
void letterSearch(BinTree BT);     //字母查找函数
void inputCreateLetter(vector<char>& letterArray, string letterString); //创建时输入字符
void letterLocation(BinTree BT);
void wordSearch(BinTree BT);
void insertLetter(BinTree BT);
void deleteLetter(BinTree BT);
void duplicateRemoval(BinTree BT);
int menu();
void createNewNode(BinTree BT, char letter);       //当insert时没有发现目标结点时，则在完全二叉树的末尾段加入一个新结点
void checkEndNode(BinTree BT);        //去尾巴的空结点
BinTree findTreeNode(BinTree BT, BinTree findNode);   //找到最后一个空结点的父节点

///


int main()
{
	int choice = 0;
	char SearchLetter;
	memset(orderList, 0, sizeof(orderList));
	BinTree T = CreateBinTree();    //输入一个元素就回一次车，它就会层序给你排好，建立一个完全二叉树
	
	while (choice != 8) {
		cout << "***********************    Linked List Binary Tree System Menu ***********************" << endl;
		cout << "              1.Create A Linked List Binary Tree    (初始化树)                     " << endl;
		cout << "              2.Search A Letter(return a group with three atoms)      (搜索一个字母)              " << endl;
		cout << "              3.Locate A Letter(return a letter)     (定位一个位置，输出字母)       " << endl;
		cout << "              4.Search A Word(return counts of groups and one of solution)      (搜索一个单词)         " << endl;
		cout << "              5.Insert A Letter(input: a, b, x;  to insert a letter)      (插入一个字母)       " << endl;
		cout << "              6.Delete A Letter(input: a, b, x;  to delete a letter)     (删除一个字母)     " << endl;
		cout << "              7.Duplicate Removal(delete same element under a tree node)    (去重)     " << endl;
		cout << "              8.Exit System              " << endl;
		cout << "\nPlease input your choice number: ";
		cin >> choice;
		if (choice == 1)
		{
			cout << "Linked List Binary Tree has been created! Connect as follows:\n";
			SequenceTraversal(T);
		}
		else if (choice == 2)
		{
			letterSearch(T);
		}
		else if (choice == 3)
		{
			letterLocation(T);

		}
		else if (choice == 4)
		{
			wordSearch(T);
		}
		else if (choice == 5)
		{
			insertLetter(T);
			SequenceTraversal(T);
		}
		else if (choice == 6)
		{
			deleteLetter(T);
			SequenceTraversal(T);
		}
		else if (choice == 7)
		{
			duplicateRemoval(T);
			SequenceTraversal(T);
		}
		else if (choice == 8)
		{
			cout << "\nThanks your using! System will be closed.\n";
			break;
		}
		else
		{
			cout << "\nYour input choice number is wrong. Please check your input data!\n";
		}
		
		system("pause");
		system("cls");
	}
	
	return 0;
}


int menu()
{
	int choice;
	cout << "***********************    Linked List Binary Tree System Menu ***********************" << endl;
	cout << "              1.Create A Linked List Binary Tree    (初始化树)                     " << endl;
	cout << "              2.Search A Letter(return a group with three atoms)      (搜索一个字母)              " << endl;
	cout << "              3.Locate A Letter(return a letter)     (定位一个位置，输出字母)       " << endl;
	cout << "              4.Search A Word(return counts of groups and one of solution)      (搜索一个单词)         " << endl;
	cout << "              5.Insert A Letter(input: a, b, x;  to insert a letter)      (插入一个字母)       " << endl;
	cout << "              6.Delete A Letter(input: a, b, x;  to delete a letter)     (删除一个字母)     " << endl;
	cout << "              7.Duplicate Removal(delete same element under a tree node)    (去重)     " << endl;
	cout << "              8.Exit System              " << endl;
	cin >> choice;
	return choice;
}

/********************************************************************************************************
创建思路：用户每输入一个非0（NoInfo）数据，我们都malloc一个QueueNode类型的节点来储存数据，并把存入队列中，
用QueueNode类型的节点来保存数据，并更改Q里面的数据，然后就是把这个数据插入到二叉树里面。再从队列中取出来一
个数据，将接下来输入的两个数据分别同样malloc一个QueueNode类型的节点来存放数据，并把它存入队列中，然后把这
两个数据插入取出的这个节点的左右孩子里面
*********************************************************************************************************/
BinTree CreateBinTree()//创建一个完全二叉树，是全过程的精髓
{
	char Data;
	string letterString;
	BinTree BT, T;
	Qt = CreateQueue();//创建一个空队列
	int i = 0, j = 0;
	char *set;
	vector<char> letterArray;
	ListNode* listPoint;
	//getline(cin, letterString);
	//inputCreateLetter(letterArray, letterString);
	//vector<char>::iterator iter = std::find(letterArray.begin(), letterArray.end(), '#');
	Data = dataSet[i][0];
	set = dataSet[i];
	//scanf_s("%c", &Data);//临时存放数据
	if (Data != NoInfo)//等于0表示输入终结
	{
		BT = (BinTree)malloc(sizeof(struct TreeNode));//为二叉树节点申请一个内存,先插入二叉树
		BT->Data = (ElementType)malloc(sizeof(ElementType));
		//BT->Data->next = (ElementType)malloc(sizeof(ElementType));
		BT->layer = 1;
		orderList[BT->layer] = orderList[BT->layer] + 1;
		BT->order = orderList[BT->layer];
		listPoint = BT->Data;
		while(j < 2)
		{ 
			listPoint->next = (ElementType)malloc(sizeof(ElementType));
			listPoint->next->val = dataSet[i][j++];
			//BT->Data->next->val = Data;
			listPoint->next->next = NULL;
			listPoint = listPoint->next;
		}
		i++;
		j = 0;
		BT->Left = BT->Right = NULL;
		AddQ(Qt, BT, BT->layer);//入队
	}
	else//等于#表示输入终结
		return NULL;
	while (Qt->Size != 0)//如果队列不为空
	{
		T = DeleteQ(Qt);//出队,已经筛选好了指针，可以直接用
		Data = dataSet[i][0];
		//cin >> Data;
		//scanf_s("%c", &Data);
		if (Data == NoInfo)
		{
			T->Left = NULL;
			T->Right = NULL;
			return BT;
		}
		else//为新数据申请内存节点，把节点插入二叉树
		{
			
			T->Left = (BinTree)malloc(sizeof(struct TreeNode));
			T->Left->Data = (ElementType)malloc(sizeof(ElementType));
			//T->Left->Data->next = (ElementType)malloc(sizeof(ElementType));

			listPoint = T->Left->Data;
			while (j < 2)
			{
				listPoint->next = (ElementType)malloc(sizeof(ElementType));
				listPoint->next->val = dataSet[i][j++];
				listPoint->next->next = NULL;
				listPoint = listPoint->next;
			}
			//T->Left->Data->next->val = Data;
			//T->Left->Data->next->next = NULL;
			i++;
			j = 0;
			T->Left->layer = T->layer + 1;
			orderList[T->Left->layer] = orderList[T->Left->layer] + 1;
			T->Left->order = (orderList[T->Left->layer]);
			T->Left->Left = T->Left->Right = NULL;
			AddQ(Qt, T->Left, T->Left->layer);
		}

		Data = dataSet[i][0];
		//cin >> Data;
		//scanf_s("%c", &Data);
		if (Data == NoInfo)
		{
			T->Right = NULL;
			return BT;
		}
		else//为新数据申请内存节点，把节点插入二叉树
		{
			T->Right = (BinTree)malloc(sizeof(struct TreeNode));
			T->Right->Data = (ElementType)malloc(sizeof(ElementType));
			//T->Right->Data->next = (ElementType)malloc(sizeof(ElementType));
			//T->Right->Data->next->val = Data;
			//T->Right->Data->next->next = NULL;
			listPoint = T->Right->Data;
			while (j < 2)
			{
				listPoint->next = (ElementType)malloc(sizeof(ElementType));
				listPoint->next->val = dataSet[i][j++];
				listPoint->next->next = NULL;
				listPoint = listPoint->next;
			}
			i++;
			j = 0;
			T->Right->layer = T->layer + 1;
			orderList[T->Right->layer] = orderList[T->Right->layer] + 1;
			T->Right->order = (orderList[T->Right->layer]);
			T->Right->Left = T->Right->Right = NULL;
			AddQ(Qt, T->Right, T->Right->layer);
		}
	}
	return BT;
}

Queue CreateQueue()//创建一个空队列,里面没有除了头节点外的其他任何节点
{
	Queue Q = (Queue)malloc(sizeof(struct QNode));
	Q->Front = Q->Rear = (Position)malloc(sizeof(struct QueueNode));//刚开始指针都指着头节点，为这个头节点申请了一块内存
	Q->Size = 0;
	Q->Front->Next = NULL;
	Q->Front->Data = NULL;
	return Q;
}
void AddQ(Queue Q, BinTree BT, int layer)
{
	Q->Size++;
	Position Temp = Q->Rear;//先保存好尾节点指针
	Q->Rear = (Position)malloc(sizeof(struct QueueNode));//尾节点指着这块内存
	Q->Rear->Data = BT;//这块内存里面的数据指针指着这个二叉树节点的内存
	Q->Rear->Next = NULL;
	Temp->Next = Q->Rear;//把上一个节点和这一个节点连接起来
}
BinTree DeleteQ(Queue Q)
{
	BinTree BT;
	if (Q->Front->Next == NULL)//如果是空的
		return NULL;//报错
	Q->Size--;//先把长度减一
	Position Temp = Q->Front->Next;//先保存好头节点的Next指针，Q->Front指着头节点
	if (Temp == Q->Rear)
		Q->Rear = Q->Front;//返回头节点
	Q->Front->Next = Temp->Next;//头节点的Next指针往下移，多出来一个节点就是要删除的节点
	BT = Temp->Data;
	free(Temp);//释放队列节点内存
	return BT;
}

void SequenceTraversal(BinTree BT)
{
	BinTree T = BT;
	Queue Q = CreateQueue();//先创建一个队列
	AddQ(Q, BT, BT->layer);//入队
	ListNode* p;
	int num = 0;
	while (1)
	{
		num = 0;
		T = DeleteQ(Q);//出队
		if (T == NULL)
			return;
		else
		{
			if (T->Left != NULL)
			{
				AddQ(Q, T->Left, T->Left->layer);
				if (T->Right != NULL)
					AddQ(Q, T->Right, T->Right->layer);
			}
			p = T->Data;
			p = p->next;
			while (p) {
				num++;
				printf("%c#  layer:%d  order:%d  list_order:%d\n", p->val, T->layer, T->order, num);
				p = p->next;
			}
		}
	}
}

char inputSearchLetter()
{
	char inputLetter;
	cout << "Please input the letter: ";
	cin >> inputLetter;
	return inputLetter;
}

//void inputCreateLetter(vector<char> &letterArray, string letterString)     //创建树时采用的输入
//{
//	char* strc = new char[strlen(letterString.c_str()) + 1];
//	string pattern = "-";
//	strcpy(strc, letterString.c_str());
//	char* temp = strtok(strc, pattern.c_str());
//	while (temp != NULL)
//	{
//		letterArray.push_back(char(temp));
//		temp = strtok(NULL, pattern.c_str());
//	}
//	delete[] strc;
//	return;
//}

void letterSearch(BinTree BT)
{
	char inputLetter = inputSearchLetter();
	ListNode* currentPoint;
	int number = 0;
	BinTree T = BT;
	Queue Q = CreateQueue();//先创建一个队列
	AddQ(Q, BT, BT->layer);//入队
	while (1)
	{
		number = 0;
		T = DeleteQ(Q);//出队
		if (T == NULL)
			return;
		else
		{
			if (T->Left != NULL)
			{
				AddQ(Q, T->Left, T->Left->layer);
				if (T->Right != NULL)
					AddQ(Q, T->Right, T->Right->layer);
			}
			//printf("%c#  layer:%d  order:%d\n", T->Data->next->val, T->layer, T->order);
			currentPoint = T->Data;      //赋值T的头节点
			currentPoint = currentPoint->next;
			while (currentPoint)
			{
				number++;
				if (currentPoint->val == inputLetter)
				{	
					//cout << "layer: " << T->layer << "\torder(layer): " << T->order << "\t\torder(list): " << number << endl;
					cout << "(" << T->layer << ", " << T->order << ", " << number << ")" << endl;
				}
				
				currentPoint = currentPoint->next;
			}
		}
	}
}

void letterLocation(BinTree BT)      //字母定位查询函数
{
	int inputLayer;      //要查找的层数
	int inputOrderOfLayer;    //要查找本层的点
	int inputOrderOfList;      //要查找链表中的第几位
	int flag = 0;    //记录错误信息   0代表无此结点   1代表有此结点但链表中无指定位置的元素   2代表成功查找到元素
	//cout << "Please input the layer: ";
	cin >> inputLayer;
	//cout << "Please inptu the order of this layer: ";
	cin >> inputOrderOfLayer;
	//cout << "Please input the order of this list: ";
	cin >> inputOrderOfList;

	ListNode* currentPoint;
	int number = 0;
	BinTree T = BT;
	Queue Q = CreateQueue();//先创建一个队列
	AddQ(Q, BT, BT->layer);//入队
	while (1)
	{
		number = 0;
		T = DeleteQ(Q);//出队
		if (T == NULL) {
			if (flag == 0)
				//cout << "The layer or order is wrong! Please check out and input again!\n";
				cout << "定位错误！";
			if (flag == 1)
				//cout << "This list don't exist this element!\n";
				cout << "这串链表中没有该定位！";
			return;
		}
			
		else
		{
			if (T->Left != NULL)
			{
				AddQ(Q, T->Left, T->Left->layer);
				if (T->Right != NULL)
					AddQ(Q, T->Right, T->Right->layer);
			}
			//printf("%c#  layer:%d  order:%d\n", T->Data->next->val, T->layer, T->order);
			if (T->layer == inputLayer && T->order == inputOrderOfLayer)
			{
				flag = 1;
				currentPoint = T->Data;      //赋值T的头节点
				currentPoint = currentPoint->next;
				while (currentPoint)
				{
					number++;
					if (number == inputOrderOfList)
					{
						flag = 2;
						cout << "Successed! The value (" << inputLayer << ", " << inputOrderOfLayer << ", " << inputOrderOfList << ") has been find! Result as follows: ";
						cout << "value: " << currentPoint->val << endl;
					}
					currentPoint = currentPoint->next;
				}
			}
		}
	}
}

void wordSearch(BinTree BT)          //查找单词组的解决方案、看一共有多少组搭配方案
{
	string inputWord;
	ListNode* currentPoint;
	int number = 0;
	BinTree T = BT;
	int flag = 0;    //0找不到   1找到了
	int wordSolution[40][4];       //向量、存储单词中每一个字母的首次匹配解决方案  int[0]代表有几种解决方案
	int successfulCount = 0;        //成功匹配到本单词中字母的个数
	int SolutionCount = 1;   //解决方案数量
	int currentIndex = 0;
	memset(wordSolution, 0, sizeof(wordSolution));
	cout << "Please input the word(press enter to end): ";
	cin >> inputWord;

	Queue Q = CreateQueue();//先创建一个队列
	AddQ(Q, BT, BT->layer);//入队
	while (1)
	{
		number = 0;
		T = DeleteQ(Q);//出队
		if (T == NULL)
		{
			for (int i = 0; i < inputWord.size(); i++)
			{
				SolutionCount = SolutionCount * wordSolution[i][0];
			}
			if (SolutionCount != 0)
			{
				//cout << "The count of solution is " << SolutionCount << " groups" << endl;
				cout << "一共有" << SolutionCount << "种解决方案\n";
				for (int i = 0; i < inputWord.size(); i++)
				{
					cout << "letter: " << inputWord.at(i) << "\t(" << wordSolution[i][1] << ", " << wordSolution[i][2] << ", " << wordSolution[i][3] << ")" << endl;
				}
			}
			else
			{
				cout << "一共有" << 0 << "种解决方案\n";
			}
			return;
		}
		else
		{
			if (T->Left != NULL)
			{
				AddQ(Q, T->Left, T->Left->layer);
				if (T->Right != NULL)
					AddQ(Q, T->Right, T->Right->layer);
			}
			

			currentPoint = T->Data;      //赋值T的头节点
			currentPoint = currentPoint->next;
			while (currentPoint)
			{
				number++;
				for (currentIndex = 0; currentIndex < inputWord.size(); currentIndex++) {
					if (currentPoint->val == inputWord.at(currentIndex))
					{
						wordSolution[currentIndex][0] = wordSolution[currentIndex][0] + 1;   //解决方案种类+1
						if (wordSolution[currentIndex][0] == 1)
						{
							successfulCount = successfulCount + 1;     //因为成功找到了单词中的一个字母，计数+1
							wordSolution[currentIndex][1] = T->layer;
							wordSolution[currentIndex][2] = T->order;
							wordSolution[currentIndex][3] = number;
						}
					}
					
				}
				currentPoint = currentPoint->next;
			}
		}
	}
}

void insertLetter(BinTree BT)
{
	int inputLayer;      //要插入的层数
	int inputOrderOfLayer;    //要插入本层的第几个点
	char x;     //要插入的字符
	int flag = 0;    //记录错误信息   0代表无此结点   1代表有此结点但链表中无指定位置的元素   2代表成功查找到元素
	cout << "Please input the data: ";
	//cout << "Please input the layer: ";
	cin >> inputLayer;
	//cout << "Please inptu the order of this layer: ";
	cin >> inputOrderOfLayer;
	//cout << "Please input the char that you want to insert: ";
	cin >> x;

	ListNode* currentPoint;    //存储本次遍历的结点
	ListNode* prePoint;     //存储上一次currentPoint经过的结点
	int number = 0;
	BinTree T = BT;
	Queue Q = CreateQueue();//先创建一个队列
	AddQ(Q, BT, BT->layer);//入队
	while (1)
	{
		number = 0;
		T = DeleteQ(Q);//出队
		if (T == NULL) {
			if (flag == 0) {
				//cout << "Your input layer and order are not exist! Please check out your input data!" << endl;
				createNewNode(BT, x);
			}
			return;
		}

		else
		{
			if (T->Left != NULL)
			{
				AddQ(Q, T->Left, T->Left->layer);
				if (T->Right != NULL)
					AddQ(Q, T->Right, T->Right->layer);
			}
			
			if (T->layer == inputLayer && T->order == inputOrderOfLayer)
			{
				flag = 1;
				currentPoint = T->Data;      //赋值T的头节点
				prePoint = currentPoint;  
				currentPoint = currentPoint->next;
				while (currentPoint)
				{
					number++;
					prePoint = currentPoint;
					currentPoint = currentPoint->next;
				}
				prePoint->next = (ElementType)malloc(sizeof(ElementType));        //这一部分开阔一个新的内存，把x值给存进去
				prePoint = prePoint->next;
				prePoint->val = x;
				prePoint->next = NULL;
			}

		}
	}
}

void createNewNode(BinTree BT, char letter)  //orderList[BT->layer]
{
	BinTree T = BT;
	Queue Q = CreateQueue();//先创建一个队列
	AddQ(Q, BT, BT->layer);//入队
	ListNode* p;
	int num = 0;
	ListNode* listPoint;
	while (1)
	{
		num = 0;
		T = DeleteQ(Q);//出队
		if (T == NULL)
			return;
		else
		{
			if (T->Left != NULL)
			{
				AddQ(Q, T->Left, T->Left->layer);
				if (T->Right != NULL)
					AddQ(Q, T->Right, T->Right->layer);
				else
				{
					T->Right = (BinTree)malloc(sizeof(struct TreeNode));
					T->Right->Data = (ElementType)malloc(sizeof(ElementType));
					listPoint = T->Right->Data;
					T->Right->layer = T->layer + 1;
					orderList[T->Right->layer] = orderList[T->Right->layer] + 1;
					T->Right->order = orderList[T->Right->layer];
					listPoint->next = (ElementType)malloc(sizeof(ElementType));
					listPoint->next->val = letter;
					listPoint->next->next = NULL;
					listPoint = listPoint->next;
					T->Right->Left = NULL;
					T->Right->Right = NULL;
					return;
				}
			}
			else
			{
				T->Left = (BinTree)malloc(sizeof(struct TreeNode));
				T->Left->Data = (ElementType)malloc(sizeof(ElementType));
				listPoint = T->Left->Data;
				T->Left->layer = T->layer + 1;
				orderList[T->Left->layer] = orderList[T->Left->layer] + 1;
				T->Left->order = orderList[T->Left->layer];
				listPoint->next = (ElementType)malloc(sizeof(ElementType));
				listPoint->next->val = letter;
				listPoint->next->next = NULL;
				listPoint = listPoint->next;
				T->Left->Left = NULL;
				T->Left->Right = NULL;
				return;
			}
		}
	}
	return;
}



void deleteLetter(BinTree BT)
{
	int inputLayer;      //要插入的层数
	int inputOrderOfLayer;    //要插入本层的第几个点
	char x;     //要删除的字符
	int flag = 0;    //记录错误信息   0代表无此结点   1代表有此结点但链表中无指定位置的元素   2代表成功查找到元素
	cout << "Please input the layer: ";
	cin >> inputLayer;
	cout << "Please inptu the order of this layer: ";
	cin >> inputOrderOfLayer;
	cout << "Please input the char that you want to delete: ";
	cin >> x;

	ListNode* currentPoint;    //存储本次遍历的结点
	ListNode* prePoint;     //存储上一次currentPoint经过的结点
	int number = 0;
	BinTree T = BT;
	Queue Q = CreateQueue();//先创建一个队列
	AddQ(Q, BT, BT->layer);//入队
	while (1)
	{
		number = 0;
		T = DeleteQ(Q);//出队
		if (T == NULL) {
			if (flag == 0) {
				cout << "Your input layer and order are not exist! Please check out your input data!" << endl;
			}
			else
			{
				checkEndNode(BT);
			}
			return;
		}

		else
		{
			if (T->Left != NULL)
			{
				AddQ(Q, T->Left, T->Left->layer);
				if (T->Right != NULL)
					AddQ(Q, T->Right, T->Right->layer);
			}

			if (T->layer == inputLayer && T->order == inputOrderOfLayer)
			{
				flag = 1;
				currentPoint = T->Data;      //赋值T的头节点
				prePoint = currentPoint;
				currentPoint = currentPoint->next;
				while (currentPoint)
				{
					
					if (currentPoint->val == x)
					{
						ListNode* deletePoint = currentPoint;
						currentPoint = currentPoint->next;
						prePoint->next = currentPoint;
					}
					else
					{
						prePoint = currentPoint;
						currentPoint = currentPoint->next;
					}
				}
			}
		}
	}
}

void checkEndNode(BinTree BT)
{
	BinTree T = BT;
	Queue Q = CreateQueue();//先创建一个队列
	AddQ(Q, BT, BT->layer);//入队
	ListNode* p;
	int num = 0;
	BinTree preT = T, fatherT = T;
	ListNode* listPoint;
	while (1)
	{
		num = 0;
		T = DeleteQ(Q);//出队
		if (T == NULL)
			return;
		else
		{
			if (T->Left != NULL)
			{
				AddQ(Q, T->Left, T->Left->layer);
				preT = T->Left;
				if (T->Right != NULL)
				{
					AddQ(Q, T->Right, T->Right->layer);
					preT = T->Right;
				}
				else
				{
					if (preT->Data->next == NULL)
					{
						fatherT = findTreeNode(BT, preT);
						--orderList[preT->layer];
						if (fatherT->Left == preT)
							fatherT->Left = NULL;
						else
							fatherT->Right = NULL;
					}
					return;
				}
			}
			else
			{
				if (preT->Data->next == NULL)
				{
					fatherT = findTreeNode(BT, preT);
					--orderList[preT->layer];
					if (fatherT->Left == preT)
						fatherT->Left = NULL;
					else
						fatherT->Right = NULL;
				}
				return;
			}
			
		}
	}
	return;
}

BinTree findTreeNode(BinTree BT, BinTree findNode)
{
	BinTree T = BT;
	Queue Q = CreateQueue();//先创建一个队列
	AddQ(Q, BT, BT->layer);//入队
	ListNode* p;
	int num = 0;
	while (1)
	{
		num = 0;
		T = DeleteQ(Q);//出队
		if (T == NULL)
			return NULL;
		else
		{
			if (T->Left != NULL)
			{
				AddQ(Q, T->Left, T->Left->layer);
				if (T->Right != NULL)
					AddQ(Q, T->Right, T->Right->layer);
			}
			//
			if (T->Left == findNode || T->Right == findNode)
				return T;
		}
	}
}

void duplicateRemoval(BinTree BT)
{
	int flag = 0;    //记录错误信息   0代表无此结点   1代表有此结点但链表中无指定位置的元素   2代表成功查找到元素
	char letterList[26];
	ListNode* currentPoint;    //存储本次遍历的结点
	ListNode* prePoint;     //存储上一次currentPoint经过的结点
	int number = 0;
	BinTree T = BT;
	Queue Q = CreateQueue();//先创建一个队列
	AddQ(Q, BT, BT->layer);//入队
	memset(letterList, 0, sizeof(letterList));
	while (1)
	{
		number = 0;
		T = DeleteQ(Q);//出队
		if (T == NULL) {
			cout << "Duplicate Removal Successfully!" << endl;
			return;
		}

		else
		{
			if (T->Left != NULL)
			{
				AddQ(Q, T->Left, T->Left->layer);
				if (T->Right != NULL)
					AddQ(Q, T->Right, T->Right->layer);
			}
			memset(letterList, 0, sizeof(letterList));
			currentPoint = T->Data;      //赋值T的头节点
			prePoint = currentPoint;
			currentPoint = currentPoint->next;
			while (currentPoint)
			{
				letterList[currentPoint->val - 'a'] = letterList[currentPoint->val - 'a'] + 1;
				if (letterList[currentPoint->val - 'a'] >= 2)
				{
					ListNode* deletePoint = currentPoint;
					currentPoint = currentPoint->next;
					prePoint->next = currentPoint;
				}
				else
				{
					prePoint = currentPoint;
					currentPoint = currentPoint->next;
				}
			}
		}
	}
}