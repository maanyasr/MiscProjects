## Relevance Based Search of Aesops Fables Using a Suffix Tree

The Program Implements Ukkonen's algorithm to create a Generalized Suffix Tree.
The Tree is used to search for strings in each of the tales and returns the most relevant tales/documents.

The Program reads the dataset from "AesopTales.txt" (Included in Repository)

To compile the code,
```g++ AA_A3_01FB16ECS450.cpp -Wall -g -o Test.out``` 

To Run the code,
```./Test.out < InputFile.txt```

Format of Query File: 
The First line containing the string to be queried.
The Query strings can have whitespaces but No New Line Characters, "Hello World" is a Valid Query.

Sample Input and Output files have been included.

The relevance criteria considered are number of occurrences, whether the string/word occurs in the title, longest common word (Not Subtring, ie, If the query string is 'Hello World', then the program searches for 'Hello World', 'Hello' and 'World')

The Tree formed for every document also has the Titles in it, Hence while querying the tree the number of occurences are inclusive of the title as well and so is the First occurance of the string or a wordin the string. 

To sort the documents and display the most relevant ones, the following steps were followed:

* Relevance score for every document was initialised to 0.
* The Tree is first queried for the string (inclusive of Punctuations) and if found, The document's relevance score is incremented by TWICE the String length + 1.
* The string is then stripped of all Punctions and is searched for again. If found, The document's relevance score is incremented by TWICE the String length. 
* The string was split into individual words.
* Each time a word is found, The document's relevance score is incremented by the String length of that word. 
* The String is then searched for in the Title and if Found, The document's relevance score is incremented by HALF the String length. 

At the end of this process, a net relevance score was obtained for each document of the text. Based on this, the documents were sorted in order of relevance, and the ones with relevance score greater than 0 were displayed.
