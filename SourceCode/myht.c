// Luiz Fernando Giongo dos Santos
// GRR20203965

#include "../mydht.h"



//---------------------------

/*
    This file is responsable for creating, and managing hash tables
*/

//---------------------------



// create and alloc memory for a hash table
HT_s* create_hashTable() {
    HT_s* HT;

    //--

    HT = malloc(sizeof(HT_s));
    if (HT == NULL) {
        fprintf(stderr, "ERRO, ao alocar Hash Table\n");
        return NULL;
    }

    //--

    return HT;
}


// desroy and free memory of a hash table
HT_s* destroy_hashTable(HT_s* HT) {

    if (HT == NULL) {
        // this hash is already destroyed, ignore
        fprintf(stderr, "Pedido para deletar uma hash ja deletada\n");
        return NULL;
    }

    //--

    free(HT);

    //--

    return NULL;
}



//---------------------------



/*
    Collision will be handled by going to T2,
    and ALWAYS assumes T2 is empty.
    Ex. We first generate key at T1, having a collision,
    we do the same but for key gen T2, and save it in T2.
*/

// Gen Hash key for T1
int hash_key1(int k) {
    return (k % SIZE_M);
}


// Gen Hash key for T2
int hash_key2(int k) {
    return (int)(SIZE_M * ((k * 0.9) - (int)(k * 0.9)));
}



//---------------------------



// searches for an individual value saved in any of T1 or T2.
// return it's index, be it for T1 or T2
// returns negative if not found
int hash_search(int val, HT_s* HashT) {
    int index = hash_key1(val);
    
    //--

    if (HashT->T1[index] == EMPTY_INDEX) {
        // if this position in T1 is empty, return negative
        return -1;
    }
    
    if (HashT->T1[index] == val) {
        // if this position is the correct value, return index
        return index;
    }

    //--

    // If both failed, then we assume the value can be found in T2 and
    // return the index from T2.

    return hash_key2(val);
}



//---------------------------



// Inserts a value in T1,
// if a collision happens, moves the old value to T2.
// returns 1 for no collision, and 2 for collision.
int hash_insert(int val, HT_s* HashT) {
    int index = hash_key1(val);

    //--

    if (HashT->T1[index] == EMPTY_INDEX || HashT->T1[index] == REMOVED_INDEX) {
        HashT->T1[index] = val;
        return 1;
    }

    //--

    // verify if the value collided is the same being inserted
    if (val == HashT->T1[index]) {
        fprintf(stderr, "ERRO, inserindo chave ja inserida\n");
        return 2;
    }

    //--

    // if we get here, there was a collision
    int old_val = HashT->T1[index];

    HashT->T1[index] = val;
    HashT->T2[hash_key2(old_val)] = old_val;
    
    //--
    
    return 2;
}



//---------------------------



// tries to remove a value in T1 or T2
// returns 1 if found in T1, or 2 in T2
int hash_remove(int val, HT_s* HashT) {
    int index = hash_key2(val);

    //--

    if (HashT->T2[index] == val) {
        // val in T2, remove it
        HashT->T2[index] = REMOVED_INDEX;
        return 2;
    }

    //--

    // if we get here, we assume the val is in T1
    HashT->T1[hash_key1(val)] = REMOVED_INDEX;
    return 1;
}



//---------------------------



// transfer keys in the range determined by {begin-end]
// this range is open in the begin and closed in the end
// from HashOrig to HashDst
int transfer_keys(HT_s* HashOrig, HT_s* HashDst, int rangeBegin, int rangeEnd) {

    int tmp_val;

    //--

    // transfer all keys in T1
    for (int i = 0; i < SIZE_T1; i++) {
        // If this is a valid key, transfer
        if (HashOrig->T1[i] > rangeBegin && HashOrig->T1[i] <= rangeEnd) {
            tmp_val = HashOrig->T1[i];  // save the value
            
            hash_remove(tmp_val, HashOrig); // remove from origin
            hash_insert(tmp_val, HashDst); // add to destine
        }
    }

    // verify all keys in T2
    for (int i = 0; i < SIZE_T2; i++) {
        // If this is a valid key, transfer
        if (HashOrig->T2[i] > rangeBegin && HashOrig->T2[i] <= rangeEnd) {
            tmp_val = HashOrig->T2[i];  // save the value
            
            hash_remove(tmp_val, HashOrig); // remove from origin
            hash_insert(tmp_val, HashDst); // add to destine
        }
    }

    //--

    return 1;
}



//---------------------------