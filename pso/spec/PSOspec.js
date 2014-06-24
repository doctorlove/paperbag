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

