
#include "utilitys.h"

bool is_big_endian()
{
	int endian = 1;
	char* p = (char*)&endian;
	if (p[0] == 0)	{
		return true;
	}
	else{
		return false;
	}
}

std::string Encrypt(std::string plaintext, int Key) 
{
	std::string Result(""), str(""), S("");
	unsigned int i = 0, j = 0;
	for (i = 0; i < plaintext.length(); i++) // 依次对字符串中各字符进行操作
	{
		Result.push_back(plaintext.at(i) ^ (Key >> 8));// 将密钥移位后与字符异或		
		Key = (Result.at(i) + Key)*C1 + C2; // 产生下一个密钥
	}
	S = Result;
	Result.clear();
	std::string t;
	for (i = 0; i < S.length(); i++) // 对加密结果进行转换
	{
		j = S.at(i); // 提取字符
		// 将字符转换为两个字母保存
		str = "XX"; // 设置str长度为2
		str[0] = (C4 + j / C5);//这里将65改大点的数例如256，密文就会变乱码，效果更好，相应的，解密处要改为相同的数
		str[1] = (C4 + j % C5);
		Result += str;
	}
	return Result;
}

std::string Decrypt(std::string ciphertext, int Key)// 解密函数
{
	std::string Result(""), str(""), S("");
	unsigned int i = 0, j = 0;

	for (i = 0; i < ciphertext.length() / 2; i++) // 将字符串两个字母一组进行处理
	{
		j = (ciphertext.at(2 * i) - C4) * C5;//相应的，解密处要改为相同的数
		j += ciphertext.at(2 * i + 1) - C4;
		str = "X"; // 设置str长度为1
		str[0] = j;
		Result += str; // 追加字符，还原字符串
	}
	S = Result;
	Result.clear();

	for (i = 0; i<S.length(); i++) // 依次对字符串中各字符进行操作
	{
		Result.push_back((S[i] ^ (Key >> 8))); // 将密钥移位后与字符异或
		Key = (S[i] + Key)*C1 + C2; // 产生下一个密钥
	}
	return Result;
}

template<class T, class M> T*  CSingleton<T,M>::_instance = 0;

template<class T, class M> M*  CSingleton<T, M>::mutext = 0;

template<class T, class M> T*  CSingleton<T, M>::Instance(void)
{
	if (_instance == 0)
	{
		mutext->lock();//双重锁定
		if (_instance == 0)
		{
			_instance = new T;
		}
		mutext->unlock();
	}
	return _instance;
}

template<class T, class M> void CSingleton<T, M>::Close(void)
{
	if (_instance)
	{		
		delete _instance;

		if(mutext)
			delete mutext;
	}
}

