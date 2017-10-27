// zombies.cpp
// code for Project 7

#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cctype>
#include <ctime>
#include <cassert>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;              // max number of rows in the arena
const int MAXCOLS = 20;              // max number of columns in the arena
const int MAXZOMBIES = 100;          // max number of zombies allowed

const int NORTH = 0;
const int EAST  = 1;
const int SOUTH = 2;
const int WEST  = 3;
const int NUMDIRS = 4;

const int EMPTY      = 0;
const int HAS_BRAIN  = 1;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
              // type name, since it's mentioned in the Zombie declaration.

class Zombie
{
  public:
      // Constructor
    Zombie(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    void move();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
	bool   m_dead;
	bool   m_poisoned;
	int    m_poison;
	int    m_dir;
};

class Player
{
  public:
      // Constructor
    Player(Arena *ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    string dropBrain();
    string move(int dir);
    void   setDead();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool    m_dead;
};

class Arena
{
  public:
      // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();

      // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     zombieCount() const;
    int     getCellStatus(int r, int c) const;
    int     numberOfZombiesAt(int r, int c) const;
    void    display(string msg) const;

      // Mutators
    void setCellStatus(int r, int c, int status);
    bool addZombie(int r, int c);
    bool addPlayer(int r, int c);
    void moveZombies();

  private:
    int     m_grid[MAXROWS][MAXCOLS];
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Zombie* m_zombies[MAXZOMBIES];
    int     m_nZombies;
    int     m_turns;

      // Helper functions
    void checkPos(int r, int c) const;
};

class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nZombies);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;

      // Helper functions
    string takePlayerTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int lowest, int highest);
