#ifndef __TESTS_H__
#define __TESTS_H__
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "signal.h"
#include "repository.h"
#include "service.h"
#include "ui.h"

void test_signal_creation_and_getters();
void test_signal_setters();
void test_signal_as_string();
void test_repository_creation();
void test_repository_add_operation();
void test_repository_delete_operation();
void test_repository_update_operation();
void test_repository_search_operation();
void test_service_creation();
void test_service_add_operation();
void test_service_delete_operation();
void test_service_update_operation();
void test_service_get_container_operation();
void test_create_undo_stack();
void test_push_command();
void test_pop_command();
void test_service_split_into_tokens();
void test_service_get_reversed_command_of_add();
void test_service_get_reversed_command_of_delete();
void test_service_get_reversed_command_of_update();
void test_service_push_last_command_on_stack__command_add();
void test_service_push_last_command_on_stack__command_delete();
void test_service_push_last_command_on_stack__command_update();
void test_service_undo_for_add();
void test_service_undo_for_delete();
void test_service_undo_for_update();
void run_all_tests();

#endif
