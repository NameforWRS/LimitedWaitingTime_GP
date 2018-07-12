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

struct CmpByPriority {
	bool operator()(const Batch k1, const Batch k2) const
	{
		return k1.Priority > k2.Priority;
	}
};

struct CmpByPjtj {
	bool operator()(const Batch k1, const Batch k2) const
	{
		return k1.sumofbatchsec > k2.sumofbatchsec;
	}
};

double GBLWT_GP(int n, vector<int> p1, vector<int> p2, vector<int> s1, vector<int> r, int W, int B, 
	ExpressionMgr * scheme_batchformation, ExpressionMgr * scheme_batchsequence, ExpressionMgr * scheme_jobsequence)
{
	ENV env;
	env.B = B;
	env.n = n;
	env.p1 = p1;
	env.p2 = p2;
	env.s1 = s1;
	env.W = W;
	env.r = r;

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
		while (CurrentBatch.capacity+1<=env.B) 
		{
			int best_ind_job = Findbestpriority_batchformation(scheme_batchformation, CurrentBatch,unsheduledjobs, env);//发现最佳的组批job
			if (best_ind_job == env.n + 1)
			{
				break;
			}
			else
			{
				CurrentBatch.jobid.push_back(best_ind_job);//将满足的job加入新批中
				CurrentBatch.sumofsecprocess += p2[best_ind_job];//SSUMPTB
				CurrentBatch.maxsecprocess = std::max(p2[best_ind_job], CurrentBatch.maxsecprocess);//SMAXPTB
				CurrentBatch.SMINPTB = std::min(p2[best_ind_job], CurrentBatch.SMINPTB);//SMINPTB
				CurrentBatch.timeofbatchprocess = std::max(p1[best_ind_job], CurrentBatch.timeofbatchprocess);//PTB
				CurrentBatch.FSUMPTB += p1[best_ind_job];//FSUMPTB
				CurrentBatch.FMINPTB = std::min(p1[best_ind_job], CurrentBatch.FMINPTB);//FMINPTB
				CurrentBatch.sumofbatchsec = CurrentBatch.sumofsecprocess + CurrentBatch.timeofbatchprocess;
				CurrentBatch.capacity++;
			}	
		}

		char PTB_S[20];
		char FSUMPTB_S[20];
		char SSUMPTB_S[20];
		char FMINPTB_S[20];
		char SMINPTB_S[20];
		char SMAXPTB_S[20];

		_itoa(CurrentBatch.timeofbatchprocess, PTB_S, 10);
		_itoa(CurrentBatch.FSUMPTB, FSUMPTB_S, 10);
		_itoa(CurrentBatch.sumofsecprocess, SSUMPTB_S, 10);
		_itoa(CurrentBatch.FMINPTB, FMINPTB_S, 10);
		_itoa(CurrentBatch.SMINPTB, SMINPTB_S, 10);
		_itoa(CurrentBatch.maxsecprocess, SMAXPTB_S, 10);

		CurrentBatch.Priority = Interpreter_batchsequence(scheme_batchsequence, PTB_S, FSUMPTB_S, SSUMPTB_S, FMINPTB_S, SMINPTB_S, SMAXPTB_S);
		FormedBatch.push_back(CurrentBatch);//所有组好的批全部进入FormedBatch中
	}

	/*////////////////////////////////////////////////
	Step 2 Sequence Batches Formed
	*////////////////////////////////////////////////

	//Step2.1 Caculate the release time of batch
	//Step2.2 Sequence the release time

	vector<int> ReleaseTime_T;//计算每批job的释放时间集合，为得到每批释放时间提供支持
	multimap<Batch, int, CmpByPriority> Sequence_Batch1;//用multimap构建自动对releasetime进行排序的sequence
	//multimap<Batch, int, CmpByPj> Sequence_Batch1;//用multimap构建自动对releasetime进行排序的sequence
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
				int goin = buffer1.getSPTnum(buffer1, env,scheme_jobsequence);
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

