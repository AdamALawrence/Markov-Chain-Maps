// -----------------------------------------------------------------
// HOMEWORK 7 WORD FREQUENCY MAPS
//
// You may use all of, some of, or none of the provided code below.
// You may edit it as you like (provided you follow the homework
// instructions).
// -----------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <cstdlib>
#include "assert.h"
#include "MersenneTwister.h"



// MY OWN TYPEDEFS
typedef std::map<std::string, int> MiniMap;
typedef std::map<std::string, MiniMap> MY_MAP_typedef;      // for window = 2
typedef std::map<std::string, MY_MAP_typedef> MegaMap;      // for window = 3


// Custom helper function that reads the input stream looking for
// double quotes (a special case delimiter needed below), and white
// space.  Contiguous blocks of alphabetic characters are lowercased &
// packed into the word.
bool ReadNextWord(std::istream &istr, std::string &word) {
  char c;
  word.clear();
  while (istr) {
    // just "peek" at the next character in the stream
    c = istr.peek();
    if (isspace(c)) {
      // skip whitespace before a word starts
      istr.get(c);
      if (word != "") {
	// break words at whitespace
	return true;
      }
    }
    else if (c == '"') {
      // double quotes are a delimiter and a special "word"
      if (word == "") {
	istr.get(c);
	word.push_back(c);
      }
      return true;
    }
    else if (isalpha(c)) {
      // this a an alphabetic word character
      istr.get(c);
      word.push_back(tolower(c));
    }
    else {
      // ignore this character (probably punctuation)
      istr.get(c);
    }
  }
  return false;
}


// Custom helper function that reads the input stream looking a
// sequence of words inside a pair of double quotes.  The words are
// separated by white space, but the double quotes might not have
// space between them and the neighboring word.  Punctuation is
// ignored and words are lowercased.
std::vector<std::string> ReadQuotedWords(std::istream &istr) {
  // returns a vector of strings of the different words
  std::vector<std::string> answer;
  std::string word;
  bool open_quote = false;
  while (ReadNextWord(istr,word)) {
    if (word == "\"") {
      if (open_quote == false) { open_quote=true; }
      else { break; }
    } else {
      // add each word to the vector
      answer.push_back(word);
    }
  }
  return answer;
}




// THIS ADDS TEXT TO DATA AT A WINDOW SIZE OF 2
void LoadSampleText2(MY_MAP_typedef &data, const std::string &filename, int window, const std::string &parse_method,std::vector<std::string> &vecText_kinda_global) {
  // open the file stream
  std::ifstream istr(filename.c_str());
  if (!istr) { 
    std::cerr << "ERROR cannot open file: " << filename << std::endl; 
    exit(1);
  }
  // verify the window parameter is appropriate
  if (window < 2) {
    std::cerr << "ERROR window size must be >= 2:" << window << std::endl;
  }
  // verify that the parse method is appropriate
  bool ignore_punctuation = false;
  if (parse_method == "ignore_punctuation") {
    ignore_punctuation = true;
  } else {
    std::cerr << "ERROR unknown parse method: " << parse_method << std::endl;
    exit(1);
  }

    std::vector<std::string> vecText;                           // ALL text is added to this vector for
    std::string word = "";                                      // access

    while (ReadNextWord(istr,word)) {                           // while true if istr is a word/character
        // skip the quotation marks (not used for this part)    // assigns read istr to std::string word
        if (word == "\"") continue;
        vecText.push_back(word);
        vecText_kinda_global.push_back(word);
    }
    
    for (int i = 0; i < (int)vecText.size()-1; i++) {           // this creates the main data structure that
        data[vecText[i]][vecText[i+1]] += 1;                    // holds all the words and pairs
    }
}

// SAME AS ABOVE FUNCTION BUT ADDS WORDS AT A DEPTH/WINDOW OF 3
void LoadSampleText3(MegaMap &data, const std::string &filename, int window, const std::string &parse_method) {
    // open the file stream
    std::ifstream istr(filename.c_str());
    if (!istr) {
        std::cerr << "ERROR cannot open file: " << filename << std::endl;
        exit(1);
    }
    // verify the window parameter is appropriate
    if (window < 2) {
        std::cerr << "ERROR window size must be >= 2:" << window << std::endl;
    }
    // verify that the parse method is appropriate
    bool ignore_punctuation = false;
    if (parse_method == "ignore_punctuation") {
        ignore_punctuation = true;
    } else {
        std::cerr << "ERROR unknown parse method: " << parse_method << std::endl;
        exit(1);
    }
    
    std::vector<std::string> vecText;
    std::string word = "";
    
    while (ReadNextWord(istr,word)) {

        if (word == "\"") continue;
        vecText.push_back(word);
    }

    for (int i = 0; i < (int)vecText.size()-2; i++) {
        data[vecText[i]][vecText[i+1]][vecText[i+2]] += 1;
    }
}


