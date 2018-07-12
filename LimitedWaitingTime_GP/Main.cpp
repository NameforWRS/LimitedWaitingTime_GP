#include"IDG.h"
#include"Notation.h"
#include"GBLWT.h"
//#include"vld.h"
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
//Control Console
int main()
{
	vector<int> n;
	vector<vector<int>> p1;
	vector<vector<int>> p2;
	vector<vector<int>> s1;
	vector<vector<int>> r;
	vector<int> W;
	vector<int> B;
	int A = 10;
	int N = 4;
	int R = 10;
	int F = 10;
	int G = 50;
	int seed = 1234567889;
	int timelimit = 10;
	
	//导入派单规则
	string code1 = "";
	string code2 = "";
	string code3 = "";
	string code_batchformation = "";
	string code_batchsequence = "";
	string code_jobsequence = "";
	code1 = "FPT";
	code2 = "RT";
	code3 = "PTS";



	ExpressionMgr mgr1;
	ExpressionMgr mgr2;
	ExpressionMgr mgr3;

	///一头一尾加上括号，方便处理
	code_batchformation = code1;
	if (code_batchformation[0] != '(') {
		if (code_batchformation.size() > 8) {
			code_batchformation = "(" + code_batchformation + ")";
		}
		else {
			code_batchformation = "(* " + code_batchformation + ")";
		}
	}
	code_batchsequence = code2;
	if (code_batchsequence[0] != '(') {
		if (code_batchsequence.size() > 4) {
			code_batchsequence = "(" + code_batchsequence + ")";
		}
		else {
			code_batchsequence = "(* " + code_batchsequence + ")";
		}
	}
	code_jobsequence = code3;
	if (code_jobsequence[0] != '(') {
		if (code_jobsequence.size() > 8) {
			code_jobsequence = "(" + code_jobsequence + ")";
		}
		else {
			code_jobsequence = "(* " + code_jobsequence + ")";
		}
	}

	///将表达式转换为树
	ExpressionMgr * scheme1 = mgr1.ParseAsIScheme(code_batchformation);
	ExpressionMgr * scheme2 = mgr2.ParseAsIScheme(code_batchsequence);
	ExpressionMgr * scheme3 = mgr3.ParseAsIScheme(code_jobsequence);

	IDG_simple(n, p1, p2, s1, r, W, B, seed);

	vector<double> Obj;

	for (int i = 0; i < n.size(); i++)
	{
		for (int batch = 0; batch < B.size(); batch++)
		{
			for (int w = 0; w < W.size(); w++)
			{
				double Obj1 = GBLWT_GP(n[i], p1[i], p2[i], s1[i], r[i], W[w], B[batch], scheme1,scheme2,scheme3);
				double obj_GDSPT = GBLWT(n[i], p1[i], p2[i], s1[i], r[i], W[w], B[batch]);
				cout << Obj1 << "	" << obj_GDSPT << endl;
				Obj.push_back(100.0 * (Obj1 - obj_GDSPT) / obj_GDSPT);
			}
		}
	}
	cout << std::accumulate(std::begin(Obj), std::end(Obj), 0.0) / Obj.size() << endl;;
	DeleteTree(scheme1);
	DeleteTree(scheme2);
	DeleteTree(scheme3);
	return 0;

}