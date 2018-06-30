#include "ExpressionMgr.h"
#include <iostream>
#include <sstream>

using namespace std;

ExpressionMgr::ExpressionMgr()
{
}

ExpressionMgr::ExpressionMgr(double _FPT, double _SPT, double _FSUMPT, double _FMAXPT, double _FMINPT, double _SSUMPT, double _SMAXPT, double _SMINPT, double _RFSUMPT, double _RFMAXPT, double _RFMINPT, double _RSSUMPT, double _RSMAXPT, double _RSMINPT)
{
	FPT=_FPT;
	SPT=_SPT;
	FSUMPT=_FSUMPT;
	FMAXPT=_FMAXPT;
	FMINPT=_FMINPT;
	SSUMPT=_SSUMPT;
	SMAXPT=_SMAXPT;
	SMINPT=_SMINPT;
	RFSUMPT=_RFSUMPT;
	RFMAXPT=_RFMAXPT;
	RFMINPT=_RFMINPT;
	RSSUMPT=_RSSUMPT;
	RSMAXPT=_RSMAXPT;
	RSMINPT=_RSMINPT;
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

void ExpressionMgr::DeleteNode(ExpressionMgr * &tree)
{
	if (tree == NULL)
		return;
	//delete tree->Parent;
	for (int i = 0; i < tree->Children.size(); i++)
	{
		DeleteNode(tree->Children[i]);
	}
	delete tree;
	tree = NULL;
	return;
}

///将表达式转化为树
ExpressionMgr * ExpressionMgr::ParseAsIScheme(string code)
{
	string str = "";
	ExpressionMgr* current = new ExpressionMgr();
	//ExpressionMgr *node("",NULL);

	//ExpressionMgr *current =ptrprogram;

	vector<string> result = Tokenize(code);
	for (vector<string>::iterator vi = result.begin(); vi != result.end(); vi++)
	{
		if (*vi == "(")
		{
			str = "(";
			ExpressionMgr* node = new ExpressionMgr(str, current);
			current->Children.push_back(node);
			current = node;
			//DeleteNode(node);
		}
		else if (*vi == ")")
		{
			current = current->Parent;
		}
		else
		{
			ExpressionMgr* newNode = new ExpressionMgr(*vi, current);
			current->Children.push_back(newNode);
			//DeleteNode(newNode->Parent);
		}
	}
	//DeleteNode(current);

	current->Children[0]->FPT = FPT;
	current->Children[0]->SPT = SPT;
	current->Children[0]->FSUMPT = FSUMPT;
	current->Children[0]->FMAXPT = FMAXPT;
	current->Children[0]->FMINPT = FMINPT;
	current->Children[0]->SSUMPT = SSUMPT;
	current->Children[0]->SMAXPT = SMAXPT;
	current->Children[0]->SMINPT = SMINPT;
	current->Children[0]->RFSUMPT = RFSUMPT;
	current->Children[0]->RFMAXPT = RFMAXPT;
	current->Children[0]->RFMINPT = RFMINPT;
	current->Children[0]->RSSUMPT = RSSUMPT;
	current->Children[0]->RSMAXPT = RSMAXPT;
	current->Children[0]->RSMINPT = RSMINPT;

	return current;
}

////将表达式转化为字符串vector
vector<string> ExpressionMgr::Tokenize(string code)
{
	 code = InsertBlank(code);
	 vector<string> result;
	 string temp("");

	 for(int i =0; i< code.size();i++)
	 {
		if(code[i] == '\t' || code[i] == '\r' || code[i] == '\n' || code[i] == ' ')
		{
			if(temp.size() > 0)
				result.push_back(temp);
			temp="";
		}
		else
		{
			temp += code[i];
		}
	 }

	
	 ////如果if iflez和括号的关系是反的，则调换过来
	/* for(vector<string>::iterator vi = result.begin();vi!=result.end();vi++)
	 {
		 if(*vi =="if")
		 {
			 *vi ="(";
			 *(vi+1)="if";
			 vi++;
		 }
		 if(*vi == "iflez")
		 {
			*vi="(";
			*(vi+1)="iflez";
			vi++;
		 }
	 }*/

	 return Replace(result);
}

////为了方便拆分，在左右括号 ( )的前后各插入一个空格
string ExpressionMgr::InsertBlank(string code)
{
	string strfront="";
	string strend="";
	for(int i =0;i<code.size(); i++)
	{
		if(code[i] == '(')
		{
			strfront = code.substr(0,i);
			strend = code.substr(i+1,code.size() - i);
			code = strfront +" ( " +strend;
			i+=2;
		}
		if(code[i] == ')')
		{
			strfront = code.substr(0,i);
			strend = code.substr(i+1,code.size() - i);
			code = strfront +" ) " +strend;
			i+=2;
		}
		strfront ="";
		strend="";
	}
	return code;
}

////替换字符串中的RM等
vector<string> ExpressionMgr::Replace(vector<string> _result)
{
	for(vector<string>::iterator vi = _result.begin();vi!=_result.end();vi++)
	{
		stringstream ss;
		string str;

		if (*vi == "FPT")
		{
			ss << FPT;
			ss >> str;
			*vi = str;
		}

		else if (*vi == "SPT")
		{
			ss << SPT;
			ss >> str;
			*vi = str;
		}
		else if (*vi == "FSUMPT")
		{
			ss << FSUMPT;
			ss >> str;
			*vi = str;
		}
		else if (*vi == "FMAXPT")
		{
			ss << FMAXPT;
			ss >> str;
			*vi = str;
		}
		
		else if(*vi =="FMINPT")
		{
			ss<< SPT;
			ss>>str;
			*vi = str;
		}
		else if (*vi == "SSUMPT")
		{
			ss << SSUMPT;
			ss >> str;
			*vi = str;
		}
		else if (*vi == "SMAXPT")
		{
			ss << SMAXPT;
			ss >> str;
			*vi = str;
		}
		else if (*vi == "SMINPT")
		{
			ss << SMINPT;
			ss >> str;
			*vi = str;
		}
		else if (*vi == "RFSUMPT")
		{
			ss << RFSUMPT;
			ss >> str;
			*vi = str;
		}
		else if (*vi == "RFMAXPT")
		{
			ss << RFMAXPT;
			ss >> str;
			*vi = str;
		}
		else if (*vi == "RFMINPT")
		{
			ss << RFMINPT;
			ss >> str;
			*vi = str;
		}
		else if (*vi == "RSSUMPT")
		{
			ss << RSSUMPT;
			ss >> str;
			*vi = str;
		}
		else if (*vi == "RSMAXPT")
		{
			ss << RSMAXPT;
			ss >> str;
			*vi = str;
		}
		else if (*vi == "RSMINPT")
		{
			ss << RSMINPT;
			ss >> str;
			*vi = str;
		}
	}
	return _result;
}

