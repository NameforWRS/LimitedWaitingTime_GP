#pragma once
#pragma once
#ifndef _GBLWT_H //如果没有定义这个宏 
#define _GBLWT_H //定义这个宏

#include"GlobalHead.h"
#include"Notation.h"
#include"IDG.h"
typedef map<string, Object> Environment;

string eval(ExpressionMgr * &tree, Environment &env, string FPT, string SPT, string FSUMPT, string FMAXPT, string FMINPT, string SSUMPT, string SMAXPT, string SMINPT, string RFSUMPT, string RFMAXPT, string RFMINPT, string RSSUMPT, string RSMAXPT, string RSMINPT);
double GBLWT_GP(int n, vector<int> p1, vector<int> p2, vector<int> s1, vector<int> r, int W, int B, ExpressionMgr * scheme);
int Findbestpriority(ExpressionMgr * scheme, Batch formedbatch, vector<int>& unshceduledjobs, ENV env);
#endif