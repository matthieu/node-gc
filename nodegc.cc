#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <string>
#include <iostream>
#include <sys/time.h>

using namespace v8;

struct Baton {
    uv_work_t request;
    Persistent<Function> callback;
    int error_code;
    std::string error_message;

    long timeBefore;
    size_t heapSizeBefore;
    v8::GCType gctype;
};

namespace nodegc {
    Baton baton;
};

long currentTimeMillis() {
    timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}

size_t heapSize() {
    HeapStatistics heapStatistics;
    V8::GetHeapStatistics(&heapStatistics);
    return heapStatistics.used_heap_size();
}

void AsyncWork(uv_work_t* req) {
    // No HandleScope!
}

void AsyncAfter(uv_work_t* req) {
    HandleScope scope;
    // Baton* baton = static_cast<Baton*>(req->data);
    Baton* baton = &nodegc::baton;

    if (baton->error_code) {
        Local<Value> err = Exception::Error(
            String::New(baton->error_message.c_str()));
        Local<Value> argv[] = { err };

        TryCatch try_catch;
        baton->callback->Call(
            Context::GetCurrent()->Global(), 1, argv);

        if (try_catch.HasCaught()) {
            node::FatalException(try_catch);
        }
    } else {
        Local<Value> argv[] = {
          Local<Value>::New(String::New((baton->gctype == kGCTypeScavenge) ? "scavenge" : "marksweep")),
          Local<Value>::New(Number::New(currentTimeMillis() - baton->timeBefore)),
          Local<Value>::New(Number::New(baton->heapSizeBefore)),
          Local<Value>::New(Number::New(heapSize()))
        };
        baton->callback->Call(Context::GetCurrent()->Global(), 4, argv);
    }
}

void GCPrologueCb(v8::GCType type, v8::GCCallbackFlags flags)
{
    nodegc::baton.heapSizeBefore = heapSize();
    nodegc::baton.timeBefore = currentTimeMillis();
    nodegc::baton.gctype = type;
}

void GCEpilogueCb(v8::GCType type, v8::GCCallbackFlags flags)
{
  uv_queue_work(uv_default_loop(), &nodegc::baton.request, AsyncWork, AsyncAfter);
}

Handle<Value> RunCallback(const Arguments& args) {
  HandleScope scope;

  if (!args[0]->IsFunction()) {
      return ThrowException(Exception::TypeError(
          String::New("Callback function required")));
  }
  Local<Function> callback = Local<Function>::Cast(args[0]);

  Baton* baton = new Baton();
  baton->request.data = baton;
  baton->callback = Persistent<Function>::New(callback);

  nodegc::baton = *baton;

  std::cout << "set cb\n";
  V8::AddGCPrologueCallback(GCPrologueCb, kGCTypeAll);
  V8::AddGCEpilogueCallback(GCEpilogueCb, kGCTypeAll);

  return scope.Close(Undefined());
}

void Init(Handle<Object> target) {
  target->Set(String::NewSymbol("gcCallback"),
      FunctionTemplate::New(RunCallback)->GetFunction());
}

NODE_MODULE(nodegc, Init)
