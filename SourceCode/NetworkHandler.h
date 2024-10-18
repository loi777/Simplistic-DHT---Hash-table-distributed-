// Luiz Fernando Giongo dos Santos
// GRR20203965

typedef struct {
    int* fingerT;               // Finger table that all nodes use
    int fingerT_size;           // how many elements are in the fingertable
    int maxValue;               // The max value node in the network

    Node_s** nodes;             // list of references to all nodes
    int nodes_qt;               // how many nodes there are in the network
} CTNode_s;



//--------------------------------------------------------



// necessary functions for the central node to work
void net_initiate();


// A Proxy function for inserting a node into the network
// returns 1 if sucess
// returns 0 if there was an error
int net_insert(int ID);

// A Proxy function for removing a node from the network
// returns 1 if success
// returns 0 if there was an error
int net_remove(int ID);