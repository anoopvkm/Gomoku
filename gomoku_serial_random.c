#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <limits.h>
#define DIM 19
#define WIN_VAL 400000
#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)
#define DEPTH 4
#define NO_ITERATION 20
// 1 - black
// 2 -white
typedef struct{
	char **board;
	int count;
} State;

// Intializes a board
void create_board(State *s);

// Generates all possible moves and stores them in *moves
void generate_moves(State *s, short *moves );

// Geneates limited moves and returns the number of moves
int  generate_moves2(State *s, short *moves, int x, int y, int player );

// Formatted printing of the board
void print_board(State *s);

// Assigns a sccore to board position
int assign_score(int player, int p1_count, int p2_count, int free_count);

// evaluates a board position , 2 types of evaluation function
// evaluate2 used
int evaluate(State *s, char player);
int evaluate2(State *s, char player);


// A human vs AI player loop
void play();


// Checks for terminal condition
int is_terminal(State *s,int x, int y, char player);

// alpha beta max value function
int max_val(State *s,int depth, int a, int b,int x, int y, char player,short *mov);

// alpha beta min value function
int min_val(State *s,int depth, int a, int b,int x, int y,char player,short *mov);


int main(int argc, char **argv){
	play();
	
}


// Maximises AI player node's value
int max_val(State *s,int depth, int a, int b,int x, int y,char player,short *mov){

	if(depth == 0){
		return evaluate2(s,2);
	}
	if(is_terminal(s,x,y,player) == 1){
		return evaluate2(s,2) ;

	}
	int score = INT_MIN;
	short *moves;
	moves = (short*)malloc((DIM*DIM - s->count)*sizeof(short));


	int len = generate_moves2(s,moves,x,y,player);
	int i;
	short move;
	char otherplayer = 1;
	int temp;
	short tmove;
	if(player == 1){
		otherplayer = 2;
	}
	for(i = 0; i < len; i++){
		move = moves[i];
		s->board[move/100][move%100] = otherplayer;
		s->count++;
		temp = score;
		score = MAX(score,min_val(s,depth-1,a,b,move/100,move%100,otherplayer,&tmove));
		if(temp != score){
			*mov = move;
		}
		s->board[move/100][move%100] = 0;
		s->count--;
		a = MAX(a,score);
		if(a >= b){
			free(moves);		
			return a;
		}

	}
	free(moves);
	return score;
}

