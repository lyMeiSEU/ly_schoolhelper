#include <iostream>
#include <process.h>
#include <Windows.h>

using namespace std;
//����5���̺߳������ֱ��ʾ5�����̶�ϵͳ��Դ������

DWORD WINAPI pro0(PVOID pvPram);

DWORD WINAPI pro1(PVOID pvPram);

DWORD WINAPI pro2(PVOID pvPram);

DWORD WINAPI pro3(PVOID pvPram);

DWORD WINAPI pro4(PVOID pvPram);
//������ȫ����

void ifsafe();

int available[3] = { 3, 3, 2 }; //ϵͳ��ǰ���õ���Դ��
//5�������ѷ������Դ��

int alloc[5][3] = { {0, 1, 0},
				   {2, 0, 0},
				   {3, 0, 2},
				   {2, 1, 1},
				   {0, 0, 2}
};
//5�����̵������Դ��

int max[5][3] = { {7, 5, 3},
				 {3, 2, 2},
				 {9, 0, 2},
				 {2, 2, 2},
				 {4, 3, 3}
};

int a, b, c, d; //�����û�������b,c,d��ʾ���̵�������Դ��

int main()
{
	//��һ���ĸ�ʽ�����ǰ����Դ�������
	cout << "******************���м��㷨**********************" << endl;
	cout << "P0 " << " All:" << alloc[0][0] << alloc[0][1] << alloc[0][2] << " Max:" << max[0][0] << max[0][1] << max[0][2] << " Need:" << max[0][0] - alloc[0][0] << max[0][1] - alloc[0][1] << max[0][2] - alloc[0][2] << endl;
	cout << "P1 " << " All:" << alloc[1][0] << alloc[1][1] << alloc[1][2] << " Max:" << max[1][0] << max[1][1] << max[1][2] << " Need:" << max[1][0] - alloc[1][0] << max[1][1] - alloc[1][1] << max[1][2] - alloc[1][2] << endl;
	cout << "P2 " << " All:" << alloc[2][0] << alloc[2][1] << alloc[2][2] << " Max:" << max[2][0] << max[2][1] << max[2][2] << " Need:" << max[2][0] - alloc[2][0] << max[2][1] - alloc[2][1] << max[2][2] - alloc[2][2] << endl;
	cout << "P3 " << " All:" << alloc[3][0] << alloc[3][1] << alloc[3][2] << " Max:" << max[3][0] << max[3][1] << max[3][2] << " Need:" << max[3][0] - alloc[3][0] << max[3][1] - alloc[3][1] << max[3][2] - alloc[3][2] << endl;
	cout << "P4 " << " All:" << alloc[4][0] << alloc[4][1] << alloc[4][2] << " Max:" << max[4][0] << max[4][1] << max[4][2] << " Need:" << max[4][0] - alloc[4][0] << max[4][1] - alloc[4][1] << max[4][2] - alloc[4][2] << endl;
	cout << "avilable: " << available[0] << " " << available[1] << " " << available[2] << endl;
	cout << "������̺ż���Ӧ������(����5���а�ȫ�Ե��ж�)" << endl;
	cin >> a;
	cout << endl;
	if (a == 5)
	{
		ifsafe();
	}
	else
	{
		cout << "��������" << endl;
		cin >> b >> c >> d;
		//���ݽ��̺���������Ӧ�ý���
		switch (a)
		{
		case 0:
		{
			HANDLE pr0 = CreateThread(NULL, 0,
				pro0, NULL, 0,
				NULL);
			CloseHandle(pr0);
			break;
		}
		case 1:
		{
			HANDLE pr1 = CreateThread(NULL, 0,
				pro1, NULL, 0,
				NULL);
			CloseHandle(pr1);
			break;
		}
		case 2:
		{
			HANDLE pr2 = CreateThread(NULL, 0,
				pro2, NULL, 0,
				NULL);
			CloseHandle(pr2);
			break;
		}
		case 3:
		{
			HANDLE pr3 = CreateThread(NULL, 0,
				pro3, NULL, 0,
				NULL);
			CloseHandle(pr3);
			break;
		}
		case 4:
		{
			HANDLE pr4 = CreateThread(NULL, 0,
				pro4, NULL, 0,
				NULL);
			CloseHandle(pr4);
			break;
		}
		}
	}
	system("pause");
	return 0;
}

DWORD WINAPI pro0(PVOID pvPram)
{
	//�ѷ������Դ�����������Դ��
	alloc[0][0] += b;
	alloc[0][1] += c;
	alloc[0][2] += d;
	//ϵͳ���õ���Դ����ȥ�������Դ��
	available[0] -= b;
	available[1] -= c;
	available[2] -= d;
	int x = 0;
	for (int i = 0; i < 3; i++)
	{
		if (available[i] < 0) //�ж�������Դ���Ƿ����ϵͳ���õ���Դ��
		{
			cout << "������Դ���ڿ�����Դ" << endl;
		}
		else
		{
			x++;
		}
	}
	if (x == 3)
		//����������͵�������Դ����С��ϵͳ���õ���Դ��������а�ȫ�㷨���ж�
		ifsafe();
	return 0;
}

