#include "alloc.h"

typedef struct aqp_Piece {} Piece;

extern Piece* aqp_piece_alloc()
{
	Piece* piece = aqp_calloc(1, sizeof *piece);

	return piece;
}

extern void aqp_piece_free(Piece* piece)
{
	aqp_free (piece);
}
