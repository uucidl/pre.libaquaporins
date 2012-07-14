#include "alloc.h"
#include "aqp.h"

typedef struct aqpPiece { char a; } Piece;

extern Piece* aqp_piece_alloc()
{
	Piece* piece = aqp_calloc(1, sizeof *piece);

	return piece;
}

extern void aqp_piece_free(Piece* piece)
{
	aqp_free (piece);
}
