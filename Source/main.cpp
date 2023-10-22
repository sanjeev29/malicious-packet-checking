#include <iostream>
#include <string>
#include <bitset>

#include "sha256.h"
 
using std::string;
using std::cout;
using std::endl;
using std:: stoi;
 
int main(int argc, char *argv[])
{
    string input = "grape!";
    string output1 = sha256(input);

    // The output of SHA256 hash is 256-bit, i.e. 64 characters
    // Using the least significant 20-bit from the SHA256 hash output
    string truncated_output =  output1.substr(output1.length() - 5);

    // Convert 20-bit binary string to integer
    int truncated_output_int = stoi(truncated_output, 0, 16);

    cout << "size: " << output1.length() << endl;
    cout << "trunc: " << truncated_output.length() << " " << truncated_output << endl;
    cout << "int: " << truncated_output_int << endl;
 
    cout << "sha256('"<< input << "'):" << output1 << endl;

    return 0;
}