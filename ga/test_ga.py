import pdb
from ga import *
import math
import unittest

class TestCumulativeProbabilites(unittest.TestCase):
    def test_that_one_result_gives_height(self):
        res = cumulative_probabilities([(10,15)])
        self.assertEqual(1, len(res))
        self.assertEqual(15, res[0])

    def test_that_two_results_gives_total_heights(self):
        res = cumulative_probabilities([(5,0), (10,15), (15, 30)])
        self.assertEqual(3, len(res))
        self.assertEqual(0, res[0])
        self.assertEqual(15, res[1])
        self.assertEqual(45, res[2])

    def test_that_one_choice_is_chosen(self):
        res = cumulative_probabilities([(10,15)])
        self.assertEqual(0, choose(res))

    def test_that_get_choices_returns_full_generation(self):
	results = [(5,10), (10,15), (15, 30)]
        choices = selection(results, 42) # 42 some width
	self.assertEqual(len(choices), len(results))

class testMutation(unittest.TestCase):
    def test_that_mutation_leaves_number_of_solutions_unaffected(self):
	generation = [(5,0), (10,15), (15, 30)]
        mutate(generation)
        self.assertEqual(len(generation), 3)


class TestHitHeight(unittest.TestCase):
    def test_that_angle_of_zero_gives_hit_height_of_zero(self):
        width = 10
        x, y = hit_coordinate(0, 10, width)
        self.assertEqual(width, x)
        self.assertEqual(0.0, y)

    def test_that_angle_of_Pi_gives_hit_height_of_zero(self):
        width = 10
        x, y = hit_coordinate(math.pi, 10, width)
        self.assertEqual(0.0, x)
        self.assertEqual(0.0, y)

    def test_that_angle_of_50_degrees_going_slow_gives_hit_height_of_less_than_bag_height(self):
        width = 10
        x, y = hit_coordinate(50*math.pi/180, 10, width)
        self.assertEqual(width, x)
        self.assertTrue(y < 5.0)

    def test_that_angle_of_50_degrees_fast_gives_hit_height_of_more_than_bag_height(self):
        width = 10
        x, y = hit_coordinate(50*math.pi/180, 50, width)
        self.assertEqual(width, x)
        self.assertTrue(y > 5.0)

    def test_that_angle_of_130_degrees_going_slow_gives_hit_height_of_less_than_bag_height(self):
        width = 10
        x, y = hit_coordinate(130*math.pi/180, 10, width)
        self.assertEqual(0.0, x)
        self.assertTrue(y < 5.0)

    def test_that_angle_of_3Pi_by_4_going_fast_gives_hit_height_of_more_than_bag_height(self):
        width = 10
        x, y = hit_coordinate(130*math.pi/180, 50, width)
        self.assertEqual(0.0, x)
        self.assertTrue(y > 5.0)


class TestLaunch(unittest.TestCase):
    def test_that_launch_returns_some_points(self):
        width = 10
        height = 5
	generation = [(1, 10)]
        result = launch(generation, height, width)
        self.assertTrue(len(generation)>0)


class TestEscape(unittest.TestCase):
    def test_going_right_fast_enough_esacpes(self):
        width = 10
        height = 5
	theta  = 1.1323018699
	v  = 17.6708379946
        self.assertTrue(esacped(theta, v, width, height))

    def test_going_left_fast_enough_esacpes(self):
        width = 10
        height = 5
	theta  = math.pi -1.1323018699
	v  = 17.6708379946
        self.assertTrue(esacped(theta, v, width, height))

    def test_that_projectile_going_up_does_not_escape(self):
        width = 10
        height = 5
	theta  = math.pi/2
	v  = 17.6708379946
        self.assertFalse(esacped(theta, v, width, height))


if __name__  == '__main__':
    unittest.main()
