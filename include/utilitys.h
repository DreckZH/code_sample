
#ifndef __USER_DEFINED_UTILILTIES_H_INCLUDED__
#define __USER_DEFINED_UTILILTIES_H_INCLUDED__
#include <stdio.h>
#include <stdlib.h>
#include<string>

#define C1 8192/*52845*/
#define C2 8773/*22719*/
#define C4 65
#define C5 26


bool is_big_endian();//检查大小端模式

std::string Encrypt(std::string plaintext, int Key); // 加密函数;
std::string Decrypt(std::string ciphertext, int Key);// 解密函数

//单件类模板
template<class T,class M>
class CSingleton
{
private:
	CSingleton(void){ mutext = new M; }

	//禁止复制和赋值
	CSingleton(const CSingleton&){}
	CSingleton& operator=(const CSingleton&){ return *this; }

	static T* _instance;
	static M* mutext;

public:
	static T* Instance(void);
	static void Close(void);
};

//延迟实例化的单件类
class SingleTon
{
public:
	static SingleTon *Instance()
	{
		if (0 == p_instance_) {
			static SingleTon instance;
			p_instance_ = &instance;

		}
		return p_instance_;
	}


private:
	SingleTon() {}
	~SingleTon() {}
	static SingleTon *p_instance_;

};

#endif
