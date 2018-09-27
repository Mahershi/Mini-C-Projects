//First Pass Assembler C Program
//Builds the Symbol Table, also Uses Forward Reference For EQU statements.
//Functionality for DS and DC symbols pending.
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<ctype.h>

int LC = -1, unresolvedCount = 0, temp;
char unresolve[10][10];

struct symtab{
    char label[10];
    int address;
    int id;
    struct symtab *next;
} *head = NULL;

struct littab{
    char literal[10];
    int address;
    int id;
    struct littab *next;
} *litHead = NULL;

int handleEQU(char[], char[]);
void handleLabel(char[]);
void handleLiteral(char[]);
int resolveFor(char[]);
void resolveUnresolved();
int toint(char[]);
void displayLabels();
void displayLiterals();
int power(int, int);


int main()
{
    FILE *p;
    char source[15], dest[10];
    char buffer[30], ch;
    char label[10], mnemonic[10], op1[10], op2[10];
    int state, temp;
    int shouldWeIncrementLC = 0;

    printf("\nSource File Name: ");
    scanf("%s", source);
    p = fopen(source, "r");
    if(p==NULL){
        printf("\nSource File Openning Error");
    }
    else{
        while((ch = fgetc(p))!=EOF){
            shouldWeIncrementLC = 1;
            //---Read Source File Line and separate them into 4 components---------
            fscanf(p, "%[^\n]", buffer);
            sscanf(buffer, "%s %s %s %s", label, mnemonic, op1, op2);
            if(op1[strlen(op1)-1] == ',')
                op1[strlen(op1)-1] = '\0';
            temp = LC;
            if(!strcmp("EQU", mnemonic))
                    LC = -1;
            printf("%10d %10s %10s %10s %10s\n", LC, label, mnemonic, op1, op2);
            LC = temp;
            //--------------------------------------------------------------------

            if(strcmp("END", mnemonic)){
                if(strcmp("*", label) && strcmp("EQU", mnemonic)){
                        shouldWeIncrementLC = 0;
                        handleLabel(label);
                }
                if(!strcmp("START", mnemonic) || !strcmp("ORIGIN", mnemonic)){
                        shouldWeIncrementLC = 0;
                        LC = toint(op1);
                }
                if(!strcmp("EQU", mnemonic)){
                        shouldWeIncrementLC = 0;
                        state = handleEQU(label, op1);
                        if(!state){
                                strcpy(unresolve[unresolvedCount], op1);
                                unresolvedCount++;
                        }
                }
                if(!strncmp(op2, "\"=", 2)){
                        shouldWeIncrementLC = 0;
                        handleLiteral(op2);
                }
                if(shouldWeIncrementLC)
                    LC++;
            }
        }
        fclose(p);
    }
    if(unresolvedCount){
        resolveUnresolved();
    }
    displayLabels();
    displayLiterals();
    getch();
    return 0;
}

void handleLabel(char label[]){
    int id = 1;

    struct symtab *newSym = malloc(sizeof(struct symtab));
    struct symtab *temp;
    strcpy(newSym->label, label);
    newSym->address = LC;
    newSym->next = NULL;

    if(head==NULL){
        newSym->id = 1;
        head = newSym;
    }
    else{
        temp = head;
        while(temp->next!=NULL){
            temp = temp->next;
            id++;
        }
        id++;
        newSym->id = id;
        temp->next = newSym;
    }
    LC++;
}

int handleEQU(char label[], char symbol[]){
    struct symtab *current, *temp, *newSym = malloc(sizeof(struct symtab));
    int found = 0;
    newSym->address = -1;
    strcpy(newSym->label, label);
    newSym->id = 1;
    newSym->next = NULL;

    if(head==NULL)
        head = newSym;

    else{
        current = head;
        while(current!=NULL){
            if(!strcmp(current->label, symbol)){
                found = 1;
                newSym->address = current->address;
            }
            newSym->id++;
            if(current->next!=NULL)
                current = current->next;
            else
                break;
        }
        current->next = newSym;
    }

    return found;
}

void handleLiteral(char literal[]){
    int id = 1;

    struct littab *newLit = malloc(sizeof(struct littab));
    struct littab *temp;
    strcpy(newLit->literal, literal);
    newLit->address = LC;
    newLit->next = NULL;

    if(litHead==NULL){
        newLit->id = 1;
        litHead = newLit;
    }
    else{
        temp = litHead;
        while(temp->next!=NULL){
            temp = temp->next;
            id++;
        }
        id++;
        newLit->id = id;
        temp->next = newLit;
    }
    LC++;
}

void resolveUnresolved(){
    struct symtab * current = head;
    temp = unresolvedCount;
    while(current!=NULL){
        if(current->address==-1){
            current->address = resolveFor(current->label);
            unresolvedCount--;
        }
        if(!unresolvedCount)
            break;
        if(current->next!=NULL)
           current = current->next;
        else
            break;
    }
}

int resolveFor(char label[]){
    struct symtab *current = head;
    while(current!=NULL){
        if(!strcmp(current->label, unresolve[temp-unresolvedCount])){
            return current->address;
        }
        if(current->next!=NULL)
            current = current->next;
        else
            break;
    }
    return -1;
}


//--------------------------------------------------
int toint(char str[])
{
    int len = strlen(str);
    int i, num = 0;
    int a;

    for (i = 0; i < len; i++)
        num = num + ((str[len - (i + 1)] - '0') * power(10, i));
    return num;
}

void displayLabels(){
    struct symtab *current = head;

    printf("\n\nLabels:");
    if(head==NULL)
        printf("\nNo Labels");
    else{
        printf("\n%20s%20s\n", "Name", "Address");
        while(current!=NULL){
            printf("\n%20s%20d", current->label, current->address);
            if(current->next!=NULL)
                current = current->next;
            else
                break;
        }
    }
}

void displayLiterals(){
    struct littab *current = litHead;

    printf("\n\nLiterals:");
    if(litHead==NULL)
        printf("\nNo Literals");
    else{
        printf("\n%20s%20s\n", "Name", "Address");
        while(current!=NULL){
            printf("\n%20s%20d", current->literal, current->address);
            if(current->next!=NULL)
                current = current->next;
            else
                break;
        }
    }
}

int power(int a, int b){
    int i, result = 1;
    for(i=0; i<b; i++)
        result *= a;
    return result;
}
