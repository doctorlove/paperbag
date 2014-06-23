//do one particle first, making sure you can see it trail around
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

function best(first, second) {
  if (first.y > second.y) {
    return first;
  }
  return second;
}

function move(item) {
  var i;
  for (i = 0; i < item.length; ++i) {
    item[i].y = item[i].y + 5; 
  }
}

function draw(item, epoch) {
  var canvas = document.getElementById('tutorial');
  if (canvas.getContext) {
    var ctx = canvas.getContext("2d");
    ctx.clearRect(0, 0, canvas.width, canvas.height); 
    ctx.fillStyle = "rgb(180, 120, 60)";
    ctx.fillRect (1, 8, canvas.width - 1, canvas.height - 8);

    epoch = epoch + 1
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

function pso(item, epoch) {
  epoch = epoch + 1;//is this by ref?
  move(item);
  draw(item, epoch);
  //Why not just loop?
  if (epoch < 25) {
    setTimeout(function () { pso(item, epoch); }, 100);
  }
}

function initialise(particles){
  var item = [];
  var canvas = document.getElementById('tutorial');
  var i;
  for (i = 0; i < particles; ++i) {
      x = canvas.width/2.0;
      y = canvas.height - canvas.height/2.0;//note the - to change y to canvas
      item.push ( { x: x, y: y } );
  }
  return item;
}

//draw
//move, draw, move etc etc
//See simulate function in aco

function start() {
  var canvas = document.getElementById('tutorial');
  //...
  //
  document.getElementById("click_draw").innerHTML="stop"; 
  item = initialise(1);
  var epoch = 0;
  draw(item, epoch);
  var id = setTimeout(function () { pso(item, epoch); }, 100);
}

