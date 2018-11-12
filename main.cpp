/*
Brian Pinson
U91813366
COP4600.002
Final Project

Computational Linguistics: FSM Lexicon

The program is designed to build a FSM Lexicon
and allow users construct their own words and see
how meaning is created by compounding morphemes

*/



#include <iostream>
#include <fstream>
#include <string>
#include <vector>




std::string word = "error"; // word created by user
std::string meaning = "error"; // meaning behind word
int initialState = 0; // sets q0 at location 0
void buildLexicon(); // builds lexicon
void setNext(); // finds next state in lexicon
void setMorphTransducer(); // sets the morpheme and transducer of all entries in the lexicon
void setInitial(); // sets initial state
void FSM(); // runs the FSM

struct morpheme // morpheme struct
{
	std::string inputName = "error";
	std::string morph = "error";
	std::string transducer = "error";
	std::vector<std::string> nextState;
	std::vector<int> next;
};

std::vector<morpheme> lexicon; // this is the list of morpheme


int main()
{


	std::cout << "--------------------------\n";
	std::cout << "Computational Linguistics: FSM Lexicon\n";
	std::cout << "By Brian Pinson\n";
	std::cout << "--------------------------\n\n";

	// reads in user's inputs and adds information to lexicon vector
	std::cout << "Input file name" << std::endl;
	buildLexicon(); // adds input text to lexicon vector
	setNext(); // adds in list of morphemes that a lexicon entry is connected to
	setMorphTransducer(); // sets morpheme and transducer for every lexicon entry
	while (1)
	{
		std::cout << "Input initial state or 'quit' to exit" << std::endl;
		setInitial(); //inputs initial state
		FSM();
		std::cout << "_________________________________" << std::endl;
	}

	return 0;
}


void buildLexicon()
{

	std::string morphs;
	std::getline(std::cin, morphs);
	std::fstream textfile;
	textfile.open(morphs);
	while (!textfile.is_open()) // won't stop until file is found
	{
		std::cout << "FILE NOT FOUND" << std::endl;
		std::getline(std::cin, morphs);
		textfile.open(morphs);
	}
	if (textfile.is_open()) //just a safety measure to make sure file is open
	{
		std::string line;

		for (int i = 0; std::getline(textfile, line) && !textfile.eof(); i++) // goes through ever line of the file
		{
			int lineSize = line.size();
			morpheme newMorph;
			lexicon.push_back(newMorph);
			int j = 0;
			int k = 0;
			for (; j < lineSize && line[j] != ' '; j++); // stops after morpheme name is found
			lexicon[i].inputName = line.substr(0, j);
			j++;
			for (k = j + 1; k < lineSize && line[k] != ' '; k++); // finds number of morphemes the morpheme is connected to
			int size = stoi(line.substr(j, k));
			j = k + 1;

			lexicon[i].nextState.resize(size);
			for (int n = 0; n < size; n++) // builds list of morphemes that the morpheme is connected to
			{
				for (k = j + 1; k < lineSize && line[k] != ' '; k++); // finds the beginning of the word
				lexicon[i].nextState[n] = line.substr(j, k - j);

				if (n == size - 1)
				{
					lexicon[i].nextState[n] = line.substr(j, k - j); // finds and extracts morpheme from string
				}
				j = k + 1;

			}

		}
		textfile.close();
	}
}


void setNext() // connects the child nodes to the node
{
	int lexiconSize = lexicon.size();
	for (int i = 0; i < lexiconSize; i++) // for every lexical entry
	{
		int size = lexicon[i].nextState.size();
		for (int j = 0; j < size; j++) // for all lexical entry child nodes
		{
			for (int k = 0; k < lexiconSize; k++) // finds the location of the child node
			{
				if (lexicon[i].nextState[j] == lexicon[k].inputName) // connects the location to the node
				{
					lexicon[i].next.push_back(k);
					break;
				}
			}
		}
	}
}

void setMorphTransducer() // needs to seperate transducer from morpheme
{
	int size = lexicon.size();
	for (int i = 0; i < size; i++) // for the entire string
	{
		std::string transduce, morph;
		std::string temp = lexicon[i].inputName;
		int StrSize = temp.size();
		int j = 0;
		for (j = 0; temp[j] != '.'; j++);
		lexicon[i].transducer = temp.substr(0, j); //split the string in half when the transducer.morpheme is found
		lexicon[i].morph = temp.substr(j + 1, temp.size()-2);
	}
}


void setInitial() // sets initial state
{
	std::string initial;
	while (1) // makes sure initial state exists
	{
		std::cout << "Input morpheme of initial state" << std::endl;
		std::cin >> initial;
		if (initial == "quit")
		{
			exit(1);
		}
		int lexiconSize = lexicon.size();

		for (int i = 0; i < lexiconSize; i++) // ensures final state exists
		{
			if (initial == lexicon[i].morph)
			{
				initialState = i;
				return;
			}
		}
		std::cout << "The initial state has not been found" << std::endl;
	}
}

void FSM()
{
	word = lexicon[initialState].morph; // sets the beginning of the string
	meaning = lexicon[initialState].transducer; // sets the beginning of the string
	int input = 0;

	int location = initialState; // sets our location to the initial state
	while (1)
	{

		int size = lexicon[location].next.size();
		if (size == 0) // no where for the FSM to go
		{
			std::cout << word << "\n" << meaning << std::endl;
			break; // FSM has reached the end;
		}
		std::cout << "Choose your next state" << std::endl;
		for (int i = 0; i < size; i++) // user is given a selection of next states to choose from
		{
			std::cout << i << " = " << lexicon[lexicon[location].next[i]].transducer << " -> " << lexicon[lexicon[location].next[i]].morph << std::endl;
		}
		std::cin >> input;
		input = input%size; // stops user from going outside array bounds
		location = lexicon[location].next[input];
		if (lexicon[input].morph == "end")
		{
			std::cout << word << "\n" << meaning << std::endl;
			break; // FSM has reached the end;
		}
		word += lexicon[location].morph;
		meaning += " " + lexicon[location].transducer;

	}
}