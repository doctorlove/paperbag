var bag_width = 300;
var width = 4;
var left = 75;
var right = left + bag_width;
var up = 25;
var down = up + bag_width;

function Beasty(x, y, id, index) {
	this.x = x;
	this.y = y;
	this.id = id;
	this.index = index;//why does this need an index?
}

var ids = []; 

function init() {
	action(0);
}

function action(id) {
	var beast;
	var x = left + Math.floor(bag_width * Math.random());
	//var y = down - width;
	var y = left + Math.floor(bag_width * Math.random());
	var index = ids.length;
	beast = new Beasty(x, y, id, index);
	ids.push(beast);
	document.getElementById("pos").innerHTML="Added new beast " + index + " at (" + x + ", " + y + ")";
        draw();
	setTimeout(function() { move(); }, 250);
}

function in_bag(x_pos, y_pos, left, right, up, down) {
	return (x_pos > left) && (x_pos < right) && (y_pos > up) && (y_pos < down);
}

function draw() {
	var c=document.getElementById("myCanvas");
	var ctx=c.getContext("2d");

	ctx.clearRect(0, 0, c.width, c.height); 
	ctx.fillStyle="#E0B044";
	ctx.fillRect(left, up, bag_width, bag_width);
	
	for (var i = 0; i < ids.length; i++) {
		var beast = ids[i];
		if (typeof beast.x === "undefined") 
			alert("something is undefined");
		
		ctx.beginPath();
		ctx.rect(beast.x, beast.y, width, width);
		ctx.strokeStyle="black";
		ctx.stroke();
	}
}

function distance_index(distance, index) {
	this.distance = distance;
	this.index = index;
}

function knn(items, index, n) {
	//unit test this
	//what if there's a tie?
	var beast = items[index];

	var results =[]; 
	for (var i=0; i<items.length; i++) {
		if (i !==index) {
			var neighbour = items[i];
			var distance = Math.sqrt(neighbour.x*neighbour.x + neighbour.y*neighbour.y);
			results.push( new distance_index(distance, i) );
		}
	}
	results.sort( function(a,b) { return a.distance - b.distance; } );
	var top_n = Math.min(n, results.length);//maybe not needed
	return results.slice(0,top_n);
}

//combine these two neatly: something more functional?
function x_move(items) {
	var x = 0, weight = 2;
	if (items.length === 0)
		return x;
	for (var i=0; i<items.length; i++) {
		var beast = ids[items[i].index];
		if (beast.id === 0) {
			x += beast.x/weight;
		}
		else {
			x += beast.x;
		}
		
	}
	return x / items.length;
}

function y_move(items) {
	var min = -1;
	if (items.length === 0) {
		return 0;
	}
	for (var i=0; i<items.length; i++) {
		var beast = ids[items[i].index];
		if (min === -1 || beast.y <= min) {
			min = beast.y;
		}
	}
	if (min === -1) {
		return 0;
	} else {
		return min;
	}
}

function move_ant(index) {
	if (index>ids.length-1)
		alert("not ready");

	var beast = ids[index];
	var weight = 0.01;
	var new_x_move = bag_width * weight * (-0.5 + Math.random());
	var new_y_move = -bag_width * weight * Math.random();//just send it up

	var weighting = Math.min(5, ids.length - 1);//experiment
	var items = knn(ids, index, weighting);
	var neighbour_weight = 0.1;
	var x_nudge = x_move(items);
	new_x_move += (x_nudge - beast.x) * neighbour_weight * (-0.5 + Math.random());
	var y_nudge = y_move(items);
	// why does it move up so much?
	new_y_move -= (beast.y - y_nudge) * neighbour_weight;//subtract if 0 is the best
	//new_y_move -= y_nudge;

	var c=document.getElementById("myCanvas");
	beast.x += new_x_move;
	beast.x %= c.width;
	beast.y += new_y_move;
	beast.y %= c.height;//wrong - can spang it out of the bag
	ids[beast.index] = beast;
}

function move() {
	var i, finished = true, beast;
	for(i = 0; i < ids.length; ++i) {
		move_ant(i);
	}
	draw();
	for(i = 0; i < ids.length; ++i) {
		beast = ids[i];
		if (!in_bag(beast.x, beast.y, left, right, up, down)) {
			document.getElementById("Go").innerHTML="Start";
			document.getElementById("demo").innerHTML="Success for beasty " + i;
		}
		else {
			finished = false;
		}
	}
	if (!finished) {
		id = setTimeout(function() { move(); }, 250);
	}
}
