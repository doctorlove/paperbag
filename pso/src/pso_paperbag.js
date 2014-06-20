

//do one particle first, majing sure you can see it trail around
//See http://msdn.microsoft.com/en-us/magazine/hh335067.aspx
//v(t+1) = (w * v(t)) + (c1 * r1 * (p(t) – x(t)) + (c2 * r2 * (g(t) – x(t))
// x(t+1) = x(t) + v(t+1)

function pso() {
  var canvas = document.getElementById('tutorial');
  if (canvas.getContext) {
    var ctx = canvas.getContext("2d");
    ctx.clearRect(0, 0, canvas.width, canvas.height); 
    ctx.fillStyle = "rgb(180, 120, 60)";
    ctx.fillRect (1, 8, canvas.width - 1, canvas.height - 8);

    var result = document.getElementById("epoch");
    result.innerHTML = parseInt(result.innerHTML) + 1;

    ctx.fillStyle = "rgb(0,0,0)";
    x = canvas.width/2.0;
    y = canvas.height - canvas.height/2.0;//note the - to change y to canvas
    ctx.fillRect (x, y - 2, 4, 4);
  }
}

function start() {
  var canvas = document.getElementById('tutorial');
  //...
  //
  document.getElementById("click_draw").innerHTML="stop"; 
  id = setTimeout(pso, 100);
}

