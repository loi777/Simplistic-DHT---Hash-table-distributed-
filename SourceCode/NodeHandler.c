// Luiz Fernando Giongo dos Santos
// GRR20203965

#include "../mydht.h"



//---------------------------

/*
    This file is responsable for creating,
    and managing our nodes.
*/

//---------------------------



// Our central node is a global variable.
extern CTNode_s CT;



//---------------------------



// create and allocate memory for a new node
Node_s* nd_create(int ID) {
    Node_s* node;

    //--

    node = malloc(sizeof(Node_s));
    if (node == NULL) {
        fprintf(stderr, "ERRO, ao alocar Nodo\n");
        return NULL;
    }

    //--

    node->nodeID = ID;
    node->hashT = create_hashTable();
    if (node->hashT == NULL) {
        fprintf(stderr, "ERRO, ao alocar Hash Table\n");
    }

    //--

    return node;
}


// destroy and free the memory of a node
Node_s* nd_destroy(Node_s* node) {

    if (node == NULL) {
        // this node is already destroyed, ignore
        fprintf(stderr, "Pedido para deletar nodo ja deletado\n");
        return NULL;
    }

    //--

    destroy_hashTable(node->hashT);
    free(node);

    //--

    return NULL;
}



//---------------------------



// return the id of the node in the CT array
// return -1 for not found
int nd_get_index(Node_s* node) {
    int nodeID = node->nodeID;

    //--

    for (int i = 0; i < CT.nodes_qt; i++) {
        if (CT.nodes[i]->nodeID == nodeID) {
            return i;
        }
    }

    //--

    fprintf(stderr, "ERRO, nodo procurado nao existe\n");
    return -1;
}


// return 1 if 'val' is in 'node' range
// return 0 for not
int nd_get_range(int val, Node_s* node) {
    int posi = nd_get_index(node);
    int prev = previousNode(posi, CT.nodes_qt);

    //--
    
    if (posi == 0) {
        // this is the first node
        if ((val > CT.nodes[prev]->nodeID) || (val <= node->nodeID)) {
            // val is greater than the last nodeID
            // or smaller than the first
            return 1;
        }

    } else {
        
        if ((val > CT.nodes[prev]->nodeID) && (val <= node->nodeID)) {
            // val is inbetween the previous nodeID and this nodeID
            return 1;
        }
    }

    //--

    // return negative
    return 0;
}


//---------------------------



// Recursive function
// Inserts the 'val' in the correct 'node'
// Uses Finger Table
// Return 1 for error
int nd_insert_key(int val, int nodeID) {

    fprintf(stderr, "DEBUGG: try insert = val %d | try find = nodeID %d\n", val, nodeID);

    // find the node or its closest higher
    Node_s* n = sh_aproximate_node(nodeID);

    fprintf(stderr, "DEBUGG: node found = nodeID %d\n", n->nodeID);

    //--

    if (nd_get_range(val, n)) {
        fprintf(stderr, "DEBUGG: inserted\n");
        hash_insert(val, n->hashT);
        return 1;
    } else {
        fprintf(stderr, "DEBUGG: recursive\n");
        // get next node to test in the finger Table
        int nn = sh_next_fingerT(val, n->nodeID);
        fprintf(stderr, "DEBUGG: next search = nodeID %d\n", nn);
        return nd_insert_key(val, nn);
    }
}