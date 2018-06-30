#pragma once

#ifndef _NOTATION_H //如果没有定义这个宏 
#define _NOTATION_H //定义这个宏
#include"GlobalHead.h"

//定义生产环境
class ENV
{
public:
	int n;
	vector<int> p1;
	vector<int> p2;
	vector<int> s1;
	vector<int> r;
	int W;
	int B;
	int A;
	int D;
	int C;
	int F;
	int G;
	clock_t start_time;
	clock_t end_time;
	int time_limit;
};
//定义抗体个体
class Antibody
{
public:
	//antibody 中包含的id序列
	vector<int> jobid;
	//bestobj
	double obj;
	//antibody的目标值
	//double GetSumCompletofSB(vector<int>& jobid,ENV& env);
	//double GetSumCompletofBB(vector<int>& jobid, ENV& env);
};
//定义种群
class Population
{
public:
	//每代所包含的antibody个体
	vector<Antibody> pop;
	//最佳抗体
	Antibody bestbody;
};
//定义仿真结果
class Generation
{
public:
	//最佳个体
	vector<Antibody> Bestbody;
	//最佳目标值
	vector<double> Bestobj;
};
//定义组批
class Batch
{
public:
	int capacity;
	int sumofsecprocess;
	int maxsecprocess;
	int releasetime;
	int timeofbatchprocess;
	int sumofbatchsec;
	vector<int> jobid;
};
//定义缓冲区
class Buffer
{
public:
	vector<int> jobid;
	int getSPTnum(Buffer& buffer,ENV& env);
};
#endif 