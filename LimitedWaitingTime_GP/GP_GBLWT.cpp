#include"GBLWT.h"

//bool operator <(const Batch &batch1, const Batch &batch2)
//{
//	return true;
//}
//为直接使用map容器的性质，直接定义对比法则，在构建map的同时，直接对组批的releasetime进行排序
/*
如map形式为
BATCH1(RELEASETIME=5)
BATCH2(RELEASETIME=6)
BATCH3(RELEASETIME=3)
BATCH4(RELEASETIME=5)
使用对比法则后，map可直接变成
BATCH3 3
BATCH1 1
BATCH4 4
BATCH2 2
*/
struct CmpByIncr {
	bool operator()(const int k1, const int k2) const
	{
		return k1 < k2;
	}
};

struct CmpByPj {
	bool operator()(const Batch k1, const Batch k2) const
	{
		return k1.sumofsecprocess > k2.sumofsecprocess;
	}
};
struct CmpByPjtj {
	bool operator()(const Batch k1, const Batch k2) const
	{
		return k1.sumofbatchsec > k2.sumofbatchsec;
	}
};

double GBLWT_GP(int n, vector<int> p1, vector<int> p2, vector<int> s1, vector<int> r, int W, int B)
{
	//导入派单规则
	string code1 = "";
	code1 = "(* FPT SPT)";

	ExpressionMgr mgr;

	///一头一尾加上括号，方便处理
	code = code1;
	if (code[0] != '(') {
		if (code.size() > 6) {
			code = "(" + code + ")";
		}
		else {
			code = "(* " + code + ")";
		}
	}

	///将表达式转换为树
	ExpressionMgr * scheme = mgr.ParseAsIScheme(code);


	ENV env;
	env.p2 = p2;
	//Step 1.1 Sort the job
	multimap<int, int, CmpByIncr> Sequence_Job;//用multimap构建自动对p1进行排序的sequence_Job

	for (int i = 0; i < p1.size(); i++)
	{
		Sequence_Job.insert(make_pair(p1[i], i));//
	}
	vector<int> job;
	for (multimap<int, int>::iterator iter = Sequence_Job.begin(); iter != Sequence_Job.end(); ++iter)
	{
		job.push_back((*iter).second);
	}
	//Initialization
	vector<Batch> FormedBatch;
	vector<int> unsheduledjobs;
	unsheduledjobs = job;

	/*////////////////////////////////////////////////
	Step 1 Group the jobs into batches
	*////////////////////////////////////////////////
	while (unsheduledjobs.size() > 0)
	{
		Batch CurrentBatch;//重新生成一批
		CurrentBatch.maxsecprocess = 0;
		CurrentBatch.capacity = 0;
		CurrentBatch.sumofsecprocess = 0;
		CurrentBatch.timeofbatchprocess = 0;
		vector<int> record_earse;//记录unscheduledjobs被组批后的位置
		for (int ind = 1; ind <= env.B; ind++) 
		{
			int best_ind_job = Findbestpriority(scheme, CurrentBatch, env);//发现最佳的组批job
			CurrentBatch.jobid.push_back(best_ind_job);//将满足的job加入新批中
			CurrentBatch.sumofsecprocess += p2[best_ind_job];
			CurrentBatch.maxsecprocess = std::max(p2[best_ind_job], CurrentBatch.maxsecprocess);
			CurrentBatch.timeofbatchprocess = std::max(p1[best_ind_job], CurrentBatch.timeofbatchprocess);
			CurrentBatch.sumofbatchsec = CurrentBatch.sumofsecprocess + CurrentBatch.timeofbatchprocess;
			CurrentBatch.capacity++;
			record_earse.push_back(best_ind_job);//记录job位置
		}
		for (int i = 0; i < record_earse.size(); i++)
		{
			unsheduledjobs.erase(unsheduledjobs.begin() + std::distance(std::begin(unsheduledjobs), find(unsheduledjobs.begin(), unsheduledjobs.end(), record_earse[i])));//去除unscheduledjob已组批的job
		}
		FormedBatch.push_back(CurrentBatch);//所有组好的批全部进入FormedBatch中
	}

	/*////////////////////////////////////////////////
	Step 2 Sequence Batches Formed
	*////////////////////////////////////////////////

	//Step2.1 Caculate the release time of batch
	//Step2.2 Sequence the release time

	vector<int> ReleaseTime_T;//计算每批job的释放时间集合，为得到每批释放时间提供支持
	multimap<Batch, int, CmpByPj> Sequence_Batch1;//用multimap构建自动对releasetime进行排序的sequence
	for (int i = 0; i < FormedBatch.size(); i++)
	{
		Sequence_Batch1.insert(make_pair(FormedBatch[i], i));//得到Batch和释放时间顺序的map集合，第一个sequence就是最早释放的
	}
	//获得依次加工的batch
	vector<Batch> AdjustedBatch1;
	for (multimap<Batch, int>::iterator iter = Sequence_Batch1.begin(); iter != Sequence_Batch1.end(); ++iter)
	{
		AdjustedBatch1.push_back((*iter).first);
	}
	/*////////////////////////////////////////////////
	Step 3 Calculate the sum of job completion time
	*////////////////////////////////////////////////
	int SimulatedTime = 0;//initial time is 0
	int finishedjob = 0;//the finished job number
	int finishedbatch = 1;//the finished batch number
	int currentbatch = 0;// the current batch number
	int TimeofStep1 = AdjustedBatch1[0].timeofbatchprocess;
	int spaceofbuffer = 0;
	Buffer buffer1;//the current jobs in the buffer
	vector<int> CmpofBatch1;
	CmpofBatch1.push_back(0);//第0批完成时间
	int Obj1 = 0;
	vector<vector<int>> CmpofJob1;
	while (1)
	{
		if (SimulatedTime >= std::max(TimeofStep1, CmpofBatch1[finishedbatch - 1]) && spaceofbuffer == 0)//when Step 1 is finished and buffer is empty, namely the batch is released
		{
			for (int i = 0; i < AdjustedBatch1[currentbatch].jobid.size(); i++)
			{
				buffer1.jobid.push_back(AdjustedBatch1[currentbatch].jobid[i]);
			}
			spaceofbuffer = buffer1.jobid.size();//the job in the batch is released
			currentbatch += 1;//we put the next batch into batch processor
			if (currentbatch < AdjustedBatch1.size())
			{
				TimeofStep1 = SimulatedTime + AdjustedBatch1[currentbatch].timeofbatchprocess;//boom! The batch is finished
			}
		}
		if (spaceofbuffer>0 && finishedjob <= n)//when the Step 2 is available and buffer has jobs and the process is continued
		{
			while (buffer1.jobid.size() > 0)
			{
				int goin = buffer1.getSPTnum(buffer1, env);
				buffer1.jobid.erase(buffer1.jobid.begin() + std::distance(std::begin(buffer1.jobid), find(buffer1.jobid.begin(), buffer1.jobid.end(), goin)));//去除buffer中已加工的job																															 //在离散机器加工
				vector<int> tmpcmp;
				tmpcmp.push_back(goin);
				SimulatedTime += p2[goin];
				Obj1 += SimulatedTime;//record the objective function
				tmpcmp.push_back(SimulatedTime);
				CmpofJob1.push_back(tmpcmp);//完成了
				finishedjob++;//计数+1				
			}
			spaceofbuffer = buffer1.jobid.size();//buffer is empty
			CmpofBatch1.push_back(SimulatedTime);
			finishedbatch++;
		}
		SimulatedTime++;
		if (finishedjob == n)
		{
			break;
		}
	}
	int OBJ1 = Obj1;
	return std::max(OBJ1, 0);
}

