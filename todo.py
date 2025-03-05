import os
import pickle
from datetime import datetime

# Configuration constants
MAX_TODOS = 20
TODO_FILE = "todos.pkl"

class TodoItem:
    def __init__(self, title, created_at, is_completed=False):
        self.title = title
        self.created_at = created_at
        self.is_completed = is_completed

# Global list to store todos
todos = []

def save_to_file():
    """Save todos to a file using pickle"""
    try:
        with open(TODO_FILE, 'wb') as f:
            pickle.dump(todos, f)
    except Exception as e:
        print(f"Oops! Couldn't save todos 😞 Error: {e}")

def load_from_file():
    """Load todos from a file if it exists"""
    global todos
    try:
        if os.path.exists(TODO_FILE):
            with open(TODO_FILE, 'rb') as f:
                todos = pickle.load(f)
            print("\nLoaded existing todos! 📂")
        else:
            print("\nNo existing todos found. Let's start fresh! 🌟")
    except Exception as e:
        print(f"Error loading todos: {e}")

def add_todo():
    """Add a new todo to the list"""
    if len(todos) >= MAX_TODOS:
        print("Todo list is full! 📚")
        return

    title = input("\nWhat's your new todo? 📝\n>> ").strip()
    if not title:
        print("Todo title can't be empty! ❌")
        return

    # Create timestamp
    now = datetime.now()
    created_at = now.strftime("%d/%m %H:%M")

    new_todo = TodoItem(title, created_at)
    todos.append(new_todo)
    print("Todo added successfully! ✅")

def display_todos():
    """Show all todos in a nice table format"""
    if not todos:
        print("\nNo todos yet! Add your first todo! ✨")
        return

    # Table header
    print("\n+----+-------------------------------------+--------------+-------------+")
    print("| ID |            Todo Title               |  Created at  |  Completed  |")
    print("+----+-------------------------------------+--------------+-------------+")

    for index, todo in enumerate(todos, 1):
        # Format completion status
        status = "✅ Yes" if todo.is_completed else "❌ No"

        # Add strikethrough for completed items
        title = todo.title
        if todo.is_completed:
            title = f"\033[9m{title}\033[0m"

        print(f"|{index:3} | {title[:35]:<35} | {todo.created_at:12} | {status:10} |")
        print("+----+-------------------------------------+--------------+-------------+")

def complete_todo():
    """Mark a todo as completed"""
    display_todos()
    if not todos:
        return

    try:
        todo_id = int(input("\nWhich todo did you finish? 🎉\n>> "))
        if 1 <= todo_id <= len(todos):
            todos[todo_id-1].is_completed = True
            print("Great job completing this! 💪")
        else:
            print("Invalid todo ID! 😕")
    except ValueError:
        print("Please enter a number! 🔢")

def delete_todo():
    """Remove a todo from the list"""
    display_todos()
    if not todos:
        return

    try:
        todo_id = int(input("\nWhich todo to delete? 🗑️\n>> "))
        if 1 <= todo_id <= len(todos):
            del todos[todo_id-1]
            print("Todo deleted successfully! 🔥")
        else:
            print("Invalid todo ID! 😕")
    except ValueError:
        print("Please enter a number! 🔢")

def show_menu():
    """Display and handle the main menu"""
    while True:
        print("\nChoose action:")
        print("[A]dd todo  [D]elete todo  [C]omplete todo  [Q]uit")
        choice = input(">> ").upper()

        if choice == 'A':
            add_todo()
        elif choice == 'D':
            delete_todo()
        elif choice == 'C':
            complete_todo()
        elif choice == 'Q':
            print("\nGoodbye! 👋")
            save_to_file()
            exit()
        else:
            print("Hmm, I don't understand that command 🤔")
            continue

        save_to_file()
        display_todos()

def main():
    """Main program entry point"""
    os.system('cls' if os.name == 'nt' else 'clear')  # Clear screen
    print("\033[32;1m")  # Green text

    load_from_file()
    display_todos()
    show_menu()

if __name__ == "__main__":
    main()
