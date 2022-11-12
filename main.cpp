/*
// Title: main.cpp
// Author: Jonathan L Jacobson
// Date:   11 Nov 2022
*/

#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
using namespace std;
namespace fs = filesystem;

int main (){
  // This path will hold the drafts to be published
  fs::path path = fs::current_path() / "TestSpace/Publish";
  // holds the current line of the document being processed
  string line;
  // holds the words of the line
  string firstWord;
  string nextWord;
  // strings to hold document information.
  string docTitle;
  string docAuthor;
  string docDate;
  string docTags;

  // Available document labels (not post tags). unusedLast (unused) must be the last tag in enum.
  enum t_label {Title, Author, Date, Tags, unusedLast};
  t_label current_DocTag;

  for(const auto & file : fs::directory_iterator(path)) {
    cout << file.path() << endl;
    ifstream sourceFile;
    sourceFile.open(file.path());

    if(sourceFile.is_open()){
      for(int i = 0; getline(sourceFile, line); i++){
        istringstream iss(line);

        iss >> firstWord; // get the first word
        for(int enumInt = 0, current_DocTag = NULL; enumInt != unusedLast; enumInt++){
          if(firstWord == to_string(t_label(enumInt)))
            current_DocTag = t_label(enumInt);
        }

        switch (current_DocTag)
        {
        case Title:
          // Get the rest of the line and set to the docTitle variable.
          while(iss >> nextWord){
            if(docTitle != "") // if this is the second+ word in the title, append a space and the word
            docTitle = docTitle + " " + nextWord;
            else // if this is the first word in the title, don't add the space.
            docTitle = nextWord;
          }
          break;
        case Author:
          // Get the rest of the line and set to the docAuthor variable.
          while(iss >> nextWord){
            if(docAuthor != "")
            docAuthor = docAuthor + ", " + nextWord;
            else
            docAuthor = nextWord;
          }
          break;
        case Date:
          // Get the rest of the line and set to the docDate variable.
          while(iss >> nextWord){
            if(docDate!= "")
            docDate = docDate + ", " + nextWord;
            else
            docDate = nextWord;
          }
          break;
        case Tags:
          // Get the rest of the line and set to the docTags variable.
          while(iss >> nextWord){
            if(docTags != "")
            docTags = docTags + ", " + nextWord;
            else
            docTags = nextWord;
          }
          break;
        default: // A document label was not found in this line. Write this line
                 // to the content (paragraphs etc.)
          //if(!(i == 0)){ // If the first line doesn't have a tag, it's probably (?) the
                         // path to the file being read.
          //}
          break;
        }
        //cout << line << '\n'; // Debug, prints every line in the document
      }
      sourceFile.close();
      // print out labels parsed from the document
      cout << "Title: "  << docTitle  << '\n';
      cout << "Author: " << docAuthor << '\n';
      cout << "Date: "   << docDate   << '\n';
      cout << "Tags: "   << docTags   << '\n';

      //cout << document text << '\n'; should add the document text here as well
    }
    else cout << "Error! File %s could not be opened." << file.path() << endl;
  }
  return 0;
}