#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <map>
#include <stdbool.h>

#include "sha256.h"

// Changed to this to simplify it
using namespace std;

const bool DEBUG = false; // Used for debugging
const bool DEBUG_VERBOSE = false; // Used for extensive debugging
const int RBF_ROW_COUNT = 3; // The third row tracks the number of entries for a particular column

class RBF {
    public:
        int m;

        // vector<int> RBFGen;

        // Initialize a 2D vector
        vector<vector<int> > RBFGen;

        // Constructor
        RBF(int user_input) {
            m = user_input;

            // Resize vector
            // RBFGen.resize(m);

            // Resize the 2D vector
            RBFGen.resize(RBF_ROW_COUNT, vector<int>(m));
            
            // Initialize RBF
            for(int j = 0; j < m; j++) {
                // Calculate chosen row index using H(j)
                int chosen;
                chosen = H(to_string(j));
                
                if(DEBUG_VERBOSE)
                    cout << "Chosen is " << chosen << endl;
                
                /**
                 * CHOSEN CELLS ARE ZERO
                 * CASE 1: H(j)=0
                 *      RBF[0][j]=0
                 *      RBF[1-0][j]=1
                 * CASE 2: H(j)=1
                 *      RBF[0][j]=1
                 *      RBF[1][j]=0
                */

                RBFGen.at(chosen).at(j) = 0;
                RBFGen.at(1 - chosen).at(j) = 1;

                // if(chosen == 0) {
                //     if(DEBUG_VERBOSE) {
                //         cout << "RBF[0][" << j << "] = 0"<<endl;
                //         cout << "RBF[1][" << j << "] = 1"<<endl;
                //     }
                //     // RBFGen.at(j) = 0;

                //     RBFGen.at(chosen).at(j) = 0;
                //     RBFGen.at(1 - chosen).at(j) = 1;

                // } else {
                //     if(DEBUG_VERBOSE){
                //         cout << "RBF[0][" << j << "] = 1"<<endl;
                //         cout << "RBF[1][" << j << "] = 0"<<endl;
                //     }
                //     // RBFGen.at(j) = 1;

                //     RBFGen.at(1 - chosen).at(j) = 1;
                // }

                if(DEBUG_VERBOSE)
                    cout<<endl; 
            }
            
            // Print array after insertion
            if(DEBUG) {
                cout << "RBF Init: " << endl;
                for(int i = 0; i < RBFGen.size(); i++) {
                    for (int j = 0; j < RBFGen[i].size(); j++) {
                        cout << RBFGen[i][j] << " ";
                    }
                    cout << endl;
                }
            }           
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
        int h_i(string key, string input) {
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
 * Insert a single IP
 * @param curr_RBF
 * @param curr_IP
 * @return 1 on success; 0 on failure
*/
void insert(RBF &curr_RBF, string IP, int i){
    int H, h_i;
    
    // H(h_key(i||IP)) determines chosen cell
    
    // Calculate h_i
    h_i = curr_RBF.h_i(to_string(i), IP);
    if(DEBUG)
        cout << "h_i: " << h_i << endl;
    
    // Calculate H(h_i)
    H = curr_RBF.H(to_string(h_i));
    if(DEBUG)
        cout << "H: " << H << endl;

     /**
     * CHOSEN CELLS ARE ONE
     * CASE 1: H(j)=0
     *      RBF[0][j]=1
     *      RBF[1-0][j]=0
     * CASE 2: H(j)=1
     *      RBF[0][j]=0
     *      RBF[1][j]=1
     **/

    // Insert only if the number of entries for the column is 0
    if (curr_RBF.RBFGen[RBF_ROW_COUNT - 1][h_i] == 0) {
        curr_RBF.RBFGen[H][h_i] = 1;
        curr_RBF.RBFGen[1 - H][h_i] = 0;

        // Increment entry to 1 at the given column to
        // indicate an IP is inserted
        curr_RBF.RBFGen[RBF_ROW_COUNT - 1][h_i] = 1;

    }

    // if(H == 0) {
    //     if(DEBUG_VERBOSE) {
    //         cout << "RBF[0][" << i << "] = 0"<<endl;
    //         cout << "RBF[1][" << i << "] = 1"<<endl;
    //     }
    //     cout << curr_RBF.RBFGen[H][h_i] << endl;

    //     // curr_RBF.RBFGen.at(i) = 1;
    //     curr_RBF.RBFGen[H][h_i] = 1;

    //     cout << curr_RBF.RBFGen[H][h_i] << endl;

    // } else {
    //     if(DEBUG_VERBOSE){
    //         cout << "RBF[0][" << i << "] = 1"<<endl;
    //         cout << "RBF[1][" << i << "] = 0"<<endl;
    //     }
    //     // curr_RBF.RBFGen.at(i) = 0;
    //     curr_RBF.RBFGen[1 - H][h_i] = 0;
    // }
}

/**
 * Output data to a file
*/

void output_to_file(string filePath, vector<int> data) {
    fstream output;
    output.open(filePath, fstream::out | fstream::trunc);

    // Output given data to a file
    for(int i = 0; i < data.size(); i++) {
        output << data[i];
    }

    output.close();
}

/**
 * Inserts all bad IPs
 * 
*/
void insert_bad_IPs(RBF &curr_RBF, vector<string> IPs) {
    for (int i = 0; i < IPs.size(); i++) {
        for (int k = 1; k < 9; k++) {
            insert(curr_RBF, IPs[i], k);
        }
    }

    // TODO: Testing
    output_to_file("Results/10test.txt", curr_RBF.RBFGen[0]);
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
    RBF RBFGen(RBF_init_val);

    // Generate IPs
    vector<string> IPs = generate_IPs();
    cout << "IPs size: " << IPs.size() << endl;

    // string IP_addr = IPs.at(0);
    // if(DEBUG)
    //     cout << IP_addr << endl;
    
    // Attempt inserting 1 IP address
    // insert(RBFGen, IP_addr, 0);

    // Insert 10,000 IPs to RBF
    insert_bad_IPs(RBFGen, IPs);
    cout << "RBFGen size: " << RBFGen.RBFGen.size() << endl;

    // Insert RBF to <filename>.txt
    output_to_file("Results/"+filename, RBFGen.RBFGen[0]);

    // Write RBF out outfile in Results folder
    // fstream output;
    // output.open("Results/"+ filename, fstream::out | fstream::trunc);

    // Put the RBF here
    // output << "Final output: \n";
    // for(int i = 0; i < RBF_init_val; i++) {
    //     output << RBFGen.RBFGen.at(i);
    // }

    if (DEBUG) {
        for(int i = 0; i < RBF_ROW_COUNT; i++) {
            for (int j = 0; j < RBF_init_val; j++) {
                cout << RBFGen.RBFGen[i][j] << " ";
            }
            cout << endl;
        }
    }

    // output.close();

    return 0;
}