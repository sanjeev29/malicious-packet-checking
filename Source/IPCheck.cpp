#include <iostream>
#include <string>
#include <bitset>

#include "sha256.h"
#include "RBFGen.h"

using namespace std;

bool check_ips(RBF &curr_RBF, string IP) {
    for(int i=0; i<9; i++) {
        int H_i = curr_RBF.H_i(to_string(i), IP);
        int H = curr_RBF.H(to_string(H_i));

        // cout <<"H: "<< H <<endl;
        // cout<<"H_i: " <<H_i<<endl;
        // cout<<"val: "<<curr_RBF.RBFGen[0][H_i]<<endl;
        if(H == 0 && curr_RBF.RBFGen[0][H_i] != 1) {
            return false;
        } else if (H == 1 && curr_RBF.RBFGen[0][H_i] != 0) {
            return false;
        } else {
            continue;
        }
    }

    return true;
}