// a simple function that outputs a word and a number
void PrintWordCounts(std::string output, int count) {
    std::cout << output << " (" << count << ")" << std::endl;
}





// this function appends a word and another word to output
void FindNextWord2(MY_MAP_typedef &data, bool random_flag, std::string &word, std::vector<std::string> &output) {
    
    MY_MAP_typedef::iterator MainMap_itr = data.find(word); // find word of interest in main map
    
    if (MainMap_itr == data.end()) {return;}                // check if you can't find the word
    
    output.push_back(MainMap_itr->first);                   // if found, push_back()
    
    int max = 0;
    if (random_flag) {
        MTRand mtrand;
        int rand_index = (int)mtrand.randInt(MainMap_itr->second.size()-1); // get a random index
        MiniMap::iterator mini_itr = MainMap_itr->second.begin();           // itr looking at indiv pairs
        std::advance(mini_itr, rand_index);                                 // move itr to random spot
        word = mini_itr->first;                                             // make word a random choice
    }
    
    else if (!random_flag) {                                                // This selects the most frequent
                                                                            // subsquent word from
        MiniMap::iterator mini_itr = (MainMap_itr->second).begin();         // the main map (word of interest).
        
        for (; mini_itr != (MainMap_itr->second).end(); mini_itr++) {
            if (mini_itr->second > max) {
                max = mini_itr->second;
                word = mini_itr->first;
            }
        }
    }
}

// THIS IS THE SAME AS THE ABOVE FUNCTION BUT CHECKS TWO SPOTS AHEAD INSTEAD OF JUST ONE
void FindNextWord3(MegaMap &data, bool random_flag, std::vector<std::string> &words, std::vector<std::string> &output) {
    
    MegaMap::iterator Mega_itr = data.find(words[0]);
    if (Mega_itr == data.end()) {return;}
    
    MY_MAP_typedef::iterator MainMap_itr = Mega_itr->second.find(words[1]);
    if (MainMap_itr == Mega_itr->second.end()) {return;}
    
    if (output.size() == 0) {
        output.push_back(Mega_itr->first);
    }
    
    output.push_back(MainMap_itr->first);                   // if found, push_back()
    
    
    int max = 0;
    if (random_flag) {
        MTRand mtrand;
        int rand_index = (int)mtrand.randInt(MainMap_itr->second.size()-1);
        
        MiniMap::iterator mini_itr = MainMap_itr->second.begin();
        std::advance(mini_itr, rand_index);
        
        words[0] = MainMap_itr->first;                                      // change words in vector accordingly
        words[1] = mini_itr->first;
    }
    
    else if (!random_flag) {                                                // this selects the most frequent subsquent word from
        MiniMap::iterator mini_itr = (MainMap_itr->second).begin();         // the main map (word of interest)
        
        for (; mini_itr != (MainMap_itr->second).end(); mini_itr++) {
            if (mini_itr->second > max) {
                max = mini_itr->second;
                words[0] = MainMap_itr->first;
                words[1] = mini_itr->first;
            }
        }
    }
}

