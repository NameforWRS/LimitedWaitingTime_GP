//Basic procedures:
//Each procedure receives as input a vector of strings and returns a string

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>

using namespace std;

#include "Procedures.h"

//Print one or more variables; e.g. (print aa), (print aa bb cc)
string print(vector<string>&vv) {
    for (int i = 0; i < vv.size(); i++) {
        cout << vv[i] << " ";
    }
    cout << endl;
    return ("");
}

//Add one or more variables; e.g. (+ aa), (+ aa bb cc)
string add(vector<string>&vv) {
    if (vv.size() == 0)return "Wrong number of arguments for procedure +";
	
	double sum = atoi(vv[0].c_str());
	for (size_t i = 1; i < vv.size(); i++)sum += atoi(vv[i].c_str());
	char SUMT[100];
	_itoa(sum, SUMT, 10);
	return SUMT;
}

//Diff, e.g.: (- aa bb)
string diff(vector<string>&vv) {
    if (vv.size() == 0)return "Wrong number of arguments for procedure -";
	
	double sum = atoi(vv[0].c_str());
	for (size_t i = 1; i < vv.size(); i++)sum -= atoi(vv[i].c_str());
	char SUMT[100];
	_itoa(sum, SUMT, 10);
	return SUMT;
}

//Division, e.g. (/ aa bb cc ...)
string divv(vector<string>&vv) {
    if (vv.size() == 0)return "Wrong number of arguments for procedure /";
	
	double sum = atoi(vv[0].c_str());
	for (size_t i = 1; i < vv.size(); i++)sum /= atoi(vv[i].c_str());
	char SUMT[100];
	_itoa(sum, SUMT, 10);
	return SUMT;
}

//Product, e.g. (* aa bb cc ...)
string prod(vector<string>&vv) {
    if (vv.size() == 0)return "Wrong number of arguments for procedure *";
	
	double sum = atoi(vv[0].c_str());
	for (size_t i = 1; i < vv.size(); i++)sum *= atoi(vv[i].c_str());
	char SUMT[100];
	_itoa(sum, SUMT, 10);
	return SUMT;
}

//Quit the interpreter, e.q. (quit) or (exit)
string scheme_quit(vector<string>&vv) {
    exit(1);
    return "ok";
}

//Logical operators: <, <=, >, >=, =, !=

string less_than(vector<string>&vv) {
    if (vv.size() == 1)return "#t";
    for (size_t i = 0; i < vv.size() - 1; i++) {
        if (!(atoi(vv[i].c_str()) < atoi(vv[i + 1].c_str()))) return "#f";
    }
    return "#t";
}

string less_or_equal(vector<string>&vv) {
    if (vv.size() == 1)return "#t";
    for (size_t i = 0; i < vv.size() - 1; i++)if (!(atoi(vv[i].c_str()) <= atoi(vv[i + 1].c_str()))) return "#f";
    return "#t";
}

string great_than(vector<string>&vv) {
    if (vv.size() == 1)return "#t";
    for (size_t i = 0; i < vv.size() - 1; i++)if (!(atoi(vv[i].c_str()) > atoi(vv[i + 1].c_str()))) return "#f";
    return "#t";
}

string great_or_equal(vector<string>&vv) {
    if (vv.size() == 1)return "#t";
    for (size_t i = 0; i < vv.size() - 1; i++)if (!(atoi(vv[i].c_str()) >= atoi(vv[i + 1].c_str()))) return "#f";
    return "#t";
}

string func_max(vector<string>&vv) {
    if (vv.size() == 1)return "Wrong number of arguments for procedure max";
    for (size_t i = 0; i < vv.size() - 1; i++)
    {
    	if (!(atoi(vv[i].c_str()) >= atoi(vv[i + 1].c_str())))
    		return vv[i];
    	else
    		return vv[i + 1];
    }
}

string equall(vector<string>&vv) {
    if (vv.size() == 1)return "#t";
    for (size_t i = 0; i < vv.size() - 1; i++)if (!(atoi(vv[i].c_str()) == atoi(vv[i + 1].c_str()))) return "#f";
    return "#t";
}

string not_equal(vector<string>&vv) {
    if (vv.size() == 1)return "#t";
    for (size_t i = 0; i < vv.size() - 1; i++)if (!(atoi(vv[i].c_str()) != atoi(vv[i + 1].c_str()))) return "#f";
    return "#t";
}

string func_if(vector<string> &vv)
{
	if(vv.size() == 0) return"Wrong number of arguments for procedure if";
	double x1 = atoi(vv[0].c_str());
	if(x1 > 0) return vv[1];
	else return vv[2];
}

string func_iflez(vector<string> &vv)
{
	if(vv.size() == 0) return"Wrong number of arguments for procedure if";
	double x1 = atoi(vv[0].c_str());
	if(x1 <= 0) return vv[1];
	else return vv[2];
}
