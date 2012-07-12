#include "alloc.h"

struct aqp_Piece {};

extern struct aqp_Piece* aqp_piece_alloc()
{
	struct aqp_Piece* piece = aqp_calloc(1, sizeof *piece);

	return piece;
}

extern void aqp_piece_free(struct aqp_Piece* piece)
{
	aqp_free (piece);
}
