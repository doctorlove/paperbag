
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


if __name__ =="__main__":
    height  =  5
    width  =  10
    x0 = width - 2
    x1 = width + 2
    y0 = height - 2
    y1 = height + 4
    print height + 1, collision_point(x0, y0, x1, y1, height, width)
    print collides(x0, y0, x1, y1, height, width)

