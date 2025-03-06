#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_TODOS 20
#define FILENAME "todos.dat"
#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_CYAN "\033[36m"
#define BOLD "\033[1m"

typedef struct {
    char title[50];
    char created_at[20];
    bool completed;
} Todo;

Todo todos[MAX_TODOS];
int todo_count = 0;

void clear_screen() {
    system("clear || cls");
}

void print_header() {
    clear_screen();
    printf(COLOR_CYAN);
    printf("┌───────────────────────────────────────────────┐\n");
    printf("│              " COLOR_YELLOW BOLD "TODO MANAGER 3000" COLOR_RESET COLOR_CYAN "             │\n");
    printf("└───────────────────────────────────────────────┘\n" COLOR_RESET);
}

void save_todos() {
    FILE *file = fopen(FILENAME, "wb");
    if (file) {
        fwrite(todos, sizeof(Todo), todo_count, file);
        fclose(file);
    }
}

void load_todos() {
    FILE *file = fopen(FILENAME, "rb");
    if (file) {
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        rewind(file);

        int max_possible = size / sizeof(Todo);
        todo_count = (max_possible > MAX_TODOS) ? MAX_TODOS : max_possible;

        // Read actual number of items successfully read
        size_t items_read = fread(todos, sizeof(Todo), todo_count, file);
        todo_count = (int)items_read;
        fclose(file);
    }
}

void add_todo() {
    if (todo_count >= MAX_TODOS) {
        printf(COLOR_RED "\n⚠️ Todo list is full! Delete some todos first.\n" COLOR_RESET);
        return;
    }

    printf(COLOR_GREEN "\n✏️  Add New Todo\n" COLOR_RESET);
    printf(COLOR_YELLOW "───────────────────────────────\n" COLOR_RESET);

    printf("Enter todo description: ");
    fflush(stdout);

    // Safe input handling with length check
    if (!fgets(todos[todo_count].title, sizeof(todos[todo_count].title), stdin)) {
        printf(COLOR_RED "\n⚠️ Error reading input!\n" COLOR_RESET);
        return;
    }

    // Trim newline and check length
    size_t len = strlen(todos[todo_count].title);
    if (len > 0 && todos[todo_count].title[len-1] == '\n') {
        todos[todo_count].title[--len] = '\0';
    }
    if (len == 0) {
        printf(COLOR_RED "\n❌ Todo title cannot be empty!\n" COLOR_RESET);
        return;
    }

    time_t now = time(NULL);
    struct tm tm;
    localtime_r(&now, &tm);
    strftime(todos[todo_count].created_at, sizeof(todos[todo_count].created_at),
             "%d/%m %H:%M", &tm);

    todos[todo_count].completed = false;
    todo_count++;

    printf(COLOR_GREEN "\n✅ Todo added successfully!\n" COLOR_RESET);
    sleep(1);
}

void display_todos() {
    if (todo_count == 0) {
        printf(COLOR_YELLOW "\n📭 No todos found! Add some todos to get started.\n" COLOR_RESET);
        return;
    }

    printf(COLOR_CYAN "\n📋 Todo List\n");
    printf("┌─────┬──────────────────────────────────────┬────────────┬────────────┐\n");
    printf("│ " BOLD "ID " COLOR_RESET COLOR_CYAN "│ " BOLD "%-40s " COLOR_RESET COLOR_CYAN "│ " BOLD "Created  " COLOR_RESET COLOR_CYAN "│ " BOLD "Status   " COLOR_RESET COLOR_CYAN "│\n", "Description");
    printf("├─────┼──────────────────────────────────────┼────────────┼────────────┤\n");

    for (int i = 0; i < todo_count; i++) {
        // Increased buffer size for status messages
        char status[32];
        if (todos[i].completed) {
            snprintf(status, sizeof(status), "%sDONE%s%s", COLOR_GREEN, COLOR_RESET, COLOR_CYAN);
        } else {
            snprintf(status, sizeof(status), "%sPENDING%s%s", COLOR_RED, COLOR_RESET, COLOR_CYAN);
        }

        printf("│ " COLOR_YELLOW "%3d " COLOR_CYAN "│ %-40.40s │ %-10.10s │ %-10.10s │\n",
               i + 1, todos[i].title, todos[i].created_at, status);

        if (i < todo_count - 1) {
            printf("├─────┼──────────────────────────────────────┼────────────┼────────────┤\n");
        }
    }
    printf("└─────┴──────────────────────────────────────┴────────────┴────────────┘\n" COLOR_RESET);
}

void mark_complete() {
    int id;
    printf("\nEnter todo ID to mark complete: ");
    if (scanf("%d", &id) != 1) {
        printf(COLOR_RED "\n⚠️ Invalid input! Please enter a number.\n" COLOR_RESET);
        while (getchar() != '\n');
        return;
    }

    if (id < 1 || id > todo_count) {
        printf(COLOR_RED "\n⚠️ Invalid ID! Please try again.\n" COLOR_RESET);
        return;
    }

    todos[id-1].completed = true;
    printf(COLOR_GREEN "\n🎉 Todo marked as complete!\n" COLOR_RESET);
    sleep(1);
}

void delete_todo() {
    int id;
    printf("\nEnter todo ID to delete: ");
    if (scanf("%d", &id) != 1) {
        printf(COLOR_RED "\n⚠️ Invalid input! Please enter a number.\n" COLOR_RESET);
        while (getchar() != '\n');
        return;
    }

    if (id < 1 || id > todo_count) {
        printf(COLOR_RED "\n⚠️ Invalid ID! Please try again.\n" COLOR_RESET);
        return;
    }

    for (int i = id-1; i < todo_count-1; i++) {
        todos[i] = todos[i+1];
    }
    todo_count--;

    printf(COLOR_GREEN "\n🗑️ Todo deleted successfully!\n" COLOR_RESET);
    sleep(1);
}

void print_menu() {
    printf(COLOR_YELLOW "\n🛠️  Main Menu\n");
    printf("───────────────────────────────\n" COLOR_RESET);
    printf(COLOR_CYAN "[A]" COLOR_RESET "dd new todo\n");
    printf(COLOR_CYAN "[D]" COLOR_RESET "elete todo\n");
    printf(COLOR_CYAN "[C]" COLOR_RESET "omplete todo\n");
    printf(COLOR_CYAN "[Q]" COLOR_RESET "uit program\n");
    printf(COLOR_YELLOW "───────────────────────────────\n" COLOR_RESET);
    printf("Enter your choice: ");
}

int main() {
    clear_screen();
    printf(COLOR_YELLOW BOLD);
    printf("Initializing Todo Manager...\n");
    for(int i = 0; i < 3; i++) {
        printf("✨");
        fflush(stdout);
        sleep(1);
    }
    printf(COLOR_RESET "\n");

    load_todos();

    while(1) {
        print_header();
        display_todos();
        print_menu();

        char choice;
        scanf(" %c", &choice);
        choice = toupper(choice);

        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        switch(choice) {
            case 'A':
                add_todo();
                break;
            case 'D':
                delete_todo();
                break;
            case 'C':
                mark_complete();
                break;
            case 'Q':
                save_todos();
                printf(COLOR_GREEN "\n👋 Goodbye! Your todos are saved.\n" COLOR_RESET);
                exit(0);
            default:
                printf(COLOR_RED "\n⚠️ Invalid choice! Please try again.\n" COLOR_RESET);
                sleep(1);
                break;
        }
        save_todos();
    }
    return 0;
}
