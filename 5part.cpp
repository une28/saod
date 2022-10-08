//вариант 108
/*Хранящуюся  в  файле  базу  данных (4000 записей) загрузить динамически в оперативную
память компьютера (в зависимости от варианта),
вывести на экран по 20 записей (строк) на странице с возможностью отказа от просмотра
и построить  индексный  массив,  упорядочивающий данные в соответствии  с  заданным
 условием  упорядочения, используя указанный метод сортировки.
Предусмотреть  возможность  поиска по  ключу  в упорядоченной  базе, в  результате
которого  из  записей с одинаковым ключом  формируется  очередь, содержимое  очереди  выводится  на экран.

B = 2         (файл testBase2.dat)
	 База данных "Пpедпpиятие"

	 Стpуктуpа записи:
		ФИО сотpудника: текстовое поле 30 символа
						фоpмат <Фамилия>_<Имя>_<Отчество>
		Hомеp отдела:   целое число
		Должность:      текстовое поле 22 символа
		Дата pождения:  текстовое поле 10 символов
						фоpмат дд-мм-гг


Пpимеp записи из БД:
		Петpов_Иван_Иванович____________
		130
		начальник_отдела______
		15-03-46

	 Ваpианты условий упоpядочения и ключи поиска (К):
	 C = 2 - по дням(!) pождения и ФИО, К = день pождения;

	 D = 3 Дерево оптимального поиска (приближеный алгоритм А1)

	 E = 4 Код Гилберта-Мура

*/

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include <iomanip>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <algorithm>

const int N = 4000;

using namespace std;

struct stack
{
	struct stack* next;

	char fio2[30];
	short number2;
	char position2[22];
	char date2[10];

	unsigned char fio3[33];
	unsigned char day[3];
};

typedef struct Node
{
	stack* data;
	Node* left, * right;
}Node;

void ReadBase(struct stack** head, struct stack** tail, char* name);
void DigitalSort(stack** head, stack** tail, bool sort_type);
void index(stack* head, stack** array);
void printArray(stack** array);
int BSearchAll1(stack** array);
void AddNode(Node** root, stack* D);
Node* alg_A1(Node* root, int n, stack** array, int* V_w, bool* V_use);
void make_tree(stack* p, int start_point, stack** array, int n);
void* obhod(Node* root, int count, int start_point);
void tree_search(Node* root, int x, int start_point);
void loop(int c, stack* head, stack* tail, stack** array);


struct tov { char fio[30]; short number; char position[22]; char date[10]; } t1;

struct Queue {
	stack* head;
	stack* tail;
};

void ReadBase(struct stack** head, struct stack** tail, char* name)
{
	int i; FILE* tf;
	system("CLS");
	tf = fopen(name, "rb");
	i = 1;

	srand(time(NULL));
	struct stack* p;

	while (fread(&t1, sizeof(t1), 1, tf))
	{
		p = new stack;


		for (int j = 0; j < 30; j++)
			p->fio2[j] = t1.fio[j];

		p->number2 = t1.number;

		for (int j = 0; j < 22; j++)
			p->position2[j] = t1.position[j];

		for (int j = 0; j < 10; j++)
			p->date2[j] = t1.date[j];

		p->day[1] = t1.date[0];

		p->day[0] = t1.date[1];

		int n = 30;
		int c = n;
		for (int j = 0; j <= n; j++)
		{
			p->fio3[j] = t1.fio[c];

			c--;
		}

		(*tail)->next = p;
		*tail = p;

		i++;
	}
	(*tail)->next = NULL;
}

void DeleteStack(struct stack** head)
{
	int i;
	struct stack* p = *head;
	while (p != NULL)
	{
		*head = p->next;
		free(p);
		p = *head;
	}
}

void DeleteQueue(struct stack** head, struct stack** tail)
{
	DeleteStack(head);
	*tail = NULL;
	*tail = (stack*)&(*head);
}

void printArray(stack** array)
{
	int i = 0;

	int count = 20;

	char c;
	while (count <= N)
	{
		printf("\n\n Print next 20 people? y/n\n");
		c = _getch();
		if (c == 'n') break;
		while (i < N && i <= count)
		{
			printf("\n %3d) %25s  %8d  %22s  %10s", i, array[i]->fio2, array[i]->number2, array[i]->position2, array[i]->date2);
			i++;
		}
		count += 20;
	}
}

void index(stack* head, stack** array)
{
	int i = 0;
	stack* p;

	p = head;

	while (p != NULL)
	{
		array[i] = p;

		p = p->next;
		i++;
	}
}

