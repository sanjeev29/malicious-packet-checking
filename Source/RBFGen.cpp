#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>

#include "RBFGen.h"
#include "sha256.h"

using namespace std;

RBF::RBF(int user_input) {
	m = user_input;

	// Resize the 2D vector
	RBFGen.resize(RBF_MAX_ROWS, vector<int>(m));
	
	// Initialize RBF
	for(int j = 0; j < m; j++) {
		// Calculate chosen row index using H(j)
		int chosen = H(to_string(j));
		
		/**
		 * CHOSEN CELLS ARE ZERO
		 * CASE 1: H(j)=0
		 *      RBF[0][j]=0
		 *      RBF[1-0][j]=1
		 * CASE 2: H(j)=1
		 *      RBF[0][j]=1
		 *      RBF[1][j]=0
		*/

		RBFGen[chosen][j] = 0;
		RBFGen[1 - chosen][j] = 1;
	}
}

int RBF::H(string input) {
	int val = -1;

	// Perform key||input, || indicates concatentation
	string input_cat = "0" + input;

	// Begin SHA256(key||ip) (mod m)
	string sha256Output = sha256(input_cat);
	
	// Consider least significant 20-bit
	int K = 20;

	string lsbHexStr = sha256Output.substr(sha256Output.length() - K / 4);
	int lsbInt = stoi(lsbHexStr, 0, 16);

	// Modulus
	val = lsbInt % 2;

	return val;
}

int RBF::H_i(string key, string input) {
	int index = -1; // If it returns -1, we know the hash failed.

	// Perform key||input, || indicates concatentation
	string input_cat = key + input;

	// Begin SHA256(key||ip) (mod m)
	string sha256Output = sha256(input_cat);
	
	// Consider least significant 20-bit
	int K = 20;

	string lsbHexStr = sha256Output.substr(sha256Output.length() - K / 4);
	int lsbInt = stoi(lsbHexStr, 0, 16);

	// Modulus
	index = lsbInt % m;

	return index;
}

void RBF::Insert(string IP, int i) {
	// Calculate h_i
    int h_i = H_i(to_string(i), IP);

	// Calculate H(h_i)
	int h = H(to_string(h_i));

	if (RBFGen[RBF_MAX_ROWS - 1][h_i] == 0) {
        RBFGen[h][h_i] = 1;
        RBFGen[1 - h][h_i] = 0;

        // Increment entry to 1 at the given column to
        // indicate an IP is inserted
        RBFGen[RBF_MAX_ROWS - 1][h_i] = 1;
    }
}