#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ui.h"
#define true 1
#define false 0

UI create_ui(Service* service) {
    UI ui;
    ui.running = true;
    ui.service = service;
    ui.last_command = (char*)malloc(sizeof(char)*35);
    return ui;
}


void ui_exit(UI* ui) {
    ui->running = false;
}


void ui_split_into_tokens(UI* ui, char* tokens[], int* token_index) {
    char* token;
    *token_index = 0;
    token = strtok(ui->last_command, " ");

    while (token != NULL) {
        if ((*token_index) > 0) {
            tokens[*(token_index)-1] = token;
        }

        token = strtok(NULL, " ");
        (*token_index)++;
    }
}


void ui_add(UI* ui) {
    int id, priority_number, token_index;
    char modulated_signal[30], type[30];
    char* tokens[4];

    ui_split_into_tokens(ui, tokens, &token_index);

    if (token_index != 5) {
        printf("Wrong usage. The command \"add\" has 4 parameters: id, modulatedSignal, type, priorityNumber\n");
        return;
    }

    id = atoi(tokens[0]);
    strcpy(modulated_signal, tokens[1]);
    strcpy(type, tokens[2]);
    priority_number = atoi(tokens[3]);

    service_add(ui->service, id, modulated_signal, type, priority_number);
}


void ui_list(UI* ui) {
    SignalContainer container = service_get_container(ui->service);
    char* tokens[2];
    int token_index;

    ui_split_into_tokens(ui, tokens, &token_index);

    if (token_index < 1 || token_index > 2) {
        printf("Wrong usage. The command \"add\" has 4 parameters: id, modulatedSignal, type, priorityNumber\n");
        return;
    }


    SignalContainer sorted_container = container;
    int sorted = 0;

    while (!sorted) {
        sorted = 1;
        for (int i = 0; i < container.number_of_elements - 1; i++) {
            Signal aux;
            if (strcmp(get_modulated_signal(&sorted_container.signals[i]), get_modulated_signal(&sorted_container.signals[i + 1])) > 0) {
                aux = sorted_container.signals[i];
                sorted_container.signals[i] = sorted_container.signals[i + 1];
                sorted_container.signals[i + 1] = aux;
                sorted = 0;
            }
        }
    }


    for (int i = 0; i < sorted_container.number_of_elements; i++) {
        if (token_index == 2) {
            if (get_signal_priority_number(&sorted_container.signals[i]) < atoi(tokens[0])) {
                display_signal(&sorted_container.signals[i]);
            }
        } else {
            display_signal(&sorted_container.signals[i]);
        }
    }
}


void ui_update(UI* ui) {
    int id, new_priority_number, token_index;
    char new_modulated_signal[30], new_type[30];
    char* tokens[4];

    ui_split_into_tokens(ui, tokens, &token_index);

    if (token_index != 5) {
        printf("Wrong usage. The command \"update\" has 4 parameters: id, newModulatedSignal, newType, newPriorityNumber\n");
        return;
    }

    id = atoi(tokens[0]);
    strcpy(new_modulated_signal, tokens[1]);
    strcpy(new_type, tokens[2]);
    new_priority_number = atoi(tokens[3]);

    service_update(ui->service, id, new_modulated_signal, new_type, new_priority_number);
}


void ui_delete(UI* ui) {
    int id, token_index;
    char* tokens[4];

    ui_split_into_tokens(ui, tokens, &token_index);

    if (token_index != 2) {
        printf("Wrong usage. The command \"delete\" has 1 parameter: id\n");
        return;
    }

    id = atoi(tokens[0]);

    service_delete(ui->service, id);
}


void ui_undo(UI* ui) {
    service_undo(ui->service);
}


void ui_redo(UI* ui) {
    service_redo(ui->service);
}


void ui_help(UI* ui) {
    printf("  Available commands: add, list, update, delete, undo, redo, exit, clear, help\n");
    printf("        - add (id) (modulated signal) (type) (priority number)\n");
    printf("            -> adds a signal to the repository\n");
    printf("        - list\n");
    printf("            -> displays all the signals in the repository\n");
    printf("        - update (id) (new modulated signal) (new type) (new priority number)\n");
    printf("            -> updates a signal from the repository\n");
    printf("        - delete (id)\n");
    printf("            -> deletes a signal from the repository\n");
    printf("        - undo\n");
    printf("            -> undoes the last executed command\n");
    printf("        - redo\n");
    printf("            -> redoes the last undone command\n");
    printf("        - exit\n");
    printf("            -> exits the program\n");
}


void ui_clear(UI* ui) {
    for (int i = 0; i < 100; i++)
        printf("\n");
}


void run(UI* ui) {
    char* commands[] = {"exit", "add", "list", "update", "delete", "undo", "redo", "help", "clear"};
    void (*command_functions[])(UI*) = {ui_exit, ui_add, ui_list, ui_update, ui_delete, ui_undo, ui_redo, ui_help, ui_clear};
    char command[40];
    int number_of_commands = sizeof(commands)/sizeof(commands[0]);

    ui_help(ui);

    while (ui->running) {
        printf(">>");
        fgets(command, 40, stdin);
        strcpy(ui->last_command, command);
        int found = 0;

        for (int i = 0; i < number_of_commands && !found; i++) {
            if (strncmp(command, commands[i], strlen(commands[i])) == 0) {
                found = 1;

                if (strncmp(command, "undo", 4) != 0 &&
                strncmp(command, "redo", 4) != 0 &&
                strncmp(command, "help", 4) != 0 &&
                strncmp(command, "list", 4) != 0 &&
                strncmp(command, "clear", 5) != 0) {
                    service_push_last_command_on_stack(ui->service, command);
                }

                command_functions[i](ui);
            }
        }

        if (!found) {
            printf("The command doesn't exist!\n");
        }
    }

    free_service(ui->service);
    free(ui->last_command);
}
