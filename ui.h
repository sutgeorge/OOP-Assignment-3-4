#ifndef __UI_H__
#define __UI_H__
#include "service.h"

typedef struct {
    Service* service;
    int running;
    char* last_command;
} UI;

UI create_ui(Service* service);
void ui_exit(UI* ui);
void ui_split_into_tokens(UI* ui, char* tokens[], int* token_index);
void ui_add(UI* ui);
void ui_list(UI* ui);
void ui_update(UI* ui);
void ui_delete(UI* ui);
void ui_undo(UI* ui);
void ui_redo(UI* ui);
void run(UI* ui);

#endif