// Minimizes human player heuristic value
int min_val(State *s,int depth, int a, int b,int x, int y,char player,short *mov){

	if (depth == 0){
		return evaluate2(s,2);
	}
	if(is_terminal(s,x,y,player) == 1){
		return evaluate2(s,2);

	}
	int score = INT_MAX;
	short *moves;
	moves = (short*)malloc((DIM*DIM - s->count)*sizeof(short));


	int len = generate_moves2(s,moves,x,y,player);
	int i;
	short move;
	char otherplayer = 1;
	int temp;
	short tmove;
	if(player == 1){
		otherplayer = 2;
	}
	for(i = 0; i < len; i++){
		move = moves[i];
		s->board[move/100][move%100] = otherplayer;
		s->count++;
		temp = score;
		score = MIN(score,max_val(s,depth-1,a,b,move/100,move%100,otherplayer,&tmove));
		if(temp != score){
			*mov = move;
		}
		s->board[move/100][move%100] = 0;
		s->count--;
		b = MIN(b,score);
		if(a >= b){
			free(moves);
			return b;
		}

	}
	free(moves);
	return score;
}
// Play loop
void play(){
	State s;
	create_board(&s);
	int i,j;
	char player = 1;
	char aiplayer = 2;
	int score;
	short mov;
	double start_time, end_time;
	struct timeval tz;
	struct timezone tx;
	printf("\nRow and column range from 0 to 18\n" );
	int no_itr = 0;
	double total = 0;
	srandom(0);
	while(no_itr < NO_ITERATION){
		no_itr++;
		i = random()%DIM;
		j = random()%DIM;
		while( i >= DIM || j >= DIM || s.board[i][j] != 0){
		
			i = random()%DIM;
			j = random()%DIM;

		}
		s.board[i][j] = player;
		s.count++;
		if(is_terminal(&s,i,j,player) == 1){
			printf("\nHuman Won!!!!!!!!\n");
			break;
		}
		gettimeofday(&tz,&tx);
		start_time = (double)tz.tv_sec;
		score = max_val(&s,DEPTH,INT_MIN,INT_MAX,i,j,player,&mov);
		gettimeofday(&tz,&tx);
		end_time = (double)tz.tv_sec;
		total += (end_time - start_time);
		printf("\nTime taken : %lf", end_time - start_time);
		s.board[mov/100][mov%100] = aiplayer;
		s.count++;
		print_board(&s);
	
		if(is_terminal(&s,mov/100,mov%100,aiplayer) == 1){
			printf("\nComputer Won!!!!!!!!!\n");
				break;
		}
	}

	printf("\nAverage time %lf",total/no_itr);

}
int traverse_alphabeta(State *s){
	return 0;
}
int is_terminal(State *s, int i, int j, char player){
	int l,m;
	int ocp1,ocp2;
	char val;
	// Main diagonal top
				l = 1;
				m = 1;
				ocp1 = 0;
				while(i-l >= 0 && j-l >=0 && l < 5 && m < 5){
				    val = s->board[i-l][j-m];
					if( val == 0 )
						break;
					else if(val == player)
						ocp1++;
					else
						break;
					l++;
					m++;
				}
				// Main diagonal bottom
				l = 1;
				m = 1;
				ocp2 = 0;
				while(i+l < DIM && j+l < DIM && l < 5 && m < 5){
				    val = s->board[i+l][j+m];
					if( val == 0 )
						break;
					else if(val == player)
						ocp2++;
					else
						break;
					l++;
					m++;
				}

				if(ocp1 + ocp2 == 4){
					return 1;
				}


				// Main vertical up
				l = 1;
			
				ocp1 = 0;
				while(i-l >= 0  && l < 5){
				    val = s->board[i-l][j];
					if( val == 0 )
						break;
					else if(val == player)
						ocp1++;
					else
						break;
					l++;
				
				}

				// Main diagonal down
				l = 1;
				ocp2 = 0;
				while(i+l < DIM && l < 5){
				    val = s->board[i+l][j];
					if( val == 0 )
						break;
					else if(val == player)
						ocp2++;
					else
						break;
					l++;
				}

				if(ocp1 + ocp2 == 4){
					return 1;
				}
				// Main horizontal back
				l = 1;
			
				ocp1 = 0;
				while(j-l >= 0  && l < 5){
				    val = s->board[i][j-l];
					if( val == 0 )
						break;
					else if(val == player)
						ocp1++;
					else
						break;
					l++;
				
				}

				// Main horizontal forward
				l = 1;
				ocp2 = 0;
				while(j+l < DIM && l < 5){
				    val = s->board[i][j+l];
					if( val == 0 )
						break;
					else if(val == player)
						ocp2++;
					else
						break;
					l++;
				}


				if (ocp1 + ocp2 == 4){
					return 1;
				}
				// Off diagonal top
				l = 1;
				m = 1;
				ocp1 = 0;
				while(i-l >= 0 && j+m < DIM && l < 5 && m < 5){
				    val = s->board[i-l][j+m];
					if( val == 0 )
						break;
					else if(val == player)
						ocp1++;
					else
						break;
					l++;
					m++;
				}

				// Off diagonal bottom
				l = 1;
				m = 1;
				ocp2 = 0;
				while(i+l < DIM && j-m >= 0 && l < 5 && m < 5){
				    val = s->board[i+l][j-m];
					if( val == 0 )
						break;
					else if(val == player)
						ocp2++;
					else
						break;
					l++;
					m++;
				}

				if(ocp1 + ocp2 == 4){
					return 1;
				}


				return 0;


}

