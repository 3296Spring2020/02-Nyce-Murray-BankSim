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
    int fromAccount = param->fromAccount;
    int maxAmount = param->maxAmount;
    free(param);
    
    for(int i = 0; i < 10000; ++i) {
        int toAccount = (int)(rand() % b->numAccounts);
        int amount = (int)(rand() % maxAmount);
        Bank_transfer(b, fromAccount, toAccount, amount);
    }

    //TASK 5
    Bank_test(b);

    printf("Account[%d] is done.\n", fromAccount);
}
