// Descriptografar senhas (não sei como fazer bem)
extern const char key[];

struct client
{
    int accounttype;
    float money;
    char name[512];
    char cpf[512];
    char passwd[512];
};
struct extrato
{
    char cpf[512];
    char details[100][512];
};