double Interpreter_batchformation(ExpressionMgr * scheme, string RT,string FPT, string SPT, string FSUMPT, string FMAXPT, string FMINPT,
	string SSUMPT, string SMAXPT, string SMINPT, string RFSUMPT, string RFMAXPT, string RFMINPT, string RSSUMPT, string RSMAXPT, string RSMINPT)
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
	res = eval_batchformation(scheme->Children[0], env, RT,FPT, SPT, FSUMPT, FMAXPT, FMINPT, SSUMPT, SMAXPT, SMINPT, RFSUMPT, RFMAXPT, RFMINPT, RSSUMPT, RSMAXPT, RSMINPT);
	return atoi(res.c_str());
}

double Interpreter_batchsequence(ExpressionMgr * scheme, string PTB, string FSUMPTB, string SSUMPTB, string FMINPTB, string SMINPTB, string SMAXPTB)
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
	res = eval_batchsequence(scheme->Children[0], env, PTB, FSUMPTB, SSUMPTB, FMINPTB, SMINPTB, SMAXPTB);
	return atoi(res.c_str());
}

double Interpreter_jobsequence(ExpressionMgr * scheme, string RT,string PTS,string RSUMPTS, string RMINPTS, string RMAXPTS)
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
	res = eval_jobsequence(scheme->Children[0], env, RT,PTS, RSUMPTS, RMINPTS, RMAXPTS);
	return atoi(res.c_str());
}

