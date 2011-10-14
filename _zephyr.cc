/* -*- c-basic-offset: 2 -*- */
#include <node.h>
#include <node_events.h>

using namespace v8;
using namespace node;


class Connection : public EventEmitter {
public:
  static void Initialize(Handle<Object> target)
  {
    HandleScope scope;

    Local<FunctionTemplate> t = FunctionTemplate::New(New);

    t->Inherit(EventEmitter::constructor_template);
    t->InstanceTemplate()->SetInternalFieldCount(1);

    target->Set(String::NewSymbol("Connection"), t->GetFunction());
  }

private:
  static Handle<Value>
  New (const Arguments &args)
  {
    HandleScope scope;

    Connection *connection = new Connection();
    connection->Wrap(args.This());

    return args.This();
  }

  Connection() : EventEmitter() {
  }
};

extern "C" void
init (Handle<Object> target)
{
  HandleScope scope;
  Connection::Initialize(target);
}
