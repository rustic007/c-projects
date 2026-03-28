#include "chess.h"
#include <stdio.h>
// which positions quens can attack: if in the same row or coloum, and one
// special positions invalid postionst: if the rows and cols are more than 7,
// and the quens mustn't be in the same positions if one situations meets then
// can attack positions

attack_status_t can_attack(position_t queen_1, position_t queen_2) {
  if (queen_1.column > 7 || queen_2.column > 7 || queen_1.row > 7 ||
      queen_2.row > 7) {
    return INVALID_POSITION;
  }

  if (queen_1.column == queen_2.column && queen_1.row == queen_2.row) {
    return INVALID_POSITION;
  }

  if (queen_1.row == queen_2.row) {
    return CAN_ATTACK;
  }

  if (queen_1.column == queen_2.column) {
    return CAN_ATTACK;
  }

  int row_dif = queen_1.row - queen_2.row;
  int col_dif = queen_1.column - queen_2.column;

  if (row_dif == col_dif || row_dif == -(col_dif)) {
    return CAN_ATTACK;
  }

  return CAN_NOT_ATTACK;
}

void print_result(const char *test_name, attack_status_t result,
                  attack_status_t expected) {
  if (result == expected) {
    printf("✓ %s: PASSED\n", test_name);
  } else {
    printf("✗ %s: FAILED (got %d, expected %d)\n", test_name, result, expected);
  }
}

int main() {
  printf("=== Chess Queen Attack Tests ===\n\n");

  // Test 1: Same row
  position_t q1 = {0, 0};
  position_t q2 = {0, 5};
  attack_status_t result = can_attack(q1, q2);
  print_result("Same row", result, CAN_ATTACK);

  // Test 2: Same column
  q1 = (position_t){2, 3};
  q2 = (position_t){5, 3};
  result = can_attack(q1, q2);
  print_result("Same column", result, CAN_ATTACK);

  // Test 3: Same diagonal
  q1 = (position_t){2, 3};
  q2 = (position_t){5, 6};
  result = can_attack(q1, q2);
  print_result("Same diagonal", result, CAN_ATTACK);

  // Test 4: Can't attack
  q1 = (position_t){0, 0};
  q2 = (position_t){2, 3};
  result = can_attack(q1, q2);
  print_result("Can't attack", result, CAN_NOT_ATTACK);

  // Test 5: Invalid position (out of bounds)
  q1 = (position_t){8, 5};
  q2 = (position_t){0, 0};
  result = can_attack(q1, q2);
  print_result("Invalid position (out of bounds)", result, INVALID_POSITION);

  // Test 6: Invalid position (same position)
  q1 = (position_t){3, 3};
  q2 = (position_t){3, 3};
  result = can_attack(q1, q2);
  print_result("Invalid position (same spot)", result, INVALID_POSITION);

  printf("\n=== All Tests Complete ===\n");
  return 0;
}