DWORD WINAPI pro1(PVOID pvPram)
{
	alloc[1][0] += b;
	alloc[1][1] += c;
	alloc[1][2] += d;
	available[0] -= b;
	available[1] -= c;
	available[2] -= d;
	int x = 0;
	for (int i = 0; i < 3; i++)
	{
		if (available[i] < 0)
		{
			cout << "������Դ���ڿ�����Դ" << endl;
		}
		else
		{
			x++;
		}
	}
	if (x == 3)
		ifsafe();
	return 0;
}

DWORD WINAPI pro2(PVOID pvPram)
{
	alloc[2][0] += b;
	alloc[2][1] += c;
	alloc[2][2] += d;
	available[0] -= b;
	available[1] -= c;
	available[2] -= d;
	int x = 0;
	for (int i = 0; i < 3; i++)
	{
		if (available[i] < 0)
		{
			cout << "������Դ���ڿ�����Դ" << endl;
		}
		else
		{
			x++;
		}
	}
	if (x == 3)
		ifsafe();
	return 0;
}

DWORD WINAPI pro3(PVOID pvPram)
{
	alloc[3][0] += b;
	alloc[3][1] += c;
	alloc[3][2] += d;
	available[0] -= b;
	available[1] -= c;
	available[2] -= d;
	int x = 0;
	for (int i = 0; i < 3; i++)
	{
		if (available[i] < 0)
		{
			cout << "������Դ���ڿ�����Դ" << endl;
		}
		else
		{
			x++;
		}
	}
	if (x == 3)
		ifsafe();
	return 0;
}

DWORD WINAPI pro4(PVOID pvPram)
{
	alloc[4][0] += b;
	alloc[4][1] += c;
	alloc[4][2] += d;
	available[0] -= b;
	available[1] -= c;
	available[2] -= d;
	int x = 0;
	for (int i = 0; i < 3; i++)
	{
		if (available[i] < 0)
		{
			cout << "������Դ���ڿ�����Դ" << endl;
		}
		else
		{
			x++;
		}
	}
	if (x == 3)
		ifsafe();
	return 0;
}

void ifsafe()
{
	bool finish[5] = { false }; //ÿ�����̵�ǰ��״̬��Ϊfalse��ʾ�ý���û���ͷ���Դ
	int n[5] = { 0 }; //��¼���飬����ý���need��С��available���򽫸ý��̺Ŵ����������У�����������
	int j = 0; //���н��̵ı�������ֵΪ0
	int q = 0; //���������ڼ�������״̬Ϊtrue�ĸ���
	int nn = 0; //
	for (; j < 5; j++)
	{
		int m = 0; //�������������������������Դ�м�������Ҫ��
		if (finish[j] == true)
		{
			continue;
			//ÿ��ѭ��ǰҪ�жϸý����Ƿ��Ѿ��������Դ���ͷţ�����Ѿ��ͷţ�������
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				if ((max[j][i] - alloc[j][i]) <= available[i]) //�������ԴС��ϵͳ���õ���Դ��
				{
					m++;
				}
			}
			if (m == 3) //��m=3˵��ϵͳ��������ý��̵�����
			{
				n[nn] = j; //��¼���̺�
				finish[j] = true; //�ͷŸý���
				for (int k = 0; k < 3; k++)
				{
					available[k] += alloc[j][k]; //ϵͳ������Դ�Ļ���
				}
				j = -1; //��j=-1��Ϊ����һ�ν��н��̵ı���ʱ��Ҫ��ͷ��ʼ����ֹǰ���һЩ���̵�ʱ������Ҫ�����ڿ�����
				nn++;
				//��¼������һ�μ�¼Ҫ����һ��λ�ӿ�ʼ
			}
		}
	}
	for (int i = 0; i < 5; i++)
	{
		if (finish[i] == false)
		{
			cout << "�����ڰ�ȫ�㷨" << endl;
			//ֻҪ5��������һ��û��ͨ�����ͷ���Դ��˵����ǰϵͳ����ȫ
		}
		else
		{
			q++;
		}
	}
	if (q == 5) //���q=5˵��ϵͳ���Խ��̶��ͷ�����Դ��ϵͳ��ȫ
	{
		cout << "��ȫ��·��Ϊ";
		for (int i = 0; i < 5; i++)
		{
			cout << "P" << n[i] << "  ";      //һ��һ���ͷż�¼�����м�¼�Ľ��̺�
		}
		cout << endl;
	}
}