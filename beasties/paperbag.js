function Particle(x, y) {
  this.x = x;
  this.y = y;
}

var id = 0;

function init() {
  var c=document.getElementById("myCanvas");
  var particle = new Particle(c.width/2, c.height/2);

  if (id === 0) {
    document.getElementById("Go").innerHTML="Stop";
    id = setInterval(function() {
      update(particle);
      },
      100); 
  }
  else {
    clearInterval(id);
    document.getElementById("Go").innerHTML="Start";
    document.getElementById("demo").innerHTML="Success";
    id = 0;
  }
}

function in_bag(particle, left, right, up, down) {
  return (particle.x > left) && (particle.x < right)
	  && (particle.y > up) && (particle.y < down);
}

function draw(particle) {
  var c=document.getElementById("myCanvas");
  var ctx=c.getContext("2d");
  
  ctx.clearRect(0, 0, c.width, c.height);       //clear
  ctx.fillStyle="#E0B044";
  x = c.width/3;
  y = c.height/3;
  ctx.fillRect(x, y, c.width/3, c.height/3);    //draw bag

  ctx.beginPath();
  ctx.rect(particle.x, particle.y, 4, 4);
  ctx.strokeStyle="black";
  ctx.stroke();

  return in_bag(particle, x, x+c.width/3, y, y+c.height/3);
}


function move(particle) {
  particle.x += 50 * (Math.random() - 0.5);
  particle.y += 50 * (Math.random() - 0.5);
}

function update(particle) {
  move(particle);

  if (!draw(particle)) {
    init();
  }
}