bool charToDir(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
void clearScreen();
bool checkPos(const Arena& a, int r, int c);
int checkSurroundings (const Arena& a, int r, int c);

///////////////////////////////////////////////////////////////////////////
//  Zombie implementation
///////////////////////////////////////////////////////////////////////////

Zombie::Zombie(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** A zombie must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Zombie created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
	m_dead = false;
	m_poisoned = false;
	m_poison = 0;
}

int Zombie::row() const
{
    return m_row;
}

int Zombie::col() const
{
    return m_col;
}

bool Zombie::isDead() const
{
      if (m_dead) return true;
	  else return false;
}

void Zombie::move()
{
	if (m_poisoned)
	{
		if (m_poison % 2 == 0)
		{
			m_poison++;
			return;
		}
		else m_poison++;
	}

	m_dir = randInt(NORTH, WEST);

	if(attemptMove(*m_arena, m_dir, m_row, m_col))
	{
		if (m_arena->player()->col() == m_col && m_arena->player()->row() == m_row)
				m_arena->player()->setDead();
		else if (m_arena->getCellStatus(m_row, m_col) == HAS_BRAIN)
		{
			if (m_poisoned)
				m_dead = true;
			m_poisoned = true;
			m_arena->setCellStatus(m_row, m_col, EMPTY);
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

string Player::dropBrain()
{
    if (m_arena->getCellStatus(m_row, m_col) == HAS_BRAIN)
        return "There's already a brain at this spot.";
    m_arena->setCellStatus(m_row, m_col, HAS_BRAIN);
    return "A brain has been dropped.";
}

string Player::move(int dir)
{
	if(attemptMove(*m_arena, dir, m_row, m_col))
	{
		if (m_arena->numberOfZombiesAt(m_row, m_col) > 0)
		{
			return "Player walked into a zombie and died.";
			setDead();
		}
		else return "Player moved north.";
	}
	else return "Player couldn't move; player stands.";
}

bool Player::isDead() const
{
      if(m_dead) return true;
	  else return false;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
             << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nZombies = 0;
    m_turns = 0;
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            setCellStatus(r, c, EMPTY);
}

Arena::~Arena()
{
    delete m_player;
	for (int k = 0; k < m_nZombies; k++)
		delete m_zombies[k];
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::zombieCount() const
{
    return m_nZombies;
}

int Arena::getCellStatus(int r, int c) const
{
    checkPos(r, c);
    return m_grid[r-1][c-1];
}

int Arena::numberOfZombiesAt(int r, int c) const
{
	int m_count = 0;

	for(int k = 0; k != m_nZombies; k++)
	{
		if (m_zombies[k]->col() == c && m_zombies[k]->row() == r)
			m_count++;
	}
    
	return m_count;
}

void Arena::display(string msg) const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    
      // Fill displayGrid with dots (empty) and stars (brains)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r-1][c-1] = (getCellStatus(r,c) == EMPTY ? '.' : '*');

    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
		{
			if (numberOfZombiesAt(r, c) == 1)
				displayGrid[r-1][c-1] = 'Z';
			else if (numberOfZombiesAt(r, c) > 1 && numberOfZombiesAt(r, c) < 9)
				displayGrid[r-1][c-1] = '0' + numberOfZombiesAt(r, c);
			else if (numberOfZombiesAt(r, c) >= 9)
				displayGrid[r-1][c-1] = '9';
		}

      // Indicate player's position
    if (m_player != nullptr)
        displayGrid[m_player->row()-1][m_player->col()-1] = (m_player->isDead() ? 'X' : '@');

      // Draw the grid
    clearScreen();
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
            cout << displayGrid[r-1][c-1];
        cout << endl;
    }
    cout << endl;

      // Write message, zombie, and player info
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << zombieCount() << " zombies remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player!" << endl;
    else if (m_player->isDead())
        cout << "The player is dead." << endl;
    cout << m_turns << " turns have been taken." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
    checkPos(r, c);
    m_grid[r-1][c-1] = status;
}

bool Arena::addZombie(int r, int c)
{
    if (m_nZombies == MAXZOMBIES)
        return false;
    m_zombies[m_nZombies] = new Zombie(this, r, c);
    m_nZombies++;
    return true;
}

bool Arena::addPlayer(int r, int c)
{
      // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

      // Dynamically alocate a new Player and add it to the arena
    m_player = new Player(this, r, c);
    return true;
}

void Arena::moveZombies()
{
	for(int k = 0; k != m_nZombies; k++)
		m_zombies[k]->move();

	for(int k = 0; k != m_nZombies; k++)
	{
		if (m_zombies[k]->col() == m_player->col() && m_zombies[k]->row() == m_player->row())
			m_player->setDead();
	}

	for(int k =0; k < m_nZombies; k++)
	{
		if (m_zombies[k]->isDead())
		{
			delete m_zombies[k];
			m_nZombies--;
			if (m_nZombies != 0)
				for(int i = k; i != m_nZombies; i++)
					m_zombies[i] = m_zombies[i + 1];
		}
	}

    m_turns++;
}

void Arena::checkPos(int r, int c) const
{
    if (r < 1  ||  r > m_rows  ||  c < 1  ||  c > m_cols)
    {
        cout << "***** " << "Invalid arena position (" << r << ","
             << c << ")" << endl;
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nZombies)
{
    if (nZombies < 0  ||  nZombies > MAXZOMBIES)
    {
        cout << "***** Game created with invalid number of zombies:  "
             << nZombies << endl;
        exit(1);
    }
    int nEmpty = rows * cols - nZombies - 1;  // 1 for Player
    if (nEmpty < 0)
    {
        cout << "***** Game created with a " << rows << " by "
             << cols << " arena, which is too small too hold a player and "
             << nZombies << " zombies!" << endl;
        exit(1);
    }

      // Create arena
    m_arena = new Arena(rows, cols);

      // Add player
    int rPlayer;
    int cPlayer;
    do
    {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
    m_arena->addPlayer(rPlayer, cPlayer);

      // Populate with zombies
    while (nZombies > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (r == rPlayer && c == cPlayer)
            continue;
        m_arena->addZombie(r, c);
        nZombies--;
    }
}

Game::~Game()
{
    delete m_arena;
}

string Game::takePlayerTurn()
{
    for (;;)
    {
        cout << "Your move (n/e/s/w/x or nothing): ";
        string playerMove;
        getline(cin, playerMove);

        Player* player = m_arena->player();
        int dir;

        if (playerMove.size() == 0)
        {
            if (recommendMove(*m_arena, player->row(), player->col(), dir))
                return player->move(dir);
            else
                return player->dropBrain();
        }
        else if (playerMove.size() == 1)
        {
            if (tolower(playerMove[0]) == 'x')
                return player->dropBrain();
            else if (charToDir(playerMove[0], dir))
                return player->move(dir);
        }
        cout << "Player move must be nothing, or 1 character n/e/s/w/x." << endl;
    }
}

void Game::play()
{
    m_arena->display("");
    while ( ! m_arena->player()->isDead()  &&  m_arena->zombieCount() > 0)
    {
        string msg = takePlayerTurn();
        Player* player = m_arena->player();
        if (player->isDead())
            break;
        m_arena->moveZombies();
        m_arena->display(msg);
    }
    if (m_arena->player()->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

  // Return a uniformly distributed random int from lowest to highest, inclusive
int randInt(int lowest, int highest)
{
    if (highest < lowest)
        swap(highest, lowest);
    return lowest + (rand() % (highest - lowest + 1));
}

bool charToDir(char ch, int& dir)
{
    switch (tolower(ch))
    {
      default:  return false;
      case 'n': dir = NORTH; break;
      case 'e': dir = EAST;  break;
      case 's': dir = SOUTH; break;
      case 'w': dir = WEST;  break;
    }
    return true;
}

  // Return false without changing anything if moving one step from (r,c)
  // in the indicated direction would run off the edge of the arena.
  // Otherwise, update r and c to the position resulting from the move and
  // return true.
bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
	if (dir == NORTH)
	{
		if (r - 1 < 1)
			return false;
		else 
		{
			r--;
			return true;
		}
	}

	if (dir == SOUTH)
	{
		if (r + 1 > a.rows())
			return false;
		else 
		{
			r++;
			return true;
		}
	}

	if (dir == EAST)
	{
		if (c + 1 > a.cols())
			return false;
		else 
		{
			c++;
			return true;
		}
	}

	if (dir == WEST)
	{
		if (c - 1 < 1)
			return false;
		else 
		{
			c--;
			return true;
		}
	}
}

  // Recommend a move for a player at (r,c):  A false return means the
  // recommendation is that the player should drop a brain and not move;
  // otherwise, this function sets bestDir to the recommended direction
  // to move and returns true.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
	int stay = checkSurroundings (a, r, c);
	int n = 0;
	int s = 0;
	int e = 0;
	int w = 0;
	int safest = 10000;

	if (!checkPos(a, r-1, c))
		n = -1;
	else if (a.numberOfZombiesAt(r-1, c) > 0)
		n = -1;

	if (!checkPos(a, r+1, c))
		s = -1;
	else if (a.numberOfZombiesAt(r+1, c) > 0)
		s = -1;

	if (!checkPos(a, r, c+1))
		e = -1;
	else if (a.numberOfZombiesAt(r, c+1) > 0)
		e = -1;

	if (!checkPos(a, r, c-1))
		w = -1;
	else if (a.numberOfZombiesAt(r, c-1) > 0)
		w = -1;

	if (n != -1)
		n = checkSurroundings(a, r-1, c);

	if (s != -1)
		s = checkSurroundings(a, r+1, c);

	if (e != -1)
		e = checkSurroundings(a, r, c+1);

	if (w != -1)
		w = checkSurroundings(a, r, c-1);

	if (n != -1 && n < safest)
	{
		safest = n;
		bestDir = NORTH;
	}

	if (s != -1 && s < safest)
	{
		safest = s;
		bestDir = SOUTH;
	}

	if (e != -1 && e < safest)
	{
		safest = e;
		bestDir = EAST;
	}

	if (w != -1 && w < safest)
	{
		safest = w;
		bestDir = WEST;
	}

	if (stay <= safest)
		safest = 10000;

	if (safest == 10000)
		return false;

	return true;
}

int checkSurroundings (const Arena& a, int r, int c)
{
	int counter = 0;
	
	if (checkPos(a, r+1, c))
	{
		if (a.numberOfZombiesAt(r+1, c) > 0)
			counter = counter + a.numberOfZombiesAt(r+1, c);
	}

	if (checkPos(a, r-1, c))
	{
		if (a.numberOfZombiesAt(r-1, c) > 0)
			counter = counter + a.numberOfZombiesAt(r-1, c);
	}

	if (checkPos(a, r, c+1))
	{
		if (a.numberOfZombiesAt(r, c+1) > 0)
			counter = counter + a.numberOfZombiesAt(r, c+1);
	}

	if (checkPos(a, r, c-1))
	{
		if (a.numberOfZombiesAt(r, c-1) > 0)
			counter = counter + a.numberOfZombiesAt(r, c-1);
	}

	return counter;
}

bool checkPos(const Arena& a, int r, int c)
{
    if (r < 1  ||  r > a.rows()  ||  c < 1  ||  c > a.cols())
		return false;
	else
		return true;
}

// DO NOT MODIFY THE CODE BETWEEN HERE AND THE MAIN ROUTINE
#ifdef _MSC_VER  //  Microsoft Visual C++

#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                                                        &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <cstring>

void clearScreen()
{
    static const char* term = getenv("TERM");
    static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
     else
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
}

#endif

///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

int main()
{
      // Initialize the random number generator
    srand(static_cast<unsigned int>(time(0)));

      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(10, 12, 40);

      // Play the game
    g.play();
}