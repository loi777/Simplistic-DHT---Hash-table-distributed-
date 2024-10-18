// Luiz Fernando Giongo dos Santos
// GRR20203965

#include "../mydht.h"


//---------------------------

/*
    This file is used for input and output operations,
    such as:
        obtain input from stdin
        print output to stdout
*/

//---------------------------


__input NextInput;
// Our central node is a global variable.
extern CTNode_s CT;


//---------------------------



// For internal use ONLY
// get the next char in stdin
// return -1 if next line is found
// return -2 if EOF is found
int get_next_char(char* dst) {

    while(1) {
        if (fscanf(stdin, "%c", dst) == EOF) {
            return -2;
        }

        if (*dst == '\n') {
            return -1;
        }

        if (*dst != ' ') {
            return 1;
        }
    }
}


// read from stdio the "T L N X" operation, saving into the global variable 'NextDHTInput'
// returns 1 if there was an operation
// returns 0 if there is no more operations to be done
int io_next_input() {
    int tmp_int;
    char tmp_char;
    char tmp_string[99];

    //--
    // get the time value

    if (fscanf(stdin, "%d", &tmp_int) == EOF) {
        // end of file
        fprintf(stderr, "FIM DO ARQUIVO DE ENTRADA\n");
        return 0;
    }

    NextInput.time = tmp_int;

    //--
    // get the type of operation

    if (get_next_char(&tmp_char) < 0) {
        // end of file or Line jump early
        fprintf(stderr, "ARQUIVO DE ENTRADA COM ERRO:2\n");
        return 0;   
    }

    NextInput.input_action = tmp_char;

    //--
    // get the node value

    if (fscanf(stdin, "%d", &tmp_int) == EOF) {
        // end of file early, this is an ERROR
        fprintf(stderr, "ARQUIVO DE ENTRADA COM ERRO:3\n");
        return 0;
    }
    NextInput.node = tmp_int;

    //--
    // get the numerical value, if any

    if (fscanf(stdin, "%99s", tmp_string) == EOF) {
        // end of file early, this is an ERROR
        fprintf(stderr, "ARQUIVO DE ENTRADA COM ERRO:4\n");
        return 0;
    }

    if (tmp_string[0] == '-') {
        NextInput.val = 0;
    } else {
        NextInput.val = atoi(tmp_string);
    }

    //--

    return 1; // an operation was correctly requested
}



//---------------------------



// For DEBUGG
// prints the information collected from input
void print_input() {
    fprintf(stderr, "DEBUGG\ninput --\n      Time: %d\n      Type:%c\n      Node:%d\n      Value:%d\n\n",
            NextInput.time, NextInput.input_action, NextInput.node, NextInput.val);
}


// For DEBUGG
// function to print our current finger table
void print_ftable() {
    fprintf(stderr, "DEBUGG\nFinger Table:\n");
    for (int i = 0; i < CT.fingerT_size; i++) {
        fprintf(stderr, "[%d] - %d\n", i, CT.fingerT[i]);
    }
}


// For DEBUGG
// function to print nodes in the network and its informations
void print_network() {
    fprintf(stderr, "DEBUGG\nNetwork information:\n     Nodes: %d\n     MaxValue: %d\n      FingerTableSize: %d\n\n", 
            CT.nodes_qt, CT.maxValue, CT.fingerT_size);
    
    //--

    fprintf(stderr, "DEBUGG\nNode list:\n");
    for (int i = 0; i < CT.nodes_qt; i++) {
        fprintf(stderr, "[%d] - %d\n", i, CT.nodes[i]->nodeID);
    }
}