string eval_batchformation(ExpressionMgr * &tree, Environment env, string RT,string FPT, string SPT, string FSUMPT, string FMAXPT, string FMINPT,
	string SSUMPT, string SMAXPT, string SMINPT, string RFSUMPT, string RFMAXPT, string RFMINPT, string RSSUMPT, string RSMAXPT, string RSMINPT)
{
	vector<string> childs;
	string proc = tree->Children[0]->Value;
	for (int i = 1; i < tree->Children.size(); i++)
	{
		string temp = "0.0";
		if (tree->Children[i]->Value == "(")
		{
			temp = eval_batchformation(tree->Children[i], env, RT,FPT, SPT, FSUMPT, FMAXPT, FMINPT, SSUMPT, SMAXPT, SMINPT, RFSUMPT, RFMAXPT, RFMINPT, RSSUMPT, RSMAXPT, RSMINPT);
		}
		if (tree->Children[i]->Value == "RT")
		{
			temp = RT;
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

string eval_batchsequence(ExpressionMgr * &tree, Environment env, string PTB, string FSUMPTB, string SSUMPTB, string FMINPTB, string SMINPTB, 
	string SMAXPTB)
{
	vector<string> childs;
	string proc = tree->Children[0]->Value;
	for (int i = 1; i < tree->Children.size(); i++)
	{
		string temp = "0.0";
		if (tree->Children[i]->Value == "(")
		{
			temp = eval_batchsequence(tree->Children[i], env, PTB, FSUMPTB,  SSUMPTB,  FMINPTB,  SMINPTB, SMAXPTB);
		}
		if (tree->Children[i]->Value == "PTB")
		{
			temp = PTB;
		}
		if (tree->Children[i]->Value == "FSUMPTB")
		{
			temp = FSUMPTB;
		}
		if (tree->Children[i]->Value == "SSUMPTB")
		{
			temp = SSUMPTB;
		}
		if (tree->Children[i]->Value == "FMINPTB")
		{
			temp = FMINPTB;
		}
		if (tree->Children[i]->Value == "SMINPTB")
		{
			temp = SMINPTB;
		}
		if (tree->Children[i]->Value == "SMAXPTB")
		{
			temp = SMAXPTB;
		}
		childs.push_back(temp);

	}
	string a = "";
	a = env[proc].apply(childs);
	return a;
}

string eval_jobsequence(ExpressionMgr * &tree, Environment env, string RT,string PTS,
	string RSUMPTS, string RMINPTS, string RMAXPTS)
{
	vector<string> childs;
	string proc = tree->Children[0]->Value;
	for (int i = 1; i < tree->Children.size(); i++)
	{
		string temp = "0.0";
		if (tree->Children[i]->Value == "(")
		{
			temp = eval_jobsequence(tree->Children[i], env,RT, PTS, RSUMPTS,  RMINPTS,  RMAXPTS);
		}
		if (tree->Children[i]->Value == "RT")
		{
			temp = RT;
		}
		if (tree->Children[i]->Value == "PTS")
		{
			temp = PTS;
		}
		if (tree->Children[i]->Value == "RSUMPTS")
		{
			temp = RSUMPTS;
		}
		if (tree->Children[i]->Value == "RMINPTS")
		{
			temp = RMINPTS;
		}
		if (tree->Children[i]->Value == "RMAXPTS")
		{
			temp = RMAXPTS;
		}
		childs.push_back(temp);

	}
	string a = "";
	a = env[proc].apply(childs);
	return a;
}


int Findbestpriority_batchformation(ExpressionMgr * scheme, Batch formedbatch, vector<int>& unshceduledjobs, ENV env)
{
	vector<int> unshcedluedjobscopy;
	Batch formedbatchcopy;
	unshcedluedjobscopy = unshceduledjobs;
	formedbatchcopy = formedbatch;
	vector<double> priority;
	for (int i = 0; i < unshceduledjobs.size(); i++)
	{
		if (formedbatchcopy.sumofsecprocess + env.p2[unshceduledjobs[i]] - std::max(env.p2[unshceduledjobs[i]], formedbatchcopy.maxsecprocess) <= env.W)
		{
			double RT = env.r[unshceduledjobs[i]];
			double FPT = env.p1[unshceduledjobs[i]];//the processing time of First Stage of Job i
			double SPT = env.p2[unshceduledjobs[i]];//the processing time of second stage of job i
			formedbatchcopy.jobid.push_back(unshceduledjobs[i]);

			std::vector<int>::iterator it = unshcedluedjobscopy.begin() + i;//delete the specific data in ith position
			unshcedluedjobscopy.erase(it);
			double FSUMPT = 0.0;
			double FMINPT = 9999999999;
			double FMAXPT = 0;
			double SSUMPT = 0.0;
			double SMINPT = 9999999999;
			double SMAXPT = 0;
			for (int j = 0; j < formedbatchcopy.jobid.size(); j++)
			{
				FSUMPT += env.p1[formedbatchcopy.jobid[j]];
				if (env.p1[formedbatchcopy.jobid[j]] > FMAXPT)
				{
					FMAXPT = env.p1[formedbatchcopy.jobid[j]];
				}
				if (env.p1[formedbatchcopy.jobid[j]]  < FMINPT)
				{
					FMINPT = env.p1[formedbatchcopy.jobid[j]];
				}
				SSUMPT += env.p2[formedbatchcopy.jobid[j]];
				if (env.p2[formedbatchcopy.jobid[j]] > SMAXPT)
				{
					SMAXPT = env.p2[formedbatchcopy.jobid[j]];
				}
				if (env.p2[formedbatchcopy.jobid[j]]  < SMINPT)
				{
					SMINPT = env.p2[formedbatchcopy.jobid[j]];
				}
			}
			double RFSUMPT = 0.0;
			double RFMAXPT = 0;
			double RFMINPT = 9999999999;
			double RSSUMPT = 0.0;
			double RSMAXPT = 0;
			double RSMINPT = 9999999999;
			for (int j = 0; j < unshceduledjobs.size(); j++)
			{
				RFSUMPT += env.p1[unshceduledjobs[j]];
				if (env.p1[unshceduledjobs[j]] > RFMAXPT)
				{
					RFMAXPT = env.p1[unshceduledjobs[j]];
				}
				if (env.p1[unshceduledjobs[j]]  < RFMINPT)
				{
					RFMINPT = env.p1[unshceduledjobs[j]];
				}
				RSSUMPT += env.p2[unshceduledjobs[j]];
				if (env.p2[unshceduledjobs[j]] > RSMAXPT)
				{
					RSMAXPT = env.p2[unshceduledjobs[j]];
				}
				if (env.p2[unshceduledjobs[j]]  < RSMINPT)
				{
					RSMINPT = env.p2[unshceduledjobs[j]];
				}
			}
			char RT_S[20];
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

			_itoa(RT, RT_S, 10);
			_itoa(FPT, FPT_S, 10);
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


			priority.push_back(Interpreter_batchformation(scheme, RT_S,FPT_S, SPT_S, FSUMPT_S, FMAXPT_S, FMINPT_S, SSUMPT_S, SMAXPT_S, SMINPT_S, RFSUMPT_S, RFMAXPT_S, RFMINPT_S, RSSUMPT_S, RSMAXPT_S, RSMINPT_S));

			formedbatchcopy.jobid.pop_back();
			std::vector<int>::iterator its = unshcedluedjobscopy.begin() + i;//insert the specific data in ith position
			unshcedluedjobscopy.insert(its, unshceduledjobs[i]);
		}		
		else
		{
			priority.push_back(DBL_MAX);
		}
	}
	if (std::accumulate(std::begin(priority), std::end(priority), 0.0)/priority.size()== DBL_MAX)
	{
		return env.n + 1;
	}
	if (priority.size() == 0)
	{
		return env.n + 1;
	}
	else
	{
		double smallest = *std::min_element(std::begin(priority), std::end(priority));
		//std::vector<double>::iterator smallest = std::min_element(std::begin(priority), std::end(priority));//search the index of smallest data using STL
		//int index = std::distance(std::begin(priority), smallest);
		int jobindex = unshceduledjobs[std::distance(std::begin(priority), find(priority.begin(), priority.end(), smallest))];
		//std::vector<int>::iterator it = ;//delete the specific data in ith position
		unshceduledjobs.erase(unshceduledjobs.begin() + std::distance(std::begin(priority), find(priority.begin(), priority.end(), smallest)));
		return jobindex;
	}	
}

int Buffer::getSPTnum(Buffer& Current, ENV& env, ExpressionMgr * scheme)
{
	Buffer Currentcopy;
	Currentcopy = Current;
	vector<double> priority;
	for (int i = 0; i < Current.jobid.size(); i++)
	{
		double RT= env.r[Current.jobid[i]];
		double PTS = env.p2[Current.jobid[i]];//PTS
		std::vector<int>::iterator it = Currentcopy.jobid.begin() + i;//delete the specific data in ith position
		Currentcopy.jobid.erase(it);
		double RSUMPTS = 0.0;
		double RMINPTS = 9999999999;
		double RMAXPTS = 0;
		for (int j = 0; j < Currentcopy.jobid.size(); j++)
		{
			RSUMPTS += env.p2[Currentcopy.jobid[j]];
			if (env.p2[Currentcopy.jobid[j]] > RMAXPTS)
			{
				RMAXPTS = env.p2[Currentcopy.jobid[j]];
			}
			if (env.p2[Currentcopy.jobid[j]]  < RMINPTS)
			{
				RMINPTS = env.p2[Currentcopy.jobid[j]];
			}
		}
		char RT_S[20];
		char PTS_S[20];
		char RSUMPTS_S[20];
		char RMAXPTS_S[20];
		char RMINPTS_S[20];
		_itoa(RT,RT_S, 10);
		_itoa(PTS, PTS_S, 10);
		_itoa(RSUMPTS, RSUMPTS_S, 10);
		_itoa(RMAXPTS, RMAXPTS_S, 10);
		_itoa(RMINPTS, RMINPTS_S, 10);

		priority.push_back(Interpreter_jobsequence(scheme,RT_S, PTS_S, RSUMPTS_S, RMAXPTS_S, RMINPTS_S));
		std::vector<int>::iterator its = Currentcopy.jobid.begin() + i;//insert the specific data in ith position
		Currentcopy.jobid.insert(its, Current.jobid[i]);
	}
	std::vector<double>::iterator smallest = std::min_element(std::begin(priority), std::end(priority));//search the index of smallest data using STL
	int index = std::distance(std::begin(priority), smallest);
	int jobindex = Current.jobid[index];
	return jobindex;
	//return Current.jobid[std::distance(std::begin(p2), std::min_element(std::begin(p2), std::end(p2)))];
}

int Buffer::getSPTnumsimple(Buffer& Current, ENV& env)
{
	vector<int> p2;
	for (int i = 0; i < Current.jobid.size(); i++)
	{
		p2.push_back(env.p2[Current.jobid[i]]);
	}
	return Current.jobid[std::distance(std::begin(p2), std::min_element(std::begin(p2), std::end(p2)))];
}