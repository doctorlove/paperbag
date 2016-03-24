import math
import matplotlib.pyplot as plt
import random

def hit_height(theta, v):
    return 0.0

def collision_point(x0, y0, x1, y1, height, width):
    if x0 == x1: return x0, y0
    x = 0.0
    if x1 > width: x = width
    m = (y1 - y0)/(1.0*x1 - x0)
    y = y0 + m*(x-x0)
    return x, y

def collides(x0, y0, x1, y1, height, width):
    x, y = collision_point(x0, y0, x1, y1, height, width)
    if y < height and ((x0 >= 0 and x1 <= 0) or (x0 <= width and x1 >= width)):
        return x, y, True
    return x, y, False 

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
            x_hit, y_hit, hits = collides(previous_x, previous_y, x, y, height, width)
            if hits:
                result.append((x_hit, y_hit, hits))
                break
            else:
                result.append((x, y, False))
        results.append(result)
    return results

def get_choices(generation, height, width, results):
    choices = [generation[i] for i in range(len(generation)) if escaped(height, width, results[i]) ]
    return choices

def crossover(generation, results, height, width):
    choices = get_choices(generation, height, width, results)
    if len(choices) == 0:
        return init_random_generation(len(generation))
    next_generation = []
    for i in range(0, len(generation)):
        mum = generation[random.randint(0, len(choices)-1)]
        dad = generation[random.randint(0, len(choices)-1)]
        t = (mum[0], dad[1])
        next_generation.append(t)
    return next_generation

def mutate(generation):
    for i in range(len(generation)):
        (theta, v) = generation[i]
        if random.random() < 0.1:
            new_theta = theta + random.uniform(-10, 10) * math.pi/180
            if new_theta > 0 and new_theta < 2*math.pi:
                theta = new_theta
        if random.random() < 0.1:
            v *= random.uniform(0.9, 1.1)
        generation[i] = (theta, v)

def display(generation, result, ax, height, width):
    rect = plt.Rectangle((0, 0), width, height, facecolor='grey')
    ax.add_patch(rect)
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    for gen in generation:
        print gen
    free = 0
    for res in result:
        if escaped(height, width, res):
            free += 1
        x = [i[0] for i in res]
        y = [i[1] for i in res]
        if escaped(height, width, res):
            ax.plot(x, y, 'ro-')
        else:
            ax.plot(x, y, 'bx-')
    print "Escaped", free

def graph_interpolation(generation0, result0, generation, result, height, width):
    fig = plt.figure()
    #http://stackoverflow.com/questions/3584805/in-matplotlib-what-does-111-means-in-fig-add-subplot111
    #subplot(m,n,i) breaks the figure window into an m-by-n matrix of small subplots and 
    #selects the ithe subplot for the current plot. The plots are numbered along the top
    #row of the figure window, then the second row, and so forth.
    ax0 = fig.add_subplot(2,1,1)
    ax0.set_title('Initial attempt')
    display(generation0, result0, ax0, height, width)
    ax = fig.add_subplot(2,1,2)
    ax.set_title('Final attempt')
    display(generation, result, ax, height, width)
    plt.show()

def init_random_generation(items):
    generation = []
    for i in range(items):
        theta = random.uniform(15, 180) * math.pi/180
        v = random.uniform(2, 20)
        generation.append((theta, v))
    return generation


def demo():
    epochs = 10
    items = 12
    height = 5
    width = 10

    generation = init_random_generation(items)

    generation0 = list(generation)
    results = launch(generation, height, width)
    results0 = list(results)
    for i in range(1, epochs):
        generation = crossover(generation, results, height, width)
        mutate(generation)
        results = launch(generation, height, width)

    graph_interpolation(generation0, results0, generation, results, height, width)

if __name__ == "__main__":
    demo()
