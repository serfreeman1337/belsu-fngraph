// serfreeman1337 // 24.05.20 //

#include <iostream>
#include "render.h"

using namespace std;

int main() {
	string s;
	
	cout << "You can specify list of functions delimited with comma\n";
	cout << "Use ctrl+c to exit\n";
	while (true) {
		cout << "Functions to graph: ";
		getline(cin, s);
		
		init_render(s);
	}
	
	
	
	return 0;
}