// For DEBUGG
// prints any keys found in the network
void print_all_keys() {

    fprintf(stderr, "DEBUGG Printing all keys in the network:\n");

    for (int n = 0; n < CT.nodes_qt; n++) {

        for (int i = 0; i < SIZE_T1; i++) {
            if (CT.nodes[n]->hashT->T1[i] > 0) {
                fprintf(stderr, "N[%d] T1[%d] == %d\n", n, i, CT.nodes[n]->hashT->T1[i]);
            }
        }

        for (int i = 0; i < SIZE_T2; i++) {
            if (CT.nodes[n]->hashT->T2[i] > 0) {
                fprintf(stderr, "N[%d] T2[%d] == %d\n", n, i, CT.nodes[n]->hashT->T1[i]);
            }
        }

    }
}



//---------------------------



// prints to stdout a Hash table
// must print lower values first,
// using table when a draw happens
// using index when a draw happens again
void hash_print(HT_s* HashT) {

    while(1) {
        int lowest = __INT_MAX__;

        //--

        // find (lowest and valid) printable value
        for (int i = 0; i < SIZE_M; i++) {
            if (HashT->T1[i] < lowest && HashT->T1[i] > 0) lowest = HashT->T1[i];
            if (HashT->T2[i] < lowest && HashT->T2[i] > 0) lowest = HashT->T2[i];
        }

        // if no remaining element was found for printing
        if (lowest == __INT_MAX__) {
            // finishes printing and returns
            return;
        }

        //--

        // print first instance of the lowest value found

        // Tries T1 first
        for (int i = 0; i < SIZE_M; i++) {
            if (HashT->T1[i] == lowest) {
                printf("%d,T1,%d\n", lowest, i);

                HashT->T1[i] = REMOVED_INDEX;  // remove so we don't print it again
            }
        }

        // Tries T2 last
        for (int i = 0; i < SIZE_M; i++) {
            if (HashT->T2[i] == lowest) {
                printf("%d,T2,%d\n", lowest, i);

                HashT->T2[i] = REMOVED_INDEX;  // remove so we don't print it again
            }
        }
    }

}


// prints to stdout the path to a certain element in the network
// return 1 for erro
int io_lookup_element(int time, int nodeID, int element) {
    Node_s* searchNode = sh_aproximate_node(nodeID);
    int searchNodeID = searchNode->nodeID;
    if (searchNode->nodeID != nodeID) {
        fprintf(stderr, "AVISO, lookup em um nodo nao existente, indo para o mais proximo = %d\n", searchNodeID);
    }

    //// ==== get the first node

    int pathSize = 0;
    int* nodePath = malloc(sizeof(int) * CT.nodes_qt);
    if (nodePath == NULL) {
        fprintf(stderr, "ERRO, ao alocar memoria para node path\n");
        return 1;
    }

    //// ==== alloc memory for the path of nodes

    int elementNotFound = 1;

    for (int i = 0; i < CT.nodes_qt; i++) {
        // saves this node in the path
        nodePath[i] = searchNodeID;
        pathSize++;

        // check if element is inside
        if (hash_search(element, searchNode->hashT) >= 0) {
            // element found
            fprintf(stderr, "DEBBUG, found %d in %d\n", element, searchNodeID);
            elementNotFound = 0;
            break;
        }

        // element not found, next node
        searchNode = sh_aproximate_node(sh_next_fingerT(element, searchNodeID));
        searchNodeID = searchNode->nodeID;
    }

    //// ==== search the network for the element

    if (elementNotFound) {
        // if the node wans't found
        fprintf(stderr, "ERRO, elemento não encontrado\n");
        return 1;
    }

    //// ==== confirm that the element was found
    
    fprintf(stdout, "%d L %d", time, element);

    fprintf(stdout, " {");

    if (pathSize > 1) {
        // at least 2 nodes in path
        fprintf(stdout, "%d", nodePath[1]);
        
        if (pathSize > 2) {
            // there are atleast 3 nodes
            for (int i = 2; i < pathSize; i++) {
                fprintf(stdout, ",%d", nodePath[i]);
            }
        }
    }

    fprintf(stdout, "}\n");

    //// ==== print the path took to find the element



    //// ==== print each node in this path

    return 0;
}