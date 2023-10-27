#ifndef RBFGen_H
#define RBFGen_H

#include <vector>

using namespace std;

const int RBF_MAX_ROWS = 3;

class RBF {
	public:
		int m;

		// Initialize a 2D vector
		vector<vector<int > > RBFGen;

		RBF(int m);

		int H(string input);

		int H_i(string key, string input);

		void Insert(string IP, int i);
};

#endif