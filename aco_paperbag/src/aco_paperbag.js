"use strict";
var middle_start = false;
var minmax = false;
var id = 0;
var scale = 30.0;
var ants = 100;

function stop() {
  clearInterval(id);
  id = 0;
  document.getElementById("click_draw").innerHTML="draw";
}

function properties_match(lhs, rhs, property) {
  return (lhs.hasOwnProperty(property) && rhs.hasOwnProperty(property) && lhs[property]==rhs[property]);
}


function contains(a, obj) {
  return a.some(z => z.x === obj.x && z.y === obj.y);
}

function possible_positions(width, pos) {
    //up to eight neighbours
    var possible = [];
    if (pos.y > 0) {
        if (pos.x > 0) {
            possible.push({x: pos.x - 1, y: pos.y - 1});
        }

        possible.push({x: pos.x, y: pos.y - 1});
        if (pos.x < width - 1) {
            possible.push({x: pos.x + 1, y: pos.y - 1});
        }
    }

    if (pos.x > 0) {
        possible.push({x: pos.x - 1, y: pos.y});
    }

    if (pos.x < width - 1) {
        possible.push({x: pos.x + 1, y: pos.y});
    }

    if (pos.x > 0) {
        possible.push({x: pos.x - 1, y: pos.y + 1});
    }
    possible.push({x: pos.x, y: pos.y + 1});
    if (pos.x < width - 1) {
        possible.push({x: pos.x + 1, y: pos.y + 1});
    }
    return possible;
}

function allowed_positions(width, pos, trail) {
  var possible = possible_positions(width, pos);

  var allowed = [];
  var i = 0;
  for (i = 0; i < possible.length; i += 1) {
    if (!contains(trail, possible[i])) {
      allowed.push(possible[i]);
    }
  }
  if (allowed.length === 0) {
      allowed = possible;
  }
  return allowed;
}

function next_pos(width, pos, trail) {
    var allowed = allowed_positions(width, pos, trail);
    var index = Math.floor(Math.random() * allowed.length);
    return allowed[index];
}

function start_pos(width) {
  if (middle_start) {
    return { x: Math.floor(width / 2), y: 0 };
  }
  return { x:Math.floor(Math.random() * (width + 1)), y:0 };
}

function random_trail(height, width) {
    // Assume we start at the bottom
    //     If we get to the top, we're out, finish
    var trail = [], pos = start_pos(width);
    trail.push(pos);

    while (pos.y < height) {
        pos = next_pos(width, pos, trail);
        trail.push(pos);
    }

    return trail;
}

function make_trails(height, width, ants) {
    var i = 0, trails = [], trail;
    for (i = 0; i < ants; i += 1) {
        trail = random_trail(height, width);
        trails.push(trail);
    }
    return trails;
}

function pheromone_at(pheromones, pos) {
  var i;
  for (i = 0; i < pheromones.length; i += 1) {
    if(pheromones[i].x === pos.x && pheromones[i].y === pos.y) {
      return i;
    }
  }
  return -1;
}

function taueta(weight, y) {
  var alpha = 1.0;
  var beta = 4.0;
  return Math.pow(weight, alpha) * Math.pow(y, beta);
}

function cumulative_probability(possible, pheromones){
  var total = 0.0, index, i;
  var cumulative = [total];
  for (i = 0; i < possible.length; i += 1) {
    index = pheromone_at(pheromones, possible[i]);
    if (index !== -1) {
      total = total + taueta(pheromones[index].weight, pheromones[index].y);
    }
    cumulative.push(total);
  }
  return cumulative;
}

function show_pheromones(pheromones) {
  var worst =  { x: 0, y: 0, weight: 1000 };
  var best =  { x: 0, y: 0, weight: -1 };
  try {
    var display = "", i = 0;
    for (i = 0; i < pheromones.length; i += 1) {
      display = display + "(" + pheromones[i].x + ", " + pheromones[i].y + "):" + pheromones[i].weight + ", ";
      if (pheromones[i].weight > best.weight) {
        best = pheromones[i];
      }
      if (pheromones[i].weight < worst.weight) {
        worst = pheromones[i];
      }
    }
    display = display + "BEST (" + best.x + ", " + best.y + "):" + best.weight + " ";
    display = display + "WORST (" + worst.x + ", " + worst.y + "):" + worst.weight + " ";
    return display;
  }
  catch (err) {
    return "pheromones";
  }
}


function roulette_wheel_choice(width, pos, trail, pheromones) {
  var p = 0, i;
  var possible = allowed_positions(width, pos, trail);
  var cumulative = cumulative_probability(possible, pheromones);
  var total = cumulative[cumulative.length-1];
  if (total === 0) {
    p = Math.floor(Math.random() * possible.length);
    return possible[p];
  }

  p = Math.random() * total;

  for (i = 0; i < cumulative.length - 1; i += 1) {
    if (p >= cumulative[i] && p <= cumulative[i+1]) {
      //the first place where it is in range, with 1 is in [1,1]
      return possible[i];
    }
  }

  throw "Cannot find valid move, for " + p + " of total " + total + " with " + cumulative + " and pheromones " + show_pheromones(pheromones) + " at pos " + pos.x + ", " + pos.y ;
}

