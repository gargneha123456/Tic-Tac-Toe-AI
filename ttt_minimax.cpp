#include <bits/stdc++.h>
using namespace std;

#define ROWS 3
#define COLS 3

#define WIN 100000
#define	DRAW 0
#define LOSS -100000

#define EMPTY 0
#define NOUGHT 1
#define CROSS 2

int board[ROWS][COLS];

vector< vector< pair<int, int> > > winning_states {
	{ make_pair(0, 0), make_pair(0, 1), make_pair(0, 2) },    //rows
	{ make_pair(1, 0), make_pair(1, 1), make_pair(1, 2) },
	{ make_pair(2, 0), make_pair(2, 1), make_pair(2, 2) },
	
	{ make_pair(0, 0), make_pair(1, 0), make_pair(2, 0) },	  //columns
	{ make_pair(0, 1), make_pair(1, 1), make_pair(2, 1) },
	{ make_pair(0, 2), make_pair(1, 2), make_pair(2, 2) },

	{ make_pair(0, 0), make_pair(1, 1), make_pair(2, 2) },    //diagonals
	{ make_pair(2, 0), make_pair(1, 1), make_pair(0, 2) }
};

void print_board() {
	cout << endl;
	int i, j;
	for( i = 0; i < ROWS; i++ ) {
		cout << "\t\t";
		for( j = 0; j < COLS; j++ ) {
			switch( board[i][j] ) {
				case EMPTY:
					cout << " " ;
					break;
				case CROSS:
					cout << "X";
					break;
				case NOUGHT:
					cout << "0";
					break;
			}
			
			if( j!= 2 ) {
				cout << " |";
			}
		}
		if( i != 2 ) {
			cout << endl << "\t\t--------" << endl;
		}
	}
	cout << endl << endl;					
}

vector<pair<int, int>> get_legal_moves() {
	vector<pair<int, int>> legal_moves;
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			if ( board[i][j] == EMPTY ) {
				legal_moves.push_back(make_pair(i, j));
			}
		}
	}

	return legal_moves;
}

bool isValid( pair<int, int> position ){
	if( position.first >= 0 && position.first < ROWS 
		&& position.second >= 0 && position.second < COLS 
		&& board[position.first][position.second] == EMPTY ) {
			return true;
	}

	return false;
}

vector<pair<int, int>> get_occupied_positions( int currentPlayer ) { //gives positions occupied by current player
	vector<pair<int, int>> occupied_positions;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if ( currentPlayer == board[i][j]) {
				occupied_positions.push_back(make_pair(i, j));
			}
		}
	}

	return occupied_positions;
}

bool isFull() {
	vector<pair<int, int>> legal_moves = get_legal_moves();

	if ( legal_moves.size() == 0 ) {
		return true;
	} else {
		return false;
	}
}

bool hasWon( vector<pair<int, int>> occupied_positions ) {
	bool game_won;

	for (int i = 0; i < winning_states.size(); i++)
	{
		game_won = true;
		vector<pair<int, int>> curr_win_state = winning_states[i];
		for (int j = 0; j < 3; j++) {
			if (!(find(begin(occupied_positions), end(occupied_positions), curr_win_state[j]) != end(occupied_positions))){
				game_won = false;
				break;
			}
		}

		if (game_won) {
			break;
		}
	}
	return game_won;
}

int getState( int currentPlayer )      //returns state of current player
{

	int opponent = ( currentPlayer == NOUGHT ) ? CROSS : NOUGHT;

	vector<pair<int, int>> occupied_positions = get_occupied_positions( currentPlayer );

	bool has_won = hasWon(occupied_positions);

	if ( has_won) {
		return WIN;
	}

	occupied_positions = get_occupied_positions( opponent );
	bool has_lost = hasWon(occupied_positions);

	if ( has_lost) {
		return LOSS;
	}

	return DRAW;
}

pair<int, pair<int, int>> minimax(int currentPlayer, int depth)
{
	pair<int, int> best_move = make_pair(-1, -1);
	int best_score = ( currentPlayer == NOUGHT ) ? LOSS : WIN;

	if ( isFull() || getState( NOUGHT ) != DRAW ) {  
		best_score = getState( NOUGHT );
		return make_pair( best_score, best_move );
	}

	vector<pair<int, int>> legal_moves = get_legal_moves();

	for (int i = 0; i < legal_moves.size(); i++) {
		pair<int, int> current_move = legal_moves[i];
		board[current_move.first][current_move.second] = currentPlayer;
		
		if ( currentPlayer == NOUGHT ) {
			int score = minimax( CROSS, depth + 1).first;

			if (best_score < score) {
				best_score = score - depth;  //score at less depth is prefered
				best_move = current_move;
			}

		} else {
			int score = minimax( NOUGHT, depth + 1).first;

			if (best_score > score) {
				best_score = score + depth ; //as the best_score must be minimum and depth should be minimum 
				best_move = current_move;
			}

		}

		board[current_move.first][current_move.second] = EMPTY; // Undo move

	}

	return make_pair(best_score, best_move);
}

int main()
{
	cout << "\n\n\t\t**TIC TAC TOE**\n\n" << endl << endl;
	
	cout << "\n\n\tPlayer = X\t  Computer = O\n\n" << endl << endl;


	print_board();
	int currentPlayer = CROSS;
	
	while ( !(isFull()) && getState(currentPlayer) == DRAW ) {   //playing state
		
		int row, col;
		cout << "Enter your move row[0..2] col[0..2]" << endl;
		cin >> row >> col;
		
		while( 1 ) {
			if ( !(isValid( make_pair(row, col))) ) { 
				
				cout << endl << "INVALID MOVE! Please enter  again" << endl;
				
				cin >> row >> col;
				
				continue;
			} else {
				board[row][col] = CROSS;
				currentPlayer = NOUGHT;
				print_board();
				break;
			}
		}
		
		
		
		if( getState(currentPlayer) == DRAW && !(isFull()) ) {   //if AI player can play
			
			pair<int, pair<int, int>> ai_move = minimax( NOUGHT, 0);

			board[ai_move.second.first][ai_move.second.second] = NOUGHT;

			print_board();
			
			currentPlayer = CROSS;
		}
	}
	
	cout << "\t\t **GAME OVER**" << endl << endl;

	int player_state = getState(currentPlayer);
	
	if( player_state == DRAW ) {
		cout << "\t\tMATCH DRAW" << endl;
	} else {
		cout << "\t\tPLAYER LOSES" << endl;
	}

	return 0;

}