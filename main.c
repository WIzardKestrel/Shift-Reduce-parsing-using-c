#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int state = 0;
char go_to[3][12][3] = { // 3 columns, 12 rows, and 3 characters max
        {"1","1","\0","\0","8","\0","\0","\0","\0","\0","\0","\0"},
        {"2","\0","\0","\0","2","\0","9","\0","\0","\0","\0","\0"},
        {"3","\0","\0","\0","3","\0","3","10","\0","\0","\0","\0"},
};
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
char rules[6][10] = {{"E > E+T"},
                     {"E > T"},
                     {"T > T*F"},
                     {"T > F"},
                     {"F > (E)"},
                     {"F > id"}};

char column[7][3] = {"id", "+", "*", "(", ")", "$"};

#define null NULL
typedef struct stk{
    char tok;//char tok[4]; // token
    int state;
    struct stk * next;
}stack;

stack * create(stack *head, char sym){
    head = (stack *) malloc(sizeof(struct stk));
    //strcpy(head -> tok, sym);
    head -> tok = sym;
    head -> state = state;
    head -> next = null;
    return head;

}

stack * push(stack * head, char sym){
    if(head == null){
        head = create(head, sym);
        return head;
    }
    else{
        stack * new_element = (stack*)malloc(sizeof(struct stk));
        new_element -> next = head;
        //strcpy(new_element -> tok, sym);
        new_element -> tok = sym;
        new_element -> state = state;
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
//void disp(stack * head){
//    while(head != null){
//        //printf("%s\n", head -> tok);
//        head = head -> next;
//    }
//}

void print_stack(stack * head){
    printf("0");
    while(head != null){
        printf("%c%d", head -> tok, head -> state);
        head = head -> next;
    }
    printf("\n");
}

stack * reduce(stack * head){
    //printf("reduction rule: %s\n", rules[state - 1]);
   // printf("%c becomes %c\n", head -> tok, rules[state - 1][0]);

    pop(&head);
    int goto_col;
    if(head != null)
        goto_col = head -> state;
    else
        goto_col = 0;

    char reduced_char = rules[state - 1][0];

    if(reduced_char == 'E')
        state = atoi(go_to[0][goto_col]);
    if(reduced_char == 'T')
       state = atoi(go_to[1][goto_col]);
    if(reduced_char == 'F')
       state = atoi(go_to[2][goto_col]);
    head = push(head, reduced_char);
    printf("goto(%d, %c)\n", goto_col, reduced_char);
    printf("stack: ");
    print_stack(head);

    return head;

}

int main(){
    printf("rules:\n");
    for (int i = 0; i < 6; ++i) {
        printf("%s\n", rules[i]);
    }
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
//        if(!strcmp(action, "\0")) {
//            setbuf(stdout, 0);
//            k++;
//            continue;
//            printf("null\n");
//        }
        printf("input: %c\n", str[k]);
        if(action[0] == 's'){// if the action is 'shift', push to the stack
            if(action[2] == '\0'){
                state = action[1] - '0';
            }else{
                state = 11; // in case s11 is encountered
            }
            start = push(start, str[k]);
            printf("shifted to stack\n");
            k++;
        }else{  // else if the action is 'reduce', reduce the first (top) token in the stack

            state = action[1] - '0';
            start = reduce(start);
            printf("reduced expression\n");
        }
        printf("action: %s\n", action);

        printf("\n");
    }
    return 0;
}
