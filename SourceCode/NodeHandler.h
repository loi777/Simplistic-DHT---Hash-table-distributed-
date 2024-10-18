// Luiz Fernando Giongo dos Santos
// GRR20203965

#define MAX_NODE_INT    __INT32_MAX__           // max node value our network allows
#define MIN_NODE_INT    (-2147483648)             // min node value our network allows



typedef struct {
    HT_s* hashT;                        // Hash table contained in this node

    int nodeID;                         // This node's ID
} Node_s;



//--------------------------------------------------------



// create and allocate memory for a new node
Node_s* nd_create(int ID);

// destroy and free the memory of a node
Node_s* nd_destroy(Node_s* node);


// return the id of the node in the CT array
// return -1 for not found
int nd_get_index(Node_s* node);

// return 1 if 'val' is in 'node' range
// return 0 for not
int nd_get_range(int val, Node_s* node);


// Recursive function that finds correct node via finger table
// and inserts 'val' key to it
// return 1 for error
int nd_insert_key(int val, int nodeID);