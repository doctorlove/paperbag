function getRandomInt(min, max) {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}


function best(first, second) {
  if (first.y > second.y) {
    return first;
  }
  return second;
}

function move(item, w, c1, c2, height, width, bestGlobal) {
//See http://msdn.microsoft.com/en-us/magazine/hh335067.aspx
//v(t+1) = (w * v(t)) + (c1 * r1 * (p(t) - x(t)) + (c2 * r2 * (g(t) - x(t))
//p this particle, g global (i.e. swarm itself)
//w an inertia weight
//c1 "cognitive" (or local) weight
//r1 random in [0,1)
//p particle's best pos so far
//c2 social (or global) weight
//r2 random in [0,1)
//g best in swarm position so far
// x(t+1) = x(t) + v(t+1)

//move the x as well, at some point...
//and avoid going over the edges
  var i;
  for (i = 0; i < item.length; ++i) {
    //item[i].y = item[i].y + 5; 
    current = item[i];
    var r1 = getRandomInt(-5, 5);//need to think about this 
    var r2 = getRandomInt(-5, 5);//need to think about this 
    var v = (w * current.v.y) + (c1 * r1 * current.best.y - current.y) + (c2 * r2 * (bestGlobal.y - current.y));
    item[i].y = item[i].y + v;
    item[i].v.y = v;
    if (item[i].y < 0) {
      item[i].y = 0
    }
    if (item[i].y > height) {
      item[i].y = height
    }
    x = item[i].x + 5*getRandomInt(-5, 5); 
    if (x > 0 && x < width) {
      item[i].x = x;
    }
  }
}

function draw(item, epoch) {
  var canvas = document.getElementById('tutorial');
  if (canvas.getContext) {
    var ctx = canvas.getContext("2d");
    ctx.clearRect(0, 0, canvas.width, canvas.height); 
    ctx.fillStyle = "rgb(180, 120, 60)";
    ctx.fillRect (1, 8, canvas.width - 1, canvas.height - 8);

    var result = document.getElementById("epoch");
    result.innerHTML =  epoch;

    var i;
    for (i = 0; i < item.length; ++i) {
      ctx.fillStyle = "rgb(0,0,0)";
      var particle = item[i];
      ctx.fillRect (particle.x, canvas.height - particle.y - 2, 4, 4);
    }
  }
}

function updateBest(item, bestGlobal) {
  var i;
  for (i = 0; i < item.length; ++i) {
    bestGlobal = best(item[i], bestGlobal);
    item[i].best = best(item[i].best, item[i]);
  }
  return bestGlobal;
}

function pso(item, epoch, bestGlobal, height, width) {
  //Consider adding a try catch
  epoch = epoch + 1;//is this by ref?
  var inertiaWeight = 0.7;
  var personalWeight = 1.5;
  var swarmWeight = 1.5;
  move(item, inertiaWeight, personalWeight, swarmWeight, height, width, bestGlobal);
  draw(item, epoch);
  bestGlobal = updateBest(item, bestGlobal);
  //Why not just loop?
  if (epoch < 25) {
    setTimeout(function () { pso(item, epoch, bestGlobal, width); }, 200);
  }
}

function initialise(particles){
  var item = [];
  var canvas = document.getElementById('tutorial');
  var i;
  for (i = 0; i < particles; ++i) {
      x = getRandomInt(0, canvas.width-4);//don't hard code the 4
      y = 0;
      var velocity = { x:getRandomInt(-5,5), y:getRandomInt(0,5)};
      item.push ( { x: x, y: y, best: {x:0, y:0}, v:velocity } );
  }
  return item;
}

function start() {
  var canvas = document.getElementById('tutorial');
  //...
  //
  document.getElementById("click_draw").innerHTML="stop"; 
  item = initialise(3);
  var epoch = 0;
  draw(item, epoch);
  var bestGlobal = {x:0, y:0};
  var id = setTimeout(function () { pso(item, epoch, bestGlobal, canvas.height, canvas.width); }, 100);
}

