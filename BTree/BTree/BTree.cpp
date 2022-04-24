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

const char NoInfo = '#';// ���������#�����������
int orderList[100];
char dataSet[6][2] = { {'c', 'f'}, {'a', 'c'}, {'e', 'f'}, {'u', 'w'}, {'t', 'q'}, {'#', '#'}};       //��Ŀ�и������ĳ�ʼ����������
int list[2];


typedef struct ListNode 
{
	char val;
	ListNode* next;
}ListNode;

typedef ListNode* ElementType;    //��������ݵ�����

typedef struct TreeNode* BinTree;
struct TreeNode//�������ڵ�
{
	ElementType Data;
	BinTree Left;
	BinTree Right;
	int layer;   //�ý�����ڵĲ���
	int order;   //�ý���ڸò��е�˳��
};

typedef struct QueueNode* PtrToNode;//�����еĽڵ�
struct QueueNode
{
	BinTree Data;//ָ���ǿ��ڴ�
	PtrToNode Next;
};
typedef PtrToNode Position;
typedef struct QNode* PtrToQNode;//���е�ͷβ���ݺͶ��еĳ���
struct QNode
{
	Position Front, Rear;
	int Size;
};
typedef PtrToQNode Queue;

Queue Qt;
///
BinTree CreateBinTree();      //��ʼ��������
Queue CreateQueue();
void AddQ(Queue Q, BinTree BT, int layer);
BinTree DeleteQ(Queue Q);
void SequenceTraversal(BinTree BT);
char inputSearchLetter();    //����Ҫ���ҵ��ַ�
void letterSearch(BinTree BT);     //��ĸ���Һ���
void inputCreateLetter(vector<char>& letterArray, string letterString); //����ʱ�����ַ�
void letterLocation(BinTree BT);
void wordSearch(BinTree BT);
void insertLetter(BinTree BT);
void deleteLetter(BinTree BT);
void duplicateRemoval(BinTree BT);
int menu();
void createNewNode(BinTree BT, char letter);       //��insertʱû�з���Ŀ����ʱ��������ȫ��������ĩβ�μ���һ���½��
void checkEndNode(BinTree BT);        //ȥβ�͵Ŀս��
BinTree findTreeNode(BinTree BT, BinTree findNode);   //�ҵ����һ���ս��ĸ��ڵ�

///


