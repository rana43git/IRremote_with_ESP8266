import tkinter as tk
from tkinter import filedialog, messagebox
import re
import os

import re

def extract_button_data(first_code):
    """Extracts integer values inside `{}` after `uint16_t rawData[67] =` and formats them with space separation."""
    pattern = r"uint16_t\s+rawData\[67\]\s*=\s*\{([^}]*)\};"
    matches = re.findall(pattern, first_code)

    if not matches:
        print("No matches found! Check if the input format is correct.")

    formatted_data = []
    for match in matches:
        numbers = re.findall(r'\d+', match)  # Extract all numbers
        formatted_data.append(" ".join(numbers))  # Join numbers with a single space

    return formatted_data if formatted_data else []


def update_buttons_in_irplus(second_code, button_data):
    """Replaces content inside <button>...</button> while keeping attributes unchanged."""
    updated_code = second_code
    button_pattern = re.compile(r'(<button[^>]*>)(.*?)</button>', re.DOTALL)

    def replacement(match):
        nonlocal button_data
        if button_data:
            return match.group(1) + button_data.pop(0) + "</button>"
        return match.group(0)  # If no more button data, keep existing content

    updated_code = button_pattern.sub(replacement, updated_code, count=len(button_data))
    
    return updated_code

def process_files(first_file, second_file):
    """Processes the files to replace button contents while keeping structure intact."""
    with open(first_file, 'r') as f:
        first_code = f.read()

    with open(second_file, 'r') as f:
        second_code = f.read()

    button_data = extract_button_data(first_code)

    if not button_data:
        messagebox.showerror("Error", "No button data found in the first file!")
        return

    updated_code = update_buttons_in_irplus(second_code, button_data)

    modified_file = os.path.splitext(second_file)[0] + "_Modified.irplus"

    with open(modified_file, 'w') as f:
        f.write(updated_code)

    messagebox.showinfo("Success", f"File saved as {modified_file}")

def select_file(file_type, file_extension):
    """Opens a file selection dialog and returns the selected file path."""
    root = tk.Tk()
    root.withdraw()
    file_path = filedialog.askopenfilename(title=f"Select {file_type} File", filetypes=[(f"{file_type} files", f"*.{file_extension}")])
    
    return file_path if file_path else None

def main():
    first_file = select_file("First Code", "txt")
    if not first_file:
        return

    second_file = select_file("Second Code", "irplus")
    if not second_file:
        return

    process_files(first_file, second_file)

if __name__ == "__main__":
    main()
