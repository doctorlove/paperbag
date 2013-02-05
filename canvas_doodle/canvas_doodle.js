//https://developer.mozilla.org/en-US/docs/HTML/Canvas

var id = 0;

function stop() {
  clearInterval(id);
  id = 0;
  xx = 0;
  document.getElementById("click_draw").innerHTML="draw";
}

function draw(x) {
  var canvas = document.getElementById('tutorial');
  if (canvas.getContext) {
    var ctx = canvas.getContext("2d");
    ctx.clearRect(0, 0, canvas.width, canvas.height); 
 
    ctx.fillStyle = "rgb(169, 130, 19)";
    ctx.fillRect (10, 20, 100, 100);
 
    ctx.fillStyle = "rgba(0, 0, 0, 0.75)";
    ctx.fillRect (10 + x, 40, 25, 25);
    var result = document.getElementById("result");
    result.innerHTML=parseInt(result.innerHTML) + 1;
  }
}

function update(x) {
    return x + 5;
}

function action(x) {
    draw(x);
    x = update(x);
    if (x < 110) {
      id = setTimeout(function() {
	     action(x);
           }, 100);
    }
    else {
      stop();
    }
}

function start() {
  if (id === 0) {  
      id = setTimeout(function() {
	     action(0);
           }, 100);
  }
  else {
    id = setTimeout(stop, 100);
  }
  document.getElementById("click_draw").innerHTML="stop";
}

