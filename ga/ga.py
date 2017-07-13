import math
import matplotlib.pyplot as plt
import random

def hit_coordinate(theta, v, width):
    angle = theta
    x = 0.5*width
    x_hit = width
    if theta > math.pi/2:
        angle = math.pi - theta
        x = -x
	x_hit = 0
    t = 0.5*width / (v * math.cos(angle)) #luckily angle and veloicty are never zero
    y = v * t * math.sin(angle) - 0.5 * 9.81 * t * t
    if y < 0 : y=0.0
    return x_hit, y

def esacped(theta, v, width, height):
    x_hit, y_hit = hit_coordinate(theta, v, width)
    return (x_hit==0 or x_hit==width) and y_hit > height

def launch(generation, height, width):
    results = []
    for (theta, v) in generation:
        result = []
        result.append((width/2.0, 0.0))
        for i in range(1, 20): #TODO - make while instead?
            t = i*0.2
            x = width/2.0 + v * t * math.cos(theta)
            y = v * t * math.sin(theta) - 0.5 * 9.81 * t * t
            if y < 0: y = 0
            result.append((x, y))
        results.append(result)
    return results

def cumulative_probabilities(results):
    #Could use from itertools import accumulate in python 3
    cp = []
    total = 0
    for res in results:
        total += res[1] 
        cp.append(total)
    return cp #not actually a probability!

def choose(choices):
    p = random.uniform(0, choices[-1])
    for i in range(len(choices)):
        if choices[i] >= p:
            return i
    return i

def selection(generation, width):
    results = []
    for gen in generation:
        theta, v = gen
        results.append(hit_coordinate(theta, v, width))
    return cumulative_probabilities(results)

def crossover(generation, width):
    choices = selection(generation, width)
    next_generation = []
    for i in range(0, len(generation)):
        mum = generation[choose(choices)]
        dad = generation[choose(choices)]
        t = (mum[0], dad[1])
        next_generation.append(t)
    return next_generation

def mutate(generation):
    #Could just pick one e.g.
    #i = random.randint(0, len(generation)-1)
    # or do all
    # or random shuffle and take top n
    for i in range(len(generation)-1):
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
    ax.set_xlim(-width, 2*width)
    ax.set_ylim(0, 3.5*height)
    free = 0
    for i in range(len(result)):
        theta, v = generation[i]
	res = result[i]
        x = [j[0] for j in res]
        y = [j[1] for j in res]
	if esacped(theta, v, width, height):
            ax.plot(x, y, 'ro-')
            free += 1
        else:
            ax.plot(x, y, 'bx-')
    print ("Escaped", free)

def display_start_and_finish(generation0, result0, generation, result, height, width):
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

    generation0 = list(generation) # save to contrast with last epoch
    results0 = launch(generation, height, width)

    for i in range(1, epochs):
        results = []
        generation = crossover(generation, width)
        mutate(generation)

    results = launch(generation, height, width)
    display_start_and_finish(generation0, results0, generation, results, height, width)

def single_item():
    height = 5
    width = 10
    generation = init_random_generation(5)
    result = launch(generation, height, width)
    fig = plt.figure()
    ax = fig.add_subplot(2,1,1)
    ax.set_title('Single cannon ball')
    display(generation, result, ax, height, width)
    plt.show()

if __name__ == "__main__":
    #single_item()
    demo()
