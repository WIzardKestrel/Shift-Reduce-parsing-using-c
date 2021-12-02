#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int state = 0;
char parse_table[12][6][4] = {
        {"s5","\0","\0","s4","\0","\0"},
        {"\0","s6","\0","\0","\0","ac"},
        {"\0","r2","s7","\0","r2","r2"},
        {"\0","r4","r4","\0","r4","r4"},
        {"s5","\0","\0","s4","\0","\0"},
        {"\0","r6", "r6","\0","r6","r6"},
        {"s5","\0","\0","s4","\0","\0"},
        {"s5","\0","\0","s4","\0","\0"},
        {"\0","s6","\0","\0","s11","\0"},
        {"\0","r1", "s7","\0","r1","r1"},
        {"\0","r3","r3","\0","r3","r3"},
        {"\0","r5","r5","\0","r5","r5"}
};
char rules[6][10] = {{"E->E+T"},
                     {"E->T"},
                     {"T->T*F"},
                     {"T->F"},
                     {"F->(E)"},
                     {"F->id"}};
char column[7][3] = {"id", "+", "*", "(", ")", "$"};

#define null NULL
typedef struct stk{
    char symbol[4];
    struct stk * next;
}stack;

stack * create(stack *head, char * sym){
    head = (stack *) malloc(sizeof(struct stk));
    strcpy(head -> symbol, sym);
    head -> next = null;
    return head;

}

stack * push(stack * head, char *sym){
    if(head == null){
        head = create(head, sym);
        return head;
    }
    else{
        stack * new_element = (stack*)malloc(sizeof(struct stk));
        new_element -> next = head;
        strcpy(new_element -> symbol, sym);
        head = new_element;
        return head;
    }
}

stack * pop(stack ** head){
    if(*head == null)
        return *head;
    stack * temp = *head;
    *head = (*head) -> next;
    return  temp;
}
int column_index(char first_char) {
    for (int i = 0; i < 7; ++i)
        if (first_char == column[i][0])
            return i;
    return -1;
}

char * check(char symbol){
    int col = column_index(symbol);
    return parse_table[state][col];
}
void disp(stack * head){
    while(head != null){
        printf("%s\n", head -> symbol);
        head = head -> next;
    }
}



int main(){
//    printf("rules:\n");
//    for (int i = 0; i < 6; ++i) {
//        printf("%s\n", rules[i]);
//    }
//    printf("parsing table:\n");
//    printf("id\t+\t*\t(\t)\t$\n");
//    for (int i = 0; i < 11; ++i) {
//        for (int j = 0; j < 7; ++j) {
//            if(parse_table[i][j] == null)
//                continue;
//            printf("%s\t", parse_table[i][j]);
//
//        }
//        printf("\n");
//    }
    printf("enter the expression you want to reduce:");

    char str[20];
    scanf("%s", str);
    int k = 0;
    char action[20];
    stack * start = null;
    while(str[k] != '\0'){
        if(str[k] == ' ' || str[k] == 'd') {
            k++;
            continue;
        }
        strcpy(action, check(str[k]));
        printf("input is: %c action: %s\n", str[k], action);
        if(action[0] == 's'){
            if(action[2] == '\0'){
                state = action[1] - '0';
            }else
                state = 11;
//            printf("%d\n", state);
        }else{
            state = action[1] - '0';
//            printf("%d\n", state);
        }
        k++;
    }
    return 0;
}