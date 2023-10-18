#define MAXCLIENTS 10

// Descriptografar senhas (não sei como fazer bem)
extern const char key[];

struct extrato
{
    int detailcount;
    char details[100][64];
};

struct client
{
    int accounttype;
    float money;
    char name[128];
    char cpf[64];
    char passwd[128];
    struct extrato extr;
};