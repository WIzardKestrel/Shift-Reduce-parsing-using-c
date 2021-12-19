#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct bracket_node {
	char data;
	struct bracket_node* next;
};

void push_para(struct bracket_node** top_ref, int new_data)
{

	struct bracket_node* new_node
		= (struct bracket_node*)malloc(sizeof(struct bracket_node));

	if (new_node == NULL) {
		getchar();
		exit(0);
	}
	new_node->data = new_data;
	new_node->next = (*top_ref);
	(*top_ref) = new_node;
}

int pop_para(struct bracket_node** top_ref)
{
	char res;
	struct bracket_node* top;

	if (*top_ref == NULL) {
		//printf("Stack overflow n");
		getchar();
		exit(0);
	}
	else {
		top = *top_ref;
		res = top->data;
		*top_ref = top->next;
		free(top);
		return res;
	}
}


int isCorrect(char character1, char character2)
{
	if (character1 == '(' && character2 == ')')
		return 1;
	else if (character1 == '{' && character2 == '}')
		return 1;
	else if (character1 == '[' && character2 == ']')
		return 1;
	else
		return 0;
}

int BRACKET_balance(char exp[])
{
	int i = 0;
	struct bracket_node* stack = NULL;
	while (exp[i])
	{

		if (exp[i] == '{' || exp[i] == '(' || exp[i] == '[')
			push_para(&stack, exp[i]);

		if (exp[i] == '}' || exp[i] == ')'
			|| exp[i] == ']') {

			if (stack == NULL)
				return 0;

			else if (!isCorrect(pop_para(&stack), exp[i]))
				return 0;
		}
		i++;
	}

	if (stack == NULL)
		return 1;
	else
		return 0; 
}

int operater_balance(char exp[])
{
    int i = 0;
    while (exp[i] != '\0')
    {
        if (exp[i] == '+')
        {
            if (exp[i+1] == '*' || exp[i+1] == '+')
            {
                return 0;
            }
            
        }else if (exp[i] == '*')
        {
            if (exp[i+1] == '*' || exp[i+1] == '+')
            {
                return 0;
            }
        }
        i++;
        
    }
    return 1;
    
}

