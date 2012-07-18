#ifndef aqp_h
#define aqp_h

/* 1- types */

/**
 * A Piece contains segments, their headers, their content (patterns)
 */
typedef struct aqpPiece* aqp_piece_t;

/**
 * A segment is a sequence of simultaneously occuring control data.
 *
 * Its control data is described in a header, and one can edit and
 * navigate the control data with random accesses.
 */
typedef struct aqpSegment* aqp_segment_t;

/**
 * A window over the control data, for reading.
 */
typedef struct aqpSegmentRange* aqp_segment_range_t;

/**
 * A Window over the control data, for reading and writing.
 *
 * Modifications are only reflected to the segment once the range has
 * been disposed of.
 */
typedef struct aqpSegmentEditionRange* aqp_segment_edition_range_t;

/* 2- operations */

/**
 * Allocate a new piece.
 */
aqp_piece_t aqp_new_piece (void);

/**
 * Free all resources related to a piece.
 */
void aqp_delete_piece (aqp_piece_t piece);

/**
 * Add a segment to the piece and returns it to you.
 */
aqp_segment_t aqp_new_segment (aqp_piece_t piece);

/**
 * Remove a segment from the piece.
 */
void aqp_delete_segment (aqp_segment_t segment);

/**
 * Starts editing a segment by obtaining an edition range.
 */
aqp_segment_edition_range_t aqp_edit_range (aqp_segment_t segment, int start, int end);

/**
 * Update the segment with operations made to the range.
 *
 * Modifications are visible to readers.
 */
void aqp_commit_range (aqp_segment_edition_range_t range);

/**
 * Starts reading a segment by obtaining a reading range.
 */
aqp_segment_range_t aqp_read_range (aqp_segment_t segment, int start, int end, int* data);

#endif
