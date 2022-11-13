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

// Available document labels (not post tags). unusedLast must be the last tag.
string t_label[]{
    "Title:",
    "Author:",
    "Date:",
    "Tags:"};

string todaysDate()
{
  // using namespace date;
  // using namespace std::chrono;
  // auto local = make_zoned(current_zone(), system_clock::now());
  // return local;
  return "January 1, 1970";
}

class Document
{
public:
  string URL, Title, Tags;
  string Author = "Frankie";
  string Date = todaysDate();
} doc;

int getFileNum(fs::path Path)
{
  string base_filename = Path;
  base_filename = base_filename.substr(base_filename.find_last_of("/") + 1);
  string::size_type const p = base_filename.find_last_of(".");
  base_filename = base_filename.substr(0, p);
  return stoi(base_filename);
}

fs::path copyTemplate()
{
  // get the base url
  fs::path livePath = fs::current_path() / "TestSpace/Live/articles";
  int articleNum = 0;
  int fileNum;
  string articleTitle;
  stringstream ss;

  // look in the directory for the numbers (links) of the other articles. Increment and assign.
  for (const auto &liveFile : fs::directory_iterator(livePath))
  {
    // cout << liveFile.path() << endl;
    fileNum = getFileNum(liveFile.path());
    if (articleNum <= fileNum)
    {
      articleNum = fileNum + 1;
    }
  }
  ss << setfill('0') << setw(10) << articleNum;
  articleTitle = ss.str() + ".html";
  // copy the template
  ifstream src("TestSpace/Templates/articleTemplate.html", ios::binary);
  ofstream dst("TestSpace/Live/articles/" + articleTitle, ios::binary);
  dst << src.rdbuf();

  return (livePath / articleTitle);
}

int updateTags(fs::path link, string tags)
{
  return 0;
}

int fillLabel(string label, istringstream &iss)
{
  string nextWord;
  if (label == "Title:")
  {
    // Get the rest of the line and set to the docTitle variable.
    doc.Title = "";
    while (iss >> nextWord)
    {
      if (doc.Title != "")
      { // if this is the second+ word in the title, append a space and the word
        doc.Title = doc.Title + " " + nextWord;
      }
      else
      { // if this is the first word in the title, don't add the space.
        doc.Title = nextWord;
      }
    }
  }
  else if (label == "Author:")
  {
    // Get the rest of the line and set to the doc.Author variable.
    doc.Author = "";
    while (iss >> nextWord)
    {
      if (doc.Author != "")
        doc.Author = doc.Author + " " + nextWord;
      else
        doc.Author = nextWord;
    }
  }
  else if (label == "Date:")
  {
    // Get the rest of the line and set to the doc.Date variable.
    doc.Date = "";
    while (iss >> nextWord)
    {
      if (doc.Date != "")
        doc.Date = doc.Date + " " + nextWord;
      else
        doc.Date = nextWord;
    }
  }
  else if (label == "Tags:")
  {
    // Get the rest of the line and set to the docTags variable.
    doc.Tags = ""; // when a new document object is created for each file, delete this line to append tags instead of overwriting.
    while (iss >> nextWord)
    {
      if (doc.Tags != "")
        doc.Tags = doc.Tags + " " + nextWord;
      else
        doc.Tags = nextWord;
    }
  }
  else
  {
    cout << "Invalid label " << label << ". Check for typos?." << endl;
  }
  return 0;
}

int fillTemplate(fs::path Path)
{
  return 0;
}

int main()
{
  // This path will hold the drafts to be published
  fs::path path = fs::current_path() / "TestSpace/Publish";
  // holds the current line of the document being processed
  string line;
  // holds the words of the line
  string firstWord;

  for (const auto &file : fs::directory_iterator(path))
  {
    cout << file.path() << endl;
    ifstream sourceFile;
    sourceFile.open(file.path());

    if (sourceFile.is_open())
    {
      for (int i = 0; getline(sourceFile, line); i++)
      {
        istringstream iss(line);

        iss >> firstWord; // get the first word
        // cout << firstWord << endl;
        for (int enumInt = 0; enumInt != (sizeof(t_label) / sizeof(t_label[0])); enumInt++)
        {
          // cout << t_label[enumInt] << endl;
          if (firstWord == t_label[enumInt])
          {
            fillLabel(t_label[enumInt], iss);
            break;
          }
        }
      }
      sourceFile.close();
      // print out labels parsed from the document
      cout << "Title: " << doc.Title << '\n';
      cout << "Author: " << doc.Author << '\n';
      cout << "Date: " << doc.Date << '\n';
      cout << "Tags: " << doc.Tags << '\n';
      // cout << document text << '\n'; should add the document text here as well

      // open a new template file and fill in the template with all of this info.
      doc.URL = copyTemplate();
      fillTemplate(doc.URL);
      // doc.linkTags(); // add a link to this doc on every relevant tag page.
      // This should probably be a class function
    }
    else
      cout << "Error! File %s could not be opened." << file.path() << endl;
  }
  return 0;
}