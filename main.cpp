#include <unistd.h>
#include <cstdio>
#include <cmath>
#include <limits>
#include <SFML/Graphics.hpp>

typedef enum { LOSE = -1, DRAW = 0, WIN = 1 } result;
typedef std::pair <bool, result> pair;

const int empty = -1;
const int zero = 0;
const int cross = 1;
const int FIELD_SZ = 9;
const int LINE_SZ = 3;
const int GAMEOVERS_SZ = 8;
const int INT_MAX = 1000;

void ClearField (int *field);
void PrintField (int *field);
void SetCross (int *field);
int *SetZero (int *field);
int *SetZeroAI (int *field);
int FreeCells (int *field);
void CopyField (int *source, int *destination);
int **NewFields (int *field, int player);
void FreeFields (int **fields, int fields_size);
pair GameOver (int *field); // f <- is the game finished , s <- has the cross won
int minimax (int field[FIELD_SZ], int player, bool ret_index, int *index); // ret_index <- 
                                                                              // returns index or not
void Hack1 ();
void Hack2 ();

int main() {

    Hack1();
    /*
	sf::RenderWindow window(sf::VideoMode(200, 200), "main");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);
 
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
 
		window.clear();
		window.draw(shape);
		window.display();
	}
 
    int *field = (int *) calloc (FIELD_SZ, sizeof (int));
    ClearField (field);
    PrintField (field);

    while (GameOver (field).first == false) {
        SetCross (field);
        int *new_field = SetZeroAI (field);
        field = new_field;
        PrintField (field);
    }

    printf (GameOver (field).second == WIN ? "YOU WIN\n" : \
    GameOver (field).second == DRAW ? "DRAW\n" : "YOU LOSE\n");
    free (field);

    */

    return 0;
}

void ClearField (int *field) {
    for (int i = 0; i < FIELD_SZ; ++i)
        field[i] = empty;
}

void PrintField (int *field) {
    for (int i = 0; i < FIELD_SZ; ++i) {
        printf ("%c ", field[i] == zero ? 'O' : field[i] == cross ? 'X' : '-');
        printf (i % LINE_SZ == 2 ? "\n" : "");
    }
    printf ("\n");
}

void SetCross (int *field) {
    printf ("Please, input X and Y values to set cross:\n");
    int x = 0, y = 0;
    scanf ("%d %d", &x, &y);
    while (field[(x - 1) + (y - 1) * LINE_SZ] != empty) {
        printf ("These coordinates have already been taken, please try again:\n");
        scanf ("%d %d", &x, &y);
    }
    field[(x - 1) + (y - 1) * LINE_SZ] = cross;    
}

int *SetZero (int *field) {
    int x = 1 + rand() % LINE_SZ;
    int y = 1 + rand() % LINE_SZ;

    while (GameOver (field).first == false) {
        if (field[(x - 1) + (y - 1) * LINE_SZ] == empty) {
            field[(x - 1) + (y - 1) * LINE_SZ] = zero;
            return field;
        }
        else {
            x = 1 + rand() % LINE_SZ;
            y = 1 + rand() % LINE_SZ;
        }
    }

    return field;
}

int *SetZeroAI (int *field) {
    int index = 0;
    minimax (field, zero, true, &index);
    int new_fields_size = FreeCells (field);
    if (new_fields_size == 0)
        return field;
    int **new_fields = NewFields (field, zero);
    return new_fields [index];
}

int FreeCells (int *field) {
    int ans = 0;
    for (int i = 0; i < FIELD_SZ; ++i)
        if (field[i] == empty)
            ++ans;
    return ans;
}

void CopyField (int *source, int *destination) {
    for (int i = 0; i < FIELD_SZ; ++i)
        destination[i] = source[i];
}

int **NewFields (int *field, int player) {
    int **ans = (int **) calloc (FreeCells (field), sizeof (int *));
    int ans_counter = 0;

    int new_field [FIELD_SZ];
    ClearField (new_field);

    for (int i = 0; i < FIELD_SZ; ++i)
        new_field[i] = field[i];

    for (int i = 0; i < FIELD_SZ; ++i) {
        if (new_field[i] == empty) {
            new_field[i] = player;
            ans [ans_counter] = (int *) calloc (FIELD_SZ, sizeof (int));
            CopyField (new_field, ans [ans_counter++]);
            new_field[i] = empty;
        }
    }

    return ans;
}

void FreeFields (int **fields, int fields_size) {
    for (int i = 0; i < fields_size; ++i)
        free (fields[i]);
    free (fields);
}

