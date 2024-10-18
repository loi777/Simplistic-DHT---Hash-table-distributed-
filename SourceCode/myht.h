// Luiz Fernando Giongo dos Santos
// GRR20203965

#define SIZE_M 33           // Size of each hash table
#define SIZE_T1 SIZE_M
#define SIZE_T2 SIZE_M

#define REMOVED_INDEX -1    // Important to note, this breaks if the input has negative numbers
#define EMPTY_INDEX 0       // Important to note, this breaks if the input has empty numbers



typedef struct {
    int T1[SIZE_T1];               // Hash table 1
    int T2[SIZE_T2];               // Hash table 2
} HT_s;



//--------------------------------------------------------



// create and alloc memory for a hash table
HT_s* create_hashTable();

// desroy and free memory of a hash table
HT_s* destroy_hashTable(HT_s* HT);

// searches for an individual value saved in any of T1 or T2.
// return it's index, be it for T1 or T2
// returns negative if not found
int hash_search(int val, HT_s* HashT);

// Inserts a value in T1,
// if a collision happens, moves the old value to T2.
// returns 1 for no collision, and 2 for collision.
int hash_insert(int val, HT_s* HashT);

// transfer keys in the range determined by {begin-end]
// this range is open in the begin and closed in the end
// from HashOrig to HashDst
int transfer_keys(HT_s* HashOrig, HT_s* HashDst, int rangeBegin, int rangeEnd);