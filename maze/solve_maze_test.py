import numpy
import unittest
#from src import *
from src import solve_maze

class TestMazeSolve(unittest.TestCase):

    #'U' -> L, U, R, D
    #'R' -> U, R, D, L
    #'D' -> R, D, L, U
    #'L' -> D, L, U, R
    def test_that_potential_moves_for_U_are_LURD(self):
        moves = solve_maze.potential_moves('U')
        self.assertEqual(['L', 'U', 'R', 'D'], moves)

    def test_that_potential_moves_for_R_are_URDL(self):
        moves = solve_maze.potential_moves('R')
        self.assertEqual(['U', 'R', 'D', 'L'], moves)

    def test_that_potential_moves_for_D_are_RDLU(self):
        moves = solve_maze.potential_moves('D')
        self.assertEqual(['R', 'D', 'L', 'U'], moves)

    def test_that_potential_moves_for_L_are_DLUR(self):
        moves = solve_maze.potential_moves('L')
        self.assertEqual(['D', 'L', 'U', 'R'], moves)

    def test_that_next_move_for_maze_without_solution_is_reported(self):
        maze = numpy.array([[1, 1, 1, 1, 1],
                            [1, 1, 0, 1, 1],
                            [1, 1, 1, 1, 1]])
        self.assertRaises(Exception, solve_maze.next_move, ('U', 1, 2, maze))
        self.assertRaises(Exception, solve_maze.next_move, ('R', 1, 2, maze))
        self.assertRaises(Exception, solve_maze.next_move, ('D', 1, 2, maze))
        self.assertRaises(Exception, solve_maze.next_move, ('L', 1, 2, maze))

    def test_that_next_move_when_facing_up_without_wall_up_is_up(self):
        maze = numpy.array([[1, 1, 0, 1, 1],
                            [1, 1, 0, 1, 1],
                            [1, 1, 1, 1, 1]])
        facing, row, col = solve_maze.next_move('U', 1, 2, maze)
        self.assertEqual('U', facing)
        self.assertEqual(0, row)
        self.assertEqual(2, col)

    def test_that_next_move_when_facing_up_with_wall_up_and_no_wall_right_is_right(self):
        maze = numpy.array([[1, 1, 1, 1, 1],
                            [1, 1, 0, 0, 1],
                            [1, 1, 1, 1, 1]])
	facing, row, col = solve_maze.next_move('U', 1, 2, maze)
        self.assertEqual('R', facing)
        self.assertEqual(1, row)
        self.assertEqual(3, col)

    def test_that_next_move_when_facing_up_with_wall_up_and_wall_right_and_no_wall_down_is_down(self):
        maze = numpy.array([[1, 1, 1, 1, 1],
                            [1, 1, 0, 1, 1],
                            [1, 1, 0, 1, 1]])
	facing, row, col = solve_maze.next_move('U', 1, 2, maze)
        self.assertEqual('D', facing)
        self.assertEqual(2, row)
        self.assertEqual(2, col)

    def test_that_next_move_when_facing_up_with_wall_up_right_and_down_no_wall_left_is_left(self):
        maze = numpy.array([[1, 1, 1, 1, 1],
                            [1, 0, 0, 1, 1],
                            [1, 1, 1, 1, 1]])
	facing, row, col = solve_maze.next_move('U', 1, 2, maze)
        self.assertEqual('L', facing)
        self.assertEqual(1, row)
        self.assertEqual(1, col)

    def test_that_next_move_when_facing_right_without_wall_right_is_right(self):
        maze = numpy.array([[1, 1, 1, 1, 1],
                            [1, 1, 0, 0, 1],
                            [1, 1, 1, 1, 1]])
	facing, row, col = solve_maze.next_move('R', 1, 2, maze)
        self.assertEqual('R', facing)
        self.assertEqual(1, row)
        self.assertEqual(3, col)

    def test_that_next_move_when_facing_right_with_wall_right_and_no_wall_down_is_down(self):
        maze = numpy.array([[1, 1, 1, 1, 1],
                            [1, 1, 0, 1, 1],
                            [1, 1, 0, 1, 1]])
	facing, row, col = solve_maze.next_move('R', 1, 2, maze)
        self.assertEqual('D', facing)
        self.assertEqual(2, row)
        self.assertEqual(2, col)

    def test_that_next_move_when_facing_right_with_wall_right_and_wall_down_and_no_wall_left_is_left(self):
        maze = numpy.array([[1, 1, 1, 1, 1],
                            [1, 0, 0, 1, 1],
                            [1, 1, 1, 1, 1]])
	facing, row, col = solve_maze.next_move('R', 1, 2, maze)
        self.assertEqual('L', facing)
        self.assertEqual(1, row)
        self.assertEqual(1, col)

    def test_that_next_move_when_facing_right_with_wall_right_down_and_left_no_wall_up_is_up(self):
        maze = numpy.array([[1, 1, 0, 1, 1],
                            [1, 1, 0, 1, 1],
                            [1, 1, 1, 1, 1]])
	facing, row, col = solve_maze.next_move('R', 1, 2, maze)
        self.assertEqual('U', facing)
        self.assertEqual(0, row)
        self.assertEqual(2, col)

    def test_that_next_move_when_facing_down_without_wall_down_is_down(self):
        maze = numpy.array([[1, 1, 1, 1, 1],
                            [1, 1, 0, 1, 1],
                            [1, 1, 0, 1, 1]])
	facing, row, col = solve_maze.next_move('D', 1, 2, maze)
        self.assertEqual('D', facing)
        self.assertEqual(2, row)
        self.assertEqual(2, col)

    def test_that_next_move_when_facing_down_with_wall_down_and_no_wall_left_is_left(self):
        maze = numpy.array([[1, 1, 1, 1, 1],
                            [1, 0, 0, 1, 1],
                            [1, 1, 1, 1, 1]])
	facing, row, col = solve_maze.next_move('D', 1, 2, maze)
        self.assertEqual('L', facing)
        self.assertEqual(1, row)
        self.assertEqual(1, col)

    def test_that_next_move_when_facing_down_with_wall_down_and_wall_left_and_no_wall_up_is_up(self):
        maze = numpy.array([[1, 1, 0, 1, 1],
                            [1, 1, 0, 1, 1],
                            [1, 1, 1, 1, 1]])
	facing, row, col = solve_maze.next_move('D', 1, 2, maze)
        self.assertEqual('U', facing)
        self.assertEqual(0, row)
        self.assertEqual(2, col)

    def test_that_next_move_when_facing_down_with_wall_down_left_and_up_no_wall_right_is_right(self):
        maze = numpy.array([[1, 1, 1, 1, 1],
                            [1, 1, 0, 0, 1],
                            [1, 1, 1, 1, 1]])
	facing, row, col = solve_maze.next_move('D', 1, 2, maze)
        self.assertEqual('R', facing)
        self.assertEqual(1, row)
        self.assertEqual(3, col)

    def test_that_next_move_when_facing_left_without_wall_down_is_left(self):
        maze = numpy.array([[1, 1, 1, 1, 1],
                            [1, 0, 0, 1, 1],
                            [1, 1, 1, 1, 1]])
	facing, row, col = solve_maze.next_move('L', 1, 2, maze)
        self.assertEqual('L', facing)
        self.assertEqual(1, row)
        self.assertEqual(1, col)

    def test_that_next_move_when_facing_left_with_wall_left_and_no_wall_up_is_up(self):
        maze = numpy.array([[1, 1, 0, 1, 1],
                            [1, 1, 0, 1, 1],
                            [1, 1, 1, 1, 1]])
	facing, row, col = solve_maze.next_move('L', 1, 2, maze)
        self.assertEqual('U', facing)
        self.assertEqual(0, row)
        self.assertEqual(2, col)

    def test_that_next_move_when_facing_left_with_wall_left_and_wall_up_and_no_wall_right_is_right(self):
        maze = numpy.array([[1, 1, 1, 1, 1],
                            [1, 1, 0, 0, 1],
                            [1, 1, 1, 1, 1]])
	facing, row, col = solve_maze.next_move('L', 1, 2, maze)
        self.assertEqual('R', facing)
        self.assertEqual(1, row)
        self.assertEqual(3, col)

    def test_that_next_move_when_facing_left_with_wall_left_up_and_right_no_wall_down_is_down(self):
        maze = numpy.array([[1, 1, 1, 1, 1],
                            [1, 1, 0, 1, 1],
                            [1, 1, 0, 1, 1]])
	facing, row, col = solve_maze.next_move('L', 1, 2, maze)
        self.assertEqual('D', facing)
        self.assertEqual(2, row)
        self.assertEqual(2, col)


    def test_that_maze_without_entry_at_top_is_reported(self):
        maze = numpy.array([[1, 1, 1, 1, 1],
                            [1, 0, 0, 0, 1],
                            [1, 1, 1, 0, 1]])
        self.assertRaises(Exception, solve_maze.wall_follow, maze)

    def test_that_maze_without_entry_at_bottom_is_reported(self):
        maze = numpy.array([[1, 0, 1, 1, 1],
                            [1, 0, 0, 0, 1],
                            [1, 1, 1, 1, 1]])
        self.assertRaises(Exception, solve_maze.wall_follow, maze)

    def test_that_maze_without_solution_is_reported(self):
        maze = numpy.array([[1, 0, 1, 1, 1],
                            [1, 0, 1, 0, 1],
                            [1, 1, 1, 0, 1]])
        self.assertRaises(Exception, solve_maze.wall_follow, maze)

    #
    # X XXX
    # X   X
    # XXX X
    #

    def test_left_wall_follow_starts_at_the_top(self):
        maze = numpy.array([[1, 0, 1, 1, 1],
                            [1, 0, 0, 0, 1],
                            [1, 1, 1, 0, 1]])
        path = solve_maze.wall_follow(maze)
        self.assertEqual((0, 1), path[0])

    def test_left_wall_follow_ends_at_the_bottom(self):
        maze = numpy.array([[1, 0, 1, 1, 1],
                            [1, 0, 0, 0, 1],
                            [1, 1, 1, 0, 1]])
        path = solve_maze.wall_follow(maze)
        self.assertEqual((2, 3), path[-1])

    def test_left_wall_follow_with_one_possible_path_along_top_follows_that_path(self):
        maze = numpy.array([[1, 0, 0, 0, 1],
                            [1, 1, 1, 0, 1],
                            [1, 1, 1, 0, 1]])
        path = solve_maze.wall_follow(maze)
        self.assertEqual((0, 1), path[0])
        self.assertEqual((0, 2), path[1])
        self.assertEqual((0, 3), path[2])
        self.assertEqual((1, 3), path[3])
        self.assertEqual((2, 3), path[4])

    def test_left_wall_follow_with_one_possible_path_through_middle_follows_that_path(self):
        maze = numpy.array([[1, 0, 1, 1, 1],
                            [1, 0, 0, 0, 1],
                            [1, 1, 1, 0, 1]])
        path = solve_maze.wall_follow(maze)
        self.assertEqual((0, 1), path[0])
        self.assertEqual((1, 1), path[1])
        self.assertEqual((1, 2), path[2])
        self.assertEqual((1, 3), path[3])
        self.assertEqual((2, 3), path[4])

    def test_left_wall_follow_with_one_possible_path_down_bottom_follows_that_path(self):
        maze = numpy.array([[1, 0, 1, 1, 1],
                            [1, 0, 1, 1, 1],
                            [1, 0, 1, 1, 1]])
        path = solve_maze.wall_follow(maze)
        self.assertEqual((0, 1), path[0])
        self.assertEqual((1, 1), path[1])
        self.assertEqual((2, 1), path[2])

    def test_left_wall_follow_with_dead_end_to_right_finds_possible_path(self):
        maze = numpy.array([[1, 0, 1, 1, 1],
                            [1, 0, 0, 0, 1],
                            [1, 0, 1, 1, 1]])
        path = solve_maze.wall_follow(maze)
        self.assertEqual((0, 1), path[0])
        self.assertEqual((1, 1), path[1])
        self.assertEqual((1, 2), path[2])
        self.assertEqual((1, 3), path[3])
        self.assertEqual((1, 2), path[4])
        self.assertEqual((1, 1), path[5])
        self.assertEqual((2, 1), path[6])

    def test_left_wall_follow_with_dead_end_to_left_finds_possible_path(self):
        maze = numpy.array([[1, 1, 0, 1, 1],
                            [1, 0, 0, 0, 1],
                            [1, 1, 1, 0, 1]])
        path = solve_maze.wall_follow(maze)
        self.assertEqual((0, 2), path[0])
        self.assertEqual((1, 2), path[1])
        self.assertEqual((1, 3), path[2])
        self.assertEqual((2, 3), path[3])

    def test_left_wall_follow_with_dead_ends_finds_possible_path(self):
        maze = numpy.array([[1, 1, 0, 1, 1],
                            [1, 1, 0, 1, 1],
                            [1, 0, 0, 0, 1],
                            [1, 1, 1, 0, 1]])
        path = solve_maze.wall_follow(maze)
        self.assertEqual((0, 2), path[0])
        self.assertEqual((1, 2), path[1])
        self.assertEqual((2, 2), path[2])
        self.assertEqual((2, 3), path[3])
        self.assertEqual((3, 3), path[4])

    def test_left_wall_follow_with_dead_ends_and_spiralling_path_finds_possible_path(self):
        maze = numpy.array([[1, 0, 1, 0, 0, 0],
                            [1, 0, 1, 0, 1, 0],
                            [1, 0, 1, 0, 1, 0],
                            [1, 0, 0, 0, 0, 0],
                            [1, 1, 1, 0, 1, 1]])
        path = solve_maze.wall_follow(maze)
        self.assertEqual((0, 1), path[0])
        self.assertEqual((4, 3), path[-1])

    # add some impossible ones
    #   check I've got tests for each direction and change of direction


if __name__ == '__main__':
    unittest.main()
    #to run one test
    #>python.exe -m unittest generate_maze_test.TestMazeSolve.test_left_wall_follow_with_one_possible_path_follows_that_path
    #
    # Look at http://avian.netne.net/blog/?p=93
