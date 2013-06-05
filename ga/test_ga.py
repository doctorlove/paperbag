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
        self.assertFalse(collides(x0, y0, x1, y1, height, width))

    def test_that_ball_staying_outside_bag_does_not_collide(self):
        height  =  5
        width  =  10
        x0 = width + 1
        x1 = width + 2
        y0 = 0
        y1 = 0
        self.assertFalse(collides(x0, y0, x1, y1, height, width))

    def test_that_ball_crossing_bag_to_right_does_collide(self):
        height  =  5
        width  =  10
        x0 = width - 1
        x1 = width + 2
        y = height - 2
        self.assertEqual(y, collision_point(x0, y, x1, y, height, width))
        self.assertTrue(collides(x0, y, x1, y, height, width))

    def test_that_ball_going_over_bag_to_right_does_not_collide(self):
        height  =  5
        width  =  10
        x0 = width - 2
        x1 = width + 2
        y0 = height - 2
        y1 = height + 4
        self.assertEqual(height + 1, collision_point(x0, y0, x1, y1, height, width))
        self.assertFalse(collides(x0, y0, x1, y1, height, width))


class TestLaunch(unittest.TestCase):
    def test_that_launch_with_v_is_zero_means_none_escaped(self):
        items = 10
        height = 5
        width = 10

        generation = []
        for i in range(items):
            theta = random.uniform(15, 180) * math.pi/180
            v = 0
        generation.append((theta, v))

        result = launch(generation, height, width)

        for res in result:
            self.assertFalse(res[2])


if __name__  == '__main__':
    unittest.main()
