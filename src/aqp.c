#include "alloc.h"
#include "aqp.h"

#include "growa.h"

typedef struct aqpSegment
{

} Segment;

typedef struct aqpSegmentRange
{
} SegmentRange;

typedef struct aqpSegmentEditionRange
{
} SegmentEditionRange;

typedef struct aqpPiece
{
	Segment** segments;
	int segments_n;
	int segments_size;
} Piece;

extern Piece* aqp_new_piece(void)
{
	Piece* piece = aqp_calloc(1, sizeof *piece);

	piece->segments   = 0;
	piece->segments_n = 0;
	piece->segments_size = 0;

	return piece;
}

extern void aqp_delete_piece(Piece* piece)
{
	for (int i = 0; i < piece->segments_n; i++) {
		aqp_free (piece->segments[i]);
		piece->segments[i] = 0;
	}
	freea_m(piece->segments, piece->segments_n, piece->segments_size);
	aqp_free (piece);
}

extern Segment* aqp_new_segment (Piece* piece)
{
	int i;
	for (i = 0; i < piece->segments_n; i++) {
		if (!piece->segments[i]) {
			break;
		}
	}

	growa_m(i + 1, piece->segments, piece->segments_n, piece->segments_size);

	Segment* segment = aqp_calloc (1, sizeof *segment);

	piece->segments[i] = segment;

	return segment;
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
