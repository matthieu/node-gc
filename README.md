Events from the V8 garbage collector, lets you know when garbage collection occurs.

# Usage

Just subscribe to events specific to each type of garbage collection ('scavenge' and 'marksweep' for scavenging and mark, sweep and collect respectively):

```javascript
var nodegc = require('node-gc');

nodegc.on('scavenge', function(info) {
  // scavenging just happened
});
nodegc.on('marksweep', function(info) {
  // got marked and sweeped
});
```

The passed info object has the following properties:

- duration: How much time was spent collecting. Note that this will in general be slightly higher than what's reported by --trace-gc.
- heapBefore: heap size in bytes before the collection occured.
- heapAfter: heap size in bytes after the collection occured.

The main application of node-gc is to log or graph GC activity. The V8 --trace-gc being just a low-level print, it offers little opportunity for analysis. Plug-in statds and graphite for best effect. You may also want to use it to apply back pressure on memory allocation if that's your thing.

# Installation

The usual:

```
npm install node-gc
```

# Development

Test cases use mocha and should. More test cases, patches and pull requests are all welcome. Licensed under the Apache 2.0 license.
