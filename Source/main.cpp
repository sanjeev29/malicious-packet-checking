#include <iostream>
#include <string>
#include <bitset>
#include <stdbool.h>

#include "sha256.h"
 
using std::string;
using std::cout;
using std::cin;
using std::endl;
using std:: stoi;

const bool DEBUG = true; // Used for debugging

class RBF {
    public:
        int m;
        int RBF_arr[];
        
        // Constructor
        RBF(int user_input) {
            m = user_input;
            // Initialze RBF to all zeros
            for(int i = 0; i < m; i++) {
                RBF_arr[i] = 0;
            }            
        }

    /**
     * h1 is one of 8 hash functions that will produce the indexes for the RBF.
     * @param key 1 through 8
     * @param input "Indeed, the “input” should be a column number in string format" (on assignment description)
     * @param m needed for modulus calculation; function needs to be refactored I think
     * @return index
    */
    // In theory, this would work. In reality, I have not tested it much yet
    int h1(string key, string input) {
        int index = -1; // If it returns -1, we know the hash failed.

        // Perform key||input, || indicates concatentation
        string input_cat = key + input;

        // Begin SHA256(key||ip) (mod m)
        string sha_no_trunc = sha256(input_cat);
        
        // Truncate
        string trunc_output_str = sha_no_trunc.substr(sha_no_trunc.length() - 5);
        int trunc_output_int = stoi(trunc_output_str, 0, 16);

        // Modulus
        index = trunc_output_int % m;

        if(DEBUG) {
            cout << "size: " << sha_no_trunc.length() << endl;
            cout << "sha256('"<< input << "'):" << sha_no_trunc << endl;
            cout << "trunc_val: " << trunc_output_str.length() << " " << trunc_output_str << endl;
            cout << "int: " << trunc_output_int << endl;
            cout << "index: " << index << endl;
        }

        return index;   
    }
};

/**
 * Proj1.pdf, item c description
 * 1. The program uses m as the RBF length. Get m as input
 * 
 * 2. The program first initializes the RBF using RBF[H(j)][j]=0 and RBF[1-H(j)][j]=1 for j=0,..., m-1
 *    a. Initialization for data structure needed; could be implemented as an array.
 *    b. H(j) is performed using the hash() function; must return index j
 * 
 * 3. The program generates the corresponding RBF that embedded 10,000 malicious IP addresses.
 * 
 * 4. The program stores the generated RBF into one text file output.txt in the same path with the RBFGen.cpp (or RBFGen.c) file.
 *    a. Output.txt should contains a row of 0 and 1; Only the first row of RBF is required to be stored
*/
int main(int argc, char *argv[])
{
    // Code now requires 2 inputs
    if(argc < 3) {
        cout << "USAGE: <RBFGen> <m> <output_file_name>" << endl;
        return -1;
    }
    
    string m = argv[1];
    int RBF_init_val;

    // Catch if user entered invalid input
    try {
        RBF_init_val = stoi(m);
    } 
    catch(...) {
        cout << "ERROR: Cannot convert " << m << " to an integer" << endl;
        return -1;
    }
        
    RBF RBFGen(RBF_init_val);

    return 0;
}