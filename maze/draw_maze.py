#http://matplotlib.org/examples/api/hinton_demo.html

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
from matplotlib.ticker import NullLocator

import generate_maze
import solve_maze

def draw(maze, solution):
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)

    ax.patch.set_facecolor('white')
    ax.set_aspect('equal', 'box')
    ax.xaxis.set_major_locator(NullLocator())
    ax.yaxis.set_major_locator(NullLocator())

    size = 1.0
    #draw the path taken - make the steps darker so we can see where it went
    total = 5.0 + len(solution)
    step = 1.0 / total
    rows = maze.shape[0]
    cols = maze.shape[1]
    color = 1.0
    for row, col in solution:
	color = color - step
        rect = make_rectangle(rows, row, col, color)
        ax.add_patch(rect)

    #draw the walls
    for row in xrange(rows):
        for col in xrange(cols):
            if maze[row, col] == 1.0:
                rect = make_rectangle(rows, row, col, 'black')
                ax.add_patch(rect)

    #draw the bag
    for row in xrange(rows - 1):
        for col in xrange(cols):
            if row == 0 or col == 0 or col == cols - 1:
                    rect = make_rectangle(rows, row, col, 'brown')
                    ax.add_patch(rect)

    rect = Rectangle([0, 1], 1.0, 1.0, facecolor='brown', edgecolor='brown')
    ax.add_patch(rect)
    rect = Rectangle([cols - 1, 1], 1.0, 1.0, facecolor='brown', edgecolor='brown')
    ax.add_patch(rect)

    #draw the start and end points to make it clear
    row, col = solution[1]
    rect = make_rectangle(rows, row, col, 'red')
    ax.add_patch(rect)
    row, col = solution[-1]
    rect = make_rectangle(rows, row, col, 'blue')
    ax.add_patch(rect)

    ax.autoscale_view()


def make_rectangle(rows, row, col, color):
        (x, y) = (col, rows - row)
        return Rectangle([x, y], 1.0, 1.0, facecolor=str(color), edgecolor=str(color))

if __name__ == '__main__':
    maze = generate_maze.maze(30, 15)
    solution = solve_maze.wall_follow(maze)
    draw(maze, solution)
    plt.title('Left wall follower')
    plt.show()