int state = 0;
int stack_num = 1;
int input_num = 1;
char go_to[3][12][3] = {  // the goto table
        {"1","1","\0","\0","8","\0","\0","\0","\0","\0","\0","\0"},
        {"2","\0","\0","\0","2","\0","9","\0","\0","\0","\0","\0"}, // 3 columns, 12 rows, and 3 characters max
        {"3","\0","\0","\0","3","\0","3","10","\0","\0","\0","\0"},
};
char parse_table[12][6][4] = { // the (main) parse table
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
char rules[6][10] = {{"E > E+T"}, // the syntax grammar
                     {"E > T"},
                     {"T > T*F"},
                     {"T > F"},
                     {"F > (E)"},
                     {"F > id"}};

char symbols[7][3] = {"id", "+", "*", "(", ")", "$"}; // the top row of the parsing table (used along with the state to get the action from the parse table)

#define null NULL // I don't pressing CapsLock
typedef struct stk{
    /// stack node, has a char attribute for storing the token and an int attribute for storing the state of each token ///
    char tok; // token
    int state;
    struct stk * next;
}stack;

stack * push(stack * head, char sym){
    /// takes a pointer to stack and a symbol character (token) and pushes the symbol to a new element on the stack
    /// uses global variable state///
    //printf("pushed %c%d to stack\n", sym, state);
    stack * new_element = (stack*)malloc(sizeof(struct stk));
    new_element -> next = head;
    new_element -> tok = sym;
    new_element -> state = state;
    head = new_element;
    return head;

}

stack * pop(stack ** head){
    /// removes the top of the stack and returns the popped value
    if(*head == null)
        return *head;
    stack * temp = *head;
    *head = (*head) -> next;
    return  temp;
}

char * get_symbol_index(char symbol){
    /// takes a symbol and returns the index of that symbol in the 'symbols' array
    //used along with the state for determining the next action
    int i = 0;
    char * fail = "\0";
    for (; i < 7; ++i)
        if (symbol == symbols[i][0])
            break;
    
    if (parse_table[state][i] != parse_table[0][1])
    {
        //printf("\n(%s)\n",parse_table[state][i]);
        return parse_table[state][i];
    }else{
        return NULL;
    }
    
}

void print_reversed(stack * head){ // recursion; don't try to understand, just trust the code
    /// prints a reversed stack without actually reversing the stack using recursion
    if(head == null)
        return;
    print_reversed(head -> next);
    printf("%c", head -> tok);
    if(head -> tok == 'i')
        printf("d");
    printf("%d", head -> state);
    stack_num++;

}
stack * reduce(stack * head){
    ///reduces an expression according to the current state
    //printf("reduction rule: %s\n", rules[state - 1]);
    //printf("reduced %c to %c\n", head -> tok, rules[state - 1][0]);
    if(state == 1){
    pop(&head);
    pop(&head);
    pop(&head);

    }else if(state == 5){
        pop(&head);
        pop(&head);
        pop(&head);
    }else if(state == 3){
        pop(&head);
        pop(&head);
        pop(&head);
    }
    else
        pop(&head);
    int goto_col;
    if(head != null)
        goto_col = head -> state;
    else
        goto_col = 0;

    char reduced_char = rules[state - 1][0];

    if(reduced_char == 'E')
        state = atoi(go_to[0][goto_col]);
    else if(reduced_char == 'T')
       state = atoi(go_to[1][goto_col]);
    else //if reduced_char == 'F'
       state = atoi(go_to[2][goto_col]);
    head = push(head, reduced_char);
    if(reduced_char == 'E' || reduced_char == 'F' || reduced_char == 'T')
	 printf("goto(%d, %c)\n", goto_col, reduced_char);
    return head;
}

int main(){
    printf("rules:\n");
    for (int i = 0; i < 6; ++i) {
        printf("%s\n", rules[i]);
    }
    printf("parsing table:\n");
    printf("\tid\t+\t*\t(\t)\t$\n");
    for (int i = 0; i < 12; ++i) {
        printf("%d\t", i);
        for (int j = 0; j < 6; ++j) {
            if(parse_table[i][j] == null)
                continue;
            printf("%s\t", parse_table[i][j]);

        }
        printf("\n");
    }
    
    printf("enter the expression you want to reduce WITHOUT SPACES:");

    char str[200];
    scanf("%s", str);
    int k = 0;
    char action[20];
    stack * start = null;
    setbuf(stdout, 0);
    int incorrect = 0;
    if(BRACKET_balance(str)){
        ;
    }else
    {
        printf("\nTHE INPUT IS INCORRECT");
        return 0;
    }
    if (operater_balance(str))
    {
        ;
    }else
    {
        printf("\nTHE INPUT IS INCORRECT");
        return 0;
    }
	for (int i = 0; i < strlen(str); i++){
		if (str[i] == '('){
			if (str[i + 1] != 'i') {
				printf("\nTHE INPUT IS INCORRECT");
				return 0;
			}
		}
		if (str[i] == ')') {
			if (str[i - 1] != 'd') {
				printf("\nTHE INPUT IS INCORRECT");
					return 0;
			}
		}
        if(str[i] != '+' && str[i] != '*' && str[i] != 'i' && str[i] != 'd' && str[i] != '(' && str[i] != ')'){
            printf("\nINPUT IS INCORRECT - illegal character\n");
            return 0;
        }
	}
    
    

    printf("stack\t\tinput\t\taction\n");
    while(str[k] != '\0'){
        if(str[k] == ' ' || str[k] == 'd') {
            k++;
            continue;
        }

        if (get_symbol_index(str[k]) == NULL)
        {
            incorrect = 1;
            break;
        }
        
        strcpy(action, get_symbol_index(str[k]));

        printf("0");
        print_reversed(start);
        //printf("           ");
        int s;
        
        if (stack_num<11)
        {
            for ( s = 11 - stack_num; s >= 0 ; s--)
            {
                printf(" ");
            }
            stack_num = 1;
        }else
        {
            for ( s = stack_num - 11; s>= 0 ; s--)
            {
                printf(" ");
                
            }
            stack_num = 1;
        }
           
        //printf("input: ");
        for (int i = k; i < strlen(str); ++i) {
            printf("%c", str[i]);
            input_num++;
        }printf("$");
        
        if (input_num<11)
        {
            for ( s = (11 - input_num ) + 5 ; s > 0 ; s--)
            {
                printf(" ");
            }
            input_num = 1;
        }else
        {
            for ( s = 4 ; s>= 0 ; s--)
            {
                printf(" ");
            }
            input_num = 1;
        }
       // printf("%-s", action);
        

        if(action[0] == 's'){// if the action is 'shift', push to the stack
            if(action[2] == '\0'){
                state = action[1] - '0';
            }else{
                state = 11; // in case s11 is encountered
            }
	    printf("Shift %d ", state);
            start = push(start, str[k]);
            k++;
            printf("\n");
        }else{  // else if the action is 'reduce', reduce the first (top) token in the stack
            state = action[1] - '0'; // state is changed to match the reduction rule
	    printf("Reduce %d ", state);
            start = reduce(start);  // expression is reduced according that rule (state)
        }
//  input: i+i*(i+i)
    printf("\n");

    }
    if (incorrect == 1)
    {
        printf("\nTHE INPUT IS INCORRECT");
    }else
    {
    
        while(1){
            int s;
            printf("0");
            print_reversed(start);
            
            printf("\t\t");
            
            stack_num = 1;
            strcpy(action, get_symbol_index('$'));
            printf("$");
            for ( s = 11; s >= 0; s--)
            {
                printf(" ");
            }
            if(action[0] != 'a')
                printf("Reduce %d ", state);
            else  if(action[0] == 'a'){
                printf("Accept\n");
                break;

            }
            state = action[1] - '0';
            start = reduce(start);
            printf("\n");
        }
    }
    return 0;
}
