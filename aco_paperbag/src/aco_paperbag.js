//https://developer.mozilla.org/en-US/docs/HTML/Canvas

var id = 0;
var x = 0;
var scale = 20.0;
var height;
var width;
var ants = 20;
var trails = [];
var pheromones = [];

function stop()
{
  clearInterval(id);
  id = 0;
  x = 0;
  document.getElementById("click_draw").innerHTML="draw";
}

function contains(a, obj) {
    for (var i = 0; i < a.length; i++) {
        if (a[i] === obj) {
            return true;
        }
    }
    return false;
}

function contains_key(a, key) {
  return a.hasOwnProperty(key);
}

function possible_positions(width, pos) {
    //up to eight neighbours
    //Consider not allowing you to revisit a node
    var possible = [], n = 0;
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

function next_pos(width, pos, trail) {
    var possible = possible_positions(width, pos);
    var allowed = [];
    var i = 0;
    for (i = 0; i < possible.length; ++i) {
        if (!contains(trail, possible[i])) {
            allowed.push(possible[i]);
        }
    }  
    n = Math.floor(Math.random() * allowed.length);//we want < total length, so don't add one here
    if (n < 0 || n > possible.length) {
        throw "n out of range " + n;
    }
    return possible[n];
}

function start_pos(width) {
  return { x: Math.floor(width/2), y: 0 };
}

function random_trail(width, height) {
    // + + + //
    // . . . //
    // . . . //
    // - - - //
    //
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
    for (i = 0; i < ants; ++i) {
        trail = random_trail(width, height);
        trails.push(trail);
    }
    return trails;
}

function nearest_pheromone(pheromones, pos) {
  var best_dist = -1, dist, i, item, index, weight = -1;
  for (i = 0; i < pheromones.length; ++i) {
    item = pheromones[i];
    dist = Math.sqrt((pos.x-item.x)*(pos.x-item.x) + (pos.y-item.y)*(pos.y-item.y));
    if ((best_dist === -1) || (dist < best_dist) || ((dist === best_dist) && item.weight > weight)) {
      best_dist = dist;
      index = i;
      weight = item.weight;
    }
  }
  if (best_dist !== -1 && best_dist < 1) {
    return index;
  }
  else {
    return -1;
  }
}


function cumulative_weights(possible, pheromones){
  var total = 0, index;
  var cumulative = [total];
  for (i = 0; i < possible.length; ++i) {
    index = nearest_pheromone(pheromones, possible[i]);
    if (index !== -1) {
      total = total + pheromones[index].weight;
    }
    // does weight * 1/distance work? (or weight if it's on top of it)
    cumulative.push(total);
  }
  return cumulative;
}

function roulette_wheel_choice(width, pos, trail, pheromones) {
  var possible = possible_positions(width, pos);
  var allowed = [];
  var i = 0;
  for (i = 0; i < possible.length; ++i) {
    if (!contains(trail, possible[i])) {
      allowed.push(possible[i]);
    }
  }
  var cumulative = cumulative_weights(possible, pheromones);
  var total = cumulative[cumulative.length-1];
  //random is in [0, 1) 
  var p = Math.floor(Math.random() * total + 1);
  if ( p > total) {
    return possible[possible.length - 1];
  }

  for (i = 0; i < cumulative.length - 1; ++i) {
    if (p >= cumulative[i] && p <= cumulative[i+1]) { 
      //the first place where it is in range, with 1 is in [1,1]
      return possible[i];
    }
  }

  throw "Cannot find valid move, for " + p + " of total " + total + " with " + cumulative;
}

function pheromone_trail(height, width, pheromones) {
  var trail = [], pos = start_pos(width);
  trail.push(pos);

  while (pos.y < height) {
    pos = roulette_wheel_choice(width, pos, trail, pheromones);
    trail.push(pos);
  }
  return trail;
}

function new_trails(height, width, ants) {
  var i = 0, trails = [], trail;
  for (i = 0; i < ants; ++i) {
      trail = pheromone_trail(width, height, pheromones);
      trails.push(trail);
  }
  return trails;
}

function next(max) {
  if (x < max) {
    id = setTimeout(simulate, 200);
  }
  else {
    stop();
  }
}

function find_best(trails) {
  var len = -1, best = 0, i = 0;

  for (i = 0; i < trails.length; ++i) {
      if (len === -1 || trails[i].length < len) {
          best = i;
          len = trails[i].length;
      }
  }
  return best;
}

function find_worst(trails) {
  var len = 0, worst = 0, i = 0;

  for (i = 0; i < trails.length; ++i) {
      if (trails[i].length > len) {
          worst = i;
          len = trails[i].length;
      }
  }
  return worst;
}

function draw() {
  var i, x, y;
  var canvas = document.getElementById('tutorial');
  if (canvas.getContext) {
    var ctx = canvas.getContext("2d");
    ctx.clearRect(0, 0, canvas.width, canvas.height); 
    ctx.fillStyle = "rgb(180, 120, 60)";
    ctx.fillRect (1, 8, canvas.width - 1, canvas.height - 8);

    var result = document.getElementById("epoch");
    result.innerHTML = parseInt(result.innerHTML) + 1;

    i = find_best(trails);
    for (j = 0;  j < trails[i].length; ++j) {
      ctx.fillStyle = "rgb(0,0,0)";
      x = trails[i][j].x * scale;
      y = canvas.height - trails[i][j].y * scale;
      ctx.fillRect (x, y - 2, 4, 4);
    }
    document.getElementById("best").innerHTML += ", " + (trails[i].length - 1);

    i = find_worst(trails);
    for (j = 0;  j < trails[i].length; ++j) {
      ctx.fillStyle = "rgb(255,0,0)";
      x = trails[i][j].x * scale;
      y = canvas.height - trails[i][j].y * scale;
      ctx.strokeRect (x, y - 1, 1, 1);
    }
    document.getElementById("worst").innerHTML += ", " + (trails[i].length - 1);
  }
}

function evapourate(pheromones) {
  var evapouration = 0.5;
  var update = [], new_pos;

  for(i = 0; i < pheromones.length; ++i) {
    new_pos = {x: pheromones[i].x, y: pheromones[i].y, weight: evapouration * pheromones[i].weight};
    if (new_pos.weight > 0.5) {
      update.push( new_pos );
    }
  }
  return update;
}

function update_pheromones(pheromones, trail) {
  var i, pos, new_pos, new_weight, bias;
  var update = evapourate(pheromones);

  for (i = 0; i < trail.length; ++i) {
    pos = trail[i];
    bias = pos.y * pos.y; //make ones near the top more attractive
    //I just want to add one here *or* update the old one
    indexExisting = nearest_pheromone(pheromones, pos);
    indexUpdated = nearest_pheromone(update, pos);
    if ( indexUpdated !== -1 ) {
      new_weight = update[indexUpdated].weight + bias;
      new_pos = {x: update[indexUpdated].x, y: update[indexUpdated].y, weight: new_weight};
      update[indexUpdated] = new_pos;
    }
    else if ( indexExisting !== -1 ) {
      new_weight = pheromones[indexExisting].weight + bias;
      new_pos = {x: pheromones[indexExisting].x, y: pheromones[indexExisting].y, weight: new_weight};
      update.push( new_pos );
    }
    else {
      new_weight = bias;
      new_pos = {x: pos.x, y: pos.y, weight: new_weight};
      update.push( new_pos );
    }

  }
  return update;
}

function update() {
  var trail, i;
  for( i = 0; i < trails.length; ++i) {
    trail = trails[i];
    if (trail.length < 2*height ) {
      pheromones = update_pheromones(pheromones, trail);
    }
  }
  if (pheromones.length === 0) {
    var trail = trails[find_best(trails)];
    pheromones = update_pheromones(pheromones, trail);
  }

  trails = new_trails(height, width, ants);
}

function simulate() {
  try {
    update();
    draw();

    x = x + 1;
    next(30);
  }
  catch(err) {
    alert(err);
  }
}

function aco() {
  draw();
  simulate();
}

function start() {
  if (id === 0) {  
    var canvas = document.getElementById('tutorial');
    height = canvas.height / scale;
    width = canvas.width / scale;
    trails = make_trails(height, width, ants);
    document.getElementById("click_draw").innerHTML="stop";
    id = setTimeout(aco, 100);
  }
  else {
    var canvas = document.getElementById('tutorial');
    if (canvas.getContext) {
      var ctx = canvas.getContext("2d");
      ctx.clearRect(0, 0, canvas.width, canvas.height); 
    }

    id = setTimeout(stop, 100);
  }
}


