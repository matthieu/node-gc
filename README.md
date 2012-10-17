Events from the V8 garbage collector, lets you know when garbage collection occurs. Just subscribe to events specific to each type of garbage collection ('scavenge' and 'marksweep' for scavenging and mark, sweep and collect respectively):

```javascript
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
- 