function pheromone_trail(height, width, pheromones) {
  var trail = [], pos = start_pos(width);
  trail.push(pos);

  while (pos.y <= height) {
    pos = roulette_wheel_choice(width, pos, trail, pheromones);
    trail.push(pos);
  }
  return trail;
}

function new_trails(pheromones, height, width, ants) {
  var i = 0, trails = [], trail;
  for (i = 0; i < ants; i += 1) {
      trail = pheromone_trail(height, width, pheromones);
      trails.push(trail);
  }
  return trails;
}

function find_best(trails) {
  var len = -1, best = 0, i = 0, this_length;

  for (i = 0; i < trails.length; i += 1) {
      this_length = total_length(trails[i]);
      if (len === -1 || this_length < len) {
          best = i;
          len = this_length;
      }
  }
  return best;
}

function find_worst(trails) {
  var len = 0, worst = 0, i = 0, this_length;

  for (i = 0; i < trails.length; i += 1) {
      this_length = total_length(trails[i]);
      if (total_length(trails[i]) > len) {
          worst = i;
          len = this_length;
      }
  }
  return worst;
}

function find_average(trails) {
  var len = 0, i = 0;

  for (i = 0; i < trails.length; i += 1) {
    len += total_length(trails[i]);
  }
  len = len / trails.length;
  return len;
}

function draw(trails) {
  var i, j, x, y;
  var canvas = document.getElementById("ant_canvas");
  if (canvas.getContext) {
    var ctx = canvas.getContext("2d");
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.fillStyle = "rgb(180, 120, 60)";
    ctx.fillRect (0, 8, canvas.width, canvas.height - 8);

    var result = document.getElementById("epoch");
    result.innerHTML = parseInt(result.innerHTML) + 1;


    var path = "";

    i = find_best(trails);
    for (j = 0;  j < trails[i].length; j += 1) {
      ctx.fillStyle = "rgb(0,0,0)";
      x = trails[i][j].x * scale;
      y = canvas.height - trails[i][j].y * scale;
      path = path +  " (" + x + ", " + y + ")";
      ctx.fillRect (x, y - 2, 4, 4);
    }
    document.getElementById("best").innerHTML += ", " + total_length(trails[i]).toFixed(2);
    document.getElementById("path").innerHTML = "\n " + path;


    i = find_worst(trails);
    for (j = 0;  j < trails[i].length; j += 1) {
      ctx.fillStyle = "rgb(255,0,0)";
      x = trails[i][j].x * scale;
      y = canvas.height - trails[i][j].y * scale;
      ctx.strokeRect (x, y - 1, 1, 1);
    }
    document.getElementById("worst").innerHTML += ", " + total_length(trails[i]).toFixed(2);

    document.getElementById("average").innerHTML += ", " + find_average(trails).toFixed(2);
  }
}

function evapourate(pheromones) {
  var evapouration = 0.25;
  var i;
  for(i = 0; i < pheromones.length; i += 1) {
    pheromones[i].weight = evapouration * pheromones[i].weight;
  }
}

function total_length(trail) {
  var i;
  var length = 0;
  var step;
  for (i = 1; i < trail.length; i += 1) {
    step = Math.pow((trail[i-1].x-trail[i].x), 2)
         + Math.pow((trail[i-1].y-trail[i].y), 2);
    step = Math.pow(step, 0.5);
    length = length + step;
  }
  return length;
}

function add_new_pheromones(height, pheromones, trail) {
  var i, pos, index;
  var Q = 2.0 * height;
  var L = Q/total_length(trail);

  for (i = 0; i < trail.length; i += 1) {
    pos = trail[i];
    index = pheromone_at(pheromones, pos);
    if ( index !== -1 ) {
      pheromones[index] = {x: pheromones[index].x, y: pheromones[index].y, weight: pheromones[index].weight + L};
    }
    else {
      pheromones.push( {x: pos.x, y: pos.y, weight: L});
    }

  }
}

function update(pheromones, trails, height, width) {
  var trail, i;
  evapourate(pheromones);
  if (minmax) {
    var best = find_best(trails);
    add_new_pheromones(height, pheromones, trails[best]);
  }
  else {
    for( i = 0; i < trails.length; i += 1) {
      trail = trails[i];
      add_new_pheromones(height, pheromones, trail);
    }
  }
}

function simulate(epoch, pheromones, trails, height, width) {
  try {
    update(pheromones, trails, height, width);
    trails = new_trails(pheromones, height, width, ants);
    draw(trails);

    epoch = epoch + 1;
    if (epoch < 25) {
      id = setTimeout(function() {
             simulate(epoch, pheromones, trails, height, width);
           }, 200);
    }
    else {
      stop();
    }
  }
  catch(err) {
    alert(err);
  }
}

function aco() {
  var canvas = document.getElementById("ant_canvas");
  var pheromones = [];
  var height = canvas.height / scale;
  var width = canvas.width / scale;
  var trails = make_trails(height, width, ants);
  draw(trails);
  simulate(0, pheromones, trails, height, width);
}

function start() {
  if (id === 0) {
    document.getElementById("click_draw").innerHTML="stop";
    var opt = document.getElementById("minmax");
    if (opt) {
      minmax = opt.checked;
    }
    opt = document.getElementById("middle_start");
    if (opt) {
      middle_start = opt.checked;
    }
    id = setTimeout(aco, 100);
 }
  else {
     stop();
  }
}


