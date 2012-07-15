#include "alloc.h"
#include "aqp.h"

typedef struct aqpPiece { char a; } Piece;

extern aqp_piece_t aqp_new_piece(void)
{
	Piece* piece = aqp_calloc(1, sizeof *piece);

	return (aqp_piece_t) { piece };
}

extern void aqp_delete_piece(aqp_piece_t piece)
{
	aqp_free (piece.impl);
	piece.impl = 0;
}
