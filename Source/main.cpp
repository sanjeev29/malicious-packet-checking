#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <map>
#include <stdbool.h>

#include "sha256.h"
 
using std::string;
using std::cout;
using std::fstream;
using std::cin;
using std::endl;
using std::stoi;
using std::map;

const bool DEBUG = true; // Used for debugging

class RBF {
    public:
        int m;
        std::vector<int> RBFGen;
        map<string, int> bad_IP_Map;

        
        // Constructor
        RBF(int user_input) {
            m = user_input;
            // Resize vector
            RBFGen.resize(m);
            
            // Initialize RBF
            for(int j = 0; j < m; j++) {
                // Calculate chosen index using H(j)
                int chosen;
                chosen = H(std::to_string(j));

                // RBF[H(j)]
                RBFGen.at(chosen) = 0;

                // RBF[1 - H(j)], check bounds to make sure it does not throw an error
                if(chosen > 0)
                    RBFGen.at(1 - chosen) = 1;
            }

            
            // Print array after insertion
            if(DEBUG) {
                for(int i = 0; i < RBFGen.capacity() - 1; i++) {
                    cout << RBFGen.at(i);
                }
                cout<<endl;
            }
            

            // USED FOR DEBUGGING -- print all IP addresses to IPGen.txt
            fstream ip_debug;
            ip_debug.open("IPGen.txt", std::ios::out | std::ofstream::trunc); // Open new file or wipe existing file to overwrite
            
            
            // Variables needed for generating "random" IP's
            string ip = "192.168.";
            int w, x, y, z = 0;         // Used for randomly generating each of the spots  
            string full_ip = "";        // Store 192.168.w.xyz in here
            bool inserted = false;
            bool temp = false;

            // Embed 10,000 IP Addresses in the RBF
            for(int i = 0; i < 10000; i++) {
                // Use a HashMap property that all keys must be unique to add a new "random" IP that was not already added
                while(!inserted) {
                    // Randomize the entry
                    w = rand() % 10;
                    x = rand() % 10;
                    y = rand() % 10;
                    z = rand() % 10;

                    // Concatenate the next couple places
                    full_ip = ip  + std::to_string(w) + "." + std::to_string(x) + std::to_string(y) + std::to_string(z);

                    // Try to insert into HashMap
                    // If true, it has successfully generated a new "random" IP to be inserted into the RBF
                    if((temp = bad_IP_Map.insert({full_ip, i}).second)) {
                        // INSERT IP INTO HASH MAP HERE
                
                        ip_debug << full_ip << endl;    // DEBUG: write to IPGen.txt
                        inserted = temp;                // Break loop
                    }
                    full_ip.clear();
                }

                // Reset
                inserted = false;
                temp = false;
                full_ip.clear();           
            }
            ip_debug.close();           
        }
        

        /**
         * H(.) to determine chosen; specific implementation is sha256("0"||input) mod 2.
         * @param index
         * @return Success: 1 or 0, Fail: -1
        */
        int H(string input) {
            int val = -1;

            // Perform key||input, || indicates concatentation
            string input_cat = "key" + input;

            // Begin SHA256(key||ip) (mod m)
            string sha_no_trunc = sha256(input_cat);
            
            // Truncate
            string trunc_output_str = sha_no_trunc.substr(sha_no_trunc.length() - 5);
            int trunc_output_int = stoi(trunc_output_str, 0, 16);

            // Modulus
            val = trunc_output_int % 2;

            return val;
        }

        /**
         * Keyed hash function. In theory, it should work. In reality, it has not been tested heavily.
         * @param key 1 through 8
         * @param input "Indeed, the “input” should be a column number in string format" (on assignment description)
         * @param m needed for modulus calculation; function needs to be refactored I think
         * @return index
        */
        int h_key(string key, string input) {
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
        if(filename.find(".txt") == std::string::npos)
            throw(filename);
    } catch (string bad_filename) {
        cout << "ERROR: Must provide valid output file name" << endl;
        return -1;
    }

    // Create RBF
    RBF RBFGen(RBF_init_val);

    // Write RBF out outfile in Results folder
    fstream output;
    output.open("Results/"+ filename+".txt", std::ios::out | std::ios::trunc);
    output << "Eventually we will put our RBF here :)\n";
    output.close();

    return 0;
}