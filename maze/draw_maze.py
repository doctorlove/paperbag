import numpy as np
import matplotlib.pyplot as plt

from matplotlib.patches import Rectangle
from matplotlib.ticker import NullLocator

import generate_maze
import solve_maze

def draw_paths(maze, solution, ax):
    #draw the path taken - make the steps darker so we can see where it went
    total = 5.0 + len(solution)
    start = 0.8
    step = start / total
    rows = maze.shape[0]
    cols = maze.shape[1]
    color = start
    index = 1
    for row, col in solution:
        color = color - step
        rect = make_rectangle(rows, row, col, color)
        ax.add_patch(rect)
        plt.draw()
        plt.savefig("solution" + str(index) + ".png")
	index = index + 1

def draw_maze(maze):
    plt.ion()
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)

    ax.patch.set_facecolor('white')
    ax.set_aspect('equal', 'box')
    ax.xaxis.set_major_locator(NullLocator())
    ax.yaxis.set_major_locator(NullLocator())

    size = 1.0
    rows = maze.shape[0]
    cols = maze.shape[1]

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

    ax.autoscale_view()
    plt.draw()
    return ax;


def make_rectangle(rows, row, col, color):
        (x, y) = (col, rows - row)
        return Rectangle([x, y], 1.0, 1.0, facecolor=str(color), edgecolor=str(color))

if __name__ == '__main__':
    maze = generate_maze.maze(30, 15)
    solution = solve_maze.wall_follow(maze)
    ax = draw_maze(maze)
    plt.savefig("solution0.png")
    draw_paths(maze, solution[1:], ax)
    plt.title('Left wall follower')
    plt.show()
    plt.savefig("solution.png")


