#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 100
#define MAX_NAME_LENGTH 20

typedef struct {
    char name[MAX_NAME_LENGTH];
    int value;
} Symbol;

typedef struct {
    Symbol symbols[MAX_SYMBOLS];
    int num_symbols;
} SymbolTable;

typedef struct {
    SymbolTable* tables[MAX_SYMBOLS];
    int num_tables;
} SymbolTableStack;

SymbolTable* create_symbol_table() {
    SymbolTable* table = (SymbolTable*) malloc(sizeof(SymbolTable));
    table->num_symbols = 0;
    return table;
}

void add_symbol(SymbolTable* table, char* name, int value) {
    if (table->num_symbols >= MAX_SYMBOLS) {
        printf("Error: symbol table is full\n");
        exit(1);
    }
    Symbol* symbol = &(table->symbols[table->num_symbols]);
    strncpy(symbol->name, name, MAX_NAME_LENGTH);
    symbol->value = value;
    table->num_symbols++;
}

Symbol* lookup_symbol(SymbolTableStack* stack, char* name) {
    for (int i = stack->num_tables - 1; i >= 0; i--) {
        SymbolTable* table = stack->tables[i];
        for (int j = 0; j < table->num_symbols; j++) {
            Symbol* symbol = &(table->symbols[j]);
            if (strcmp(symbol->name, name) == 0) {
                return symbol;
            }
        }
    }
    return NULL;
}

void push_symbol_table(SymbolTableStack* stack, SymbolTable* table) {
    if (stack->num_tables >= MAX_SYMBOLS) {
        printf("Error: symbol table stack overflow\n");
        exit(1);
    }
    stack->tables[stack->num_tables] = table;
    stack->num_tables++;
}

void pop_symbol_table(SymbolTableStack* stack) {
    if (stack->num_tables <= 0) {
        printf("Error: symbol table stack underflow\n");
        exit(1);
    }
    stack->num_tables--;
}

int main() {
    SymbolTableStack stack;
    stack.num_tables = 0;

    SymbolTable* global_table = create_symbol_table();
    add_symbol(global_table, "x", 1);
    add_symbol(global_table, "y", 2);
    push_symbol_table(&stack, global_table);

    SymbolTable* local_table = create_symbol_table();
    add_symbol(local_table, "x", 3);
    add_symbol(local_table, "z", 4);
    push_symbol_table(&stack, local_table);

    Symbol* symbol = lookup_symbol(&stack, "x");
    printf("x = %d\n", symbol->value);

    symbol = lookup_symbol(&stack, "y");
    printf("y = %d\n", symbol->value);

    symbol = lookup_symbol(&stack, "z");
    printf("z = %d\n", symbol->value);

    pop_symbol_table(&stack);

    symbol = lookup_symbol(&stack, "x");
    printf("x = %d\n", symbol->value);

    symbol = lookup_symbol(&stack, "y");
    printf("y = %d\n", symbol->value);

    symbol = lookup_symbol(&stack, "z");
    if (symbol == NULL) {
        printf("z is not defined\n");
    }

    return 0;
}
