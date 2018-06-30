#pragma once
#pragma once
#ifndef _GBLWT_H //如果没有定义这个宏 
#define _GBLWT_H //定义这个宏

#include"GlobalHead.h"
#include"Notation.h"
#include"IDG.h"


double GBLWT_GP(int n, vector<int> p1, vector<int> p2, vector<int> s1, vector<int> r, int W, int B);
typedef map<string, Object> Environment;
string code = "";
#endif