#ifndef aqp_h
#define aqp_h

/* 1- types */

/**
 * A Piece contains segments, their headers, their content (patterns)
 */
typedef struct { struct aqpPiece* impl; } aqp_piece_t;

/**
 * A segment is a sequence of simultaneously occuring control data.
 *
 * Its control data is described in a header, and one can edit and
 * navigate the control data with random accesses.
 */
typedef struct { struct aqpSegment* impl; } aqp_segment_t;

/**
 * A window over the control data, for reading.
 */
typedef struct { struct aqpSegmentRange* impl; } aqp_segment_range_t;

/**
 * A Window over the control data, for reading and writing.
 *
 * Modifications are only reflected to the segment once the range has
 * been disposed of.
 */
typedef struct { struct aqpSegmentEditionRange* impl; } aqp_segment_edition_range_t;

/* 2- operations */

/**
 * Allocate a new piece.
 */
extern aqp_piece_t aqp_piece_alloc ();

/**
 * Free all resources related to a piece.
 */
extern void aqp_piece_free (aqp_piece_t piece);

/**
 * Add a segment to the piece and returns it to you.
 */
extern aqp_segment_t aqp_segment_add (aqp_piece_t piece);

/**
 * Remove a segment from the piece.
 *
 * You should not use the pointer anymore past that point.
 */
extern void aqp_segment_remove (aqp_segment_t segment);

extern aqp_segment_edition_range_t aqp_segment_edit (aqp_segment_t segment, int start, int end);

extern aqp_segment_range_t aqp_segment_read (aqp_segment_t segment, int start, int end, int* data);

#endif
