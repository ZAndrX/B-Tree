#include <iostream>
#include <windows.h>
#include <string>
using namespace std;
struct BTree {
	bool full = false;//���������� ������������� ����
	string* Data;//������ ������
	BTree* Prev = nullptr; //���������� �������
	BTree** Next;//�������� ��������
	int countData = 0;//������� ������
	int countNext = 0;//������� �������� ���������
};
void simple_insert(string x, BTree* p, int N) {
	p->countData++; //����������� ������� ���������
	if (p->countData == 2 * N - 1) p->full = true; //��������� �������������
	if (x < p->Data[0]) {//���� ������� ��� ������� ������ 1 ��������
		for (int j = (2 * N) - 1; j > 0; j--) p->Data[j] = p->Data[j - 1];
		for (int j = (2 * N); j > 0; j--) p->Next[j] = p->Next[j - 1];
		p->Data[0] = x;
		p->Next[0] = nullptr;
	}
	else
		if (p->countData == 1) {
			//���� ��������� ������
			p->Data[0] = x;
			p->Next[0] = nullptr;
		}
		else
			if (x > p->Data[p->countData - 2]) {
				//���� ������ ��� ������� ������ �������� ��������
				p->Data[p->countData - 1] = x;
				p->Next[p->countData - 1] = nullptr;
			}
			else
				for (int i = 0; i < p->countData - 2; i++) {
					if (x > p->Data[i] && x < p->Data[i + 1]) {//����� ����� � ����
						//�������� ��������� ����� �������
						for (int j = (2 * N) - 1; j > i + 1; j--) p->Data[j] = p->Data[j - 1];
						for (int j = (2 * N); j > i + 1; j--) p->Next[j] = p->Next[j - 1];
						p->Data[i + 1] = x;
						p->Next[i + 1] = nullptr;
						break;
					}
				}

}
void drob(BTree* p, int N) {//��������� ����
	BTree* ch1 = new BTree;
	//������� ����� ������� ������� ����� ���������� ������ N-1 ���������
	ch1->Data = new string[2 * N];
	ch1->Next = new BTree * [2 * N + 1];
	for (int i = 0; i < N - 1; i++) {//�������� � ���� ������ N-1 ���������
		ch1->countData++;
		ch1->Data[i] = p->Data[i];
	}
	for (int i = 0; i <= 2 * N; i++) ch1->Next[i] = nullptr;
	if (p->countNext > 0) { //�������������� ����� �������� ���������
		for (int i = 0; i < N; i++) {
			ch1->Next[i] = p->Next[i];
			if (ch1->Next[i] != nullptr) ch1->Next[i]->Prev = ch1;
		}
		ch1->countNext = N - 1;
	}
	if (ch1->countData == 2 * N - 1) ch1->full = true;
	//������� ����� ������� ������� ����� ���������� ��������� N-1 ���������
	BTree* ch2 = new BTree;
	ch2->Data = new string[2 * N];
	ch2->Next = new BTree * [2 * N + 1];
	for (int i = 0; i < N && (i + N) < p->countData; i++) {//�������� � ���� ��������� N-1 ��������
		ch2->countData++;
		ch2->Data[i] = p->Data[i + N];
	}
	for (int i = 0; i <= 2 * N; i++) ch2->Next[i] = nullptr;
	if (p->countNext > 0) {//�������������� ����� �������� ���������
		for (int i = 0; i <= N; i++) {
			ch2->Next[i] = p->Next[i + N];
			if (ch2->Next[i] != nullptr) ch2->Next[i]->Prev = ch2;
		}
		ch2->countNext = N;
	}
	if (p->Prev != nullptr) {//���� ������� ������� �� ������ ������
		simple_insert(p->Data[N - 1], p->Prev, N);
		ch1->Prev = p->Prev; //��������� � ���� �����
		ch2->Prev = p->Prev;
		for (int i = 0; i <= 2 * N; i++) {
			if (p->Prev->Next[i] == p) {
				p->Prev->Next[i] = nullptr;
				break;
			}
		}
		for (int i = 0; i <= 2 * N; i++) {//�������������� ����� �� ��������
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
		//���� ������� ������� ��� ������ ������
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
	if (p->countData == 0) simple_insert(x, p, N);//���� ���� ������ ������ ���������
	else
		//��� ���� ������� ��� �������
		if (x >= p->Data[p->countData - 1]) {
			
			if (p->full && p->Next[p->countData] == nullptr) {
				//������� ���� ���� �����
				simple_insert(x, p, N);
				drob(p, N);
			}
			else {
				if (p->Next[p->countData] == nullptr)simple_insert(x, p, N);//���� ���� ����� � ����
				else
					search(x, p->Next[p->countData], N);//����������� ����� �� �������
			}
			return;
		}
		else {
			for (int i = 0; i < p->countData; i++) {
				if (x < p->Data[i]) {
					if (p->Next[i] == nullptr) {
						if (p->full) {
							//������� ���� ���� �����
							simple_insert(x, p, N);
							drob(p, N);//��������� ����

						}
						else simple_insert(x, p, N);//���� ���� ����� � ����

					}
					else {
						search(x, p->Next[i], N);//����������� ����� �� �������

					}
					return;
				}
			}
		}
}
void output(string str[], int l, BTree* p, int& max_level, int N) {
	if (!p) {//���� ����� �� ��������� �����-�
		return;
	}
	if (max_level < l) max_level = l; //����������� ������������ ������� ��� ������
	str[l] += "|";//������� �������
	for (int i = 0; i < p->countData; i++) {
		if (i == p->countData - 1) str[l] = str[l] + p->Data[i] + "|  ";
		else
			str[l] = str[l] + p->Data[i] + ":";//�������� ������� � ������
	}
	for (int i = 0; i < 2 * N + 1; i++) {//���� ���� �������� ��������, ���������� ��������� � ���
		if (p->Next[i] != nullptr) output(str, l + 1, p->Next[i], max_level, N);
	}
}
void simple_del(string x, BTree* p, int N) {
	for (int i = 0; i < p->countData; i++) {
		if (p->Data[i] == x) {//���� ������� � ����
			for (int j = i; j < p->countData; j++) {//������� ����� � �������� ��������
				p->Data[j] = p->Data[j + 1];
				p->Next[j] = p->Next[j + 1];
			}
			p->Data[p->countData - 1] = "";//������ ������ ��������� ����
			if (p->Next[p->countNext] != nullptr) {//������� ����� ������ �������� �������,���� �� ����
				p->Next[p->countNext - 1] = p->Next[p->countNext];
				p->Next[p->countNext] = nullptr;
			}
			p->countData--;//��������� ������� ������
			break;
		}
	}
	if (p->countData < 2 * N - 1 && p->full) p->full = false;
}
void leaf_del(string x, BTree* p, int N, BTree* head) {
	for (int i = 0; p->countData; i++) {
		if (p->Data[i] == x) {//���� ���� � ����
			int j = 0;
			for (; j < 2 * N + 1; j++) if (p->Prev->Next[j] == p) break;//���� ������� ���� � ������������ ����
			if (p->Prev->Next[j + 1] != nullptr) {//���� ���������� ���� ������
				if (p->Prev->Next[j + 1]->countData > N - 1) {//���� � ���� ������ ������ N-1 ������
					simple_del(x, p, N);
					simple_insert(p->Prev->Data[j], p, N);//��������� ������� �������������� ��������
					p->Prev->Data[j] = p->Prev->Next[j + 1]->Data[0];//������ ��� �� �� ������ ������� � ������ ����
					simple_del(p->Prev->Next[j + 1]->Data[0], p->Prev->Next[j + 1], N);//������� �������������� ����
					break;
				}
				else {
					simple_del(x, p, N);//������� ���� �� ����
					while (p->countData > 0) { //��������� ������� ���� � ����� ������
						simple_insert(p->Data[0], p->Prev->Next[j + 1], N);
						simple_del(p->Data[0], p, N);
					}
					simple_insert(p->Prev->Data[j], p->Prev->Next[j + 1], N);//��������� �������������� ������� � ���� ������
					simple_del(p->Prev->Data[j], p->Prev, N);//������� ��� �� ��������������� ����
					delete p; //������� ����,� � �� ��� ��������� � ���� ������
					break;
				}
			}
			if (p->Prev->Next[j - 1] != nullptr) {//���� ����� ������ ���� ��� ������ ������,�� ���� ���� �����
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
		if (p->Data[i] == x) {//���� ������� � ����
			if (p->Next[i]->countData > N-1) {//���� � ����� ���� ������ N-1 ����������, ��������� ������ ��������� �����, ����� ������ ������� ��������� ����
				p->Data[i] = p->Next[i]->Data[p->Next[i]->countData - 1];
				simple_del(p->Next[i]->Data[p->Next[i]->countData - 1], p->Next[i], N);
			}
			else {
				if (p->Next[i + 1]->countData > N-1) {//���� � ������ ���� ������ N-1 ����������, ��������� ������ ��������� �����, ����� ����� ������� ��������� ����
					p->Data[i] = p->Next[i + 1]->Data[0];
					simple_del(p->Next[i + 1]->Data[0], p->Next[i + 1], N);
				}
				else {
					while (p->Next[i + 1]->countData > 0) {//���� � �������� ��������� N-1 ������ ��� �����, ��������� �� � ��������� ����� ����� ���� �� ����� ���������� ��������
						simple_insert(p->Next[i + 1]->Data[0], p->Next[i], N);
						simple_del(p->Next[i + 1]->Data[0], p->Next[i + 1], N);
					}
					p->Data[i] = p->Next[i]->Data[0];
					simple_del(p->Next[i]->Data[0], p->Next[i], N);
					p->Next[i + 1] = nullptr;
					delete p->Next[i + 1];//��������� ������ �������� ����
				}
			}
			break;
		}
	}
}
void Del(string x, BTree* p, int N, BTree* head) {
	for (int i = 0; i < p->countData; i++) {//���� ���������� ����, ��� �� ��� � ��������� ������
		if (x == p->Data[i]) {
			if (p == head && p->Next[i] == nullptr && i == 0|| p == head && p->Next[i] == nullptr) {//���� ���� ��������� � ����-�������
				simple_del(x, p, N);
				break;
			}
			if (p->countData >= N && p->Next[i] == nullptr) simple_del(x, p, N);//���� � ����� ��� �������� ����� � � ���� ����� N-1 ������
			else
				if (p->Next[i] == nullptr) {
					leaf_del(x, p, N, head);//���� � ����� ��� �������� �����
				}
				else
					knot_del(x, p, N);//���� � ����� ���� �������� ����
			break;
		}
		if (x < p->Data[i]) {//���� ����� �� ����������
			if (p->Next[i] == nullptr) cout << "������� �� ������";
			else Del(x, p->Next[i], N, head);
			break;
		}
		if (x > p->Data[i] && p->countData - 1 == i) {
			if (p->Next[i + 1] == nullptr) cout << "������� �� ������";
			else Del(x, p->Next[i + 1], N, head);
			break;
		}
	}
}
int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	string s;
	cout << "������� ������� ������ ";
	getline(cin, s);//��������� ������� ������
	const int N = stoi(s);
	cout << "������� ������: " << N << endl;
	BTree* head = new BTree;//������� ������� ������
	head->Data = new string[2 * N];
	head->Next = new BTree * [2 * N + 1];
	head->Prev = nullptr;
	for (int i = 0; i < 2 * N + 1; i++) {
		head->Next[i] = nullptr;
	}
	int max_level = 0;//������ ������������ �������, ��� ����������� ������
	string comand;//������ ��� ����� ������
	cout << "�������: insert, delete" << endl;
	cout << "������� �������: ";
	for (getline(cin, comand); s != ""&&comand!=""; cout << "������� �������: ", getline(cin, comand)) {
		if (comand == "delete" || comand == "insert") {//��������� �������
			cout << "������� �������: ";
			getline(cin, s);//��������� �������
			if (comand == "delete") Del(s, head, N, head);
			else
				if (comand == "insert") search(s, head, N);
			string str[1000];
			output(str, 0, head, max_level, N);
			for (int i = 0; i <= max_level; i++) {
				cout << i << ") " << str[i] << endl;
			}//������������ ����� ������ �� �������
			cout << endl;
		}
		else
			cout << "������� �� ����������" << endl;//���� ������� �� ����������-������ ��������� ����
	}
}