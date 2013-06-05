import math
import random

def collision_point(x0, y0, x1, y1, height, width):
    x = 0.0
    if x1 > width: x = width
    m = (y1 - y0)/(1.0*x1 - x0)
    y = y0 + m*(x-x0)
    return y

def collides(x0, y0, x1, y1, height, width):
    if x0 >= 0 and x0 <= width and x1 >= 0 and x1 <= width:
        return False
    if (x0 <= 0 and x1 <= 0) or (x0 >= width and x1 >= width):
        return False
    y = collision_point(x0, y0, x1, y1, height, width)
    return y < height

def launch(generation, height, width):
    result = []
    for (theta, v) in generation:
        print theta, v
        for t in [i*0.2 for i in range(20)]:
            x = width/2.0 + v * t * math.cos(theta)
            y = v * t * math.sin(theta) - 0.5 * 9.81 * i * i
            if y < 0: y = 0
            #print '\t',x,y
            escaped = False
            result.append((x, y, escaped))
    return result

if __name__ == "__main__":
    epochs = 25
    items = 10
    height = 5
    width = 10

    generation = []
    for i in range(items):
        theta = random.uniform(15, 180) * math.pi/180
        v = random.uniform(2, 20)
        generation.append((theta, v))

    escaped = launch(generation, height, width)

    for i in range(1, epochs):
        launch(generation, height, width)


