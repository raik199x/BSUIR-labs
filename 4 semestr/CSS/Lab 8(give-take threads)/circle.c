#include "circle.h"
#include <malloc.h>
#include "../../../MyLibs/C_Lib/functions.h"

void AppendCircle(struct Circle **to_append){
    struct Circle *SetRound;

    SetRound = (struct Circle*)malloc(sizeof(struct Circle));
    SetRound->added = 0;
    SetRound->deleted = 0;
    SetRound->message[0] = '\0';
    SetRound->checksum = 0;

    if((*to_append) == NULL){
        (*to_append) = SetRound;
        (*to_append)->left = SetRound;
        (*to_append)->right = SetRound;
    } else if ((*to_append)->right == (*to_append)){
        SetRound->left = (*to_append);
        SetRound->right = (*to_append);
        (*to_append)->right = SetRound;
        (*to_append)->left = SetRound;
    } else{
        SetRound->right = (*to_append);
        SetRound->left = (*to_append)->left;

        (*to_append)->left->right = SetRound;
        (*to_append)->left = SetRound;
    }
}
