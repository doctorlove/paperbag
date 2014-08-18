describe("best", function() {

  it("should return the highest particle", function() {
    low  = { x: 5, y: 0 };
    high = { x: 5, y: 8 };
    expect(best(low, high).y).toBe(8);//a rubbish way to test, perhaps?
  });

});

describe("getRandomInt", function() {
  it("should return a number in [min, max)", function() {
    var value = getRandomInt(0, 11);
    expect(value >=  0).toBe(true);
    expect(value <  11).toBe(true);
  });
});

describe("initialise", function() {
  it("should return the requesed number of particles", function() {
    var width = 250;
    var height = 250;
    expect(initialise(1, width, height).length == 1).toBe(true);
    expect(initialise(2, width, height).length == 2).toBe(true);
    expect(initialise(3, width, height).length == 3).toBe(true);
  });

  it("should set best to initial position", function() {
    var width = 250;
    var height = 250;
    var item = initialise(3, width, height);
    var i;
    for(i=0; i<item.length; ++i) {
      var current = item[i];
      //Which equals do I mean anyway?
      expect(item[i].best.x).toEqual(item[i].x);
      expect(item[i].best.y).toEqual(item[i].y);
      expect(current.best.x).toEqual(current.x);
      expect(current.best.y).toEqual(current.y);
    }
  });
});

describe("move", function() {
  it("should keep x inside width of bag for each step", function() {
    var width = 250;
    item = { x: 0, y: 0, best: {x:0, y:0}, v: { x: 1, y: 1} };
    move_in_range(-10, width, item, "x");
    expect(item.x).toEqual(0);
  });
  it("should keep y inside width of bag for each step", function() {
    var height = 20;
    item = { x: 0, y: 0, best: {x:0, y:0}, v: { x: 1, y: 1} };
    move_in_range(100, height, item, "y");
    expect(item.y).toEqual(height);
  });
  it("should move y if possible", function() {
    var height = 20;
    item = { x: 0, y: 0, best: {x:0, y:0}, v: { x: 1, y: 1} };
    move_in_range(1, height, item, "y");
    expect(item.y).toEqual(1);
  });
  it("should move x if possible", function() {
    var width = 20;
    item = { x: 0, y: 0, best: {x:0, y:0}, v: { x: 1, y: 1} };
    move_in_range(1, width, item, "x");
    expect(item.x).toEqual(1);
  });
});

describe("initialise", function() {
  it("should return the requesed number of particles", function() {
    var width = 250;
    var height = 250;
    expect(initialise(1, width, height).length == 1).toBe(true);
    expect(initialise(2, width, height).length == 2).toBe(true);
    expect(initialise(3, width, height).length == 3).toBe(true);
  });

  it("should set best to initial position", function() {
    var width = 250;
    var height = 250;
    var item = initialise(3, width, height);
    var i;
    for(i=0; i<item.length; ++i) {
      var current = item[i];
      //Which equals do I mean anyway?
      expect(item[i].best.x).toEqual(item[i].x);
      expect(item[i].best.y).toEqual(item[i].y);
      expect(current.best.x).toEqual(current.x);
      expect(current.best.y).toEqual(current.y);
    }
  });
});


