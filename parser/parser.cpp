// serfreeman1337 // 24.05.20 //

#include "parser.h"

#include <algorithm> // find, distance
#include <math.h>

using namespace std;

const fns_t Parser::fns[] = {&sqrt, &cbrt, &log, &sin, &cos, &tan}; // C++ functions
const string Parser::fns_name[] = {"sqrt", "cbrt", "ln", "sin", "cos", "tan"}; // functions names
#define fns_size sizeof(fns)/sizeof(fns_t)

//
// Shunting Yard Algorithm
//
Parser::Parser(string str) {
	while (!qu.empty()) qu.pop(); // clear queue
	while (!st.empty()) st.pop(); // clear stack

	unsigned short pos = 0; // current position in expression
	short input_pos = -1; // number or function start position in expression

	for (char const& c : str) {
		
		switch(c) {
			case ')': // handle parentheses
				if (input_pos != -1) {  // process numbers/functions
					process_input(str.substr(input_pos, pos - input_pos));
					input_pos = -1;
				}

				while (!st.empty()) {
					sym_t d = st.top();
					st.pop();

					if (d.type == '(') {
						// check for function, pop it from stack and push it to queue
						if (!st.empty() && (d = st.top()).type == 'f') {
							st.pop();
							qu.push(d);
						}

						break;
					}
					
					qu.push(d);
				}
			break;
			// push operators to stack
			case '(':
			case '^':
			case '*':
			case '/':
			case '+':
			case '-':
				if (input_pos != -1) {  // process numbers/functions
					process_input(str.substr(input_pos, pos - input_pos));
					input_pos = -1;
				}

				while (!st.empty()) {
					sym_t d = st.top();

					// maintain operators order
					if (!is_higher_operator(d.type, c))
						break;

					st.pop();
					qu.push(d);
				}

				st.push({.type = c, .value = 0});
			break;
			default:
				if (input_pos == -1) input_pos = pos; // mark numbers/functions position for processing
			break;
		}

		pos ++;
	}

	if (input_pos != -1) {  // process numbers/functions
		process_input(str.substr(input_pos, pos - input_pos));
		input_pos = -1;
	}

	// form our queue
	while (!st.empty()) {
		sym_t d = st.top();
		st.pop();
		qu.push(d);
	}
}

float Parser::evaluate(float x) {
	queue<sym_t> eq = qu;

	// cout << "queue contains: ";
	while (!eq.empty()) {
		sym_t d = eq.front();
		eq.pop();

		// if (d.type == '0') {
		// 	cout << ' ' << d.value;
		// } else {
		// 	cout << ' ' << d.type;
		// }

		sym_t op1, op2;
		
		switch (d.type) {
			case '0':
				st.push(d);
			break;
			case 'x':
				d = {.type = '0', .value = x};
				st.push(d);
			break;
			case 'f': // process functions
				// cout << d.value;

				if (!st.size()) {
					throw "function without operand";
					break;
				}

				op1 = st.top(); st.pop();
				d.value = fns[(int)d.value](op1.value);

				d.type = '0';
				st.push(d);
			break;
			default:
				if (!st.size()) {
					throw "operator without operand";
					break;
				}
				op2 = st.top(); st.pop();

				if (st.size()) { // binary operation
					op1 = st.top(); st.pop();

					switch(d.type) {
						case '+': 
							// cout << "\n!" << op1.value << "+" << op2.value << "!";
							d.value = op1.value + op2.value;
						break;
						case '-': 
							// cout << "\n!" << op1.value << "-" << op2.value << "!";
							d.value = op1.value - op2.value;
						break;
						case '*': d.value = op1.value * op2.value;
							// cout << "\n!" << op1.value << "*" << op2.value << "!";
						break;
						case '/': d.value = op1.value / op2.value;
						break;
						case '^':
							// cout << "\n!" << op1.value << "^" << op2.value << "!"; 
							d.value = pow(op1.value, op2.value);
						break;
					}
				} else { // unary operation
					switch(d.type) {
						case '-':
							d.value = -op2.value;
						break;
						case '+':
							d.value = op2.value;
						break;
						default:
							throw "unary operator wihout operand";
						break;
					}
				}

				d.type = '0';
				// cout << "pushin to que " << d.value << "\n";
				st.push(d);
			break;
		}
	}

	// kimi no kako mo kimi no asu mo

	if (!st.size()) throw "invalid expression";
	
	// karekore naku natta 

	float val = st.top().value;
	st.pop();

	return val;
}

void Parser::process_input(string s) {
	sym_t d;

	// find valid functions and push them to stack
	if ((d.value = distance(fns_name, find(fns_name, fns_name+fns_size, s))) != fns_size) {
		d.type = 'f';
		st.push(d);

		return;
	}
	
	if (s == "x") { // variable
		d = {.type = 'x', .value = 0};
	} else { // number
		d.type = '0';

		try {
			d.value = stof(s);
		} catch(...) {
			throw "expression failed at " + s;
		}
	}

	// push numbers to queue
	qu.push(d);
}

bool Parser::is_higher_operator(char operator1, char operator2) {
	if (wmap.count(operator1) == 0 || wmap.count(operator2) == 0) return false;

	return wmap[operator1] > wmap[operator2];
}