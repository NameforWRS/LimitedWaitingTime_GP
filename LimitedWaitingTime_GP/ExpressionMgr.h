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
	ExpressionMgr(double _RT,double _FPT, double _SPT, double _FSUMPT, double _FMAXPT, double _FMINPT, double _SSUMPT,
		double _SMAXPT, double _SMINPT, double _RFSUMPT, double _RFMAXPT, double _RFMINPT, double _RSSUMPT, double _RSMAXPT,
		double _RSMINPT, double _PTB, double _FSUMPTB, double _SSUMPTB, double _FMINPTB, double _SMINPTB, double _SMAXPTB, double _PTS,
		double _RSUMPTS, double _RMINPTS, double _RMAXPTS);
	ExpressionMgr(string &_value, ExpressionMgr* &_parent);
	~ExpressionMgr();

	string Value;
	ExpressionMgr *Parent;
	vector<ExpressionMgr *> Children;

	double RT;
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
	double PTB;
	double FSUMPTB;
	double SSUMPTB;
	double FMINPTB;
	double SMINPTB;
	double SMAXPTB;
	double PTS;
	double RSUMPTS;
	double RMINPTS;
	double RMAXPTS;
	


	ExpressionMgr * ParseAsIScheme(string _code);
	vector<string> Tokenize(string code);
	string InsertBlank(string code);
	
	

private:

};