double Interpreter(ExpressionMgr * scheme, string FPT, string SPT, string FSUMPT, string FMAXPT, string FMINPT, string SSUMPT, string SMAXPT, string SMINPT, string RFSUMPT, string RFMAXPT, string RFMINPT, string RSSUMPT, string RSMAXPT, string RSMINPT)
{
	////做函数指针进行映射相应的逻辑操作 + - * /等
	Environment env;
	env.insert(map<string, Object>::value_type("+", add));
	env.insert(map<string, Object>::value_type("-", diff));
	env.insert(map<string, Object>::value_type("*", prod));
	env.insert(map<string, Object>::value_type("div", divv));
	env.insert(map<string, Object>::value_type("If", func_if));
	env.insert(map<string, Object>::value_type("max", func_max));


	////采取后序遍历，逐层进行计算，并返回计算的值
	string res = "";
	res = eval(scheme->Children[0], env, FPT, SPT, FSUMPT, FMAXPT, FMINPT, SSUMPT, SMAXPT, SMINPT, RFSUMPT, RFMAXPT, RFMINPT, RSSUMPT, RSMAXPT, RSMINPT);
	return atoi(res.c_str());
}

string eval(ExpressionMgr * &tree, Environment &env, string FPT, string SPT, string FSUMPT, string FMAXPT, string FMINPT, string SSUMPT, string SMAXPT, string SMINPT, string RFSUMPT, string RFMAXPT, string RFMINPT, string RSSUMPT, string RSMAXPT, string RSMINPT)
{
	vector<string> childs;
	string proc = tree->Children[0]->Value;
	for (int i = 1; i < tree->Children.size(); i++)
	{
		string temp = "0.0";
		if (tree->Children[i]->Value == "(")
		{
			temp = eval(tree->Children[i], env, FPT, SPT, FSUMPT, FMAXPT, FMINPT, SSUMPT, SMAXPT, SMINPT, RFSUMPT, RFMAXPT, RFMINPT, RSSUMPT, RSMAXPT, RSMINPT);
		}
		if (tree->Children[i]->Value == "FPT")
		{
			temp = FPT;
		}
		if (tree->Children[i]->Value == "SPT")
		{
			temp = SPT;
		}
		if (tree->Children[i]->Value == "FSUMPT")
		{
			temp = FSUMPT;
		}
		if (tree->Children[i]->Value == "FMAXPT")
		{
			temp = FMAXPT;
		}
		if (tree->Children[i]->Value == "FMINPT")
		{
			temp = FMINPT;
		}
		if (tree->Children[i]->Value == "SSUMPT")
		{
			temp = SSUMPT;
		}
		if (tree->Children[i]->Value == "SMAXPT")
		{
			temp = SMAXPT;
		}
		if (tree->Children[i]->Value == "SMINPT")
		{
			temp = SMINPT;
		}
		if (tree->Children[i]->Value == "RFSUMPT")
		{
			temp = RFSUMPT;
		}
		if (tree->Children[i]->Value == "RFMAXPT")
		{
			temp = RFMAXPT;
		}
		if (tree->Children[i]->Value == "RFMINPT")
		{
			temp = RFMINPT;
		}
		if (tree->Children[i]->Value == "RSSUMPT")
		{
			temp = RSSUMPT;
		}
		if (tree->Children[i]->Value == "RSMAXPT")
		{
			temp = RSMAXPT;
		}
		if (tree->Children[i]->Value == "RSMINPT")
		{
			temp = RSMINPT;
		}
		childs.push_back(temp);

	}
	string a = "";
	a = env[proc].apply(childs);
	return a;
}

