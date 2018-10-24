//This anti-chess, also known as Reversi, Othello and flip move, is actually a challenging mind game.
//Have you heard of the saying "A minute to learn, a lifetime to master"? It talks about Reversi.
//Here is an AI of Reversi Lite.
//Hope you like it.

#include<stdlib.h>
#include<stdio.h>
#include <malloc.h>
#include<string.h>

#define START   "START"
#define PLACE   "PLACE"
#define TURN    "TURN"
#define RATE_1  200
#define RATE1   81 
#define RATE2   90
#define RATE3   40
#define RATE4   145
#define RATE5   0 
#define RATE6   46
#define RATE7   55
#define RATE8   48
#define RATE9   46

double weightboard[16][16] = {
	{ RATE4, RATE3, RATE7, RATE2, RATE2, RATE2, RATE2, RATE2, RATE2, RATE2, RATE2, RATE2, RATE2, RATE7, RATE3, RATE4 },
	{ RATE3, RATE5, RATE6, RATE6, RATE6, RATE6, RATE6, RATE6, RATE6, RATE6, RATE6, RATE6, RATE6, RATE6, RATE5, RATE3 },
	{ RATE7, RATE6, RATE8, RATE9, RATE9, RATE9, RATE9, RATE9, RATE9, RATE9, RATE9, RATE9, RATE9, RATE8, RATE6, RATE7 },
	{ RATE2, RATE6, RATE9, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE9, RATE6, RATE2 },
	{ RATE2, RATE6, RATE9, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE9, RATE6, RATE2 },
	{ RATE2, RATE6, RATE9, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE9, RATE6, RATE2 },
	{ RATE2, RATE6, RATE9, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE9, RATE6, RATE2 },
	{ RATE2, RATE6, RATE9, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE9, RATE6, RATE2 },
	{ RATE2, RATE6, RATE9, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE9, RATE6, RATE2 },
	{ RATE2, RATE6, RATE9, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE9, RATE6, RATE2 },
	{ RATE2, RATE6, RATE9, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE9, RATE6, RATE2 },
	{ RATE2, RATE6, RATE9, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE9, RATE6, RATE2 },
	{ RATE2, RATE6, RATE9, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE1, RATE9, RATE6, RATE2 },
	{ RATE7, RATE6, RATE8, RATE9, RATE9, RATE9, RATE9, RATE9, RATE9, RATE9, RATE9, RATE9, RATE9, RATE8, RATE6, RATE7 },
	{ RATE3, RATE5, RATE6, RATE6, RATE6, RATE6, RATE6, RATE6, RATE6, RATE6, RATE6, RATE6, RATE6, RATE6, RATE5, RATE3 },
	{ RATE4, RATE3, RATE7, RATE2, RATE2, RATE2, RATE2, RATE2, RATE2, RATE2, RATE2, RATE2, RATE2, RATE7, RATE3, RATE4 } };
int Tchessboard[16][16] = { 0 };
int direction[8][2] = { { 1, 0 },{ 1, -1 },{ 0, -1 },{ -1, -1 },{ -1, 0 },{ -1, 1 },{ 0, 1 },{ 1, 1 } };
int whose_turn = 0, my_side = 0, my_rival = 0, row = 0, col = 0, final_x = 0, final_y = 0;
double final_score = 0;

typedef struct chess_list
{
	int x;
	int y;
	struct chess_list *next;
}chess_list; //储存所有可能落子坐标的链表的结构体声明
typedef struct tree_node
{
	struct tree_node* up;
	struct tree_node* down_head;
	struct tree_node* left;
	int chessboard[16][16];
	double score;
}tree_node; //二叉树链表的结构体声明
void flip(int x, int y, int chessboard[16][16], int whose_turn); //实现翻子操作
void acquire(int chessboard[16][16]); //获得对方的落子坐标
void place(int chessboard[16][16]); //己方输出落子坐标
double calculate(int chessboard[16][16], int whose_turn); //根据棋盘状态返回估值的函数
int is_available(int x, int y, int chessboard[16][16], int whose_turn); //判断该坐标是否能落子的函数
chess_list* get_available(int chessboard[16][16], int whose_turn); //将所有可落子的坐标存入链表
tree_node create(chess_list* head_1, int chessboard[16][16], tree_node *father, int whose_turn); //建立二叉树
void envision(int chessboard[16][16]); //根据搜索确定己方下一步落子坐标

