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


  it("should not return to a previous point", function() {
    expect(true).toBe(true);
    var width = 4;
    var pos = { x:0, y:1 };
    var trail = [];
    var old_pos = { x:0, y:1 };
    trail.push( old_pos );
    var new_pos = next_pos(width, pos, trail);
    expect(contains(trail, new_pos)).toBe(false);
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

  it("should find the only trail is there is just one", function() {
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

describe("contains", function() {

  it("should not contain a item when it is empty", function() {
    var trails = []
    var pos = { x: 0, y: 1};
    expect(contains(trails, pos)).toBe(false);
  });

  it("should contain a item when it is the only item", function() {
    var trails = []
    var pos = { x: 0, y: 1};
    trails.push( pos );
    expect(contains(trails, pos)).toBe(true);
  });

});

describe("update_pheromones", function () {

  it("should contain each point in a new trail", function() {
    var found, i, pos, pheromones = [], updated;
    var trail = random_trail(5, 6);
    updated = update_pheromones(pheromones, trail);
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

describe("cumulative_weights", function() {

  it("should give the cumulative sums of the weights", function() {
    var pheromones = [];
    var pos = {x: 1, y: 2, weight: 1};
    pheromones.push(pos);
    var possible = [];
    possible.push( { x: 1, y:2 } );
    var cumulative = cumulative_weights(possible, pheromones); 
    expect(cumulative.length === 2).toBe(true);
    expect(cumulative[0]).toEqual(0);
    expect(cumulative[1]).toEqual(1);
  });

  it("should give total of one when there is a one and the rest are zero", function() {
    var pheromones = [], possible = [];

    pheromones.push({x: 0, y: 1, weight: 1});
    pheromones.push({x: 1, y: 1, weight: 0});
    pheromones.push({x: 1, y: 0, weight: 0});

    possible.push( { x: 0, y: 1 } );
    possible.push( { x: 1, y: 1 } );
    possible.push( { x: 1, y: 0 } );

    var cumulative = cumulative_weights(possible, pheromones); 
    expect(cumulative.length === 4).toBe(true);
    expect(cumulative[0]).toEqual(0);
    expect(cumulative[1]).toEqual(1);
    expect(cumulative[2]).toEqual(1);
    expect(cumulative[3]).toEqual(1);
  });

});

describe("roulette_wheel_choice", function() {

  it("should return a position", function() {
    var width = 5, i, pos = {x: 0, y: 0}, pheromones = [], updated;
    var trail = random_trail(width, 6);
    updated = update_pheromones(pheromones, trail);
    var new_pos = roulette_wheel_choice(width, pos, trail, updated);
    expect(new_pos.x >= 0).toBe(true);
  });

  it("should go to non-zero pheromones point if all other pheromones are 0", function() {
    //the randomness makes this fail sometimes
    var pheromones = [], possible = [], trail = [], pos = {x: 0, y: 0};

    pheromones.push({x: 0, y: 1, weight: 1});
    pheromones.push({x: 1, y: 1, weight: 0});
    pheromones.push({x: 1, y: 0, weight: 0});

    possible.push( { x: 0, y: 1 } );
    possible.push( { x: 1, y: 1 } );
    possible.push( { x: 1, y: 0 } );

    trail.push( { x: 0, y: 0 } );

    var new_pos = roulette_wheel_choice(4, pos, trail, pheromones);
    expect(new_pos.x).toEqual(0);
    expect(new_pos.y).toEqual(1);
  });

});