// Evaluation function
// Counts the filled positions along diagonals and vertical and horizontal
// and passes them to assign score function to obtain a value
int evaluate2(State *s, char player){
	int i, j ;
	int free_count, p1_count, p2_count;
	char opp = 1;
	if(player == 1){
		opp = 2;
	}

	int l,m;
	int score = 0;
	int t_score = 0;
	for(i = 0; i < DIM; i++){
		for(j = 0; j < DIM; j++){

			// Column
			l =0; 
		
			p1_count = 0;
			p2_count = 0;
			free_count = 0;
			while(i+l < DIM  && l < 5){
				if(s->board[i+l][j] == 1){
					p1_count++;
				}
				else if(s->board[i+l][j] == 2){
					p2_count++;
				}
				else{
					free_count++;
				}
				l++;
			}

			score += assign_score(player,p1_count,p2_count,free_count);

			// Row
			l =0; 
		
			p1_count = 0;
			p2_count = 0;
			free_count = 0;
			while(j+l < DIM  && l < 5){
				if(s->board[i][j+l] == 1){
					p1_count++;
				}
				else if(s->board[i][j+l] == 2){
					p2_count++;
				}
				else{
					free_count++;
				}
				l++;
			}

			score += assign_score(player,p1_count,p2_count,free_count);

			l =0; 
			m = 0;
		
			p1_count = 0;
			p2_count = 0;
			free_count = 0;
			while(i+l < DIM && j+m < DIM  && l < 5 && m < 5){
				if(s->board[i+l][j+m] == 1){
					p1_count++;
				}
				else if(s->board[i+l][j+m] == 2){
					p2_count++;
				}
				else{
					free_count++;
				}
				l++;
				m++;
			}

			score += assign_score(player,p1_count,p2_count,free_count);



			l =0; 
			m = 0;
		
			p1_count = 0;
			p2_count = 0;
			free_count = 0;
			while(i - l >= 0  && j+m < DIM  && l < 5 && m < 5){
				if(s->board[i-l][j+m] == 1){
					p1_count++;
				}
				else if(s->board[i-l][j+m] == 2){
					p2_count++;
				}
				else{
					free_count++;
				}
				l++;
				m++;
			}

			score += assign_score(player,p1_count,p2_count,free_count);



		}
	}

	return score;
	
}

// Assigns score
int assign_score(int player, int p1_count, int p2_count, int free_count){
	int score = 0;
	if(player == 1){
		if(p1_count == 5){
			score += 2000;
			return score;
		}
		if(p1_count == 4 && free_count == 1){
			return 1000;
		}
		if(p1_count + free_count == 5){
			score += p1_count*2 + free_count;
		}
		if(p1_count  == 1 && p2_count == 4){
			score += 1000;
		}
		if(p1_count == 2 && p2_count == 3){
			score += 700;
		}
	}
	else{
		if(p2_count == 5){
			score += 2000;
		}
		if(p2_count == 4 && free_count == 1){
			return 1000;
		}
		if(p2_count + free_count == 5){
			score += p2_count*2 + free_count;
		}
		if(p2_count == 1 && p1_count == 4){
			score += 1000;
		}
		if(p1_count == 2 && p2_count == 3){
			score += 700;
		}
	}

	return score;


}

