// Luiz Fernando Giongo dos Santos
// GRR20203965

#include "../mydht.h"



//---------------------------

/*
    This file is responsable for creating,
    and managing our network
*/

//---------------------------



// Our central node is a global variable.
CTNode_s CT;



//---------------------------



// necessary functions for the central node to work
void net_initiate() {
    CT.fingerT = malloc(sizeof(int));   // initially start the finger table necessary for 1 element
    CT.fingerT_size = 0;                // initially a non-generated Ftable has no indices

    CT.nodes = malloc(sizeof(Node_s*));  // initially start the node list for 1 empty slot
    CT.nodes_qt = 0;                    // initially there are no nodes
    CT.maxValue = 0;                    // since there's no node, max value is 0
}



//---------------------------



// For internal use ONLY
// realloc has the bad habit of corrupting memory
// properly realocs memory of network without corrupting
// returns 1 for error
int net_realoc(int newSize, int prevSize) {
    Node_s** newNodeList = malloc(newSize * (sizeof(Node_s*)));
    if (newNodeList == NULL) {
        fprintf(stderr, "ERRO, ao alocar nova network\n");
        return 1;
    }

    //--

    // transfer nodes
    for (int i = 0; i < prevSize; i++) {
        if (i >= newSize) {
            continue;
        }

        newNodeList[i] = CT.nodes[i];
    }

    //--

    free(CT.nodes);
    CT.nodes = newNodeList;
    CT.nodes_qt = newSize;
    return 0;
}



//---------------------------//---------------------------//---------------------------



// For internal use ONLY
// a collection of necessary steps when adding a node to network
// 1. increase the length of the network
// 2. reposition the other nodes in the networks
// 3. add the node in the correct position
// 4. update central node maxvalue
// 5. update fingertable
// 6. update hash tables
// return 0 for erro
int add_processor(Node_s* node, int posi) {
    //// ==== Increase the length of the network

    if (net_realoc((CT.nodes_qt + 1), CT.nodes_qt)) {
        fprintf(stderr, "ERRO ao adicionar nodo\n");
        return 0;
    }

    //// ==== Reposition the other nodes

    for (int i = CT.nodes_qt-1; i > posi; i--) {
        CT.nodes[i] = CT.nodes[i-1];
    }

    //// ==== Add the new node

    CT.nodes[posi] = node;

    //// ==== Update CT maxValue

    if (node->nodeID > CT.maxValue) {
        CT.maxValue = node->nodeID;
    }

    //// ==== Update fingertable

    sh_fingerT_gen();

    //// ==== Update hash tables

    int nextN = nextNode(posi, CT.nodes_qt);
    int previousN = previousNode(posi, CT.nodes_qt);

    if (CT.nodes_qt == 1) {
        // this is the first node, bootstrap
    } else {
        // there is at least one other node in the networks
        // get keys from {previousID - ID} from the next Hash Table

        transfer_keys(CT.nodes[nextN]->hashT, node->hashT, CT.nodes[previousN]->nodeID, node->nodeID);
    }

    //// ==== Return sucessful

    return 1;
}


// For internal use ONLY
// a collection of necessary steps when removing a node from network
// 1. update central node maxvalue
// 2. update fingertable
// 3. update hash tables
// 4. reposition the other nodes
// 5. decrease the length of the network
// 6. delete node memory
// return 0 for erro
int remove_processor(int posi, Node_s* node) {
    int nextN = nextNode(posi, CT.nodes_qt);
    int previousN = previousNode(posi, CT.nodes_qt);

    //// ==== Update CT maxValue

    if (node->nodeID == CT.maxValue) {
        // this node is the last index and highest value
        if (CT.nodes_qt > 1) {
            // as long as theres another node, get its value
            CT.maxValue = CT.nodes[previousN]->nodeID;
        } else {
            // when this is the last node aswell
            CT.maxValue = 0;
        }
    }

    //// ==== Update fingertable

    sh_fingerT_gen();

    //// ==== Update hash tables

    if (CT.nodes_qt == 1) {
        // this is the last node, Hash table will be lost
    } else {
        // there is at least one other node in the networks
        // send keys from {previousID - ID} to the next Hash Table

        transfer_keys(node->hashT, CT.nodes[nextN]->hashT, CT.nodes[previousN]->nodeID, node->nodeID);
    }

    //// ==== Reposition the other nodes

    for (int i = posi; i < CT.nodes_qt-1; i++) {
        CT.nodes[i] = CT.nodes[i+1];
    }

    //// ==== Decrease the length of the network

    if (net_realoc((CT.nodes_qt - 1), CT.nodes_qt)) {
        fprintf(stderr, "ERRO CATASTROFICO ao remover nodo\n");
        return 0;
    }

    //// ==== Clean nodes memory alocation

    nd_destroy(node);
    
    //// ==== Return sucessful

    return 1;
}



//---------------------------



// A Proxy function for inserting a node into the network
// returns 1 if sucess
// returns 0 if there was an error
int net_insert(int ID) {
    //// ==== Create node

    Node_s* nodeI = nd_create(ID);

    //// ==== Find position to insert

    int posi = CT.nodes_qt; // default is at the end of the list

    for (int i = 0; i < CT.nodes_qt; i++) {
        if (ID == CT.nodes[i]->nodeID) {
            // this node already exists
            fprintf(stderr, "ERRO, nodo inserido ja existe ignorando");
            return 0;
        }

        if (ID < CT.nodes[i]->nodeID) {
            // the new node is smaller so it should be put here
            posi = i;
            break;
        }
    }

    //// ==== Add the node to the network

    add_processor(nodeI, posi);

    //// ==== Update system with new node

    return 1;
}


// A Proxy function for removing a node from the network
// returns 1 if success
// returns 0 if there was an error
int net_remove(int ID) {
    //// ==== Find position of node
    int posi = -1;

    for (int i = 0; i < CT.nodes_qt; i++) {
        if (CT.nodes[i]->nodeID == ID) {
            posi = i;
        }
    }

    // Node not found
    if (posi == -1) {
        fprintf(stderr, "ERRO, Nodo para deletar nao encontrado\n");
        return 0;
    }

    //// ==== Remove the node from the network

    remove_processor(posi, CT.nodes[posi]);     // fully remove the node from the network

    //// ==== return sucessful

    return 1;
}