#pragma once

#ifndef _IDG_H //如果没有定义这个宏 
#define _IDG_H //定义这个宏
#include"GlobalHead.h"

void IDG_1(vector<int> &n, vector<vector<int>>& p1, vector<vector<int>>& p2, vector<vector<int>>& s1, vector<vector<int>>& r, vector<int> &W, vector<int> &B, long seed);
void IDG_2(vector<int> &n, vector<vector<int>>& p1, vector<vector<int>>& p2, vector<vector<int>>& s1, vector<vector<int>>& r, vector<int> &W, vector<int> &B, long seed);
int uniform(int l, int u,long seed);
#endif