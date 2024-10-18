// Luiz Fernando Giongo dos Santos
// GRR20203965

#define INPUT_I_NODE    'E'       // E:Insert a node
#define INPUT_R_NODE    'S'       // S:Remove a node
//--
#define INPUT_I_VAL     'I'       // I:insert a value
#define INPUT_L_VAL     'L'       // L:sarch for a value



typedef struct {
    // contains the necessary information to perform an input
    int time;               // UNUSED

    int val;                // VALUE INFORMATION
    int node;               // NODE INFORMATION

    char input_action;      // TYPE OF ACTION THAT NEEDS TO BE PERFORMED

} __input;



//--------------------------------------------------------



// read from stdio the "T L N X" operation, saving into the global variable 'NextInput'
// returns 1 if there was an operation
// returns 0 if there is no more operations to be done
int get_next_input();


//// ==== DEBUGGers


// For DEBUGG
// prints the information collected from input
void print_input();

// For DEBUGG
// function to print our current finger table
void print_ftable();

// For DEBUGG
// function to print nodes in the network and its informations
void print_network();

// For DEBUGG
// prints any keys found in the network
void print_all_keys();


//// ==== Output


// prints to stdout the path to a certain element in the network
// return 1 for erro
int io_lookup_element(int time, int nodeID, int element);