from ga import *
import unittest

class TestCollisions(unittest.TestCase):

    def test_that_ball_staying_inside_bag_does_not_collide(self):
        height  =  5
        width  =  10
        x0 = 1
        x1 = 0
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
            theta = random.uniform(15, 180) * math.pi/180
            v = 1
            generation.append((theta, v))

        result = launch(generation, height, width)

        result = launch(generation, height, width)
        self.assertEqual(1, len(result))
        self.assertTrue(escaped(result[0]))

    def test_that_escaped_False_for_empty_path(self):
        self.assertFalse(escaped([]))

    def test_that_escaped_True_for_one_point_escaping(self):
        self.assertFalse(escaped([(0,0,True)]))



if __name__  == '__main__':
    unittest.main()
