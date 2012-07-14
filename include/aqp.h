#ifndef aqp_h
#define aqp_h

/* 1- types */

/**
 * A Piece contains segments, their headers, their content (patterns)
 */
typedef struct aqpPiece aqpPiece;

/**
 * A segment is a sequence of simultaneously occuring control data.
 *
 * Its control data is described in a header, and one can edit and
 * navigate the control data with random accesses.
 */
typedef struct aqpSegment aqpSegment;

/**
 * A window over the control data, for reads
 */
typedef struct aqpSegmentRange aqpSegmentRange;

/**
 * A Window over the control data, for read/writes
 */
typedef struct aqpSegmentEditionRange aqpSegmentEditionRange;

/* 2- operations */

/**
 * Allocate a new piece.
 */
extern struct aqpPiece* aqp_piece_alloc ();

/**
 * Free all resources related to a piece.
 */
extern void aqp_piece_free (aqpPiece* piece);

extern aqpSegment* aqp_segment_alloc (aqpPiece* piece);
extern void aqp_segment_free (aqpSegment* segment);

extern aqpSegmentEditionRange* aqp_segment_edit (aqpSegment* segment, int start, int end);
extern aqpSegmentRange* aqp_segment_read (aqpSegment* segment, int start, int end, int* data);

#endif
