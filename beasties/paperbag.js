var x = 150;
var y = 150;
var width = 4;

var id = 0;

function init() {
	if (id === 0) {
		id = setInterval(update, 100); 
	}
	else {
		clearInterval(id);
		id = 0;
		x = 150;
		y = 150;
	}
}

function in_bag(x_pos, y_pos, left, right, up, down) {
	return (x_pos > left) && (x_pos < right) && (y_pos > up) && (y_pos < down);
}

function update() {
	//draw the bag
	var c=document.getElementById("myCanvas");
	var ctx=c.getContext("2d");
	
	ctx.clearRect(0,0,c.width,c.height); 
	ctx.fillStyle="#E0B044";
	var bag_width = 300;
	var left = 75;
	var right = left + bag_width;
	var up = 25;
	var down = up + bag_width;
	ctx.fillRect(left,up,bag_width,bag_width);
	
        //Make this a move function...
	var new_x_move = bag_width * 0.2 * (-0.5 + Math.random());
	var new_y_move = bag_width * 0.2 * (-0.5 + Math.random());
	x += new_x_move;
	y += new_y_move;
	
	ctx.beginPath();
	ctx.rect(x,y,width,width);
	ctx.strokeStyle="black";
	ctx.stroke();
	if (in_bag(x, y, left, right, up, down)) {
		document.getElementById("Go").innerHTML="Stop";
		document.getElementById("demo").innerHTML=new_x_move + ", " + new_y_move;
	}
	else {
		document.getElementById("Go").innerHTML="Start";
		document.getElementById("demo").innerHTML="Success";
		init();
	}
}