// Unused evalautation function
int evaluate(State *s , char player){

	int i,j;
	int free1,ocp1;
	int free2,ocp2;
	char opp = 1;
	int l,m;
	char val;
	int score = 0;
	if(player == 1){
		opp = 2;
	}
	for (i =0 ; i < DIM ; i++){
		for(j = 0; j < DIM; j++){
			if(s->board[i][j] == player){
				// Main diagonal top
				l = 1;
				m = 1;
				free1 = 0;
				ocp1 = 0;
				while(i-l >= 0 && j-l >=0 && l < 5 && m < 5){
				    val = s->board[i-l][j-m];
					if( val == 0 )
						free1++;
					else if(val == player)
						ocp1++;
					else
						break;
					l++;
					m++;
				}

				// Main diagonal bottom
				l = 1;
				m = 1;
				free2 = 0;
				ocp2 = 0;
				while(i+l < DIM && j+l < DIM && l < 5 && m < 5){
				    val = s->board[i+l][j+m];
					if( val == 0 )
						free2++;
					else if(val == player)
						ocp2++;
					else
						break;
					l++;
					m++;
				}

				if(free2+ocp2 == 4){
					score += ocp2*10+free2*2;
				}
				if(free1+ocp1 == 4){
					score += ocp1*10+free1*2;
				}
				if(free1+ocp1+free2+ocp2 == 4){
					score += (ocp1+ocp2)*10 + (free1+free2)*2;
				}
				else if((free1+ocp1+free2+ocp2) < 4){
					// TODO
				}



				// Main vertical up
				l = 1;
			
				free1 = 0;
				ocp1 = 0;
				while(i-l >= 0  && l < 5){
				    val = s->board[i-l][j];
					if( val == 0 )
						free1++;
					else if(val == player)
						ocp1++;
					else
						break;
					l++;
				
				}

				// Main diagonal down
				l = 1;
				free2 = 0;
				ocp2 = 0;
				while(i+l < DIM && l < 5){
				    val = s->board[i+l][j];
					if( val == 0 )
						free2++;
					else if(val == player)
						ocp2++;
					else
						break;
					l++;
				}

				if(free2+ocp2 == 4){
					score += ocp2*10+free2*2;
				}
				if(free1+ocp1 == 4){
					score += ocp1*10+free1*2;
				}
				if(free1+ocp1+free2+ocp2 == 4){
					score += (ocp1+ocp2)*10 + (free1+free2)*2;
				}
				else if(free1+ocp1+free2+ocp2 < 4){
					// TODO
				}

				// Main horizontal back
				l = 1;
			
				free1 = 0;
				ocp1 = 0;
				while(j-l >= 0  && l < 5){
				    val = s->board[i][j-l];
					if( val == 0 )
						free1++;
					else if(val == player)
						ocp1++;
					else
						break;
					l++;
				
				}

				// Main horizontal forward
				l = 1;
				free2 = 0;
				ocp2 = 0;
				while(j+l < DIM && l < 5){
				    val = s->board[i][j+l];
					if( val == 0 )
						free2++;
					else if(val == player)
						ocp2++;
					else
						break;
					l++;
				}

				if(free2+ocp2 == 4){
					score += ocp2*10+free2*2;
				}
				if(free1+ocp1 == 4){
					score += ocp1*10+free1*2;
				}
				if(free1+ocp1+free2+ocp2 == 4){
					score += (ocp1+ocp2)*10 + (free1+free2)*2;
				}
				else if(free1+ocp1+free2+ocp2 < 4){
					// TODO
				}

				// Off diagonal top
				l = 1;
				m = 1;
				free1 = 0;
				ocp1 = 0;
				while(i-l >= 0 && j+m < DIM && l < 5 && m < 5){
				    val = s->board[i-l][j+m];
					if( val == 0 )
						free1++;
					else if(val == player)
						ocp1++;
					else
						break;
					l++;
					m++;
				}

				// Off diagonal bottom
				l = 1;
				m = 1;
				free2 = 0;
				ocp2 = 0;
				while(i+l < DIM && j-m >= 0 && l < 5 && m < 5){
				    val = s->board[i+l][j-m];
					if( val == 0 )
						free2++;
					else if(val == player)
						ocp2++;
					else
						break;
					l++;
					m++;
				}

				if(free2+ocp2 == 4){
					score += ocp2*10+free2*2;
				}
				if(free1+ocp1 == 4){
					score += ocp1*10+free1*2;
				}
				if(free1+ocp1+free2+ocp2 == 4){
					score += (ocp1+ocp2)*10 + (free1+free2)*2;
				}
				else if(free1+ocp1+free2+ocp2 < 4){
					// TODO
				}



			
				player = opp;
				// Main diagonal top
				l = 1;
				m = 1;
				free1 = 0;
				ocp1 = 0;
				while(i-l >= 0 && j-l >=0 && l < 5 && m < 5){
				    val = s->board[i-l][j-m];
					if( val == 0 )
						free1++;
					else if(val == player)
						ocp1++;
					else
						break;
					l++;
					m++;
				}

				// Main diagonal bottom
				l = 1;
				m = 1;
				free2 = 0;
				ocp2 = 0;
				while(i+l < DIM && j+l < DIM && l < 5 && m < 5){
				    val = s->board[i+l][j+m];
					if( val == 0 )
						free2++;
					else if(val == player)
						ocp2++;
					else
						break;
					l++;
					m++;
				}

				if(free2+ocp2 == 4){
					score +=2*(ocp2*10+free2*2);
				}
				if(free1+ocp1 == 4){
					score += 2*(ocp1*10+free1*2);
				}
				if(free1+ocp1+free2+ocp2 == 4){
					score += 2*((ocp1+ocp2)*10 + (free1+free2)*2);
				}
				else if((free1+ocp1+free2+ocp2) < 4){
					// TODO
				}
				if(ocp1+ocp2 == 4){
					score += 100;
				}
				


				// Main vertical up
				l = 1;
			
				free1 = 0;
				ocp1 = 0;
				while(i-l >= 0  && l < 5){
				    val = s->board[i-l][j];
					if( val == 0 )
						free1++;
					else if(val == player)
						ocp1++;
					else
						break;
					l++;
				
				}

				// Main diagonal down
				l = 1;
				free2 = 0;
				ocp2 = 0;
				while(i+l < DIM && l < 5){
				    val = s->board[i+l][j];
					if( val == 0 )
						free2++;
					else if(val == player)
						ocp2++;
					else
						break;
					l++;
				}

				if(free2+ocp2 == 4){
					score += 2*ocp2*10+2*free2*2;
				}
				if(free1+ocp1 == 4){
					score += 2*ocp1*10+2*free1*2;
				}
				if(free1+ocp1+free2+ocp2 == 4){
					score += 2*(ocp1+ocp2)*10 + 2*(free1+free2)*2;
				}
				else if(free1+ocp1+free2+ocp2 < 4){
					// TODO
				}
				if(ocp1+ocp2 == 4){
					score += 100;
				}
				// Main horizontal back
				l = 1;
			
				free1 = 0;
				ocp1 = 0;
				while(j-l >= 0  && l < 5){
				    val = s->board[i][j-l];
					if( val == 0 )
						free1++;
					else if(val == player)
						ocp1++;
					else
						break;
					l++;
				
				}

				// Main horizontal forward
				l = 1;
				free2 = 0;
				ocp2 = 0;
				while(j+l < DIM && l < 5){
				    val = s->board[i][j+l];
					if( val == 0 )
						free2++;
					else if(val == player)
						ocp2++;
					else
						break;
					l++;
				}

				if(free2+ocp2 == 4){
					score += 2*ocp2*10+2*free2*2;
				}
				if(free1+ocp1 == 4){
					score += 2*ocp1*10+2*free1*2;
				}
				if(free1+ocp1+free2+ocp2 == 4){
					score += 2*(ocp1+ocp2)*10 + 2*(free1+free2)*2;
				}
				else if(free1+ocp1+free2+ocp2 < 4){
					// TODO
				}

				if(ocp1+ocp2 == 4){
					score += 100;
				}
				
				// Off diagonal top
				l = 1;
				m = 1;
				free1 = 0;
				ocp1 = 0;
				while(i-l >= 0 && j+m < DIM && l < 5 && m < 5){
				    val = s->board[i-l][j+m];
					if( val == 0 )
						free1++;
					else if(val == player)
						ocp1++;
					else
						break;
					l++;
					m++;
				}

				// Off diagonal bottom
				l = 1;
				m = 1;
				free2 = 0;
				ocp2 = 0;
				while(i+l < DIM && j-m >= 0 && l < 5 && m < 5){
				    val = s->board[i+l][j-m];
					if( val == 0 )
						free2++;
					else if(val == player)
						ocp2++;
					else
						break;
					l++;
					m++;
				}

				if(free2+ocp2 == 4){
					score += 2*ocp2*10+2*free2*2;
				}
				if(free1+ocp1 == 4){
					score += 2*ocp1*10+2*free1*2;
				}
				if(free1+ocp1+free2+ocp2 == 4){
					score += 2*(ocp1+ocp2)*10 +2* (free1+free2)*2;
				}
				else if(free1+ocp1+free2+ocp2 < 4){
					// TODO
				}
				if(ocp1+ocp2 == 4){
					score += 100;
				}
				


			}
		}
	}
	return score;

}
void create_board(State *s){

	s->board = (char**)malloc(DIM*sizeof(char*));
	int i;
	int j;
	for(i = 0; i < DIM; i++){
		s->board[i] = (char*)malloc(DIM*sizeof(char));
		for(j = 0; j < DIM ; j++){
			s->board[i][j] = 0;
		}
	}
	s->count = 0;


}

