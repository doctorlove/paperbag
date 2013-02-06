var bag_width = 300;
var width = 4;
var left = 75;
var right = left + bag_width;
var up = 25;
var down = up + bag_width;

function Beasty(x, y, id, index)
{
	this.x = x;
	this.y = y;
	this.id = id;
	this.index = index;
}

var ids =  [];

function init()
{
	action(0);
}

function action(id) {
	if (id === 0) {
		var x = 175;
		var y = 150;
		var index = ids.length;
		id = setInterval(function() { move(index); }, 100); 
		var beast = new Beasty(x, y, id, index);
		ids.push(beast);
	}
	else {
		for (var i=0; i<ids.length; i++) {
			var beast = ids[i];
			clearInterval(beast.id);
		}
		ids.length = 0;//or do I want to keep escaped ones... maybe if they just escape from the same side
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

function move(index) {
	if(index>ids.length-1)
		alert("not ready");

	beast = ids[index];

	var c=document.getElementById("myCanvas");
	var new_x_move = bag_width * 0.2 * (-0.5 + Math.random());
	var new_y_move = bag_width * 0.2 * (-0.5 + Math.random());
	beast.x += new_x_move;//By ref or value?
	beast.x %= c.width;
	beast.y += new_y_move;
	beast.y %= c.height;
	ids[beast.index] = beast;//is this needed... by ref or value?
	draw();

	if (!in_bag(beast.x, beast.y, left, right, up, down)) {
		document.getElementById("Go").innerHTML="Start";
		document.getElementById("demo").innerHTML="Success";//which one?
		action(beast.id);
	}
}
