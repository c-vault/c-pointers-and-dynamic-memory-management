/* count.h */

#define MAX_CHARS 5000

typedef struct score score;
typedef score *scorep;
struct score {
	int char_count;
	int word_count;
	int par_count;
};

typedef struct cursor cursor;
typedef cursor *cursorp;
struct cursor {
	int row;
	int col;
};

typedef struct count_pos count_pos;
typedef count_pos *count_posp;
struct count_pos {
	cursor char_pos;
	cursor word_pos;
	cursor par_pos;
};
	