// Luiz Fernando Giongo dos Santos
// GRR20203965

#include "../mydht.h"



//---------------------------

/*
    This file is responsable for all functions involving
    finding a node, such as Finger table management.
*/

//---------------------------



// Our central node is a global variable.
extern CTNode_s CT;



//---------------------------



// according to the current network, generate the finger table
// saves in global variable 'CT'
void sh_fingerT_gen() {
    // get max binary needed to represent all nodes
    int tmp_val = (CT.maxValue << 1) - 1;
    int max_bin = 0;

    while(tmp_val > 1) {
        tmp_val = tmp_val >> 1;
        max_bin ++;
    }

    //--

    // memory alloc the new size
    CT.fingerT_size = max_bin;
    CT.fingerT = realloc(CT.fingerT, (sizeof(int) * CT.fingerT_size));

    //--

    // fill the new finger table
    for (int i = 0; i < CT.fingerT_size; i++) {
        CT.fingerT[i] = (1 << i);
    }
}



//---------------------------



// returns the next nodeID to search according to the finger table
// return 0 if the value is lower than the current node, forcing a full circle
int sh_next_fingerT(int val, int nodeID) {

    for (int i = 0; i < CT.fingerT_size-1; i++) {
        // Find the FT right before val exceeds it
        if (val < (CT.fingerT[i+1] + nodeID)) {
            return (CT.fingerT[i] + nodeID);
        }
    }

    //--

    fprintf(stderr, "DEBUGG, max fingertable reached %d\n", (nodeID + CT.fingerT[CT.fingerT_size-1]));
    // will return the end of the fingertable
    return (nodeID + CT.fingerT[CT.fingerT_size-1]);
}



//---------------------------



// returns the node ceil to nodeID
// ex. nodes={2,8,12,23} nodeID=14 // return 23
Node_s* sh_aproximate_node(int nodeID) {

    // search all nodes
    for (int i = 0; i < CT.nodes_qt; i++) {
        if (CT.nodes[i]->nodeID >= nodeID) {
            return CT.nodes[i];
        }
    }

    //--

    // if nodeID is above all, then return the first node
    return CT.nodes[0];
}