int Findbestpriority(ExpressionMgr * scheme, Batch formebatch, ENV env)
{
	vector<int> wcopy;
	vector<int> loadcopy;
	wcopy = w;
	loadcopy = load;
	vector<double> priority;
	for (int i = 0; i < w.size(); i++)
	{
		double SPT = pt[w[i]];
		loadcopy.push_back(w[i]);
		std::vector<int>::iterator it = wcopy.begin() + i;//delete the specific data in ith position
		wcopy.erase(it);
		double ESUMPT = 0.0;
		double EMINPT = 9999999999;
		double EMAXPT = 0;
		for (int j = 0; j < loadcopy.size(); j++)
		{
			ESUMPT += pt[loadcopy[j]];
			if (pt[loadcopy[j]] > EMAXPT)
			{
				EMAXPT = pt[loadcopy[j]];
			}
			if (pt[loadcopy[j]]  < EMINPT)
			{
				EMINPT = pt[loadcopy[j]];
			}
		}
		double RSUMPT = 0.0;
		double RMINPT = 9999999999;
		double RMAXPT = 0;
		for (int j = 0; j < wcopy.size(); j++)
		{
			RSUMPT += pt[wcopy[j]];
			if (pt[wcopy[j]] > RMAXPT)
			{
				RMAXPT = pt[wcopy[j]];
			}
			if (pt[wcopy[j]]  < RMINPT)
			{
				RMINPT = pt[wcopy[j]];
			}
		}

		char FPT_S[20];
		char SPT_S[20];
		char FSUMPT_S[20];
		char FMAXPT_S[20];
		char FMINPT_S[20];
		char SSUMPT_S[20];
		char SMAXPT_S[20];
		char SMINPT_S[20];
		char RFSUMPT_S[20];
		char RFMAXPT_S[20];
		char RFMINPT_S[20];
		char RSSUMPT_S[20];
		char RSMAXPT_S[20];
		char RSMINPT_S[20];


		_itoa(FPT,FPT_S,10);
		_itoa(SPT, SPT_S, 10);
		_itoa(FSUMPT, FSUMPT_S, 10);
		_itoa(FMAXPT, FMAXPT_S, 10);
		_itoa(FMINPT, FMINPT_S, 10);
		_itoa(SSUMPT, SSUMPT_S, 10);
		_itoa(SMAXPT, SMAXPT_S, 10);
		_itoa(SMINPT, SMINPT_S, 10);
		_itoa(RFSUMPT, RFSUMPT_S, 10);
		_itoa(RFMAXPT, RFMAXPT_S, 10);
		_itoa(RFMINPT, RFMINPT_S, 10);
		_itoa(RSSUMPT, RSSUMPT_S, 10);
		_itoa(RSMAXPT, RSMAXPT_S, 10);
		_itoa(RSMINPT, RSMINPT_S, 10);
		

		priority.push_back(Interpreter(scheme, FPT_S, SPT_S, FSUMPT_S, FMAXPT_S, FMINPT_S, SSUMPT_S, SMAXPT_S, SMINPT_S, RFSUMPT_S, RFMAXPT_S, RFMINPT_S, RSSUMPT_S, RSMAXPT_S, RSMINPT_S));

		loadcopy.pop_back();
		std::vector<int>::iterator its = wcopy.begin() + i;//insert the specific data in ith position
		wcopy.insert(its, w[i]);
	}
	std::vector<double>::iterator smallest = std::min_element(std::begin(priority), std::end(priority));//search the index of smallest data using STL
	int index = std::distance(std::begin(priority), smallest);
	int jobindex = w[index];
	std::vector<int>::iterator it = w.begin() + index;//delete the specific data in ith position
	w.erase(it);
	return jobindex;
}

void DeleteTree(ExpressionMgr * &tree)
{
	if (tree == NULL)
		return;
	for (int i = 0; i < tree->Children.size(); i++)
	{
		DeleteTree(tree->Children[i]);
	}
	delete tree;
	tree = NULL;
	return;
}

int Buffer::getSPTnum(Buffer& Current, ENV& env)
{
	vector<int> p2;
	for (int i = 0; i < Current.jobid.size(); i++)
	{
		p2.push_back(env.p2[Current.jobid[i]]);
	}
	return Current.jobid[std::distance(std::begin(p2), std::min_element(std::begin(p2), std::end(p2)))];
}