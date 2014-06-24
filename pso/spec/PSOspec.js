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
    expect(initialise(1, width).length == 1).toBe(true);
    expect(initialise(2, width).length == 2).toBe(true);
    expect(initialise(3, width).length == 3).toBe(true);
  });

  it("should set best to initial position", function() {
    var width = 250;
    var item = initialise(3, width);
    var i;
    for(i=0; i<item.length; ++i) {
      var current = item[i];
      //Which equals do I mean anyway?
      expect(item[i].best.x == item[i].x).toBe(true);
      expect(item[i].best.y == item[i].y).toBe(true);
      expect(current.best.x == current.x).toBe(true);
      expect(current.best.y == current.y).toBe(true);
    }
  });
});

