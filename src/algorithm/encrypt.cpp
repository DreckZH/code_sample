
#include <stdio.h>
#include <stdlib.h>
#include<string>

#define C1 8192/*52845*/
#define C2 8773/*22719*/
#define C4 65
#define C5 26

std::string Encrypt(std::string plaintext, int Key) // ���ܺ���
{
	std::string Result(""), str(""), S("");
	int i = 0, j = 0;

	for (i = 0; i<plaintext.length(); i++) // ���ζ��ַ����и��ַ����в���
	{
		Result.push_back(plaintext.at(i) ^ (Key >> 8));// ����Կ��λ�����ַ����		
		Key = (Result.at(i) + Key)*C1 + C2; // ������һ����Կ
	}
	S = Result;
	Result.clear();
	std::string t;
	for (i = 0; i<S.length(); i++) // �Լ��ܽ������ת��
	{
		j = S.at(i); // ��ȡ�ַ�
		// ���ַ�ת��Ϊ������ĸ����
		str = "XX"; // ����str����Ϊ2
		str[0] = (C4 + j / C5);//���ｫ65�Ĵ���������256�����ľͻ�����룬Ч�����ã���Ӧ�ģ����ܴ�Ҫ��Ϊ��ͬ����
		str[1] = (C4 + j % C5);
		Result += str;
	}
	return Result;
}



std::string Decrypt(std::string ciphertext, int Key)// ���ܺ���
{
	std::string Result(""), str(""), S("");
	int i = 0, j = 0;

	for (i = 0; i < ciphertext.length() / 2; i++) // ���ַ���������ĸһ����д���
	{
		j = (ciphertext.at(2 * i) - C4) * C5;//��Ӧ�ģ����ܴ�Ҫ��Ϊ��ͬ����
		j += ciphertext.at(2 * i + 1) - C4;
		str = "X"; // ����str����Ϊ1
		str[0] = j;
		Result += str; // ׷���ַ�����ԭ�ַ���
	}
	S = Result;
	Result.clear();

	for (i = 0; i<S.length(); i++) // ���ζ��ַ����и��ַ����в���
	{
		Result.push_back((S[i] ^ (Key >> 8))); // ����Կ��λ�����ַ����
		Key = (S[i] + Key)*C1 + C2; // ������һ����Կ
	}
	return Result;
}