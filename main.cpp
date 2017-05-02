#include <iostream>
#include <cstring>
#include <list>
#include <vector>
#include <thread>
#include <math.h>

using namespace std;

// this is the basic structure, Parent and LevelDeep are needed in order
// to go back and print the solution. They are not needed otherwise.
struct word_in_tree {
    string word;
    list<word_in_tree *> close_words;
    word_in_tree *parent;
    int leveldeep;

    word_in_tree (string s) {
        word = s;
        close_words.clear();
        parent=NULL;
        leveldeep=0;
    }
};

// Checks whether words are one letter apart
// We assume all words are of same length.
bool close_words(string s1, string s2)
{
    int no_diff_let=0;

    for (int i=0; i<s1.length(); i++)
    {
        if (s1[i]!=s2[i])
            no_diff_let++;
    }
    if (no_diff_let==1)
        return true;
    else
        return false;
}

// The thread loop.
// At least two alternatives were considered. One would add the changes of the dictionary in the thread funtion,
// and the other without it.
// In order to avoid the need of Locks I decided to limit the threads to the search of the close words only
void dictloop (int start_place, int end_place, vector<string> dict, vector<bool>& places, string word)
{
    for (int i=start_place; i<=end_place; i++)
    {
        if (close_words(dict[i],word))
        {
           places[i]=true;
        }
    } 
}

// The main function
// It builds and runs/delete a tree in paralel. neighbours is the queue that contains the word
// and its neighbours, starting with the start-word, adding its neighbours, checking are we done,
// and if not deleting the first word and continueing adding the neighbours of the second one.
// It is similar (if not identical) to Breadth first search (https://en.wikipedia.org/wiki/Breadth-first_search)
// but without the weights. Also, since we want to 'keep' the path back, the 'parent' is kept and the level of 'depth'.
// How do we know we have found the shortest path?! simply since we are advancing layer by layer, so if additional
// paths exist they must be of same or longer length.
// The code is not that 'elegant'. I believe lines 104,106 (the way I break the threads) and
// lines 124-130 could/should be written must nicely.

void build_path_thread(string start_word, string end_word, vector<string> dict)
{
    list<word_in_tree *> neighbours;
    word_in_tree *first_word_in_tree = new word_in_tree(start_word);
    neighbours.push_back(first_word_in_tree);
    word_in_tree *curr;

    cout << "starting with "<< start_word << "\n";
    cout << "looking for " << end_word << "\n";
    unsigned num_threads = thread::hardware_concurrency();
    std::thread t[num_threads];

    vector<bool> places_in_dict;



    while (!neighbours.empty())
    {
        places_in_dict.clear();
        for (int i=0; i< dict.size(); i++)
            places_in_dict.push_back(false);

        curr = neighbours.front();
        neighbours.pop_front(); // deleting the word we're working on from neighbours

        //checkign if we're done and printing
        if (curr->word==end_word)
        {
            cout << "success\n";
            while (curr->leveldeep) {
                cout << curr->word << " \n";
                curr = curr->parent;
            }
            cout << start_word << "\n";
            return;
        }

        float interval = (float)(dict.size())/num_threads;
        for (int i=0;i<num_threads;i++){
            t[i] = std::thread(dictloop, floor(i*interval), floor((i+1)*interval)-1, dict, ref(places_in_dict), curr->word);
        }
        for (int i=0;i<num_threads;i++){
            t[i].join();
        }

        for (int i=0; i<dict.size(); i++)
        {
            if (places_in_dict[i])
            {
                word_in_tree *new_word_in_tree = new word_in_tree(dict[i]);
                new_word_in_tree->parent = curr;
                new_word_in_tree->leveldeep = curr->leveldeep + 1;
                neighbours.push_back(new_word_in_tree);
                curr->close_words.push_back(new_word_in_tree);
            }
        }
        int j=dict.size();
        int counter = 0;
        for (int i=0; i<j-counter; i++) {
            if (places_in_dict[i]) {
              dict.erase(dict.begin()+i-counter);
              counter++;
            }
        }

        if (dict.size() == 0) {
            cout << "not path found\n";
            continue;
        }
    } // while loop
    cout << "no path found\n";
}


// filling the dictionary and adding the start/end to the dictionary
void fill_dict(vector<string> *dict, vector<string> *args)
{
    args->push_back("smart");
    args->push_back("brain");
    dict->push_back(args->front());
    dict->push_back(args->back());
    dict->push_back("aaaaa");
    dict->push_back("abcde");
    dict->push_back("smart");
    dict->push_back("brain");
    dict->push_back("start");
    dict->push_back("stark");
    dict->push_back("stack");
    dict->push_back("kjjhf");
    dict->push_back("slack");
    dict->push_back("black");
    dict->push_back("blank");
    dict->push_back("bland");
    dict->push_back("brand");
    dict->push_back("braid");
}

void fill_dict2(vector<string> *dict, vector<string> *args)
{
    args->push_back("jjj");
    args->push_back("kkk");
    dict->push_back(args->front());
    dict->push_back(args->back());
    dict->push_back("aam");
    dict->push_back("aae");
    dict->push_back("abe");
    dict->push_back("abn");
    dict->push_back("mna");
    dict->push_back("kkj");
    dict->push_back("kkj");
    dict->push_back("jkj");
}

void fill_dict3(vector<string> *dict, vector<string> *args)
{
    args->push_back("ae");
    args->push_back("fa");
    dict->push_back(args->front());
    dict->push_back(args->back());
    dict->push_back("fe");
    dict->push_back("ab");
    dict->push_back("ba");
    dict->push_back("da");
}

int main() {
    vector<string> dict;
    vector<string> args;

    fill_dict(&dict, &args);
    build_path_thread(args[0], args[1], dict);

    return 0;
}
