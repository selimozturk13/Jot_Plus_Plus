#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <exception>
#include <fstream>
#include "sqlite_modern_cpp/hdr/sqlite_modern_cpp.h"
#include <ostream>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

Fl_Text_Editor* editor;
Fl_Hold_Browser* browser;
sqlite::database db("notes.db");
string titleofnote;
Fl_Text_Buffer *editorbuffer=new Fl_Text_Buffer();
Fl_Input* input;
int darkmode;



void writecallback(Fl_Widget*,void*)
{
    try 
    {
        db<<"INSERT OR REPLACE INTO notes(title,content) VALUES (?,?)"<<titleofnote<<editorbuffer->text();
    }
    catch (exception &e)
    {
        cerr << "SQLite error: "<<e.what()<<endl;
    }
    fl_message("Note saved succesfully.");
}

void on_select(Fl_Widget* w, void* data) {
    int idx = browser->value();
    if(idx <= 0) return; 
    titleofnote = browser->text(idx);
    string note_title = browser->text(idx); 

    try {
        db << "SELECT content FROM notes WHERE title=?;" << note_title
           >> [&](string content) {
               editorbuffer->text(content.c_str());
           };
    } catch (const exception &e) {
        cerr << "SQLite error: " << e.what() << endl;
    }
}

void delcallback(Fl_Widget*,void*)
{
    vector<string> titles;
    int choice=fl_choice("Do you want to delete the note?", "Yes", "No", NULL);
    if(choice==1) return;
    try 
    {
        db<<"DELETE FROM notes WHERE title=?"<<titleofnote;
        editorbuffer->text("");
        
        db<<"SELECT * FROM notes;">>[&titles](int id,string title,string content){
            titles.push_back(title); 
        };
        browser->clear();
        for(const auto& n : titles)
            browser->add(n.c_str());
        browser->redraw();
        
    }
    catch (exception &e)
    {
        cerr << "SQLite error: "<<e.what()<<endl;
    }
}

void inputcallback(Fl_Widget*,void*)
{
    editorbuffer->text("");
}

void darklightmode(Fl_Widget*,void*)
{
    if (darkmode==1) {
        Fl::background(240, 240, 240);   
        Fl::foreground(20, 20, 20);     
        Fl::background2(255, 255, 255);
        Fl::redraw();
        darkmode=0;
    } 
    else {
        Fl::background(45, 45, 48);    
        Fl::foreground(230, 230, 230); 
        Fl::background2(60, 60, 65);
        Fl::redraw();
        darkmode=1;
    }
}

void reducesize(Fl_Widget*,void*)
{
    editor->textsize(editor->textsize()-5);
    editor->redraw();
    browser->textsize(browser->textsize()-5);
}

void increasesize(Fl_Widget*,void*)
{
    editor->textsize(editor->textsize()+5);
    editor->redraw();
    browser->textsize(browser->textsize()+5);
}

void exporttxt(Fl_Widget*,void*)
{
    Fl_Native_File_Chooser native_chooser;
    native_chooser.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
    native_chooser.title("Export Note as TXT");
    native_chooser.filter("TXT files\t*.txt");
    native_chooser.preset_file("Notes.txt");
    switch (native_chooser.show()) {
        case -1: 
            cerr << "File chooser ERROR\n";
            return;
        case 1: 
            return;
    }
    ofstream notes(native_chooser.filename());
    try {
        db << "SELECT * FROM notes;">> [&notes](int id,string title,string content){
            notes<<"ID: "<<id<<"\n"<<title<<"\n"<<content<<"\n";
        };
    }
    catch(exception &e) {
        cerr<<"Database error: "<<e.what()<<endl;
    }
    fl_message("Note exported succesfully");
}

