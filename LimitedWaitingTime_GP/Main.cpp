#include"IDG.h"
#include"Notation.h"
#include"GBLWT.h"

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
	string code = "";
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
	IDG_1(n, p1, p2, s1, r, W, B, seed);

	for (int i = 0; i < n.size(); i++)
	{
		for (int batch = 0; batch < B.size(); batch++)
		{
			for (int w = 0; w < W.size(); w++)
			{
				cout << GBLWT_GP(n[i], p1[i], p2[i], s1[i], r[i], W[w], B[batch]) << endl;
				cout << endl;
			}
		}
	}
	DeleteTree(scheme);
	return 0;

}