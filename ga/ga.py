import pdb

import math
import matplotlib.pyplot as plt
import random

def collision_point(x0, y0, x1, y1, height, width):
    if x0 == x1: return x0, y0
    x = 0.0
    if x1 > width: x = width
    m = (y1 - y0)/(1.0*x1 - x0)
    y = y0 + m*(x-x0)
    return x, y

def collides(x0, y0, x1, y1, height, width):
    x, y = collision_point(x0, y0, x1, y1, height, width)
    if x0 >= 0 and x0 <= width and x1 >= 0 and x1 <= width:
        return x, y, False
    if (x0 <= 0 and x1 <= 0) or (x0 >= width and x1 >= width):
        return x, y, False
    return x, y, y < height

def escaped(height, width, path):
    if len(path) == 0: return False
    if path[-1][0]>0 and path[-1][0]<width: return False
    return path[-1][2] == False

def launch(generation, height, width):
    results = []
    for (theta, v) in generation:
        result = []
        x = width/2.0
        y = 0
        result.append((x, y, False))
        for i in xrange(1, 20):
            previous_x = x
            previous_y = y
            t  = i*0.2
            x = width/2.0 + v * t * math.cos(theta)
            y = v * t * math.sin(theta) - 0.5 * 9.81 * t * t
            if y < 0: y = 0
            #pdb.set_trace()
            x_hit, y_hit, hits = collides(previous_x, previous_y, x, y, height, width)
            if hits:
                result.append((x_hit, y_hit, hits))
                break
            else:
                result.append((x, y, False))
        results.append(result)
    return results

def crossover(generation, results):
    next_generation = []
    for i in range(0, len(generation)):
        next_generation.append(generation[i])
    return next_generation

def mutate(generation):
    for i in range(len(generation)):
        (theta, v) = generation[i]
        if random.random() < 0.1:
            theta += random.uniform(-10, 10)
        if random.random() < 0.1:
            v *= random.uniform(0.9, 1.1)
        generation[i] = (theta, v)

def graph_interpolation(generation, results, height, width):
    fig = plt.figure()
    ax = fig.add_subplot(111)
    rect = plt.Rectangle((0, 0), width, height, facecolor='brown')
    ax.add_patch(rect)
    
    for gen in generation:
        print gen
    for res in results:
        print "res", res, escaped(height, width, res)
        print "escaped", escaped(height, width, res)
        x = [i[0] for i in res]
        y = [i[1] for i in res]
        if escaped(height, width, res):
            ax.plot(x, y, 'ro-')
        else:
            ax.plot(x, y, 'bx-')
    plt.show()

if __name__ == "__main__":
    epochs = 1
    items = 5
    height = 5
    width = 10

    generation = []
    for i in range(items):
        theta = random.uniform(15, 180) * math.pi/180
        v = random.uniform(2, 20)
        generation.append((theta, v))

    results = []

    for i in range(epochs):
        results = launch(generation, height, width)
        generation = crossover(generation, results)
        mutate(generation)

    graph_interpolation(generation, results, height, width)

