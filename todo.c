#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

// Maximum number of todos our app can handle
#define MAX_TODOS 20
// File name where todos are saved
#define TODO_FILE "todos.bin"

int todoCount = 0;          // Tracks how many todos we currently have
FILE *todoFile;             // Pointer to manage our todo file

// Structure to store todo information
struct TodoItem {
    char title[50];         // Title of the todo (max 49 characters)
    char createdAt[50];     // When the todo was created (format: day/month hour:min)
    bool isCompleted;       // Whether the todo is done (true/false)
} todoItems[MAX_TODOS];     // Array to store all todo items

// Save all todos to a binary file
void saveToFile() {
    todoFile = fopen(TODO_FILE, "w");
    if (!todoFile) {
        printf("Oops! Couldn't save todos 😞\n");
    } else {
        // Write each todo to the file
        for (int i = 0; i < todoCount; i++) {
            fwrite(&todoItems[i], sizeof(struct TodoItem), 1, todoFile);
        }
        fclose(todoFile);
    }
}

// Calculate how many todos are stored in the file
void calculateTodoCount() {
    fseek(todoFile, 0L, SEEK_END);      // Jump to end of file
    long fileSize = ftell(todoFile);    // Get file size
    fseek(todoFile, 0L, SEEK_SET);      // Jump back to start
    todoCount = fileSize / sizeof(struct TodoItem); // Calculate number of todos
}

// Load todos from the file into our array
void loadFromFile() {
    todoFile = fopen(TODO_FILE, "r");
    if (!todoFile) {
        printf("No existing todos found. Let's start fresh! 🌟\n");
    } else {
        calculateTodoCount();
        // Read each todo from the file
        for (int i = 0; i < todoCount; i++) {
            fread(&todoItems[i], sizeof(struct TodoItem), 1, todoFile);
        }
        fclose(todoFile);
    }
}

// Add a new todo to the list
void addTodo() {
    char todoTitle[50];
    printf("What's your new todo? 📝\n>> ");
    scanf(" %[^\n]", todoTitle);  // Read until newline (space before % skips whitespace)

    // Copy title to our todo list
    strncpy(todoItems[todoCount].title, todoTitle, 50);

    // Get current time and format it
    time_t currentTimeRaw;
    struct tm currentTime;
    time(&currentTimeRaw);
    localtime_r(&currentTimeRaw, &currentTime);

    // Create time string (day/month hour:min)
    snprintf(todoItems[todoCount].createdAt, 50, "%d/%d %02d:%02d",
             currentTime.tm_mday,
             currentTime.tm_mon + 1,  // Months are 0-11, so +1 to make it 1-12
             currentTime.tm_hour,
             currentTime.tm_min);

    todoItems[todoCount].isCompleted = false;  // New todos start as incomplete
    todoCount++;  // Increase our todo counter
}

// Display all todos in a nice table
void displayTodos() {
    // Table header
    printf("\n+----+-------------------------------------+--------------+-------------+\n");
    printf("| ID |            Todo Title               |  Created at  |  Completed  |\n");
    printf("+----+-------------------------------------+--------------+-------------+\n");

    for (int i = 0; i < todoCount; i++) {
        // Strikethrough text for completed items
        if (todoItems[i].isCompleted) {
            printf("\033[9m");  // Enable strikethrough
        } else {
            printf("\033[1m");  // Bold text for active todos
        }

        // Print todo details
        printf("|%3d | %-35s | %-12s | %-10s |\n",
               i + 1,
               todoItems[i].title,
               todoItems[i].createdAt,
               todoItems[i].isCompleted ? "✅ Yes" : "❌ No");

        printf("+----+-------------------------------------+--------------+-------------+\n");
        printf("\033[0m");  // Reset text formatting
    }
}

// Mark a todo as complete using its ID
void completeTodo() {
    int todoId;
    printf("Which todo did you finish? 🎉\n>> ");
    scanf("%d", &todoId);
    todoId--;  // Convert to 0-based index

    if (todoId < 0 || todoId >= todoCount) {  // Check for valid ID
        printf("That ID doesn't exist! 😕\n");
    } else {
        todoItems[todoId].isCompleted = true;
        printf("Great job completing this! 💪\n");
    }
}

// Delete a todo from the list
void deleteTodo() {
    int todoId;
    printf("Which todo to delete? 🗑️\n>> ");
    scanf("%d", &todoId);

    if (todoId < 1 || todoId > todoCount) {  // Check for valid ID
        printf("That ID doesn't exist! 😕\n");
    } else {
        todoId--;  // Convert to 0-based index
        // Shift todos after the deleted one to fill the gap
        memmove(&todoItems[todoId], &todoItems[todoId + 1],
               (todoCount - todoId - 1) * sizeof(struct TodoItem));
        todoCount--;
        printf("Todo deleted successfully! 🔥\n");
    }
}

// Show menu options and handle user input
void showMenu() {
    char choice;
    printf("\nChoose action: (A)dd, (D)elete, (C)omplete, (Q)uit\n>> ");
    scanf(" %c", &choice);  // Space before %c skips any leftover newlines
    choice = toupper(choice);

    // Clear input buffer
    while (getchar() != '\n');

    switch (choice) {
        case 'A':
            addTodo();
            break;
        case 'D':
            deleteTodo();
            break;
        case 'C':
            completeTodo();
            break;
        case 'Q':
            printf("Goodbye! 👋\n");
            exit(0);
        default:
            printf("Hmm, I don't understand that command 🤔\n");
            showMenu();  // Show menu again if invalid input
            break;
    }

    saveToFile();     // Always save changes
    displayTodos();   // Show updated list
    showMenu();       // Keep showing menu until quit
}

// Check if this is the first time running the app
void checkFirstRun() {
    if (access(TODO_FILE, F_OK) != -1) {  // File exists
        loadFromFile();
        displayTodos();
        showMenu();
    } else {  // First run - create first todo
        printf("Welcome to your Todo Manager! 🚀\n");
        addTodo();
        saveToFile();
        displayTodos();
        showMenu();
    }
}

int main() {
    system("clear || cls");  // Clear screen for both Unix/Windows
    printf("\033[32;1m");    // Make text bright green
    checkFirstRun();
    return 0;
}
