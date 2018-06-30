#pragma once
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <vector>

using namespace std;

class ExpressionMgr
{
public:
	ExpressionMgr();
	ExpressionMgr(double _FPT, double _SPT, double _FSUMPT, double _FMAXPT, double _FMINPT, double _SSUMPT, double _SMAXPT, double _SMINPT, double _RFSUMPT, double _RFMAXPT, double _RFMINPT, double _RSSUMPT,double _RSMAXPT, double _RSMINPT);
	ExpressionMgr(string &_value, ExpressionMgr* &_parent);
	~ExpressionMgr();

	string Value;
	ExpressionMgr *Parent;
	vector<ExpressionMgr *> Children;

	double FPT;
	double SPT;
	double FSUMPT;
	double FMAXPT;
	double FMINPT;
	double SSUMPT;
	double SMAXPT;
	double SMINPT;
	double RFSUMPT;
	double RFMAXPT;
	double RFMINPT;
	double RSSUMPT;
	double RSMAXPT;
	double RSMINPT;
	


	void DeleteNode(ExpressionMgr * &tree);
	ExpressionMgr * ParseAsIScheme(string _code);
	vector<string> Tokenize(string code);
	string InsertBlank(string code);
	vector<string> Replace(vector<string> _result);
	
	

private:

};


