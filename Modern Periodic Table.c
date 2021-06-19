//	When compiling on Linux, please add -lm as a compiler flag!
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct N {
	int z;				//	z = Atomic no.
	float a;			//	a = Atomic mass
	char sym[4];		//	Atomic symbol	[at most 3 letters in case of Uuo + 1 '\0']
	char name[21];		//	20 letters [+1 null] should be enough
	char descr[150];	//	150 characters should suffice
	struct N* next;
} Element;

//	Insertion
void insert (Element**);

//	Search functions
void nsearch (const Element*, const char*);
void ssearch (const Element*, const char*);
void zsearch (const Element*, const int);
void asearch (const Element*, const float);

//	Display details
void display (const Element*);

//	Destruction of list
void destroy (Element**);

int main() {
	Element* first = NULL;
	int ch, val;
	float fval;
	char str[21];

	while (1) {
		printf("Please choose from the following options:\n");
		printf("\t[0] Exit\n");
		printf("\t[1] Insert New Element\n");
		printf("\t[2] Search by Name\n");
		printf("\t[3] Search by Symbol\n");
		printf("\t[4] Search by Atomic Number\n");
		printf("\t[5] Search by Atomic Mass\n");

		scanf("%d", &ch);

		switch (ch) {
			case 0:
				destroy(&first);																//	To avoid memory leaks of any kind
				printf("Terminating program...\n");
				return 0;

			case 1:
				insert(&first);
				break;

			case 2:
				printf("Please enter the name of the element: ");
				scanf("%s", str);
				nsearch(first, str);
				break;

			case 3:
				printf("Please enter the symbol of the element: ");
				scanf("%s", str);
				ssearch(first, str);
				break;

			case 4:
				printf("Please enter the atomic number (Z) of the element: ");
				scanf("%d", &val);
				zsearch(first, val);
				break;

			case 5:
				printf("Please enter the atomic mass (A) of the element: ");
				scanf("%f", &fval);
				asearch(first, fval);
				break;
		}

		//	This program clears the screen b/w sessions
		//	We must wait for user input before doing anything of the sort
		printf("Please press any number and enter to continue...\n");
		scanf("%d", &ch);
		system("cls");														//	Use system("clear") on Linux instead of system("cls")
	}
}

//	Insert an element into the list. The periodic table should remain in sorted order.
void insert (Element** first) {
	Element* new = (Element*) malloc(sizeof(Element));

	//	Throw an error message if allocation fails
	if (!new) {
		printf("Error: Failed to allocate the memory.\n");
		return;
	}

	//	Get details of the new element
	printf("Please enter the details of the element:\n");
	printf("\tName:\t");
	scanf("%s", new -> name);								//	We can use scanf() since the element name is one word only
	printf("\tZ:\t");
	scanf("%d", &(new -> z));
	printf("\tA:\t");
	scanf("%f", &(new -> a));
	printf("\tSymbol:\t");
	scanf("%s", new -> sym);
	printf("\tDesc.:\t");
	fgets(new -> descr, 150, stdin);
	fgets(new -> descr, 150, stdin);

	//	If the list is empty, or the atomic number is less than that of the current first element, a simple front insertion will suffice.
	if ((!(*first)) || (((*first) -> z) > (new -> z))) {
		new -> next = *first;
		*first = new;
	}

	//	If there is only one Element, a rear insertion is still trivial to perform
	else if (!((*first) -> next)) {
		new -> next = NULL;
		(*first) -> next = new;
	}

	//	Otherwise, things become a bit complicated
	else {
		Element *i, *j;

		//	Traverse through the list - j is the Element after i.
		//	Termination condition 1 - (!j), i.e., end of list is reached
		//	Termination condition 2 - j -> z > new -> z, i.e., the insertion position is reached
		for (i = *first, j = i -> next; ((j) && ((j -> z) < (new -> z))); i = j, j = j -> next);

		//	Insert the new element between i and j - note that j may be null
		new -> next = j;
		i -> next = new;
	}

	printf("Element successfuly inserted.\n");
}

void nsearch (const Element* first, const char* nm) {
	Element* i;
	int found = 0;

	for (i = first; (i); i = i -> next) {
		if (!stricmp(i -> name, nm)) {
			found = 1;
			break;
		}
	}

	if (!found) {
		printf("Search failure.\n");
	}

	else {
		printf("Search successful.\n");
		printf("Details of the found element:\n");
		display(i);
	}
}

void ssearch (const Element* first, const char* sy) {
	Element* i;
	int found = 0;

	for (i = first; (i); i = i -> next) {
		if (!stricmp(i -> sym, sy)) {
			found = 1;
			break;
		}
	}

	if (!found) {
		printf("Search failure.\n");
	}

	else {
		printf("Search successful.\n");
		printf("Details of the found element:\n");
		display(i);
	}
}

void zsearch (const Element* first, const int z) {
	Element* i;
	int found = 0;

	for (i = first; (i); i = i -> next) {
		if (i -> z == z) {
			found = 1;
			break;
		}
	}

	if (!found) {
		printf("Search failure.\n");
	}

	else {
		printf("Search successful.\n");
		printf("Details of the found element:\n");
		display(i);
	}
}

void asearch (const Element* first, const float a) {
	Element* i;
	int found = 0;

	for (i = first; (i); i = i -> next) {
		//	It is dangerous to compare floating point numbers directly
		//	So instead we round them off
		//	This way we can also get oxygen (A = 16.0008) simply by searching for A = 16 and similar results
		if (round(i -> a) == round(a)) {
			found = 1;
			break;
		}
	}

	if (!found) {
		printf("Search failure.\n");
	}

	else {
		printf("Search successful.\n");
		printf("Details of the found element:\n");
		display(i);
	}
}

void display (const Element* e) {
	printf("Name:\t%s\n", e -> name);
	printf("Z:\t%d\n", e -> z);
	printf("A:\t%f\n", e -> a);
	printf("Symbol:\t%s\n", e -> sym);
	printf("Desc.:\t%s\n", e -> descr);
}

void destroy (Element** first) {
	Element* temp;

	while (*first) {
		temp = *first;
		*first = temp -> next;
		free(temp);
	}
}
