# Jot++

Jot++ is a fast and lightweight note-taking program written in C++.

![Jot++ Main Window](images/screenshoot.png)

## Features

-   **Fast** note-taking.
-   **Lightweight** design.
-   User-**friendly** interface.
-   Supports **light** and **dark** mode.
-   **Export** notes as `.txt` or `.md`.
-   Simple and clean UI powered by **FLTK**.

## Requirements

-   Compiler: `g++`
-   Build Tool: `make`
-   Dependencies:
    -   FLTK 1.4 (`libfltk` and `libfltk_images`)
    -   SQLite3 (`libsqlite3`)

### Download requirements

``` bash
# Install dependencies on macOS using Homebrew
brew install fltk sqlite

# On Linux (Ubuntu/Debian)
sudo apt install g++ make libfltk1.3-dev libsqlite3-dev
```

## Installation

### From Releases

Download a release from releases page.
After downloading, give **Jot++** execution permission by running the command below in your terminal:
```bash
# Replace this with the actual path where you extracted Jot++
chmod +x /path/to/Jot++/jotplusplus
```
Once the file is executable, simply double-click Jot++ to launch it.

### From Source Code (Advance users only)

Install Jot++ by running the commands below in your terminal:

``` bash
git clone https://github.com/selimozturk13/Jot_Plus_Plus.git
cd Jot_Plus_Plus
git clone https://github.com/SqliteModernCpp/sqlite_modern_cpp
make && sudo make install
```

Then, open it by running `./jotplusplus` in your terminal or by clicking
the installed application from your file manager.

## Usage

Open **Jot++**.

### To **create** a note

-   Write the note title into the title input field.
-   Write the content of the note into the editor.
-   Press the **create** button.

### To **view** and **edit** a note

-   Click on the note you want to view or edit.
-   The note's content will appear in the editor.
-   Edit it as you like.
-   Press the **save** button when you are done.

### To **delete** a note

-   Click the note you want to delete.
-   Press the **delete** button.

### To **export notes**

-   Open the **File** menu.
-   Click **Export**.
-   Choose the file type (`.txt` or `.md`).
-   Your file manager will open; choose the folder to export your note.

### To change **view** settings

-   Open the **View** menu.
-   Choose between light/dark modes or adjust other view-related
    settings.
