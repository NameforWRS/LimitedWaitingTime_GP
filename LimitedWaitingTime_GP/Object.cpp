#include "Object.h"


Object::Object()
{
}

Object::~Object()
{
}

Object::Object(string(* _p)(vector<string>&))
{
	pp = _p;
	value="";
}

string Object::get_value()
{
	return value;
}

ExpressionMgr Object::get_expression()
{
	return expression;
}


string Object::apply(vector<string>&V) 
{
	return pp(V);
}