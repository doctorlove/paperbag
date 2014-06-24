describe("best", function() {

  it("should return the highest particle", function() {
    low  = { x: 5, y: 0 };
    high = { x: 5, y: 8 };
    expect(best(low, high).y).toBe(8);//a rubbish way to test, perhaps?
  });



});

