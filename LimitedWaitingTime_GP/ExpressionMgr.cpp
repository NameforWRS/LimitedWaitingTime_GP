#include "ExpressionMgr.h"
#include <iostream>
#include <sstream>

using namespace std;

ExpressionMgr::ExpressionMgr()
{
}

ExpressionMgr::ExpressionMgr(double _RT,double _FPT, double _SPT, double _FSUMPT, double _FMAXPT, double _FMINPT, double _SSUMPT, 
	double _SMAXPT, double _SMINPT, double _RFSUMPT, double _RFMAXPT, double _RFMINPT, double _RSSUMPT, double _RSMAXPT, 
	double _RSMINPT, double _PTB,double _FSUMPTB,double _SSUMPTB,double _FMINPTB,double _SMINPTB,double _SMAXPTB,double _PTS,
	double _RSUMPTS,double _RMINPTS,double _RMAXPTS)
{
	RT = _RT;
	FPT = _FPT;
	SPT = _SPT;
	FSUMPT = _FSUMPT;
	FMAXPT = _FMAXPT;
	FMINPT = _FMINPT;
	SSUMPT = _SSUMPT;
	SMAXPT = _SMAXPT;
	SMINPT = _SMINPT;
	RFSUMPT = _RFSUMPT;
	RFMAXPT = _RFMAXPT;
	RFMINPT = _RFMINPT;
	RSSUMPT = _RSSUMPT;
	RSMAXPT = _RSMAXPT;
	RSMINPT = _RSMINPT;
	PTB = _PTB;
	FSUMPTB = _FSUMPTB;
	SSUMPTB = _SSUMPTB;
	FMINPTB = _FMINPTB;
	SMINPTB = _SMINPTB;
	SMAXPTB = _SMAXPTB;
	PTS = _PTS;
	RSUMPTS = _RSUMPTS;
	RMINPTS = _RMINPTS;
	RMAXPTS = _RMAXPTS;
}


ExpressionMgr::ExpressionMgr(string &_value, ExpressionMgr* &_parent)
{
	Value = _value;
	Parent = _parent;
	Children.clear();
}

ExpressionMgr::~ExpressionMgr()
{
}

///将表达式转化为树
ExpressionMgr * ExpressionMgr::ParseAsIScheme(string code)
{
	string str = "";
	ExpressionMgr* ptrprogram = new ExpressionMgr();

	vector<string> result = Tokenize(code);
	for (vector<string>::iterator vi = result.begin(); vi != result.end(); vi++)
	{
		if (*vi == "(")
		{
			str = "(";
			ExpressionMgr* node = new ExpressionMgr(str, ptrprogram);
			ptrprogram->Children.push_back(node);
			ptrprogram = node;
		}
		else if (*vi == ")")
		{
			ptrprogram = ptrprogram->Parent;
		}
		else
		{
			ExpressionMgr* newNode = new ExpressionMgr(*vi, ptrprogram);
			ptrprogram->Children.push_back(newNode);
		}
	}
	return ptrprogram;
}

////将表达式转化为字符串vector
vector<string> ExpressionMgr::Tokenize(string code)
{
	code = InsertBlank(code);
	vector<string> result;
	string temp("");

	for (int i = 0; i< code.size(); i++)
	{
		if (code[i] == '\t' || code[i] == '\r' || code[i] == '\n' || code[i] == ' ')
		{
			if (temp.size() > 0)
				result.push_back(temp);
			temp = "";
		}
		else
		{
			temp += code[i];
		}
	}
	return result;
}

////为了方便拆分，在左右括号 ( )的前后各插入一个空格
string ExpressionMgr::InsertBlank(string code)
{
	string strfront = "";
	string strend = "";
	for (int i = 0; i<code.size(); i++)
	{
		if (code[i] == '(')
		{
			strfront = code.substr(0, i);
			strend = code.substr(i + 1, code.size() - i);
			code = strfront + " ( " + strend;
			i += 2;
		}
		if (code[i] == ')')
		{
			strfront = code.substr(0, i);
			strend = code.substr(i + 1, code.size() - i);
			code = strfront + " ) " + strend;
			i += 2;
		}
		strfront = "";
		strend = "";
	}
	return code;
}