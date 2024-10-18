// Luiz Fernando Giongo dos Santos
// GRR20203965

// For internal use
// returns the next node of POSI
#define nextNode(POSI, QTD) (((POSI+1) >= QTD) ? (0) : (POSI+1))
// For internal use
// returns the previous node of POSI
#define previousNode(POSI, QTD) (((POSI-1) < 0) ? (QTD-1) : (POSI-1))



//--------------------------------------------------------



// according to the current network, generate the finger table
// saves in global variable 'CT'
void sh_fingerT_gen();

// returns the next value to search according to the finger table
// return 0 if the value is lower than the current node, forcing a full circle
int sh_next_fingerT(int val, int nodeID);

// returns the highest closest number to nodeID
// ex. nodes={2,8,12,23} val=14 // return 23
Node_s* sh_aproximate_node(int nodeID);