int main()
{
	int choice = 0;
	char SearchLetter;
	memset(orderList, 0, sizeof(orderList));
	BinTree T = CreateBinTree();    //����һ��Ԫ�ؾͻ�һ�γ������ͻ��������źã�����һ����ȫ������
	
	while (choice != 8) {
		cout << "***********************    Linked List Binary Tree System Menu ***********************" << endl;
		cout << "              1.Create A Linked List Binary Tree    (��ʼ����)                     " << endl;
		cout << "              2.Search A Letter(return a group with three atoms)      (����һ����ĸ)              " << endl;
		cout << "              3.Locate A Letter(return a letter)     (��λһ��λ�ã������ĸ)       " << endl;
		cout << "              4.Search A Word(return counts of groups and one of solution)      (����һ������)         " << endl;
		cout << "              5.Insert A Letter(input: a, b, x;  to insert a letter)      (����һ����ĸ)       " << endl;
		cout << "              6.Delete A Letter(input: a, b, x;  to delete a letter)     (ɾ��һ����ĸ)     " << endl;
		cout << "              7.Duplicate Removal(delete same element under a tree node)    (ȥ��)     " << endl;
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
	cout << "              1.Create A Linked List Binary Tree    (��ʼ����)                     " << endl;
	cout << "              2.Search A Letter(return a group with three atoms)      (����һ����ĸ)              " << endl;
	cout << "              3.Locate A Letter(return a letter)     (��λһ��λ�ã������ĸ)       " << endl;
	cout << "              4.Search A Word(return counts of groups and one of solution)      (����һ������)         " << endl;
	cout << "              5.Insert A Letter(input: a, b, x;  to insert a letter)      (����һ����ĸ)       " << endl;
	cout << "              6.Delete A Letter(input: a, b, x;  to delete a letter)     (ɾ��һ����ĸ)     " << endl;
	cout << "              7.Duplicate Removal(delete same element under a tree node)    (ȥ��)     " << endl;
	cout << "              8.Exit System              " << endl;
	cin >> choice;
	return choice;
}

/********************************************************************************************************
����˼·���û�ÿ����һ����0��NoInfo�����ݣ����Ƕ�mallocһ��QueueNode���͵Ľڵ����������ݣ����Ѵ�������У�
��QueueNode���͵Ľڵ����������ݣ�������Q��������ݣ�Ȼ����ǰ�������ݲ��뵽���������档�ٴӶ�����ȡ����һ
�����ݣ���������������������ݷֱ�ͬ��mallocһ��QueueNode���͵Ľڵ���������ݣ���������������У�Ȼ�����
�������ݲ���ȡ��������ڵ�����Һ�������
*********************************************************************************************************/
BinTree CreateBinTree()//����һ����ȫ����������ȫ���̵ľ���
{
	char Data;
	string letterString;
	BinTree BT, T;
	Qt = CreateQueue();//����һ���ն���
	int i = 0, j = 0;
	char *set;
	vector<char> letterArray;
	ListNode* listPoint;
	//getline(cin, letterString);
	//inputCreateLetter(letterArray, letterString);
	//vector<char>::iterator iter = std::find(letterArray.begin(), letterArray.end(), '#');
	Data = dataSet[i][0];
	set = dataSet[i];
	//scanf_s("%c", &Data);//��ʱ�������
	if (Data != NoInfo)//����0��ʾ�����ս�
	{
		BT = (BinTree)malloc(sizeof(struct TreeNode));//Ϊ�������ڵ�����һ���ڴ�,�Ȳ��������
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
		AddQ(Qt, BT, BT->layer);//���
	}
	else//����#��ʾ�����ս�
		return NULL;
	while (Qt->Size != 0)//������в�Ϊ��
	{
		T = DeleteQ(Qt);//����,�Ѿ�ɸѡ����ָ�룬����ֱ����
		Data = dataSet[i][0];
		//cin >> Data;
		//scanf_s("%c", &Data);
		if (Data == NoInfo)
		{
			T->Left = NULL;
			T->Right = NULL;
			return BT;
		}
		else//Ϊ�����������ڴ�ڵ㣬�ѽڵ���������
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
		else//Ϊ�����������ڴ�ڵ㣬�ѽڵ���������
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

Queue CreateQueue()//����һ���ն���,����û�г���ͷ�ڵ���������κνڵ�
{
	Queue Q = (Queue)malloc(sizeof(struct QNode));
	Q->Front = Q->Rear = (Position)malloc(sizeof(struct QueueNode));//�տ�ʼָ�붼ָ��ͷ�ڵ㣬Ϊ���ͷ�ڵ�������һ���ڴ�
	Q->Size = 0;
	Q->Front->Next = NULL;
	Q->Front->Data = NULL;
	return Q;
}
void AddQ(Queue Q, BinTree BT, int layer)
{
	Q->Size++;
	Position Temp = Q->Rear;//�ȱ����β�ڵ�ָ��
	Q->Rear = (Position)malloc(sizeof(struct QueueNode));//β�ڵ�ָ������ڴ�
	Q->Rear->Data = BT;//����ڴ����������ָ��ָ������������ڵ���ڴ�
	Q->Rear->Next = NULL;
	Temp->Next = Q->Rear;//����һ���ڵ����һ���ڵ���������
}
BinTree DeleteQ(Queue Q)
{
	BinTree BT;
	if (Q->Front->Next == NULL)//����ǿյ�
		return NULL;//����
	Q->Size--;//�Ȱѳ��ȼ�һ
	Position Temp = Q->Front->Next;//�ȱ����ͷ�ڵ��Nextָ�룬Q->Frontָ��ͷ�ڵ�
	if (Temp == Q->Rear)
		Q->Rear = Q->Front;//����ͷ�ڵ�
	Q->Front->Next = Temp->Next;//ͷ�ڵ��Nextָ�������ƣ������һ���ڵ����Ҫɾ���Ľڵ�
	BT = Temp->Data;
	free(Temp);//�ͷŶ��нڵ��ڴ�
	return BT;
}

void SequenceTraversal(BinTree BT)
{
	BinTree T = BT;
	Queue Q = CreateQueue();//�ȴ���һ������
	AddQ(Q, BT, BT->layer);//���
	ListNode* p;
	int num = 0;
	while (1)
	{
		num = 0;
		T = DeleteQ(Q);//����
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

//void inputCreateLetter(vector<char> &letterArray, string letterString)     //������ʱ���õ�����
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
	Queue Q = CreateQueue();//�ȴ���һ������
	AddQ(Q, BT, BT->layer);//���
	while (1)
	{
		number = 0;
		T = DeleteQ(Q);//����
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
			currentPoint = T->Data;      //��ֵT��ͷ�ڵ�
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

void letterLocation(BinTree BT)      //��ĸ��λ��ѯ����
{
	int inputLayer;      //Ҫ���ҵĲ���
	int inputOrderOfLayer;    //Ҫ���ұ���ĵ�
	int inputOrderOfList;      //Ҫ���������еĵڼ�λ
	int flag = 0;    //��¼������Ϣ   0�����޴˽��   1�����д˽�㵫��������ָ��λ�õ�Ԫ��   2����ɹ����ҵ�Ԫ��
	//cout << "Please input the layer: ";
	cin >> inputLayer;
	//cout << "Please inptu the order of this layer: ";
	cin >> inputOrderOfLayer;
	//cout << "Please input the order of this list: ";
	cin >> inputOrderOfList;

	ListNode* currentPoint;
	int number = 0;
	BinTree T = BT;
	Queue Q = CreateQueue();//�ȴ���һ������
	AddQ(Q, BT, BT->layer);//���
	while (1)
	{
		number = 0;
		T = DeleteQ(Q);//����
		if (T == NULL) {
			if (flag == 0)
				//cout << "The layer or order is wrong! Please check out and input again!\n";
				cout << "��λ����";
			if (flag == 1)
				//cout << "This list don't exist this element!\n";
				cout << "�⴮������û�иö�λ��";
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
				currentPoint = T->Data;      //��ֵT��ͷ�ڵ�
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

void wordSearch(BinTree BT)          //���ҵ�����Ľ����������һ���ж�������䷽��
{
	string inputWord;
	ListNode* currentPoint;
	int number = 0;
	BinTree T = BT;
	int flag = 0;    //0�Ҳ���   1�ҵ���
	int wordSolution[40][4];       //�������洢������ÿһ����ĸ���״�ƥ��������  int[0]�����м��ֽ������
	int successfulCount = 0;        //�ɹ�ƥ�䵽����������ĸ�ĸ���
	int SolutionCount = 1;   //�����������
	int currentIndex = 0;
	memset(wordSolution, 0, sizeof(wordSolution));
	cout << "Please input the word(press enter to end): ";
	cin >> inputWord;

	Queue Q = CreateQueue();//�ȴ���һ������
	AddQ(Q, BT, BT->layer);//���
	while (1)
	{
		number = 0;
		T = DeleteQ(Q);//����
		if (T == NULL)
		{
			for (int i = 0; i < inputWord.size(); i++)
			{
				SolutionCount = SolutionCount * wordSolution[i][0];
			}
			if (SolutionCount != 0)
			{
				//cout << "The count of solution is " << SolutionCount << " groups" << endl;
				cout << "һ����" << SolutionCount << "�ֽ������\n";
				for (int i = 0; i < inputWord.size(); i++)
				{
					cout << "letter: " << inputWord.at(i) << "\t(" << wordSolution[i][1] << ", " << wordSolution[i][2] << ", " << wordSolution[i][3] << ")" << endl;
				}
			}
			else
			{
				cout << "һ����" << 0 << "�ֽ������\n";
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
			

			currentPoint = T->Data;      //��ֵT��ͷ�ڵ�
			currentPoint = currentPoint->next;
			while (currentPoint)
			{
				number++;
				for (currentIndex = 0; currentIndex < inputWord.size(); currentIndex++) {
					if (currentPoint->val == inputWord.at(currentIndex))
					{
						wordSolution[currentIndex][0] = wordSolution[currentIndex][0] + 1;   //�����������+1
						if (wordSolution[currentIndex][0] == 1)
						{
							successfulCount = successfulCount + 1;     //��Ϊ�ɹ��ҵ��˵����е�һ����ĸ������+1
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
	int inputLayer;      //Ҫ����Ĳ���
	int inputOrderOfLayer;    //Ҫ���뱾��ĵڼ�����
	char x;     //Ҫ������ַ�
	int flag = 0;    //��¼������Ϣ   0�����޴˽��   1�����д˽�㵫��������ָ��λ�õ�Ԫ��   2����ɹ����ҵ�Ԫ��
	cout << "Please input the data: ";
	//cout << "Please input the layer: ";
	cin >> inputLayer;
	//cout << "Please inptu the order of this layer: ";
	cin >> inputOrderOfLayer;
	//cout << "Please input the char that you want to insert: ";
	cin >> x;

	ListNode* currentPoint;    //�洢���α����Ľ��
	ListNode* prePoint;     //�洢��һ��currentPoint�����Ľ��
	int number = 0;
	BinTree T = BT;
	Queue Q = CreateQueue();//�ȴ���һ������
	AddQ(Q, BT, BT->layer);//���
	while (1)
	{
		number = 0;
		T = DeleteQ(Q);//����
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
				currentPoint = T->Data;      //��ֵT��ͷ�ڵ�
				prePoint = currentPoint;  
				currentPoint = currentPoint->next;
				while (currentPoint)
				{
					number++;
					prePoint = currentPoint;
					currentPoint = currentPoint->next;
				}
				prePoint->next = (ElementType)malloc(sizeof(ElementType));        //��һ���ֿ���һ���µ��ڴ棬��xֵ�����ȥ
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
	Queue Q = CreateQueue();//�ȴ���һ������
	AddQ(Q, BT, BT->layer);//���
	ListNode* p;
	int num = 0;
	ListNode* listPoint;
	while (1)
	{
		num = 0;
		T = DeleteQ(Q);//����
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
	int inputLayer;      //Ҫ����Ĳ���
	int inputOrderOfLayer;    //Ҫ���뱾��ĵڼ�����
	char x;     //Ҫɾ�����ַ�
	int flag = 0;    //��¼������Ϣ   0�����޴˽��   1�����д˽�㵫��������ָ��λ�õ�Ԫ��   2����ɹ����ҵ�Ԫ��
	cout << "Please input the layer: ";
	cin >> inputLayer;
	cout << "Please inptu the order of this layer: ";
	cin >> inputOrderOfLayer;
	cout << "Please input the char that you want to delete: ";
	cin >> x;

	ListNode* currentPoint;    //�洢���α����Ľ��
	ListNode* prePoint;     //�洢��һ��currentPoint�����Ľ��
	int number = 0;
	BinTree T = BT;
	Queue Q = CreateQueue();//�ȴ���һ������
	AddQ(Q, BT, BT->layer);//���
	while (1)
	{
		number = 0;
		T = DeleteQ(Q);//����
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
				currentPoint = T->Data;      //��ֵT��ͷ�ڵ�
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
	Queue Q = CreateQueue();//�ȴ���һ������
	AddQ(Q, BT, BT->layer);//���
	ListNode* p;
	int num = 0;
	BinTree preT = T, fatherT = T;
	ListNode* listPoint;
	while (1)
	{
		num = 0;
		T = DeleteQ(Q);//����
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
	Queue Q = CreateQueue();//�ȴ���һ������
	AddQ(Q, BT, BT->layer);//���
	ListNode* p;
	int num = 0;
	while (1)
	{
		num = 0;
		T = DeleteQ(Q);//����
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
	int flag = 0;    //��¼������Ϣ   0�����޴˽��   1�����д˽�㵫��������ָ��λ�õ�Ԫ��   2����ɹ����ҵ�Ԫ��
	char letterList[26];
	ListNode* currentPoint;    //�洢���α����Ľ��
	ListNode* prePoint;     //�洢��һ��currentPoint�����Ľ��
	int number = 0;
	BinTree T = BT;
	Queue Q = CreateQueue();//�ȴ���һ������
	AddQ(Q, BT, BT->layer);//���
	memset(letterList, 0, sizeof(letterList));
	while (1)
	{
		number = 0;
		T = DeleteQ(Q);//����
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
			currentPoint = T->Data;      //��ֵT��ͷ�ڵ�
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