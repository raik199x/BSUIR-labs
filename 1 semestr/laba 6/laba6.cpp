// laba 6 powered by alexander
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>
int main() {
	int *m, *m2, n, max, kordi[2], flag = 0, n2;
	printf("welcom user!\nenter n size of matrix\n");
	scanf_s("%d", &n); // ��� �������
	n2 = n - 1;
	m = (int*)malloc(sizeof(int)*n*n); // ������������� ������������� ���������� �������
	system("CLS"); // ������� �������
	printf("dear user, enter the whole matrix\n");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf_s("%d", m + i * n + j); // �� ��������� ������
		}
	}
	system("CLS"); // ������� �������
	max = *(m + 0 * n + 0); //����� 1 �����
	//���� ���������� ����� �� ������
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (abs(max) < abs(*(m + i * n + j)) || abs(max) == abs(*(m + i * n + j))) { // ����� ���������� ��� ���� ��������
				max = *(m + i * n + j);                                                // ���, ���� ����� ���������� ����������, �� ���������� ��� �����
				kordi[0] = i; //���������� �����
				kordi[1] = j; // ���� �����
			}
		}
	}
	//����� ����������� ����� (��� ����� ������)
	 // � �� ���� ��������� ��������� ������, � ������ ��������� ����� ��� ������������
	for (int i = 0; i < n; i++) {
		if (i == kordi[0]) {  //� ��� �� ���������� ������ ������� ������ ��������� ���������, ����� �� ���������� ������ ������ �� �������� �����
			flag = 1; //��� ���������� � ���
		}
		for (int j = 0; j < n; j++) {
			if (i == kordi[0] || j == kordi[1]) { // ���� ������ �� ����� �������� ������ ������� ��
				continue;
			}
			printf("%d ", *(m + i * n + j));
		}
		if (flag == 1) { //�� ����� ����� �����������, � �� ���������� ������ (������� �� �������� ��� ������ �����)
			flag = 0; //������� ������
		}
		else {
			printf("\n");
		}
	}
}