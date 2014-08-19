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
	this.index = index;
}

var ids = []; 

function init() {
	action(0);
}

function action(id) {
	var beast;
	if (id === 0) {
		var x = left + 0.5 * bag_width + Math.random();
		var y = up + 0.5 * bag_width + Math.random();
		var index = ids.length;
		id = setInterval(function() { move(index); }, 150); 
		beast = new Beasty(x, y, id, index);
		ids.push(beast);
		document.getElementById("demo").innerHTML="Added new beast " + index;
	}
	else {
				clearInterval(id);
	}
}

function in_bag(x_pos, y_pos, left, right, up, down) {
	return (x_pos > left) && (x_pos < right) && (y_pos > up) && (y_pos < down);
}

function draw() {
	var c=document.getElementById("myCanvas");
	var ctx=c.getContext("2d");

	ctx.clearRect(0,0,c.width,c.height); 
	ctx.fillStyle="#E0B044";
	ctx.fillRect(left,up,bag_width,bag_width);
	
	for (var i=0; i<ids.length; i++) {
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
	var results =[]; 
	var item = items[index];
	for (var i=0; i<items.length; i++) {
		if (i !==index) {
			var neighbour = items[i];
			var distance = Math.sqrt(item.x*neighbour.x + item.y*neighbour.y);
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
	var y = 0, weight = 2;
	if (items.length === 0)
		return y;
	for (var i=0; i<items.length; i++) {
		var beast = ids[items[i].index];
		if (beast.id === 0) {
			y += beast.y/weight;
		}
		else {
			y += beast.y;
		}
	}
	return y / items.length;
}

function move(index) {
	if (index>ids.length-1)
		alert("not ready");

	var beast = ids[index];
	var weight = 0.01;
	var new_x_move = bag_width * weight * (-0.5 + Math.random());
	var new_y_move = bag_width * weight * (-0.5 + Math.random());

	var weighting = Math.min(5, ids.length - 1);//experiment
	var items = knn(ids, index, weighting);
	var neighbour_weight = 0.8;
	var x_nudge = x_move(items);
	new_x_move += (x_nudge - beast.x) * neighbour_weight * (-0.5 + Math.random());
	var y_nudge = y_move(items);
	new_y_move += (y_nudge - beast.y) * neighbour_weight * (-0.5 + Math.random());

	var c=document.getElementById("myCanvas");
	beast.x += new_x_move;
	beast.x %= c.width;
	beast.y += new_y_move;
	beast.y %= c.height;
	ids[beast.index] = beast;
	draw();

	if (!in_bag(beast.x, beast.y, left, right, up, down)) {
		document.getElementById("Go").innerHTML="Start";
		document.getElementById("demo").innerHTML="Success for beasty " + index;
		action(beast.id);
	}
}
