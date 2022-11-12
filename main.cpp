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
  fs::path path = fs::current_path() / "TestSpace/Publish";
  string line;
  string firstWord;
  enum tag {Title, Folder, Tags, Author, Date}

  for(const auto & file : fs::directory_iterator(path)) {
    cout << file.path() << endl;
    ifstream sourceFile;
    sourceFile.open(file.path());

    if(sourceFile.is_open()){
      for(int i = 0; getline(sourceFile, line); i++){
        istringstream iss(line);

        iss >> firstWord; // get the first word
        if(firstWord inside tag)
        switch (tag)
        {
        case /* constant-expression */:
          /* code */
          break;
        
        default:
          break;
        }
        

        //cout << line << '\n';

        }
      }
      sourceFile.close();
    } 
    else cout << "Error! File %s could not be opened." << file.path() << endl;
  }
  return 0;
}