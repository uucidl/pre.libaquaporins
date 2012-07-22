Feature: Creation & Destruction

  As a user, I would like to be able to create and delete
  various pieces in order to group data into a collection.

Scenario: Creation of unique pieces
  Given I have created a piece
  And I have created another piece
  Then the first and second piece should be different

Scenario: Deletion without leaks
  Given I want to monitor memory
    And I have created a piece
    And a segment with tracks:
  | track_name | signature |
  | A          | uint8     |
   Then memory should have been allocated
   When I delete the piece
   Then memory should not be allocated

Scenario: Creation, Deletion of segments
  Given I want to monitor memory
    And I have created a piece
    And a segment with tracks:
  | track_name | signature |
  | A          | uint8     |
   When I delete the segment
    And I delete the piece
   Then memory should not be allocated

Scenario: A simple flat piece
  Given I want to monitor memory
    And a segment with tracks:
  | track_name | signature |
  | A          | int8      |

    And the track 'A' contains:
| field 0 |
|       2 |
|       3 |
|       4 |
|     nil |
|      28 |
   Then the checksum of track 'A' should be 37
   When I delete the piece
   Then memory should not be allocated
