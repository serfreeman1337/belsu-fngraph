// serfreeman1337 // 24.05.20 //

#pragma once

#include <iostream>
#include <queue>
#include <stack>

using namespace std;

// symbol type for queue
struct sym_t {
	char type; // operator/number/function etc
	float value; // number or function ID 
};


	
typedef double (*fns_t)(double);
class Parser {
public:
	Parser(string str);
	float evaluate(float x);
private:
	static const fns_t fns[];
	static const string fns_name[];

	queue<sym_t> qu;
	stack<sym_t> st;

	void process_input(string s);
	bool is_higher_operator(char operator1, char operator2);
};