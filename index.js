var nodegc = require('./build/Release/nodegc');
var events = require('events');

var gcEmitter = new events.EventEmitter();
nodegc.gcCallback(function(gctype, gctime, heapBefore, heapAfter){
  gcEmitter.emit(gctype, {duration: gctime, heapBefore: heapBefore, heapAfter: heapAfter});
});

module.exports = gcEmitter;
