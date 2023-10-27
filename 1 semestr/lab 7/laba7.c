//powered by Alexander
// ��� ��������� ������� ����������� ��������: �������� � ����� - �������
#include <stdio.h>
#include <malloc.h>

//#define scanf_s scanf // uncomment for linux

int main() {
	int n,flag=0, flag2 = 1,m=0,k=0;
	char *mas, z;
	printf("enter length:\n");
	scanf_s("%d", &n);
	n += 1;
	mas = (char*)malloc(sizeof(char)*n); //������������ ���������
	printf("type message:\n");
	//����
	for (int i = 0; i < n; i++) {
		scanf_s("%c", &mas[i]);
	}
	//������� ������� � ������
	if (mas[1] == ' ') {
		for (int i = 1; i < n; i++) {
			if (mas[i] == ' ') {
				flag += 1;
			} else
			if (mas[i] != ' ') {
				break;
			}
		}
	}
 //(���� ��� ����) ������� ������� � ������
	if (mas[1] == ' ') {
		for (int i = 1; i < n; i++) {
			mas[i] = mas[i + flag];
		}
		n -= flag;
	}
	// �������� �������� � �����
	while (mas[n - 1] == ' ') {
		n--;
	}
	//������� ������� �������
	while (flag2 == 1) {
		k = 0;
		//���� ��������
		for (int i = 0; i < n; i++) {
			if (mas[i] == ' ') {
				m += 1;
			}
			else {
				m = 0;
			}
			if (m >= 2) {
				flag = i;
				for (; flag-m < n; flag++) {
					mas[flag] = mas[flag + 1]; //������ ������
				}
				n--; //������
			}
		}
		// ���� ��������
		for (int i = 0; i < n; i++) {
			if (mas[i] == ' ') {
				k += 1;
			}
			else {
				k = 0;
			}
			if (k >= 2) {
				flag2 = 1;
				break; //�� ��� �� �������� ��� ���� ������ ������ ������ ��������� ����� �����
			}
			else {
				flag = 0; //�������� ����
			}
		}
		if (k == 0) { // ���� ������ �������� ���� = �������
			break;
		}
	}
	//���������
	for (int i = 0; i < n; i++) {
		printf("%c", mas[i]);
	}
}