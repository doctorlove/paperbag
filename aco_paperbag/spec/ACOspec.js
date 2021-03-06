describe("next_pos", function() {

  //using (x,y) mathematically - so 0 is at the bottom
  //the drawing code flips them - as 0 is at the top on the canvas

  it("should not be below bag", function() {
    var width = 4;
    for(var i=0; i<width; ++i)
    {
      var pos = { x:i, y:0 };
      var next = next_pos(width, pos, []);
      expect(next.y).not.toBeLessThan(0);
    }
  });


  it("should not be beside bag", function() {
    var width = 4;
    for(var i=0; i<width; ++i)
    {
      var pos = { x:i, y:0 };
      var next = next_pos(width, pos, []);
      expect(next.x >= 0).toBe(true);
      expect(next.x).toBeLessThan(width);
    }
  });


  it("should not return to a current point", function() {
    expect(true).toBe(true);
    var width = 4;
    var pos = { x:0, y:1 };
    var trail = [];
    var old_pos = { x:0, y:1 };
    trail.push( old_pos );
    var new_pos = next_pos(width, pos, trail);
    expect(new_pos.x === 0 && new_pos.y === 1).toBe(false);
  });

  it("should not return to a previous point", function() {
    expect(true).toBe(true);
    var width = 4;
    var pos = { x:0, y:0 };
    var trail = [];
    trail.push( {x:0, y:0 } );
    trail.push( {x:0, y:1 } );
    trail.push( {x:1, y:0 } );

    var new_pos = next_pos(width, pos, trail);
    expect(new_pos.x).toEqual(1);
    expect(new_pos.y).toEqual(1);
  });


});

describe("allowed_positions", function() {

  it("should not go to a position that has already been visited", function() {
    var pheromones = [], trail = [], pos = {x: 0, y: 0};
 
    pheromones.push({x: 0, y: 1, weight: 1});
    pheromones.push({x: 1, y: 0, weight: 0});

    trail.push( { x: 0, y: 0 } );
    trail.push( { x: 0, y: 1 } );

    var allowed = allowed_positions(4, pos, trail);
    expect(allowed.length).toEqual(2);
  });
});

describe("random_trail", function() {

  it("should start at bottom of bag", function() {
    var width = 4;
    var height = 3;
    var trail = random_trail(height, width);
    expect(trail.length>0).toBe(true);
    expect(trail[0].y).toEqual(0);
  });

  it("should end above bag", function() {
    var width = 4;
    var height = 3;
    var trail = random_trail(height, width);
    expect(trail.length).toBeGreaterThan(0);
    expect(trail[trail.length-1].y).not.toBeLessThan(height);
  });

});

describe("make_trails", function() {

  it("should return same number of trails as ants", function() {
    var height = 5;
    var width = 5;
    var ants = 10;
    var trails = make_trails(height, width, ants);
    expect(trails.length).toBe(ants);
  });

});

describe("total_length", function() {
  it("will see a straight line as shorter than a diagonal line", function() {
    trail_shorter = [];
    trail_shorter.push({x: 0, y: 1});
    trail_shorter.push({x: 0, y: 0});

    trail_longer = [];
    trail_longer.push({x: 1, y: 1});
    trail_longer.push({x: 0, y: 0});

    var short = total_length(trail_shorter);
    var long = total_length(trail_longer);
    expect(short).toBeLessThan(long);
  });

  it("will see a straight line of three steps as length three", function() {
    trail = [];
    trail.push({x: 0, y: 3});
    trail.push({x: 0, y: 0});

    var length = total_length(trail);
    expect(length).toEqual(3);
  });

  it("will be greater than zero for a path of several steps", function() {
    var trails = [];
    trails.push( [{x:120, y:260},  {x:150, y:230},  {x:180, y:200},  {x:180, y:170},  {x:150, y:140},  {x:150, y:110},  {x:150, y:80},  {x:180, y:50},  {x:180, y:20},  {x:150, y:-10}] );
    expect(total_length(trails[0])).toBeGreaterThan(0);
  });
});

