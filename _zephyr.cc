#include <node.h>
#include <node_events.h>

using namespace v8;
using namespace node;

extern "C" void
init (Handle<Object> target)
{
  HandleScope scope;
  target->Set(String::New("hello"), String::New("world"));
}
