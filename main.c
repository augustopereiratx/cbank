#include <stdio.h>
#include "func.h"
#include "vars.h"
#include <string.h>

int main()
{
    init();
    int c;
    char str[1024];
    int numclients = 0;
    struct client clients[MAXCLIENTS];
    loaddata(clients,&numclients,sizeof(clients),sizeof(numclients));
    printf("\n%d\n",numclients);
    printf("Bem vindo(a) ao QuemPoupaTem!\n\n");
    while (1)
    {
        printf("Selecione:\n1. Novo cliente\n2. Apaga cliente\n3. Listar clientes\n4. Débito\n5. Depósito\n6. Extrato\n7. Transferência Entre Contas\n0. Sair");
        while (1)
        {
            c = intinput("\n-> ");
            if (c >= 0 && c <= 7)
            {
                break;
            }
            printf("\nSelecione uma opção válida.");
        }
        switch (c)
        {
        case 1:
            newclient(str, &clients[numclients]);
            numclients++;
            break;
        case 2:
            deleteclient(str,numclients,clients);
            numclients--;
            break;
        case 3:
            listclients(numclients, clients);
            break;
        case 4:
            debito(str,clients,numclients);
            break;
        case 5:
            deposito(str,clients,numclients);
            break;
        case 6:
            showextr(str,clients,numclients);
            break;
        case 7:
            printf("Transferido\n");
            break;
        case 0:
            printf("Até Logo!\n");
            savedata(clients,&numclients,sizeof(clients),sizeof(numclients));
            break;
        default:
            break;
        }
        if (c == 0)
        {
            break;
        }
    }
}
