var nodegc = require('./build/Release/nodegc');
var events = require('events');

// Native calls are forwarded through an event emitter, one event for each type of garbage
// collection: scavenge or marksweep.
var gcEmitter = new events.EventEmitter();
nodegc.gcCallback(function(gctype, gctime, heapBefore, heapAfter){ 
  gcEmitter.emit(gctype, {duration: gctime, heapBefore: heapBefore, heapAfter: heapAfter});
});

module.exports = gcEmitter;