describe("find_best", function() {

  it("should find the only trail if there is just one", function() {
    var trails = [];
    trails.push( [{x:2, y:0}, {x:3, y:0}] );
    expect(find_best(trails)).toBe(0);
  });

  it("should find the shortest trail when it is first", function() {
    var trails = [];
    trails.push( [{x:2, y:0}, {x:3, y:0}] );
    trails.push( [{x:2, y:0}, {x:3, y:0}, {x:4, y:0}, {x:5, y:0}] );
    expect(find_best(trails)).toBe(0);
  });

  it("should find the shortest trail when it is last", function() {
    var trails = [];
    trails.push( [{x:2, y:0}, {x:3, y:0}, {x:4, y:0}, {x:5, y:0}] );
    trails.push( [{x:2, y:0}, {x:3, y:0}] );
    expect(find_best(trails)).toBe(1);
  });
});

describe("find_worst", function() {

  it("should find the only trail if there is just one", function() {
    var trails = [];
    trails.push( [{x:2, y:0}, {x:3, y:0}] );
    expect(find_worst(trails)).toBe(0);
  });

  it("should find the longest trail when it is first", function() {
    var trails = [];
    trails.push( [{x:2, y:0}, {x:3, y:0}, {x:4, y:0}, {x:5, y:0}] );
    trails.push( [{x:2, y:0}, {x:3, y:0}] );
    expect(find_worst(trails)).toBe(0);
  });

  it("should find the longest trail when it is last", function() {
    var trails = [];
    trails.push( [{x:2, y:0}, {x:3, y:0}] );
    trails.push( [{x:2, y:0}, {x:3, y:0}, {x:4, y:0}, {x:5, y:0}] );
    expect(find_worst(trails)).toBe(1);
  });
});

describe("find_average", function() {

  it("should return length of trail if there is just one just one", function() {
    var trails = [];
    trails.push( [{x:2, y:0}, {x:3, y:0}] );
    expect(find_average(trails)).toEqual(total_length(trails[0]));
  });

  it("should return length of trail if there all the same length", function() {
    var trails = [];
    trails.push( [{x:2, y:0}, {x:3, y:0}] );
    trails.push( [{x:2, y:0}, {x:3, y:0}] );
    trails.push( [{x:2, y:0}, {x:3, y:0}] );
    trails.push( [{x:2, y:0}, {x:3, y:0}] );
    expect(find_average(trails)).toEqual(total_length(trails[0]));
    trails.push( [{x:2, y:0}, {x:3, y:0}] );
    expect(find_average(trails)).toEqual(total_length(trails[0]));
  });

  it("should match best and worst for a single path", function() {
    var trails = [];
    trails.push( [{x:120, y:260},  {x:150, y:230},  {x:180, y:200},  {x:180, y:170},  {x:150, y:140},  {x:150, y:110},  {x:150, y:80},  {x:180, y:50},  {x:180, y:20},  {x:150, y:-10}] );
    var average = find_average(trails);
    expect(average).toEqual(total_length(trails[find_best(trails)]));
    expect(average).toEqual(total_length(trails[find_worst(trails)]));
  });
});

describe("properties_match", function() {

  it("should be true for matching positions", function() {
    var pos = { x: 0, y: 1};
    var matching_pos = { x: 0, y: 1};
    expect(properties_match(pos, matching_pos, "x")).toBe(true);
    expect(properties_match(pos, matching_pos, "y")).toBe(true);
  });

  it("should be false for non-matching positions", function() {
    var pos = { x: 0, y: 1};
    var matching_pos = { x: 1, y: 1};
    expect(properties_match(pos, matching_pos, "x")).toBe(false);
    expect(properties_match(pos, matching_pos, "y")).toBe(true);
  });
});


