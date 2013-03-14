library(RUnit)
#need install.packages("RUnit")

move <- function(x, y, posx, posy, width, height)
{
  result = list()

  if (posx >= 0 && posx <= width)
  {
    result$x = posx
    if (posy < 0 )
    {
      result$y = 0
    }
    else
    {
      result$y = posy
    }
  }
  else if (length(x)>0)
  {
    interceptx = 0
    if (posx > width)
    {
      interceptx = width
    }
    m = (interceptx-tail(x, 1)) / (posx-tail(x,1))
    intercepty = tail(y, 1) + (posy - tail(y, 1)) * m
    if (intercepty >= height || tail(x, 1) > width || tail(x, 1) < 0)
    {
      result$x = posx
      result$y = posy
    }
    else
    {
      if (intercepty < 0)
      {
         interceptx = tail(x, 1)
         intercepty = 0
      }
    
      result$x = interceptx
      result$y = intercepty
    }
  }
  else
  {
    #todo - raise an exception
    result$x = posx
    result$y = posy
  }
  
  return (result)
}

launch <- function(theta, v, width, height)
{
  t = 0.2 * 0:20
  x <- vector(mode="numeric")
  y <- vector(mode="numeric")
  for( i in t ) 
  {
    posx = width/2 + v * i * cos(theta)
    posy = v * i * sin(theta) - 0.5 * 9.81 * i*i
    xy = move(x, y, posx, posy, width, height)
    x <- c(x, xy$x)
    y <- c(y, xy$y)
  }
  result = list()
  result$x = x
  result$y = y
  result$escaped = tail(x,1)<0 || tail(x,1)>width
  return (result)
}

show <- function(x, y, width, height, colour)
{
}

move_tests <- function()
{
  height = 5
  width = 10
  
  checkEqualsNumeric(6, move(c(5), c(0), 6, -1, width, height)$x)
  checkEqualsNumeric(0, move(c(5), c(0), 6, -1, width, height)$y)
  
  checkEqualsNumeric(6, move(c(5), c(0), 6, 7, width, height)$x)
  checkEqualsNumeric(7, move(c(5), c(0), 6, 7, width, height)$y)

  checkEqualsNumeric(0, move(c(5), c(0), -5, 3, width, height)$x)
  checkEqualsNumeric(1.5, move(c(5), c(0), -5, 3, width, height)$y)
  
  checkEqualsNumeric(16, move(c(15), c(5), 16, 4, width, height)$x)
  checkEqualsNumeric(4, move(c(15), c(5), 16, 4, width, height)$y)

  checkEqualsNumeric(-0.666, move(c(0.75), c(8.75), -0.666, 10.88, width, height)$x)
  checkEqualsNumeric(10.88, move(c(0.75), c(8.75), -0.666, 10.88, width, height)$y)
}

crossover <- function(potential_parents, old_generationtheta, old_generationv)
{
  result = list()
  theta = 0
  v = 0
  if (length(potential_parents) < 1)
  {
    theta = runif(1, 15, 180) * pi / 180
    v = runif(1, 2, 20)
  }
  else
  {
    index = sample(potential_parents, 2, replace=TRUE)
    theta = old_generationtheta[index[1]]
    v = old_generationv[index[2]]
    # todo - mutate
  }
  result$theta = theta
  result$v = v
  return (result)
}

mutate <- function(genes)
{
  result = list()

  result$theta = genes$theta
  if (runif(1) <0.5)
  {
    result$theta = result$theta + runif(1, -10, 10)
  }
  result$v = genes$v
  if (runif(1) <0.5)
  {
    result$v = result$v * runif(1, 0.9, 1.1)
  }
  return (result)
}

run <- function(epochs, n)
{
  width = 10
  height = 5
  
  generationtheta = vector()
  generationv = vector()
  generationescaped = vector()
 
  plot(c(), c(), xlim=c(-20,20), ylim=c(-20,20), xlab="x", ylab="y") 

  for (epoch in 1:epochs)
  {
    old_generationtheta = generationtheta
    old_generationv = generationv
    old_generationescaped = generationescaped

    generationtheta = vector()
    generationv = vector()
    generationescaped = vector()

    for (i in 1:n)
    {
      if (epoch == 1)
      {
        theta = runif(1, 15, 180) * pi / 180
        v = runif(1, 2, 20)
      }
      else
      {
        potential_parents = which(old_generationescaped)
        result = mutate(crossover(potential_parents, old_generationtheta, old_generationv))
        theta = result$theta
        v = result$v
      }
    
      l = launch(theta, v, width, height)
      x = l$x
      y = l$y
      if (epoch == epochs)
      {
        if (l$escaped)
        {
          lines(x, y, type="b", col="black")
        }
        else
        {
          lines(x, y, type="l", col="red")
        }  
      }
      generationtheta <- c(generationtheta, theta)
      generationv <- c(generationtheta, v)
      generationescaped <- c(generationescaped, l$escaped)
      
    }
  }
  lines(c(0, 0, width, width), c(height, 0, 0, height), lwd=3)
      show(x, y, width, height, "black") #Show theta and v as well 
}
