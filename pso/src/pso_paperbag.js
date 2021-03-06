function getRandomInt(min, max) {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}


function best(first, second) {
  if (first.y > second.y) {
    return first;
  }
  return second;
}

function move_in_range(velocity, max, item, property) {
  var value = item[property] + velocity;
  if (value < 0) {
    item[property] = 0;
  }
  else if (value > max) {
    item[property] = max;
  }
  else {
    item[property] = value;
    item.v[property] = velocity;
  }
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
    var current = item[i];
    var r1 = getRandomInt(0, 5);//need to think about this 
    var r2 = getRandomInt(0, 5);//need to think about this 
    var vy = (w * current.v.y) + (c1 * r1 * (current.best.y - current.y)) + (c2 * r2 * (bestGlobal.y - current.y));
    var vx = (w * current.v.x) + (c1 * r1 * (current.best.x - current.x)) + (c2 * r2 * (bestGlobal.x - current.x));
    move_in_range(vy, height, item[i], "y");
    move_in_range(vx, width, item[i], "x");
    //what about the 4 to allow space to draw it?
  }

}

function draw(item, epoch) {
  var canvas = document.getElementById('tutorial');
  if (canvas.getContext) {
    var ctx = canvas.getContext("2d");
    ctx.clearRect(0, 0, canvas.width, canvas.height); 
    ctx.fillStyle = "rgb(180, 120, 60)";
    ctx.fillRect (1, 8, canvas.width - 6, canvas.height - 8);

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

function pso(item, epoch, bestGlobal, height, width, milliseconds) {
  //Consider adding a try catch
  epoch = epoch + 1;//is this by ref?
  var inertiaWeight = 0.9;
  var personalWeight = 0.1;
  var swarmWeight = 0.09;
  move(item, inertiaWeight, personalWeight, swarmWeight, height, width, bestGlobal);
  draw(item, epoch);
  bestGlobal = updateBest(item, bestGlobal);
  if (epoch < 40) {//TODO keep going til they escape
    setTimeout(function () { pso(item, epoch, bestGlobal, height, width, milliseconds); }, 300);
   var total_milliseconds = new Date().getTime() - milliseconds;
    document.getElementById("update").innerHTML = total_milliseconds;
    update.innerHTML =  epoch;
  }
}

function initialise(particles, width, height) {
  var item = [];
  var i;
  for (i = 0; i < particles; ++i) {
      x = getRandomInt(0, width);
      y = height/2.0;
      var velocity = { x:getRandomInt(-5,5), y:getRandomInt(0,5)};
      item.push ( { x: x, y: y, best: {x:x, y:y}, v:velocity } );
  }
  return item;
}

function start() {
  var canvas = document.getElementById('tutorial');
  //...
  //
  document.getElementById("click_draw").innerHTML="stop"; 
  //make stop button work
  var canvas = document.getElementById('tutorial');
  item = initialise(20, canvas.width - 4, canvas.height); //don't hard code the 4
  var epoch = 0;
  draw(item, epoch);
  var bestGlobal = item[0]; 
  var milliseconds = new Date().getTime();
  var id = setTimeout(function () { pso(item, epoch, bestGlobal, canvas.height, canvas.width, milliseconds); }, 100);
}