void DigitalSort(stack** head, stack** tail, bool sort_type)
{
	int d, i, j, k, l;
	if (sort_type == 0)
		l = 3;
	else
		l = 33;
	stack* p;
	Queue q[256];
	for (j = l; j > 0; j--)
	{
		for (i = 0; i < 256; i++) q[i].tail = (stack*)&q[i].head;
		p = *head;
		k = l - 1 - j;
		while (p != NULL)
		{
			if (sort_type == 0)
				d = p->day[k];
			else
				d = p->fio3[k];

			q[d].tail->next = p;
			q[d].tail = p;
			p = p->next;
		}

		p = (stack*)&(*head);
		for (i = 0; i < 256; i++)
		{
			if (q[i].tail != (stack*)&q[i].head)
			{
				p->next = q[i].head;
				p = q[i].tail;
			}
		}
		p->next = NULL;
	}
}

int BSearchAll1(stack** array)
{
	struct stack* p, * head, * tail;
	int n = N-1, i = 0;

	int C[n];
	for (i = 1; i <= n; i++)
	{
		C[i] = i;
	}

	char key1;
	char key2;
	int ind = 0;

	int KEY;
	cout << "\n Enter day: ";
	cin >> KEY;

	int L = 1, R = n, found = 0, m = n, d, M;
	while (L <= R)
	{
		m = (L + R) / 2;
		d = C[m];
		if (((int)array[d]->day[1] - '0') * 10 + array[d]->day[0] - '0' < KEY)
		{
			L = m + 1;
		}
		else
			if (((int)array[d]->day[1] - '0') * 10 + array[d]->day[0] - '0' == KEY)
			{
				int M = d;
				M++;


				if (((int)array[d - 1]->day[1] - '0') * 10 + array[d - 1]->day[0] - '0' == KEY - 1 || M == 2 && KEY == 1)
				{
					p = new stack;
					p = array[d];
					head = p; tail = p;
					p = p->next;
					while (((int)array[d]->day[1] - '0') * 10 + array[d]->day[0] - '0' == KEY && d < N)
					{
						p = new struct stack;
						p = array[d];
						tail->next = p;
						tail = p;

						found++;
						d++;
					}
					tail->next = NULL;
				}

				R = m - 1;
			}
			else R = m - 1;
	}
	if (found == 0)
		printf("No matches");

	int start_point = d + 1;
	make_tree(head, start_point, array, found);
}

void make_tree(stack* p, int start_point, stack** array, int n)
{
	stack* head = p;

	index(head, array);

	int V_w[n];
	for (int i = 0; i < n; i++)
	{
		V_w[i] = rand() % 100;
	}

	bool V_use[n];
	for (int i = 1; i < n; i++)
	{
		V_use[i] = 0;
	}

	Node* root = NULL;
	root = alg_A1(root, n, array, V_w, V_use);
	obhod(root, 0, start_point);
	printf("\n\n 1 - Additional search\n 2 - Exit\n ");
	int c;
	scanf("%d", &c);
	switch (c)
	{
	case 1:
	{
		printf("\n Enter № of the department: ");
		int number;
		scanf("%d", &number);
		tree_search(root, number, start_point);
	}
	case 2:
	{
		break;
	}
	}
}

void tree_search(Node* root, int x, int start_point)
{
	Node* p = root;
	while (p != NULL)
	{
		if (p->data->number2 < x)
		{
			p = p->right;
			start_point++;
		}
		else
			if (p->data->number2 > x)
			{
				p = p->left;
				start_point;
			}
			else if (p->data->number2 == x)
			{
				while (p->data->number2 == x)
				{
					printf("\n %d) %25s  %8d  %22s  %10s", start_point, p->data->fio2, p->data->number2, p->data->position2, p->data->date2);
					p = p->left;
				}
				break;
			}
	}
}

void AddNode(Node** root, stack* D)
{
	Node** p = root;

	while ((*p) != NULL)
	{
		if (D->number2 <= (*p)->data->number2)
		{
			p = &((*p)->left);
		}
		else if (D > (*p)->data)
		{
			p = &((*p)->right);
		}
		else
		{
			break;
		}
	}

	if ((*p) == NULL)
	{
		(*p) = new Node;
		(*p)->data = D;
		(*p)->left = (*p)->right = NULL;
	}
}

Node* alg_A1(Node* root, int n, stack** array, int* V_w, bool* V_use)
{

	int  max, index;

	root = NULL;

	for (int i = 0; i < n; i++)
	{
		max = 0;
		index = 0;
		for (int j = 0; j < n; j++)
		{
			if (V_w[j] > max && V_use[j] == 0)
			{
				max = V_w[j];
				index = j;
			}
		}
		V_use[index] = 1;
		AddNode(&root, array[index]);
	}
	return root;
}

