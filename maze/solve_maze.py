import numpy

def potential_moves(facing):
    l = ['L', 'U', 'R', 'D']
    if facing == 'U':
        return [l[i] for i in [0, 1, 2, 3]]
    if facing == 'R':
        return [l[i] for i in [1, 2, 3, 0]]
    if facing == 'D':
        return [l[i] for i in [2, 3, 0, 1]]
    if facing == 'L':
        return [l[i] for i in [3, 0, 1, 2]]
    message = 'Invalid facing ' + facing
    raise Exception(message)

def next_move(facing, row, col, maze):
    l = potential_moves(facing)
    index = 0
    rows = maze.shape[0]
    cols = maze.shape[1]
    while index < len(l):
        if l[index] == 'U' and (row - 1 >= 0) and (maze[row - 1, col] == 0):
            return 'U', row - 1, col
        elif l[index] == 'R' and (col + 1 < cols) and (maze[row, col + 1] == 0):
            return 'R', row , col + 1
        elif l[index] == 'D' and (row + 1 < rows) and (maze[row + 1, col] == 0):
            return 'D', row + 1, col
        elif l[index] == 'L' and (col - 1 >= 0) and (maze[row, col - 1] == 0):
            return 'L', row, col - 1
        index = index + 1
    message = 'Stuck at ' + str(row)  + ', ' + str(col) + ', facing ' + facing
    raise Exception(message)


def wall_follow(maze):
    if any([t == 0 for t in maze[0,:]]) == False:
        raise Exception('Expected maze to have an exit at the top') 
    if any([t == 0 for t in maze[-1,:]]) == False:
        raise Exception('Expected maze to have an exit at the bottom') 

    #for now we are going down, and escaping out the bottom
    row = 0

    #return an array of points
    path = []

    #find starting point at top
    col = 0
    while maze[0, col] == 1:
      col = col + 1

    rows = maze.shape[0]
    cols = maze.shape[1]
    path.append( (row, col) )

    steps = 0
    facing = 'D' 
    while row < rows-1:
	facing, row, col = next_move(facing, row, col, maze)
        path.append( (row, col) )
        steps = steps + 1
        if steps > rows*cols:
            message = 'Too many steps. Stuck at ' + str(row)  + ', ' + str(col) + ', facing ' + facing
            raise Exception(message)

    return path

def drawable_solution(maze):
    path = wall_follow(maze)
    solution = maze.copy()
    # i = pyplot.imshow(m)
    # pyplot.show()
    # i = pyplot.imshow(s, cmap = pyplot.cm.Greys_r)
    # pyplot.show()
    # https://github.com/vinaysrao/maze-generator/blob/master/generatemaze.py
    # lookup  savefig and how to make an animation
    #  http://stackoverflow.com/questions/2546780/python-animation-with-matplotlib-pyplot
    #
    #  Maybe lookup patchlabel too...
    for (x,y) in path:
        solution[x,y] = 0.5
    return solution 


if __name__ == '__main__':
    maze = numpy.array([[1, 0, 1, 0, 0, 0],
                            [1, 0, 1, 0, 1, 0],
                            [1, 0, 1, 0, 1, 0],
                            [1, 0, 0, 0, 0, 0],
                            [1, 1, 1, 0, 1, 1]])
    path = wall_follow(maze)
    print maze
    print path
    #to run one test
    #>python.exe -m unittest generate_maze_test.TestMazeSolve.test_left_wall_follow_with_one_possible_path_follows_that_path
    #
    # Look at http://avian.netne.net/blog/?p=93

