HOMEWORK 7: WORD FREQUENCY MAPS


NAME:  Adam Lawrence


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

Josh Goldberg
Stack about cstdlib

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  6



ANALYSIS OF PERFORMANCE OF YOUR ALGORITHM:
(order notation & concise paragraph, < 200 words)

Order notation is based on which kinds of operations are completed in a specific program or function.
In this homework, there are three main functions: load, print, and generate.
Load reads through all the words in the text tile, and also adds them, at a certain depth (w) to a map. Adding words to a map is log. So loading a map depends on how many words there are and how deep the map is.
Print uses find() which is log, however you also print out p times the number of words in a specific part of the map.
Generate revolves around the find function and accessing the words within the value of a certain key. It also depends on the window - how far ahead we want to look to get to necessary words. Lastly, all this is done a number of times dependent on how long we want our sentence to be (which I call ‘k’)

n = total number of words in the sample text file
m = number of unique words in the file
w = width of the sequencing window
p = average number of words observed to follow a particular word
k = length of sentence generated


How much memory will the map data structure require, in terms of n, m,
w, and p (order notation for memory use)?

O( (m*p)^w-1 ) - each beginning word is followed by an avg of p words

What is the order notation for performance (running time) of each of
the commands?

load: O( n*wlogn ) go through all words, for each word find and insert
print: O( logm + p) find word, print out all following words
generate: O( k*(w*(logm + p)) )



MISC. COMMENTS TO GRADER:  
(optional, please be concise!)


