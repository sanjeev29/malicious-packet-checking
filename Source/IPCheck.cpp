#include <iostream>
#include <string>
#include <bitset>
#include <stdio.h>

#include "sha256.h"
#include "RBFGen.h"
 
using std::string;
using std::cout;
using std::endl;
using std:: stoi;

bool check_ips(RBF &curr_RBF, string IP) {
    for (int i = 0; i < 9; i++) {
        int h_i = curr_RBF.H_i(to_string(i), IP);
        int H = curr_RBF.H(to_string(h_i));

        if(H == 0 && curr_RBF.RBFGen[0][h_i] != 1) {
            return false;
        } else if (H == 1 && curr_RBF.RBFGen[0][h_i] == 1) {
            return false;
        }
    }

    return true;
}