It is obvious that running over the word, changing letters, and checking against the dictionary is the most trivial but not optimal solution (number_of_letters^letters_in_word in our case). Retrieving from dictionary and checking against the current word is much faster (< number_of_words_in_dict^2). And this becomes basically a shortest path in tree search.

word_in_tree is the basic tree structure with a parent and LevelDeep which are needed only in order to walk back and print the solution. They are not needed otherwise.

The main function build_path_thread is a simple implementation of breadth first search but without weights. Since we want to keep the path back, the 'parent' is kept and the level of depth. 

It builds and runs/delete a tree in paralel. neighbours is the queue that contains the word and its neighbours, starting with the start-word, adding its neighbours, checking if are we done, and if not deleting the first word and continueing adding the neighbours of the second one.
How do we know we have found the shortest path?! simply since we are advancing layer by layer, so if additional paths exist they must be of same or longer length.
The code is not that 'elegant'. I believe lines 104,106 (the way I break the threads) and lines 124-130 could/should be written must nicely.

The threads I implemented after implementing the function without threads. This resulted in the less elegant parts mentioned above (we have to be more careful about deleting). I hesitated between two threading alternatives: One would add the changes of the dictionary in the thread funtion including the deletion from the dictionary but will demands 'locks' and another one with less functionality in the thread but without locks. I decided to do the second but am not sure which is better.
The tests I ran (with many variations during dev):
start: "smart", end with "brain" with
    "aaaaa"
    "abcde"
    "smart"
    "brain"
    "start"
    "stark"
    "stack"
    "kjjhf"
    "slack"
    "black"
    "blank"
    "bland"
    "brand"
    "braid"

start: "jjj" end "kkk" (and many variations below):
    "aam"
    "aae"
    "abe"
    "abn"
    "mna"
    "kkj"
    "kkj"
    "jkj"

start: "ae", end: "fa"
    "fe"
    "ab"
    "ba"
    "da"
    