int main () {

  // ASSIGNMENT: THE MAIN DATA STRUCTUR
    MY_MAP_typedef data2;
    MegaMap data3;
    std::vector<std::string> vecText_kinda_global;                              // vector used for check last word in text when

    
  // Parse each command
  std::string command;
  int window;
  while (std::cin >> command) {

    // load the sample text file
    if (command == "load") {
      std::string filename;
      std::string parse_method;
      std::cin >> filename >> window >> parse_method;
        
////////////////////////////////////////////////////////////////////////////////
                                                                     //MY CODE//
        
        assert(parse_method == "ignore_punctuation");
        LoadSampleText2(data2, filename, 2, parse_method, vecText_kinda_global);// load the text into two different datas so they can be
                                                                                // used independently
        assert(parse_method == "ignore_punctuation");
        LoadSampleText3(data3, filename, 3, parse_method);
        
        std::cout << "Loaded " << filename << " with window = " << window << " and parse method = ignore_punctuation" << std::endl;
        std::cout << std::endl;
        
                                                                     //MY CODE//
////////////////////////////////////////////////////////////////////////////////
    }
      
    // print the portion of the map structure with the choices for the
    // next word given a particular sequence.
    else if (command == "print") {
        std::vector<std::string> sentence = ReadQuotedWords(std::cin);
        
        
////////////////////////////////////////////////////////////////////////////////
                                                                     //MY CODE//
        
        if (sentence.size() == 1) {

            MY_MAP_typedef::iterator MY_MAP_itr = data2.find(sentence[0]);       // point iterator to main word of interest
            std::string main_word = MY_MAP_itr->first;                          // assign string to word of interest
            
            int num_next_words = 0;
            MiniMap::iterator mini_itr;
            for (mini_itr = (MY_MAP_itr->second).begin()                        // loop through word pairs
                ;mini_itr != (MY_MAP_itr->second).end(); mini_itr++) {          // to find out the number of words
                
                num_next_words += mini_itr->second;
            }
            if (sentence[0] == vecText_kinda_global[vecText_kinda_global.size()-1]) { // add 1 to word if its last in .txt
                num_next_words++;
            }
            PrintWordCounts(main_word, num_next_words);                         // print out word and count
            
            
            for (mini_itr = (MY_MAP_itr->second).begin()                        // do the same process as above but instead
                 ;mini_itr != (MY_MAP_itr->second).end(); mini_itr++) {         // print out the second word and how many times the succession it appears
                
                std::cout << main_word << " ";
                PrintWordCounts(mini_itr->first, mini_itr->second);
            }
            std::cout << std::endl;

        }
        else if (sentence.size() == 2) {                                        // SAME AS ABOVE BUT FOR TWO WORDS
            
            MegaMap::iterator Mega_itr = data3.find(sentence[0]);
            MY_MAP_typedef::iterator MY_MAP_itr = Mega_itr->second.find(sentence[1]);
            std::string main_words = Mega_itr->first + " " + MY_MAP_itr->first;
            
            int num_next_words = 0;
            MiniMap::iterator mini_itr;
            for (mini_itr = (MY_MAP_itr->second).begin()
                 ;mini_itr != (MY_MAP_itr->second).end(); mini_itr++) {
                
                num_next_words += mini_itr->second;
            }
            PrintWordCounts(main_words, num_next_words);

            
            for (mini_itr = (MY_MAP_itr->second).begin()
                 ;mini_itr != (MY_MAP_itr->second).end(); mini_itr++) {
                
                std::cout << main_words << " ";
                PrintWordCounts(mini_itr->first, mini_itr->second);
            }
            std::cout << std::endl;
        }
        else {
            std::cout << "\n";

            return 0; }
                                                                    //MY CODE//
////////////////////////////////////////////////////////////////////////////////

    }

    // generate the specified number of words
    else if (command == "generate") {
        std::vector<std::string> sentence = ReadQuotedWords(std::cin);
        // how many additional words to generate
        int length;
        std::cin >> length;
        std::string selection_method;
        std::cin >> selection_method;
        bool random_flag;
        
        if (selection_method == "random") {
            random_flag = true;
        }
        else {
            assert(selection_method == "most_common");
            random_flag = false;
        }
        
        
////////////////////////////////////////////////////////////////////////////////
                                                                     //MY CODE//
        
        
        
        std::vector<std::string> output;

       
        if (window == 2) {                                                      // for a window size of just two
            std::string word = sentence[0];                                     // assign word to first word (only word) in sentence
            
            for (int i = 0; i < length+1; i++) {
                FindNextWord2(data2, random_flag, word, output);                // this function will append chained words to 'output'
            }
        }
        else if (window == 3) {                                                 // for a window size of just three
            
            if (sentence.size() == 2) {                                         // two words
                std::vector<std::string> words;
                words.push_back(sentence[0]);                                   // add first two words to start process
                words.push_back(sentence[1]);
                std::string word = sentence[0];
                
                for (int i = 0; i < length+1; i++) {
                    FindNextWord3(data3, random_flag, words, output);
                }
            }
            
            /* work on this */
            else if (sentence.size() == 1) {                                    // input of one word
                
                std::string word = sentence[0];                                 // assign starting word
                
                FindNextWord2(data2, random_flag, word, output);
            
                std::vector<std::string> words;
                words.push_back(sentence[0]);                                   // do the same as above but with an extra word at the beginning
                words.push_back(word);
                
                for (int i = 0; i < length+1; i++) {
                    FindNextWord3(data3, random_flag, words, output);
                }
                output.pop_back();                                              // delete the extra word at the end
            }
        }
    
        
        for (int i = 0; i < (int)output.size(); i++) {                          // print out whatever is in output
            if (i == (int)output.size() - 1) {
                std::cout << output[i];
            }
            else
                std::cout << output[i] << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;

        output.clear();
        
        
        
                                                                     //MY CODE//
////////////////////////////////////////////////////////////////////////////////


    } else if (command == "quit") {
      break;
    } else {
      std::cout << "WARNING: Unknown command: " << command << std::endl;
    }
  }
}