void* obhod(Node* root, int count, int start_point)
{
	if (root != NULL) {
		obhod(root->left, count + 1, start_point);
		printf("\n %d) %25s  %8d  %22s  %10s", start_point, root->data->fio2, root->data->number2, root->data->position2, root->data->date2);
		start_point++;
		obhod(root->right, count + 1, start_point);
	}
}

void Mur(const int n, double p[], int Length[], char c[][20]) {
	double* q = new double[n];
	Length[0] = -floor(log2(p[0])) + 1;
	q[0] = p[0] / 2;
	for (int i = 1; i < n; ++i) {
		double tmp = 0;
		for (int k = 0; k < i; k++)
			tmp += p[k];

		q[i] = tmp + p[i] / 2;
		Length[i] = -floor(log2(p[i])) + 1;
	}

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < Length[i]; ++j) {
			q[i] *= 2;
			c[i][j] = floor(q[i]) + '0';
			if (q[i] >= 1) {
				q[i] -= 1;
			}
		}

	}
}

unordered_map<char, int> get_char_counts_from_file(const string& file_name, int& file_size, int n = N) {
	ifstream file(file_name, ios::binary);
	if (!file.is_open()) {
		throw runtime_error("No file");
	}
	char ch_arr[sizeof(tov) * n];
	file.read((char*)ch_arr, sizeof(tov) * n);
	file.close();

	unordered_map<char, int> counter_map;
	file_size = 0;
	for (auto ch : ch_arr) {
		counter_map[ch]++;
		file_size++;
	}
	return counter_map;
}

vector<pair<double, char>> calc_probabilities(const unordered_map<char, int>& counter_map, int count) {
	vector<pair<double, char>> probabilities;
	probabilities.reserve(counter_map.size());
	for (auto i : counter_map) {
		probabilities.emplace_back(make_pair((double)i.second / count, i.first));
	}
	return probabilities;
}

void coding() {
	int file_size;
	unordered_map<char, int> counter_map;
	counter_map = get_char_counts_from_file("testBase2.dat", file_size);

	auto probabilities = calc_probabilities(counter_map, file_size);
	counter_map.clear();

	sort(probabilities.begin(), probabilities.end(), greater<pair<char, int>>());
	cout << "Prob. of a symbol\n";
	for (auto i : probabilities) {
		cout << fixed << i.first << " | " << i.second << '\n';
	}

	const int n = (int)probabilities.size();

	auto c = new char[n][20];
	auto Length = new int[n];
	auto p = new double[n];
	for (int i = 0; i < n; ++i) {
		p[i] = probabilities[i].first;
	}

	double shen = 0;
	Mur(n, p, Length, c);
	cout << "\nGilbert-Moore code:\n";
	cout << "\nSymb Prob Code\n";
	double avg_len = 0;
	double entropy = 0;
	for (int i = 0; i < n; i++) {
		avg_len += Length[i] * p[i];
		entropy -= p[i] * log2(p[i]);
		printf("%4c | %.5lf | ", probabilities[i].second, p[i]);
		for (int j = 0; j < Length[i]; ++j) {
			printf("%c", c[i][j]);
		}
		cout << '\n';
		shen += p[i];
	}
	cout << "Avg length = " << avg_len << '\n'
		<< "Entropy = " << entropy << '\n'
		<< "Avg length < entropy + 2\n"
		<< "N = " << n << endl;
}

void loop(int c, stack* head, stack* tail, stack** array)
{
	switch (c)
	{
	case 1:
	{
		index(head, array);
		printArray(array);
		break;
	}
	break;
	case 2:
	{
		index(head, array);
		DigitalSort(&head, &tail, 0);
		index(head, array);
		printArray(array);
	}
	break;
	case 3:
	{
		index(head, array);
		DigitalSort(&head, &tail, 1);
		index(head, array);
		printArray(array);
	}
	break;
	case 4:
	{
		index(head, array);
		DigitalSort(&head, &tail, 0);
		index(head, array);
		BSearchAll1(array);
	}
	break;
	case 5:
	{
		coding();
	}
	case 6:
		break;
	default:
		break;
	}
}

int main()
{
	srand(time(0));
	stack* head, * tail, * array[N];
	int parts[30];
	head = NULL;
	tail = (stack*)&head;

	ReadBase(&head, &tail, "testBase2.dat");

	printf("\n 1 - Print base \n 2 - Sort by days\n 3 - Sort by name\n 4 - Search by days\n 5 - Encode\n 6 - Exit\n");

	int c = 0;
	while (c != 6)
	{
		printf("\nEnter your choice: ");
		scanf("%d", &c);
		loop(c, head, tail, array);
	}

	system("PAUSE");
	return 0;
}
