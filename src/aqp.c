#include "alloc.h"
#include "aqp.h"

typedef struct aqpPiece { char a; } Piece;
typedef struct aqpSegment {} Segment;
typedef struct aqpSegmentEditionRange {} SegmentEditionRange;
typedef struct aqpSegmentRange {} SegmentRange;

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

extern SegmentEditionRange* aqp_edit_range (Segment* segment, int start, int end)
{
	return 0;
}

void aqp_field_write_int (aqp_segment_edition_range_t range, int row, int field, int value)
{
	/* NOT IMPLEMENTED */
}

void aqp_commit_range (SegmentEditionRange* range)
{
	/* NOT IMPLEMENTED */
}

SegmentRange* aqp_read_range (Segment* segment, int start, int end)
{
	return 0;
}

int aqp_field_read_int (aqp_segment_range_t range, int row, int field)
{
	return -1;
}
