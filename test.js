var nodegc = require('./index');
var should = require('should');

var scavenge = null, marksweep = null;
nodegc.on('scavenge', function(info) {
  scavenge = info;
});
nodegc.on('marksweep', function(info) {
  marksweep = info;
});

describe("V8 GC events", function() {

  before(function() {
  });
  
  it('should fire some scavenge events', function(done) {
    for (var m = 0; m < 1000; m++) {
      var a = [];
      for (var o = 0; o < 1000; o++) {
        a.push('foo'+o);
      }
    }
    setTimeout(function() {
      console.log(scavenge);
      scavenge.should.not.be.null;
      scavenge.duration.should.be.above(0);
      scavenge.duration.should.be.below(1000);
      scavenge.heapBefore.should.be.above(1000);
      scavenge.heapAfter.should.be.above(1000);
      done()
    }, 100);
  });

  it('should fire some mark&sweep events after applying some memory pressure', function(done) {
    this.timeout(5000);
    var all = []
    for (var m = 0; m < 1000; m++) {
      var a = [];
      for (var o = 0; o < 10000; o++) {
        a.push('foo'+o);
        all.push('bar'+o);
      }
    }
    setTimeout(function() {
      console.log(marksweep);
      marksweep.should.not.be.null;
      marksweep.duration.should.be.above(0);
      marksweep.duration.should.be.below(1000);
      marksweep.heapBefore.should.be.above(1000);
      marksweep.heapAfter.should.be.above(1000);
      done()
    }, 100);
  });
});
