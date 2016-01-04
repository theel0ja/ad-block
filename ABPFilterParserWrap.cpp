/* Copyright (c) 2015 Brian R. Bondy. Distributed under the MPL2 license.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "ABPFilterParserWrap.h"

namespace ABPFilterParserWrap {

using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Int32;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Boolean;
using v8::Value;

Persistent<Function> ABPFilterParserWrap::constructor;

ABPFilterParserWrap::ABPFilterParserWrap() {
}

ABPFilterParserWrap::~ABPFilterParserWrap() {
}

void ABPFilterParserWrap::Init(Local<Object> exports) {
  Isolate* isolate = exports->GetIsolate();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "ABPFilterParser"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "parse", ABPFilterParserWrap::Parse);
  NODE_SET_PROTOTYPE_METHOD(tpl, "matches", ABPFilterParserWrap::Matches);

  Local<Object> filterOptions = Object::New(isolate);
  filterOptions->Set(String::NewFromUtf8(isolate, "noFilterOption"),
    Int32::New(isolate, 0));
  filterOptions->Set(String::NewFromUtf8(isolate, "script"),
    Int32::New(isolate, 01));
  filterOptions->Set(String::NewFromUtf8(isolate, "image"),
    Int32::New(isolate, 02));
  filterOptions->Set(String::NewFromUtf8(isolate, "stylesheet"),
    Int32::New(isolate, 04));
  filterOptions->Set(String::NewFromUtf8(isolate, "object"),
    Int32::New(isolate, 010));
  filterOptions->Set(String::NewFromUtf8(isolate, "xmlHttpRequest"),
    Int32::New(isolate, 020));
  filterOptions->Set(String::NewFromUtf8(isolate, "objectSubrequest"),
    Int32::New(isolate, 040));
  filterOptions->Set(String::NewFromUtf8(isolate, "subdocument"),
    Int32::New(isolate, 0100));
  filterOptions->Set(String::NewFromUtf8(isolate, "document"),
    Int32::New(isolate, 0200));
  filterOptions->Set(String::NewFromUtf8(isolate, "other"),
    Int32::New(isolate, 0400));
  filterOptions->Set(String::NewFromUtf8(isolate, "xBL"),
    Int32::New(isolate, 01000));
  filterOptions->Set(String::NewFromUtf8(isolate, "collapse"),
    Int32::New(isolate, 02000));
  filterOptions->Set(String::NewFromUtf8(isolate, "doNotTrack"),
    Int32::New(isolate, 04000));
  filterOptions->Set(String::NewFromUtf8(isolate, "elemHide"),
    Int32::New(isolate, 010000));
  filterOptions->Set(String::NewFromUtf8(isolate, "thirdParty"),
    Int32::New(isolate, 020000));
  filterOptions->Set(String::NewFromUtf8(isolate, "notThirdParty"),
    Int32::New(isolate, 040000));

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "ABPFilterParser"),
               tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "FilterOptions"), filterOptions);
}

void ABPFilterParserWrap::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new ABPFilterParser(...)`
    ABPFilterParserWrap* obj = new ABPFilterParserWrap();
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  } else {
    // Invoked as plain function `ABPFilterParser(...)`,
    // turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    args.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void ABPFilterParserWrap::Parse(const FunctionCallbackInfo<Value>& args) {
  String::Utf8Value str(args[0]->ToString());
  const char * buffer = *str;

  ABPFilterParserWrap* obj =
    ObjectWrap::Unwrap<ABPFilterParserWrap>(args.Holder());
  obj->parse(buffer);
}

void ABPFilterParserWrap::Matches(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  String::Utf8Value str(args[0]->ToString());
  const char * buffer = *str;
  int32_t filterOption = static_cast<FilterOption>(args[1]->Int32Value());
  String::Utf8Value currentPageDomain(args[2]->ToString());
  const char * currentPageDomainBuffer = *currentPageDomain;

  ABPFilterParserWrap* obj =
    ObjectWrap::Unwrap<ABPFilterParserWrap>(args.Holder());
  bool matches = obj->matches(buffer,
    static_cast<FilterOption>(filterOption),
    currentPageDomainBuffer);

  args.GetReturnValue().Set(Boolean::New(isolate, matches));
}


}  // namespace ABPFilterParserWrap