describe("contains", function() {

  it("should not contain a item when it is empty", function() {
    var trails = []
    var pos = { x: 0, y: 1};
    expect(contains(trails, pos)).toBe(false);
  });

  it("should contain a item when it is the only item", function() {
    var trails = []
    var pos = { x: 0, y: 1};
    trails.push( {x: 0, y:1 } ); 
    expect(contains(trails, pos)).toBe(true);
  });

});

describe("add_new_pheromones", function () {

  it("should contain each point in a new trail", function() {
    var i, pos, pheromones = [], updated;
    var width = 4;
    var height = 6;
    var trail = random_trail(height, width);
    add_new_pheromones(height, pheromones, trail);
    //there may be some repeated points in the trail
    expect(pheromones.length).not.toBeGreaterThan(trail.length);

    for( i = 0; i < trail.length; ++i) {
      pos = trail[i];
      for(j = 0; j<pheromones.length; ++j) {
        expect(contains(pheromones, pos)).toBe(true);
      }
    }
  });
});

describe("tau eta", function () {
  //passed when alhpa 1, beta 4 - so not totally convinced
  it("should be larger for each higher up point", function() {
    var found, i, pos, pheromones = [];
    var width = 4;
    var height = 6;
    var trail = [ { x: 2, y: 0 }, { x: 1, y: 0 }, { x: 0, y: 1 }, { x: 0, y: 0 }, { x: 1, y: 1 }, { x: 1, y: 2 }, { x: 1, y: 3 }, { x: 2, y: 3 }, { x: 2, y: 2 }, { x: 3, y: 2 }, { x: 2, y: 1 }, { x: 3, y: 0 }, { x: 3, y: 1 }, { x: 2, y: 1 }, { x: 1, y: 2 }, { x: 0, y: 2 }, { x: 0, y: 3 }, { x: 1, y: 4 }, { x: 2, y: 5 }, { x: 2, y: 4 }, { x: 3, y: 4 }, { x: 3, y: 3 }, { x: 3, y: 2 }, { x: 2, y: 1 }, { x: 3, y: 2 }, { x: 3, y: 3 }, { x: 2, y: 4 }, { x: 1, y: 5 }, { x: 0, y: 6 } ]; 
    add_new_pheromones(height, pheromones, trail);
    for( i = 0; i < pheromones.length; ++i) {
      for(j = 0; j<pheromones.length; ++j) {
        if (pheromones[i].y > pheromones[j].y) {
	  var high = taueta(pheromones[i].weight, pheromones[i].y);
	  var low = taueta(pheromones[j].weight, pheromones[j].y);
          expect(high).not.toBeLessThan(low);
        } 
      }
    }
  });
});

describe("evapourate", function() {
  it("should decrease the pheromones", function() {
    var original_value= 100;
    var pheromones = [{x:0, y:0, weight:original_value}];
    evapourate(pheromones);
    expect(pheromones[0].weight).toBeLessThan(original_value);
  });
});

describe("update", function() {
  it("should add pheromone where ant went", function() {
    var height = 5;
    var width = 5;
    var ants = 1;
    var trail = make_trails(height, width, ants);
    var pheromones = [];
    update(pheromones, trail, height, width);
    for (i = 0; i < trail.length; ++i) {
      pos = trail[0][i];
      index = pheromone_at(pheromones, pos);
      expect(index).not.toBe(-1);
      if ( index !== -1 ) {
        expect(pheromones[index].weight).toBeGreaterThan(0);
      }
    }
  });

  it("should not add pheromone where ant did not go", function() {
    var height = 5;
    var width = 5;
    var ants = 1;
    var trail = make_trails(height, width, ants);
    var pheromones = [];
    update(pheromones, trail, height, width);
    for (i = 0; i < pheromones.length; ++i) {
      pos = pheromones[i];
      expect(contains(trail[0], pos)).toBe(true);
    }
  });
});

