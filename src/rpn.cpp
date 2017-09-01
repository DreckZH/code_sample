
//#include <QtCore/QCoreApplication>
//#include <QMap>
//#include <QList>
//#include <QStringList>
//
//#include <glog/logging.h>
//
//QMap<QString,QStringList> var_exp_Map;
//
//QList<QString> opStack;
//QList<QString> rpnList;
//
//class ExpressionParser
//{
//public:
//	ExpressionParser()
//	{
//		_opPriority.insert("(",1);
//		_opPriority.insert(")",1);
//		_opPriority.insert("[",2);
//		_opPriority.insert("]",2);
//		_opPriority.insert("{",3);
//		_opPriority.insert("}",3);		
//		_opPriority.insert("!",4);
//		_opPriority.insert("*",5);
//		_opPriority.insert("/",5);		
//		_opPriority.insert("+",6);
//		_opPriority.insert("-",6);
//		_opPriority.insert("&",7);
//		_opPriority.insert("|",7);
//	};
//	~ExpressionParser(){};
//
//	QMap<QString, int> _opPriority;
//};
//
//class RPNParser : public ExpressionParser
//{
//public:
//	RPNParser();
//	~RPNParser();
//
//	void decomposeExpression(QString exp);
//	QStringList convertToRPN();
//	QString calculate();
//
////private:
//	QStringList m_expTags;
//	QStringList m_rpnTags;
//};
//
//RPNParser::RPNParser()
//{
//}
//RPNParser::~RPNParser()
//{
//	m_expTags.clear();
//}
//
//void RPNParser::decomposeExpression(QString exp)
//{	
//	if (exp.isEmpty())
//		return;
//
//	int index = -1;
//	QRegExp rx("[)(}{*/&|!+-]");
//	if (exp.contains(rx))
//	{
//		index = exp.indexOf(rx);
//		if (index == 0){
//			m_expTags.push_back(exp.left(1));
//			exp.remove(index, 1);
//		}
//		else{
//			m_expTags.push_back(exp.left(index));
//			exp.remove(0, index);
//			m_expTags.push_back(exp.left(1));
//			exp.remove(0, 1);
//		}
//	}
//	else
//	{
//		m_expTags.push_back(exp);
//		exp.clear();
//	}
//
//	decomposeExpression(exp);
//}
//
//bool isOpr(QString tag)
//{
//	return tag.contains(QRegExp("[*/&|!+-]"));
//}
//
//bool isVar(QString tag)
//{
//	return !tag.contains(QRegExp("[)(}{*/&|!+-]"));
//}
//
//void resort(QStringList oplist)
//{
//	/*QStringListIterator it1(opStack);
//	while (it1.hasNext())
//	{
//		if (_opPriority.value(tag) >= _opPriority.value(opStack.last()))
//		{
//			m_rpnTags.push_back(opStack.last());
//			opStack.pop_back();
//		}
//		else
//		{
//			opStack.push_back(tag);
//			break;
//		}
//	}*/
//}
//
//QStringList RPNParser::convertToRPN()
//{
//	QStringListIterator it(m_expTags); 
//	QStringList opStack;
//	opStack.clear();
//	bool flag = false;
//	int index=-1, index2 = -1;
//	while (it.hasNext())
//	{
//		QString tag = it.next();
//		if (isVar(tag))	//变量
//			m_rpnTags.push_back(tag);
//
//		else if (isOpr(tag))			//运算符
//		{
//			if (flag){
//				opStack.push_back(tag);
//				continue;
//			}
//			if (opStack.isEmpty() ||
//				_opPriority.value(tag) < _opPriority.value(opStack.last()))//tag的优先级高于栈顶运算符优先级
//			{
//				opStack.push_back(tag);
//				//m_rpnTags.push_back(tag);
//			}
//			else//tag的优先级低于或等于栈顶运算符优先级
//			{
//				QStringListIterator it1(opStack);
//				while (it1.hasNext())
//				{
//					if (_opPriority.value(tag) >= _opPriority.value(opStack.last()))
//					{
//						m_rpnTags.push_back(opStack.last());
//						opStack.pop_back();
//					}
//					else
//					{
//						opStack.push_back(tag);
//						break;
//					}
//				}
//			}
//		}
//		else if (tag == "(")//如果A是左括号
//		{
//			opStack.push_back(tag);//直接入栈
//			index = opStack.size();
//			flag = true;
//		}
//		else if (tag == ")")//如果是右括号
//		{
//			index2 = opStack.size();
//			m_rpnTags.append(opStack.mid(index, index2 - index));
//			opStack.move(index,index2);			
//		}
//		else
//			;
//	}
//	return m_rpnTags;
//}
//QString RPNParser::calculate()
//{
//	QString str;
//	return str;
//}
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <QDateTime>
typedef struct _tagCP56Time2a
{
	WORD	wMilSecond;
	BYTE	byMinute : 6,
	byRes1 : 1,
		 bInvalid : 1;
	BYTE	byHour : 5,
	byRes2 : 2,
		 bSU : 1;
	BYTE	byDay : 5,
	byDayOfWeek : 3;
	BYTE	byMonth : 4,
	byRes3 : 4;
	BYTE	byYear : 7,
	byRes4 : 1;

	
	void setTime(qint64 time)
	{
		QDateTime	dt = QDateTime::fromMSecsSinceEpoch(time);		
		wMilSecond = dt.time().msec();
		byMinute = dt.time().minute();
		byHour = dt.time().hour();
		byDay = dt.date().day();
		byMonth = dt.date().month();
		byYear = dt.date().year();
		byRes1 = byRes2 = byRes3 = byRes4 = bInvalid = byDayOfWeek = bSU = 0;
	}

}*PCP56Time2a, CP56Time2a;

#include <iostream>
int main(int argc, char *argv[])
{
	//QCoreApplication a(argc, argv);
	//RPNParser rpn;
	//rpn.decomposeExpression("awd+be-c.wej.i$j*(5d+e1)");
	//rpn.m_expTags;
	////rpn.convertToRPN();
	////rpn.m_rpnTags;

	//return a.exec();
	QDateTime	dt = QDateTime::currentDateTime();
	qint64 time = dt.toMSecsSinceEpoch();
	CP56Time2a tm;
	tm.setTime(time);
	return 0;
}