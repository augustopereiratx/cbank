#include <stdio.h>
#include "func.h"
#include "vars.h"
#include <string.h>

int main()
{
    init();
    int c;
    char str[512];
    int numclients = 0;
    struct client clients[100];
    struct extrato extratos[100];
    while (1)
    {
        printf("Bem vindo ao QuemPoupaTem!\n\nSelecione:\n1. Novo cliente\n2. Apaga cliente\n3. Listar clientes\n4. Débito\n5. Depósito\n6. Extrato\n7. Transferência Entre Contas\n0. Sair");
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
            newclient(str, &numclients, &clients[numclients],&extratos[numclients]);
            break;
        case 2:
            deleteclient(str,numclients,clients,extratos);
            numclients--;
            break;
        case 3:
            listclients(numclients, clients);
            break;
        case 4:
            printf("Debitado\n");
            break;
        case 5:
            printf("Depositado\n");
            break;
        case 6:
            printf("Extrato\n");
            break;
        case 7:
            printf("Transferido\n");
            break;
        case 0:
            printf("Até Logo!\n");
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