describe("find pheromone", function() {
  it("should return -1 when none at that point", function () {
    var pheromones = [];
    var pos = {x: 1, y: 2, weight: 1};
    var index = pheromone_at(pheromones, pos);
    expect(index === -1).toBe(true);
  });

  it("should return point's index when point is there", function () {
    var pheromones = [];
    var pos = {x: 1, y: 2, weight: 1};
    pheromones.push(pos);
    var index = pheromone_at(pheromones, pos);
    expect(index === 0).toBe(true);
  });

});

describe("cumulative_probability", function() {

  it("should give the sum of tau eta when there is one point", function() {
    var pheromones = [];
    var pos = {x: 1, y: 2, weight: 1};
    pheromones.push(pos);
    var possible = [];
    possible.push( { x: 1, y:2 } );
    var cumulative = cumulative_probability(possible, pheromones); 
    expect(cumulative.length === 2).toBe(true);
    expect(cumulative[0]).toEqual(0);
    expect(cumulative[1]).toEqual(taueta(pos.weight, pos.y));
  });

  it("should give the sum of tau eta when there is a non-zero weight and the rest are zero", function() {
    var pheromones = [], possible = [];

    pheromones.push({x: 0, y: 1, weight: 1});
    pheromones.push({x: 1, y: 1, weight: 0});
    pheromones.push({x: 1, y: 0, weight: 0});

    possible.push( { x: 0, y: 1 } );
    possible.push( { x: 1, y: 1 } );
    possible.push( { x: 1, y: 0 } );

    var cumulative = cumulative_probability(possible, pheromones);
    expect(cumulative.length === 4).toBe(true);
    expect(cumulative[0]).toEqual(0);
    expect(cumulative[1]).toEqual(taueta(1,1));
    expect(cumulative[2]).toEqual(taueta(1,1) + taueta(0,1));
    expect(cumulative[3]).toEqual(taueta(1,1) + taueta(0,1) + taueta(0,0));
  });

});

describe("roulette_wheel_choice", function() {

  it("should return a position", function() {
    var width = 5, height = 6, i, pos = {x: 0, y: 0}, pheromones = [];
    var trail = random_trail(height, width);
    add_new_pheromones(height, pheromones, trail);
    var new_pos = roulette_wheel_choice(width, pos, trail, pheromones);
    expect(new_pos.x).not.toBeLessThan(0);
  });

  it("should go to best pheromone point if all other points have zero probability", function() {
    var pheromones = [], possible = [], trail = [], pos = {x: 0, y: 0};

    pheromones.push({x: 0, y: 1, weight: 1});//best weight and height
    pheromones.push({x: 1, y: 0, weight: 0});

    trail.push( { x: 0, y: 0 } );

    var new_pos = roulette_wheel_choice(4, pos, trail, pheromones);
    expect(new_pos.x).toEqual(0);
    expect(new_pos.y).toEqual(1);
  });

  it("should go to a better pheromone point if some other points have zero probability", function() {
    var pheromones = [], possible = [], trail = [], pos = {x: 0, y: 0};

    pheromones.push({x: 0, y: 0, weight: 0});
    pheromones.push({x: 1, y: 0, weight: 0});
    pheromones.push({x: 2, y: 0, weight: 0});
    pheromones.push({x: 0, y: 1, weight: 1});//equal best weight and height
    pheromones.push({x: 1, y: 1, weight: 1});//equal best weight and height

    var new_pos = roulette_wheel_choice(4, pos, trail, pheromones);
    expect(new_pos.y).toEqual(1);//first two have 0 probablility
  })

  it("should not go to a place already visited", function() {
    var pheromones = [], trail = [], pos = {x: 0, y: 0};

    pheromones.push({x: 0, y: 1, weight: 1});
    pheromones.push({x: 1, y: 0, weight: 0});

    trail.push( { x: 0, y: 0 } );
    trail.push( { x: 0, y: 1 } );
    trail.push( { x: 1, y: 1 } );

    var new_pos = roulette_wheel_choice(4, pos, trail, pheromones);
    expect(new_pos.x).toEqual(1);
    expect(new_pos.y).toEqual(0);
  });
});


