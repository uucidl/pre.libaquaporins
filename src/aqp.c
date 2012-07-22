#include <string.h>
#include <assert.h>

#include "alloc.h"
#include "aqp.h"
#include "growa.h"
#include "ca.h"

typedef struct aqpSegment
{
	struct aqpPiece* piece;
	struct FieldDescription
	{
		int type_id;
		int size;
	} row_type[1];

	int row_size;

	char* rowdata;
	char rowdata_capacity;
	char rowdata_n;
} Segment;

static void delete_segment (Segment* segment)
{
	freea_m(segment->rowdata, segment->rowdata_n, segment->rowdata_capacity);
	aqp_free (segment);
}

static int count_rows (Segment* segment)
{
	return segment->rowdata_n / segment->row_size;
}

static int count_fields(Segment* segment)
{
	return array_count_m(segment->row_type);
}

typedef struct aqpSegmentRange
{
	Segment* segment;
	int row_start;
	int row_end;
} SegmentRange;

static void make_range (SegmentRange* range, Segment* segment, int start, int end)
{
	assert(start <= end);
	assert(start >= 0);

	int const n = count_rows(segment);
	int effective_end = end >= n ? n : end;

	*range = (SegmentRange) {
		.segment   = segment,
		.row_start = start,
		.row_end   = effective_end,
	};
}

static char* field_pointer(SegmentRange* view_range, int row, int field)
{
	Segment* segment = view_range->segment;
	int field_n = count_fields(segment);

	assert(row < view_range->row_end - view_range->row_start);
	assert(field < field_n);

	char* rowptr = segment->rowdata + segment->row_size * (view_range->row_start + row);
	char* fieldptr = rowptr;

	for (int i = 0; i < field; i++) {
		fieldptr += segment->row_type[i].size;
	}

	return fieldptr;
}

typedef struct aqpSegmentEditionRange
{
	SegmentRange view_range;
} SegmentEditionRange;

typedef struct aqpPiece
{
	Segment** segments;
	int segments_n;
	int segments_capacity;

	SegmentRange** read_ranges;
	int read_ranges_n;
	int read_ranges_capacity;

	SegmentEditionRange** edit_ranges;
	int edit_ranges_n;
	int edit_ranges_capacity;
} Piece;

extern Piece* aqp_new_piece(void)
{
	Piece* piece = aqp_calloc(1, sizeof *piece);

	inita_m(piece->segments,    piece->segments_n,    piece->segments_capacity);
	inita_m(piece->read_ranges, piece->read_ranges_n, piece->read_ranges_capacity);
	inita_m(piece->edit_ranges, piece->edit_ranges_n, piece->edit_ranges_capacity);

	return piece;
}

extern void aqp_delete_piece(Piece* piece)
{
	for (int i = 0; i < piece->segments_n; i++) {
		if (!piece->segments[i])
			continue;

		delete_segment (piece->segments[i]);
		piece->segments[i] = 0;
	}

	for (int i = 0; i < piece->read_ranges_n; i++) {
		if (!piece->read_ranges[i])
			continue;

		aqp_free (piece->read_ranges[i]);
		piece->read_ranges[i] = NULL;
	}

	for (int i = 0; i < piece->edit_ranges_n; i++) {
		if (!piece->edit_ranges[i])
			continue;

		aqp_free (piece->edit_ranges[i]);
		piece->edit_ranges[i] = NULL;
	}

	freea_m(piece->segments,    piece->segments_n,    piece->segments_capacity);
	freea_m(piece->read_ranges, piece->read_ranges_n, piece->read_ranges_capacity);
	freea_m(piece->edit_ranges, piece->edit_ranges_n, piece->edit_ranges_capacity);

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

	Segment* segment = aqp_calloc (1, sizeof *segment);
	*segment = (Segment) {
		.piece = piece,
		.row_type = {
			{
				0,
				sizeof(char)
			}
		},
		.rowdata = 0,
		.rowdata_capacity = 0,
		.rowdata_n = 0,
	};

	segment->row_size = 0;
	for (int i = 0; i < array_count_m(segment->row_type); i++) {
		segment->row_size += segment->row_type[i].size;
	}

	growa_m(64 * segment->row_size, segment->rowdata, segment->rowdata_n, segment->rowdata_capacity);

	inca_m(piece->segments, piece->segments_n, piece->segments_capacity);
	piece->segments[piece->segments_n - 1] = segment;

	return segment;
}

extern void aqp_delete_segment (Segment* segment)
{
	Piece* piece = segment->piece;
	for (int i = 0; i < piece->segments_n; i++) {
		if (piece->segments[i] == segment) {
			piece->segments[i] = NULL;
		}
	}

	delete_segment(segment);
}

extern SegmentEditionRange* aqp_edit_range (Segment* segment, int start, int end)
{
	SegmentEditionRange* range = aqp_calloc (1, sizeof *range);

	make_range (&range->view_range, segment, start, end);

	Piece* piece = segment->piece;
	inca_m(piece->edit_ranges, piece->edit_ranges_n, piece->edit_ranges_capacity);
	piece->edit_ranges[piece->edit_ranges_n - 1] = range;

	return range;
}

void aqp_field_write_int (SegmentEditionRange* range, int row, int field, int value)
{
	Segment* segment = range->view_range.segment;

	*field_pointer(&range->view_range, row, field) = value;
}

void aqp_commit_range (SegmentEditionRange* range)
{
	/* NOT IMPLEMENTED */
}

SegmentRange* aqp_read_range (Segment* segment, int start, int end)
{
	SegmentRange* range = aqp_calloc (1, sizeof *range);

	make_range (range, segment, start, end);

	Piece* piece = segment->piece;
	inca_m(piece->read_ranges, piece->read_ranges_n, piece->read_ranges_capacity);
	piece->read_ranges[piece->read_ranges_n - 1] = range;

	return range;
}

int aqp_field_read_int (SegmentRange* range, int row, int field)
{
	return *field_pointer(range, row, field);
}