void flip(int x, int y, int chessboard[16][16], int whose_turn)
{
	int dynamic_row = 0, dynamic_col = 0, flip_number = 0, can_flip = 0, dir_index = 0;
	for (dir_index = 0; dir_index < 8; dir_index++)
	{
		flip_number = 0;
		can_flip = 0;
		dynamic_row = x;
		dynamic_col = y;
		dynamic_row += direction[dir_index][0];
		dynamic_col += direction[dir_index][1];
		while (dynamic_row >= 0 && dynamic_col >= 0 && dynamic_row < 16 && dynamic_col < 16)
		{
			if (chessboard[dynamic_row][dynamic_col] == whose_turn)
			{
				can_flip = 1;
				break;
			}
			if (chessboard[dynamic_row][dynamic_col] == 3 - whose_turn)
			{
				flip_number++;
			}
			else
			{
				break;
			}
			dynamic_row += direction[dir_index][0];
			dynamic_col += direction[dir_index][1];
		}
		if (can_flip && flip_number > 0)
		{
			dynamic_row = x;
			dynamic_col = y;
			while (flip_number > 0)
			{
				dynamic_row += direction[dir_index][0];
				dynamic_col += direction[dir_index][1];
				chessboard[dynamic_row][dynamic_col] = whose_turn;
				flip_number--;
			}
		}
	}
}
void acquire(int chessboard[16][16])
{
	scanf_s("%d %d", &row, &col);
	getchar();
	chessboard[row][col] = my_rival;
	flip(row, col, chessboard, my_rival);
}
void place(int chessboard[16][16])
{
	printf("%d %d\n", final_x, final_y);
	fflush(stdout);
	chessboard[final_x][final_y] = my_side;
	flip(final_x, final_y, chessboard, my_side);
}
double calculate(int chessboard[16][16], int whose_turn)
{
	double score = 0;
	int mobility_1 = 0, mobility_2 = 0;
	for (int x = 0; x < 16; x++)
	{
		for (int y = 0; y < 16; y++)
		{
			if (chessboard[x][y] == whose_turn)
				score += weightboard[x][y];
			if (chessboard[x][y] == 3 - whose_turn)
				score -= weightboard[x][y];
			if (chessboard[x][y] == 0 && is_available(x, y, chessboard, whose_turn) == 1)
				mobility_1++;
			if (chessboard[x][y] == 0 && is_available(x, y, chessboard, 3 - whose_turn) == 1)
				mobility_2++;
		}
	}
	return (score + (mobility_1 - mobility_2) * RATE_1);
}
int is_available(int x, int y, int chessboard[16][16], int whose_turn)
{
	int dynamic_row = 0, dynamic_col = 0, flip_number = 0, can_flip = 0;
	for (int dir_index = 0; dir_index < 8; dir_index++)
	{
		flip_number = 0;
		can_flip = 0;
		dynamic_row = x;
		dynamic_col = y;
		dynamic_row += direction[dir_index][0];
		dynamic_col += direction[dir_index][1];
		while (dynamic_row >= 0 && dynamic_col >= 0 && dynamic_row < 16 && dynamic_col < 16)
		{
			if (chessboard[dynamic_row][dynamic_col] == whose_turn)
			{
				can_flip = 1;
				break;
			}
			if (chessboard[dynamic_row][dynamic_col] == 3 - whose_turn)
			{
				flip_number++;
			}
			if (chessboard[dynamic_row][dynamic_col] == 0)
			{
				break;
			}
			dynamic_row += direction[dir_index][0];
			dynamic_col += direction[dir_index][1];
		}
		if (can_flip == 1 && flip_number > 0)
		{
			return 1;
		}
	}
	return 0;
}
chess_list* get_available(int chessboard[16][16], int whose_turn)
{
	chess_list *head;
	chess_list *p1, *p2;
	int n = 0;
	p1 = p2 = (chess_list*)malloc(sizeof(chess_list));
	head = NULL;
	for (int x = 0; x < 16; x++)
	{
		for (int y = 0; y < 16; y++)
		{
			if (is_available(x, y, chessboard, whose_turn) == 1 && chessboard[x][y] == 0)
			{
				n++;
				if (n == 1)
				{
					head = p1;
					p1->x = x;
					p1->y = y;
					p1->next = NULL;
				}
				else
				{
					p2 = (chess_list*)malloc(sizeof(chess_list));
					p1->next = p2;
					p1 = p2;
					p1->x = x;
					p1->y = y;
					p1->next = NULL;
				}
			}
		}
	}
	return head;
}
tree_node create(chess_list* head_1, int chessboard[16][16], tree_node *father, int whose_turn)
{
	int temp[16][16] = { 0 };
	tree_node *head;
	tree_node *p1, *p2;
	chess_list *p = head_1;
	p1 = p2 = (tree_node*)malloc(sizeof(tree_node));
	head = NULL;
	father->down_head = p1;
	head = p1;
	head->down_head = NULL;
	head->left = NULL;
	head->up = father;
	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 16; y++)
			temp[x][y] = father->chessboard[x][y];
	temp[head_1->x][head_1->y] = whose_turn;
	flip(head_1->x, head_1->y, temp, whose_turn);
	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 16; y++)
			head->chessboard[x][y] = temp[x][y];
	head->score = calculate(head->chessboard, whose_turn);
	while (p->next != NULL)
	{
		for (int a = 0; a < 16; a++)
		{
			for (int b = 0; b < 16; b++)
			{
				temp[a][b] = 0;
			}
		}
		p2 = (tree_node*)malloc(sizeof(tree_node));
		p1->left = p2;
		p1->down_head = NULL;
		p1->up = father;
		p1 = p2;
		p = p->next;
		for (int x = 0; x < 16; x++)
			for (int y = 0; y < 16; y++)
				temp[x][y] = father->chessboard[x][y];
		temp[p->x][p->y] = whose_turn;
		flip(p->x, p->y, temp, whose_turn);
		for (int x = 0; x < 16; x++)
			for (int y = 0; y < 16; y++)
				p1->chessboard[x][y] = temp[x][y];
		p1->score = calculate(p1->chessboard, whose_turn);
	}
	return *head;
}
void envision(int chessboard[16][16])
{
	chess_list *chess_1 = get_available(chessboard, my_side);
	tree_node *root;
	root = (tree_node*)malloc(sizeof(tree_node));
	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 16; y++)
			root->chessboard[x][y] = chessboard[x][y];
	create(chess_1, root->chessboard, root, my_side);
	tree_node *p = root->down_head;
	final_x = chess_1->x;
	final_y = chess_1->y;
	final_score = p->score;
	if (p->left != NULL)
	{
		p = p->left;
		chess_1 = chess_1->next;
		for (; p != NULL; p = p->left, chess_1 = chess_1->next)
		{
			if (p->score > final_score)
			{
				final_x = chess_1->x;
				final_y = chess_1->y;
				final_score = p->score;
			}
		}
	}
	place(chessboard);
}
int main() //主函数
{
	Tchessboard[7][7] = 2;
	Tchessboard[7][8] = 1;
	Tchessboard[8][8] = 2;
	Tchessboard[8][7] = 1;
	char instruction[10];
	while (1)
	{
		scanf_s("%s", instruction, 10);
		getchar();
		if (strcmp(instruction, START) == 0)
		{
			scanf_s("%d", &my_side);
			getchar();
			printf("OK\n");
			fflush(stdout);
			my_rival = 3 - my_side;
		}
		if (strcmp(instruction, PLACE) == 0)
		{
			acquire(Tchessboard);
		}
		if (strcmp(instruction, TURN) == 0)
		{
			envision(Tchessboard);
		}
	}
	return 0;
}