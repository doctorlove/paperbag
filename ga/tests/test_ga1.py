import pdb
from ga.ga1 import *
import unittest

class TestHitHeight(unittest.TestCase):
    def test_that_angle_of_zero_gives_hit_height_of_zero(self):
        self.assertEqual(0.0, hit_height(0,10))


class TestChoices(unittest.TestCase):
    def test_that_get_choices_returns_escaped_items(self):
        results = [[(5, 0, True)], [(10, 10, False)], [(15, 20, False)]]
        gen = [(0,0), (0,0), (0,0)]
        choices = get_choices(gen, 10, 5, results)
        self.assertEqual(len(choices), 2)

    def test_that_get_choices_returns_full_generation(self):
        results = [[(5,0, True)], [(10, 0, True)], [(15, 0, True)]]
        gen = [(0,0), (0,0), (0,0)]
        choices = get_choices(gen, 10, 5, results)
        self.assertEqual(len(choices), 0)


class TestInitialise(unittest. TestCase):
    def test_that_init_random_generation_gives_number_of_items_requested(self):
        items = 5
        gen = init_random_generation(items)
        self.assertEqual(len(gen), items)

class TestCollisions(unittest.TestCase):

    def test_that_ball_staying_inside_bag_does_not_collide(self):
        height  =  5
        width  =  10
        x0 = 2
        x1 = 1
        y0 = 1
        y1 = 1
        x_hit, y_hit, hits = collides(x0, y0, x1, y1, height, width)
        self.assertFalse(hits)

    def test_that_ball_staying_outside_bag_does_not_collide(self):
        height  =  5
        width  =  10
        x0 = width + 1
        x1 = width + 2
        y0 = 0
        y1 = 0
        x_hit, y_hit, hits = collides(x0, y0, x1, y1, height, width)
        self.assertFalse(hits)

    def test_that_ball_crossing_bag_to_right_does_collide(self):
        height  =  5
        width  =  10
        x0 = width - 1
        x1 = width + 2
        y = height - 2
        x_hit, y_hit, hits = collides(x0, y, x1, y, height, width)
        self.assertEqual(width, x_hit)
        self.assertEqual(y, y_hit)
        self.assertTrue(hits)

    def test_that_ball_going_over_bag_to_right_does_not_collide(self):
        height  =  5
        width  =  10
        x0 = width - 2
        x1 = width + 2
        y0 = height - 2
        y1 = height + 4
        x_hit, y_hit, hits = collides(x0, y0, x1, y1, height, width)
        self.assertEqual(width, x_hit)
        self.assertEqual(height + 1, y_hit)
        self.assertFalse(hits)

    def test_that_ball_crossing_bag_to_left_does_collide(self):
        height  =  5
        width  =  10
        x0 = 1
        x1 = -2
        y = height - 2
        x_hit, y_hit, hits = collides(x0, y, x1, y, height, width)
        self.assertEqual(0.0, x_hit)
        self.assertEqual(y, y_hit)
        self.assertTrue(hits)

    def test_that_ball_going_over_bag_to_left_does_not_collide(self):
        height  =  5
        width  =  10
        x0 = 2
        x1 = -2
        y0 = height - 2
        y1 = height + 4
        x_hit, y_hit, hits = collides(x0, y0, x1, y1, height, width)
        self.assertEqual(0.0, x_hit)
        self.assertEqual(height + 1, y_hit)
        self.assertFalse(hits)


class TestLaunch(unittest.TestCase):
    def test_that_launch_with_v_is_zero_means_none_escaped(self):
        items = 1
        height = 5
        width = 10

        generation = []
        for i in range(items):
            theta = random.uniform(15, 180) * math.pi/180
            v = 0
            generation.append((theta, v))

        result = launch(generation, height, width)
        self.assertEqual(1, len(result))
        for res in result[0]:
            self.assertEqual(0, res[1])

    def test_that_launch_with_height_and_width_is_zero_means_all_escaped(self):
        items = 1
        height = 0
        width = 0

        generation = []
        for i in range(items):
            theta = random.uniform(15, 180) * math.pi/90
            v = 1
            generation.append((theta, v))

        result = launch(generation, height, width)
        self.assertEqual(1, len(result))
        self.assertTrue(escaped(height, width, result[0]))

    def test_that_projectile_going_up_does_not_escape(self):
        height = 5
        width = 10
        generation = [(1.57, 6.5978380724998082)] 
        result = launch(generation, height, width)
        self.assertEqual(len(generation), len(result))
        self.assertFalse(0, escaped(height, width, result[0]))

    def test_that_projectile_going_through_origin_does_not_escape(self):
        height = 5
        width = 10
        x0 = 0.13608130789921624
        y0 = 0
        x = 0.0
        y = 0.0
        self.assertTrue(collides(x0, y0, x, y, height, width))
            
    def test_that_projectile_not_making_the_edge_does_not_escape(self):
        height = 5
        width = 10
        path = [(2.4868625562327922, 0, False), (2.3297914659973418, 0, False), (2.1727203757618918, 0, False), (2.015649285526441, 0, False)] 
        self.assertFalse(escaped(height, width, path)) 

    def test_that_escaped_False_for_empty_path(self):
        height = 5
        width = 10
        self.assertFalse(escaped(height, width, []))

    def test_that_escaped_True_for_one_point_escaping(self):
        height = 5
        width = 10
        self.assertTrue(escaped(height, width, [(0,0,False)]))#bool means if it hit or not

    def test_that_escaped_is_false_when_last_item_says_hit(self):
        height = 5
        width = 10
        res1 = [(5.0, 0, False), (6.6641708557952946, 2.4354348728539814, False), (8.3283417115905891, 4.4784697457079634, False), (9.9925125673858837, 6.1291046185619455, False), (11.656683423181178, 7.3873394914159256, False), (12, 4.9593311501214679, True)] 
        self.assertFalse(escaped(height, width, res1))
        res2 = [(5.0, 0, False), (2.4897309478420744, 0.11915097568091043, False), (0.0, 0.0, True)]
        self.assertFalse(escaped(height, width, res2))


if __name__  == '__main__':
    unittest.main()
