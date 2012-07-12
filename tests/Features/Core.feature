Feature: Creation & Destruction

  As a user, I would like to be able to create and delete
  various pieces in order to group data into a collection.

Scenario: Creation of unique pieces
  Given I have created a piece
  And I have created another piece
  Then the first and second piece should be different

Scenario: Deletion without leaks
  Given I have created a piece
  Then memory may have been allocated
  When I delete the piece
  Then memory should not be allocated
