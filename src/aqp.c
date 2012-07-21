#include "alloc.h"
#include "aqp.h"

typedef struct aqpPiece { char a; } Piece;
typedef struct aqpSegment {} Segment;

extern Piece* aqp_new_piece(void)
{
	Piece* piece = aqp_calloc(1, sizeof *piece);

	return piece;
}

extern void aqp_delete_piece(Piece* piece)
{
	aqp_free (piece);
}

extern Segment* aqp_new_segment (Piece* piece)
{
	return 0;
}
