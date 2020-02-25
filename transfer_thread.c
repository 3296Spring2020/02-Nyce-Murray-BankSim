#include "Bank.h"
TransferThreadParameters *TransferThreadParameters_new(Bank *b, int fromAccount, int maxAmount) {
    TransferThreadParameters *param = (TransferThreadParameters *)malloc(sizeof(TransferThreadParameters));
    param->b = b;
    param->fromAccount = fromAccount;
    param->maxAmount = maxAmount;

    return param;
}

void *transfer_thread(void *vargp) {
    TransferThreadParameters *param = (TransferThreadParameters *)vargp;
    Bank *b = param->b;
    //int fromAccount = param->fromAccount;
    int maxAmount = param->maxAmount;
    //printf("Thread created for account %d.\n", fromAccount);
    free(param);

    for (int i = 0; i < 10000; ++i)
    {
        int fromAccount = (int)(rand() % b->numAccounts);
        int toAccount = (int)(rand() % b->numAccounts);
        int amount = (int)(rand() % maxAmount);
        Bank_transfer(b, fromAccount, toAccount, amount);
    }

    //printf("Account[%d] is done.\n", fromAccount);
}
