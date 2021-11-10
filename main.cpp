#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <string.h>
#include "instances.hpp"

using namespace std;

bool ForcaBruta (const char *P, const char *T, int *O)
{
	int i = 0, j = 0;

	bool flag = false;
	while (T[i + j] != '\0')
	{
		if (T[i + j] == P[j])
		{
			j++;
			if (P[j] == '\0')
			{
				if (O != nullptr)
				{
					*O = i;
					O++;
				}
				flag = true;
			}
		}
		else
		{
			if (j != 0)
				j = 0;
			i++;
		}
	}
	if (O != nullptr)
		*O = -1;
	return flag;
}

void kmp_lps (const char *P, int *lps)
{
	int len = 0;

	lps[0] = 0;

	int i = 1;
	while (P[i] != '\0') 
	{
		if (P[i] == P[len]) 
		{
			len++;
			lps[i] = len;
			i++;
		}
		else
		{
			if (len != 0) 
			{
				len = lps[len - 1];
			}
			else
			{
				lps[i] = 0;
				i++;
			}
		}
	}
}

bool kmp_find (const char *P, const char *T, int *O)
{
	int *lps = (int *)malloc(sizeof(int) * strlen(P));
	if (lps == nullptr)
	{
		return false;
	}

	kmp_lps(P, lps);
	
	int i = 0;
	int j = 0;
	while (T[i] != '\0') 
	{
		if (P[j] == T[i]) 
		{
			j++;
			i++;
		}

		if (P[j] == '\0') 
		{
			if (O != nullptr)
			{
				*O = i - j;
				O++;
			}
			j = lps[j - 1];
		}

		else if (T[i] != '\0' && P[j] != T[i]) 
		{
			if (j != 0)
				j = lps[j - 1];
			else
				i = i + 1;
		}
	}
	if (O != nullptr)
	{
		*O = -1;
	}
	free(lps);
	return true;
}

void gerar_instancia_aleatoria (char l, int m, int n, char *P, char *T)
{
	for (int i = 0; i < m; i++)
	{									//int('a') = 97
		int dif = int(l) - 96; 			//dif = int(l) - 97 - 1
		*P = char((rand() % dif) + 97);	
		P++;
	}

	for (int i = 0; i < n; i++)
	{									//int('a') = 97
		int dif = int(l) - 96;			//dif = int(l) - 97 - 1
		*T = char((rand() % dif) + 97);
		T++;
	}
}

bool tempoGastoA (char l, int m, int n, int k)
{
	float tot1 = 0, tot2 = 0;

	std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;

	for (int i = 0; i < k; i++)
	{
		int *O1 = (int *)malloc(sizeof(int) * (n + 1));
		int *O2 = (int *)malloc(sizeof(int) * (n + 1));
		char *P = (char *)malloc(sizeof(int) * (n + 1));
		char *T = (char *)malloc(sizeof(int) * (n + 1));

		if (O1 == nullptr || O2 == nullptr)
		{
			cout << "Erro na alocacao";
			return false;
		}

		gerar_instancia_aleatoria(l, m, n, P, T);

		t1 = std::chrono::high_resolution_clock::now();
		ForcaBruta(P, T, O1);
		std::chrono::duration<float> d1 = std::chrono::high_resolution_clock::now() - t1;
		tot1 = tot1 + d1.count();

		t2 = std::chrono::high_resolution_clock::now();
		kmp_find(P, T, O2);
		std::chrono::duration<float> d2 = std::chrono::high_resolution_clock::now() - t2;
		tot2 = tot2 + d2.count();

		int cont = 0;
		for (int j = 0; O1[j] != -1; j++)
		{
			if (O1[j] != O2[j])
			{
				cout << "O's errados\n";
				return false;
			}
			else if (O1[j] == O2[j])cont++;
		}
		/*cout << "P = ";
		for (int j = 0; j < m; j++)
		{
			cout << P[j] << " ";	
		}
		cout << "\nT = ";
		for (int j = 0; j < n; j++)
		{
			cout << T[j] << " ";
		} 
		cout << "\ncont = " << cont << "\n";*/
		free(O1);
		free(O2);
		free(P);
		free(T);
	}
	cout << "ForcaBruta = " << tot1 << "\n";
	cout << "kmp_find = " << tot2 << "\n";
	return true;
}

bool tempoGastoR (int x, int y)
{
	float tot1 = 0, tot2 = 0;

	std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;

	for (int i = x; i <= y; i++)
	{
		int *O1 = (int *)malloc(sizeof(int) * strlen(Texto_Livros + 1));
		int *O2 = (int *)malloc(sizeof(int) * strlen(Texto_Livros + 1));

		if (O1 == nullptr || O2 == nullptr)
		{
			cout << "Erro na alocacao";
			return false;
		}

		t1 = std::chrono::high_resolution_clock::now();
		ForcaBruta(Padroes_Palavras[i], Texto_Livros, O1);
		std::chrono::duration<float> d1 = std::chrono::high_resolution_clock::now() - t1;
		tot1 = tot1 + d1.count();

		t2 = std::chrono::high_resolution_clock::now();
		kmp_find(Padroes_Palavras[i], Texto_Livros, O2);
		std::chrono::duration<float> d2 = std::chrono::high_resolution_clock::now() - t2;
		tot2 = tot2 + d2.count();

		int cont = 0;
		for (int j = 0; O1[j] != -1; j++)
		{
			if (O1[j] != O2[j])
			{
				cout << "O's errados\n";
				return false;
			}
			else if (O1[j] == O2[j])cont++;
		}
		free(O1);
		free(O2);
		// cout << "P = " << Padroes_Palavras[i] << "\n" << "cont = " << cont << "\n\n";
	}
	cout << "ForcaBruta = " << tot1 << "\n";
	cout << "kmp_find = " << tot2 << "\n";
	return true;
}

int main(int argc, char *argv[])
{
	srand(time(NULL));

	if (argc == 6 && argv[1][0] == 'A' && int(argv[2][0]) >= 97 && int(argv[2][0]) <= 122)
	{
		
		char l = argv[2][0];
		int m = atoi(argv[3]);
		int n = atoi(argv[4]);
		int k = atoi(argv[5]);

		tempoGastoA(l, m, n, k);
	}
	else if (argc == 4 && argv[1][0] == 'R' && atoi(argv[2]) >= 0 && atoi(argv[2]) <= 35129 && atoi(argv[3]) >= 0 && atoi(argv[3]) <= 35129)
	{
		int x = atoi(argv[2]);
		int y = atoi(argv[3]);

		cout << "Calculando...\n";
		tempoGastoR(x, y);
	}
	else cout << "Entrada invalida\n";
}
