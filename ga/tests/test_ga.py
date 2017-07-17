import pdb
from ga.ga0 import *
import math
import unittest

class TestCumulativeProbabilites(unittest.TestCase):
  def test_that_one_result_gives_height(self):
    res = cumulative_probabilities([15])
    self.assertEqual(1, len(res))
    self.assertEqual(15, res[0])

  def test_that_two_results_gives_total_heights(self):
    res = cumulative_probabilities([0, 15, 30])
    self.assertEqual(3, len(res))
    self.assertEqual(0, res[0])
    self.assertEqual(15, res[1])
    self.assertEqual(45, res[2])

  def test_that_one_choice_is_chosen(self):
    res = cumulative_probabilities([15])
    self.assertEqual(0, choose(res))

  def test_that_get_choices_returns_full_generation(self):
    generation = [(5,1), (10,15), (15, 30)]
    choices = selection(generation, 42) # 42 some width
    self.assertEqual(len(choices), len(generation))

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

  def test_going_right_but_not_fast_enough_hits_below_top_of_bag(self):
    width = 10
    x, y = hit_coordinate(math.pi/4, 15, width)
    self.assertLess(y, 5)

  def test_going_left_but_not_fast_enough_hits_below_top_of_bag(self):
    width = 10
    x, y = hit_coordinate(3*math.pi/4, 15, width)
    self.assertLess(y, 5)

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
    self.assertGreater(y, 5.0)


class TestLaunch(unittest.TestCase):
  def test_that_launch_returns_some_points(self):
    width = 10
    height = 5
    generation = [(1, 10)]
    result = launch(generation, height, width)
    self.assertEqual(len(result), len(generation))
    self.assertGreater(len(result[0]), 0)

  def test_that_launch_stops_at_edge_of_bag_if_it_hits_the_bag(self):
    width = 10
    height = 5
    generation = [(math.pi/4, 15)]
    result = launch(generation, height, width)
    self.assertTrue(len(result)>0)
    self.assertEqual(result[0][-1][0], width)

  def test_that_launch_stops_at_edge_of_bag_if_it_hits_the_bag_between_steps(self):
    width = 10
    height = 5
    generation = [(math.pi/3, 8)]
    result = launch(generation, height, width)
    self.assertTrue(len(result)>0)
    self.assertEqual(result[0][-1][0], width)


  def test_that_launch_going_over_bag_keeps_going(self):
    width = 10
    height = 5
    generation = [(math.pi/3, 15)]
    result = launch(generation, height, width)
    self.assertTrue(len(result)>0)
    self.assertGreater(result[0][-1][0], width)

class TestEscape(unittest.TestCase):
  def test_going_right_but_not_fast_enough_hits_below_top_of_bag(self):
    width = 10
    height = 5
    theta = math.pi/4
    v = 15
    self.assertFalse(escaped(theta, v, width, height))

  def test_going_right_fast_enough_esacpes(self):
    width = 10
    height = 5
    theta  = 1.1323018699
    v  = 17.6708379946
    self.assertTrue(escaped(theta, v, width, height))

  def test_going_right_but_not_fast_enough_hits_below_top_of_bag(self):
    width = 10
    height = 5
    theta = 3*math.pi/4
    v = 15
    self.assertFalse(escaped(theta, v, width, height))

  def test_going_left_fast_enough_esacpes(self):
    width = 10
    height = 5
    theta  = math.pi -1.1323018699
    v  = 17.6708379946
    self.assertTrue(escaped(theta, v, width, height))

  def test_that_projectile_going_up_does_not_escape(self):
    width = 10
    height = 5
    theta  = math.pi/2
    v  = 17.6708379946
    self.assertFalse(escaped(theta, v, width, height))


if __name__  == '__main__':
  unittest.main()
