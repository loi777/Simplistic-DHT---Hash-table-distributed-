// Luiz Fernando Giongo dos Santos
// GRR20203965

#include "mydht.h"



//---------------------------



int main(int argc, char* argv[]) {

    net_initiate();     // Initiate Network

    //--

    int choice = 1;
    int node = 0;
    int element = 0;

    while(1) {
        fprintf(stderr, "Oque deseja: addNode(1), removeNode(2), addElement(3), lookup(4), exit(0)\n");
        scanf("%d", &choice);

        switch(choice) {
            case(1):
                fprintf(stderr, "Adicionar qual nodo:\n");
                scanf("%d", &node);

                net_insert(node);
            break;

            case(2):
                fprintf(stderr, "Remover qual nodo:\n");
                scanf("%d", &node);

                net_remove(node);
            break;

            case(3):
                fprintf(stderr, "adicionar qual elemento:\n");
                scanf("%d", &element);
                fprintf(stderr, "em qual nodo:\n");
                scanf("%d", &node);

                nd_insert_key(element, node);
            break;

            case(4):
                fprintf(stderr, "procurar qual elemento:\n");
                scanf("%d", &element);
                fprintf(stderr, "em qual nodo:\n");
                scanf("%d", &node);

                io_lookup_element(0, node, element);        
            break;

            default:
                fprintf(stderr, "saindo..\n");
                return 0;
            break;
        }

        //--

        print_network();

        print_ftable();

        print_all_keys();
    }

    //--

    return 0;
}