void print_board(State *s){
	int i,j;

	printf("\n");
	char val;
	for(i = 0 ; i < 2*DIM + 1 ; i ++){
		for( j = 0 ; j < 2*DIM + 1; j++){
			if( i%2 == 0){
				printf("__");
			}
			else{
				if(j%2 == 0){
					printf("|");
				}
				else{
					val = s->board[i/2][j/2];
					if(val == 0){
						printf("   ");
					}
					else
						printf(" %d ",s->board[i/2][j/2]);
				}
			}
		}
		printf("\n");
	}
}
void generate_moves(State *s, short *moves){

	int i,j;


	int pos_front = 0;
	int pos_back = DIM*DIM - s->count -1;
	for (i = 0 ; i < DIM; i++){
		for(j = 0; j < DIM; j++){
			if(s->board[i][j] == 0){
				if(i > 0 && i < DIM - 1 && j > 0 && j <DIM -1 
						&& (s->board[i+1][j] != 0 || s->board[i][j+1] != 0 
							|| s->board[i-1][j] != 0 || s->board[i][j-1] != 0
							|| s->board[i-1][j-1] != 0 || s->board[i+1][j+1] != 0
							|| s->board[i-1][j+1] != 0 || s->board[i+1][j-1] != 0))
					moves[pos_front++] = 100*i+j;		
				else
					moves[pos_back--] = 100*i + j;
			}
		}
	}


}
int abs(int x){
	if(x < 0){
		return -1*x;
	}
	return x;
}
int generate_moves2(State *s, short *moves, int x, int y, int player){

	int i,j;

	int len = 0;
	int pos_front = 0;
	int pos_back = DIM*DIM - s->count -1;
	int depth = 1;
	// Generates nodes at 5 distance from current point
	while(depth < 5){
		 i = x + depth;
		 j = y;
		 if( i < DIM && s->board[i][j] == 0){
			 moves[pos_front++] = 100*i+j;
		 }

		 i = x - depth;
		 j = y;
		 if( i >= 0 && s->board[i][j] == 0){
			 moves[pos_front++] = 100*i+j;
		 }

		 i = x;
		 j = y + depth;
		 if( j < DIM && s->board[i][j] == 0){
			 moves[pos_front++] = 100*i+j;
		 }

		 i = x ;
		 j = y - depth;
		 if( j >= 0 && s->board[i][j] == 0){
			 moves[pos_front++] = 100*i+j;
		 }

		 i = x - depth;
		 j = y - depth;
		 if( i >=0 && j >= 0  && s->board[i][j] == 0){
			 moves[pos_front++] = 100*i+j;
		 }

		 i = x + depth;
		 j = y + depth;
		 if( i < DIM && j < DIM  && s->board[i][j] == 0){
			 moves[pos_front++] = 100*i+j;
		 }

		 i = x - depth;
		 j = y + depth;
		 if( i >=0 && j < DIM  && s->board[i][j] == 0){
			 moves[pos_front++] = 100*i+j;
		 }

		 i = x + depth;
		 j = y -  depth;
		 if( i < DIM  && j >= 0  && s->board[i][j] == 0){
			 moves[pos_front++] = 100*i+j;
		 }
		 depth++;
	}

	// If no free space, searching the entire tree
	if(pos_front == 0){
		for (i = 0 ; i < DIM; i++){
		if(abs(i-x) > 5){
			break;
		}
		for(j = 0; j < DIM; j++){
			if( abs(y-j) > 5){
				break;
			}
			if(s->board[i][j] == 0){
				if(i > 0 && i < DIM - 1 && j > 0 && j <DIM -1 
						&& (s->board[i+1][j] != 0 || s->board[i][j+1] != 0 
							|| s->board[i-1][j] != 0 || s->board[i][j-1] != 0
							|| s->board[i-1][j-1] != 0 || s->board[i+1][j+1] != 0
							|| s->board[i-1][j+1] != 0 || s->board[i+1][j-1] != 0))
					moves[pos_front++] = 100*i+j;		
				else
					moves[pos_front++] = 100*i+j;
				len++;
				}
			}
		
		}
	}
	len = pos_front;
	if(pos_front == 0){
		generate_moves(s,moves);
		len = DIM*DIM - s->count -1; 
	}
	return len;

}

