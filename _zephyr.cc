/* -*- c-basic-offset: 2 -*- */
#include <node.h>
#include <node_events.h>

#include <errno.h>
#include <stdlib.h>
extern "C" {
#include <zephyr/zephyr.h>
};

using namespace v8;
using namespace node;

static Persistent<String> init_symbol;
static Persistent<String> error_symbol;

class Connection : public EventEmitter {
public:
  static void Initialize(Handle<Object> target)
  {
    HandleScope scope;

    Local<FunctionTemplate> t = FunctionTemplate::New(New);

    init_symbol = NODE_PSYMBOL("init");
    error_symbol = NODE_PSYMBOL("zephyr_error");

    t->Inherit(EventEmitter::constructor_template);
    t->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(t, "init", Init);

    target->Set(String::NewSymbol("Connection"), t->GetFunction());
    NODE_SET_METHOD(target, "error_message", ErrorMessage);
  }

  static Handle<Value>
  Init (const Arguments& args)
  {
    Connection *connection = ObjectWrap::Unwrap<Connection>(args.This());
    connection->Init();
    return Undefined();
  }

  static Handle<Value>
  ErrorMessage (const Arguments& args)
  {
    HandleScope scope;
    if (args.Length() == 0 || !args[0]->IsInt32()) {
      return ThrowException(Exception::Error(
            String::New("ErrorString must be given a number.")));
    }

    return scope.Close(String::New(error_message(args[0]->Int32Value())));
  }

protected:
  static Handle<Value>
  New (const Arguments &args)
  {
    HandleScope scope;

    Connection *connection = new Connection();
    connection->Wrap(args.This());

    return args.This();
  }

  static int EioInit (eio_req *req) {
    req->result = ZInitialize();
    return 0;
  }

  static int AfterInit (eio_req *req) {
    Connection *conn = static_cast<Connection*>(req->data);
    conn->AfterInit(req->result);

    return 0;
  }

  void Init ( )
  {
    if (initialized_)
      return;
    initialized_ = true;

    eio_req *req = eio_custom(EioInit, EIO_PRI_DEFAULT, AfterInit, this);
    if (req == NULL) {
      Error(ENOMEM);
      return;
    }

    Ref();
  }

  void AfterInit (ssize_t result) {
    if (result == 0)
      Emit(init_symbol, 0, NULL);
    else
      Error(result);
    Unref();
  }

  Connection() : EventEmitter(), initialized_(false) {
  }

  void Error(int err) {
    HandleScope scope;
    Local<Value> error = Integer::New(err);
    Emit(error_symbol, 1, &error);
  }

  bool initialized_;
};

extern "C" void
init (Handle<Object> target)
{
  HandleScope scope;
  Connection::Initialize(target);
}
