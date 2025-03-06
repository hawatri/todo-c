import os
import pickle
from datetime import datetime

class Todo:
    def __init__(self, title, created_at, completed=False):
        self.title = title
        self.created_at = created_at
        self.completed = completed

class TodoManager:
    COLOR_RED = '\033[31m'
    COLOR_GREEN = '\033[32m'
    COLOR_YELLOW = '\033[33m'
    COLOR_CYAN = '\033[36m'
    COLOR_RESET = '\033[0m'
    BOLD = '\033[1m'

    def __init__(self, filename="todos.dat", max_todos=20):
        self.filename = filename
        self.max_todos = max_todos
        self.todos = []
        self.load_todos()

    def clear_screen(self):
        os.system('cls' if os.name == 'nt' else 'clear')

    def print_header(self):
        self.clear_screen()
        print(f"{self.COLOR_CYAN}┌───────────────────────────────────────────────┐")
        print(f"│              {self.BOLD}{self.COLOR_YELLOW}TODO MANAGER 3000{self.COLOR_RESET}{self.COLOR_CYAN}             │")
        print(f"└───────────────────────────────────────────────┘{self.COLOR_RESET}")

    def save_todos(self):
        with open(self.filename, 'wb') as f:
            pickle.dump(self.todos, f)

    def load_todos(self):
        try:
            if os.path.exists(self.filename):
                with open(self.filename, 'rb') as f:
                    self.todos = pickle.load(f)
        except Exception as e:
            print(f"{self.COLOR_RED}Error loading todos: {e}{self.COLOR_RESET}")

    def add_todo(self):
        if len(self.todos) >= self.max_todos:
            print(f"{self.COLOR_RED}\n⚠️ Todo list is full! Delete some todos first.{self.COLOR_RESET}")
            return

        print(f"{self.COLOR_GREEN}\n✏️  Add New Todo")
        print(f"{self.COLOR_YELLOW}───────────────────────────────{self.COLOR_RESET}")
        title = input("Enter todo description: ").strip()

        if not title:
            print(f"{self.COLOR_RED}\n❌ Todo title cannot be empty!{self.COLOR_RESET}")
            return

        created_at = datetime.now().strftime("%d/%m %H:%M")
        self.todos.append(Todo(title, created_at))
        print(f"{self.COLOR_GREEN}\n✅ Todo added successfully!{self.COLOR_RESET}")

    def display_todos(self):
        if not self.todos:
            print(f"{self.COLOR_YELLOW}\n📭 No todos found! Add some todos to get started.{self.COLOR_RESET}")
            return

        print(f"{self.COLOR_CYAN}\n📋 Todo List")
        print("┌─────┬──────────────────────────────────────┬────────────┬────────────┐")
        print(f"│ {self.BOLD}ID {self.COLOR_RESET}{self.COLOR_CYAN}│ {self.BOLD}%-40s {self.COLOR_RESET}{self.COLOR_CYAN}│ {self.BOLD}Created  {self.COLOR_RESET}{self.COLOR_CYAN}│ {self.BOLD}Status   {self.COLOR_RESET}{self.COLOR_CYAN}│" % "Description")
        print("├─────┼──────────────────────────────────────┼────────────┼────────────┤")

        for idx, todo in enumerate(self.todos, 1):
            status = f"{self.COLOR_GREEN}DONE{self.COLOR_RESET}{self.COLOR_CYAN}" if todo.completed else f"{self.COLOR_RED}PENDING{self.COLOR_RESET}{self.COLOR_CYAN}"
            title = todo.title
            if todo.completed:
                title = f"\033[9m{title}\033[0m"

            print(f"│ {self.COLOR_YELLOW}{idx:3} {self.COLOR_CYAN}│ {title[:35]:<35} │ {todo.created_at:10} │ {status:10} │")
            print("├─────┼──────────────────────────────────────┼────────────┼────────────┤" if idx < len(self.todos) else "└─────┴──────────────────────────────────────┴────────────┴────────────┘")

        print(f"{self.COLOR_RESET}")

    def mark_complete(self):
        try:
            todo_id = int(input("\nEnter todo ID to mark complete: "))
            if 1 <= todo_id <= len(self.todos):
                self.todos[todo_id-1].completed = True
                print(f"{self.COLOR_GREEN}\n🎉 Todo marked as complete!{self.COLOR_RESET}")
            else:
                print(f"{self.COLOR_RED}\n⚠️ Invalid ID!{self.COLOR_RESET}")
        except ValueError:
            print(f"{self.COLOR_RED}\nPlease enter a number!{self.COLOR_RESET}")

    def delete_todo(self):
        try:
            todo_id = int(input("\nEnter todo ID to delete: "))
            if 1 <= todo_id <= len(self.todos):
                del self.todos[todo_id-1]
                print(f"{self.COLOR_GREEN}\n🗑️ Todo deleted successfully!{self.COLOR_RESET}")
            else:
                print(f"{self.COLOR_RED}\n⚠️ Invalid ID!{self.COLOR_RESET}")
        except ValueError:
            print(f"{self.COLOR_RED}\nPlease enter a number!{self.COLOR_RESET}")

    def show_menu(self):
        while True:
            print(f"\n{self.COLOR_YELLOW}🛠️  Main Menu")
            print(f"───────────────────────────────{self.COLOR_RESET}")
            print(f"{self.COLOR_CYAN}[A]{self.COLOR_RESET}dd new todo")
            print(f"{self.COLOR_CYAN}[D]{self.COLOR_RESET}elete todo")
            print(f"{self.COLOR_CYAN}[C]{self.COLOR_RESET}omplete todo")
            print(f"{self.COLOR_CYAN}[Q]{self.COLOR_RESET}uit program")
            print(f"{self.COLOR_YELLOW}───────────────────────────────{self.COLOR_RESET}")
            choice = input("Enter your choice: ").upper()

            if choice == 'A':
                self.add_todo()
            elif choice == 'D':
                self.delete_todo()
            elif choice == 'C':
                self.mark_complete()
            elif choice == 'Q':
                self.save_todos()
                print(f"{self.COLOR_GREEN}\n👋 Goodbye! Your todos are saved.{self.COLOR_RESET}")
                exit()
            else:
                print(f"{self.COLOR_RED}\n⚠️ Invalid choice!{self.COLOR_RESET}")

            self.save_todos()
            input("\nPress Enter to continue...")
            self.print_header()
            self.display_todos()

if __name__ == "__main__":
    manager = TodoManager()
    manager.print_header()
    print(f"{manager.COLOR_YELLOW}Initializing Todo Manager...✨✨✨{manager.COLOR_RESET}")
    manager.display_todos()
    manager.show_menu()
