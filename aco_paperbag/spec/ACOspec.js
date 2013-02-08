describe("next_pos", function() {

  it("should not be below bag", function() {
    var width = 4;
    for(var i=0; i<width; ++i)
    {
      var pos = { x:i, y:0 };
      var next = next_pos(width, pos, []);
      expect(next.y >= 0).toBe(true);
    }
  });


  it("should not be beside bag", function() {
    var width = 4;
    for(var i=0; i<width; ++i)
    {
      var pos = { x:i, y:0 };
      var next = next_pos(width, pos, []);
      expect(next.x >= 0).toBe(true);
      expect(next.x < width).toBe(true);
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
    var trail = random_trail(width, height);
    expect(trail.length>0).toBe(true);
    expect(trail[0].y === 0).toBe(true);
  });

  it("should end above bag", function() {
    var width = 4;
    var height = 3;
    var trail = random_trail(width, height);
    expect(trail.length>0).toBe(true);
    expect(trail[trail.length-1].y>=height).toBe(true);
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

describe("find_best", function() {

  it("should find the only trail if there is just one", function() {
    var trails = [];
    trails.push( [ 2, 3] );
    expect(find_best(trails)).toBe(0);
  });

  it("should find the shortest trail when it is first", function() {
    var trails = [];
    trails.push( [2, 3] );
    trails.push( [2, 3, 4, 5] );
    expect(find_best(trails)).toBe(0);
  });

  it("should find the shortest trail when it is last", function() {
    var trails = [];
    trails.push( [2, 3, 4, 5] );
    trails.push( [2, 3] );
    expect(find_best(trails)).toBe(1);
  });

});

describe("properties_match", function() {

  it("should be true for matching positions", function() {
    var pos = { x: 0, y: 1};
    var matching_pos = { x: 0, y: 1};
    expect(properties_match(pos, matching_pos, "x")).toBe(true);
    expect(properties_match(pos, matching_pos, "y")).toBe(true);
  });

  it("should be flase for non-matching positions", function() {
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
    //trails.push( pos );
    trails.push( {x: 0, y:1 } ); //I'm comparing the ref: bug!
    expect(contains(trails, pos)).toBe(true);
  });

});

describe("add_new_pheromones", function () {

  it("should contain each point in a new trail", function() {
    var found, i, pos, pheromones = [], updated;
    var trail = random_trail(5, 6);
    updated = add_new_pheromones(pheromones, trail);
    for( i = 0; i < trail.length; ++i) {
      pos = trail[i];
      found = false;
      for(j = 0; j<updated.length; ++j) {
        if ((pos.x === updated[j].x) && (pos.y === updated[j].y)) {
          found = true;
        }
      }
      expect(found).toBe(true);
    }
  });

});


describe("nearest_pheromones", function() {

  it("should find the only item if just one exists", function() {
    //notice this says the nearest one is where it already is - watch it
    var pheromones = [];
    var pos = {x: 1, y: 2, weight: 1};
    pheromones.push(pos);
    var lookup = nearest_pheromone(pheromones, pos);
    var result = pheromones[lookup];
    expect(result === pos).toBe(true);
  });

  it("should report -1 if none are near enough", function() {
    var pheromones = [], pos = {x: 0, y: 0};

    pheromones.push({x: 1, y: 0, weight: 0});
    pheromones.push({x: 0, y: 1, weight: 1});
    pheromones.push({x: 1, y: 1, weight: 0});
    
    var lookup = nearest_pheromone(pheromones, pos);
    expect(lookup).toEqual(-1);
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
    var width = 5, i, pos = {x: 0, y: 0}, pheromones = [], updated;
    var trail = random_trail(6, width);
    updated = add_new_pheromones(pheromones, trail);
    var new_pos = roulette_wheel_choice(width, pos, trail, updated);
    expect(new_pos.x >= 0).toBe(true);
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


