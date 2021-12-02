#include <iostream>
#include <windows.h>
#include <string>
using namespace std;
struct BTree {
	bool full = false;//показывает заполненность узла
	string* Data;//массив ключей
	BTree* Prev = nullptr; //пердыдущий элемент
	BTree** Next;//дочерние элементы
	int countData = 0;//счетчик ключей
	int countNext = 0;//счетчик дочерних элементов
};
void simple_insert(string x, BTree* p, int N) {
	p->countData++; //увелечиваем счетчик элементов
	if (p->countData == 2 * N - 1) p->full = true; //проверяем заполненность
	if (x < p->Data[0]) {//если элемент для вставки меньше 1 элемента
		for (int j = (2 * N) - 1; j > 0; j--) p->Data[j] = p->Data[j - 1];
		for (int j = (2 * N); j > 0; j--) p->Next[j] = p->Next[j - 1];
		p->Data[0] = x;
		p->Next[0] = nullptr;
	}
	else
		if (p->countData == 1) {
			//если контейнер пустой
			p->Data[0] = x;
			p->Next[0] = nullptr;
		}
		else
			if (x > p->Data[p->countData - 2]) {
				//если элемнт для вставки больше крайнего элемента
				p->Data[p->countData - 1] = x;
				p->Next[p->countData - 1] = nullptr;
			}
			else
				for (int i = 0; i < p->countData - 2; i++) {
					if (x > p->Data[i] && x < p->Data[i + 1]) {//поиск места в узле
						//смещение элементов после вставки
						for (int j = (2 * N) - 1; j > i + 1; j--) p->Data[j] = p->Data[j - 1];
						for (int j = (2 * N); j > i + 1; j--) p->Next[j] = p->Next[j - 1];
						p->Data[i + 1] = x;
						p->Next[i + 1] = nullptr;
						break;
					}
				}

}
void drob(BTree* p, int N) {//разбиение узла
	BTree* ch1 = new BTree;
	//создаем новый элемент который будет содеражать первые N-1 элементов
	ch1->Data = new string[2 * N];
	ch1->Next = new BTree * [2 * N + 1];
	for (int i = 0; i < N - 1; i++) {//копируем в него первые N-1 элементов
		ch1->countData++;
		ch1->Data[i] = p->Data[i];
	}
	for (int i = 0; i <= 2 * N; i++) ch1->Next[i] = nullptr;
	if (p->countNext > 0) { //перенаправляем связи дочерних элементов
		for (int i = 0; i < N; i++) {
			ch1->Next[i] = p->Next[i];
			if (ch1->Next[i] != nullptr) ch1->Next[i]->Prev = ch1;
		}
		ch1->countNext = N - 1;
	}
	if (ch1->countData == 2 * N - 1) ch1->full = true;
	//создаем новый элемент который будет содеражать последние N-1 элементов
	BTree* ch2 = new BTree;
	ch2->Data = new string[2 * N];
	ch2->Next = new BTree * [2 * N + 1];
	for (int i = 0; i < N && (i + N) < p->countData; i++) {//копируем в него последние N-1 элементы
		ch2->countData++;
		ch2->Data[i] = p->Data[i + N];
	}
	for (int i = 0; i <= 2 * N; i++) ch2->Next[i] = nullptr;
	if (p->countNext > 0) {//перенаправляем связи дочерних элементов
		for (int i = 0; i <= N; i++) {
			ch2->Next[i] = p->Next[i + N];
			if (ch2->Next[i] != nullptr) ch2->Next[i]->Prev = ch2;
		}
		ch2->countNext = N;
	}
	if (p->Prev != nullptr) {//если текущий элемент не голова дерева
		simple_insert(p->Data[N - 1], p->Prev, N);
		ch1->Prev = p->Prev; //соеденяем с пред узлом
		ch2->Prev = p->Prev;
		for (int i = 0; i <= 2 * N; i++) {
			if (p->Prev->Next[i] == p) {
				p->Prev->Next[i] = nullptr;
				break;
			}
		}
		for (int i = 0; i <= 2 * N; i++) {//перенаправляем связь от родителя
			if (p->Data[N - 1] == p->Prev->Data[i]) {
				p->Prev->Next[i] = ch1;
				p->Prev->countNext++;
				if (p->Prev->Next[i + 1] == nullptr) p->Prev->Next[i + 1] = ch2;
				else {
					for (int j = 2 * N; j > i; j--) p->Prev->Next[j] = p->Prev->Next[j - 1];
					p->Prev->Next[i + 1] = ch2;
				}
				break;
			}
		}
		if (p->Prev->countData == 2 * N) drob(p->Prev, N);
		delete p;
	}
	else {
		//если текущий элемент это голова дерева
		p->Data[0] = p->Data[N - 1];
		p->Next[0] = ch1;
		p->Next[1] = ch2;
		for (int i = 1; i < p->countData; i++) p->Data[i] = "";
		for (int i = 2; i <= 2 * N; i++) p->Next[i] = nullptr;
		p->countData = 1;
		p->countNext = 2;
		ch1->Prev = p;
		ch2->Prev = p;
		p->full = false;
	}

}
void search(string x, BTree* p, int N) {
	if (p->countData == 0) simple_insert(x, p, N);//если узел пустой просто вставляем
	else
		//или ищем позицию для вставки
		if (x >= p->Data[p->countData - 1]) {
			
			if (p->full && p->Next[p->countData] == nullptr) {
				//вставка если узел полон
				simple_insert(x, p, N);
				drob(p, N);
			}
			else {
				if (p->Next[p->countData] == nullptr)simple_insert(x, p, N);//если есть место в узле
				else
					search(x, p->Next[p->countData], N);//рекурсивный поиск по ребенку
			}
			return;
		}
		else {
			for (int i = 0; i < p->countData; i++) {
				if (x < p->Data[i]) {
					if (p->Next[i] == nullptr) {
						if (p->full) {
							//вставка если узел полон
							simple_insert(x, p, N);
							drob(p, N);//разделяем узел

						}
						else simple_insert(x, p, N);//если есть место в узле

					}
					else {
						search(x, p->Next[i], N);//рекурсивный поиск по ребенку

					}
					return;
				}
			}
		}
}
void output(string str[], int l, BTree* p, int& max_level, int N) {
	if (!p) {//если пусто то завершаем функц-ю
		return;
	}
	if (max_level < l) max_level = l; //увеличиваем максимальный уровень для вывода
	str[l] += "|";//создаем границу
	for (int i = 0; i < p->countData; i++) {
		if (i == p->countData - 1) str[l] = str[l] + p->Data[i] + "|  ";
		else
			str[l] = str[l] + p->Data[i] + ":";//копируем элемент в строку
	}
	for (int i = 0; i < 2 * N + 1; i++) {//если есть дочерние элементы, рекурсивно переходим в них
		if (p->Next[i] != nullptr) output(str, l + 1, p->Next[i], max_level, N);
	}
}
void simple_del(string x, BTree* p, int N) {
	for (int i = 0; i < p->countData; i++) {
		if (p->Data[i] == x) {//ищем элемент в узле
			for (int j = i; j < p->countData; j++) {//смещаем ключи и дочерние элементы
				p->Data[j] = p->Data[j + 1];
				p->Next[j] = p->Next[j + 1];
			}
			p->Data[p->countData - 1] = "";//делаем пустой последний ключ
			if (p->Next[p->countNext] != nullptr) {//смещаем самый правый дочерний элемент,если он есть
				p->Next[p->countNext - 1] = p->Next[p->countNext];
				p->Next[p->countNext] = nullptr;
			}
			p->countData--;//уменьшаем счетчик ключей
			break;
		}
	}
	if (p->countData < 2 * N - 1 && p->full) p->full = false;
}
void leaf_del(string x, BTree* p, int N, BTree* head) {
	for (int i = 0; p->countData; i++) {
		if (p->Data[i] == x) {//ищем ключ в узле
			int j = 0;
			for (; j < 2 * N + 1; j++) if (p->Prev->Next[j] == p) break;//ищем текущий узел в родительском узле
			if (p->Prev->Next[j + 1] != nullptr) {//если существует узел справа
				if (p->Prev->Next[j + 1]->countData > N - 1) {//если в узле справа больше N-1 ключей
					simple_del(x, p, N);
					simple_insert(p->Prev->Data[j], p, N);//вставляем элемент предшествующий текущему
					p->Prev->Data[j] = p->Prev->Next[j + 1]->Data[0];//меняем его же на первый элемент в правом узле
					simple_del(p->Prev->Next[j + 1]->Data[0], p->Prev->Next[j + 1], N);//удаляем предшествующий узлу
					break;
				}
				else {
					simple_del(x, p, N);//удаляем ключ из узла
					while (p->countData > 0) { //соеденяем текущий узел с узлом справа
						simple_insert(p->Data[0], p->Prev->Next[j + 1], N);
						simple_del(p->Data[0], p, N);
					}
					simple_insert(p->Prev->Data[j], p->Prev->Next[j + 1], N);//вставляем предшествующий элемент в узел справа
					simple_del(p->Prev->Data[j], p->Prev, N);//удаляем его из предшествующего узла
					delete p; //удаляем узел,т к мы его перенесли в узел справа
					break;
				}
			}
			if (p->Prev->Next[j - 1] != nullptr) {//тоже самое делаем если нет соседа справа,но есть узел слева
				if (p->Prev->Next[j - 1]->countData > N - 1) {
					simple_del(x, p, N);
					simple_insert(p->Prev->Data[j], p, N);
					p->Prev->Data[j] = p->Prev->Next[j - 1]->Data[p->Prev->Next[j - 1]->countData-1];
					simple_del(p->Prev->Next[j - 1]->Data[p->Prev->Next[j - 1]->countData - 1], p->Prev->Next[j - 1], N);
					break;
				}
				else {
					simple_del(x, p, N);
					while (p->countData > 0) {
						simple_insert(p->Data[0], p->Prev->Next[j - 1], N);
						simple_del(x, p, N);
					}
					simple_insert(p->Prev->Data[j], p->Prev->Next[j - 1], N);
					simple_del(p->Prev->Data[j], p->Prev, N);
					delete p;
					break;
				}
			}
		}
	}
}
void knot_del(string x, BTree* p, int N) {
	for (int i = 0; p->countData; i++) {
		if (p->Data[i] == x) {//ищем элемент в узле
			if (p->Next[i]->countData > N-1) {//если в левом узле больше N-1 эмлементов, переносим вместо удалймого ключа, самый правый элемент дочернего узла
				p->Data[i] = p->Next[i]->Data[p->Next[i]->countData - 1];
				simple_del(p->Next[i]->Data[p->Next[i]->countData - 1], p->Next[i], N);
			}
			else {
				if (p->Next[i + 1]->countData > N-1) {//если в правом узле больше N-1 эмлементов, переносим вместо удалймого ключа, самый левый элемент дочернего узла
					p->Data[i] = p->Next[i + 1]->Data[0];
					simple_del(p->Next[i + 1]->Data[0], p->Next[i + 1], N);
				}
				else {
					while (p->Next[i + 1]->countData > 0) {//если в дочерних элементах N-1 ключей или менее, соеденяем их и переносим самый левый ключ на место удаляймого элемента
						simple_insert(p->Next[i + 1]->Data[0], p->Next[i], N);
						simple_del(p->Next[i + 1]->Data[0], p->Next[i + 1], N);
					}
					p->Data[i] = p->Next[i]->Data[0];
					simple_del(p->Next[i]->Data[0], p->Next[i], N);
					p->Next[i + 1] = nullptr;
					delete p->Next[i + 1];//удаляемый правый дочерний узел
				}
			}
			break;
		}
	}
}
void Del(string x, BTree* p, int N, BTree* head) {
	for (int i = 0; i < p->countData; i++) {//ущем подходящий узел, так же как в алгоритме поиска
		if (x == p->Data[i]) {
			if (p == head && p->Next[i] == nullptr && i == 0|| p == head && p->Next[i] == nullptr) {//если ключ находится в узле-вершине
				simple_del(x, p, N);
				break;
			}
			if (p->countData >= N && p->Next[i] == nullptr) simple_del(x, p, N);//если у ключа нет дочерних узлов и в узле более N-1 ключей
			else
				if (p->Next[i] == nullptr) {
					leaf_del(x, p, N, head);//если у ключа нет дочерних узлов
				}
				else
					knot_del(x, p, N);//если у ключа есть дочерние узлы
			break;
		}
		if (x < p->Data[i]) {//если ключа не существует
			if (p->Next[i] == nullptr) cout << "Элемент не найден";
			else Del(x, p->Next[i], N, head);
			break;
		}
		if (x > p->Data[i] && p->countData - 1 == i) {
			if (p->Next[i + 1] == nullptr) cout << "Элемент не найден";
			else Del(x, p->Next[i + 1], N, head);
			break;
		}
	}
}
int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	string s;
	cout << "Введите степень дерева ";
	getline(cin, s);//считываем степень дерева
	const int N = stoi(s);
	cout << "Степень дерева: " << N << endl;
	BTree* head = new BTree;//создаем вершину дерева
	head->Data = new string[2 * N];
	head->Next = new BTree * [2 * N + 1];
	head->Prev = nullptr;
	for (int i = 0; i < 2 * N + 1; i++) {
		head->Next[i] = nullptr;
	}
	int max_level = 0;//задаем максимальную глубину, для дальнейшего вывода
	string comand;//строка для ввода команд
	cout << "Команды: insert, delete" << endl;
	cout << "Введите команду: ";
	for (getline(cin, comand); s != ""&&comand!=""; cout << "Введите команду: ", getline(cin, comand)) {
		if (comand == "delete" || comand == "insert") {//считываем команду
			cout << "Введите элемент: ";
			getline(cin, s);//считываем элемент
			if (comand == "delete") Del(s, head, N, head);
			else
				if (comand == "insert") search(s, head, N);
			string str[1000];
			output(str, 0, head, max_level, N);
			for (int i = 0; i <= max_level; i++) {
				cout << i << ") " << str[i] << endl;
			}//осуществляем вывод дерева по уровням
			cout << endl;
		}
		else
			cout << "Команда не распознана" << endl;//если команда не распознана-просим повторить ввод
	}
}