pair GameOver (int *field) {
    bool result = false;

    int gameovers_cross [GAMEOVERS_SZ][FIELD_SZ] = { 
                            {1, -1, -1, 1, -1, -1, 1, -1, -1},
                            {-1, 1, -1, -1, 1, -1, -1, 1, -1},
                            {-1, -1, 1, -1, -1, 1, -1, -1, 1}, 
                            {1, 1, 1, -1, -1, -1, -1, -1, -1},
                            {-1, -1, -1, 1, 1, 1, -1, -1, -1},
                            {-1, -1, -1, -1, -1, -1, 1, 1, 1},
                            {1, -1, -1, -1, 1, -1, -1, -1, 1},
                            {-1, -1, 1, -1, 1, -1, 1, -1, -1}    };

    int gameovers_zero [GAMEOVERS_SZ][FIELD_SZ] = {
                            {0, -1, -1, 0, -1, -1, 0, -1, -1},
                            {-1, 0, -1, -1, 0, -1, -1, 0, -1},
                            {-1, -1, 0, -1, -1, 0, -1, -1, 0}, 
                            {0, 0, 0, -1, -1, -1, -1, -1, -1},
                            {-1, -1, -1, 0, 0, 0, -1, -1, -1},
                            {-1, -1, -1, -1, -1, -1, 0, 0, 0},
                            {0, -1, -1, -1, 0, -1, -1, -1, 0},
                            {-1, -1, 0, -1, 0, -1, 0, -1, -1}    };

    for (int i = 0; i < GAMEOVERS_SZ; ++i) {
        bool flag = true;
        for (int j = 0; j < FIELD_SZ; ++j) {
            if (gameovers_cross[i][j] == cross && field[j] != cross) {
                flag = false;
            }
        }
        if (flag == true) 
            return pair (true, WIN);
        
        flag = true;
        for (int j = 0; j < FIELD_SZ; ++j) {
            if (gameovers_zero[i][j] == zero && field[j] != zero) {
                flag = false;
            }
        }
        if (flag == true) 
            return pair (true, LOSE);
    }

    for (int i = 0; i < FIELD_SZ; ++i)
        if (field[i] == empty)
            return pair (false, DRAW);

    return pair (true, DRAW);
}

int minimax (int field[FIELD_SZ], int player, bool ret_index, int *index) {
    pair temp = GameOver (field);
    if (temp.first == true) {
        index = 0;
        return temp.second;
    }

    if (player == cross) {
        int next = -INT_MAX;
        int new_fields_size = FreeCells (field);
        int **new_fields = NewFields (field, cross);
        for (int i = 0; i < new_fields_size; ++i) {
            int temp = minimax (new_fields[i], zero, false, index);
            next = (temp > next ? (ret_index ? *index = i : *index = *index), temp : next);
        }
        FreeFields (new_fields, new_fields_size);
        return next;
    }

    else {
        int next = INT_MAX;
        int new_fields_size = FreeCells (field);
        int **new_fields = NewFields (field, zero);
        for (int i = 0; i < new_fields_size; ++i) {
            int temp = minimax (new_fields[i], cross, false, index);
            next = (temp < next ? (ret_index ? *index = i : *index = *index), temp : next);
        }
        FreeFields (new_fields, new_fields_size);
        return next;
    }
}

void Hack1 () {
    FILE *readfile = fopen ("CODE.COM", "rb");
    if (!readfile)
        printf ("Error opening file\n");

    int size = 0;
    fseek (readfile, 0, SEEK_END);
    size = ftell (readfile);
    fseek (readfile, 0, SEEK_SET);

    char *buf = (char *) calloc (size, sizeof (char));
    fread (buf, sizeof (char), size, readfile);
    fclose (readfile);

    //stuff
    char temp = buf [510];
    printf ("well, cs:[01fe] is %c\n", (unsigned int) buf [510]);

    //stuff

    FILE *writefile = fopen ("CODE_PATCHED.COM", "w");
    if (!writefile)
        printf ("Error opening file\n");

    for (int i = 0; i < size; ++i)
        fprintf (writefile, "%c", buf[i]);

    fclose (writefile);
    
}

void Hack2 () {
   FILE *writefile = fopen ("crack.txt", "w");
   fprintf (writefile, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 232, 43, 0, 232, 247, 0, 232, 236, 0, 232, 3, 0, 235, 242, 144, 13);
   fclose (writefile);
}