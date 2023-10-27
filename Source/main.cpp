#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <map>
#include <stdbool.h>

#include "sha256.h"
#include "RBFGen.h"

// Changed to this to simplify it
using namespace std;

const bool DEBUG = false; // Used for debugging

/**
 * Generate all the bad IP addresses
*/
vector<string> generate_IPs() {
    vector<string> bad_ips;

    // USED FOR DEBUGGING -- print all IP addresses to IPGen.txt
    fstream ip_debug;
    ip_debug.open("Results/IPGen.txt", fstream::out | fstream::trunc); // Open new file or wipe existing file to overwrite
    
    // Variables needed for generating "random" IP's
    string ip = "192.168.";
    int w=0, x=0, y=0, z=0;         // Used for randomly generating each of the spots  
    string full_ip = "";            // Store 192.168.w.xyz in here
    
    // Embed 10,000 IP Addresses in the RBF
    // There is probably a recursive way to do this, 
    // but I decided to go with the ugly but effective if-statement approach
    for(int i = 0; i < 10000; i++) {
        full_ip = ip  + to_string(w) + "." + to_string(x) + to_string(y) + to_string(z);
        bad_ips.emplace_back(full_ip);

        ip_debug << full_ip << endl;
        

        if(z < 9) {
            z++;
        } else {
            z = 0;
            if(y < 9) {
                y++;
            } else {
                y = 0;
                if(x < 9) {
                    x++;
                } else {
                    x = 0;
                    if(w < 9) {
                        w++;
                    } else {
                        w = 0;
                    }
                }
            }
        }
        full_ip.clear();               
    }
    ip_debug.close();

    return bad_ips;
}

/**
 * Output data to a file
*/

void output_to_file(string filePath, vector<int> data) {
    fstream output;
    fstream output2;
    
    output.open(filePath, fstream::out | fstream::trunc);
    output2.open("Result/", fstream::out | fstream::trunc);

    // Output given data to a file
    for(int i = 0; i < data.size(); i++) {
        output << data[i];
        output2 << data[i];
    }

    output.close();
    output2.close();
}

/**
 * Inserts all bad IPs
 * 
*/
void insert_bad_IPs(RBF &curr_RBF, vector<string> IPs) {
    for (int i = 0; i < IPs.size(); i++) {
        for (int k = 1; k < 9; k++) {
            curr_RBF.Insert(IPs[i], k);
        }
    }
}

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
    
    string exe = argv[0];

    if(exe.compare("./RBFGen") == 0){
        string m = argv[1];
        int RBF_init_val;

        // Validate user input for m
        try {
            RBF_init_val = stoi(m);
        } catch(...) {
            cout << "ERROR: Cannot convert " << m << " to an integer" << endl;
            return -1;
        }

        // Validate user input for output file name
        string filename = argv[2];
        try {
            if(filename.find(".txt") == string::npos)
                throw(filename);
        } catch (string bad_filename) {
            cout << "ERROR: Must provide valid output file name" << endl;
            return -1;
        }

        // Create RBF
        RBF RBF(RBF_init_val);

        // Print array after insertion
        if(DEBUG) {
            cout << "RBF Init: " << endl;
            for(int i = 0; i < RBF.RBFGen.size(); i++) {
                for (int j = 0; j < RBF.m; j++) {
                    cout << RBF.RBFGen[i][j] << " ";
                }
                cout << endl;
            }
        }

        // Generate IPs
        vector<string> IPs = generate_IPs();

        // Insert 10,000 IPs to RBF
        insert_bad_IPs(RBF, IPs);

        // Insert RBF to <filename>.txt
        output_to_file(filename, RBF.RBFGen[0]);

        if (DEBUG) {
            cout << "RBF Out: " << endl;
            for(int i = 0; i < RBF_MAX_ROWS; i++) {
                for (int j = 0; j < RBF_init_val; j++) {
                    cout << RBF.RBFGen[i][j] << " ";
                }
                cout << endl;
            }
        }
    } else {
        // Validate user input for output file name
        string filename = argv[1];
        try {
            if(filename.find(".txt") == string::npos)
                throw(filename);
        } catch (string bad_filename) {
            cout << "ERROR: Must provide valid output file name" << endl;
            return -1;
        }

        // Get the size
        int end = filename.find(".txt");
        int size = stoi(filename.substr(0, end));

        RBF RBFCheck(size);

        // Read in from the file
        fstream fin(filename, fstream::in);
        int curr;
        char c;
        int counter = 0;
        while(fin.get(c)) {    
            curr = c;
            RBFCheck.RBFGen[0][counter] = curr;
            counter++;
        }
    }

    return 0;
}