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

int hash(string key, string input); // Declare function for use in main()

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
    if(argc < 3) {
        cout << "USAGE <RBFGen> <m> <output_file_name>" << endl;
        return -1;
    }
    
    string m = argv[1];
    string k = "0";

    // Hash a single value for testing
    int hash_val = hash(k, "3", m);

    // Initialize all of the RBF here


    /* IMPLEMENTED IN CODE BELOW in hash() */
    // string input = "grape!";
    // string output1 = sha256(input);

    // // The output of SHA256 hash is 256-bit, i.e. 64 characters
    // // Using the least significant 20-bit from the SHA256 hash output
    // string truncated_output =  output1.substr(output1.length() - 5);

    // // Convert 20-bit binary string to integer
    // int truncated_output_int = stoi(truncated_output, 0, 16);

    // if(DEBUG) {
    //     cout << "size: " << output1.length() << endl;
    //     cout << "sha256('"<< input << "'):" << output1 << endl;
    //     cout << "trunc_val: " << truncated_output.length() << " " << truncated_output << endl;
    //     cout << "int: " << truncated_output_int << endl;
    // }
    
    return 0;
}

/**
 * hash will produce the indexes for the RBF.
 * @param key 1 through 8
 * @param input "Indeed, the “input” should be a column number in string format"
 * @param m needed for modulus calculation; function needs to be refactored I think
 * @return index
*/
// In theory, this would work. In reality, I have not tested it much yet
int hash(string key, string input, string m) {
    int index = -1; // If it returns -1, we know the hash failed.

    // Perform key||input, || indicates concatentation
    string input_cat = key + input;

    // Begin SHA256(key||ip) (mod m)
    string sha_no_trunc = sha256(input_cat);
    
    // Truncate
    string trunc_output_str = sha_no_trunc.substr(sha_no_trunc.length() - 5);
    int trunc_output_int = stoi(trunc_output_str, 0, 16);

    // Modulus
    index = trunc_output_int % stoi(m);

    if(DEBUG) {
        cout << "size: " << sha_no_trunc.length() << endl;
        cout << "sha256('"<< input << "'):" << sha_no_trunc << endl;
        cout << "trunc_val: " << trunc_output_str.length() << " " << trunc_output_str << endl;
        cout << "int: " << trunc_output_int << endl;
        cout << "index: " << index << endl;
    }

    return index;   
}