void exportmd(Fl_Widget*,void*)
{
    Fl_Native_File_Chooser native_chooser;
    native_chooser.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
    native_chooser.title("Export Note as MARKDOWN");
    native_chooser.filter("TXT files\t*.md");
    native_chooser.preset_file("Notes.md");
    switch (native_chooser.show()) {
        case -1: 
            cerr << "File chooser ERROR\n";
            return;
        case 1: 
            return;
    }
    ofstream notes(native_chooser.filename());
    try {
        db << "SELECT * FROM notes;">> [&notes](int id,string title,string content){
            notes<<"ID: *"<<id<<"*\n**"<<title<<"**\n"<<content<<"\n";
        };
    }
    catch(exception &e) {
        cerr<<"Database error: "<<e.what()<<endl;
    }
    fl_message("Note exported succesfully");
    
}

int main()
{
    Fl::scheme("gtk+");
    Fl::background(45, 45, 48);    
    Fl::foreground(230, 230, 230); 
    Fl::background2(60, 60, 65);
    darkmode=1;
    int width=1000,height=500;
    Fl_Window *win=new Fl_Window(width,height,"Jot++");
    editorbuffer->text("\t\tJot++\n\n\tPress a note to view and edit in this editor.");
    int editor_x = width / 3;
    int editor_y = 28;
    int editor_w = width - editor_x;
    int editor_h = height-78;

    editor = new Fl_Text_Editor(editor_x, editor_y, editor_w, editor_h);
    editor->buffer(editorbuffer);
    editor->textsize(16);
    Fl_Button* save= new Fl_Button(890,editor_h+35,100,30,"Save");
    save->callback(writecallback);
    Fl_Button* del= new Fl_Button(780,editor_h+35,100,30,"Delete");
    del->callback(delcallback);
    
    input = new Fl_Input(560, editor_h+35, 100, 30, "Title:");
    input->textsize(16);
    input->callback(inputcallback);
    input->when(FL_WHEN_CHANGED);
    Fl_Button* create= new Fl_Button(670,editor_h+35,100,30,"Create");
    create->callback([](Fl_Widget* w, void* data){
        auto pair = (std::pair<Fl_Input*, Fl_Text_Buffer*>*)data;
        Fl_Input* input = pair->first;
        Fl_Text_Buffer* editorbuffer = pair->second;

        string title = input->value();
        string content = editorbuffer->text();
        vector<string> titles;

        try {
            db << "INSERT INTO notes(title,content) VALUES(?,?)" << title << content;
            db<<"SELECT * FROM notes;">>[&titles](int id,string title,string content){
            titles.push_back(title); 
        };
        input->value("");
        browser->clear();
        for(const auto& n : titles)
            browser->add(n.c_str());
        browser->redraw();
        } catch(const std::exception& e){
            cerr<<"SQLite error: "<<e.what()<<endl;
        }
        fl_message("Note created succesfuly.");
    }, new std::pair<Fl_Input*, Fl_Text_Buffer*>(input, editorbuffer));
    vector<string> titles_ids;
    try
    {
        
        db<<"CREATE TABLE IF NOT EXISTS notes (id INTEGER PRIMARY KEY,title TEXT,content TEXT);";
        db<<"SELECT * FROM notes;">>[&titles_ids](int id,string title,string content){
            titles_ids.push_back(title); 
        };
    } 
    catch (exception &e)
    {
        cerr<<"SQLite error: "<<e.what()<<endl;
    }
    browser = new Fl_Hold_Browser(10, 28, width/3 - 20, height - 78);
    for(const auto& n : titles_ids)
        browser->add(n.c_str());
    browser->callback(on_select);
    browser->textsize(16);

    Fl_Menu_Bar* menubar=new Fl_Menu_Bar(0,0,width,25);

    menubar->add("File/Export Notes/As a TXT file","",exporttxt,NULL);
    menubar->add("File/Export Notes/As a MARKDOWN file","",exportmd,NULL);
    menubar->add("View/Dark\\Light mode",FL_CTRL+'l',darklightmode,NULL);
    menubar->add("View/Reduce The Font Size",FL_CTRL+'r',reducesize,NULL);
    menubar->add("View/Increase The Font Size",FL_CTRL+'i',increasesize,NULL);

    win->end();
    win->show();
    return Fl::run();
}